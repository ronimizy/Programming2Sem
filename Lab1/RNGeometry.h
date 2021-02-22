//
// Created by Георгий Круглов on 21.02.2021.
//

#ifndef LAB1_RNGEOMETRY_H
#define LAB1_RNGEOMETRY_H

#include <iostream>
#include <vector>

namespace RNGeometry {
    struct Point {
        double x, y;

        explicit Point(double x = 0, double y = 0);

        Point(const Point &origin);

        double distanceFrom(const Point &point) const;

        double distanceFromCenter() const;

        Point normal() const;

        Point operator+(const Point &rhs) const;

        Point operator-(const Point &rhs) const;

        Point operator-() const;

        Point &operator+=(const Point &rhs);

        Point &operator-=(const Point &rhs);

        friend std::istream &operator>>(std::istream &in, Point &point);

        friend std::ostream &operator<<(std::ostream &out, const Point &point);
    };

    namespace Lines {
        class PolygonalLine {
        protected:
            std::vector <RNGeometry::Point> points;

        public:
            PolygonalLine(std::vector <RNGeometry::Point> points);

            PolygonalLine(PolygonalLine &&origin) noexcept;

            PolygonalLine(const PolygonalLine &origin);

            virtual double length();

            long vertexCount() const;

            virtual std::vector<RNGeometry::Point>::iterator begin();

            virtual std::vector<RNGeometry::Point>::iterator end();

            RNGeometry::Point &operator[](const long &index);

            const RNGeometry::Point &operator[](const long &index) const;

            PolygonalLine &operator+=(const RNGeometry::Point &point);

            PolygonalLine operator+(const RNGeometry::Point &point);

            friend std::ostream &operator<<(std::ostream &out, PolygonalLine &line);
        };

        class ClosedPolygonalLine : public PolygonalLine {
        public:
            ClosedPolygonalLine(const std::vector <RNGeometry::Point> &points);

            ClosedPolygonalLine(ClosedPolygonalLine &&origin);

            ClosedPolygonalLine(const ClosedPolygonalLine &origin);

            std::vector<RNGeometry::Point>::iterator begin() override;

            std::vector<RNGeometry::Point>::iterator end() override;

            double length() override;
        };
    }

    namespace Figures {
        class Figure {
        protected:
            std::vector <Point> points;

            virtual ~Figure();

        public:
            Figure(std::vector <Point> points);

            virtual double area() = 0;

            double perimeter() const;

            std::vector<Point>::iterator begin();

            std::vector<Point>::iterator end();
        };

        class ConvexPolygon : public Figure {
            bool isConvex();

        public:
            ConvexPolygon(const std::vector <Point> &points);

            ConvexPolygon(ConvexPolygon &&origin) noexcept;

            ConvexPolygon(const ConvexPolygon &origin);

            ~ConvexPolygon() override;

            double area() override;
        };

        class Triangle : public ConvexPolygon {
        public:
            Triangle(const std::vector <Point> &points);

            Triangle(Triangle &&origin) noexcept;

            Triangle(const Triangle &origin);

            ~Triangle() override;
        };

        class Trapeze : public ConvexPolygon {
            bool isTrapeze();

        public:
            Trapeze(const std::vector <Point> &points);

            Trapeze(Trapeze &&origin) noexcept;

            Trapeze(const Trapeze &origin);

            ~Trapeze() override;
        };

        class AppropriatePolygon : public ConvexPolygon {
            bool isAppropriate();

        public:
            AppropriatePolygon(const std::vector <Point> &points);

            AppropriatePolygon(AppropriatePolygon &&origin) noexcept;

            AppropriatePolygon(const AppropriatePolygon &origin);

            ~AppropriatePolygon() override;
        };
    }
}

#endif //LAB1_RNGEOMETRY_H
