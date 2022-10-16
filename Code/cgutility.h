#ifndef CGUTILITY_H
#define CGUTILITY_H


#include <vector>
#include <cstdio>
#include "cg_constants.h"
#include <cfloat>
#include <cmath>
#include <algorithm>
#include <QVector3D>
#include <QVector4D>
#include "data_structs.h"

using namespace std;


void    rotateToMaster(XYZ2 &pt, double phi, double theta);
void    rotateToGlobal(XYZ2 &pt, double phi, double theta);
void    rotateToGlobal(QVector3D &pt, double phi, double theta);

double  crossProduct(double *dir1, double *dir2, double *dir3);
bool    linesIntersect(XYZ2 p1, XYZ2 d1, XYZ2 p2, XYZ2 d2, XYZ2 &pt);
void    rccEndCap(const double theta, const double phi, const double height, QVector4D &peq);

double vectorLength(double *dir1);
double vectorLength(XYZ2 &a, XYZ2 &b);
double boxCorners(double *v1, double *a1, double *a2, double *a3, double *corners);

double sign(double x, double y);

int quart(int n, double *c, double *r4, int mm);
void torus(int &nr4, double xxx, double yyy, double zzz, double *wt,
           double *r4, vector<double> &bodyData);

/*
class cgutility
{
public:
    cgutility();
};
*/

#endif // CGUTILITY_H
