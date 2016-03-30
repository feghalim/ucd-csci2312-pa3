//
// Created by Mario on 3/28/2016.
//

#include "Point.h"
#include "Exceptions.h"
#include <cmath>

namespace Clustering {

    unsigned int Point::__idGen = 0;
    const char Point::POINT_VALUE_DELIM = ',';

//default constructor
    Point::Point(unsigned int dim) {
        if(dim == 0) {
            throw ZeroDimensionsEx();
        }
        __id = __idGen++;
        __dim = dim;
        __values = new double[__dim];

        for (int i = 0; i < __dim; i++) {
            __values[i] = 0;
        }
    }

//copy constructor
    Point::Point(const Point &p) {
        this->__id = p.__id;
        this->__dim = p.__dim;
        this->__values = new double[__dim];

        for (int i = 0; i < __dim; i++) {
            this->__values[i] = p.__values[i];
        }
    }

    Point &Point::operator=(const Point &p) {
        if(p.__dim != __dim) {
            throw DimensionalityMismatchEx(p.__dim, __dim);
        }
        __id = p.__id;
        __dim = p.__dim;
        delete[] this->__values;
        __values = new double[__dim];

        for (int i = 0; i < __dim; i++) {
            this->__values[i] = p.getValue(i);
        }
        return *this;
    }

//destructor
    Point::~Point() {
        delete[] __values;
    }

    int Point::getId() const {
        return __id;
    }

    unsigned int Point::getDims() const {
        return __dim;
    }

    double Point::getValue(unsigned int i) const {
        if(i >= __dim) {
            throw OutOfBoundsEx(i, __dim);
        }
        return __values[i];
    }

    void Point::setValue(unsigned int i, double val) {
        if(i >= __dim) {
            throw OutOfBoundsEx(i, __dim);
        }
        __values[i] = val;
    }

    double Point::distanceTo(const Point &p) const {
        if(__dim != p.__dim) {
            throw DimensionalityMismatchEx(__dim, p.__dim);
        }
        double d = 0, sq;
        for(int i = 0; i < __dim; i++) {
            if(__values[i] == p.__values[i]) {
                d += 0;
            }
            else {
                sq = p.__values[i] - __values[i];
                d += pow(sq, 2);
            }
        }
        d = sqrt(d);
        return d;
    }

    const double &Point::operator[](unsigned int index) const {
        if (index >= __dim) {
            throw OutOfBoundsEx(index, __dim);
        }

        return __values[index];
    }

    double &Point::operator[](unsigned int index) {
        if (index >= __dim) {
            throw OutOfBoundsEx(index, __dim);
        }

        return __values[index];
    }

    Point &Point::operator*=(double multiF) {
        for (int i = 0; i < __dim; i++) {
            __values[i] *= multiF;
        }
        return *this;
    }

    Point &Point::operator/=(double divQ) {
        for (int i = 0; i < __dim; i++) {
            __values[i] /= divQ;
        }
        return *this;
    }

    const Point Point::operator/(double divQ) const {
        Point p(__dim);
        for (int i = 0; i < __dim; i++) {
            p.__values[i] = __values[i] / divQ;
        }
        return p;
    }

    const Point Point::operator*(double multiF) const {
        Point p(__dim);
        for (int i = 0; i < __dim; i++) {
            p.__values[i] = __values[i] * multiF;
        }
        return p;
    }

    Point &operator+=(Point &p1, const Point &p2) {
        //test if points have equal dimensions
        if (p1.__dim != p2.__dim) {
            throw DimensionalityMismatchEx(p1.__dim, p2.__dim);
        }

        for (int i = 0; i < p1.__dim; i++) {
            p1.__values[i] += p2.__values[i];
        }
        return p1;
    }

    const Point operator+(const Point &p1, const Point &p2) {
        //test if points have equal dimensions
        if (p1.__dim != p2.__dim) {
            throw DimensionalityMismatchEx(p1.__dim, p2.__dim);
        }

        Point p3(p1.__dim);
        for (int i = 0; i < p1.__dim; i++) {
            p3.__values[i] = p1.__values[i] + p2.__values[i];
        }
        return p3;
    }

