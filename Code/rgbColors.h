#ifndef RGBCOLORS_H
#define RGBCOLORS_H

#include <vector>
#include <stdio.h>
#include <QtCore>
#include <QColor>

using namespace std;

void setRGBcolors(int maxColors, int numColors, vector<int> &r, vector<int> &g, vector<int> &b);
void setGrayScale(int maxColors, int numColors, vector<int> &r, vector<int> &g, vector<int> &b);
void setRampColors(int maxColors, int nColorCount, QColor lowColor, QColor hiColor, vector<double> &r, vector<double> &g, vector<double> &b);

#endif // RGBCOLORS_H
