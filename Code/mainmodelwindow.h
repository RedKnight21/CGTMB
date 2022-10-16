#ifndef MAINMODELWINDOW_H
#define MAINMODELWINDOW_H

#include <QObject>
#include <QWidget>
#include <QtWidgets>
#include "cgbodybasics.h"
#include "celldata.h"
#include "baseVtkWindow.h"
//#include "model3dwidget.h"
//#include "buildcellgraphics.h"

class MainWindow;
class MasterData;

class MainModelWindow : public QWidget
{
    Q_OBJECT

public:
    MainModelWindow(MasterData *mdIn, QVector<CGBodyBasics> *bBasics,
                    QVector<CellData> *mCellList, QVector<ReadFileAbaqus> *abqFiles);
    ~MainModelWindow();

    void updateModel(bool newProjectFlag);
    void updateModel(int which, int cell);
    void handleCloseProject();
    void showMeshUniverse(int universe, int persistence);
    void showMeshUniverse(int persistence);
    void showPtracData(int sig, QVector<GLfloat> *data);
    void handlePtracPersistence(int sig);
    void handleGhostGeometry(int sig);
    void buildModelZones();

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);
    void boundsChanged(Bounds b);
    void mainModelWindowMessage(QString);
    void windowSizeChanged(int);
    void squareWindowSize(const int);

public slots:
    void setXRotation(int);
    void setYRotation(int);
    void setZRotation(int);

    void setXCutAwayPlane(double);
    void setYCutAwayPlane(double);
    void setZCutAwayPlane(double);

    void setXAxisCullDirChange(int);
    void setYAxisCullDirChange(int);
    void setZAxisCullDirChange(int);

    void setOnXYZPosition(int i, double x, double y, double z);
    void setAutoRotateAngle(int axis, double angle);
    void saveRotationAngles();
    void restoreRotationAngles();

private slots:
    void handleXRotation(int angle);
    void handleYRotation(int angle);
    void handleZRotation(int angle);
    void handleBounds(Bounds b);
    void handleModel3DWidgetMessage(QString s);
    void handleSizeChange(const int value);
    void handleSquareWindow(const int value);

private:
    void setupSlots();

    MasterData *md;

    Model3DWidget     *model3DWidget;
//    BuildCellGraphics *buildCellGraphics;

    QVector<CGBodyBasics> *bodyBasics;
    QVector<CellData>     *masterCellList;

};

#endif // MAINMODELWINDOW_H
