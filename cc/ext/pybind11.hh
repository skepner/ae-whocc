#pragma once

#include "ext/fmt.hh"

// ----------------------------------------------------------------------

#pragma GCC diagnostic push

#if defined(__clang__)

#pragma GCC diagnostic ignored "-Wcovered-switch-default"
#pragma GCC diagnostic ignored "-Wdocumentation"
#pragma GCC diagnostic ignored "-Wexit-time-destructors"
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wreserved-identifier"
#pragma GCC diagnostic ignored "-Wreserved-macro-identifier"
#pragma GCC diagnostic ignored "-Wshadow-field"
#pragma GCC diagnostic ignored "-Wshadow-field-in-constructor"
#pragma GCC diagnostic ignored "-Wshadow-uncaptured-local"
#pragma GCC diagnostic ignored "-Wundefined-reinterpret-cast"
// #pragma GCC diagnostic ignored "-Wcast-align"
// #pragma GCC diagnostic ignored "-Wdocumentation-unknown-command"
// #pragma GCC diagnostic ignored "-Wextra-semi"
// #pragma GCC diagnostic ignored "-Wfloat-equal"
// #pragma GCC diagnostic ignored "-Wpadded"
// #pragma GCC diagnostic ignored "-Wreserved-id-macro"
// #pragma GCC diagnostic ignored "-Wweak-vtables"
// #pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"

#elif defined(__GNUG__)

// #pragma GCC diagnostic ignored "-Weffc++"

#endif

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/embed.h>
#include <pybind11/functional.h>

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------

// template <> struct fmt::formatter<pybind11::object> : fmt::formatter<ae::fmt_helper::default_formatter> {
//     template <typename FormatCtx> auto format(const py::object& value, FormatCtx& ctx) const
//     {
//         return format_to(ctx.out(), "{}", py::repr(value).cast<std::string>());
//     }
// };

template <> struct fmt::formatter<pybind11::object> : fmt::formatter<std::string> {
    template <typename FormatCtx> auto format(const pybind11::object& value, FormatCtx& ctx) const
    {
        return fmt::formatter<std::string>::format(pybind11::repr(value).cast<std::string>(), ctx);
    }
};

// ----------------------------------------------------------------------
