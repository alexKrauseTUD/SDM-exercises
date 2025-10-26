#include <sstream>
#include "utils.hpp"

std::expected<std::vector<std::string>, csv::err_t>
csv::split_line(std::string_view line, size_t max_fields, char delim) {
    std::vector<std::string> out;
    out.reserve(max_fields);
    std::string current{line};
    std::istringstream line_stream{current};

    for (std::string field; std::getline(line_stream, field, delim);) {
        out.push_back(field);
    }
    

    if (out.size() > max_fields) {
        return std::unexpected(
            "Expected at most " + std::to_string(max_fields) +
            " fields, got " + std::to_string(out.size()));
    }
    return out;
}