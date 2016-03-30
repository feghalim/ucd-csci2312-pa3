// File: Point.h
//
// Created by Ivo Georgiev on 9/14/15.
//

#ifndef CLUSTERING_POINT_H
#define CLUSTERING_POINT_H

#include <iostream>

namespace Clustering {

    class Point {
        unsigned int __id;
        unsigned int __dim;        // number of dimensions of the point
        double *__values; // values of the point's dimensions

        static unsigned int __idGen; // id generator

    public:
        static const char POINT_VALUE_DELIM;

        static void rewindIdGen();

        Point(unsigned int dim);
        Point(unsigned int dim, double * array);

        // Big three: cpy ctor, overloaded operator=, dtor
        Point(const Point &p);
        Point &operator=(const Point &p);
        ~Point();

        // Accessors & mutators
        int getId() const;
        unsigned int getDims() const;
        void setValue(unsigned int i, double val);
        double getValue(unsigned int i) const;

        // Functions
        double distanceTo(const Point & p) const;

        // Overloaded operators

        // Members
        Point &operator*=(double multiF); // p *= 6; p.operator*=(6);
        Point &operator/=(double divQ);
        const Point operator*(double multiF) const; // prevent (p1 * 2) = p2;
        const Point operator/(double divQ) const; // p3 = p2 / 2;

        double &operator[](unsigned int index);
        const double &operator[](unsigned int index) const;

        // Friends
        friend Point &operator+=(Point & p1, const Point & p2);
        friend Point &operator-=(Point & p1, const Point & p2);
        friend const Point operator+(const Point & p1, const Point & p2);
        friend const Point operator-(const Point & p1, const Point & p2);

        friend bool operator==(const Point & p1, const Point & p2);
        friend bool operator!=(const Point & p1, const Point & p2);

        friend bool operator<(const Point & p1, const Point & p2);
        friend bool operator>(const Point & p1, const Point & p2);
        friend bool operator<=(const Point & p1, const Point & p2);
        friend bool operator>=(const Point & p1, const Point & p2);

        friend std::ostream &operator<<(std::ostream &os, const Point &p);
        friend std::istream &operator>>(std::istream &is, Point &p);
    };

}
#endif //CLUSTERING_POINT_H
