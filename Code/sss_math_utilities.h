#ifndef SSS_MATH_UTILITIES_H
#define SSS_MATH_UTILITIES_H
#include <QtCore>
#include <cmath>

int matrixInverse3x3(float a[][3]);
void matMult3(float a3x3[][3], float b3[], float result[]);
int searchBins(double &x, std::vector<double> &array, int aSize);
int binaryIntSearch(int value, std::vector<int> &array);
void getRotatedPoint(double *ap1, double *ap2, double angle, double *oldPt, double *newPt);
void rotatePoint(double &x, double &y, double &z, double phi, double theta);

void calculateFaceNormal(double face[3][3], float fNorm[3]);


double Max(std::vector<double> &x);
double Min(std::vector<double> &x);
double Max(double *x, int num);
double Min(double *x, int num);
double deter3x3(double (*matrx)[3]);

std::string trim(std::string str);

double find1PentVolume(double (*pts)[6]);
void   load1PentCoefArray(double *pts, double *coArray, int *indexes);
double find1TetVolume(double (*pts)[4]);
double find1HexVolume(double (*pts)[8]);
void   load1HexCoefArray(double *pts, double *coArray, int *indexes);
double  determinant4x4(double (*m)[4]);
void    load2TetCoefArray(double *pts, double *coArray, int *indexes);
double  find2TetVolume(double (*pts)[10]);
void    load2HexCoefArray(double *pts, double *coArray, int *indexes);
double  find2HexVolume(double (*pts)[20]);
double  find2PentVolume(double (*pts)[15]);
void load2PentCoefArray(double *pts, double *coArray, int *indexes);


#endif // SSS_MATH_UTILITIES_H
