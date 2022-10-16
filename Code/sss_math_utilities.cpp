#include <sss_math_utilities.h>

// ------------------------------------------------------------------------------------------------
int matrixInverse3x3(float a[][3])
{
    int error;
    float   z;
    float   x[3][3];

    x[0][0] = a[1][1] * a[2][2] - a[1][2] * a[2][1];
    x[1][0] = a[1][2] * a[2][0] - a[1][0] * a[2][2];
    x[2][0] = a[1][0] * a[2][1] - a[1][1] * a[2][0];

    z = a[0][0] * x[0][0] + a[0][1] * x[1][0] + a[0][2] * x[2][0];

    if (z > 1.0e-8)
    {
        error = 0;
        z = 1.0 / z;
        x[0][0] = (a[1][1] * a[2][2] - a[1][2] * a[2][1]) * z;
        x[0][1] = (a[0][2] * a[2][1] - a[0][1] * a[2][2]) * z;
        x[0][2] = (a[0][1] * a[1][2] - a[0][2] * a[1][1]) * z;

        x[1][0] = (a[1][2] * a[2][0] - a[1][0] * a[2][2]) * z;
        x[1][1] = (a[0][0] * a[2][2] - a[0][2] * a[2][0]) * z;
        x[1][2] = (a[0][2] * a[1][0] - a[0][0] * a[1][2]) * z;

        x[2][0] = (a[1][0] * a[2][1] - a[1][1] * a[2][0]) * z;
        x[2][1] = (a[0][1] * a[2][0] - a[0][0] * a[2][1]) * z;
        x[2][2] = (a[0][0] * a[1][1] - a[0][1] * a[1][0]) * z;

        // Re-assign to output array.
        a[0][0] = x[0][0];
        a[0][1] = x[0][1];
        a[0][2] = x[0][2];

        a[1][0] = x[1][0];
        a[1][1] = x[1][1];
        a[1][2] = x[1][2];

        a[2][0] = x[2][0];
        a[2][1] = x[2][1];
        a[2][2] = x[2][2];
    }
    else
    {
        error = -1;
    }
    return error;
}
// ------------------------------------------------------------------------------------------------
void matMult3(float a3x3[][3], float b3[], float result[])
{
    int i, j;

    for (i = 0;  i < 3; i++)
    {
        //qDebug() << "-- matMult3 -- row is " << i;
        result[i] = 0.0;
        for (j = 0 ; j < 3; j++)
        {
            result[i] += a3x3[i][j] * b3[j];
            //qDebug() << "i: " << i << "j: " << j << "a3x3: " << a3x3[i][j] << "b3: " << b3[j] << "result: " << result[i];
        }
    }
}
// ------------------------------------------------------------------------------------------------
int searchBins(double &x, vector<double> &y, int aSize)
{
    int low, high, mid;
    low = 0; high = aSize-1;

    if (x < y[low])
    {
        return low;
    }
    if (x > y[high])
    {
        return high;
    }

    //qDebug() << "searchBins:: aSize: " << aSize;
    do
    {
        mid = (low + high) / 2;

        //qDebug() << "low: " << low << " mid: " << mid << "x: " << x << "\t" << y[low] << "\t"<< y[mid];
        if ((x >= y[low]) && (x <= y[mid]))
        {
            high = mid;
        }
        else if ((x > y[mid]) && (x <= y[high]))
        {
            low = mid;
        }

    } while (((high - low) > 1));

    if (low == 0)
    {
       if (x == y[low+1]) low++;
    }
    //else if (low == (aSize-1))
    //{
    //   if (x == y[low-1]) low--;
    //}

    return low;
}
// ------------------------------------------------------------------------------------

void getRotatedPoint(double *ap1, double *ap2, double angle, double *oldPt, double *newPt)
{
    // Locals
    float  a[3];          // axis unit vector
    float  oldVec[3];     // vector to point
    float  cosAng;        // cos of angle
    float  sinAng;        // sin of angle
    float  rotMat[3][3];  // rotation matrix
    float  newPt2[3];

    //qDebug() << "-- getRotatedPoint --";

    // Generate a unit vector from the axis points about
    // which the rotation takes place.
    a[0] = ap2[0] - ap1[0];
    a[1] = ap2[1] - ap1[1];
    a[2] = ap2[2] - ap1[2];

    // Generate a vector from the unit vector origin to the
    // old point (which is to be rotated).
    oldVec[0] = oldPt[0] - ap1[0];
    oldVec[1] = oldPt[1] - ap1[1];
    oldVec[2] = oldPt[2] - ap1[2];

    //qDebug() << "oldVec: " << oldVec[0] << "\t" << oldVec[1] << "\t" << oldVec[2];

    cosAng = cos(angle*M_PI/180.0);
    sinAng = sin(angle*M_PI/180.0);

    //qDebug() << " cosAng: " << cosAng << "\t sinAng" << sinAng;

    // Calculate the rotation matrix.
    rotMat[0][0] = cosAng + (1.0 - cosAng) * a[0] * a[0];
    rotMat[0][1] = (1.0 - cosAng) * a[0] * a[1] - sinAng * a[2];
    rotMat[0][2] = (1.0 - cosAng) * a[0] * a[2] + sinAng * a[1];

    rotMat[1][0] = (1.0 - cosAng) * a[0] * a[1] + sinAng * a[2];
    rotMat[1][1] = cosAng + (1.0 - cosAng) * a[1] * a[1];
    rotMat[1][2] = (1.0 - cosAng) * a[1] * a[2] - sinAng * a[0];

    rotMat[2][0] = (1.0 - cosAng) * a[0] * a[2] - sinAng * a[1];
    rotMat[2][1] = (1.0 - cosAng) * a[1] * a[2] + sinAng * a[0];
    rotMat[2][2] = cosAng + (1.0 - cosAng) * a[2] * a[2];

    //matMult3(rotMat, oldVec, (float *) newPt);
    matMult3(rotMat, oldVec, newPt2);

    //qDebug() << "newPt: " << newPt2[0] << "\t" << newPt2[1] << "\t" << newPt2[2];

    newPt[0] = newPt2[0] + ap1[0];
    newPt[1] = newPt2[1] + ap1[1];
    newPt[2] = newPt2[2] + ap1[2];
}
// ------------------------------------------------------------------------------------------------
void rotatePoint(double &x, double &y, double &z, double phi, double theta)
{
    // Point must already be adjusted by center of object.  i.e., so that
    // the object is at the origin.

    double sinphi, cosphi;
    double sintheta, costheta;
    double xx, yy, zz;

    sinphi   = sin(phi);
    cosphi   = cos(phi);
    sintheta = sin(theta);
    costheta = cos(theta);

    // Rotate about Z-axis first.
    xx = (x * cosphi + y * sinphi);
    yy = (y * cosphi - x * sinphi);
    zz = z;

    // Rotate about Y-axis last.
    x = (xx * costheta - zz * sintheta);
    y =  yy;
    z = (zz * costheta + xx * sintheta);
}

// ------------------------------------------------------------------------------------------------
// Function to remove leading and trailing white space from a given string

string trim(string str)
{
    const string WHITESPACE = " \n\r\t\f\v";
    string s = str;

    size_t p = s.find_first_not_of(WHITESPACE);
    s.erase(0, p);

    p = s.find_last_not_of(WHITESPACE);
    if (string::npos != p)
        s.erase(p+1);

    return s;
}

// ------------------------------------------------------------------------------------------------
double Max(vector<double> &x)
{
  size_t num = x.size();

  double Maximum = x[0];

  for(size_t i = 1; i < num; i++)
    if ( x[i] > Maximum )
      Maximum = x[i];

  return Maximum;
}
// ------------------------------------------------------------------------------------------------


double Min(vector<double> &x)
{
  size_t num = x.size();

  double Minimum = x[0];

  for(size_t i = 1; i < num; i++)
    if ( x[i] < Minimum )
      Minimum = x[i];

  return Minimum;
}
// ------------------------------------------------------------------------------------------------

double Max(double *x, int num)
{
  double Maximum = x[0];

  for(int i = 1; i < num; i++)
    if ( x[i] > Maximum )
      Maximum = x[i];

  return Maximum;
}
// ------------------------------------------------------------------------------------------------


double Min(double *x, int num)
{
  double Minimum = x[0];

  for(int i = 1; i < num; i++)
    if ( x[i] < Minimum )
      Minimum = x[i];

  return Minimum;
}

// ------------------------------------------------------------------------------------
double deter3x3(double (*a)[3])
{
    double det;

    det = a[0][0]*((a[1][1]*a[2][2]) - (a[2][1]*a[1][2])) - a[0][1]*(a[1][0]*a[2][2] - a[2][0]*a[1][2])
          + a[0][2]*(a[1][0]*a[2][1] - a[2][0]*a[1][1]);

    return det;
}
// ------------------------------------------------------------------------------------
void calculateFaceNormal(double face[3][3], float fNorm[3])
{
    unsigned char   i;
    double          u[3], v[3], norm;

    for (i=0; i<3; ++i)
    {
        u[i] = face[i][1] - face[i][0];
        v[i] = face[i][2] - face[i][0];

        //for (j=0; j<3; ++j)
        //{
        //    qDebug() << "i: " << i << "\tj: " << j << "\tface: " << face[i][j];
        //}
    }
    fNorm[0] = u[1] * v[2] - u[2] * v[1];
    fNorm[1] = u[2] * v[0] - u[0] * v[2];
    fNorm[2] = u[0] * v[1] - u[1] * v[0];

    //qDebug() << "fNorm[0]: " << fNorm[0];
    //qDebug() << "fNorm[1]: " << fNorm[1];
    //qDebug() << "fNorm[2]: " << fNorm[2];

    for (i=0; i<3; ++i)
    {
        if ((fNorm[i] >= -1.e-10) && (fNorm[i] <= 1.e-10))
             fNorm[i] = 0.0;
    }

    norm = 0.0;
    for (i=0; i<3; ++i)
    {
        norm += fNorm[i] * fNorm[i];
    }
    norm = sqrt(norm);

    //qDebug() << "norm is " << norm;

    for (i=0; i<3; ++i)
    {
        fNorm[i] = fNorm[i] / norm;
    }
}