    Point &operator-=(Point &p1, const Point &p2) {
        //test if points have equal dimensions
        if (p1.__dim != p2.__dim) {
            throw DimensionalityMismatchEx(p1.__dim, p2.__dim);
        }

        for (int i = 0; i < p1.__dim; i++) {
            p1.__values[i] -= p2.__values[i];
        }
        return p1;
    }

    const Point operator-(const Point &p1, const Point &p2) {
        //test if points have equal dimensions
        if (p1.__dim != p2.__dim) {
            throw DimensionalityMismatchEx(p1.__dim, p2.__dim);
        }

        Point p3(p1.__dim);
        for (int i = 0; i < p1.__dim; i++) {
            p3.__values[i] = p1.__values[i] - p2.__values[i];
        }
        return p3;
    }

    bool operator==(const Point &p1, const Point &p2) {
        // test ids
        if (p1.__id == p2.__id) {

            bool answer;
            //test dimensions
            if (p1.__dim != p2.__dim) {
                throw DimensionalityMismatchEx(p1.__dim, p2.__dim);
            }
            for (int i = 0; i < p1.__dim; i++) {
                answer = (p1.__values[i] == p2.__values[i]);
                if(answer != true) {
                    return false;
                }
            }
            return answer;
        }
        else {
            return false;
        }
    }

    bool operator!=(const Point &p1, const Point &p2) {
        return !(p1 == p2);
    }

    bool operator<(const Point &p1, const Point &p2) {
        //test dimensions
        if (p1.__dim != p2.__dim) {
            throw DimensionalityMismatchEx(p1.__dim, p2.__dim);
        }

        bool answer;

        for (int i = 0; i < p1.__dim; i++) {
            if (p1.__values[i] < p2.__values[i]) {
                answer = true;
            }
            else {
                return false;
            }
        }
        return answer;
    }

    bool operator<=(const Point &p1, const Point &p2) {
        //test dimensions
        if (p1.__dim != p2.__dim) {
            throw DimensionalityMismatchEx(p1.__dim, p2.__dim);
        }

        bool answer;

        for (int i = 0; i < p1.__dim; i++) {
            if (p1.__values[i] <= p2.__values[i]) {
                answer = true;
            }
            else {
                return false;
            }
        }
        return answer;
    }

    bool operator>(const Point &p1, const Point &p2) {
        //test dimensions
        if (p1.__dim != p2.__dim) {
            throw DimensionalityMismatchEx(p1.__dim, p2.__dim);
        }

        bool answer;

        for (int i = 0; i < p1.__dim; i++) {
            if (p1.__values[i] > p2.__values[i]) {
                answer = true;
            }
            else {
                return false;
            }
        }
        return answer;
    }

    bool operator>=(const Point &p1, const Point &p2) {
        //test dimensions
        if (p1.__dim != p2.__dim) {
            throw DimensionalityMismatchEx(p1.__dim, p2.__dim);
        }

        bool answer;

        for (int i = 0; i < p1.__dim; i++) {
            if (p1.__values[i] >= p2.__values[i]) {
                answer = true;
            }
            else {
                return false;
            }
        }
        return answer;
    }

    std::ostream &operator<<(std::ostream &os, const Point &p) {
        int k = 0;
        for (int i = 0; i < p.__dim - 1; i++) {
            os << p.__values[k] << p.POINT_VALUE_DELIM;
            k++;
        }
        os << p.__values[k] << std::endl;
        return os;
    }

    std::istream &operator>>(std::istream &is, Point &p) {
        int dim = 0;
        int i = 0;
        std::string insertPoint;
        double value;
        while (std::getline(is, insertPoint, p.POINT_VALUE_DELIM)) {
            value = stod(insertPoint);
            if(i < p.__dim) {
                p.setValue(i, value);
                i++;
            }
            dim++;
        }
        //test dimension
        if (p.__dim != dim) {
            throw DimensionalityMismatchEx(p.__dim, dim);
        }
        return is;
    }
}