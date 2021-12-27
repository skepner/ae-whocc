#include "py/module.hh"
#include "xlsx/xlsx.hh"
#include "xlsx/sheet-extractor.hh"

// ======================================================================

PYBIND11_MODULE(ae_whocc, mdl)
{
    using namespace pybind11::literals;

    mdl.doc() = "AE WHOCC backend";

    // ----------------------------------------------------------------------

    auto xlsx_submodule = mdl.def_submodule("xlsx", "xlsx access");

    xlsx_submodule.def(
        "open", [](pybind11::object filename) { return ae::xlsx::open(static_cast<std::string>(pybind11::str(filename))); }, "filename"_a);

    xlsx_submodule.def(
        "extractor",
        [](std::shared_ptr<ae::xlsx::Sheet> sheet, bool winf) { return extractor_factory(sheet, winf ? ae::xlsx::Extractor::warn_if_not_found::yes : ae::xlsx::Extractor::warn_if_not_found::no); },
        "sheet"_a, "warn_if_not_found"_a = true);

    pybind11::class_<ae::xlsx::Doc, std::shared_ptr<ae::xlsx::Doc>>(xlsx_submodule, "Doc") //
        .def("number_of_sheets", &ae::xlsx::Doc::number_of_sheets)                         //
        .def("sheet", &ae::xlsx::Doc::sheet, "sheet_no"_a)                                 //
        ;

    pybind11::class_<ae::xlsx::Sheet, std::shared_ptr<ae::xlsx::Sheet>>(xlsx_submodule, "Sheet")           //
        .def("name", &ae::xlsx::Sheet::name)                                                                      //
        .def("number_of_rows", [](const ae::xlsx::Sheet& sheet) { return *sheet.number_of_rows(); })       //
        .def("number_of_columns", [](const ae::xlsx::Sheet& sheet) { return *sheet.number_of_columns(); }) //
        ;
}

// ======================================================================
