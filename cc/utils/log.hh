#pragma once

#include <vector>

#include "ext/fmt.hh"

// ----------------------------------------------------------------------

namespace ae
{
    enum class debug { no, yes };
    enum class verbose { no, yes };

    template <typename Int> constexpr auto number_of_decimal_digits(Int max_value) { return static_cast<int>(std::log10(max_value)) + 1; }

    // constexpr inline debug debug_from(bool verb) { return verb ? debug::yes : debug::no; }
    constexpr inline verbose verbose_from(bool verb) { return verb ? verbose::yes : verbose::no; }

    // ----------------------------------------------------------------------

    namespace log::inline v1
    {
        // https://clang.llvm.org/docs/LanguageExtensions.html#source-location-builtins
        struct source_location
        {
            const char* file{__builtin_FILE()};
            int line{__builtin_LINE()};
            const char* function{__builtin_FUNCTION()};
        };

        constexpr const source_location no_source_location { .file = nullptr, .line = 0, .function = nullptr };

        // ----------------------------------------------------------------------

        namespace prefix
        {
            constexpr const std::string_view error{"> ERROR "};
            constexpr const std::string_view warning{">> WARNING "};
            constexpr const std::string_view info{">>> "};
            constexpr const std::string_view debug{">>>> "};
            constexpr const std::string_view none{};

        } // namespace prefix

        // ----------------------------------------------------------------------

        template <typename... Ts> inline std::string format(const source_location& sl, fmt::format_string<Ts...> format, Ts&&... ts)
        {
            try {
                return fmt::format(format, std::forward<Ts>(ts)...);
            }
            catch (fmt::format_error& err) {
                fmt::print(stderr, "> fmt::format_error ({}) format: \"{}\"{}", err.what(), format, sl);
                throw;
            }
        }

        template <typename... Ts> inline void print(const source_location& sl, bool do_print, std::string_view prefix, fmt::format_string<Ts...> format, Ts&&... ts)
        {
            if (do_print)
                fmt::print(stderr, "{}{}{}\n", prefix, log::format(sl, format, std::forward<Ts>(ts)...), sl);
        }

        template <typename MesssageGetter> requires std::is_invocable_v<MesssageGetter> inline void print(const source_location& sl, bool do_print, std::string_view prefix, MesssageGetter get_message)
        {
            if (do_print)
                fmt::print(stderr, "{}{}{}\n", prefix, get_message(), sl);
        }

        template <typename... Ts> inline void ad_assert(bool condition, const source_location& sl, fmt::format_string<Ts...> format, Ts&&... ts)
        {
            if (!condition) {
                print(sl, true, "> ASSERTION FAILED", format, std::forward<Ts>(ts)...);
                std::abort();
            }
        }

        // ----------------------------------------------------------------------

    } // namespace log::inline v1

} // namespace ae

// ----------------------------------------------------------------------

template <> struct fmt::formatter<ae::log::source_location> : fmt::formatter<ae::fmt_helper::default_formatter> {
    template <typename FormatCtx> auto format(const ae::log::source_location& sl, FormatCtx& ctx)
    {
        if (sl.file)
            return format_to(ctx.out(), " @@ {}:{}", sl.file, sl.line);
        else
            return ctx.out();
    }
};

// ======================================================================

// https://www.cppstories.com/2021/non-terminal-variadic-args/

template <typename... Ts> struct AD_ERROR
{
    AD_ERROR(fmt::format_string<Ts...> format, Ts&&... ts, const ae::log::source_location& sl = ae::log::source_location{})
    {
        ae::log::print(sl, true, ae::log::prefix::error, format, std::forward<Ts>(ts)...);
    }
};

template <typename... Ts> AD_ERROR(fmt::format_string<Ts...>, Ts&&...) -> AD_ERROR<Ts...>;

// ----------------------------------------------------------------------

