#include "cgutility.h"
#include <QDebug>

// ------------------------------------------------------------------------------------------------
void rotateToMaster(XYZ2 &pt, double phi, double theta)
{
    // Pass in reference to point to be transformed back to master space.
    // Point must already be adjusted by center of object.  i.e., so that
    // the object is at the origin.

    double sinphi, cosphi;
    double sintheta, costheta;
    double x, y, z;

    sinphi   = sin(phi);
    cosphi   = cos(phi);
    sintheta = sin(theta);
    costheta = cos(theta);

    // Rotate about Z-axis first.
    x = (pt.x * cosphi + pt.y * sinphi);
    y = (pt.y * cosphi - pt.x * sinphi);
    z = pt.z;

    // Rotate about Y-axis last.
    pt.x = (x * costheta - z * sintheta);
    pt.y =  y;
    pt.z = (z * costheta + x * sintheta);
}

// ------------------------------------------------------------------------------------------------
void rotateToGlobal(XYZ2 &pt, double phi, double theta)
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

    // Rotate about Y-axis first.
    x = (pt.x * costheta + pt.z * sintheta);
    y =  pt.y;
    z = (pt.z * costheta - pt.x * sintheta);

    // Rotate about Z-axis last.
    pt.x = (x * cosphi - y * sinphi);
    pt.y = (y * cosphi + x * sinphi);
    pt.z = z;
}


// ------------------------------------------------------------------------------------------------
void rotateToGlobal(QVector3D &pt, double phi, double theta)
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

    // Rotate about Y-axis first.
    x = (pt.x() * costheta + pt.z() * sintheta);
    y =  pt.y();
    z = (pt.z() * costheta - pt.x() * sintheta);

    // Rotate about Z-axis last.
    pt.setX(x * cosphi - y * sinphi);
    pt.setY(y * cosphi + x * sinphi);
    pt.setZ(z);
}


// ----------------------------------------------------------------------------
double vectorLength(XYZ2 &a, XYZ2 &b)
{
    double x = a.x - b.x;
    double y = a.y - b.y;
    double z = a.z - b.z;

    double len = x*x + y*y + z*z;
    len = sqrt(len);
    return len;
}

// ----------------------------------------------------------------------------
double vectorLength(double *dir1)
{
    double x = dir1[0] * dir1[0] + dir1[1] * dir1[1] + dir1[2] * dir1[2];
    x = sqrt(x);
    return x;
}
// ----------------------------------------------------------------------------
double boxCorners(double *v1, double *a1, double *a2, double *a3, double *corners)
{
    // Calculate the XYZ locations for the 8 corners making up a box body.
    corners[0] = v1[0];
    corners[1] = v1[1];
    corners[2] = v1[2];

    corners[3] = v1[0] + a1[0];
    corners[4] = v1[1] + a1[1];
    corners[5] = v1[2] + a1[2];

    corners[6] = v1[0] + a2[0];
    corners[7] = v1[1] + a2[1];
    corners[8] = v1[2] + a2[2];

    corners[ 9] = corners[6] + a1[0];
    corners[10] = corners[7] + a1[1];
    corners[11] = corners[8] + a1[2];

    corners[12] = corners[0] + a3[0];
    corners[13] = corners[1] + a3[1];
    corners[14] = corners[2] + a3[2];

    corners[15] = corners[3] + a3[0];
    corners[16] = corners[4] + a3[1];
    corners[17] = corners[5] + a3[2];

    corners[18] = corners[6] + a3[0];
    corners[19] = corners[7] + a3[1];
    corners[20] = corners[8] + a3[2];

    corners[21] = corners[ 9] + a3[0];
    corners[22] = corners[10] + a3[1];
    corners[23] = corners[11] + a3[2];
}



