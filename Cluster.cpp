//
// Created by Mario on 3/28/2016.
//

#include <limits>
#include <assert.h>
#include <sstream>
#include <algorithm>
#include "Cluster.h"
#include "Point.h"
#include "Exceptions.h"

namespace Clustering {

    unsigned int Cluster::__idGenerator = 0;
    const char Cluster::POINT_CLUSTER_ID_DELIM = ':';

    LNode::LNode(const Point &p, LNodePtr n) : point(p) {
        next = n;
    }

    Cluster::Centroid::Centroid(unsigned int d, const Cluster &c) : __p(d), __c(c) {
        __dimensions = d;
        __valid = false;
    }
/*    Cluster::Centroid::Centroid(const Centroid &cent) : __p(cent.__p), __c(cent.__c){
        __dimensions = cent.__dimensions;
        __valid = cent.__dimensions;
    }*/
    const Point Cluster::Centroid::get() const {
        return __p;
    }
    void Cluster::Centroid::set(const Point &p) {
        __p = p;
        __valid = true;
    }
    bool Cluster::Centroid::isValid() const {
        return __valid;
    }
    void Cluster::Centroid::setValid(bool valid) {
        __valid = valid;
    }
    void Cluster::Centroid::compute() {
        int size = __c.__size, dim = __dimensions;
        double average, total;
        if(size == 1) {
            __p = __c[0];
        }
        else if (size > 0) {
            for(int i = 0; i < dim; i++) {
                for (int k = 0; k < size; k++) {
                    Point p(__c[k]);
                    total += p[i];
                }
                average = total / size;
                __p.setValue(i, average);
                total = 0;
            }
        }
        else {
            toInfinity();
        }
        setValid(true);
        //std::cout << isValid() << " should be 1" << std::endl;
    }
    bool Cluster::Centroid::equal(const Point &p) const {
        return __p == p;
    }
    void Cluster::Centroid::toInfinity() {
        for(int i = 0; i < __p.getDims(); i++) {
            __p[i] = std::numeric_limits<double>::max();
        }
    }

