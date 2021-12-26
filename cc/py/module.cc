#include "py/module.hh"

// ======================================================================

PYBIND11_MODULE(ae_whocc, mdl)
{
    using namespace pybind11::literals;

    mdl.doc() = "AE WHOCC backend";

    // ----------------------------------------------------------------------

    mdl.def("open_xlsx", [](pybind11::object filename) {}, "filename"_a);
}

// ======================================================================