// ----------------------------------------------------------------------------
double crossProduct(double *dir1, double *dir2, double *dir3)
{
    // Return the sine of the angle from the cross product of two vectors,
    // and return the new vector.

    // Cross product
    dir3[0]  = dir1[1] * dir2[2] - dir1[2] * dir2[1];
    dir3[1]  = dir1[2] * dir2[0] - dir1[0] * dir2[2];
    dir3[2]  = dir1[0] * dir2[1] - dir1[1] * dir2[0];

    // Vector magnitudes;
    int i;
    double m1, m2, m3;
    m1 = m2 = m3 = 0.0;

    for (i=0; i<3; ++i)
    {
        m1  += (dir1[i] * dir1[i]);
        m2  += (dir2[i] * dir2[i]);
        m3  += (dir3[i] * dir3[i]);
    }
    m1 = sqrt(m1);
    m2 = sqrt(m2);
    m3 = sqrt(m3);

    double pi = acos(-1.0);

    // Sine
    double x = m3 / (m1 * m2);
    // To prevent issues with asin when x is slightly > 1.
    if (fabs(1.0 - x) < 0.00001) x = 1.0;

    int ii = 0;
    for (i=0; i<3; ++i)
    {
        if (dir3[i] >= 0) ++ii;
        if (dir3[i] < 0) --ii;
    }

    if (ii > 0) x = asin(x);
    else x = asin(-x);

    x = x * 180. / pi;

    dir3[0] /= m3;
    dir3[1] /= m3;
    dir3[2] /= m3;

    return -x;
}
// ----------------------------------------------------------------------------
bool linesIntersect(XYZ2 p1, XYZ2 d1, XYZ2 p2, XYZ2 d2, XYZ2 &pt)
{
    bool ans;
    ans = false;

    double denom, mu;


    if (fabs(d1.y + d2.y) > 1.0E-10)
    {
        denom = (d2.x * d1.y - d2.y * d1.x);

        mu  = d1.y * (p1.x - p2.x) + d1.x * (p2.y - p1.y);
        mu /= denom;
        pt.x = p2.x + mu * d2.x;
        pt.y = p2.y + mu * d2.y;
        pt.z = p2.z + mu * d2.z;
        ans  = true;
    }
    else if (fabs(d1.z + d2.z) > 1.0E-10)
    {
        denom = (d2.x * d1.z - d2.z * d1.x);

        mu  = d1.z * (p1.x - p2.x) + d1.x * (p2.z - p1.z);
        mu /= denom;
        pt.x = p2.x + mu * d2.x;
        pt.y = p2.y + mu * d2.y;
        pt.z = p2.z + mu * d2.z;
        ans  = true;
    }


    return ans;
}
// ----------------------------------------------------------------------------
void rccEndCap(const double theta, const double phi, const double height, QVector4D &peq)
{
    QVector3D pt0, pt1, pt2, cp, v1, v2, nm;

    pt0.setX(0.0);
    pt0.setY(0.0);
    pt0.setZ(height);

    pt1 = pt0;
    pt2 = pt0;

    if (fabs(height) > 1.E-10)
    {
        pt1.setX(height);
        pt2.setY(height);
    }
    else
    {
        pt1.setX(10.0);
        pt2.setY(10.0);
    }

    // Rotate each point.
    rotateToGlobal(pt0, phi, theta);
    rotateToGlobal(pt1, phi, theta);
    rotateToGlobal(pt2, phi, theta);

    // Translate rotated points to the RCC's origin.
    float a;
    a = pt0.x() + peq.x();
    pt0.setX(a);
    a = pt0.y() + peq.y();
    pt0.setY(a);
    a = pt0.z() + peq.z();
    pt0.setZ(a);

    a = pt1.x() + peq.x();
    pt1.setX(a);
    a = pt1.y() + peq.y();
    pt1.setY(a);
    a = pt1.z() + peq.z();
    pt1.setZ(a);

    a = pt2.x() + peq.x();
    pt2.setX(a);
    a = pt2.y() + peq.y();
    pt2.setY(a);
    a = pt2.z() + peq.z();
    pt2.setZ(a);

    // Create the vectors in the plane.
    v1 = pt1 - pt0;
    v2 = pt2 - pt0;

    cp = QVector3D::crossProduct(v1, v2);
    nm = QVector3D::normal(v1, v2);

    peq = cp;
    float w =  peq.x() * (v1.x() - pt0.x()) + peq.y() * (v1.y() - pt0.y()) + peq.z() * (v1.z() - pt0.z());

    peq.setW(-w);
}
// ----------------------------------------------------------------------------
double sign(double x, double y)
{
    double z;
    if (y >= 0.0) z = fabs(x);
    else z = -fabs(x);
    return z;
}

