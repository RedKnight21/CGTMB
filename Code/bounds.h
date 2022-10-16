#ifndef BOUNDS_H
#define BOUNDS_H

#include <cfloat>

struct Bounds
{
    Bounds()
    {
        nodesMinX = 0.0;
        nodesMaxX = 0.0;
        nodesMinY = 0.0;
        nodesMaxY = 0.0;
        nodesMinZ = 0.0;
        nodesMaxZ = 0.0;
    }

    void reset()
    {
        nodesMinX = 0.0;
        nodesMaxX = 0.0;
        nodesMinY = 0.0;
        nodesMaxY = 0.0;
        nodesMinZ = 0.0;
        nodesMaxZ = 0.0;
    }

    void setStartingLimits()
    {
        nodesMinX =  DBL_MAX;
        nodesMaxX = -DBL_MAX;
        nodesMinY =  DBL_MAX;
        nodesMaxY = -DBL_MAX;
        nodesMinZ =  DBL_MAX;
        nodesMaxZ = -DBL_MAX;
    }

    double  nodesMinX;
    double  nodesMaxX;
    double  nodesMinY;
    double  nodesMaxY;
    double  nodesMinZ;
    double  nodesMaxZ;
};

#endif // BOUNDS_H
