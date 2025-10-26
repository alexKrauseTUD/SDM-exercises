#pragma once
#include <string>
#include <string_view>
#include <cstdint>
#include <expected>
#include <vector>
#include "utils.hpp"

class company_name;

class company_name_record {
    private:
        company_name_record(std::int64_t id,
                            std::string name,
                            std::string country_code,
                            std::int64_t imdb_id,
                            std::string name_pcode_nf,
                            std::string name_pcode_sf,
                            std::string md5sum)
        : _id(id)
        , _name(std::move(name))
        , _country_code(std::move(country_code))
        , _imdb_id(imdb_id)
        , _name_pcode_nf(std::move(name_pcode_nf))
        , _name_pcode_sf(std::move(name_pcode_sf))
        , _md5sum(std::move(md5sum)) 
        {}

        static inline std::expected<company_name_record, csv::err_t>
        from_csv_row(std::string_view row, char delim = '|') {
            auto fields_exp = csv::split_line(row, 8, delim);
            if (!fields_exp) {
                return std::unexpected(fields_exp.error());
            }
            auto& f = *fields_exp;
            auto id     = csv::from_field<std::int64_t>(f[0]);
            auto imdb   = csv::from_field<std::int64_t>(f[3]);
            if (!id) {
                return std::unexpected("Field 0 (id): "   + id.error());
            }
            if (!imdb) {
                return std::unexpected("Field 3 (imdb): " + imdb.error());
            }

            return company_name_record{
                *id,
                f[1],
                f[2],
                *imdb,
                f[4],
                f[5],
                f[6],
            };
        }
    public:
        int64_t              id()           const noexcept { return _id; }
        std::string const & name()          const noexcept { return _name; }
        std::string const & country_code()  const noexcept { return _country_code; }
        int64_t             imdb_id()       const noexcept { return _imdb_id; }
        std::string const & name_pcode_nf() const noexcept { return _name_pcode_nf; }
        std::string const & name_pcode_sf() const noexcept { return _name_pcode_sf; }
        std::string const & md5sum()        const noexcept { return _md5sum; }
    private:
        int64_t     _id;
        std::string _name;
        std::string _country_code;
        int64_t     _imdb_id;
        std::string _name_pcode_nf;
        std::string _name_pcode_sf;
        std::string _md5sum;
    friend class company_name;
    friend std::ostream & operator<<(std::ostream & os, company_name_record const & record) {
        os << "company_name_record { "
           << "id: " << record.id() << ", "
           << "name: " << record.name() << ", "
           << "country_code: " << record.country_code() << ", "
           << "imdb_id: " << record.imdb_id() << ", "
           << "name_pcode_nf: " << record.name_pcode_nf() << ", "
           << "name_pcode_sf: " << record.name_pcode_sf() << ", "
           << "md5sum: " << record.md5sum()
           << " }";
        return os;
    }
};


class company_name {
    using record_type = company_name_record;
    private:
        company_name() = default;
    public:
        static std::expected<company_name, csv::err_t> load_from_file(std::string_view filepath, char delim = '|');
        std::vector<record_type> const & records() const noexcept {
            return _records;
        }
    private:
        std::vector<record_type> _records;
};