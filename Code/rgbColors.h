#ifndef RGBCOLORS_H
#define RGBCOLORS_H

#include <vector>
#include <stdio.h>
#include <QtCore>
#include <QColor>

void setRGBcolors(int maxColors, int numColors, std::vector<int> &r, std::vector<int> &g, std::vector<int> &b);
void setGrayScale(int maxColors, int numColors, std::vector<int> &r, std::vector<int> &g, std::vector<int> &b);
void setRampColors(int maxColors, int nColorCount, QColor lowColor, QColor hiColor, std::vector<double> &r, std::vector<double> &g, std::vector<double> &b);

#endif // RGBCOLORS_H
