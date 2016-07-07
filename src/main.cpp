#include <iostream>
#include <ctime>
#include <chrono>

#include <arranger.hpp>

const int MAX_SIDE = 16;

int main(int argc, char* argv[]) {

    int max_side = MAX_SIDE;
    if (argc > 1) max_side = atoi(argv[1]);

    using namespace std::chrono;
    high_resolution_clock::time_point start_time = high_resolution_clock::now();

    for (int n = 3; n <= max_side; n++) {
        std::cout << "Side: " << n << ", Layouts: ";
        std::vector<nepd::layout> layouts;
        nepd::arranger arr(n);
        arr.find_conforming_layouts(layouts);

        high_resolution_clock::time_point cur_time = high_resolution_clock::now();
        auto int_ms = duration_cast<std::chrono::milliseconds>(cur_time - start_time);
        if (layouts.empty()) {
            std::cout << "None"; 
        } else {
            std::cout << layouts.size();
        }
        std::cout << ". " << " Time used: " << int_ms.count() << "ms" << std::endl;
        start_time = cur_time;

        if (layouts.size() > 0) {
            for (int row = 0; row < n; row++) {
                std::cout << "|";
                for (const auto& lt: layouts) {
                    lt.print(std::cout, row);
                    std::cout << "|";
                }
                std::cout << std::endl;
            }
        } 
    }
            
    return 0;
}

