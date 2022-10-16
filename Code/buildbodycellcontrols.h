#ifndef BUILDBODYCELLCONTROLS_H
#define BUILDBODYCELLCONTROLS_H

#include <QObject>
#include <QWidget>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QGridLayout>

#include "buildbodycontrols.h"
//#include "buildspatialcontrols.h"
#include "buildcellcontrols.h"
//#include "cgzone.h"
//#include "cgzone2.h"
#include "masterdata.h"
#include "license.h"
#include "cutawaywidget.h"

class BuildBodyCellControls  : public QWidget
{
    Q_OBJECT

public:
    BuildBodyCellControls(MasterData *mdIn, License *inLic, QProgressBar *pBar,
                          QVector<CGBodyBasics> *inBodyBasics, QVector<CGZoneOps> *inZoneOps, Bounds *inBounds);
    ~BuildBodyCellControls();

    void closeFileOperations();

    QStandardItemModel *bodyModel;
    QStandardItemModel *cellModel;

    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void setUnclippedFlag(bool value);
    void resetForProject();


signals:
//    void    zoneReadyToShow(CGZone2 *, int);
    void    zoneReadyToShow();
    void    xRotationChanged(int angle);
    void    yRotationChanged(int angle);
    void    zRotationChanged(int angle);

    void      xCutawayChanged(double plane, int direction);
    void      yCutawayChanged(double plane, int direction);
    void      zCutawayChanged(double plane, int direction);

    void      xAxisCullDirChange(int value);
    void      yAxisCullDirChange(int value);
    void      zAxisCullDirChange(int value);

    void    bodyCellControlMessage(QString);
    void    cellListReady(int);
    void    cellChangedSignal(int, int);
    void    centerOnXYZPosition(int i, double x, double y, double z);

private slots:
    void handleBodyAddSignal(int addType, QStandardItem *item);
    void handleBodyChangedSignal();
    void handleCellShowSignal();
    void handleCellListReady(int val);
    void handleCellChangedSignal(int val, int cell);
    void handleBodyNumberSignal(int oldCell, int newCell);

    void handleXRotation(int angle);
    void handleYRotation(int angle);
    void handleZRotation(int angle);

    void handleXCutAwayChanged(double plane, int direction);
    void handleYCutAwayChanged(double plane, int direction);
    void handleZCutAwayChanged(double plane, int direction);

    void handleXAxisCullDirChange(int value);
    void handleYAxisCullDirChange(int value);
    void handleZAxisCullDirChange(int value);

    void handleMessages(QString msg);
    void handleBodyQueryRequestSignal(int bodyNo);
    void handleBodyRenumberRequestSignal(int bodyNo);
    void handleCenterOnXYZ(int i, double x, double y, double z);

private:

    void    buildTab1();
    void    buildTab2();
    void    setupSlots();

    void    updateZoneBounds(Bounds *zBounds, int bn);

    QVector<CGBodyBasics>   *cgBodyBasics;
    QVector<CGZoneOps>      *cgZoneOps;

    QProgressBar            *progressBar;
    QTabWidget              *tabWidget;
    QWidget                 *tab1Widget;
    QWidget                 *tab2Widget;

    BuildBodyControls       *bodyControls;
    BuildCellControls       *cellControls;
//    BuildSpatialControls    *spatialControls;
    CutAwayWidget           *cutawayWidget;
    License                 *license;

    QVector<CGZoneOps>  zoneOps;

    int                 projectType;
    int                 tabIndex1;
    int                 tabIndex2;

//    CGZone              zone_old;
//    CGZone2             zone;
    MasterData          *md;
    Bounds          *bounds;

    bool unclipCell;
};

#endif // BUILDBODYCELLCONTROLS_H
