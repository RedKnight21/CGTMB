#ifndef BODYBUTTONINFO_H
#define BODYBUTTONINFO_H

#include <QVector>
#include <QDebug>

using namespace std;

struct BodyButtonInfo
{
    BodyButtonInfo()
    {
        bodyButtonTips.push_back("SPH - sphere");
        bodyButtonTips.push_back("ELL - elipse");
        bodyButtonTips.push_back("ELR - elipsoid of revolution");

        bodyButtonTips.push_back("RCC - right circular cylinder");
        bodyButtonTips.push_back("REC - ripght elliptical cylinder");
        bodyButtonTips.push_back("TRC - truncated right-angle cone");
        bodyButtonTips.push_back("RPP - rectabgular parallelipiped");
        bodyButtonTips.push_back("BOX - arbitrarily oriented orthogonal box");
        bodyButtonTips.push_back("WED - arbitrarily oriented wedge");
        bodyButtonTips.push_back("TOR - torus");

        bodyButtonLabels.push_back("SPH");
        bodyButtonLabels.push_back("ELL");
        bodyButtonLabels.push_back("ELR");
        bodyButtonLabels.push_back("RCC");
        bodyButtonLabels.push_back("REC");
        bodyButtonLabels.push_back("TRC");
        bodyButtonLabels.push_back("RPP");
        bodyButtonLabels.push_back("BOX");
        bodyButtonLabels.push_back("WED");
        bodyButtonLabels.push_back("TOR");

        bodyTypeTags.clear();

        bodyTextLabels.clear();
        vector<QString> tmp;
        QString s;

        // NOTE: Order is important in bulding bodyTextLabels!

        // SPH
        s = "Vx";  tmp.push_back(s);
        s = "Vy";  tmp.push_back(s);
        s = "Vz";  tmp.push_back(s);
        s = "R ";  tmp.push_back(s);
        s = ""; for(int i=5; i<20; ++i) tmp.push_back(s);
        bodyTextLabels.push_back(tmp);
        bodyTypeTags.push_back("SPH");

        // ELL
        tmp.clear();
        s = "V1x";  tmp.push_back(s);
        s = "V1y";  tmp.push_back(s);
        s = "V1z";  tmp.push_back(s);
        s = "V2x";  tmp.push_back(s);
        s = "V2y";  tmp.push_back(s);
        s = "V2z";  tmp.push_back(s);
        s = "R  ";  tmp.push_back(s);
        s = ""; for(int i=8; i<20; ++i) tmp.push_back(s);
        bodyTextLabels.push_back(tmp);
        bodyTypeTags.push_back("ELL");

        // ELR
        tmp.clear();
        s = "Vx";  tmp.push_back(s);
        s = "Vy";  tmp.push_back(s);
        s = "Vz";  tmp.push_back(s);
        s = "Hx";  tmp.push_back(s);
        s = "Hy";  tmp.push_back(s);
        s = "Hz";  tmp.push_back(s);
        s = "R ";  tmp.push_back(s);
        s = ""; for(int i=8; i<20; ++i) tmp.push_back(s);
        bodyTextLabels.push_back(tmp);
        bodyTypeTags.push_back("ELR");

        // RCC
        tmp.clear();
        s = "Vx";  tmp.push_back(s);
        s = "Vy";  tmp.push_back(s);
        s = "Vz";  tmp.push_back(s);
        s = "Hx";  tmp.push_back(s);
        s = "Hy";  tmp.push_back(s);
        s = "Hz";  tmp.push_back(s);
        s = "R ";  tmp.push_back(s);
        s = ""; for(int i=8; i<20; ++i) tmp.push_back(s);
        bodyTextLabels.push_back(tmp);
        bodyTypeTags.push_back("RCC");

        // REC
        tmp.clear();
        s = "Vx ";  tmp.push_back(s);
        s = "Vy ";  tmp.push_back(s);
        s = "Vz ";  tmp.push_back(s);
        s = "Hx ";  tmp.push_back(s);
        s = "Hy ";  tmp.push_back(s);
        s = "Hz ";  tmp.push_back(s);
        s = "R1x";  tmp.push_back(s);
        s = "R1y";  tmp.push_back(s);
        s = "R1z";  tmp.push_back(s);
        s = "R2x";  tmp.push_back(s);
        s = "R2y";  tmp.push_back(s);
        s = "R2z";  tmp.push_back(s);
        s = ""; for(int i=13; i<20; ++i) tmp.push_back(s);
        bodyTextLabels.push_back(tmp);
        bodyTypeTags.push_back("REC");

        // TRC
        tmp.clear();
        s = "Vx";  tmp.push_back(s);
        s = "Vy";  tmp.push_back(s);
        s = "Vz";  tmp.push_back(s);
        s = "Hx";  tmp.push_back(s);
        s = "Hy";  tmp.push_back(s);
        s = "Hz";  tmp.push_back(s);
        s = "R1";  tmp.push_back(s);
        s = "R2";  tmp.push_back(s);
        s = ""; for(int i=9; i<20; ++i) tmp.push_back(s);
        bodyTextLabels.push_back(tmp);
        bodyTypeTags.push_back("TRC");

        // RPP
        tmp.clear();
        s = "Xmin";  tmp.push_back(s);
        s = "Xmax";  tmp.push_back(s);
        s = "Ymin";  tmp.push_back(s);
        s = "Ymax";  tmp.push_back(s);
        s = "Zmin";  tmp.push_back(s);
        s = "Zmax";  tmp.push_back(s);
        s = ""; for(int i=7; i<20; ++i) tmp.push_back(s);
        bodyTextLabels.push_back(tmp);
        bodyTypeTags.push_back("RPP");

        // BOX
        tmp.clear();
        s = "Vx ";  tmp.push_back(s);
        s = "Vy ";  tmp.push_back(s);
        s = "Vz ";  tmp.push_back(s);
        s = "A1x";  tmp.push_back(s);
        s = "A1y";  tmp.push_back(s);
        s = "A1z";  tmp.push_back(s);
        s = "A2x";  tmp.push_back(s);
        s = "A2y";  tmp.push_back(s);
        s = "A2z";  tmp.push_back(s);
        s = "A3x";  tmp.push_back(s);
        s = "A3y";  tmp.push_back(s);
        s = "A3z";  tmp.push_back(s);
        s = ""; for(int i=13; i<20; ++i) tmp.push_back(s);
        bodyTextLabels.push_back(tmp);
        bodyTypeTags.push_back("BOX");

        // WED
        tmp.clear();
        s = "Vx ";  tmp.push_back(s);
        s = "Vy ";  tmp.push_back(s);
        s = "Vz ";  tmp.push_back(s);
        s = "A1x";  tmp.push_back(s);
        s = "A1y";  tmp.push_back(s);
        s = "A1z";  tmp.push_back(s);
        s = "A2x";  tmp.push_back(s);
        s = "A2y";  tmp.push_back(s);
        s = "A2z";  tmp.push_back(s);
        s = "A3x";  tmp.push_back(s);
        s = "A3y";  tmp.push_back(s);
        s = "A3z";  tmp.push_back(s);
        s = ""; for(int i=13; i<20; ++i) tmp.push_back(s);
        bodyTextLabels.push_back(tmp);
        bodyTypeTags.push_back("WED");

        // TOR
        tmp.clear();
        s = "Vx ";  tmp.push_back(s);
        s = "Vy ";  tmp.push_back(s);
        s = "Vz ";  tmp.push_back(s);
        s = "Hx ";  tmp.push_back(s);
        s = "Hy ";  tmp.push_back(s);
        s = "Hz ";  tmp.push_back(s);
        s = "R  ";  tmp.push_back(s);
        s = "RH ";  tmp.push_back(s);
        s = "Rp ";  tmp.push_back(s);
        s = ""; for(int i=10; i<20; ++i) tmp.push_back(s);
        bodyTextLabels.push_back(tmp);
        bodyTypeTags.push_back("TOR");

        /*
        qDebug() << "size of tmp: " << tmp.size();

        for(int i=0; i<15; ++i)
        {
            qDebug() << "i: " << i << " bodyTextLabels[1] " << bodyTextLabels[1][i];
        }
        */
        tmp.clear();
    }
    QVector<QString> bodyTypeTags;
    QVector<QString> bodyButtonTips;
    QVector<QString> bodyButtonLabels;
    vector< vector<QString> > bodyTextLabels;

    unsigned char skipTo[10] = {5, 8, 8, 8, 13, 9, 7, 13, 13, 10};
};

#endif // BODYBUTTONINFO_H
