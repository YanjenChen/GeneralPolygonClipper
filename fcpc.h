/**
 * @file fgpcpp.h
 * @brief Fast Convex Polygon Clipping Library
 * @author Antares
 */

#ifndef FCPC_H
#define FCPC_H

#include <array>
#include <cassert>
#include <cmath>
#include <iostream>

namespace fcpc {

struct Point {
    double x;
    double y;

    Point() : x(0), y(0) {}
    Point(double x, double y) : x(x), y(y) {}
};

template <size_t N>
class PolygonBase {
   public:
    std::array<Point, N> vertices_;
    int num_vertices_;

    PolygonBase() : num_vertices_(0) {}
    PolygonBase(const PolygonBase&) = default;
    PolygonBase(PolygonBase&&) = default;
    PolygonBase& operator=(const PolygonBase&) = default;
    PolygonBase& operator=(PolygonBase&&) = default;
    ~PolygonBase() {}

    void AddVertex(const double& x, const double& y) {
        assert(num_vertices_ < N);
        vertices_[num_vertices_].x = x;
        vertices_[num_vertices_].y = y;
        num_vertices_++;
    }
    void AddVertex(const Point& p) { AddVertex(p.x, p.y); }
    void Clear(void) { num_vertices_ = 0; }
    void PrintVertices(void) {
        for (int i = 0; i < num_vertices_; i++) {
            std::cout << "[" << vertices_[i].x << ", " << vertices_[i].y << "], ";
        }
        std::cout << std::endl;
    }

    // Getters
    bool is_empty(void) const { return num_vertices_ == 0; }
    double area(void) const {
        assert(num_vertices_ >= 3);
        double a = 0;
        for (int i = 1; i < num_vertices_ - 1; i++) {
            a += 0.5 * std::fabs(vertices_[0].x * (vertices_[i].y - vertices_[i + 1].y) +
                                 vertices_[i].x * (vertices_[i + 1].y - vertices_[0].y) +
                                 vertices_[i + 1].x * (vertices_[0].y - vertices_[i].y));
        }
        return a;
    }
    Point centroid(void) const {
        assert(num_vertices_ >= 3);
        double cx = 0;
        double cy = 0;
        for (int i = 0; i < num_vertices_; i++) {
            cx += vertices_[i].x;
            cy += vertices_[i].y;
        }
        cx /= num_vertices_;
        cy /= num_vertices_;
        return Point(cx, cy);
    }
};

class Polygon : public PolygonBase<8> {
   public:
    Polygon() {}
    Polygon(const Polygon&) = default;
    Polygon(Polygon&&) = default;
    Polygon& operator=(const Polygon&) = default;
    Polygon& operator=(Polygon&&) = default;
    ~Polygon() {}
};

class Rectangle : public PolygonBase<4> {
   private:
    double x_;
    double y_;
    double width_;
    double height_;
    double angle_;

   public:
    Rectangle() : x_(0), y_(0), width_(0), height_(0), angle_(0) {}
    Rectangle(double x, double y, double width, double height, double angle);
    Rectangle(const Rectangle&) = default;
    Rectangle(Rectangle&&) = default;
    Rectangle& operator=(const Rectangle&) = default;
    Rectangle& operator=(Rectangle&&) = default;
    ~Rectangle() {}

    void Set(double x, double y, double width, double height, double angle);

    // Getters
    const Point& vertex(int i) const { return vertices_[i]; }

   private:
    void UpdateVertices(void);
};

void SuthHodgClip(const Rectangle& subject_rect, const Rectangle& clip_rect, Polygon& result,
                  Polygon& work_area);

}  // namespace fcpc

#endif  // FCPC_H
