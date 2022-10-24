#ifndef SKDTREE_H
#define SKDTREE_H

#include <iostream>
#include <vector>
#include "unstructuredmesh.h"
#include "umDataStructures.h"
#include <sss_math_utilities.h>

class AlignedBox
{
    public:
        float _min, _max;
};

class SkdTree
{
    private:
        int             *mIndex;
        std::vector<int>     nodesFound;
        float           asign[2];
        float           gsign[4];
        FILE            *fp;

        std::vector<UM_t1element> *t1elements;
        std::vector<UM_p1element> *p1elements;
        std::vector<UM_h1element> *h1elements;
        std::vector<UM_t2element> *t2elements;
        std::vector<UM_p2element> *p2elements;
        std::vector<UM_h2element> *h2elements;
        std::vector<UM_nodes>     *nodes;

        void findNodeBox(Point &point, int node);
        bool isInside1stOrderHex(int hex, Point &point, Point &ghr, unsigned char eType, bool pFlag);
        bool isInside1stOrderTet(int tet, Point &point, Point &ghr, unsigned char eType);
        bool isInside1stOrderPent(int pent, Point &point, Point &ghr, unsigned char eType);

    public:
        size_t        number;
        int           *elAdder;
        int           *elLimits;
        int           *elAdjust;
        int           *ctNumbers1;
        int           *ctNumbers2;
        int           *elNumbers;
        int           *linkp;
        AlignedBox    *sbox;
        unsigned char *cutDir;
        std::vector<Bbox>  eBoxes;     // Bounding boxes for elements in this tree.
        std::vector<int>   eType;

        /*SkdTree(vector<UM_t1element> &t1, vector<UM_p1element> &p1, vector<UM_h1element> &h1,
                vector<UM_t2element> &t2, vector<UM_p2element> &p2, vector<UM_h2element> &h2,
                vector<UM_nodes>     &n,  int *limits, int *adder)
        {
            number   = 0;
            linkp    = NULL;
            sbox     = NULL;
            cutDir   = NULL;
            asign[0] = -1.0;  asign[1] = 1.0;
            gsign[0] = -1.0;  gsign[1] = 1.0;  gsign[2] = 1.0;  gsign[3] = -1.0;
            t1elements = t1;
            p1elements = p1;
            h1elements = h1;
            t2elements = t2;
            p2elements = p2;
            h2elements = h2;
            nodes      = n;
            elLimits   = limits;
            elAdder    = adder;
            for (int i = 0 ; i<6; i++)
            {
                qDebug() << "-- skdTree:: i: " << i << "limits: " << limits[i] << "\t elAdder: " << elAdder[i];
            }
        } */

        SkdTree(std::vector<UM_t1element> *t1, std::vector<UM_p1element> *p1, std::vector<UM_h1element> *h1,
                std::vector<UM_t2element> *t2, std::vector<UM_p2element> *p2, std::vector<UM_h2element> *h2,
                std::vector<UM_nodes>     *n,  int *ctN1, int *ctN2, int *elNum, int *adj)
        {
            number   = 0;
            linkp    = NULL;
            sbox     = NULL;
            cutDir   = NULL;
            asign[0] = -1.0;  asign[1] = 1.0;
            gsign[0] = -1.0;  gsign[1] = 1.0;  gsign[2] = 1.0;  gsign[3] = -1.0;
            t1elements = t1;
            p1elements = p1;
            h1elements = h1;
            t2elements = t2;
            p2elements = p2;
            h2elements = h2;
            nodes      = n;
            ctNumbers1 = ctN1;
            ctNumbers2 = ctN2;
            elNumbers  = elNum;
            elAdjust   = adj;


            //for (int i = 0 ; i<6; i++)
            //{
            //    qDebug() << "-- skdTree:: i: " << i << "ctNumbers2: " << ctNumbers2[i] << "\t elAdjust: " << elAdjust[i];
            //}
        }

        ~SkdTree()
        {
            if (linkp)
                delete [] linkp;
            if (sbox)
                delete [] sbox;
            if (cutDir)
                delete [] cutDir;
        }

        void printDebugLinkp();
        void printDebugCutDir();
        void printDebugSbox();
        void createTree(std::vector<Point> &centroids);
        void buildBranch(std::vector<Point> &centroids, int &nextp, int ileft, int iright, int jnode);
        unsigned char find_centroid_coord_extremes(std::vector<Point> &centroids, int blow, int bupp);
        void sort_coordinates(std::vector<Point> &xyz, int coord, int center, int lindx, int rindx);
        int findInstanceElement(Point &point, Point &ghr, int &gEl, unsigned char &eType);
        int findInstanceElement(Point &point, Point &ghr, int &gEl, unsigned char &eType, bool pFlag);
        int getGlobalTypeElementNumber(int localEl, unsigned char &iType, unsigned char &eType);

};

#endif // SKDTREE_H
