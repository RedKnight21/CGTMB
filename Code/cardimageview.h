#ifndef CARDIMAGEVIEW_H
#define CARDIMAGEVIEW_H

#include <QObject>
#include <QWidget>
#include <QMap>
#include "bodytypemap.h"
#include "masterdata.h"
#include "textviewwindow.h"
#include "sss_string_utils.h"

class CardImageView : public TextViewWindow
{
public:
    CardImageView(QWidget *parent, bool readFlag, bool saveFlag,
                  bool clrFlag, bool clsFlag);
    ~CardImageView();

    QSize sizeHint() const override;
    void  writeSettings()  override;
    void  readSettings()  override;
    void  generateView(MasterData *masterData, int which);

private:
    void  saveFile()  override;
    void  clearText()  override;

    void  generateBodyCardImage(QString &bodyString);
    void  generateCellCardImage(QString &cellString, int mult, MasterData *md);
    void  generateMtlCardImage(MasterData *masterData, QString &mtlString);
    void  findUsedBodies();
    void  generateViewIts(MasterData *masterData);
    void  generateViewMcnp(MasterData *masterData);
    void  generateViewXgen(MasterData *masterData);
    void  generateItsBodyCardImage(QString &bodyString, MasterData *masterData);
    void  generateItsZoneImage(QString &cellString, int mult);
    void  generateItsZoneImage2(QString &cellString);


    void  generateItsMtlCardImage(QString &cellString);
    void  generateMcnpEmbedCardImage(MasterData *md, QString &embString);
    void  addMeshZonesToMap(MasterData *masterData);
    void  addCGZonesToMap(int mult);

    QVector<CGBodyBasics> bodyBasics;
    QVector<CellData>     masterCellList;

    QMap<int, QString> zoneMap;
    QMap<int, QString> materialMap;

    QString ruler1;
    QString ruler40;
    QString ruler50;
    QString ruler60;
    QString ruler70;
    QString ruler80;
    QString ruler90;
    QString ruler100;
    QString ruler110;
    QString ruler120;

    QString impString;
    QString fileOutputString;
    int lineLength;

    bool doBody;
    bool doCell;
    bool doImp;
    bool doRuler;
    bool doMtls;

};

#endif // CARDIMAGEVIEW_H
