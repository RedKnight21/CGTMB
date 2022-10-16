#ifndef sss_geometry_helpers_h
#define sss_geometry_helpers_h

void   findThetaPhi(double *dist, double &theta, double &phi);
void   rotateToGlobal(double *pt, double phi, double theta);
bool   linesIntersect(double *p1, double *d1, double *p2, double *d2, double *pt);


#endif
