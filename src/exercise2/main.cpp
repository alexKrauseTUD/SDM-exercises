#include <iostream>
#include "company_name.hpp"

#include <set>

int main() {
    auto cn = company_name::load_from_file("/exercise/imdb/csv/company_name.csv");
    if (!cn) {
        std::cerr << "Error loading company_name.csv: " << cn.error() << "\n";
        return 1;
    }

    std::cout << "Loaded " << (*cn).records().size() << " records from company_name.csv\n";

    // SELECT COUNT(*) FROM company_name WHERE country_code LIKE '[us]'
    int count = 0;
    for (auto const& record: (*cn).records()) {
        if (record.country_code().compare("[us]") == 0) {
            ++count;
        }
    }
    std::cout << "Count of records with country_code LIKE '[us]': " << count << " (should be 138946)\n";
    
    return 0;
}