#include "gpcpp.h"

#include <cassert>
#include <iostream>

namespace gpcpp {

/**
 * @brief Clamp the angle to [0, ub)
 *
 * @param theta
 * @param ub
 */
double ClampTheta(double theta, const double ub) {
    theta = std::fmod(theta, ub);
    if (theta == -0.) {
        theta = 0;
    }
    if (theta < 0) {
        theta += ub;
    }
    return theta;
}

/**
 * @brief Construct a new Polygon:: Polygon object
 *
 */
Polygon::Polygon() {
    polygon_.num_contours = 0;
    polygon_.hole = nullptr;
    polygon_.contour = nullptr;
    tristrip_.num_strips = 0;
    tristrip_.strip = nullptr;
}

/**
 * @brief Destroy the Polygon:: Polygon object
 *
 */
Polygon::~Polygon() {
    gpc_free_polygon(&polygon_);
    gpc_free_tristrip(&tristrip_);
}

/**
 * @brief Clear the polygon
 *
 */
void Polygon::Clear(void) {
    gpc_free_polygon(&polygon_);
    gpc_free_tristrip(&tristrip_);
    polygon_.num_contours = 0;
    polygon_.hole = nullptr;
    polygon_.contour = nullptr;
    tristrip_.num_strips = 0;
    tristrip_.strip = nullptr;
}

/**
 * @brief Update the tristrip
 *
 */
void Polygon::UpdateTristrip(void) {
    if (tristrip_.num_strips > 0) {
        gpc_free_tristrip(&tristrip_);
    }
    gpc_polygon_to_tristrip(&polygon_, &tristrip_);
}

void Polygon::PrintVertices(void) {
    assert(polygon_.num_contours == 1);
    auto& contour = polygon_.contour[0];
    for (int j = 0; j < contour.num_vertices; j++) {
        std::cout << "[" << contour.vertex[j].x << ", " << contour.vertex[j].y << "], ";
    }
    std::cout << std::endl;
}

/**
 * @brief Calculate the area of the polygon
 *
 * @return double
 */
double Polygon::area(void) {
    double a = 0;
    for (int i = 0; i < tristrip_.num_strips; i++) {
        auto& strip = tristrip_.strip[i];
        for (int j = 0; j < strip.num_vertices - 2; j++) {
            a += 0.5 *
                 std::fabs(strip.vertex[j].x * (strip.vertex[j + 1].y - strip.vertex[j + 2].y) +
                           strip.vertex[j + 1].x * (strip.vertex[j + 2].y - strip.vertex[j].y) +
                           strip.vertex[j + 2].x * (strip.vertex[j].y - strip.vertex[j + 1].y));
        }
    }
    return a;
}

/**
 * @brief Calculate the centroid of the polygon
 *
 * @return std::pair<double, double>
 */
std::pair<double, double> Polygon::centroid(void) {
    double cx = 0;
    double cy = 0;
    assert(polygon_.num_contours == 1);
    for (int i = 0; i < polygon_.contour[0].num_vertices; i++) {
        cx += polygon_.contour[0].vertex[i].x;
        cy += polygon_.contour[0].vertex[i].y;
    }
    cx /= polygon_.contour[0].num_vertices;
    cy /= polygon_.contour[0].num_vertices;
    return std::make_pair(cx, cy);
}

/**
 * @brief Construct a new Rectangle:: Rectangle object
 *
 * @param x
 * @param y
 * @param width
 * @param height
 * @param angle
 */
Rectangle::Rectangle(double x, double y, double width, double height, double angle)
    : x_(x), y_(y), width_(width), height_(height), angle_(ClampTheta(angle, 360)) {
    polygon_.num_contours = 1;
    polygon_.hole = new int[1];
    polygon_.hole[0] = 0;
    polygon_.contour = new gpc_vertex_list[1];
    polygon_.contour[0].num_vertices = 4;
    polygon_.contour[0].vertex = new gpc_vertex[4];

    UpdateVertices();
}

/**
 * @brief Set the rectangle
 *
 * @param x
 * @param y
 * @param width
 * @param height
 * @param angle
 */
void Rectangle::Set(double x, double y, double width, double height, double angle) {
    x_ = x;
    y_ = y;
    width_ = width;
    height_ = height;
    angle_ = ClampTheta(angle, 360);

    UpdateVertices();
}

/**
 * @brief Get the vertices of the rectangle
 *
 * @param i
 * @return std::pair<double, double>
 */
std::pair<double, double> Rectangle::vertex(int i) {
    assert(i >= 0 && i < 4);
    return std::make_pair(polygon_.contour[0].vertex[i].x, polygon_.contour[0].vertex[i].y);
}

/**
 * @brief Update the vertices of the rectangle
 *
 */
void Rectangle::UpdateVertices(void) {
    double x1 = -width_ / 2;
    double y1 = -height_ / 2;
    double x2 = width_ / 2;
    double y2 = -height_ / 2;
    double x3 = width_ / 2;
    double y3 = height_ / 2;
    double x4 = -width_ / 2;
    double y4 = height_ / 2;

    assert(angle_ >= 0 && angle_ < 360);
    double rad = angle_ * M_PI / 180;
    double cos_angle = std::cos(rad);
    double sin_angle = std::sin(rad);

    polygon_.contour[0].vertex[0].x = x_ + x1 * cos_angle - y1 * sin_angle;
    polygon_.contour[0].vertex[0].y = y_ + x1 * sin_angle + y1 * cos_angle;
    polygon_.contour[0].vertex[1].x = x_ + x2 * cos_angle - y2 * sin_angle;
    polygon_.contour[0].vertex[1].y = y_ + x2 * sin_angle + y2 * cos_angle;
    polygon_.contour[0].vertex[2].x = x_ + x3 * cos_angle - y3 * sin_angle;
    polygon_.contour[0].vertex[2].y = y_ + x3 * sin_angle + y3 * cos_angle;
    polygon_.contour[0].vertex[3].x = x_ + x4 * cos_angle - y4 * sin_angle;
    polygon_.contour[0].vertex[3].y = y_ + x4 * sin_angle + y4 * cos_angle;
}

/**
 * @brief Calculate the intersection of two polygons
 *
 * @param rect1
 * @param rect2
 * @param result
 */
void Intersect(Polygon& rect1, Polygon& rect2, Polygon& result) {
    if (result.num_contours() > 0) {
        result.Clear();
    }

    gpc_polygon_clip(GPC_INT, &rect1.polygon_, &rect2.polygon_, &result.polygon_);
    if (!result.is_empty()) {
        result.UpdateTristrip();
    }
}

}  // namespace gpcpp
