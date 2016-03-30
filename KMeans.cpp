//
// Created by Mario on 3/28/2016.
//

#include "KMeans.h"
#include "Cluster.h"
#include "Exceptions.h"
#include "Point.h"
#include <fstream>
namespace Clustering {
    KMeans::KMeans(unsigned int dim, unsigned int k, std::string filename, unsigned int maxIter) {
        if(k == 0) {
            throw ZeroClustersEx();
        }

        __dimensionality = dim;
        __k = k;
        __iFileName = filename;
        __maxIter = maxIter;
        for(int i = 0; i < dim; i++) {
            __clusters[i] = new Cluster(dim);
        }
    }
    //destructor
    KMeans::~KMeans() {

    }
    unsigned int KMeans::getMaxIter() {
        return __maxIter;
    }
    unsigned int KMeans::getNumIters() {
        return __numIter;
    }
    unsigned int KMeans::getNumMovesLastIter() {
        return __numMovesLastIter;
    }
    unsigned int KMeans::getNumNonemptyClusters() {
        return __numNonempty;
    }
    void KMeans::run() {

    }
    Cluster& KMeans::operator[](unsigned int u) {
        return *__clusters[u];
    }
    const Cluster& KMeans::operator[](unsigned int u) const {
        return *__clusters[u];
    }
    std::ostream& operator<<(std::ostream &os, const KMeans &kmeans) {

    }
}