// ------------------------------------------------------------------------------------
double find1TetVolume(double (*pts)[4])
{
    double  volume;
    double  a[4][4];
    int     i, j;

    volume = 0.0;

    for (i=0; i<4; ++i)
    {
        for (j=0; j<3; ++j)
        {
            a[i][j] = pts[j][i];
        }
        a[i][3] = 1.0;
    }
    volume = determinant4x4(a);
    volume = fabs(volume) / 6.0;

    return volume;
}

// ------------------------------------------------------------------------------------
double determinant4x4(double (*m)[4])
{
    double value, val1, val2, val3, val4, val5, val6;

    val1 = m[0][3] * m[1][2] * m[2][1] * m[3][0]-m[0][2] * m[1][3] * m[2][1] * m[3][0]-m[0][3] * m[1][1] * m[2][2] * m[3][0]+m[0][1] * m[1][3] * m[2][2] * m[3][0];
    val2 = m[0][2] * m[1][1] * m[2][3] * m[3][0]-m[0][1] * m[1][2] * m[2][3] * m[3][0]-m[0][3] * m[1][2] * m[2][0] * m[3][1]+m[0][2] * m[1][3] * m[2][0] * m[3][1];
    val3 = m[0][3] * m[1][0] * m[2][2] * m[3][1]-m[0][0] * m[1][3] * m[2][2] * m[3][1]-m[0][2] * m[1][0] * m[2][3] * m[3][1]+m[0][0] * m[1][2] * m[2][3] * m[3][1];
    val4 = m[0][3] * m[1][1] * m[2][0] * m[3][2]-m[0][1] * m[1][3] * m[2][0] * m[3][2]-m[0][3] * m[1][0] * m[2][1] * m[3][2]+m[0][0] * m[1][3] * m[2][1] * m[3][2];
    val5 = m[0][1] * m[1][0] * m[2][3] * m[3][2]-m[0][0] * m[1][1] * m[2][3] * m[3][2]-m[0][2] * m[1][1] * m[2][0] * m[3][3]+m[0][1] * m[1][2] * m[2][0] * m[3][3];
    val6 = m[0][2] * m[1][0] * m[2][1] * m[3][3]-m[0][0] * m[1][2] * m[2][1] * m[3][3]-m[0][1] * m[1][0] * m[2][2] * m[3][3]+m[0][0] * m[1][1] * m[2][2] * m[3][3];

    value =
        m[0][3] * m[1][2] * m[2][1] * m[3][0]-m[0][2] * m[1][3] * m[2][1] * m[3][0]-m[0][3] * m[1][1] * m[2][2] * m[3][0]+m[0][1] * m[1][3] * m[2][2] * m[3][0] +
        m[0][2] * m[1][1] * m[2][3] * m[3][0]-m[0][1] * m[1][2] * m[2][3] * m[3][0]-m[0][3] * m[1][2] * m[2][0] * m[3][1]+m[0][2] * m[1][3] * m[2][0] * m[3][1] +
        m[0][3] * m[1][0] * m[2][2] * m[3][1]-m[0][0] * m[1][3] * m[2][2] * m[3][1]-m[0][2] * m[1][0] * m[2][3] * m[3][1]+m[0][0] * m[1][2] * m[2][3] * m[3][1] +
        m[0][3] * m[1][1] * m[2][0] * m[3][2]-m[0][1] * m[1][3] * m[2][0] * m[3][2]-m[0][3] * m[1][0] * m[2][1] * m[3][2]+m[0][0] * m[1][3] * m[2][1] * m[3][2] +
        m[0][1] * m[1][0] * m[2][3] * m[3][2]-m[0][0] * m[1][1] * m[2][3] * m[3][2]-m[0][2] * m[1][1] * m[2][0] * m[3][3]+m[0][1] * m[1][2] * m[2][0] * m[3][3] +
        m[0][2] * m[1][0] * m[2][1] * m[3][3]-m[0][0] * m[1][2] * m[2][1] * m[3][3]-m[0][1] * m[1][0] * m[2][2] * m[3][3]+m[0][0] * m[1][1] * m[2][2] * m[3][3] ;
   return value;
}

// ------------------------------------------------------------------------------------
double find1PentVolume(double (*pts)[6])
{
    double volume;
    double detValue;
    double dArray[3][3], ddArray[3];
    double pent1IntegralVals[6];
    int    i, k;
    int    ijkIndexes[6][3];

    pent1IntegralVals[0] = 1.0;
    pent1IntegralVals[1] = 1.0 / 3.0;
    pent1IntegralVals[2] = 1.0 / 3.0;
    pent1IntegralVals[3] = 1.0 / 3.0;
    pent1IntegralVals[4] = 1.0 / 9.0;
    pent1IntegralVals[5] = 1.0 / 9.0;

    ijkIndexes[0][0] = 0;   ijkIndexes[0][1] = 1;   ijkIndexes[0][2] = 2;
    ijkIndexes[1][0] = 0;   ijkIndexes[1][1] = 1;   ijkIndexes[1][2] = 3;
    ijkIndexes[2][0] = 0;   ijkIndexes[2][1] = 1;   ijkIndexes[2][2] = 4;
    ijkIndexes[3][0] = 3;   ijkIndexes[3][1] = 4;   ijkIndexes[3][2] = 2;
    ijkIndexes[4][0] = 3;   ijkIndexes[4][1] = 4;   ijkIndexes[4][2] = 3;
    ijkIndexes[5][0] = 3;   ijkIndexes[5][1] = 4;   ijkIndexes[5][2] = 4;

    volume = 0.0;
    for(i=0; i<6; ++i)
    {
        load1PentCoefArray(pts[0], ddArray, ijkIndexes[i]);
        for(k=0; k<3; ++k) dArray[k][0] = ddArray[k];

        load1PentCoefArray(pts[1], ddArray, ijkIndexes[i]);
        for(k=0; k<3; ++k) dArray[k][1] = ddArray[k];

        load1PentCoefArray(pts[2], ddArray, ijkIndexes[i]);
        for(k=0; k<3; ++k) dArray[k][2] = ddArray[k];

        detValue  = deter3x3(dArray);
        //qDebug() << "i: " << i << "\tdetValue: " << detValue;
        volume   += detValue * pent1IntegralVals[i];
    }                       // -- end i-loop
    return volume;
}
// ------------------------------------------------------------------------------------
void load1PentCoefArray(double *pts, double *coArray, int *indexes)
{
    int i;
    for (i=0; i<3; ++i)
    {
        switch(indexes[i])
        {
             case 0:
                coArray[i] = pts[1] - pts[0] - pts[3] + pts[4];
                break;
             case 1:
                coArray[i] = pts[2] - pts[0] - pts[3] + pts[5];
                break;
             case 2:
                coArray[i] = pts[3] - pts[0];
                break;
             case 3:
                coArray[i] = pts[0] - pts[1] - pts[3] + pts[4];
                break;
             case 4:
                coArray[i] = pts[0] - pts[2] - pts[3] + pts[5];
                break;

        }
        coArray[i] = coArray[i] * 0.5;
    }                                       // -- end i-loop
}
// ------------------------------------------------------------------------------------

