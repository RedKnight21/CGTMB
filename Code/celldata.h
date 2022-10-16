#ifndef CELLDATA_H
#define CELLDATA_H

#include <QObject>
#include <QColor>
#include <QVector>
#include "cgzoneops.h"

struct CellData
{
    QString name;
    QString cellNo;
    QString mtlNo;
    QString density;
    QString importance;
    QColor  color;
    QVector<CGZoneOps> zoneOps;
    int     xRes;
    int     yRes;
    int     zRes;
    int     meshReduction;
    bool    display;

    void clear(const int cell)
    {
        name    = "";
        if (cell > 0) cellNo.setNum(cell);
        else cellNo = "";
        mtlNo   = "";
        density = "";
        importance = "1.0";
        display = true;
        zoneOps.clear();
        xRes = 50;
        yRes = 50;
        zRes = 50;
        meshReduction = 0;
    }
};

#endif // CELLDATA_H
