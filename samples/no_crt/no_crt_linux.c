#include "no_crt.h"

/* Standard Linux Constants
*/

#define STDOUT_FD 1

#define FUTEX_WAIT_PRIVATE 128
#define FUTEX_WAKE_PRIVATE 129

#define MUTEX_STATE_UNLOCKED 0
#define MUTEX_STATE_LOCKED_NO_WAITERS 1
#define MUTEX_STATE_LOCKED_HAS_WAITERS 2

#define PROT_NONE 0x00
#define PROT_READ 0x01
#define PROT_WRITE 0x02

#define MMAP_ERROR_RANGE_BEGIN ((ee_size_t)-4095)

#define MAP_PRIVATE 0x02
#define MAP_ANONYMOUS 0x20
#define MAP_NORESERVE 0x4000

#define MADV_NORMAL 0x00
#define MADV_DONTNEED 0x04
#define MADV_FREE 0x08

#define O_RDONLY 0

#define SEEK_SET 0
#define SEEK_END 2

/* Custom Constants
*/

#define PAGE_BOUNDARY 4096
#define HEAP_SIZE ((ee_size_t)16 * 1024 * 1024 * 1024)

#define HEAP_FLAGS_MASK ((ee_size_t)0x0F)
#define HEAP_PREVIOUS_HEADER_MASK (~HEAP_FLAGS_MASK)

#define HEAP_FLAG_NONE 0
#define HEAP_FLAG_ALLOCATED ((ee_size_t)1 << 0)

/* Types
*/

typedef struct {

    ee_uint32_t mtx;
    void* ptr;
    ee_size_t size;
    void* peak_ptr;

} heap_state_t;

typedef struct {

    ee_size_t size;
    ee_size_t flags;

} heap_region_header_t;

EE_STATIC_ASSERT(HEAP_SIZE > sizeof(heap_region_header_t), heap_size_must_be_bigger_than_region_header_size);

#ifdef EE_TARGET_64BIT
    #define MEMORY_ALIGNMENT 16
#elif EE_TARGET_32BIT
    #define MEMORY_ALIGNMENT 8
#else
    #error Unknown target platform.
#endif

EE_STATIC_ASSERT(MEMORY_ALIGNMENT == sizeof(heap_region_header_t), heap_region_header_size_must_match_memory_alignment);

/* Function / syscall Declarations
*/

void* nc_mmap(void* addr, ee_size_t length, int prot, int flags, int fd, int offset);
int nc_mprotect(void* addr, ee_size_t size, int prot);
int nc_madvise(void* addr, ee_size_t size, int advice);
int nc_munmap(void* addr, ee_size_t length);
long nc_futex(ee_uint32_t* uaddr, int op, ee_uint32_t val, void* ign0, ee_uint32_t* ign1, ee_uint32_t ign2);
long nc_write(int fd, const void* buf, unsigned long count);
int nc_open(const char* path, int flags);
int nc_lseek(int fd, int offset, int whence);
ee_size_t nc_read(int fd, void* buf, ee_size_t count);
int nc_close(int fd);

int wrapped_main(int argc, char** argv);

/* Global State
*/

static heap_state_t heap_state;

/* Private Implementation
*/

/* Mutex
*/

static void mutex_lock(ee_uint32_t* mtx) {

    /* Based on "Futexes Are Tricky".
    */

    ee_uint32_t expected = MUTEX_STATE_UNLOCKED;
    ee_uint32_t old = 0;

    if (__atomic_compare_exchange_n(mtx, &expected, MUTEX_STATE_LOCKED_NO_WAITERS, EE_FALSE, __ATOMIC_ACQUIRE, __ATOMIC_RELAXED))
        return;

    old = expected;
    if (old != MUTEX_STATE_LOCKED_HAS_WAITERS)
        old = __atomic_exchange_n(mtx, MUTEX_STATE_LOCKED_HAS_WAITERS, __ATOMIC_ACQUIRE);

    while (old != MUTEX_STATE_UNLOCKED) {

        const long futex_result = nc_futex(mtx, FUTEX_WAIT_PRIVATE, MUTEX_STATE_LOCKED_HAS_WAITERS, 0, 0, 0);
        if (futex_result)
            nc_trap();
        
        old = __atomic_exchange_n(mtx, MUTEX_STATE_LOCKED_HAS_WAITERS, __ATOMIC_ACQUIRE);
    }
}

