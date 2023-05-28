#include <fcpc.h>
#include <gpcpp.h>

#include <chrono>
#include <iostream>

int main() {
    // start timing
    auto start = std::chrono::high_resolution_clock::now();
    double r1[] = {-6, -3, 7, 7, 0};
    double r2[] = {-6, -3, 7, 7, 50};

    {
        fcpc::Rectangle clip_rect(r1[0], r1[1], r1[2], r1[3], r1[4]);
        fcpc::Rectangle subj_rect(r2[0], r2[1], r2[2], r2[3], r2[4]);
        fcpc::Polygon result, work_area;
        fcpc::SuthHodgClip(subj_rect, clip_rect, result, work_area);
        fcpc::SuthHodgClip(subj_rect, clip_rect, result, work_area);

        std::cout << "=== fcpc ===" << std::endl;
        if (!result.is_empty()) {
            auto centroid = result.centroid();
            std::cout << "Area: " << result.area() << std::endl;
            std::cout << "Centroid: " << centroid.x << ", " << centroid.y << std::endl;
            std::cout << "Vertices: ";
            result.PrintVertices();
        } else {
            std::cout << "No intersection" << std::endl;
        }
    }

    std::cout << std::endl;
    {
        gpcpp::Rectangle clip_rect(r1[0], r1[1], r1[2], r1[3], r1[4]);
        gpcpp::Rectangle subj_rect(r2[0], r2[1], r2[2], r2[3], r2[4]);
        gpcpp::Polygon result;
        gpcpp::Intersect(subj_rect, clip_rect, result);

        std::cout << "=== gpcpp ===" << std::endl;
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
    }
    // end timing
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Time: "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us"
              << std::endl;
    return 0;
}
