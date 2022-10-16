#ifndef GRIDCELL_H
#define GRIDCELL_H

#include <math.h>

class XYZ
{
  public:
   float x,y,z;
};

class TRIANGLE
{
  public:
   XYZ p[3];
};

class GRIDCELL
{
  private:
    void VertexInterp(float isolevel, XYZ p1, XYZ p2, float valp1, float valp2, XYZ &p);

  public:
   XYZ p[8];
   float val[8];
   int Polygonise(float isolevel, TRIANGLE *triangles);
};


#endif // GRIDCELL_H