template <typename... Ts> struct AD_WARNING
{
    AD_WARNING(fmt::format_string<Ts...> format, Ts&&... ts, const ae::log::source_location& sl = ae::log::source_location{})
    {
        ae::log::print(sl, true, ae::log::prefix::warning, format, std::forward<Ts>(ts)...);
    }
    AD_WARNING(bool do_print, fmt::format_string<Ts...> format, Ts&&... ts, const ae::log::source_location& sl = ae::log::source_location{})
    {
        ae::log::print(sl, do_print, ae::log::prefix::warning, format, std::forward<Ts>(ts)...);
    }
};

template <typename... Ts> AD_WARNING(const char*, Ts&&...) -> AD_WARNING<Ts...>;
template <typename... Ts> AD_WARNING(fmt::format_string<Ts...>, Ts&&...) -> AD_WARNING<Ts...>;
template <typename... Ts> AD_WARNING(bool, fmt::format_string<Ts...>, Ts&&...) -> AD_WARNING<Ts...>;

// ----------------------------------------------------------------------

template <typename... Ts> struct AD_INFO
{
    AD_INFO(fmt::format_string<Ts...> format, Ts&&... ts, const ae::log::source_location& sl = ae::log::source_location{})
    {
        ae::log::print(sl, true, ae::log::prefix::info, format, std::forward<Ts>(ts)...);
    }
    AD_INFO(bool do_print, fmt::format_string<Ts...> format, Ts&&... ts, const ae::log::source_location& sl = ae::log::source_location{})
    {
        ae::log::print(sl, do_print, ae::log::prefix::info, format, std::forward<Ts>(ts)...);
    }
    AD_INFO(ae::verbose do_print, fmt::format_string<Ts...> format, Ts&&... ts, const ae::log::source_location& sl = ae::log::source_location{})
    {
        ae::log::print(sl, do_print == ae::verbose::yes, ae::log::prefix::info, format, std::forward<Ts>(ts)...);
    }
    AD_INFO(ae::debug do_print, fmt::format_string<Ts...> format, Ts&&... ts, const ae::log::source_location& sl = ae::log::source_location{})
    {
        ae::log::print(sl, do_print == ae::debug::yes, ae::log::prefix::info, format, std::forward<Ts>(ts)...);
    }
};

template <typename... Ts> AD_INFO(const char*, Ts&&...) -> AD_INFO<Ts...>;
template <typename... Ts> AD_INFO(fmt::format_string<Ts...>, Ts&&...) -> AD_INFO<Ts...>;
template <typename... Ts> AD_INFO(bool, fmt::format_string<Ts...>, Ts&&...) -> AD_INFO<Ts...>;
template <typename... Ts> AD_INFO(ae::verbose, fmt::format_string<Ts...>, Ts&&...) -> AD_INFO<Ts...>;
template <typename... Ts> AD_INFO(ae::debug, fmt::format_string<Ts...>, Ts&&...) -> AD_INFO<Ts...>;

// ----------------------------------------------------------------------

template <typename... Ts> struct AD_DEBUG
{
    AD_DEBUG(fmt::format_string<Ts...> format, Ts&&... ts, const ae::log::source_location& sl = ae::log::source_location{})
    {
        ae::log::print(sl, true, ae::log::prefix::debug, format, std::forward<Ts>(ts)...);
    }
    AD_DEBUG(bool do_print, fmt::format_string<Ts...> format, Ts&&... ts, const ae::log::source_location& sl = ae::log::source_location{})
    {
        ae::log::print(sl, do_print, ae::log::prefix::debug, format, std::forward<Ts>(ts)...);
    }
    AD_DEBUG(ae::verbose dbg, fmt::format_string<Ts...> format, Ts&&... ts, const ae::log::source_location& sl = ae::log::source_location{})
    {
        ae::log::print(sl, dbg == ae::verbose::yes, ae::log::prefix::debug, format, std::forward<Ts>(ts)...);
    }
    AD_DEBUG(ae::debug dbg, fmt::format_string<Ts...> format, Ts&&... ts, const ae::log::source_location& sl = ae::log::source_location{})
    {
        ae::log::print(sl, dbg == ae::debug::yes, ae::log::prefix::debug, format, std::forward<Ts>(ts)...);
    }
};

