#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <memory>
#include <ctime>
#include <iomanip>

const double PI = 3.1415926535897932384626433;

class Point3D {
public:
    double x, y, z;
    Point3D(double x, double y, double z) : x(x), y(y), z(z) {}
};


class Curve3D {
public:

    virtual Point3D GetPoint(double t) const = 0;

    virtual Point3D GetDerivative(double t) const = 0;
};

class Circle : public Curve3D {
private:
    double radius;

public:
    Circle(double radius) : radius(radius) {}

    Point3D GetPoint(double t) const override {
        double x = radius * cos(t);
        double y = radius * sin(t);
        double z = 0.0;

        return { x, y, z };
    }

    Point3D GetDerivative(double t) const override {
        double x = -radius * sin(t);
        double y = radius * cos(t);
        double z = 0.0;

        return { x, y, z };
    }

    double GetRadius() const { return radius; }
};


class Ellipse : public Curve3D {
private:
    double radiusX;
    double radiusY;

public:
    Ellipse(double radiusX, double radiusY) : radiusX(radiusX), radiusY(radiusY) {}

    Point3D GetPoint(double t) const override {
        double x = radiusX * cos(t);
        double y = radiusY * sin(t);
        double z = 0.0;

        return { x, y, z };
    }

    Point3D GetDerivative(double t) const override {
        double x = -radiusX * sin(t);
        double y = radiusY * cos(t);
        double z = 0.0;

        return { x, y, z };
    }

    double GetRadiusX() const { return radiusX; }
    double GetRadiusY() const { return radiusY; }
};

class Helix : public Curve3D {
private:
    double radius;
    double step;

public:
    Helix(double radius, double step) : radius(radius), step(step) {}

    Point3D GetPoint(double t) const override {
        double x = radius * cos(t);
        double y = radius * sin(t);
        double z = step * t / (2 * PI);

        return { x, y, z };
    }

    Point3D GetDerivative(double t) const override {
        double x = -radius * sin(t);
        double y = radius * cos(t);
        double z = step / (2 * PI);

        return{ x, y, z };
    }

    double GetRadius() const { return radius; }
    double GetStep() const { return step; }
};

int main() {
    std::vector<std::unique_ptr<Curve3D>> curves;
    srand(time(NULL));

    for (int i = 0; i < 5; i++) {
        double radius = (rand() % 10) + 1.0;
        curves.push_back(std::make_unique<Circle>(radius));
    }

    for (int i = 0; i < 5; i++) {
        double radiusX = (rand() % 10) + 1.0;
        double radiusY = (rand() % 10) + 1.0;
        curves.push_back(std::make_unique<Ellipse>(radiusX, radiusY));
    }

    for (int i = 0; i < 5; i++) {
        double radius = (rand() % 10) + 1.0;
        double step = (rand() % 5) + 1.0;
        curves.push_back(std::make_unique<Helix>(radius, step));
    }

    for (const auto& curve : curves) {
        Point3D point = curve->GetPoint(PI / 4);
        Point3D derivative = curve->GetDerivative(PI / 4);
        std::cout << "Point: (" << point.x << ", " << point.y << ", " << point.z << "), ";
        std::cout << "Derivative: (" << derivative.x << ", " << derivative.y << ", " << derivative.z << ")\n";
    }

    std::vector<Circle*> circles;
    for (const auto& curve : curves) {
        if (dynamic_cast<Circle*>(curve.get())) {
            circles.push_back(static_cast<Circle*>(curve.get()));
        }
    }

    std::sort(circles.begin(), circles.end(), [](Circle* a, Circle* b) {
        return a->GetRadius() < b->GetRadius();
    });

    double totalRadius = 0.0;
    for (const auto& circle : circles) {
        totalRadius += circle->GetRadius();
    }

    std::cout << "Total Radius of Circles: " << std::fixed << std::setprecision(2) << totalRadius << std::endl;

    return 0;
}