// ----------------------------------------------------------------------------
int quart(int n, double *c, double *r4, int mm)
{
    // FUNCTION
    //     USES THE PROCEDURE OF LA-4299 TO SOLVE THE QUARTIC EQUATION
    //     X**4+B*X**3+C*X**2+D*X+E=0  (C(I),I=1,5)=1,B,C,D,E
    //     THE JJ REAL ROOTS ARE RETURNED IN R4.  THE ROOTS WITH ODD
    //     MULTIPLICITY ARE LISTED FIRST, IN ASCENDING ORDER.  if MM=0,
    //     THE ROOTS WITH EVEN MULTIPLICITY ARE OMITTED.

    double th = 1.0 / 3.0;
    double c1ov16 = 0.0625;
    double c1em10 = 1.0e-10;
    double c1em8  = 1.0e-8;
    double pf = 2.0 * M_PI / 3.0;
    double tp[16];
    double temp12, tmp12, tmptp5, tmpt5, x;
    int jj;

    jj = 0;
    if (n == 3) goto label90;

    // LET Y=X+B/4 TO REDUCE THE QUARTIC EQUATION TO
    // Y**4 + Q*Y**2 + R*Y + S = 0  (TP(I), I=1,5) = B/2, B/4, Q, R, S
    tp[0] = 0.5 * c[1];
    tp[1] = 0.25 * c[1];
    tp[2] = c[2] - 1.5 * tp[0] * tp[0];
    tp[3] = c[3] + tp[0] * (tp[0] * tp[0] - c[2]);
    tp[4] = c[4] - c1ov16 * tp[0] * (5.0 * c[3] - c[2] * tp[0] + 3.0 * tp[3]);

//    qDebug() << "\t tp[0]: " << tp[0];
//    qDebug() << "\t tp[1]: " << tp[1];
//    qDebug() << "\t tp[2]: " << tp[2];
//    qDebug() << "\t tp[3]: " << tp[3];
//    qDebug() << "\t tp[4]: " << tp[4];

    // STEPS 2-7 (LA-4299) (TP(I), I=6,11) = B/3, C, D, P/3, Q/2, W
    tp[5] = 2.0 * th * tp[2];
    tp[6] = tp[2] * tp[2] - 4.0 * tp[4];
    tp[7] = -tp[3] * tp[3];
    tp[8] = th * tp[6]- tp[5] * tp[5];
    tp[9] = 0.5 * (tp[7] - tp[5] * (tp[6] - 2.0 * tp[5] * tp[5]));
    tp[10] = tp[8] * tp[8] * tp[8] + tp[9] * tp[9];

    tmp12 = fabs(tp[8] * tp[8] * tp[8]);
    temp12 = tp[9] * tp[9];
    x = std::max(tmp12, temp12);

    if (fabs(tp[10])  <=  c1em10 * x) goto label10;
    if (tp[10] > 0.) goto label30;

    tp[11] = sqrt(-tp[8]);
    if (tp[5] >= 0.0  ||  tp[6] <= 0.0)  return jj;

    tp[12] = -tp[9] / (tp[11] * tp[11] * tp[11]);
    if (fabs(tp[12]) >= 1.0)  goto label10;

    tp[12] = acos(tp[12]);
    tp[13] = 2.0 * tp[11] * cos(th * tp[12]) - tp[5];
    if (tp[13] < c1em10 * fabs(tp[5]))  goto label40;

    tp[14] = 0.5 * sqrt(tp[13]);
    tp[13] = -tp[13] - 3.0 * tp[5];
    tp[15] = tp[3] / tp[14];
    if (fabs(tp[15]) >= tp[13]) goto label10;

    jj = 4;
    tp[12] = 0.5 * sqrt(tp[13] + tp[15]);
    r4[0]  = -tp[12] - tp[14] - tp[1];
    r4[1]  =  tp[12] - tp[14] - tp[1];
    tp[12] = 0.5 * sqrt(tp[13] - tp[15]);
    r4[2]  = tp[14] - tp[12] - tp[1];
    r4[3]  = tp[14] + tp[12] - tp[1];
    return jj;

    label10:
//    qDebug() << "quart:: label10:";

    // STEPS 8-11 (LA-4299) DOUBLE OR TRIPLE ROOTS.
    x = sqrt(fabs(tp[8]));
    tp[11] = sign(x, tp[9]);
    tp[12] = -2.0 * tp[11] - tp[5];
    tp[13] = tp[11] - tp[5];
    x = 0.5 * sqrt(fabs(tp[12]));
    tp[14] = sign(x, tp[3]);

//    qDebug() << "\t tp[11]: " << tp[11];
//    qDebug() << "\t tp[12]: " << tp[12];
//    qDebug() << "\t tp[13]: " << tp[13];
//    qDebug() << "\t tp[14]: " << tp[14];


    if (tp[13] <= 0.0) goto label20;

    tp[15] = sqrt(tp[13]);
    jj     = 2;
    r4[0]  = -tp[14] - tp[1]  - tp[15];
    r4[1]  =  tp[15] - tp[14] - tp[1];
    if (mm == 0)      return jj;
    if (tp[9] == 0.0) return jj;
    jj    = 3;
    r4[2] = tp[14] - tp[1];
    return jj;

    label20:
//    qDebug() << "quart:: label20:";

    if (mm == 0) return jj;
    jj    = 1;
    r4[0] = -tp[14] -tp[2];

    label30:
//    qDebug() << "quart:: label30:";

    // STEP 12 (LA-4299)
    tp[11] = -tp[9] - sign(sqrt(tp[10]), tp[9]);
    x = fabs(tp[11]);
    temp12 = pow(x, th);
    tmp12  = tp[11];
    tp[11] = sign(temp12, tmp12);
    tp[13] = tp[11] - tp[8] / tp[11] - tp[5];
    if (tp[13] < c1em10 * fabs(tp[5])) goto label40;

    tp[14] = 0.5 * sign(sqrt(tp[13]), tp[3]);
    tp[15] = fabs(tp[3] / tp[14]) - tp[13] - 3.0 * tp[5];
    if (tp[15] <= 0.0) return jj;

    tp[15] = 0.5 * sqrt(tp[15]);
    jj = 2;
    r4[0] = -tp[15] - tp[14] - tp[1];
    r4[1] =  tp[15] - tp[14] - tp[1];
    return jj;

    label40:
//    qDebug() << "quart:: label40:";

    // STEPS 13-24 (LA-4299) TRIVIAL CASE; R = TP(4) = 0
    tp[5] = 0.5 * tp[2];
    x     = tp[5] * tp[5];
    tp[6] = x - tp[4];
    tp[8] = c1em8 * std::max(x, fabs(tp[4]));
    if (tp[6] <= tp[8])  goto label60;

    tp[7]  = sqrt(tp[6]);
    tp[15] = tp[7] - tp[5];
    if (tp[15] <= 0.0) goto label70;

    tp[14] = -tp[5] - tp[7];
    tp[15] = sqrt(tp[15]);
    if (tp[14] > 0.0) goto label50;

    jj = 2;

//    qDebug() << "tp[ 1] : "<< tp[1];
//    qDebug() << "tp[15] : "<< tp[15];

    r4[0] = -tp[15] - tp[1];
    r4[1] =  tp[15] - tp[1];
    if (mm == 0) return jj;
    if (tp[14] /= 0.0) return jj;

    jj = 3;
    r4[2] = -tp[1];
    return jj;

    label50:
//    qDebug() << "quart:: label50:";

    tp[14] = sqrt(tp[14]);
    jj     = 4;
    r4[0]  = -tp[15] - tp[1];
    r4[1]  = -tp[14] - tp[1];
    r4[2]  =  tp[14] - tp[1];
    r4[3]  =  tp[15] - tp[1];
    return jj;

    label60:
//    qDebug() << "quart:: label60:";

    if (mm == 0) return jj;
    if (tp[6] < -tp[8]  ||  tp[5] > 0.0) return jj;
    if (tp[5] == 0.0) goto label80;

    jj     =  2;
    tp[15] =  sqrt(-tp[5]);
    r4[0]  =  tp[15] - tp[1];
    r4[1]  = -tp[15] - tp[1];
    return jj;

    label70:
//    qDebug() << "quart:: label70:";

    if (mm == 0)      return jj;
    if (tp[15] < 0.0) return jj;

    label80:
//    qDebug() << "quart:: label180:";

    jj    = 1;
    r4[0] = -tp[1];
    return jj;


    label90:
//    qDebug() << "quart:: label90:";

    // CUBIC EQUATION.  X**3+B*X**2+C*X+D=0  (C(I), I=1,4) = 1, B, C, D
    // LET Y=X+B/3.  then  Y**3+P*Y+Q=0   (TP(I), I=1,4) = B/3, P/3, Q/2, W
    tp[0] = th * c[1];
    tp[1] = th * c[2] - tp[0] * tp[0];
    tp[2] = 0.5 * (c[3] - tp[0] * (c[2] - 2.0 * tp[0] * tp[0]));
    tp[3] = tp[1] * tp[1] * tp[1] + tp[2] * tp[2];
    if (tp[3] >  0.0) goto label110;
    if (tp[3] == 0.0) goto label100;

    // CASE I (LA-4299)  3 DISTINCT ROOTS
    tp[4] = 2.0 * sqrt(-tp[1]);
    tp[5] = 8.0 * tp[2] / (tp[4] * tp[4] * tp[4]);
    if (fabs(tp[5]) >= 1.0) goto label100;
    tp[6] = th * acos(-tp[5]);
    r4[0] = tp[4] * cos(pf + tp[6]) - tp[0];
    r4[1] = tp[4] * cos(pf - tp[6]) - tp[0];
    r4[2] = tp[4] * cos(tp[6]) - tp[0];

    jj = 3;
    return jj;

    label100:
//    qDebug() << "quart:: label100:";

    // CASE II (LA-4299) 1 DISTINCT AND 1 DOUBLE ROOT
    r4[0] = -2.0 * sign(sqrt(-tp[1]), tp[2]) - tp[0];
    jj = 1;
    if (mm == 0) return jj;
    if (r4[0] == 0.0) return jj;
    jj = 2;
    r4[1] = -0.5 * r4[0];
    return jj;

    label110:
//    qDebug() << "quart:: label110:";

    // CASE III (LA-4299)  1 DISTINCT ROOT
    tp[4] = -tp[2] - sign( sqrt(tp[3]), tp[2] );
    tmptp5 = fabs(tp[4]);
    tmptp5 = pow(tmptp5, th);
    tmpt5  = tp[4];
    tp[4]  = sign(tmptp5, tmpt5);
    r4[0]  = tp[4] - tp[1] / tp[4] - tp[0];
    jj     = 1;
    return jj;
}                   // -- end quart