double find1HexVolume(double (*pts)[8])
{
    double volume;
    double detValue;
    double dArray[3][3], ddArray[3];
    double hex1IntegralVals[8];
    int    i, k;
    int    ijkIndexes[8][3];

    hex1IntegralVals[0] = 8.0;
    hex1IntegralVals[1] = 8.0 / 3.0;
    hex1IntegralVals[2] = 8.0 / 3.0;
    hex1IntegralVals[3] = 8.0 / 9.0;
    hex1IntegralVals[4] = 8.0 / 9.0;
    hex1IntegralVals[5] = 8.0 / 9.0;
    hex1IntegralVals[6] = 8.0 / 9.0;
    hex1IntegralVals[7] = 8.0 / 27.0;

    ijkIndexes[0][0] = 0;   ijkIndexes[0][1] = 1;   ijkIndexes[0][2] = 2;
    ijkIndexes[1][0] = 0;   ijkIndexes[1][1] = 3;   ijkIndexes[1][2] = 5;
    ijkIndexes[2][0] = 3;   ijkIndexes[2][1] = 1;   ijkIndexes[2][2] = 4;
    ijkIndexes[3][0] = 3;   ijkIndexes[3][1] = 3;   ijkIndexes[3][2] = 6;
    ijkIndexes[4][0] = 5;   ijkIndexes[4][1] = 4;   ijkIndexes[4][2] = 2;
    ijkIndexes[5][0] = 5;   ijkIndexes[5][1] = 6;   ijkIndexes[5][2] = 5;
    ijkIndexes[6][0] = 6;   ijkIndexes[6][1] = 4;   ijkIndexes[6][2] = 4;
    ijkIndexes[7][0] = 6;   ijkIndexes[7][1] = 6;   ijkIndexes[7][2] = 6;

    volume = 0.0;
    for(i=0; i<8; ++i)
    {
        load1HexCoefArray(pts[0], ddArray, ijkIndexes[i]);
        for(k=0; k<3; ++k) dArray[k][0] = ddArray[k];

        load1HexCoefArray(pts[1], ddArray, ijkIndexes[i]);
        for(k=0; k<3; ++k) dArray[k][1] = ddArray[k];

        load1HexCoefArray(pts[2], ddArray, ijkIndexes[i]);
        for(k=0; k<3; ++k) dArray[k][2] = ddArray[k];

        detValue  = deter3x3(dArray);
        //qDebug() << "i: " << i << "\tdetValue: " << detValue;
        volume   += detValue * hex1IntegralVals[i];
    }                       // -- end i-loop
    return volume;
}
// ------------------------------------------------------------------------------------
void load1HexCoefArray(double *pts, double *coArray, int *indexes)
{
    int i;
    for (i=0; i<3; ++i)
    {
        switch(indexes[i])
        {
             case 0:
                coArray[i] = pts[1] + pts[2] + pts[5] + pts[6] - pts[0] - pts[3] - pts[4] - pts[7];
                break;
             case 1:
                coArray[i] = pts[2] + pts[3] + pts[6] + pts[7] - pts[0] - pts[1] - pts[4] - pts[5];
                break;
             case 2:
                coArray[i] = pts[4] + pts[5] + pts[6] + pts[7] - pts[0] - pts[1] - pts[2] - pts[3];
                break;
             case 3:
                coArray[i] = pts[0] - pts[1] + pts[2] - pts[3] + pts[4] - pts[5] + pts[6] - pts[7];
                break;
             case 4:
                coArray[i] = pts[0] + pts[1] - pts[2] - pts[3] - pts[4] - pts[5] + pts[6] + pts[7];
                break;
             case 5:
                coArray[i] = pts[0] - pts[1] - pts[2] + pts[3] - pts[4] + pts[5] + pts[6] - pts[7];
                break;
             case 6:
                coArray[i] = pts[1] - pts[0] - pts[2] + pts[3] + pts[4] - pts[5] + pts[6] - pts[7];
                break;
        }
        coArray[i] = coArray[i] * 0.125;
    }                                       // -- end i-loop
}
// ------------------------------------------------------------------------------------
double find2TetVolume(double (*pts)[10])
{
    double volume;
    double detValue;
    double dArray[3][3], ddArray[3];
    double tet2IntegralVals[4][4][4];
    int    i, j, k, n;
    int    iIndexes[4], jIndexes[4], kIndexes[4], tIndexes[3];

    tet2IntegralVals[0][0][0] = 1.0 /  6.0;
    tet2IntegralVals[0][1][0] = 1.0 / 24.0;
    tet2IntegralVals[0][2][0] = 1.0 / 24.0;
    tet2IntegralVals[0][3][0] = 1.0 / 12.0;

    tet2IntegralVals[1][0][0] = 1.0 /  24.0;
    tet2IntegralVals[1][1][0] = 1.0 / 120.0;
    tet2IntegralVals[1][2][0] = 1.0 / 120.0;
    tet2IntegralVals[1][3][0] = 1.0 /  30.0;

    tet2IntegralVals[2][0][0] = 1.0 /  24.0;
    tet2IntegralVals[2][1][0] = 1.0 / 120.0;
    tet2IntegralVals[2][2][0] = 1.0 /  60.0;
    tet2IntegralVals[2][3][0] = 1.0 /  60.0;

    tet2IntegralVals[3][0][0] = 1.0 /  12.0;
    tet2IntegralVals[3][1][0] = 1.0 /  30.0;
    tet2IntegralVals[3][2][0] = 1.0 /  60.0;
    tet2IntegralVals[3][3][0] = 1.0 /  30.0;

    tet2IntegralVals[0][0][1] = 1.0 /  24.0;
    tet2IntegralVals[0][1][1] = 1.0 / 120.0;
    tet2IntegralVals[0][2][1] = 1.0 / 120.0;
    tet2IntegralVals[0][3][1] = 1.0 /  30.0;

    tet2IntegralVals[1][0][1] = 1.0 /  60.0;
    tet2IntegralVals[1][1][1] = 1.0 / 360.0;
    tet2IntegralVals[1][2][1] = 1.0 / 360.0;
    tet2IntegralVals[1][3][1] = 1.0 /  60.0;

    tet2IntegralVals[2][0][1] = 1.0 / 120.0;
    tet2IntegralVals[2][1][1] = 1.0 / 720.0;
    tet2IntegralVals[2][2][1] = 1.0 / 360.0;
    tet2IntegralVals[2][3][1] = 1.0 / 180.0;

    tet2IntegralVals[3][0][1] = 1.0 /  60.0;
    tet2IntegralVals[3][1][1] = 1.0 / 180.0;
    tet2IntegralVals[3][2][1] = 1.0 / 360.0;
    tet2IntegralVals[3][3][1] = 1.0 /  90.0;

    tet2IntegralVals[0][0][2] = 1.0 /  24.0;
    tet2IntegralVals[0][1][2] = 1.0 /  60.0;
    tet2IntegralVals[0][2][2] = 1.0 / 120.0;
    tet2IntegralVals[0][3][2] = 1.0 /  60.0;

    tet2IntegralVals[1][0][2] = 1.0 / 120.0;
    tet2IntegralVals[1][1][2] = 1.0 / 360.0;
    tet2IntegralVals[1][2][2] = 1.0 / 720.0;
    tet2IntegralVals[1][3][2] = 1.0 / 180.0;

    tet2IntegralVals[2][0][2] = 1.0 / 120.0;
    tet2IntegralVals[2][1][2] = 1.0 / 360.0;
    tet2IntegralVals[2][2][2] = 1.0 / 360.0;
    tet2IntegralVals[2][3][2] = 1.0 / 360.0;

    tet2IntegralVals[3][0][2] = 1.0 /  30.0;
    tet2IntegralVals[3][1][2] = 1.0 /  60.0;
    tet2IntegralVals[3][2][2] = 1.0 / 180.0;
    tet2IntegralVals[3][3][2] = 1.0 /  90.0;

    tet2IntegralVals[0][0][3] = 1.0 /  12.0;
    tet2IntegralVals[0][1][3] = 1.0 /  60.0;
    tet2IntegralVals[0][2][3] = 1.0 /  30.0;
    tet2IntegralVals[0][3][3] = 1.0 /  30.0;

    tet2IntegralVals[1][0][3] = 1.0 /  60.0;
    tet2IntegralVals[1][1][3] = 1.0 / 360.0;
    tet2IntegralVals[1][2][3] = 1.0 / 180.0;
    tet2IntegralVals[1][3][3] = 1.0 /  90.0;

    tet2IntegralVals[2][0][3] = 1.0 /  30.0;
    tet2IntegralVals[2][1][3] = 1.0 / 180.0;
    tet2IntegralVals[2][2][3] = 1.0 /  60.0;
    tet2IntegralVals[2][3][3] = 1.0 /  90.0;

    tet2IntegralVals[3][0][3] = 1.0 /  30.0;
    tet2IntegralVals[3][1][3] = 1.0 /  90.0;
    tet2IntegralVals[3][2][3] = 1.0 /  90.0;
    tet2IntegralVals[3][3][3] = 1.0 /  90.0;

    iIndexes[0] = 0;  iIndexes[1] = 3;  iIndexes[2] = 5;  iIndexes[3] = 6;
    jIndexes[0] = 1;  jIndexes[1] = 3;  jIndexes[2] = 4;  jIndexes[3] = 7;
    kIndexes[0] = 2;  kIndexes[1] = 4;  kIndexes[2] = 5;  kIndexes[3] = 8;

    volume = 0.0;
    for(i=0; i<4; ++i)
    {
        tIndexes[0] = iIndexes[i];
        for(j=0; j<4; ++j)
        {
            tIndexes[1] = jIndexes[j];
            for(k=0; k<4; ++k)
            {
                tIndexes[2] = kIndexes[k];
                load2TetCoefArray(pts[0], ddArray, tIndexes);
                for(n=0; n<3; ++n) dArray[n][0] = ddArray[n];

                load2TetCoefArray(pts[1], ddArray, tIndexes);
                for(n=0; n<3; ++n) dArray[n][1] = ddArray[n];

                load2TetCoefArray(pts[2], ddArray, tIndexes);
                for(n=0; n<3; ++n) dArray[n][2] = ddArray[n];

                detValue  = deter3x3(dArray);
                //qDebug() << "i: " << i << "\tdetValue: " << detValue;
                volume   += detValue * tet2IntegralVals[i][j][k];
            }
        }
    }                       // -- end i-loop
    return volume;
}
// ------------------------------------------------------------------------------------
double find2PentVolume(double (*pts)[15])
{
    double volume;
    double detValue;
    double dArray[3][3], ddArray[3];
    double pent2IntegralVals[7][7][9];
    int    i, j, k, n;
    int    iIndexes[7], jIndexes[7], kIndexes[9], tIndexes[3];

    for (i=0; i<7; ++i)
        for (j=0; j<7; ++j)
            for (k=0; k<9; ++k)
                pent2IntegralVals[i][j][k] = 0.0;

    iIndexes[0] = 0;  iIndexes[1] = 3;   iIndexes[2] = 4;  iIndexes[3] = 6;
    iIndexes[4] = 7;  iIndexes[5] = 10;  iIndexes[6] = 12;
    jIndexes[0] = 1;  jIndexes[1] = 3;   jIndexes[2] = 5;  jIndexes[3] = 6;
    jIndexes[4] = 8;  jIndexes[5] = 11;  jIndexes[6] = 3;
    kIndexes[0] = 2;  kIndexes[1] = 4;   kIndexes[2] = 5;  kIndexes[3] = 6;
    kIndexes[4] = 9;  kIndexes[5] = 10;  kIndexes[6] = 11; kIndexes[7] = 12; kIndexes[8] = 13;

    // Plane #1
    pent2IntegralVals[0][0][0]  =  1.0;       pent2IntegralVals[0][1][0]  =  1.0 / 3.0;  pent2IntegralVals[0][4][0]  =  2.0 / 3.0;
    pent2IntegralVals[0][6][0]  =  1.0 / 3.0;
    pent2IntegralVals[1][0][0]  =  1.0 / 3.0; pent2IntegralVals[1][1][0]  =  1.0 / 12.0; pent2IntegralVals[1][4][0]  =  1.0 / 3.0;
    pent2IntegralVals[1][6][0]  =  2.0 / 9.0;
    pent2IntegralVals[2][2][0]  =  1.0 / 3.0; pent2IntegralVals[2][3][0]  =  1.0 / 9.0;  pent2IntegralVals[2][5][0]  =  2.0 / 9.0;
    pent2IntegralVals[3][2][0]  =  2.0 / 9.0; pent2IntegralVals[3][3][0]  =  1.0 / 36.0; pent2IntegralVals[3][5][0]  =  1.0 / 9.0;
    pent2IntegralVals[4][0][0]  =  2.0 / 3.0; pent2IntegralVals[4][1][0]  =  1.0 / 3.0;  pent2IntegralVals[4][4][0]  =  1.0 / 3.0;
    pent2IntegralVals[4][6][0]  =  2.0 / 9.0;
    pent2IntegralVals[5][2][0]  =  2.0 / 9.0; pent2IntegralVals[5][3][0]  =  1.0 / 9.0;  pent2IntegralVals[5][5][0]  =  1.0 / 9.0;
    pent2IntegralVals[6][0][0]  =  1.0 / 3.0; pent2IntegralVals[6][1][0]  =  1.0 / 9.0;  pent2IntegralVals[6][4][0]  =  2.0 / 9.0;
    pent2IntegralVals[6][6][0]  =  1.0 / 5.0;

    // Plane #2
    pent2IntegralVals[0][0][1]  =  1.0 / 3.0;  pent2IntegralVals[0][1][1] =  1.0 / 6.0;  pent2IntegralVals[0][4][1]  =  1.0 / 6.0;
    pent2IntegralVals[0][6][1]  =  1.0 / 9.0;
    pent2IntegralVals[1][0][1]  =  1.0 / 12.0; pent2IntegralVals[1][1][1] =  1.0 / 30.0; pent2IntegralVals[1][4][1]  =  1.0 / 15.0;
    pent2IntegralVals[1][6][1]  =  1.0 / 36.0;
    pent2IntegralVals[2][2][1]  =  1.0 / 9.0;  pent2IntegralVals[2][3][1] =  1.0 / 18.0; pent2IntegralVals[2][5][1]  =  1.0 / 18.0;
    pent2IntegralVals[3][2][1]  =  1.0 / 36.0; pent2IntegralVals[3][3][1] =  1.0 / 90.0; pent2IntegralVals[3][5][1]  =  1.0 / 45.0;
    pent2IntegralVals[4][0][1]  =  1.0 / 3.0;  pent2IntegralVals[4][1][1] =  1.0 / 5.0;  pent2IntegralVals[4][4][1]  =  2.0 / 15.0;
    pent2IntegralVals[4][6][1]  =  1.0 / 9.0;
    pent2IntegralVals[5][2][1]  =  1.0 / 9.0;  pent2IntegralVals[5][3][1] =  1.0 / 15.0; pent2IntegralVals[5][5][1]  =  2.0 / 45.0;
    pent2IntegralVals[6][0][1]  =  1.0 / 9.0;  pent2IntegralVals[6][1][1] =  1.0 / 18.0; pent2IntegralVals[6][4][1]  =  1.0 / 18.0;
    pent2IntegralVals[6][6][1]  =  1.0 / 15.0;

    // Plane #3
    pent2IntegralVals[0][0][2]  =  1.0 / 3.0;   pent2IntegralVals[0][1][2]  =  1.0 / 12.0;  pent2IntegralVals[0][4][2]  =  1.0 / 3.0;
    pent2IntegralVals[0][6][2]  =  1.0 / 9.0;
    pent2IntegralVals[1][0][2]  =  1.0 / 6.0;   pent2IntegralVals[1][1][2]  =  1.0 / 30.0;  pent2IntegralVals[1][4][2]  =  1.0 / 5.0;
    pent2IntegralVals[1][6][2]  =  1.0 / 18.0;
    pent2IntegralVals[2][2][2]  =  1.0 / 9.0;   pent2IntegralVals[2][3][2]  =  1.0 / 36.0;  pent2IntegralVals[2][5][2]  =  1.0 / 9.0;
    pent2IntegralVals[3][2][2]  =  1.0 / 18.0;  pent2IntegralVals[3][3][2]  =  1.0 / 90.0;  pent2IntegralVals[3][5][2]  =  1.0 / 15.0;
    pent2IntegralVals[4][0][2]  =  1.0 / 6.0;   pent2IntegralVals[4][1][2]  =  1.0 / 15.0;  pent2IntegralVals[4][4][2]  =  2.0 / 15.0;
    pent2IntegralVals[4][6][2]  =  1.0 / 18.0;
    pent2IntegralVals[5][2][2]  =  1.0 / 18.0;  pent2IntegralVals[5][3][2]  =  1.0 / 45.0;  pent2IntegralVals[5][5][2]  =  2.0 / 45.0;
    pent2IntegralVals[6][0][2]  =  1.0 / 9.0;   pent2IntegralVals[6][1][2]  =  1.0 / 36.0;  pent2IntegralVals[6][4][2]  =  1.0 / 9.0;
    pent2IntegralVals[6][6][2]  =  1.0 / 15.0;

    // Plane #4
    pent2IntegralVals[0][0][3]  =  1.0 / 12.0;  pent2IntegralVals[0][1][3] =  1.0 / 30.0;  pent2IntegralVals[0][4][3] =  1.0 / 15.0;
    pent2IntegralVals[0][6][3]  =  1.0 / 36.0;
    pent2IntegralVals[1][0][3]  =  1.0 / 30.0;  pent2IntegralVals[1][1][3] =  1.0 / 90.0;  pent2IntegralVals[1][4][3] =  1.0 / 30.0;
    pent2IntegralVals[1][6][3]  =  1.0 / 90.0;
    pent2IntegralVals[2][2][3]  =  1.0 / 36.0;  pent2IntegralVals[2][3][3] =  1.0 / 90.0;  pent2IntegralVals[2][5][3] =  1.0 / 45.0;
    pent2IntegralVals[3][2][3]  =  1.0 / 90.0;  pent2IntegralVals[3][3][3] =  1.0 / 270.0; pent2IntegralVals[3][5][3] =  1.0 / 90.0;
    pent2IntegralVals[4][0][3]  =  1.0 / 15.0;  pent2IntegralVals[4][1][3] =  1.0 / 30.0;  pent2IntegralVals[4][4][3] =  2.0 / 45.0;
    pent2IntegralVals[4][6][3]  =  1.0 / 45.0;
    pent2IntegralVals[5][2][3]  =  1.0 / 45.0;  pent2IntegralVals[5][3][3] =  1.0 / 90.0;  pent2IntegralVals[5][5][3] =  2.0 / 135.0;
    pent2IntegralVals[6][6][3]  =  1.0 / 36.0;  pent2IntegralVals[6][1][3] =  1.0 / 90.0;  pent2IntegralVals[5][4][3] =  1.0 / 45.0;

    // Plane #5
    pent2IntegralVals[0][2][4]  =  2.0 / 3.0;  pent2IntegralVals[0][3][4]  =  2.0 / 9.0;  pent2IntegralVals[0][5][4]  =  4.0 / 9.0;
    pent2IntegralVals[1][2][4]  =  2.0 / 9.0;  pent2IntegralVals[1][3][4]  =  1.0 / 18.0; pent2IntegralVals[1][5][4]  =  2.0 / 9.0;
    pent2IntegralVals[2][0][4]  =  2.0 / 3.0;  pent2IntegralVals[2][1][4]  =  2.0 / 9.0;  pent2IntegralVals[2][4][4]  =  4.0 / 9.0;
    pent2IntegralVals[2][6][4]  =  2.0 / 5.0;
    pent2IntegralVals[3][0][4]  =  2.0 / 9.0;  pent2IntegralVals[3][1][4]  =  1.0 / 18.0; pent2IntegralVals[3][4][4]  =  2.0 / 9.0;
    pent2IntegralVals[3][6][4]  =  2.0 / 15.0;
    pent2IntegralVals[4][2][4]  =  4.0 / 9.0;  pent2IntegralVals[4][3][4]  =  2.0 / 9.0;  pent2IntegralVals[4][5][4]  =  2.0 / 9.0;
    pent2IntegralVals[5][0][4]  =  4.0 / 9.0;  pent2IntegralVals[5][1][4]  =  2.0 / 9.0;  pent2IntegralVals[5][4][4]  =  2.0 / 9.0;
    pent2IntegralVals[5][6][4]  =  4.0 / 15.0;
    pent2IntegralVals[6][2][4]  =  2.0 / 5.0;  pent2IntegralVals[6][3][4]  =  2.0 / 15.0; pent2IntegralVals[6][5][4]  =  4.0 / 15.0;

    // Plane #6
    pent2IntegralVals[0][0][5]  =  1.0 / 6.0;   pent2IntegralVals[0][1][5]  =  1.0 / 10.0;  pent2IntegralVals[0][4][5]  =  1.0 / 15.0;
    pent2IntegralVals[0][6][5]  =  1.0 / 18.0;
    pent2IntegralVals[1][0][5]  =  1.0 / 30.0;  pent2IntegralVals[1][1][5]  =  1.0 / 60.0;  pent2IntegralVals[1][4][5]  =  1.0 / 45.0;
    pent2IntegralVals[1][6][5]  =  1.0 / 90.0;
    pent2IntegralVals[2][2][5]  =  1.0 / 18.0;  pent2IntegralVals[2][3][5]  =  1.0 / 30.0;  pent2IntegralVals[2][5][5]  =  1.0 / 45.0;
    pent2IntegralVals[3][2][5]  =  1.0 / 90.0;  pent2IntegralVals[3][3][5]  =  1.0 / 180.0; pent2IntegralVals[3][5][5]  =  1.0 / 135.0;
    pent2IntegralVals[4][0][5]  =  1.0 / 5.0;   pent2IntegralVals[4][1][5]  =  2.0 / 15.0;  pent2IntegralVals[4][4][5]  =  1.0 / 15.0;
    pent2IntegralVals[4][6][5]  =  1.0 / 15.0;
    pent2IntegralVals[5][2][5]  =  1.0 / 15.0;  pent2IntegralVals[5][3][5]  =  2.0 / 45.0;  pent2IntegralVals[5][5][5]  =  1.0 / 45.0;
    pent2IntegralVals[6][0][5]  =  1.0 / 18.0;  pent2IntegralVals[6][1][5]  =  1.0 / 30.0;  pent2IntegralVals[6][4][5]  =  1.0 / 45.0;
    pent2IntegralVals[6][6][5]  =  1.0 / 30.0;

    // Plane #7
    pent2IntegralVals[0][0][6]  =  1.0 / 6.0;   pent2IntegralVals[0][1][6]  =  1.0 / 30.0;  pent2IntegralVals[0][4][6]  =  1.0 / 5.0;
    pent2IntegralVals[0][6][6]  =  1.0 / 18.0;
    pent2IntegralVals[1][0][6]  =  1.0 / 10.0;  pent2IntegralVals[1][1][6]  =  1.0 / 60.0;  pent2IntegralVals[1][4][6]  =  2.0 / 15.0;
    pent2IntegralVals[1][6][6]  =  1.0 / 30.0;
    pent2IntegralVals[2][2][6]  =  1.0 / 18.0;  pent2IntegralVals[2][3][6]  =  1.0 / 90.0;  pent2IntegralVals[2][5][6]  =  1.0 / 15.0;
    pent2IntegralVals[3][2][6]  =  1.0 / 30.0;  pent2IntegralVals[3][3][6]  =  1.0 / 180.0; pent2IntegralVals[3][5][6]  =  2.0 / 45.0;
    pent2IntegralVals[4][0][6]  =  1.0 / 15.0;  pent2IntegralVals[4][1][6]  =  1.0 / 45.0;  pent2IntegralVals[4][4][6]  =  1.0 / 15.0;
    pent2IntegralVals[4][6][6]  =  1.0 / 45.0;
    pent2IntegralVals[5][2][6]  =  1.0 / 45.0;  pent2IntegralVals[5][3][6]  =  1.0 / 135.0; pent2IntegralVals[5][5][6]  =  1.0 / 45.0;
    pent2IntegralVals[6][0][6]  =  1.0 / 18.0;  pent2IntegralVals[6][1][6]  =  1.0 / 90.0;  pent2IntegralVals[6][4][6]  =  1.0 / 15.0;
    pent2IntegralVals[6][6][6]  =  1.0 / 30.0;

    // Plane #8
    pent2IntegralVals[0][2][7]  =  2.0 / 9.0;  pent2IntegralVals[0][3][7]  =  1.0 / 9.0;  pent2IntegralVals[0][5][7]  =  1.0 / 9.0;
    pent2IntegralVals[1][2][7]  =  1.0 / 18.0; pent2IntegralVals[1][3][7]  =  1.0 / 45.0; pent2IntegralVals[1][5][7]  =  2.0 / 45.0;
    pent2IntegralVals[2][0][7]  =  2.0 / 9.0;  pent2IntegralVals[2][1][7]  =  1.0 / 9.0;  pent2IntegralVals[2][4][7]  =  1.0 / 9.0;
    pent2IntegralVals[2][6][7]  =  2.0 / 15.0;
    pent2IntegralVals[3][0][7]  =  1.0 / 18.0; pent2IntegralVals[3][1][7]  =  1.0 / 45.0; pent2IntegralVals[3][4][7]  =  2.0 / 45.0;
    pent2IntegralVals[3][6][7]  =  1.0 / 30.0;
    pent2IntegralVals[4][2][7]  =  2.0 / 9.0;  pent2IntegralVals[4][3][7]  =  2.0 / 15.0; pent2IntegralVals[4][5][7]  =  4.0 / 45.0;
    pent2IntegralVals[5][0][7]  =  2.0 / 9.0;  pent2IntegralVals[5][1][7]  =  2.0 / 15.0; pent2IntegralVals[5][4][7]  =  4.0 / 45.0;
    pent2IntegralVals[5][6][7]  =  2.0 / 15.0;
    pent2IntegralVals[6][2][7]  =  2.0 / 15.0; pent2IntegralVals[6][3][7]  =  1.0 / 15.0; pent2IntegralVals[6][5][7]  =  1.0 / 15.0;

    // Plane #9
    pent2IntegralVals[0][2][8]  =  2.0 / 9.0;  pent2IntegralVals[0][3][8]  =  1.0 / 18.0; pent2IntegralVals[0][5][8]  =  2.0 / 9.0;
    pent2IntegralVals[1][2][8]  =  1.0 / 9.0;  pent2IntegralVals[1][3][8]  =  1.0 / 45.0; pent2IntegralVals[1][5][8]  =  2.0 / 15.0;
    pent2IntegralVals[2][0][8]  =  2.0 / 9.0;  pent2IntegralVals[2][1][8]  =  1.0 / 18.0; pent2IntegralVals[2][4][8]  =  2.0 / 9.0;
    pent2IntegralVals[2][6][8]  =  2.0 / 15.0;
    pent2IntegralVals[3][0][8]  =  1.0 / 9.0;  pent2IntegralVals[3][1][8]  =  1.0 / 45.0; pent2IntegralVals[3][4][8]  =  2.0 / 15.0;
    pent2IntegralVals[3][6][8]  =  1.0 / 15.0;
    pent2IntegralVals[4][2][8]  =  1.0 / 9.0;  pent2IntegralVals[4][3][8]  =  2.0 / 45.0; pent2IntegralVals[4][5][8]  =  4.0 / 45.0;
    pent2IntegralVals[5][0][8]  =  1.0 / 9.0;  pent2IntegralVals[5][1][8]  =  2.0 / 45.0; pent2IntegralVals[5][4][8]  =  4.0 / 45.0;
    pent2IntegralVals[5][6][8]  =  1.0 / 15.0;
    pent2IntegralVals[6][2][8]  =  2.0 / 15.0; pent2IntegralVals[6][3][8]  =  1.0 / 30.0; pent2IntegralVals[6][5][8]  =  2.0 / 15.0;

    volume = 0.0;
    for(i=0; i<7; ++i)
    {
        tIndexes[0] = iIndexes[i];
        for(j=0; j<7; ++j)
        {
            tIndexes[1] = jIndexes[j];
            for(k=0; k<9; ++k)
            {
                tIndexes[2] = kIndexes[k];
                load2PentCoefArray(pts[0], ddArray, tIndexes);
                for(n=0; n<3; ++n) dArray[n][0] = ddArray[n];

                load2PentCoefArray(pts[1], ddArray, tIndexes);
                for(n=0; n<3; ++n) dArray[n][1] = ddArray[n];

                load2PentCoefArray(pts[2], ddArray, tIndexes);
                for(n=0; n<3; ++n) dArray[n][2] = ddArray[n];

                detValue  = deter3x3(dArray);
                //qDebug() << "i: " << i << "\tdetValue: " << detValue;
                volume   += detValue * pent2IntegralVals[i][j][k];
            }
        }
    }                       // -- end i-loop
    return volume;
}

