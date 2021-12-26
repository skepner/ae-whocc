#include "py/module.hh"
#include "xlsx/xlsx.hh"

// ======================================================================

PYBIND11_MODULE(ae_whocc, mdl)
{
    using namespace pybind11::literals;

    mdl.doc() = "AE WHOCC backend";

    // ----------------------------------------------------------------------

    mdl.def(
        "open_xlsx", [](pybind11::object filename) { return ae::xlsx::open(static_cast<std::string>(pybind11::str(filename))); }, "filename"_a);

    pybind11::class_<ae::xlsx::Doc, std::shared_ptr<ae::xlsx::Doc>>(mdl, "XlsxDoc") //
        .def("number_of_sheets", &ae::xlsx::Doc::number_of_sheets)                  //
        .def("sheet", &ae::xlsx::Doc::sheet, "sheet_no"_a)                          //
        ;

    pybind11::class_<ae::sheet::Sheet, std::shared_ptr<ae::sheet::Sheet>>(mdl, "Sheet")                     //
        .def("name", &ae::sheet::Sheet::name)                                                               //
        .def("number_of_rows", [](const ae::sheet::Sheet& sheet) { return *sheet.number_of_rows(); })       //
        .def("number_of_columns", [](const ae::sheet::Sheet& sheet) { return *sheet.number_of_columns(); }) //
        ;
}

// ======================================================================
