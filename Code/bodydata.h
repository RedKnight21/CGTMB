#ifndef BODYDATA_H
#define BODYDATA_H

#include <QObject>

class BodyData
{

public:

    QString bodyName;
    QString bodyTag;
    QString parameterTags[12];
    QString parameterValues[12];
    int     bodyNo;
    unsigned char limit;

    void clear()
    {
        bodyNo   = 0;
        bodyName = "";
        bodyTag  = "";
        for(unsigned char i=0; i<12; ++i)
        {
            parameterTags[i]   = "";
            parameterValues[i] = "";
        }
    }
};

#endif // BODYDATA_H
