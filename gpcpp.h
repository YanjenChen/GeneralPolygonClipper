#ifndef GPCPP_H
#define GPCPP_H

#include <cmath>
#include <utility>

extern "C" {
#include "gpc.h"
}

namespace gpcpp {

class Polygon {
   protected:
    gpc_polygon polygon_;
    gpc_tristrip tristrip_;

   public:
    Polygon();
    // Copy constructor and assignment operator are deleted because gpc_polygon
    Polygon(const Polygon&) = delete;
    Polygon(Polygon&&) = delete;
    Polygon& operator=(const Polygon&) = delete;
    Polygon& operator=(Polygon&&) = delete;
    ~Polygon();

    void Clear(void);
    void UpdateTristrip(void);
    void PrintVertices(void);

    // Getters
    int num_contours(void) const { return polygon_.num_contours; }
    int num_strips(void) const { return tristrip_.num_strips; }
    bool is_empty(void) const { return polygon_.num_contours == 0; }
    double area(void);
    std::pair<double, double> centroid(void);

    // Friends
    friend void Intersect(Polygon& rect1, Polygon& rect2, Polygon& result);
};

class Rectangle : public Polygon {
   private:
    double x_;
    double y_;
    double width_;
    double height_;
    double angle_;

   public:
    Rectangle(double x, double y, double width, double height, double angle);
    // Copy constructor and assignment operator are deleted because gpc_polygon
    Rectangle(const Rectangle&) = delete;
    Rectangle(Rectangle&&) = delete;
    Rectangle& operator=(const Rectangle&) = delete;
    Rectangle& operator=(Rectangle&&) = delete;
    ~Rectangle() {}

    void Set(double x, double y, double width, double height, double angle);

    // Getters
    std::pair<double, double> vertex(int i);

   private:
    void UpdateVertices(void);
};

void Intersect(Polygon& rect1, Polygon& rect2, Polygon& result);

}  // namespace gpcpp

#endif  // GPCPP_H
