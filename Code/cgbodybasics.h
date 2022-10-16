#ifndef CGBODYBASICS_H
#define CGBODYBASICS_H

#include <QString>
#include <QVector>
#include "cg_constants.h"

class CGBodyBasics
{
public:
    CGBodyBasics() {  };

    std::vector<double> parameters;

    QString bodyName;
    int     bodyNumber;
    bool    used;
    unsigned char bodyType;

};


#endif // CGBODYBASICS_H
