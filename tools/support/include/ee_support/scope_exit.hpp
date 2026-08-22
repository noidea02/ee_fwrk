#pragma once

#include <concepts>
#include <type_traits>
#include <utility>

namespace ee_supp {

    template<std::invocable F>
    class [[nodiscard]] scope_exit final {

    public:

        template<typename Fn>
            requires (!std::same_as<std::remove_cvref_t<Fn>, scope_exit>) && std::constructible_from<F, Fn>
        explicit scope_exit(Fn&& f) : f{ std::forward<Fn>(f) } {}
        
        ~scope_exit() { f(); }

        scope_exit(const scope_exit&) = delete;
        scope_exit(scope_exit&&) = delete;
        scope_exit& operator=(const scope_exit&) = delete;
        scope_exit& operator=(scope_exit&&) = delete;

    private:

        F f;
    };

    template<typename Fn>
    scope_exit(Fn) -> scope_exit<std::decay_t<Fn>>;
}