template <typename... Ts> AD_DEBUG(const char*, Ts&&...) -> AD_DEBUG<Ts...>;
template <typename... Ts> AD_DEBUG(fmt::format_string<Ts...>, Ts&&...) -> AD_DEBUG<Ts...>;
template <typename... Ts> AD_DEBUG(bool, fmt::format_string<Ts...>, Ts&&...) -> AD_DEBUG<Ts...>;
template <typename... Ts> AD_DEBUG(ae::verbose, fmt::format_string<Ts...>, Ts&&...) -> AD_DEBUG<Ts...>;
template <typename... Ts> AD_DEBUG(ae::debug, fmt::format_string<Ts...>, Ts&&...) -> AD_DEBUG<Ts...>;

// ----------------------------------------------------------------------

template <typename... Ts> struct AD_ASSERT
{
    AD_ASSERT(bool condition, fmt::format_string<Ts...> format, Ts&&... ts, const ae::log::source_location& sl = ae::log::source_location{})
    {
        ae::log::ad_assert(condition, sl, format, std::forward<Ts>(ts)...);
    }
};

template <typename... Ts> AD_ASSERT(bool, fmt::format_string<Ts...>, Ts&&...) -> AD_ASSERT<Ts...>;

// ----------------------------------------------------------------------

template <typename... Ts> struct AD_FORMAT
{
    AD_FORMAT(fmt::format_string<Ts...> format, Ts&&... ts, const ae::log::source_location& sl = ae::log::source_location{})
        : text{fmt::format("{} {}", ae::log::format(sl, format, std::forward<Ts>(ts)...), sl)}
    {
    }
    operator std::string() const { return text; }
    std::string text;
};

template <typename... Ts> AD_FORMAT(fmt::format_string<Ts...>, Ts&&...) -> AD_FORMAT<Ts...>;

template <typename... Ts> struct fmt::formatter<AD_FORMAT<Ts...>> : fmt::formatter<ae::fmt_helper::default_formatter> {
    template <typename FormatCtx> auto format(const AD_FORMAT<Ts...>& value, FormatCtx& ctx) const
    {
        return format_to(ctx.out(), "{}", value.text);
    }
};


// ----------------------------------------------------------------------

template <typename... Ts> inline void AD_PRINT_IF(bool do_print, fmt::format_string<Ts...> format, Ts&&... ts)
{
    ae::log::print(ae::log::no_source_location, do_print, ae::log::prefix::none, format, std::forward<Ts>(ts)...);
}

template <typename MesssageGetter> requires std::is_invocable_v<MesssageGetter> inline void AD_PRINT_L(bool do_print, MesssageGetter get_message)
{
    ae::log::print(ae::log::no_source_location, do_print, ae::log::prefix::none, get_message);
}

template <typename... Ts> inline void AD_PRINT_IF(ae::verbose verb, fmt::format_string<Ts...> format, Ts&&... ts)
{
    ae::log::print(ae::log::no_source_location, verb == ae::verbose::yes, ae::log::prefix::none, format, std::forward<Ts>(ts)...);
}

template <typename... Ts> inline void AD_PRINT_IF(ae::debug dbg, fmt::format_string<Ts...> format, Ts&&... ts)
{
    ae::log::print(ae::log::no_source_location, dbg == ae::debug::yes, ae::log::prefix::none, format, std::forward<Ts>(ts)...);
}

template <typename... Ts> inline void AD_PRINT(fmt::format_string<Ts...> format, Ts&&... ts)
{
    ae::log::print(ae::log::no_source_location, true, ae::log::prefix::none, format, std::forward<Ts>(ts)...);
}

inline void AD_PRINT_NEWLINE()
{
    ae::log::print(ae::log::no_source_location, true, ae::log::prefix::none, "\n");
}

// ----------------------------------------------------------------------
