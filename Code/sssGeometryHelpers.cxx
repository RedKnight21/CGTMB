#include "sssGeometryHelpers.h"

#include <iostream>
#include <cfloat>
#include <cmath>

// ------------------------------------------------------------------------------------------------
void findThetaPhi(double *dist, double &theta, double &phi)
{
    theta = dist[0] * dist[0]  +  dist[1] * dist[1];

//  printf("findThetaPhi:: theta: %f\n", theta);
//  printf("dist: %f  %f  %f\n", dist[0], dist[1], dist[2]);

    if (theta > 0.00001)
    {
        theta    = sqrt( theta );
        phi      = acos( dist[0] / theta );
//  printf("1 phi: %f\n", phi);


        if (fabs(dist[2]) > 0.00001)
            theta    = atan( theta / dist[2]);
        else
        {
            theta *= theta;
            theta += (dist[2] * dist[2]);
            theta  = acos( dist[2] / theta );
        }
    }
    else
    {
        if (fabs(dist[0]) < 0.0001) phi = 0.0;
        else
            phi   = atan( dist[1] / dist[0] );

//  printf("2 phi: %f\n", phi);

        theta += (dist[2] * dist[2]);
        theta  = sqrt(theta);
        double c1 = acos(0.0);
        c1 = dist[2] / theta;

//  printf("findThetaPhi:: dist[2]: %f  theta: %f\n", dist[2], theta);

        theta = acos(c1);
    }
}

// ------------------------------------------------------------------------------------------------
void rotateToGlobal(double *pt, double phi, double theta)
{
    // Pass in reference to point to be transformed back to global space.
    // Point must already be adjusted by center of object.  i.e., so that
    // the object is at the origin.

    double sinphi, cosphi;
    double sintheta, costheta;
    double x, y, z;

    sinphi   = sin(phi);
    cosphi   = cos(phi);
    sintheta = sin(theta);
    costheta = cos(theta);

//  printf("\npt: %e  %e  %e\n", pt[0], pt[1], pt[2]);
//  printf("theta: %e   costheta: %e  sintheta: %e\n", theta, costheta, sintheta);
//  printf("phi  : %e   cosphi  : %e  sinphi  : %e\n", phi, cosphi,   sinphi);


    // Rotate about Y-axis first.
    x = (pt[0] * costheta + pt[2] * sintheta);
    y =  pt[1];
    z = (pt[2] * costheta - pt[0] * sintheta);

//	printf("x: %e  y: %e  z: %e \n", x,y, z);

    // Rotate about Z-axis last.
    pt[0] = (x * cosphi - y * sinphi);
    pt[1] = (y * cosphi + x * sinphi);
    pt[2] = z;

    if (fabs(pt[0]) < 1.0E-10) pt[0] = 0.0;
    if (fabs(pt[1]) < 1.0E-10) pt[1] = 0.0;
    if (fabs(pt[2]) < 1.0E-10) pt[2] = 0.0;
}


// ----------------------------------------------------------------------------

bool   linesIntersect(double *p1, double *d1, double *p2, double *d2, double *pt)
{
    bool ans;
    ans = false;

    double denom, mu;


    if (fabs(d1[1] + d2[1]) > 1.0E-10)
    {
        denom = (d2[0] * d1[1] - d2[1] * d1[0]);

        mu    = d1[1] * (p1[0] - p2[0]) + d1[0] * (p2[1] - p1[1]);
        mu   /= denom;
        pt[0] = p2[0] + mu * d2[0];
        pt[1] = p2[1] + mu * d2[1];
        pt[2] = p2[2] + mu * d2[2];
        ans   = true;
    }
    else if (fabs(d1[2] + d2[2]) > 1.0E-10)
    {
        denom = (d2[0] * d1[2] - d2[2] * d1[0]);

        mu    = d1[2] * (p1[0] - p2[0]) + d1[0] * (p2[2] - p1[2]);
        mu   /= denom;
        pt[0] = p2[0] + mu * d2[0];
        pt[1] = p2[1] + mu * d2[1];
        pt[2] = p2[2] + mu * d2[2];
        ans   = true;
    }

    return ans;
}












