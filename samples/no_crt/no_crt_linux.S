.global _start
_start:
    mov (%rsp), %rdi
    lea 8(%rsp), %rsi
    call no_crt_main

    // exit()
    mov %rax, %rdi
    mov $60, %rax
    syscall

.global nc_mmap
nc_mmap:
    mov %rcx, %r10
    mov $9, %rax
    syscall
    ret

.global nc_mprotect
nc_mprotect:
    mov $10, %rax
    syscall
    ret

.global nc_madvise
nc_madvise:
    mov $28, %rax
    syscall
    ret

.global nc_munmap
nc_munmap:
    mov $11, %rax
    syscall
    ret

.global nc_futex
nc_futex:
    mov %rcx, %r10
    mov $202, %rax
    syscall
    ret

.global nc_write
nc_write:
    mov $1, %rax
    syscall
    ret

.global nc_open
nc_open:
    mov $2, %rax
    syscall
    ret

.global nc_lseek
nc_lseek:
    mov $8, %rax
    syscall
    ret

.global nc_read
nc_read:
    mov $0, %rax
    syscall
    ret

.global nc_close
nc_close:
    mov $3, %rax
    syscall
    ret