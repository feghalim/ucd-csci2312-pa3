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
        if (k == 0) {
            throw ZeroClustersEx();
        }

        __dimensionality = dim;
        __k = k;
        __iFileName = filename;
        __maxIter = maxIter;
        __numIter = 0;
        __numNonempty = 0;
        __numMovesLastIter = 0;

        std::ifstream inFile(__iFileName);

        if(!inFile) {
            throw DataFileOpenEx(__iFileName);
        }

        __clusters = new Cluster * [__k];
        for(int i = 0; i < __k; i++) {
            __clusters[i] = new Cluster(dim);
        }
        inFile >> *__clusters[0];
        inFile.close();

        if((*__clusters[0]).getSize() != 0) {
            __numNonempty++;
        }

        __initCentroids = new Point * [__k];
        for(int i = 0; i < __k; i++) {
            __initCentroids[i] = new Point(dim);
        }

        __clusters[0]->pickCentroids(__k, __initCentroids);

        for(int i = 0; i < k; i++) {
            __clusters[i]->centroid.set(*__initCentroids[i]);
        }
    }
    //destructor
    KMeans::~KMeans() {
        delete [] __clusters;
        delete [] __initCentroids;
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
        int moves = 100, iter = 0, closest = 0;
        bool mov = false;
        if(__k == 1) {
            return;
        }
        while (moves > 0 && iter < __maxIter) {
            moves = 0;
            //cluster iterate
            for(int i = 0; i < __k; i++) {
                //point iterate
                for(int k = 0; k < (*(__clusters)[i]).getSize(); k++) {
                    closest = i;

                    for(int j = 0; j < __k; j ++) {
                        if(i != k && ((*(__clusters)[i]).getSize() > 1) && (((*(__clusters)[i])[k].distanceTo((*(__clusters)[closest]).centroid.get())) > ((*(__clusters)[i]).getSize() > 1 && (((*(__clusters)[i])[k].distanceTo((*(__clusters)[j]).centroid.get())))))) {
                            closest = j;
                            mov = true;
                        }
                    }
                    if(mov) {
                        Cluster::Move move ((*(__clusters)[i])[k], (*(__clusters)[i]), (*(__clusters)[closest]));
                        move.perform();
                        moves++;
                        k--;
                        mov = false;
                    }
                }
            }
            for(int x = 0; x < __k; x++) {
                if(!(*(__clusters)[x]).centroid.isValid()) {
                    (*(__clusters)[x]).centroid.compute();
                }
            }
            iter++;
        }
        __numNonempty = 0;
        for(int n = 0; n < __k; n++) {
            if((*(__clusters)[n]).getSize() > 0) {
                __numNonempty++;
            }
        }

        __numIter = iter;
        __numMovesLastIter = moves;
    }
    Cluster& KMeans::operator[](unsigned int u) {
        return *__clusters[u];
    }
    const Cluster& KMeans::operator[](unsigned int u) const {
        return *__clusters[u];
    }
    std::ostream& operator<<(std::ostream &os, const KMeans &kmeans) {
        for(int i = 0; i < kmeans.__k; i++) {
            if((*(kmeans.__clusters)[i]).getSize() > 0) {
                for(int k = 0; k < (*(kmeans.__clusters)[i]).getSize(); k++) {
                    os << (*(kmeans.__clusters)[i])[k] << Cluster::POINT_CLUSTER_ID_DELIM << (*(kmeans.__clusters)[i]).getId() << std::endl;
                }
            }
        }
    }
}
