#ifndef UMDATASTRUCTURES_H
#define UMDATASTRUCTURES_H

#include <QColor>
#include <vector>

using namespace std;



struct Point
{
    public:
        float coord[3];
        Point operator-(Point p2);
        Point operator/(float x);
};

struct Point3
{
    public:
        float coord[9];
};

struct Bbox
{
    public:
        float box[6];
};

struct AnnotatePoint
{
    public:
        float   x;
        float   y;
        float   z;
        int     ptSize;
        bool    display;
        QColor  color;
};

struct AnnotateLine
{
    public:
        short   pt1;
        short   pt2;
        int     lineSize;
        bool    display;
        QColor  color;
};

struct AnnotateString
{
    public:
        QColor  color;
        QString string;
        short   pt1;
        bool    display;
        int     stringSize;
};

struct AnnotateObject
{
    vector<AnnotatePoint>   annotatePoints;
    vector<AnnotateLine>    annotateLines;
    vector<AnnotateString>  annotateStrings;

    bool persistence;
};


#endif // UMDATASTRUCTURES_H
