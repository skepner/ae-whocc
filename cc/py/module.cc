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
        .def("name", &ae::xlsx::Sheet::name)                                                               //
        .def("number_of_rows", [](const ae::xlsx::Sheet& sheet) { return *sheet.number_of_rows(); })       //
        .def("number_of_columns", [](const ae::xlsx::Sheet& sheet) { return *sheet.number_of_columns(); }) //
        .def(
            "cell_as_str", [](const ae::xlsx::Sheet& sheet, size_t row, size_t column) { return fmt::format("{}", sheet.cell(ae::xlsx::nrow_t{row}, ae::xlsx::ncol_t{column})); }, "row"_a,
            "column"_a) //
        .def(
            "grep",
            [](const ae::xlsx::Sheet& sheet, const std::string& rex, size_t min_row, size_t max_row, size_t min_col, size_t max_col) {
                if (max_row == ae::xlsx::max_row_col)
                    max_row = *sheet.number_of_rows();
                else
                    ++max_row;
                if (max_col == ae::xlsx::max_row_col)
                    max_col = *sheet.number_of_columns();
                else
                    ++max_col;
                return sheet.grep(std::regex(rex, std::regex::icase | std::regex::ECMAScript | std::regex::optimize), {ae::xlsx::nrow_t{min_row}, ae::xlsx::ncol_t{min_col}},
                                  {ae::xlsx::nrow_t{max_row}, ae::xlsx::ncol_t{max_col}});
            },                                                                                                                     //
            "regex"_a, "min_row"_a = 0, "max_row"_a = ae::xlsx::max_row_col, "min_col"_a = 0, "max_col"_a = ae::xlsx::max_row_col, //
            pybind11::doc("max_row and max_col are the last row and col to look in"))                                              //
        ;

    pybind11::class_<ae::xlsx::cell_match_t>(xlsx_submodule, "cell_match_t")                                                                   //
        .def_property_readonly("row", [](const ae::xlsx::cell_match_t& cm) { return *cm.row; })                                                //
        .def_property_readonly("col", [](const ae::xlsx::cell_match_t& cm) { return *cm.col; })                                                //
        .def_readonly("matches", &ae::xlsx::cell_match_t::matches)                                                                             //
        .def("__repr__", [](const ae::xlsx::cell_match_t& cm) { return fmt::format("<cell_match_t: {}:{} {}>", cm.row, cm.col, cm.matches); }) //
        ;
}

// ======================================================================