// ------------------------------------------------------------------------------------
double find2HexVolume(double (*pts)[20])
{
    double volume;
    double detValue;
    double dArray[3][3], ddArray[3];
    double hex2IntegralVals[12][12][12];
    int    i, j, k, n;
    int    iIndexes[12], jIndexes[12], kIndexes[12], tIndexes[3];

    for (i=0; i<12; ++i)
        for (j=0; j<12; ++j)
            for (k=0; k<12; ++k)
                hex2IntegralVals[i][j][k] = 0.0;

    // Plane #1
    hex2IntegralVals[0][0][0]  =  8.0;        hex2IntegralVals[0][5][0]  =  8.0 / 3.0;  hex2IntegralVals[0][10][0]  =  8.0 / 3.0;
    hex2IntegralVals[1][4][0]  =  16.0 / 3.0;
    hex2IntegralVals[2][2][0]  =   8.0 / 3.0; hex2IntegralVals[2][6][0]  =  8.0 / 9.0;
    hex2IntegralVals[3][8][0]  =  16.0 / 9.0;
    hex2IntegralVals[4][1][0]  =  16.0 / 3.0; hex2IntegralVals[4][11][0]  =  16.0 / 9.0;
    hex2IntegralVals[5][7][0]  =  32.0 / 9.0;
    hex2IntegralVals[6][3][0]  =  16.0 / 9.0;
    hex2IntegralVals[7][9][0] =  32.0 / 27.0;
    hex2IntegralVals[8][0][0]  =   8.0 / 3.0; hex2IntegralVals[8][5][0]  =  8.0 / 9.0;  hex2IntegralVals[8][10][0]  =  8.0 / 9.0;
    hex2IntegralVals[9][2][0] =   8.0 / 9.0;  hex2IntegralVals[9][6][0]  =  8.0 / 27.0;
    hex2IntegralVals[10][0][0] =   8.0 / 3.0; hex2IntegralVals[10][5][0] =  8.0 / 9.0;  hex2IntegralVals[10][10][0] =  8.0 / 5.0;
    hex2IntegralVals[11][4][0] =  16.0 / 9.0;

    // Plane #2
    hex2IntegralVals[0][4][1]  =  16.0 / 3.0;
    hex2IntegralVals[1][0][1]  =   8.0 / 3.0;  hex2IntegralVals[1][5][1]  =  8.0 / 9.0;  hex2IntegralVals[1][10][1] =  8.0 / 9.0;
    hex2IntegralVals[2][8][1]  =  16.0 / 9.0;
    hex2IntegralVals[3][2][1]  =   8.0 / 9.0;  hex2IntegralVals[3][6][1]  =  8.0 / 27.0;
    hex2IntegralVals[4][7][1]  =  32.0 / 9.0;
    hex2IntegralVals[5][1][1]  =  16.0 / 9.0;  hex2IntegralVals[5][11][1] =  16.0 / 27.0;
    hex2IntegralVals[6][9][1]  =  32.0 / 27.0;
    hex2IntegralVals[7][3][1]  =  16.0 / 27.0;
    hex2IntegralVals[8][4][1]  =  16.0 / 5.0;
    hex2IntegralVals[9][8][1]  =  16.0 / 15.0;
    hex2IntegralVals[10][4][1] =  16.0 / 9.0;
    hex2IntegralVals[11][0][1] =   8.0 / 9.0;  hex2IntegralVals[11][5][1]  =  8.0 / 27.0;  hex2IntegralVals[11][10][1] =  8.0 / 15.0;

    // Plane #3
    hex2IntegralVals[0][1][2]   =   8.0 / 3.0;  hex2IntegralVals[0][11][2]  =  8.0 / 9.0;
    hex2IntegralVals[1][7][2]   =  16.0 / 9.0;
    hex2IntegralVals[2][3][2]   =   8.0 / 9.0;
    hex2IntegralVals[3][9][2]   =  16.0 / 27.0;
    hex2IntegralVals[4][0][2]   =  16.0 / 3.0;  hex2IntegralVals[4][5][2]   =  16.0 / 5.0;  hex2IntegralVals[4][10][2] =  16.0 / 9.0;
    hex2IntegralVals[5][4][2]   =  32.0 / 9.0;
    hex2IntegralVals[6][2][2]   =  16.0 / 9.0;  hex2IntegralVals[6][6][2]   =  16.0 / 15.0;
    hex2IntegralVals[7][9][2]   =  32.0 / 27.0;
    hex2IntegralVals[8][2][2]   =   8.0 / 9.0;  hex2IntegralVals[8][11][2]  =  8.0 / 27.0;
    hex2IntegralVals[9][3][2]   =   8.0 / 27.0;
    hex2IntegralVals[10][2][2]  =   8.0 / 9.0;  hex2IntegralVals[10][11][1] =  8.0 / 15.0;
    hex2IntegralVals[11][7][2]  =  16.0 / 17.0;

    // Plane #4
    hex2IntegralVals[0][7][3]   =  16.0 / 9.0;
    hex2IntegralVals[1][1][3]   =   8.0 / 9.0;   hex2IntegralVals[1][11][3]  =  8.0 / 27.0;
    hex2IntegralVals[2][9][3]   =  16.0 / 27.0;
    hex2IntegralVals[3][3][3]   =   8.0 / 27.0;
    hex2IntegralVals[4][4][3]   =  32.0 / 9.0;
    hex2IntegralVals[5][0][3]   =  16.0 / 9.0;   hex2IntegralVals[5][5][3]  =  16.0 / 15.0; hex2IntegralVals[5][10][3]  =  16.0 / 27.0;
    hex2IntegralVals[6][8][3]   =  32.0 / 27.0;
    hex2IntegralVals[7][2][3]   =  16.0 / 27.0;  hex2IntegralVals[7][6][3]  =  16.0 / 45.0;
    hex2IntegralVals[8][7][3]   =  16.0 / 15.0;
    hex2IntegralVals[9][9][3]   =  16.0 / 45.0;
    hex2IntegralVals[10][7][3]  =  16.0 / 27.0;
    hex2IntegralVals[11][1][3]  =   8.0 / 27.0;  hex2IntegralVals[11][11][3] =  8.0 / 45.0;

    // Plane #5
    hex2IntegralVals[0][2][4]   =  16.0 / 3.0;  hex2IntegralVals[0][6][4]   =  16.0 / 9.0;
    hex2IntegralVals[1][8][4]   =  32.0 / 9.0;
    hex2IntegralVals[2][0][4]   =  16.0 / 3.0;  hex2IntegralVals[2][5][4]   =  16.0 / 9.0;  hex2IntegralVals[2][10][4]  =  16.0 / 5.0;
    hex2IntegralVals[3][4][4]   =  32.0 / 9.0;
    hex2IntegralVals[4][3][4]   =  32.0 / 9.0;
    hex2IntegralVals[5][9][4]   =  64.0 / 27.0;
    hex2IntegralVals[6][1][4]   =  32.0 / 9.0;  hex2IntegralVals[6][11][4]  =  32.0 / 15.0;
    hex2IntegralVals[7][7][4]   =  64.0 / 27.0;
    hex2IntegralVals[8][2][4]   =  16.0 / 9.0;  hex2IntegralVals[8][6][4]   =  16.0 / 27.0;
    hex2IntegralVals[9][0][4]   =  16.0 / 9.0;  hex2IntegralVals[9][5][4]   =  16.0 / 27.0; hex2IntegralVals[9][10][4] =  16.0 / 15.0;
    hex2IntegralVals[10][6][4]  =  16.0 / 15.0; hex2IntegralVals[10][2][4]  =  16.0 / 5.0;
    hex2IntegralVals[11][8][4]  =  32.0 / 15.0;

    // Plane #6
    hex2IntegralVals[0][0][5]  =   8.0 / 3.0;  hex2IntegralVals[0][5][5]  =  8.0 / 5.0;  hex2IntegralVals[0][10][5]  = 8.0 / 9.0;
    hex2IntegralVals[1][4][5]  =  16.0 / 9.0;
    hex2IntegralVals[2][2][5]  =   8.0 / 9.0;  hex2IntegralVals[2][6][5]  =  8.0 / 15.0;
    hex2IntegralVals[3][8][5]  =  16.0 / 27.0;
    hex2IntegralVals[4][1][5]  =  16.0 / 5.0;  hex2IntegralVals[4][11][5] =  16.0 / 15.0;
    hex2IntegralVals[5][6][5]  =  32.0 / 15.0;
    hex2IntegralVals[6][3][5]  =  16.0 / 15.0;
    hex2IntegralVals[7][9][5]  =  32.0 / 45.0;
    hex2IntegralVals[8][0][5]  =   8.0 / 9.0;  hex2IntegralVals[8][5][5]  =  8.0 / 15.0;  hex2IntegralVals[8][10][5] =  8.0 / 27.0;
    hex2IntegralVals[9][2][5]  =  8.0 / 27.0;  hex2IntegralVals[9][6][5]  =  8.0 / 45.0;
    hex2IntegralVals[10][0][5] =  8.0 / 9.0;   hex2IntegralVals[10][5][5] =  8.0 / 15.0;  hex2IntegralVals[10][10][5] = 8.0 / 15.0;
    hex2IntegralVals[11][4][5] =  16.0 / 27.0;

    // Plane #7
    hex2IntegralVals[0][4][6]  =  16.0 / 9.0;
    hex2IntegralVals[1][0][6]  =   8.0 / 9.0;  hex2IntegralVals[1][5][6]  =  8.0 / 15.0;  hex2IntegralVals[1][10][6]  =  8.0 / 27.0;
    hex2IntegralVals[2][8][6]  =  16.0 / 27.0;
    hex2IntegralVals[3][2][6]  =   8.0 / 27.0; hex2IntegralVals[3][6][6]  =  8.0 / 45.0;
    hex2IntegralVals[4][7][6]  =  32.0 / 15.0;
    hex2IntegralVals[5][1][6]  =  16.0 / 15.0; hex2IntegralVals[5][11][6] =  16.0 / 45.0;
    hex2IntegralVals[6][9][6]  =  32.0 / 45.0;
    hex2IntegralVals[7][3][6]  =  16.0 / 45.0;
    hex2IntegralVals[8][4][6]  =  16.0 / 15.0;
    hex2IntegralVals[9][8][6]  =  16.0 / 45.0;
    hex2IntegralVals[10][4][6] =  16.0 / 27.0;
    hex2IntegralVals[11][0][6] =   8.0 / 27.9;  hex2IntegralVals[11][5][6] =  8.0 / 45.0;  hex2IntegralVals[11][10][6] =  8.0 / 45.0;

    // Plane #8
    hex2IntegralVals[0][0][7]  =   8.0 / 3.0;  hex2IntegralVals[0][5][7]  =  8.0 / 9.0;  hex2IntegralVals[0][10][7] =  8.0 / 9.0;
    hex2IntegralVals[1][4][7]  =  16.0 / 5.0;
    hex2IntegralVals[2][2][7]  =   8.0 / 9.0;  hex2IntegralVals[2][6][7]  =  8.0 / 27.0;
    hex2IntegralVals[3][8][7]  =  16.0 / 15.0;
    hex2IntegralVals[4][1][7]  =  16.0 / 9.0;  hex2IntegralVals[4][11][7] =  16.0 / 27.0;
    hex2IntegralVals[5][7][7]  =  32.0 / 15.0;
    hex2IntegralVals[6][3][7]  =  16.0 / 27.0;
    hex2IntegralVals[7][9][7]  =  32.0 / 45.0;
    hex2IntegralVals[8][0][7]  =   8.0 / 5.0;  hex2IntegralVals[8][5][7]  =  8.0 / 15.0;  hex2IntegralVals[8][10][7] =  8.0 / 15.0;
    hex2IntegralVals[9][2][7]  =   8.0 / 15.0; hex2IntegralVals[9][6][7]  =  8.0 / 45.0;
    hex2IntegralVals[10][0][7] =   8.0 / 9.0;  hex2IntegralVals[10][5][7] =  8.0 / 27.0;  hex2IntegralVals[10][10][7] =  8.0 / 15.0;
    hex2IntegralVals[11][4][7] =  16.0 / 15.0;

    // Plane #9
    hex2IntegralVals[0][1][8]  =   8.0 / 9.0;  hex2IntegralVals[0][11][8]  =  8.0 / 27.0;
    hex2IntegralVals[1][7][8]  =  16.0 / 15.0;
    hex2IntegralVals[2][3][8]  =   8.0 / 27.0;
    hex2IntegralVals[3][9][8]  =  16.0 / 45.0;
    hex2IntegralVals[4][0][8]  =  16.0 / 9.0;  hex2IntegralVals[4][5][8]  =  16.0 / 15.0; hex2IntegralVals[4][10][8]  =  16.0 / 27.0;
    hex2IntegralVals[5][4][8]  =  32.0 / 15.0;
    hex2IntegralVals[6][2][8]  =  16.0 / 25.0; hex2IntegralVals[6][6][8]  =  16.0 / 45.0;
    hex2IntegralVals[7][8][8]  =  32.0 / 45.0;
    hex2IntegralVals[8][1][8]  =   8.0 / 15.0; hex2IntegralVals[8][11][8] =  8.0 / 45.0;
    hex2IntegralVals[9][3][8]  =   8.0 / 45.0;
    hex2IntegralVals[10][1][8] =   8.0 / 27.0;  hex2IntegralVals[10][11][8] =  8.0 / 45.0;
    hex2IntegralVals[11][7][8] =  16.0 / 45.0;

    // Plane #10
    hex2IntegralVals[0][3][9]  =  16.0 / 9.0;
    hex2IntegralVals[1][9][9] =  32.0 / 27.0;
    hex2IntegralVals[2][1][9]  =  16.0 / 9.0;  hex2IntegralVals[2][11][9]  =  16.0 / 15.0;
    hex2IntegralVals[3][7][9]  =  32.0 / 27.0;
    hex2IntegralVals[4][2][9]  =  32.0 / 9.0;  hex2IntegralVals[4][6][9]   =  32.0 / 15.0;
    hex2IntegralVals[5][8][9]  =  64.0 / 27.0;
    hex2IntegralVals[6][0][9]  =  32.0 / 9.0;  hex2IntegralVals[6][5][9]   =  32.0 / 15.0; hex2IntegralVals[6][10][9] =  32.0 / 15.0;
    hex2IntegralVals[7][4][9]  =  64.0 / 27.0;
    hex2IntegralVals[8][3][9]  =  16.0 / 27.0;
    hex2IntegralVals[9][1][9]  =  16.0 / 27.0; hex2IntegralVals[9][11][9] =  16.0 / 45.0;
    hex2IntegralVals[10][3][9] =  16.0 / 15.0;
    hex2IntegralVals[11][9][9] =  32.0 / 45.0;

    // Plane #11
    hex2IntegralVals[0][8][10]   =  32.0 / 9.0;
    hex2IntegralVals[1][2][10]   =  16.0 / 9.0;  hex2IntegralVals[1][6][10]  =  16.0 / 27.0;
    hex2IntegralVals[2][4][10]   =  32.0 / 9.0;
    hex2IntegralVals[3][0][10]   =  16.0 / 9.0;  hex2IntegralVals[3][5][10]  =  16.0 / 27.0; hex2IntegralVals[3][10][10] =  16.0 / 15.0;
    hex2IntegralVals[4][9][10]   =  64.0 / 27.0;
    hex2IntegralVals[5][3][10]   =  32.0 / 27.0;
    hex2IntegralVals[6][7][10]   =  64.0 / 27.0;
    hex2IntegralVals[7][1][10]   =  32.0 / 27.0; hex2IntegralVals[7][11][10]  =  32.0 / 45.0;
    hex2IntegralVals[8][8][10]   =  32.0 / 15.0;
    hex2IntegralVals[9][4][10]   =  32.0 / 15.0;
    hex2IntegralVals[10][8][10]  =  32.0 / 15.0;
    hex2IntegralVals[11][2][10]  =  16.0 / 15.0; hex2IntegralVals[11][6][10]  =  16.0 / 45.0;

    // Plane #12
    hex2IntegralVals[0][9][11]   =  32.0 / 27.0;
    hex2IntegralVals[1][3][11]   =  16.0 / 27.0;
    hex2IntegralVals[2][7][11]   =  32.0 / 27.0;
    hex2IntegralVals[3][1][11]   =  16.0 / 27.0; hex2IntegralVals[3][11][11]  =  16.0 / 45.0;
    hex2IntegralVals[4][8][11]   =  64.0 / 27.0;
    hex2IntegralVals[5][2][11]   =  32.0 / 27.0; hex2IntegralVals[5][6][11]   =  32.0 / 45.0;
    hex2IntegralVals[6][8][11]   =  64.0 / 27.0;
    hex2IntegralVals[7][0][11]   =  32.0 / 27.0; hex2IntegralVals[8][5][11]   =  32.0 / 45.0; hex2IntegralVals[7][10][11] =  32.0 / 45.0;
    hex2IntegralVals[8][9][11]   =  32.0 / 45.0;
    hex2IntegralVals[9][7][11]   =  32.0 / 45.0;
    hex2IntegralVals[10][9][11]  =  32.0 / 45.0;
    hex2IntegralVals[11][3][11]  =  16.0 / 45.0;

    iIndexes[0] = 0;  iIndexes[1] = 3;   iIndexes[2]  = 5;   iIndexes[3]  = 6;
    iIndexes[4] = 7;  iIndexes[5] = 10;  iIndexes[6]  = 11;  iIndexes[7]  = 12;
    iIndexes[8] = 13; iIndexes[9] = 15;  iIndexes[10] = 16;  iIndexes[11] = 18;

    jIndexes[0] = 1;  jIndexes[1] = 3;   jIndexes[2]  = 4;   jIndexes[3]  = 6;
    jIndexes[4] = 8;  jIndexes[5] = 10;  jIndexes[6]  = 12;  jIndexes[7]  = 13;
    jIndexes[8] = 14; jIndexes[9] = 15;  jIndexes[10] = 17;  jIndexes[11] = 18;

    kIndexes[0] = 2;  kIndexes[1] = 4;   kIndexes[2]  = 5;   kIndexes[3]  = 6;
    kIndexes[4] = 9;  kIndexes[5] = 11;  kIndexes[6]  = 12;  kIndexes[7]  = 14;
    kIndexes[8] = 15; kIndexes[9] = 16;  kIndexes[10] = 17;  kIndexes[11] = 18;

    volume = 0.0;
    //for(i=0; i<12; ++i)
    for(i=0; i<1; ++i)
    {
        tIndexes[0] = iIndexes[i];
      //  for(j=0; j<12; ++j)
        for(j=0; j<1; ++j)
        {
            tIndexes[1] = jIndexes[j];
            for(k=0; k<12; ++k)
            {
                tIndexes[2] = kIndexes[k];
                load2HexCoefArray(pts[0], ddArray, tIndexes);
                for(n=0; n<3; ++n) dArray[n][0] = ddArray[n];

                load2HexCoefArray(pts[1], ddArray, tIndexes);
                for(n=0; n<3; ++n) dArray[n][1] = ddArray[n];

                load2HexCoefArray(pts[2], ddArray, tIndexes);
                for(n=0; n<3; ++n) dArray[n][2] = ddArray[n];

                detValue  = deter3x3(dArray);
                volume   += detValue * hex2IntegralVals[i][j][k];
                //if (hex2IntegralVals[i][j][k] > 0.0)
                //{
                 //   qDebug() << "tIndexes: " << tIndexes[0] << "\t" << tIndexes[1] << "\t" << tIndexes[2];
                //    qDebug() << " i j k: " << i << "\t" << j << "\t" << k << "\tdetValue: " << detValue << "hex2Int: " << hex2IntegralVals[i][j][k] << "\t vol: " << volume;
                //qDebug() << "i: " << i << "\tdetValue: " << detValue << "hex2Int: " << hex2IntegralVals[i][j][k] << "\t vol: " << volume;
                //}
            }
        }
    }                       // -- end i-loop
    return volume;
}
// ------------------------------------------------------------------------------------
void load2TetCoefArray(double *pts, double *coArray, int *indexes)
{
    int i;
    for (i=0; i<3; ++i)
    {
        switch(indexes[i])
        {
             case 0:
                coArray[i] = 4.0 * pts[4] - pts[1] - 3.0 * pts[0];
                break;
             case 1:
                coArray[i] = 4.0 * pts[6] - pts[2] - 3.0 * pts[0];
                break;
             case 2:
                coArray[i] = 4.0 * pts[7] - pts[3] - 3.0 * pts[0];
                break;
             case 3:
                coArray[i] = 4.0 * (pts[0] - pts[4] + pts[5] - pts[6]);
                break;
             case 4:
                coArray[i] = 4.0 * (pts[0] - pts[6] - pts[7] + pts[9]);
                break;
             case 5:
                coArray[i] = 4.0 * (pts[0] - pts[4] - pts[7] + pts[8]);
                break;
             case 6:
                coArray[i] = 2.0 * (pts[0] + pts[1]) - 4.0 * pts[4];
                break;
             case 7:
                coArray[i] = 2.0 * (pts[0] + pts[2]) - 4.0 * pts[6];
                break;
             case 8:
                coArray[i] = 2.0 * (pts[0] + pts[3]) - 4.0 * pts[7];
                break;
        }
    }                                       // -- end i-loop
}
// ------------------------------------------------------------------------------------
void load2PentCoefArray(double *pts, double *coArray, int *indexes)
{
    int i;
    double  r1, r2;

    for (i=0; i<3; ++i)
    {
        switch(indexes[i])
        {
             case 0:    // g
                r1 = (pts[13] - pts[12] - pts[0] - pts[1] - pts[3] - pts[4]);
                r2 = (pts[6]  + pts[9])  * 2.0;
                coArray[i] = r1 + r2;
                break;
             case 1:    // h
                r1 = (pts[14] - pts[0] - pts[12] - pts[2] - pts[3] - pts[5]);
                r2 = (pts[11] + pts[8]) * 2.0;
                coArray[i] = r1 + r2;
                break;
             case 2:    // r
                coArray[i] = 0.5 * (pts[3] - pts[0]);
                break;
             case 3:    // gh
                coArray[i] = (pts[10] + pts[7] + pts[0] - pts[9] - pts[11] + pts[3] - pts[6] - pts[8]) * 2.0;
                break;
             case 4:    // gr
                r1 = (3.0 * (pts[0] - pts[3]) + pts[1] - pts[4]) * 0.5;
                r2 =  2.0 * (pts[9] - pts[6]);
                coArray[i] = r1 + r2;
                break;
             case 5:    // hr
                r1 = (3.0 * (pts[0]  - pts[3]) + pts[2] - pts[5]) * 0.5;
                r2 =  2.0 * (pts[11] - pts[8]);
                coArray[i] = r1 + r2;
                break;
             case 6:    // ghr
                coArray[i] = (pts[10] - pts[7] - pts[0] - pts[9] - pts[11] + pts[3] + pts[6] + pts[8]) * 2.0;
                break;
             case 7:    // g^2
                coArray[i] = (pts[0] + pts[1] + pts[3] + pts[4] - 2.0 * (pts[9] + pts[6]));
                break;
             case 8:    // h^2
                coArray[i] = (pts[0] + pts[2] + pts[3] + pts[5] - 2.0 * (pts[11] + pts[8]));
                break;
             case 9:    // r^2
                coArray[i] = (pts[0] + pts[3]) * 0.5 - pts[12];
                break;
             case 10:   // g^2 r
                coArray[i] = (pts[3] + pts[4] - pts[0] - pts[1] + 2.0 * (pts[6] - pts[9]));
                break;
             case 11:   // h^2 r
                coArray[i] = (pts[3] + pts[5] - pts[0] - pts[2] + 2.0 * (pts[8] - pts[11]));
                break;
             case 12:   // r^2 g
                coArray[i] = pts[12] - pts[13] + (pts[1] - pts[0] - pts[3] + pts[4]) * 0.5;
                break;
             case 13:   // r^2 h
                coArray[i] = pts[12] - pts[14] + (pts[2] - pts[0] - pts[3] + pts[5]) * 0.5;
                break;
        }
    }                                       // -- end i-loop
}
// ------------------------------------------------------------------------------------
void load2HexCoefArray(double *pts, double *coArray, int *indexes)
{
    int     i;
    double  r1, r2;

    for (i=0; i<3; ++i)
    {
        switch(indexes[i])
        {
             case 0:    // g
                r1 = (pts[9] - pts[11] + pts[13] - pts[15] - pts[16] + pts[17] + pts[18] - pts[19]) * 0.25;
                r2 = (pts[0] - pts[1]  - pts[2]  + pts[3]  + pts[4]  - pts[5]  - pts[6]  + pts[7]) * 0.125;
                coArray[i] = r1 + r2;
                break;
             case 1:    // h
                r1 = (pts[10] - pts[12] + pts[14] - pts[16] - pts[17] + pts[18] + pts[19] - pts[8]) * 0.25;
                r2 = (pts[0]  + pts[1]  - pts[2]  - pts[3]  + pts[4]  + pts[5]  - pts[6]  - pts[7]) * 0.125;
                coArray[i] = r1 + r2;
                break;
             case 2:    // r
                r1 = (pts[12] + pts[13] + pts[14] + pts[15] - pts[9]  - pts[10] - pts[11] - pts[8]) * 0.25;
                r2 = (pts[0]  + pts[1]  + pts[2]  + pts[3]  - pts[4]  - pts[5]  - pts[6]  - pts[7]) * 0.125;
                coArray[i] = r1 + r2;
                break;
             case 3:    // gh
                coArray[i] =  (pts[16] - pts[17] + pts[18] - pts[19]) * 0.25;
                break;
             case 4:    // hr
                coArray[i] =  (pts[8] + pts[14] - pts[12] - pts[10]) * 0.25;
                break;
             case 5:    // gr
                coArray[i] =  (pts[11] - pts[9] + pts[13] - pts[15]) * 0.25;
                break;
             case 6:    // ghr
                coArray[i] = (pts[1] - pts[0] - pts[2] + pts[3] + pts[4] - pts[5] + pts[6] - pts[7]) * 0.125;
                break;
             case 7:    // g^2
                r1 = (pts[0]  + pts[1]  + pts[2]  + pts[3] + pts[4] + pts[5] + pts[6] + pts[7]) * 0.125;
                r2 = (pts[10] + pts[12] + pts[14] + pts[8]) * 0.25;
                coArray[i] = r1 - r2;
                break;
             case 8:    // h^2
                r1 = (pts[0]  + pts[1]  + pts[2]  + pts[3] + pts[4] + pts[5] + pts[6] + pts[7]) * 0.125;
                r2 = (pts[9]  + pts[11] + pts[13] + pts[15]) * 0.25;
                coArray[i] = r1 - r2;
                break;
             case 9:    // r^2
                r1 = (pts[0]  + pts[1]  + pts[2]  + pts[3] + pts[4] + pts[5] + pts[6] + pts[7]) * 0.125;
                r2 = (pts[16] + pts[17] + pts[18] + pts[19]) * 0.25;
                coArray[i] = r1 - r2;
                break;
             case 10:   // g^2 h
                r1 = (pts[2]  + pts[3]  - pts[4]  - pts[5] + pts[6] + pts[7] - pts[0] - pts[1]) * 0.125;
                r2 = (pts[12] - pts[10] - pts[14] + pts[8]) * 0.25;
                coArray[i] = r1 + r2;
                break;
             case 11:   // g^2 r
                r1 = (pts[4]  + pts[5]  + pts[6]  + pts[7] - pts[0] - pts[1] - pts[2] - pts[3]) * 0.125;
                r2 = (pts[10] - pts[12] - pts[14] + pts[8]) * 0.25;
                coArray[i] = r1 + r2;
                break;
             case 12:   // g^2 hr
                r1 = (pts[0]  + pts[1]  - pts[2]  - pts[3] - pts[4] - pts[5] + pts[6] + pts[7]) * 0.125;
                r2 = (pts[10] + pts[12] - pts[14] - pts[8]) * 0.25;
                coArray[i] = r1 + r2;
                break;
             case 13:   // h^2 g
                r1 = (pts[1]  - pts[0]  + pts[2]  - pts[3] - pts[4] + pts[5] + pts[6] - pts[7]) * 0.125;
                r2 = (pts[11] - pts[9]  - pts[13] + pts[15]) * 0.25;
                coArray[i] = r1 + r2;
                break;
             case 14:   // h^2 r
                r1 = (pts[4]  + pts[5]  + pts[6]  + pts[7] - pts[0] - pts[1] - pts[2] - pts[3]) * 0.125;
                r2 = (pts[9]  + pts[11] - pts[13] - pts[15]) * 0.25;
                coArray[i] = r1 + r2;
                break;
             case 15:   // h^2 gr
                r1 = (pts[0]  - pts[1]  - pts[2]  + pts[3] - pts[4] + pts[5] + pts[6] - pts[7]) * 0.125;
                r2 = (pts[9]  - pts[11] - pts[13] + pts[15]) * 0.25;
                coArray[i] = r1 + r2;
                break;
             case 16:   // r^2 g
                r1 = (pts[1]  - pts[0]  + pts[2]  - pts[3] - pts[4] + pts[5] + pts[6] - pts[7]) * 0.125;
                r2 = (pts[16] - pts[17] - pts[18] + pts[19]) * 0.25;
                coArray[i] = r1 + r2;
                break;
             case 17:   // r^2 h
                r1 = (pts[2]  - pts[0]  - pts[1]  + pts[3] - pts[4] - pts[5] + pts[6] + pts[7]) * 0.125;
                r2 = (pts[16] + pts[17] - pts[18] - pts[19]) * 0.25;
                coArray[i] = r1 + r2;
                break;
             case 18:   // r^2 gh
                r1 = (pts[0]  - pts[1]  + pts[2]  - pts[3] + pts[4] - pts[5] + pts[6] - pts[7]) * 0.125;
                r2 = (pts[17] - pts[16] - pts[18] + pts[19]) * 0.25;
                coArray[i] = r1 + r2;
                break;
        }
    }                                       // -- end i-loop
}
