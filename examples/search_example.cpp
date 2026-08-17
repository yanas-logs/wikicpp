#include <wikicpp/wikicpp.hpp>
#include <iostream>

int main() {
    try {
        wikicpp::Wikipedia wiki("en");
        auto results = wiki.search("C++ programming language", 5);

        std::cout << "Found " << results.size() << " results:" << std::endl;
        std::cout << std::endl;

        for (const auto& res : results) {
            std::cout << "Title: " << res.title << std::endl;
            std::cout << "Snippet: " << res.snippet << std::endl;
            std::cout << "PageID: " << res.pageid << std::endl;
            std::cout << "Size: " << res.size << " bytes" << std::endl;
            std::cout << "---" << std::endl;
            std::cout << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
