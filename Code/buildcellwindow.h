#ifndef BUILDCELLWINDOW_H
#define BUILDCELLWINDOW_H

#include <QMainWindow>
#include <QSplitter>
//#include "buildcellgraphics.h"
#include "buildbodycellcontrols.h"
#include "cg_constants.h"
#include "rgbColors.h"
#include "masterdata.h"
#include "license.h"
#include "baseVtkWindow.h"
#include "ssslightsource.h"

class MasterData;

class BuildCellWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit BuildCellWindow(MasterData *mdIn, License *inLic, QWidget *parent = nullptr);
    ~BuildCellWindow();

    QSize sizeHint() const override;

    void    closeFileOperations();
    void    writeSettings();
    void    resetForProject();

    QProgressBar *progressBar;

signals:
    void cellListReady(int);
    void cellChangedSignal(int, int);
    void saveNeeded();

public slots:
    void doClose();
    void handleZoneReadyToShow();
    void handleCellListReady(int val);
    void handleCellChangedSignal(int val, int cell);

private slots:
    void handleXRotation(int anlge);
    void handleYRotation(int anlge);
    void handleZRotation(int anlge);

    void handleXCutAwayChanged(double plane, int direction);
    void handleYCutAwayChanged(double plane, int direction);
    void handleZCutAwayChanged(double plane, int direction);

    void handleXAxisCullDirChange(int value);
    void handleYAxisCullDirChange(int value);
    void handleZAxisCullDirChange(int value);

    void handleMessages(QString);
    void handleClearTextEdit();
    void handleSizeChange(const int value);
    void handleCenterOnXYZ(int i, double x, double y, double z);

    void handleUnclipCB(int state);
    void handleClipAction();
    void handleLightingDialog();
    void handleLightSource();
    void handleSaveNeeded();
    void handleOrientationAction();
    void handleCameraAction();

private:
    void        makeActions();
    void        makeToolBar();


    Bounds cellBounds;

    BaseVtkWindow           *cellVtkWindow;
    BuildBodyCellControls   *bodyControlsWidget;
    License                 *license;
    MasterData              *md;
    SSSLightSource          *lightSource;
    LightingControl         lightingControl;

    QVector<CGBodyBasics>   cgBodyBasics;
    QVector<CGZoneOps>      cgZoneOps;

    QSplitter           *mainSplitter;
    QSplitter           *leftSplitter;
    QSplitter           *rightSplitter;
    QWidget             *bottomLeftWidget;

    QTextEdit           *bottomLeftTextEdit;
    QPushButton         *closePB;
    QPushButton         *clearPB;
    QCheckBox           *unclipCB;

    QToolBar            *toolBar;

    QAction *plotAction;
    QAction *messageAction;

    QAction                 *exitAction;
    QAction                 *saveAction;
    QAction                 *helpAction;
    QAction                 *printAction;
    QAction                 *openAction;
    QAction                 *clipAction;
    QAction                 *cameraAction;
    QAction                 *orientationAction;
    QAction                 *lightingAction;

    int projectType;

    void    buildBottomLeftWidget();
    void    readSettings();
    void    setupSlots();
    void    createDockWidgets();

protected:

};

#endif // BUILDCELLWINDOW_H
