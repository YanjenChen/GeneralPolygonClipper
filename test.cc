#include <gpcpp.h>

#include <iostream>

int main() {
    gpcpp::Rectangle rect1(0, 0, 10, 10, 0);
    gpcpp::Rectangle rect2(5, 5, 10, 10, 45);
    gpcpp::Polygon result;

    gpcpp::Intersect(rect1, rect2, result);
    if (!result.is_empty()) {
        auto centroid = result.centroid();
        std::cout << "Area: " << result.area() << std::endl;
        std::cout << "Centroid: " << centroid.first << ", " << centroid.second << std::endl;
        std::cout << "Num contours: " << result.num_contours() << std::endl;
        std::cout << "Num strips: " << result.num_strips() << std::endl;
    } else {
        std::cout << "No intersection" << std::endl;
    }

    return 0;
}
