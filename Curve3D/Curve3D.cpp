#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <memory>
#include <random>
#include <numeric>
#include <iomanip>

constexpr double PI = 3.14159265358979323846;

struct Point3D {
    double x, y, z;
    friend std::ostream& operator<<(std::ostream& os, const Point3D& p) {
        return os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
    }
};

class Curve3D {
public:
    virtual ~Curve3D() = default;
    virtual Point3D getPoint(double t) const = 0;
    virtual Point3D getDerivative(double t) const = 0;
    virtual void printInfo(double t) const = 0;
};

class Circle : public Curve3D {
    double radius;
public:
    Circle(double r) : radius(r) {
        if (radius <= 0) throw std::invalid_argument("Radius must be positive");
    }

    Point3D getPoint(double t) const override {
        return { radius * cos(t), radius * sin(t), 0.0 };
    }

    Point3D getDerivative(double t) const override {
        return { -radius * sin(t), radius * cos(t), 0.0 };
    }

    void printInfo(double t) const override {
        std::cout << "Circle (r=" << radius << ")";
    }

    double getRadius() const { return radius; }
};

class Ellipse : public Curve3D {
    double radiusX, radiusY;
public:
    Ellipse(double rx, double ry) : radiusX(rx), radiusY(ry) {
        if (radiusX <= 0 || radiusY <= 0)
            throw std::invalid_argument("Radii must be positive");
    }

    Point3D getPoint(double t) const override {
        return { radiusX * cos(t), radiusY * sin(t), 0.0 };
    }

    Point3D getDerivative(double t) const override {
        return { -radiusX * sin(t), radiusY * cos(t), 0.0 };
    }

    void printInfo(double t) const override {
        std::cout << "Ellipse (rx=" << radiusX << ", ry=" << radiusY << ")";
    }
};

class Helix : public Curve3D {
    double radius, step;
public:
    Helix(double r, double s) : radius(r), step(s) {
        if (radius <= 0 || step <= 0)
            throw std::invalid_argument("Radius and step must be positive");
    }

    Point3D getPoint(double t) const override {
        return { radius * cos(t), radius * sin(t), step * t / (2 * PI) };
    }

    Point3D getDerivative(double t) const override {
        return { -radius * sin(t), radius * cos(t), step / (2 * PI) };
    }

    void printInfo(double t) const override {
        std::cout << "Helix (r=" << radius << ", step=" << step << ")";
    }
};

std::unique_ptr<Curve3D> createRandomCurve() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> type_dist(0, 2);
    static std::uniform_real_distribution<> param_dist(0.1, 10.0);

    switch (type_dist(gen)) {
    case 0: return std::make_unique<Circle>(param_dist(gen));
    case 1: return std::make_unique<Ellipse>(param_dist(gen), param_dist(gen));
    case 2: return std::make_unique<Helix>(param_dist(gen), param_dist(gen));
    default: throw std::runtime_error("Invalid curve type");
    }
}

int main() {
    try {
        // 1. Create container of random curves
        std::vector<std::shared_ptr<Curve3D>> curves;
        for (int i = 0; i < 15; ++i) {
            curves.push_back(std::shared_ptr<Curve3D>(createRandomCurve().release()));
        }

        // 2. Print points and derivatives at t=PI/4
        const double t = PI / 4;
        std::cout << "Points and derivatives at t=PI/4:\n";
        for (const auto& curve : curves) {
            curve->printInfo(t);
            std::cout << "\nPoint: " << curve->getPoint(t)
                << ", Derivative: " << curve->getDerivative(t) << "\n\n";
        }

        // 3. Create second container with circles only
        std::vector<std::shared_ptr<Circle>> circles;
        for (const auto& curve : curves) {
            if (auto circle = dynamic_cast<Circle*>(curve.get())) {
                circles.emplace_back(std::static_pointer_cast<Circle>(curve));
            }
        }

        // 4. Sort circles by radius
        std::sort(circles.begin(), circles.end(),
            [](const auto& a, const auto& b) {
            return a->getRadius() < b->getRadius();
        });

        // 5. Compute total sum of radii
        double total_radius = std::accumulate(circles.begin(), circles.end(), 0.0,
            [](double sum, const auto& circle) {
            return sum + circle->getRadius();
        });

        std::cout << "\nSorted circle radii:";
        for (const auto& circle : circles) {
            std::cout << " " << circle->getRadius();
        }
        std::cout << "\nTotal sum of radii: " << std::fixed << std::setprecision(2)
            << total_radius << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}