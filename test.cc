#include <fcpc.h>
#include <gpcpp.h>

#include <chrono>
#include <iostream>

int main() {
    // start timing
    auto start = std::chrono::high_resolution_clock::now();
    fcpc::Rectangle clip_rect(5, 5, 10, 10, 0);
    fcpc::Rectangle subj_rect(5, 15, 10, 10, 0);
    fcpc::Polygon result, work_area;
    fcpc::SuthHodgClip(subj_rect, clip_rect, result, work_area);

    /* gpcpp::Rectangle clip_rect(5, 5, 8, 6, 0);
    gpcpp::Rectangle subj_rect(7, 7, 10, 10, 44);
    gpcpp::Polygon result;
    gpcpp::Intersect(subj_rect, clip_rect, result); */
    if (!result.is_empty()) {
        auto centroid = result.centroid();
        std::cout << "Area: " << result.area() << std::endl;
        std::cout << "Centroid: " << centroid.x << ", " << centroid.y << std::endl;
        /* std::cout << "Centroid: " << centroid.first << ", " << centroid.second << std::endl; */
        std::cout << "Vertices: ";
        result.PrintVertices();
        /* std::cout << "Num contours: " << result.num_contours() << std::endl;
        std::cout << "Num strips: " << result.num_strips() << std::endl; */
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