    //constructor
    Cluster::Cluster(unsigned int d) : centroid(d, *this) {
        __id = __idGenerator;
        __idGenerator++;
        __size = 0;
        __points = nullptr;
        __dimensionality = d;
    }
    //copy constructor
    Cluster::Cluster(const Cluster &c) : centroid(c.__dimensionality, c) {
        __id = c.__id;
        __points = c.__points;
        __dimensionality = c.__dimensionality;
        __size = c.__size;
    }
    Cluster& Cluster::operator=(const Cluster &c) {
        if(__dimensionality != c.__dimensionality) {
            throw DimensionalityMismatchEx(__dimensionality, c.__dimensionality);
        }
        __id = c.__id;
        if(c.__points != nullptr) {
            delete c.__points;
        }
        __points = c.__points;
        __dimensionality = c.__dimensionality;
        __size = c.__size;
        return *this;
    }
    //destructor
    Cluster::~Cluster() {
        delete __points;
    }
    unsigned int Cluster::getSize() const {
        return __size;
    }
    unsigned int Cluster::getId() const {
        return __id;
    }
    unsigned int Cluster::getDimensionality() const {
        return __dimensionality;
    }
    void Cluster::add(const Point &p) {
        if(p.getDims() != __dimensionality) {
            throw DimensionalityMismatchEx(p.getDims(), __dimensionality);
        }
        int iter = 0;
        LNodePtr cursor = __points;
        LNodePtr pCursor = nullptr;
        LNodePtr point = new LNode(p, nullptr);
        if(__points == nullptr) {
            assert(__size == 0);
            __points = point;
            __points->next = nullptr;
            __size++;
            return;
        }
        else if(__points->next == nullptr) {
            __points->next = point;
            __size++;
            return;
        }
        while(cursor != nullptr) {
            pCursor = cursor;
            cursor = cursor->next;
        }
        point->next = cursor;
        pCursor->next = point;
        centroid.setValid(false);
        //std::cout << centroid.isValid() << " should be 0" << std::endl;
        __size++;
    }
    const Point& Cluster::remove(const Point &p) {
        if(p.getDims() != __dimensionality) {
            throw DimensionalityMismatchEx(p.getDims(), __dimensionality);
        }
        LNodePtr cursor = __points;
        LNodePtr pCursor = nullptr;
        int i;
        if(__points == nullptr) {
            return p;
        }
        if(__points->point == p) {
            cursor = __points->next;
            pCursor = __points;
            delete __points;
            __points = cursor;
            __size--;
            return p;
        }
        else {
            while (cursor->point != p) {
                cursor = cursor->next;
                i++;
            }
            cursor = cursor->next;
            __points = cursor;
            centroid.setValid(false);
            //std::cout << centroid.isValid() << " should be 0" << std::endl;
            __size--;
        }
        return p;
    }
    bool Cluster::contains(const Point &p) const {
        if(p.getDims() != __dimensionality) {
            throw DimensionalityMismatchEx(p.getDims(), __dimensionality);
        }
        LNodePtr cursor = __points;
        if(__size == 0) {
            assert(__points == nullptr);
            return false;
        }
        if(__points->point == p) {
            return true;
        }
        else {
            for(cursor; cursor->next != nullptr; cursor = cursor->next) {
                if(cursor->point == p) {
                    return true;
                }
            }
        }
        return false;
    }
    void Cluster::pickCentroids(unsigned int k, Point **pointArray) {
        try {
            if (k >= __size) {
                for (int i = 0; i < k; i++) {
                    if (i < __size) {
                        *pointArray[i] = (*this)[i];
                    }
                    else {
                        for (int j = 0; j < pointArray[i]->getDims(); j++) {
                            pointArray[i][j] = std::numeric_limits<double>::max();
                        }
                    }
                }
            }
            else {
                double x = static_cast<double>(__size) / k;
                for (int i = 0; i < k; i++) {
                    *pointArray[i] = (*this)[static_cast<int>(i * x)];
                }
            }
        }
        catch(ZeroDimensionsEx) {
            std::cout << ZeroDimensionsEx();
        }
    }
    const Point& Cluster::operator[](unsigned int u) const {
        if(__size == 0) {
            throw EmptyClusterEx();
        }
        if(u >= __size) {
            throw OutOfBoundsEx(u, __size);
        }
        LNodePtr cursor = __points;
        for(int i = 0; i < u; i++) {
            cursor = cursor->next;
        }
        return cursor->point;
    }
    std::ostream &operator<<(std::ostream &os, const Cluster &c) {
        for(int i = 0; i < c.__size; i++) {
            os << c[i] << Cluster::POINT_CLUSTER_ID_DELIM << c.__id << std::endl;
        }
        return os;
    }
    std::istream &operator>>(std::istream &is, Cluster &c) {
        unsigned int size;
        std::string line;
        while (std::getline(is, line)) {
            if (line == "") {
                break;
            }
            size = std::count(line.begin(), line.end(), Point::POINT_VALUE_DELIM);
            std::stringstream ss(line);

            Point p(size + 1);
            ss >> p;
            c.add(p);
        }
        return is;
    }
    bool operator==(const Cluster &lhs, const Cluster &rhs) {
        if(lhs.__dimensionality != rhs.__dimensionality) {
            throw DimensionalityMismatchEx(lhs.__dimensionality, rhs.__dimensionality);
        }
        if(lhs.__id != rhs.__id) {
            return false;
        }
        if(lhs.__size != rhs.__size) {
            return false;
        }
        for(int i = 0; i < lhs.__size; i++) {
            if(lhs[i] == rhs[i]) {
                break;
            }
            else {
                return false;
            }
        }
        return true;
    }
    bool operator!=(const Cluster &lhs, const Cluster &rhs) {
        return !(lhs == rhs);
    }
    Cluster& Cluster::operator+=(const Point &rhs) {
        if(this->contains(rhs)) {
            return *this;
        }
        add(rhs);
        return *this;
    }
    Cluster& Cluster::operator-=(const Point &rhs) {
        if(!this->contains(rhs)) {
            return *this;
        }
        remove(rhs);
        return *this;
    }
    Cluster& Cluster::operator+=(const Cluster &rhs) {
        for(LNodePtr cursor = rhs.__points; cursor != nullptr; cursor = cursor->next) {
            if(!this->contains(cursor->point)) {
                this->add(cursor->point);
            }
        }
        return *this;
    }
    Cluster& Cluster::operator-=(const Cluster &rhs) {
        for(LNodePtr cursor = rhs.__points; cursor != nullptr; cursor = cursor->next) {
            if(this->contains(cursor->point)) {
                this->remove(cursor->point);
            }
        }
        return *this;
    }
    const Cluster operator+(const Cluster &lhs, const Cluster &rhs) {
        Cluster sum(lhs);
        sum += rhs;
        return sum;
    }
    const Cluster operator-(const Cluster &lhs, const Cluster &rhs) {
        Cluster difference(rhs);
        difference -= lhs;
        return difference;
    }
    const Cluster operator+(const Cluster &lhs, const Point &rhs) {
        if(lhs.__dimensionality != rhs.getDims()) {
            throw DimensionalityMismatchEx(lhs.__dimensionality, rhs.getDims());
        }
        Cluster sum(lhs);
        sum += rhs;
        return sum;
    }
    const Cluster operator-(const Cluster &lhs, const Point &rhs) {
        if(lhs.__dimensionality != rhs.getDims()) {
            throw DimensionalityMismatchEx(lhs.__dimensionality, rhs.getDims());
        }
        Cluster difference(lhs);
        difference -= rhs;
        return difference;
    }
    Cluster::Move::Move(const Point &p, Cluster &from, Cluster &to) : __p(p), __from(from), __to(to) {}
    void Cluster::Move::perform() {
        __from.remove(__p);
        __to.add(__p);
    }
}