// ----------------------------------------------------------------------------
void torus(int &nr4, double xxx, double yyy, double zzz, double *wt,
           double *r4, vector<double> &bodyData)
{
    double uu[5], xx[5], tpp[6];
    double t1, t2, t3, pvh, wh, pvw, z0, ecentq, x;
    double c1em16 = 1.0E-16;
    int n, jj, m, nr;

    // TRANSFORM TO BODY-BASED COORDINATES:
    // PV = P - V
    xx[0] = xxx - bodyData[0];
    xx[1] = yyy - bodyData[1];
    xx[2] = zzz - bodyData[2];

    pvh = xx[0] * bodyData[3] + xx[1] * bodyData[4] + xx[2] * bodyData[5];
    wh  = wt[0] * bodyData[3] + wt[1] * bodyData[4] + wt[2] * bodyData[5];
    pvw = xx[0] * wt[0] + xx[1] * wt[1] + xx[2] * wt[2];

    // Note these assume H is a unit vector:
    tpp[0] = wh * wh;
    tpp[1] = 1.0 - tpp[0];
    tpp[2] = 1.0;
    tpp[3] = bodyData[10];

    // In the following two definitions, it is more convenient
    // to use body-independent coordinates.
    tpp[4] = 2.0 * (pvw - wh * pvh);
    tpp[5] = xx[0] * xx[0] + xx[1] * xx[1] + xx[2] * xx[2] - pvh * pvh;

    t1 = 2.0 * pvw;
    t2 = xx[0] * xx[0] + xx[1] * xx[1] + xx[2] * xx[2] + bodyData[11];

//    qDebug() << "torus:: nr4: " << nr4;
//    qDebug() << "xx: " << xx[0] << xx[1] << xx[2];
//    qDebug() << "wt: " << wt[0] << wt[1] << wt[2];

/*
    // ------------------------------------------------------------------------
    // For elliptical cross-section:
    ecentq  = bodyData[8] / bodyData[7];
    ecentq *= ecentq;
    z0  = tpp[1] + tpp[0] * ecentq;
    z0 *= z0;
    tpp[2] = 1.0 / z0;
    z0 = 2.0 + bodyData[6] + tpp[2];
    tpp[3] = z0 * z0;
    tpp[4] = 2.0 * (pvw - wh * pvh);
    tpp[5] = xx[0] * xx[0] + xx[1] * xx[1] + xx[2] * xx[2] - pvh * pvh;

    z0 = tpp[4] + 2.0 * wh * pvh * ecentq;
    t1 = z0 * z0 * tpp[2];

    z0  = ecentq * pvh * pvh;
    t2  = tpp[5] + z0 + bodyData[6] * bodyData[6] - bodyData[8] * bodyData[8];
    t2 *= tpp[2];
    // ------------------------------------------------------------------------
*/
    // SET UP THE QUARTIC COEFFICIENTS.  (UU(I), I=1,5)=  1, B, C, D, E
    uu[0] = 1.0;
    uu[1] = 2.0 * t1;
    uu[2] = t1 * t1 + 2.0 * t2 - tpp[3] * tpp[1];
    uu[3] = uu[1] * t2 - tpp[3] * tpp[4];
    uu[4] = t2 * t2    - tpp[3] * tpp[5];
    n     = 4;

//    qDebug() << "\t uu[0]: " << uu[0];
//    qDebug() << "\t uu[1]: " << uu[1];
//    qDebug() << "\t uu[2]: " << uu[2];
//    qDebug() << "\t uu[3]: " << uu[3];
//    qDebug() << "\t uu[4]: " << uu[4];

    // ELIMINATE THE SMALLEST ROOT if THE PARTICLE IS SITTING ON
    // THE TORUS.  SOLVE CUBIC INSTEAD OF QUARTIC.
    // if (CUR_LSURF .EQ. J) N = 3
    //
    // THERE ARE NO POSITIVE ROOTS if NO COEFFICIENTS ARE NEGATIVE.
    for (int m=1; m<n; ++m)
    {
        if (uu[m] < 0.0) goto label20;
    }
    if (uu[n] >= 0.0) return;

    label20:
    //SOLVE THE QUARTIC OR CUBIC EQUATION FOR THE INTERSECTIONS.
    jj =  quart(n, uu, xx, 0);

//    qDebug() << "torus:: quart:: jj: " << jj;

    if (jj == 0) return;

    // ZERO IN ON LEGITIMATE POSITIVE ROOTS.
    nr4 = 0;
    for (m=0; m < jj; ++m)
    {
        bool flag = false;
        // ELIMINATE NEGATIVE ROOTS.
        if (xx[m] < 0.0) continue;

        // ELIMINATE ROOTS ON THE WRONG BRANCH OF A DEGENERATE TORUS.
        if ((t2 + xx[m] * (t1 + xx[m])) * bodyData[6] < 0.0) continue;

        z0 = c1em16 * (1.0 + fabs(xx[m]));
        for (nr = 0; nr < 20; ++nr)
        {
            x = 2.0 * uu[2] + xx[m] * (3.0 * uu[1] + 4.0 * xx[m]);
            t3 = uu[3] + xx[m] * x;
            if (t3 == 0.0)
            {
                printf("\n >>>>> WARNING: BAD QUARTIC SOLVER ROOT IN TORUS\n\n");
                flag = true;
                break;
            }
            x = uu[3] + xx[m] * (uu[2] + xx[m] * (uu[1] + xx[m]));
            t3 = (uu[4] + xx[m] * x) / t3;

            xx[m] = xx[m] - t3;
            if (fabs(t3) <= z0) break;
        }
        if (flag) continue;
        r4[nr4] = xx[m];
        ++nr4;
    }               // -- end m-loop

    return;
}

