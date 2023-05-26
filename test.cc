#include <gpcpp.h>

#include <chrono>
#include <iostream>

int main() {
    // start timing
    auto start = std::chrono::high_resolution_clock::now();
    gpcpp::Rectangle rect1(5, 5, 8, 6, 0);
    gpcpp::Rectangle rect2(7, 7, 10, 10, 44);
    gpcpp::Polygon result;

    gpcpp::Intersect(rect1, rect2, result);
    if (!result.is_empty()) {
        auto centroid = result.centroid();
        std::cout << "Area: " << result.area() << std::endl;
        std::cout << "Centroid: " << centroid.first << ", " << centroid.second << std::endl;
        std::cout << "Vertices: ";
        result.PrintVertices();
        std::cout << "Num contours: " << result.num_contours() << std::endl;
        std::cout << "Num strips: " << result.num_strips() << std::endl;
    } else {
        std::cout << "No intersection" << std::endl;
    }
    // end timing
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Time: "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us"
              << std::endl;
    return 0;
}
