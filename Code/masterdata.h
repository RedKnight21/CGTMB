#ifndef MASTERDATA_H
#define MASTERDATA_H

#include <QtWidgets>
#include <QFile>
#include "celldata.h"
#include "cgbodybasics.h"
#include "bodytypemap.h"
#include "cgtmb_constants.h"
// #include "materialdata.h"
#include "abaqusReadFile.h"

class MasterData
{
public:
    MasterData();
    ~MasterData();

    bool saveToFile(QString fName);
    bool readFromFile(QString fName);
    void makeBodyBasics(QVector<CGBodyBasics> *bodyBasics);
    void makeCellList(QVector<CellData> *cellList);
    void clear();
    void getAbaqusNames(QVector<QString> *list);
    bool checkAbaqusFileName(QString &name);
    void getCellNumbers(QVector<QString> &list);

    void initialAddToInstanceDataStringList(ReadFileAbaqus *rfa, QString &objName);

    int  getAbaqusFileIndex(QString &name);
    void removeInstanceFromList(QString &univ);
    int  getTypeEnergyBins(const int index, QVector<double> &bins);
    int  getStringListIndex(QVector<QStringList> &vsl, const QString key);
    void setInstanceDisplay(int univ, int cell, int state);
    int  getInstanceDisplay(int univ, int cell);
    int  getCellNumberIndex(int cell);
    void getInstanceDataColors(std::vector<double> *r, std::vector<double> *g, std::vector<double> *b);
    void getMeshModParameters(QString *name, double *params);
    int  getMeshUniverseNumber(QString *name);
    int  getInstanceCellNumberByIndex(int univ, int index);

    QVector<QStringList> bodyDataStringList;
    QVector<QStringList> cellDataStringList;
    QVector<QStringList> mtlDataStringList;
    QVector<QString>     abaqusFileStringList;
    QVector<QStringList> umeshDataStringList;
    QVector<QStringList> instanceDataStringList;
    QVector<QString>     usedUniverseNumbers;
    QVector<QStringList> universeDataStringList;
    QVector<QStringList> ptracBatchesStringList;
    QVector<QStringList> ptracEnergyStringList;

    QVector<int> customZaidIntList;
    QMap<QString, QString> umObjNameUniverse;

    QVector<QColor> ptracEventColors;


    QString projectTitle;
    QString XSDIRfile;
    QString ptracFile;
    int projectType;
    int displayRuler;
    int displayImportances;
    int inputLineWidth;
    int displayBodyComments;
    int displayCellComments;
    int displayMtlComments;
    int particleType;
    int customZaidListType;
    int ptracEventPointSize;

private:
    QString strippedName(const QString &fullFileName);

    int lastUniverseNumber;
    void setDefaults();
};

#endif // MASTERDATA_H
