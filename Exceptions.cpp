//
// Created by Mario on 3/28/2016.
//

#include "Exceptions.h"

namespace Clustering {

    OutOfBoundsEx::OutOfBoundsEx(unsigned int c, int r) {
        __name = "OutOfBoundsEx";
        __current = c;
        __rhs = r;
    }

    int OutOfBoundsEx::getRhs() const {
        return __rhs;
    }

    std::string OutOfBoundsEx::getName() const {
        return __name;
    }

    unsigned int OutOfBoundsEx::getCurrent() const {
        return __current;
    }

    std::ostream &operator<<(std::ostream &os, const OutOfBoundsEx &ex) {
        os << ex.getName() << ": " << ex.getCurrent() << " is outside the range of " << ex.getRhs();
        return os;
    }

    DimensionalityMismatchEx::DimensionalityMismatchEx(unsigned int c, unsigned int r) {
        __name = "DimensionalityMismatchEx";
        __current = c;
        __rhs = r;
    }

    unsigned int DimensionalityMismatchEx::getCurrent() const {
        return __current;
    }

    std::string DimensionalityMismatchEx::getName() const {
        return __name;
    }

    unsigned int DimensionalityMismatchEx::getRhs() const {
        return __rhs;
    }

    std::ostream &operator<<(std::ostream &os, const DimensionalityMismatchEx &ex) {
        os << ex.getName() << ": " << ex.getCurrent() << " != " << ex.getRhs();
        return os;
    }

    ZeroClustersEx::ZeroClustersEx() {
        __name = "ZeroClustersEx";
    }

    std::string ZeroClustersEx::getName() const {
        return __name;
    }

    std::ostream &operator<<(std::ostream &os, const ZeroClustersEx &ex) {
        os << ex.getName() << ": " << " K = 0";
        return os;
    }

    DataFileOpenEx::DataFileOpenEx(std::string filename) {
        __name = "DataFileOpenEx";
        __filename = filename;
    }

    std::string DataFileOpenEx::getName() const {
        return __name;
    }

    std::string DataFileOpenEx::getFilename() const {
        return __filename;
    }

    std::ostream &operator<<(std::ostream &os, const DataFileOpenEx &ex) {
        os << ex.getName() << ": " << "could not find file of " << ex.getFilename();
        return os;
    }

    ZeroDimensionsEx::ZeroDimensionsEx() {
        __name = "ZeroDimensionsEx";
    }

    std::string ZeroDimensionsEx::getName() const {
        return __name;
    }

    std::ostream &operator<<(std::ostream &os, const ZeroDimensionsEx &ex) {
        os << ex.getName() << ": " << "dimension of point is zero";
        return os;
    }

    EmptyClusterEx::EmptyClusterEx() {
        __name = "EmptyClusterEx";
    }

    std::string EmptyClusterEx::getName() const {
        return __name;
    }

    std::ostream &operator<<(std::ostream &os, const EmptyClusterEx &ex) {
        os << ex.getName() << ": " << "Cluster is empty";
        return os;
    }
}