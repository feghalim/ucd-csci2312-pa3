//
// Created by Mario on 3/28/2016.
//

#include <limits>
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
        Cluster c2(c);
        return c2;
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
        LNodePtr cursor = __points->next;
        while(cursor != nullptr) {
            cursor = cursor->next;
        }
        cursor->next = new LNode(p, nullptr);
        __size++;
    }
    const Point& Cluster::remove(const Point &p) {
        if(p.getDims() != __dimensionality) {
            throw DimensionalityMismatchEx(p.getDims(), __dimensionality);
        }
        LNodePtr cursor = __points;
        int i;
        if(__points->point == p) {
            __points->next = __points->next->next;
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
        }
        return p;
    }
    bool Cluster::contains(const Point &p) const {
        LNodePtr cursor = __points;
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

    }
    const Point& Cluster::operator[](unsigned int u) const {
        if(u > __size) {
            throw OutOfBoundsEx(u, __size);
        }
        if(__size == 0) {
            throw EmptyClusterEx();
        }
        LNodePtr cursor = __points;
        for(int i = 0; i < u; i++) {
            cursor = cursor->next;
        }
        return cursor->point;
    }
    std::ostream &operator<<(std::ostream &os, const Cluster &c) {
        for(int i = 0; i < c.__size; i++) {
            os << c[i] << ", ";
        }
        os << " : " << c.__id;
        return os;
    }
    std::istream &operator>>(std::istream &is, Cluster &c) {

    }
    bool operator==(const Cluster &lhs, const Cluster &rhs) {
        if(lhs.__dimensionality != rhs.__dimensionality) {
            throw DimensionalityMismatchEx(lhs.__dimensionality, rhs.__dimensionality);
        }
        if(lhs.__id != rhs.__id) {
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
        add(rhs);
        return *this;
    }
    Cluster& Cluster::operator-=(const Point &rhs) {
        remove(rhs);
        return *this;
    }
    Cluster& Cluster::operator+=(const Cluster &rhs) {

    }
    Cluster& Cluster::operator-=(const Cluster &rhs) {

    }
    const Cluster operator+(const Cluster &lhs, const Cluster &rhs) {

    }
    const Cluster operator-(const Cluster &lhs, const Cluster &rhs) {

    }
    const Cluster operator+(const Cluster &lhs, const Point &rhs) {

    }
    const Cluster operator-(const Cluster &lhs, const Point &rhs) {

    }
    Cluster::Move::Move(const Point &p, Cluster &from, Cluster &to) : __p(p), __from(from), __to(to) {}
    void Cluster::Move::perform() {
        __to.add(__from.remove(__p));
    }
}