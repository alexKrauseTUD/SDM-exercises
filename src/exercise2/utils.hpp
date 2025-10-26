#pragma once

#include <expected>
#include <string>
#include <string_view>
#include <vector>
#include <cstdint>
#include <charconv>
#include <limits>

namespace csv {
    using err_t = std::string;

    std::expected<std::vector<std::string>, err_t>
    split_line(std::string_view line, size_t max_fields, char delim = '|');

    template <class T>
    inline std::expected<T, err_t> from_field(std::string_view);

    template <>
    inline std::expected<std::string, err_t>
    from_field<std::string>(std::string_view sv) {
        return std::string(sv);
    }

    template <>
    inline std::expected<std::int64_t, err_t>
    from_field<std::int64_t>(std::string_view sv) {
        std::int64_t v{};
        auto* first = sv.data();
        auto* last  = sv.data() + sv.size();
        if (sv.empty())
            return std::numeric_limits<std::int64_t>::max();
        auto [ptr, ec] = std::from_chars(first, last, v, 10);
        if (ec != std::errc{} || ptr != last)
            return std::unexpected("Invalid int64_t: '" + std::string(sv) + "'");
        return v;
    }

}