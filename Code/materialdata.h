#ifndef MATERIALDATA_H
#define MATERIALDATA_H

#include<QObject>
#include<QColor>
#include<QVector>

struct MtlData
{
    QString name;
    QString mtlNo;
    QString density;
    QColor  color;
    bool    display;
    QVector<QString> zaid;
    QVector<QString> fraction;

    void clear()
    {
        name    = "";
        mtlNo   = "";
        density = "";
        display = true;
        color.setNamedColor("MidnightBlue");
    }
};

#endif // MATERIALDATA_H
