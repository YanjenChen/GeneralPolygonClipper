#include "fcpc.h"

#include <cassert>
#include <cmath>

namespace fcpc {

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
    : PolygonBase(4), x_(x), y_(y), width_(width), height_(height), angle_(angle) {
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
    angle_ = angle;
    UpdateVertices();
}

/**
 * @brief Update the vertices of the rectangle
 *
 */
void Rectangle::UpdateVertices(void) {
    // Vertices are in clockwise order

    double x1 = -width_ / 2;
    double y1 = -height_ / 2;
    double x2 = -width_ / 2;
    double y2 = height_ / 2;
    double x3 = width_ / 2;
    double y3 = height_ / 2;
    double x4 = width_ / 2;
    double y4 = -height_ / 2;

    assert(angle_ >= 0 && angle_ < 360);
    double rad = angle_ * M_PI / 180;
    double cos_angle = std::cos(rad);
    double sin_angle = std::sin(rad);

    vertices_[0].x = x_ + x1 * cos_angle - y1 * sin_angle;
    vertices_[0].y = y_ + x1 * sin_angle + y1 * cos_angle;
    vertices_[1].x = x_ + x2 * cos_angle - y2 * sin_angle;
    vertices_[1].y = y_ + x2 * sin_angle + y2 * cos_angle;
    vertices_[2].x = x_ + x3 * cos_angle - y3 * sin_angle;
    vertices_[2].y = y_ + x3 * sin_angle + y3 * cos_angle;
    vertices_[3].x = x_ + x4 * cos_angle - y4 * sin_angle;
    vertices_[3].y = y_ + x4 * sin_angle + y4 * cos_angle;
}

/**
 * @brief Returns x-value of point of intersection of two lines
 *
 * @param p1
 * @param p2
 * @param p3
 * @param p4
 * @return double
 */
inline double XIntersect(const Point &p1, const Point &p2, const Point &p3, const Point &p4) {
    double num =
        (p1.x * p2.y - p1.y * p2.x) * (p3.x - p4.x) - (p1.x - p2.x) * (p3.x * p4.y - p3.y * p4.x);
    double den = (p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x);
    return num / den;
}

/**
 * @brief Returns y-value of point of intersection of two lines
 *
 * @param p1
 * @param p2
 * @param p3
 * @param p4
 * @return double
 */
inline double YIntersect(const Point &p1, const Point &p2, const Point &p3, const Point &p4) {
    double num =
        (p1.x * p2.y - p1.y * p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x * p4.y - p3.y * p4.x);
    double den = (p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x);
    return num / den;
}

/**
 * @brief This functions clips all the edges w.r.t one clip edge of clipping area
 *
 * @param result
 * @param p1
 * @param p2
 */
void Clip(Polygon &poly, const Point &p1, const Point &p2, Polygon &work_area) {
    work_area.Clear();

    // (ix,iy), (kx,ky) are the co-ordinate values of the points
    for (int i = 0; i < poly.num_vertices_; i++) {
        // i and k form a line in polygon
        int k = (i + 1) % poly.num_vertices_;
        const Point &pi = poly.vertices_[i];
        const Point &pk = poly.vertices_[k];

        // Calculating position of first point w.r.t. clipper line
        double i_pos = (p2.x - p1.x) * (pi.y - p1.y) - (p2.y - p1.y) * (pi.x - p1.x);

        // Calculating position of second point w.r.t. clipper line
        double k_pos = (p2.x - p1.x) * (pk.y - p1.y) - (p2.y - p1.y) * (pk.x - p1.x);

        // Case 1 : When both points are inside
        if (i_pos < 0 && k_pos < 0) {
            // Only second point is added
            work_area.AddVertex(pk);
        }

        // Case 2: When only first point is outside
        else if (i_pos >= 0 && k_pos < 0) {
            // Point of intersection with edge and the second point is added
            work_area.AddVertex(Point(XIntersect(p1, p2, pi, pk), YIntersect(p1, p2, pi, pk)));
            work_area.AddVertex(pk);
        }

        // Case 3: When only second point is outside
        else if (i_pos < 0 && k_pos >= 0) {
            // Only point of intersection with edge is added
            work_area.AddVertex(Point(XIntersect(p1, p2, pi, pk), YIntersect(p1, p2, pi, pk)));
        }

        // Case 4: When both points are outside
        else {
            // No points are added
        }
    }

    // Copying new points into original array and changing the no. of vertices
    poly.num_vertices_ = work_area.num_vertices_;
    for (int i = 0; i < poly.num_vertices_; i++) {
        poly.vertices_[i] = work_area.vertices_[i];
    }
}

/**
 * @brief Sutherland-Hodgman clipping algorithm
 *
 * @param subject_rect
 * @param clip_rect
 * @param result
 */
void SuthHodgClip(const Rectangle &subject_rect, const Rectangle &clip_rect, Polygon &result,
                  Polygon &work_area) {
    // copy subject polygon to result
    result.num_vertices_ = subject_rect.num_vertices_;
    for (int i = 0; i < subject_rect.num_vertices_; i++) {
        result.vertices_[i] = subject_rect.vertices_[i];
    }

    // i and k are two consecutive indexes
    for (int i = 0; i < clip_rect.num_vertices_; i++) {
        int k = (i + 1) % clip_rect.num_vertices_;

        // We pass the current array of vertices, it's size and the end points of the selected
        // clipper line
        Clip(result, clip_rect.vertices_[i], clip_rect.vertices_[k], work_area);
    }

    if (result.num_vertices_ < 3) {
        result.num_vertices_ = 0;
    }
}

}  // namespace fcpc
