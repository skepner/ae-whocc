#pragma once

#include "ext/filesystem.hh"
#include "xlsx/sheet.hh"

// ----------------------------------------------------------------------

namespace ae::xlsx::inline v1
{
    namespace csv
    {
        class Sheet : public ae::sheet::Sheet
        {
          public:
            Sheet(const std::filesystem::path& filename);

            sheet::nrow_t number_of_rows() const override { return sheet::nrow_t{data_.size()}; }
            sheet::ncol_t number_of_columns() const override { return number_of_columns_; }
            std::string name() const override { return {}; }
            ae::sheet::cell_t cell(sheet::nrow_t row, sheet::ncol_t col) const override { return data_.at(*row).at(*col); } // row and col are zero based

          private:
            std::vector<std::vector<ae::sheet::cell_t>> data_;
            sheet::ncol_t number_of_columns_{0};
        };

        class Doc
        {
          public:
            Doc(const std::filesystem::path& filename) : sheet_{std::make_shared<Sheet>(filename)} {}

            size_t number_of_sheets() const { return 1; }
            std::shared_ptr<ae::sheet::Sheet> sheet(size_t /*sheet_no*/) { return sheet_; }

          private:
            std::shared_ptr<Sheet> sheet_;
        };

    } // namespace csv

} // namespace ae::xlsx::inline v1

// ----------------------------------------------------------------------
