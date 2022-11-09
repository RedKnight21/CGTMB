#ifndef VIEW3DMODEL_H
#define VIEW3DMODEL_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "masterdata.h"
#include "view3dcontrol.h"
#include "celllistwidget.h"
#include "buildcellcontrols.h"
#include "bounds.h"
#include <abaqusReadFile.h>
#include <meshlistwidget.h>
#include "ssslightsource.h"

class Model3DWidget;

class View3DModel : public QMainWindow
{
    Q_OBJECT
public:
    View3DModel(MasterData *mdIn, QVector<CGBodyBasics> *bBasics,
                QVector<CellData> *mCellList,
                QVector<ReadFileAbaqus> *abqFiles,
                int *m3Dangles,
                QWidget *parent = nullptr);

    ~View3DModel();

//    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
    void writeSettings();
    void resetForProject();
    void displayMeshDock();
    void updateModelBounds(Bounds *inBounds);

signals:
    void modelUpdate();

    void      xCutawayChanged(double plane, int direction);
    void      yCutawayChanged(double plane, int direction);
    void      zCutawayChanged(double plane, int direction);

    void      xAxisCullDirChange(int value);
    void      yAxisCullDirChange(int value);
    void      zAxisCullDirChange(int value);
    void      centerOnXYZPosition(int, double, double, double);

public slots:
    void setBounds(Bounds);
    void setMessage(QString);
    void handleUpdate();
    void handleShow();

private slots:
    void doClose();
    void handleClearTextEdit();

    void handleXCutAwayChanged(double plane, int direction);
    void handleYCutAwayChanged(double plane, int direction);
    void handleZCutAwayChanged(double plane, int direction);

    void handleXAxisCullDirChange(int value);
    void handleYAxisCullDirChange(int value);
    void handleZAxisCullDirChange(int value);

private:
    void readSettings();
    void createDockWidget();
    void buildCloseButton();
    void buildMessageWindow();
    void setupSlots();
    void handleLightingDialog();
    void handleLightSource();

    void keyPressEvent(QKeyEvent *event) override;

    View3DControl   *view3DControl;
    CellListWidget  *cellListWidget;
    MeshListWidget  *meshListWidget;
    QWidget         *zoneList;
    QWidget         *messageWindow;
    QAction         *cellZoneAction;
    QAction         *messageAction;
    QAction         *meshUniverseAction;
    QPushButton     *closePB;
    QPushButton     *clearPB;
    QPushButton     *updatePB;
    QPushButton     *showPB;

    QHBoxLayout     *closeLayout;
    QVBoxLayout     *messageLayout;

    QTextEdit       *messageTextWindow;

    MasterData      *md;
    Model3DWidget   *model3DWidget;

    CellData        cellData;
    Bounds          bounds;

    QVector<CGBodyBasics> *bodyBasics;
    QVector<CellData>     *masterCellList;

    QDockWidget *dock2;


protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

};

#endif // VIEW3DMODEL_H
