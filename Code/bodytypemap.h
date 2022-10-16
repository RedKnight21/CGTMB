#ifndef BODYTYPEMAP_H
#define BODYTYPEMAP_H

#include <QMap>
#include "cg_constants.h"

struct BodyTypeMap
{
    BodyTypeMap()
    {
        bodyNumMap.clear();
        bodyNumMap["RCC"] = RCC;
        bodyNumMap["REC"] = REC;
        bodyNumMap["TRC"] = TRC;
        bodyNumMap["SPH"] = SPH;
        bodyNumMap["ELL"] = ELL;
        bodyNumMap["ELR"] = ELR;
        bodyNumMap["BOX"] = BOX;
        bodyNumMap["WED"] = WED;
        bodyNumMap["RPP"] = RPP;
        bodyNumMap["TOR"] = TOR;
    }
    QMap<QString, int>          bodyNumMap;
};

struct BodyNameMap
{
    BodyNameMap()
    {
        bodyNameMap.clear();
        bodyNameMap[RCC] = "RCC";
        bodyNameMap[REC] = "REC";
        bodyNameMap[TRC] = "TRC";
        bodyNameMap[SPH] = "SPH";
        bodyNameMap[ELL] = "ELL";
        bodyNameMap[ELR] = "ELR";
        bodyNameMap[BOX] = "BOX";
        bodyNameMap[WED] = "WED";
        bodyNameMap[RPP] = "RPP";
        bodyNameMap[TOR] = "TOR";
    }

    QMap<int, QString> bodyNameMap;
};

#endif // BODYTYPEMAP_H
