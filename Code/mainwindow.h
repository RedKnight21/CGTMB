/*******************************************************************************
 **                                                                           **
 ** Copyright (C) 2022 Stag Sceintific Solutions, Inc.                        **
 ** Contact:	Roger Martz, President & CEO                                  **
 **			PO Box 427, Wadley, GA 30477                                      **
 **                                                                           **
 ********************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QDebug>
#include <QtWidgets>
#include <QMenuBar>
#include <QMenu>
#include "cgtmb_constants.h"
#include "bounds.h"
#include "buildcellwindow.h"
#include "masterdata.h"
#include "view3dmodel.h"
//#include "mainmodelwindow.h"
#include "projectsettings.h"
#include "license.h"
// #include "sss_textviewdialog.h"
 #include "materialmainwindow.h"
// #include "ptracmain.h"
#include "abaqusReadFile.h"
#include <embeddedmeshbuilder.h>
#include "baseVtkWindow.h"
#include "ssslightsource.h"
#include "cardimageview.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private slots:
    void onAddNew();
    void doClose();
    void doCloseQuit();
    void launchBuildCellZone();
    void launchView3DModel();
    void launchMaterialBuilder();
    void launchEmbeddedBuilder();
    void launchPtracDisplay();
    void handleNewProject();
    void handleSaveAs(const int mode);
    void handleSaveAs();
    void handleSaveProject();
    void handleOpenProject();
    void handleCloseProject();
    void openRecentProjectFile();
    void handleCellListReady(int val);
    void handleCellChangedSignal(int val, int cell);
    void handleModelUpdate();
    void handleSizeChange(const int value);
    void handleSquareWindow(const int value);
    void handleExportCardImage(int which);
    void handleITSExportCardImage();
    void handleXGENExportCardImage();
    void handleMCNPExportCardImage();

    void handleProjectSettings();
    void handleProjectSettingsUpdate(int val);
    void handlePtracShowSignal(int);
    void showCGTMBversion();
    void handleTextWindowAnswerSignal(bool agree);
    void handleAbaqusImport();
    void handleShowMeshUniverse(int);
    void handleMeshUniverseTransparancy(double);
    void handleMeshUniversePersistence(int persistence);
    void saveXRotation(int);
    void saveYRotation(int);
    void saveZRotation(int);
    void save3DBounds(Bounds);
    void handlePtracPersistenceSignal(int);
    void handlePtracGhostSignal(int);
    void handleUpdateMeshList();
    void handleXCutawayChanged(double, int);
    void handleYCutawayChanged(double, int);
    void handleZCutawayChanged(double, int);

    void handleLightingDialog();
    void handleLightSource();
    void handleOrientationAction();
    void handleCameraAction();

private:
    void    closeFile();
    void    createActions();
    void    createMenus();
    void    createStatusBar();

    void    doMeshImports();

    void    readSettings();
    void    writeSettings();
    void    updateRecentProjectFileActions();
    void    setRecentProjectFileList(const QString &fileName);
    void    openProjectFile(QString fileName);
    void    updateStatusBar(QString fileName);
    void    showCGTMBversion(unsigned char which);
    void    updateMenus();
    void    fileSaveFailureMessage();
    void    makeToolBar();


    BaseVtkWindow       *modelVtkWindow;
    BuildCellWindow     *buildCellWindow;
    View3DModel         *view3DModelWindow;

    SSSLightSource          *lightSource;
    LightingControl         lightingControl;

//    MainModelWindow     *mainModelWindow;
    CardImageView       *cardImageView;
    ProjectSettings     *projectSettings;
//    SSS_TextViewDialog  *agreementWindow;
    MaterialMainWindow  *materialMainWindow;
    EmbeddedMeshBuilder *embeddedMeshWindow;
//    PtracMain           *ptrackMain;
    License             license;

    QSplitter           *leftSplitter;


    QAction *buildCellZone;
    QAction *view3DModel;
    QAction *materialWindow;
    QAction *embeddedMeshAction;
    QAction *ptracAction;

    QAction *closeProjectAction;
    QAction *closeQuitProjectAction;
    QAction *exitAction;
    QAction *newProject;
    QAction *openProject;
    QAction *saveProjectAction;
    QAction *saveAsProjectAction;
    QAction *separatorAction;
    QAction *exportCardImageAction;
    QAction *importAbaqusMeshAction;
    QAction *projectSettingsAction;
    QAction *aboutCGTMBaction;
    QAction *exportITSAction;
    QAction *exportMCNPAction;
    QAction *exportXGENAction;
    QAction *lightingAction;
    QAction *cameraAction;
    QAction *orientationAction;

    QToolBar *toolBar;

    QMenu *closeProjectMenu;
    QMenu *fileMenu;
    QMenu *newProjectMenu;
    QMenu *newProjectSubMenu;
    QMenu *openProjectMenu;
    QMenu *saveProjectmenu;
    QMenu *saveAsProjectmenu;
    QMenu *toolsMenu;
    QMenu *recentProjectFileMenu;
    QMenu *helpMenu;
    QMenu *importMeshSubMenu;
    QMenu *exportCardImageSubmenu;

    QMenuBar *menuBar;

    QString     currentPath;
    QString     projectFileName;
    QStringList recentProjectFiles;
    QString     strippedName(const QString &fullFileName);
    QString     statusFileName;
    QString     settingsVersion;

    QString     version;

    QLabel      *statusFileNameLabel;

    int model3DWidgetAngles[3];
    int whichCardImage;

    bool needsSaved;
    bool newProjectFlag;
    bool projectSettingsChanged;

    MasterData masterData;
    Bounds     saved3DBounds;
    Bounds     modelBounds;

    QVector<CGBodyBasics>   bodyBasics;
    QVector<CellData>       masterCellList;
    QVector<ReadFileAbaqus> abaqusFiles;
    QVector<float>          ptracPointData;

    enum    { MaxRecentFiles = 10 };

    QAction     *recentProjectFileActions[MaxRecentFiles];
};

#endif