static void mutex_unlock(ee_uint32_t* mtx) {

    const ee_uint32_t old = __atomic_exchange_n(mtx, MUTEX_STATE_UNLOCKED, __ATOMIC_RELEASE);
    if (old == MUTEX_STATE_LOCKED_HAS_WAITERS) {
        
        const long futex_result = nc_futex(mtx, FUTEX_WAKE_PRIVATE, 1, 0, 0, 0);
        if (futex_result < 0)
            nc_trap();
    }
}

/* Heap

   WARNING: Heap implementation has NOT been subjected to QA procedures. Usage outside of the sample projects of ee_fwrk is discouraged.
*/

static ee_bool_t heap_is_address_aligned(const void* ptr) {
    return !((ee_size_t)ptr & 0x0F);
}

static ee_bool_t heap_is_page_boundary(const void* ptr) {
    return !((ee_size_t)ptr & 0x0FFF);
}

static void* heap_get_next_aligned_address(const void* ptr) {
    return (void*)(((ee_size_t)ptr + (MEMORY_ALIGNMENT - 1)) & ~(MEMORY_ALIGNMENT - 1));
}

static void* heap_get_page_boundary_low(const void* ptr) {
    return (void*)((ee_size_t)ptr & ~0xFFF);
}

static void* heap_get_next_page_boundary(const void* ptr) {
    return (void*)(((ee_size_t)ptr + (PAGE_BOUNDARY - 1)) & ~(PAGE_BOUNDARY - 1));
}

static void heap_commit_page(void* addr, ee_size_t size) {

    void* const page_addr = heap_get_page_boundary_low(addr);
    const ee_size_t upper_boundary = (ee_size_t)addr + size;
    const ee_size_t total_size = upper_boundary - (ee_size_t)page_addr;
    
    const int mprotect_result = nc_mprotect(page_addr, total_size, PROT_READ | PROT_WRITE);
    if (mprotect_result)
        nc_trap();
}

static void heap_free_page(void* addr, ee_size_t size) {

    const void* const page_end = (ee_byte_t*)addr + size;
    int madvise_result = 0;

    if (!heap_is_page_boundary(addr) || !heap_is_page_boundary(page_end))
        nc_trap();

    madvise_result = nc_madvise(addr, size, MADV_DONTNEED);
    if (madvise_result)
        nc_trap();
}

static void heap_create() {

    heap_region_header_t* root_hdr = 0;

    heap_state.mtx = MUTEX_STATE_UNLOCKED;
    heap_state.size = HEAP_SIZE;

    heap_state.ptr = nc_mmap(0, heap_state.size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
    if ((ee_size_t)heap_state.ptr >= MMAP_ERROR_RANGE_BEGIN)
        nc_trap();

    heap_commit_page(heap_state.ptr, sizeof(heap_region_header_t));

    root_hdr = (heap_region_header_t*)heap_state.ptr;

    root_hdr->size = heap_state.size - sizeof(heap_region_header_t);
    root_hdr->flags = HEAP_FLAG_NONE;
}

static void heap_destroy() {
    
    const int result = nc_munmap(heap_state.ptr, heap_state.size);
    if (result)
        nc_trap();
}

static heap_region_header_t* heap_locked_find_free_region(ee_size_t size) {

    ee_size_t heap_addr = (ee_size_t)heap_state.ptr;
    const ee_size_t heap_end = heap_addr + heap_state.size;

    while (heap_addr <= heap_end - sizeof(heap_region_header_t)) {

        heap_region_header_t* const hdr = (heap_region_header_t*)heap_addr;
        if (hdr->size >= size && !(hdr->flags & HEAP_FLAG_ALLOCATED))
            return hdr;

        heap_addr += sizeof(heap_region_header_t) + hdr->size;
        heap_addr = (ee_size_t)heap_get_next_aligned_address((void*)heap_addr);
    }

    return 0;
}

static void heap_locked_commit_next_region_header(const heap_region_header_t* region, ee_size_t new_data_size) {

    const ee_size_t region_end = (ee_size_t)region + sizeof(heap_region_header_t) + region->size;
    ee_size_t next_hdr_addr = 0;
    heap_region_header_t* next_hdr = 0;

    next_hdr_addr = (ee_size_t)region + sizeof(heap_region_header_t) + new_data_size;
    next_hdr_addr = (ee_size_t)heap_get_next_aligned_address((void*)next_hdr_addr);

    if (next_hdr_addr + sizeof(heap_region_header_t) > region_end)
        return;

    next_hdr = (heap_region_header_t*)next_hdr_addr;
    heap_commit_page(next_hdr, sizeof(heap_region_header_t));

    if (!heap_is_address_aligned(region))
        nc_trap();

    next_hdr->size = region_end - (next_hdr_addr + sizeof(heap_region_header_t));
    next_hdr->flags = (ee_size_t)region;
}

static void heap_locked_commit_region(heap_region_header_t* hdr, ee_size_t data_size) {

    if (data_size > hdr->size)
        nc_trap();

    heap_commit_page(hdr + sizeof(heap_region_header_t), data_size);

    hdr->size = data_size;
    hdr->flags |= HEAP_FLAG_ALLOCATED;
}

static heap_region_header_t* heap_locked_get_region_header(const void* data_begin) {

    const ee_size_t addr = (ee_size_t)data_begin;
    ee_size_t hdr_addr = 0;

    if (addr < sizeof(heap_region_header_t))
        nc_trap();

    hdr_addr = addr - sizeof(heap_region_header_t);
    if (hdr_addr % sizeof(heap_region_header_t))
        nc_trap();

    return (heap_region_header_t*)hdr_addr;
}

static void heap_locked_coalesce_free_regions(heap_region_header_t** start_ref) {

    heap_region_header_t* const start_region = *start_ref;
    heap_region_header_t* end_region = start_region;

    const ee_size_t heap_start = (ee_size_t)heap_state.ptr;
    const ee_size_t heap_end = heap_start + heap_state.size;

    ee_size_t region_start = 0;
    ee_size_t region_end = 0;

    region_start = (ee_size_t)start_region + sizeof(heap_region_header_t) + start_region->size;
    region_start = (ee_size_t)heap_get_next_aligned_address((void*)region_start);

    if (!heap_is_address_aligned(start_region))
        nc_trap();

    /* Step 1: Merge start with subsequent unallocated regions. */
    while (region_start <= heap_end - sizeof(heap_region_header_t)) {

        heap_region_header_t* const hdr = (heap_region_header_t*)region_start;
        
        if (hdr->flags & HEAP_FLAG_ALLOCATED) {
            
            const ee_size_t prev_hdr_addr = hdr->flags & HEAP_PREVIOUS_HEADER_MASK;
            if (prev_hdr_addr != (ee_size_t)start_region)
                hdr->flags = (ee_size_t)start_region | HEAP_FLAG_ALLOCATED;

            break;
        }

        region_start += sizeof(heap_region_header_t) + hdr->size;
        region_start = (ee_size_t)heap_get_next_aligned_address((void*)region_start);
    }

    start_region->size = region_start - ((ee_size_t)start_region + sizeof(heap_region_header_t));

    /* Step 2: Merge start with preceeding unallocated regions. */
    region_start = start_region->flags & HEAP_PREVIOUS_HEADER_MASK;
    region_end = (ee_size_t)start_region + sizeof(heap_region_header_t) + start_region->size;

    while (region_start >= heap_start) {

        heap_region_header_t* const hdr = (heap_region_header_t*)region_start;

        if (hdr->flags & HEAP_FLAG_ALLOCATED)
            break;

        end_region = hdr;
        end_region->size = region_end - ((ee_size_t)hdr + sizeof(heap_region_header_t));

        region_start = hdr->flags & HEAP_PREVIOUS_HEADER_MASK;
    }

    *start_ref = end_region;
}

/* Public Implementation
*/

NC_NO_RETURN void nc_trap(void) {
    __builtin_trap();
}

int no_crt_main(int argc, char** argv) {
    
    int exit_code = 0;
    heap_create();

    exit_code = wrapped_main(argc, argv);

    heap_destroy();
    return exit_code;
}

void* nc_calloc(ee_size_t num, ee_size_t size) {
    
    const ee_size_t total_size = num * size;
    heap_region_header_t* hdr = 0;
    void* result = 0;

    mutex_lock(&heap_state.mtx);

    hdr = heap_locked_find_free_region(total_size);
    if (!hdr)
        nc_trap();

    heap_locked_commit_next_region_header(hdr, total_size);
    heap_locked_commit_region(hdr, total_size);

    result = (ee_byte_t*)hdr + sizeof(heap_region_header_t);
    ee_memset(result, 0, total_size);

    if (result > heap_state.peak_ptr)
        heap_state.peak_ptr = result;

    mutex_unlock(&heap_state.mtx);
    return result;
}

void nc_free(void* ptr) {

    heap_region_header_t* hdr = 0;
    
    const void* region_end = 0;
    void* page_start = 0;
    ee_size_t freeable_region_size = 0;

    if (!ptr)
        return;

    mutex_lock(&heap_state.mtx);

    hdr = heap_locked_get_region_header(ptr);
    if (!hdr)
        nc_trap();

    hdr->flags &= ~HEAP_FLAG_ALLOCATED;

    heap_locked_coalesce_free_regions(&hdr);
    
    region_end = (ee_byte_t*)hdr + sizeof(heap_region_header_t) + hdr->size;
    region_end = heap_get_page_boundary_low(region_end);

    if (heap_is_page_boundary(hdr))
        page_start = hdr;
    else
        page_start = heap_get_next_page_boundary(hdr);

    if (page_start < region_end)
        freeable_region_size = (ee_size_t)region_end - (ee_size_t)page_start;

    if (freeable_region_size)
        heap_free_page(page_start, freeable_region_size);

    mutex_unlock(&heap_state.mtx);
}

void nc_print(const char* str) {
    nc_write(STDOUT_FD, str, ee_strlen(str));
}

void nc_println(const char* ln) {

    ee_char8_t actual_ln[1024] = { 0 };
    if (!ee_strapp(actual_ln, sizeof(actual_ln), ln) || !ee_strapp(actual_ln, sizeof(actual_ln), "\n"))
        nc_trap();

    nc_print(actual_ln);
}

ee_bool_t nc_read_file(const char* path, ee_byte_t** content, ee_size_t* size) {
    
    const int fd = nc_open(path, O_RDONLY);
    int lseek_result = 0;

    ee_byte_t* tmp_content = 0;
    ee_size_t tmp_file_size = 0;
    ee_bool_t result = EE_FALSE;

    if (fd == -1)
        return EE_FALSE;

    lseek_result = nc_lseek(fd, 0, SEEK_END);
    if (lseek_result < 0)
        goto cleanup;

    nc_lseek(fd, 0, SEEK_SET);

    tmp_file_size = (ee_size_t)lseek_result;
    tmp_content = nc_calloc(tmp_file_size, 1);
    
    if (nc_read(fd, tmp_content, tmp_file_size) != tmp_file_size)
        goto cleanup;

    *content = tmp_content;
    *size = tmp_file_size;
    result = EE_TRUE;

cleanup:

    nc_close(fd);

    if (!result && tmp_content)
        nc_free(tmp_content);

    return result;
}
