/****************************************************************************
**
** Copyright (C) 2020 Stag Sceintific Solutions, Inc.
** Contact:
**
****************************************************************************/
#include "mainwindow.h"

MainWindow::MainWindow()
{
    // *** Do not append bug release digit to version number
    // *** to this variable.
    version                = "0.7";
    masterData.projectType = NOPROJECT;
    needsSaved             = false;
    newProjectFlag         = true;
    projectSettingsChanged = false;
    projectFileName        = "";
    buildCellWindow        = nullptr;
    modelVtkWindow         = nullptr;
    cardImageView          = nullptr;
    projectSettings        = nullptr;
    view3DModelWindow      = nullptr;
//    mainModelWindow        = nullptr;
    materialMainWindow     = nullptr;
    embeddedMeshWindow     = nullptr;
//    ptrackMain             = nullptr;
    abaqusFiles.clear();

    model3DWidgetAngles[0] = 0;
    model3DWidgetAngles[1] = 0;
    model3DWidgetAngles[2] = 0;

//    license.readLicenseFile();
//    license.checkLicense("CGTMB", version);
    license.devOption1 = true;
    license.allowXsdir = true;
    license.allowEmbedded = true;
    license.allowPtrack = true;

    createActions();
    createMenus();

    statusBar()->showMessage(tr("Status Bar"));

    onAddNew();

    QString str = "CGTMB -- Combinatorial Geometry Transport Model Builder :: Version ";
    str += QApplication::applicationVersion();
    setWindowTitle(str);

    createStatusBar();
//    setWindowIcon(QIcon(":/Resources/hand_holding_cylinders.icns"));
    readSettings();
    makeToolBar();

//    if ((!license.accepted) || (settingsVersion != QApplication::applicationVersion()))
//    {
//        showCGTMBversion(1);
//    }
    masterData.clear();
}
// ------------------------------------------------------------------------------------------------
MainWindow::~MainWindow()
{
    if (buildCellWindow    != nullptr) buildCellWindow->~BuildCellWindow();
    if (cardImageView      != nullptr) cardImageView->~CardImageView();
    if (view3DModelWindow  != nullptr) view3DModelWindow->~View3DModel();
//    if (mainModelWindow    != nullptr) mainModelWindow->~MainModelWindow();
    if (materialMainWindow != nullptr) materialMainWindow->~MaterialMainWindow();
//    if (ptrackMain         != nullptr) ptrackMain->~PtracMain();
}
// ------------------------------------------------------------------------------------------------
void MainWindow::doClose()
{
    if (needsSaved)
    {
        QFont courierFont("Courier", 14);
        QMessageBox about;
        QString str = " Project needs to be saved! \n";
        about.setIcon(QMessageBox::Critical);
        about.setFont(courierFont);
        about.setInformativeText(str);
        about.setStandardButtons(QMessageBox::Ok);
        about.setDefaultButton(QMessageBox::Ok);
        about.setBaseSize(300, 150);
        about.show();
        about.exec();
        return;
    }
    else
    {
        if (buildCellWindow    != nullptr) buildCellWindow->writeSettings();
        if (cardImageView      != nullptr) cardImageView->writeSettings();
        if (view3DModelWindow  != nullptr) view3DModelWindow->writeSettings();
        if (modelVtkWindow     != nullptr) modelVtkWindow->makeDefaultView();
        if (materialMainWindow != nullptr) materialMainWindow->writeSettings();
//        if (ptrackMain         != nullptr) ptrackMain->writeSettings();
        writeSettings();
        closeFile();
        close();
    }
}

// ------------------------------------------------------------------------------------------------
void MainWindow::doCloseQuit()
{
    handleSaveProject();

    if (buildCellWindow    != nullptr) buildCellWindow->writeSettings();
    if (cardImageView      != nullptr) cardImageView->writeSettings();
    if (view3DModelWindow  != nullptr) view3DModelWindow->writeSettings();
    if (materialMainWindow != nullptr) materialMainWindow->writeSettings();
//    if (ptrackMain         != nullptr) ptrackMain->writeSettings();
    writeSettings();
    closeFile();
    close();
}

// ------------------------------------------------------------------------------------------------
void MainWindow::closeFile()
{
//    qDebug() << "MainWindow:: closeFile" << endl;
    masterData.clear();
    if (buildCellWindow != nullptr)
    {
        buildCellWindow->closeFileOperations();  // may not need this
        buildCellWindow->~BuildCellWindow();
        buildCellWindow = nullptr;
    }
    if (view3DModelWindow != nullptr)
    {
        view3DModelWindow->resetForProject();
        view3DModelWindow->~View3DModel();
        view3DModelWindow = nullptr;
    }
    if (embeddedMeshWindow != nullptr)
    {
        embeddedMeshWindow->~EmbeddedMeshBuilder();
        embeddedMeshWindow = nullptr;
    }
    if (modelVtkWindow != nullptr)
    {
        modelVtkWindow->makeDefaultView();
        modelVtkWindow->resetForProject();
    }
    if (materialMainWindow != nullptr)
    {
        materialMainWindow->~MaterialMainWindow();
        materialMainWindow = nullptr;
    }

//    if (mainModelWindow   != nullptr)
//    {
//        mainModelWindow->handleCloseProject();
//    }
//    if (ptrackMain != nullptr)
//    {
//        ptrackMain->~PtracMain();
//        ptrackMain = nullptr;
//    }

    statusFileName = "no file loaded ";
//    statusFileNameLabel->setText(statusFileName);

    abaqusFiles.clear();
    bodyBasics.clear();
    masterCellList.clear();
    ptracPointData.clear();
}
// ------------------------------------------------------------------------------------------------
void MainWindow::writeSettings()
{
    QSettings settings(QApplication::organizationName(),  QApplication::applicationName());
    settings.setValue("CGTMB/version",                    QApplication::applicationVersion());
    settings.setValue("currentPath",                      this->currentPath);
    settings.setValue("mainWindow/size",                  this->size());
    settings.setValue("mainWindow/Properties",            this->saveState());
    settings.setValue("mainWindow/Geometry",              this->saveGeometry());
    settings.setValue("recentProjectFiles",   			  recentProjectFiles);
}
// ------------------------------------------------------------------------------------------------
void MainWindow::readSettings()
{
//    qDebug() << " readSettings";

    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    settingsVersion   = settings.value("CGTMB/version", "").toString();
    this->currentPath = settings.value("currentPath", ".").toString();
    resize( settings.value("mainWindow/size", sizeHint()).toSize() );
    this->restoreState(settings.value("mainWindow/Properties").toByteArray());
    this->restoreGeometry(settings.value("mainWindow/Geometry").toByteArray());
//    license.accepted   = settings.value("license/accept", false).toBool();
    recentProjectFiles = settings.value("recentProjectFiles").toStringList();
    updateRecentProjectFileActions();
}

// ----------------------------------------------------------------------------
void MainWindow::onAddNew()
{
/*
    mainModelWindow = new MainModelWindow(&masterData, &bodyBasics, &masterCellList, &abaqusFiles);
    setCentralWidget(mainModelWindow);

    connect(mainModelWindow, SIGNAL(windowSizeChanged(const int)),
            this, SLOT(handleSizeChange(const int)));

    connect(mainModelWindow, SIGNAL(squareWindowSize(const int)),
            this, SLOT(handleSquareWindow(const int)));

    connect(mainModelWindow,     SIGNAL(xRotationChanged(int)),
            this,                SLOT(saveXRotation(int)));
    connect(mainModelWindow,     SIGNAL(yRotationChanged(int)),
            this,                SLOT(saveYRotation(int)));
    connect(mainModelWindow,     SIGNAL(zRotationChanged(int)),
            this,                SLOT(saveZRotation(int)));
    connect(mainModelWindow,     SIGNAL(boundsChanged(Bounds)),
            this,                SLOT(save3DBounds(Bounds)));
*/
//	QWidget *tmpWidget = new QWidget();
//    tmpWidget->setMinimumSize(400, 400);

//    setCentralWidget(tmpWidget);

    modelVtkWindow = new BaseVtkWindow(&masterData);
    leftSplitter   = new QSplitter(Qt::Vertical);
    modelVtkWindow->makeVtkWidget(leftSplitter);
    leftSplitter->setMinimumSize(800, 800);
    setCentralWidget(leftSplitter);
}
// ------------------------------------------------------------------------------------------------
void MainWindow::handleSquareWindow(const int value)
{
    int width  = this->width();
    int height = this->height();

    int newSize;
    newSize = (width < height) ? width : height;

//    qDebug() << " MW:: newSize: " << newSize << width << height;

//    qDebug() << " width: " << width << "\t height : " << height;
//    qDebug() << " value: " << value << "\t newSize: " << newSize;

    this->resize(newSize, newSize);
}

// ------------------------------------------------------------------------------------------------
void MainWindow::handleSizeChange(const int value)
{
    int width = this->width();
    int height = this->height();

    width  += value;
    height += value;

//    qDebug() << "MainWindow::handleSizeChange :: width: " << width << "\theight: " << height;

    this->resize(width, height);
}
// ----------------------------------------------------------------------------
void MainWindow::createActions()
{
    // ----------------------
    // Actions for file menu.

    closeProjectAction = new QAction(tr("Close Project"), this);
    closeProjectAction->setStatusTip(tr("Close the CGTMB project file"));
    connect(closeProjectAction, SIGNAL(triggered()), this, SLOT(handleCloseProject()));

    openProject = new QAction(tr("&Open Project ..."), this);
    openProject->setStatusTip(tr("Open an existng CGTMB project"));
    connect(openProject, SIGNAL(triggered()), this, SLOT(handleOpenProject()));

    newProject = new QAction(tr("&New Project..."), this);
    newProject->setStatusTip(tr("Create a new CGTMB project"));
    connect(newProject, SIGNAL(triggered()), this, SLOT(handleNewProject()));

    saveProjectAction = new QAction(tr("&Save Project..."), this);
    saveProjectAction->setStatusTip(tr("Save CGTMB project file"));
    connect(saveProjectAction, SIGNAL(triggered()), this, SLOT(handleSaveProject()));

    saveAsProjectAction = new QAction(tr("&Save As Project..."), this);
    saveAsProjectAction->setStatusTip(tr("Save As CGTMB project file"));
    connect(saveAsProjectAction, SIGNAL(triggered()), this, SLOT(handleSaveAs()));

    exportITSAction = new QAction(tr("&ITS Card Image to File ..."), this);
    exportITSAction->setStatusTip(tr("Export ITS card image to ASCII file"));
    connect(exportITSAction, SIGNAL(triggered()), this, SLOT(handleITSExportCardImage()));

    exportXGENAction = new QAction(tr("&XGEN Card Image to File ..."), this);
    exportXGENAction->setStatusTip(tr("Export XGEN card image to ASCII file"));
    connect(exportXGENAction, SIGNAL(triggered()), this, SLOT(handleXGENExportCardImage()));


    exportMCNPAction = new QAction(tr("&MCNP Card Image to File ..."), this);
    exportMCNPAction->setStatusTip(tr("Export MCNP4/5/6 card image to ASCII file"));
    connect(exportMCNPAction, SIGNAL(triggered()), this, SLOT(handleMCNPExportCardImage()));

    if (license.allowEmbedded)
    {
        importAbaqusMeshAction = new QAction(tr("&Abaqus ..."), this);
        importAbaqusMeshAction->setStatusTip(tr("Import an Abaqus mesh file"));
        connect(importAbaqusMeshAction, SIGNAL(triggered()), this, SLOT(handleAbaqusImport()));
        importAbaqusMeshAction->setEnabled(false);
    }

    closeQuitProjectAction = new QAction(tr("Close / &Quit"), this);
    closeQuitProjectAction->setShortcut(tr("Ctrl+Q"));
    closeQuitProjectAction->setStatusTip(tr("Close & Exit the application"));
    connect(closeQuitProjectAction, SIGNAL(triggered()), this, SLOT(doCloseQuit()));

    projectSettingsAction = new QAction(tr("&Project Settings ..."), this);
    projectSettingsAction->setStatusTip(tr("Change project setttings"));
    connect(projectSettingsAction, SIGNAL(triggered()), this, SLOT(handleProjectSettings()));

    saveProjectAction->setEnabled(false);
    saveAsProjectAction->setEnabled(false);
    exportITSAction->setEnabled(false);
    exportMCNPAction->setEnabled(false);
    exportXGENAction->setEnabled(false);

    closeProjectAction->setEnabled(false);
    closeQuitProjectAction->setEnabled(false);

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl+X"));
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(doClose()));

    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        //qDebug() << "Adding eeout recent files: " << i;
        recentProjectFileActions[i] = new QAction(this);
        recentProjectFileActions[i]->setVisible(false);
        connect(recentProjectFileActions[i], SIGNAL(triggered()), this, SLOT(openRecentProjectFile()));
    }

    // -----------------------
    // Actions for tools menu.

    buildCellZone = new QAction(tr("Cell / Zone Builder"), this);
    connect(buildCellZone,    SIGNAL(triggered()), this, SLOT(launchBuildCellZone()));

    view3DModel = new QAction(tr("3-D Model View Controls"), this);
    connect(view3DModel,      SIGNAL(triggered()), this, SLOT(launchView3DModel()));

    materialWindow = new QAction(tr("Material Builder"), this);
    connect(materialWindow,      SIGNAL(triggered()), this, SLOT(launchMaterialBuilder()));

    if (license.allowEmbedded)
    {
        embeddedMeshAction = new QAction(tr("Embedded Mesh Builder"), this);
        connect(embeddedMeshAction, SIGNAL(triggered()), this, SLOT(launchEmbeddedBuilder()));
    }

    if (license.allowPtrack)
    {
        ptracAction = new QAction(tr("Ptrac Display"), this);
        connect(ptracAction, SIGNAL(triggered()), this, SLOT(launchPtracDisplay()));
    }

    aboutCGTMBaction = new QAction(tr("About My CGTMB"), this);
    aboutCGTMBaction->setStatusTip(tr("Show CGTMB version information"));
    connect(aboutCGTMBaction, SIGNAL(triggered()), this, SLOT(showCGTMBversion()));

    lightingAction = new QAction(tr("Color (light) control"), this);
    lightingAction->setStatusTip("Show lighting dialog.");
    connect(lightingAction, SIGNAL(triggered()), this, SLOT(handleLightingDialog()));

    cameraAction = new QAction(tr("Camera Orientation"), this);
    cameraAction->setStatusTip(tr("Show / Hide camera orientation widget."));
    connect(cameraAction, SIGNAL(triggered()), this, SLOT(handleCameraAction()));

    orientationAction = new QAction(tr("Orientation Axes"), this);
    orientationAction->setStatusTip(tr("Show / Hide moveable orientation axes."));
    connect(orientationAction, SIGNAL(triggered()), this, SLOT(handleOrientationAction()));
}
// ----------------------------------------------------------------------------
void MainWindow::createMenus()
{
    menuBar = new QMenuBar;
//    menuBar->setNativeMenuBar(false);

    fileMenu  = menuBar->addMenu(tr("&File  "));
    toolsMenu = menuBar->addMenu(tr("&Tools  "));

    newProjectSubMenu = fileMenu->addMenu(tr("New Project"));
    newProjectSubMenu->addAction(newProject);

    newProjectSubMenu = fileMenu->addMenu(tr("Open Project"));
    newProjectSubMenu->addAction(openProject);

    recentProjectFileMenu = fileMenu->addMenu(tr("Recent Project Files"));
    for (int i = 0; i < MaxRecentFiles; ++i)
        recentProjectFileMenu->addAction(recentProjectFileActions[i]);

    separatorAction = fileMenu->addSeparator();

    exportCardImageSubmenu = fileMenu->addMenu(tr("Export Card Image"));
    exportCardImageSubmenu->addAction(exportITSAction);
    exportCardImageSubmenu->addAction(exportMCNPAction);
    exportCardImageSubmenu->addAction(exportXGENAction);

//    if (license.allowEmbedded)
    {
        importMeshSubMenu = fileMenu->addMenu(tr("Import mesh"));
        importMeshSubMenu->addAction(importAbaqusMeshAction);
    }

    separatorAction = fileMenu->addSeparator();

    fileMenu->addAction(saveProjectAction);
    fileMenu->addAction(saveAsProjectAction);

    fileMenu->addAction(closeProjectAction);
    fileMenu->addAction(closeQuitProjectAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    toolsMenu->addAction(buildCellZone);
    toolsMenu->addAction(view3DModel);
//    if (license.allowXsdir  ||  license.allowPnnlMats)
    {
        toolsMenu->addAction(materialWindow);
    }
    if (license.allowEmbedded)
    {
        toolsMenu->addAction(embeddedMeshAction);
    }

//    if (license.allowPtrack)
    {
        toolsMenu->addAction(ptracAction);
    }

    separatorAction = toolsMenu->addSeparator();
    toolsMenu->addAction(projectSettingsAction);

    toolsMenu->setEnabled(false);

    helpMenu = menuBar->addMenu(tr("&Help"));
    helpMenu->addAction(aboutCGTMBaction);

    setMenuBar(menuBar);
}

// ------------------------------------------------------------------------------------------------
void MainWindow::updateRecentProjectFileActions()
{
    QMutableStringListIterator i(recentProjectFiles);
    while (i.hasNext())
    {
        if (!QFile::exists(i.next()))  i.remove();
    }

    for (int j = 0; j < MaxRecentFiles; ++j)
    {
        if (j < recentProjectFiles.count())
        {
            QString s1 = strippedName(recentProjectFiles[j]);
            QString text = tr("&%1) %2").arg(j + 1).arg(strippedName(recentProjectFiles[j]));
            recentProjectFileActions[j]->setText(text);
            recentProjectFileActions[j]->setData(recentProjectFiles[j]);
            recentProjectFileActions[j]->setVisible(true);
        }
        else
        {
            recentProjectFileActions[j]->setVisible(false);
        }
    }
    separatorAction->setVisible(!recentProjectFiles.isEmpty());
}

// ------------------------------------------------------------------------------------------------
QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

// ------------------------------------------------------------------------------------------------
void MainWindow::setRecentProjectFileList(const QString &fileName)
{
    if (!fileName.isEmpty())
    {
        recentProjectFiles.removeAll(fileName);
        recentProjectFiles.prepend(fileName);
        updateRecentProjectFileActions();
    }
}

// ------------------------------------------------------------------------------------------------
void MainWindow::openRecentProjectFile()
{
    //qDebug() << "openRecentEeoutFile -- ";

    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        QString fileName = action->data().toString();
        if (!fileName.isEmpty())
        {
            currentPath = QFileInfo(fileName).absolutePath();
            openProjectFile(fileName);
        }
    }
}
// ----------------------------------------------------------------------------
void MainWindow::launchBuildCellZone()
{
    //qDebug() << "launchBuildCellZone called \n";

    if (buildCellWindow == nullptr)
    {
        buildCellWindow = new BuildCellWindow(&masterData, &license);
        buildCellWindow->resize(buildCellWindow->sizeHint());

        connect(buildCellWindow, SIGNAL(cellListReady(int)), this, SLOT(handleCellListReady(int)));
        connect(buildCellWindow, SIGNAL(cellChangedSignal(int, int)),
                this,            SLOT(handleCellChangedSignal(int, int)));

        connect(buildCellWindow, SIGNAL(saveNeeded()), this, SLOT(handleSaveProject()));
    }
    buildCellWindow->show();
    buildCellWindow->raise();
    buildCellWindow->activateWindow();
}

// ----------------------------------------------------------------------------
void MainWindow::launchMaterialBuilder()
{
//    masterData.projectType = MCNP;

    if (materialMainWindow == nullptr)
    {
        materialMainWindow = new MaterialMainWindow(&masterData);
        materialMainWindow->resize(materialMainWindow->sizeHint());
    }
    materialMainWindow->show();
    materialMainWindow->raise();
    materialMainWindow->activateWindow();

}
// ----------------------------------------------------------------------------
void MainWindow::launchEmbeddedBuilder()
{

    if (embeddedMeshWindow == nullptr)
    {
        embeddedMeshWindow = new EmbeddedMeshBuilder(&masterData, &abaqusFiles);

        connect(embeddedMeshWindow, SIGNAL(showMeshUniverse(int)),
                this,               SLOT(handleShowMeshUniverse(int)));

        connect(embeddedMeshWindow, SIGNAL(updateMeshUniverse()),
                this,               SLOT(handleUpdateMeshList()));

        connect(embeddedMeshWindow, SIGNAL(showMeshUniverse(int)),
                this,               SLOT(handleMeshUniversePersistence(int)));
    }
    embeddedMeshWindow->show();
    embeddedMeshWindow->raise();
    embeddedMeshWindow->activateWindow();
}
// ----------------------------------------------------------------------------
void MainWindow::launchPtracDisplay()
{
/*
    if (ptrackMain == nullptr)
    {
        ptrackMain = new PtracMain(&masterData, &ptracPointData);

        connect(ptrackMain, SIGNAL(ptracShowSignal(int)),
                this,       SLOT(handlePtracShowSignal(int)));

        connect(ptrackMain, SIGNAL(ptracPersistenceSignal(int)),
                this,       SLOT(handlePtracPersistenceSignal(int)));

        connect(ptrackMain, SIGNAL(ptracGhostSignal(int)),
                this,       SLOT(handlePtracGhostSignal(int)));
    }
    ptrackMain->show();
    ptrackMain->raise();
    ptrackMain->activateWindow();
*/
}
// ----------------------------------------------------------------------------
void MainWindow::launchView3DModel()
{
//    qDebug() << "launchView3DModel called \n";


    if (view3DModelWindow == nullptr)
    {
        view3DModelWindow = new View3DModel(&masterData,     &bodyBasics,
                                            &masterCellList, &abaqusFiles,
                                            &model3DWidgetAngles[0]);
        view3DModelWindow->resize(view3DModelWindow->sizeHint());


        connect(view3DModelWindow, SIGNAL(modelUpdate()),
                this,              SLOT(handleModelUpdate()));

        connect(view3DModelWindow, SIGNAL(xCutawayChanged(double, int)),
                this,              SLOT(handleXCutawayChanged(double, int)));
        connect(view3DModelWindow, SIGNAL(yCutawayChanged(double, int)),
                this,              SLOT(handleYCutawayChanged(double, int)));
        connect(view3DModelWindow, SIGNAL(zCutawayChanged(double, int)),
                this,              SLOT(handleZCutawayChanged(double, int)));

                    /*
        connect(view3DModelWindow, SIGNAL(xAxisCullDirChange(int)),
                mainModelWindow,   SLOT(setXAxisCullDirChange(int)));
        connect(view3DModelWindow, SIGNAL(yAxisCullDirChange(int)),
                mainModelWindow,   SLOT(setYAxisCullDirChange(int)));
        connect(view3DModelWindow, SIGNAL(zAxisCullDirChange(int)),
                mainModelWindow,   SLOT(setZAxisCullDirChange(int)));

        connect(mainModelWindow,     SIGNAL(boundsChanged(Bounds)),
                view3DModelWindow,   SLOT(setBounds(Bounds)));

        connect(mainModelWindow,     SIGNAL(mainModelWindowMessage(QString)),
                view3DModelWindow,   SLOT(setMessage(QString)));
*/
    }
    view3DModelWindow->setBounds(saved3DBounds);
    view3DModelWindow->show();
    view3DModelWindow->raise();
    view3DModelWindow->activateWindow();
}

// ------------------------------------------------------------------------------------------------
void MainWindow::handleXCutawayChanged(double plane, int direction)
{
    modelVtkWindow->updateClippingSurfaces2(1, direction, plane);
}
// ------------------------------------------------------------------------------------------------
void MainWindow::handleYCutawayChanged(double plane, int direction)
{
    modelVtkWindow->updateClippingSurfaces2(2, direction, plane);
}
// ------------------------------------------------------------------------------------------------
void MainWindow::handleZCutawayChanged(double plane, int direction)
{
    modelVtkWindow->updateClippingSurfaces2(3, direction, plane);
}

// ----------------------------------------------------------------------------
void MainWindow::handleNewProject()
{
    //bool ok;

    //QInputDialog d; // = new QInputDialog;
    /*
     * The following 2 lines would not work to resize the dialog.
        d->resize(400, 200);
        d->setFixedWidth(400);

        It appears that the only way to widen the dialog is to pad the
        title with tab characters (\t) or other white space.
    */
    /*
    QString text = d.getText(this, tr("CGTMB Project Title Input"),
                                         tr("Project Title: \t\t\t\t\t"), QLineEdit::Normal,
                                         tr(""), &ok);

    if (ok && !text.isEmpty())
       */

    handleProjectSettings();
}
// ----------------------------------------------------------------------------
void MainWindow::handleSaveAs()
{
    handleSaveAs(0);
}
// ----------------------------------------------------------------------------
void MainWindow::handleSaveAs(const int mode)
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    currentPath = settings.value("currentPath").toString();
    QString format = ".cgtmb";
    QString initialPath;

    if (currentPath.isEmpty())
    {
        initialPath = QDir::currentPath() + tr("/untitled") + format;
    }
    else
    {
        initialPath = currentPath;
    }

    projectFileName = QFileDialog::getSaveFileName(this, tr("Save CGTMB Project File As"),
                                                   initialPath, tr("*.cgtmb;; *.*"));

    currentPath = QFileInfo(projectFileName).absolutePath();
    settings.setValue("currentPath", currentPath);

    if (mode == 0)
    {
        bool flag = masterData.saveToFile(projectFileName);
        if (flag)
        {
            needsSaved = false;
            setRecentProjectFileList(projectFileName);
        }
        else
        {
            fileSaveFailureMessage();
        }
    }
    updateStatusBar(projectFileName);
}
// ----------------------------------------------------------------------------
void MainWindow::handleSaveProject()
{
    bool flag;
    if (projectFileName.length() > 0)
    {
        flag = masterData.saveToFile(projectFileName);
    }
    else
        handleSaveAs(0);

    if (flag)
    {
        needsSaved = false;
        setRecentProjectFileList(projectFileName);
    }
    else
    {
        fileSaveFailureMessage();
    }
    updateStatusBar(projectFileName);
}
// ----------------------------------------------------------------------------
void MainWindow::fileSaveFailureMessage()
{
    QFont courierFont("Courier", 14);
    QMessageBox about;
    QString str = " Project file save rejected! \n\n";
    if (masterData.projectType == MCNP)
    {
        str += "At least 1 cell must be created.\n";
    }
    else
    {
        str += "At least 1 zone must be created.\n";
    }
    about.setIcon(QMessageBox::Critical);
    about.setFont(courierFont);
    about.setInformativeText(str);
    about.setStandardButtons(QMessageBox::Ok);
    about.setDefaultButton(QMessageBox::Ok);
    about.setBaseSize(300, 180);
    about.show();
    about.exec();
    return;
}

// ----------------------------------------------------------------------------
void MainWindow::handleOpenProject()
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    currentPath = settings.value("currentPath").toString();
    QString format = ".cgtmb";
    QString initialPath;

    if (currentPath.isEmpty())
    {
        initialPath = QDir::currentPath() + tr("/untitled") + format;
    }
    else
    {
        initialPath = currentPath;
    }

    projectFileName = QFileDialog::getOpenFileName(this, tr("Open CGTMB Project File"), initialPath,
                                     tr("*.cgtmb;; *.*"));

    currentPath = QFileInfo(projectFileName).absolutePath();
    settings.setValue("currentPath", currentPath);

    openProjectFile(projectFileName);
    setRecentProjectFileList(projectFileName);
}
// ----------------------------------------------------------------------------
void MainWindow::openProjectFile(QString fileName)
{
    bool ok = masterData.readFromFile(fileName);

    if (ok)
    {
        toolsMenu->setEnabled(true);
        saveProjectAction->setEnabled(true);
        saveAsProjectAction->setEnabled(true);
        closeProjectAction->setEnabled(true);
        closeQuitProjectAction->setEnabled(true);
        exportITSAction->setEnabled(true);
        exportMCNPAction->setEnabled(true);
        if (license.allowEmbedded) importAbaqusMeshAction->setEnabled(true);
        newProject->setEnabled(false);
        openProject->setEnabled(false);
        recentProjectFileMenu->setEnabled(false);
        setRecentProjectFileList(fileName);
        projectFileName = fileName;
        updateStatusBar(fileName);
        newProjectFlag = false;
        needsSaved = true;
        doMeshImports();

//        if (mainModelWindow != nullptr)
 //       {
//            mainModelWindow->buildModelZones();
//        }

        if (masterData.projectType == MCNP)
        {
            buildCellZone->setText("Cell Builder");
        }
        else if (masterData.projectType == ITS)
        {
            buildCellZone->setText("Zone Builder");
        }
        else
        {
            buildCellZone->setText("Cell / Zone Builder");
        }

        if (masterData.projectType == ITS)
        {
            exportXGENAction->setEnabled(true);
        }
        else
        {
            exportXGENAction->setEnabled(false);
        }
    }
}
// ----------------------------------------------------------------------------
void MainWindow::handleCloseProject()
{
    newProject->setEnabled(true);
    openProject->setEnabled(true);
    recentProjectFileMenu->setEnabled(true);
    closeProjectAction->setEnabled(false);
    closeQuitProjectAction->setEnabled(false);
    saveProjectAction->setEnabled(false);
    saveAsProjectAction->setEnabled(false);
    exportITSAction->setEnabled(false);
    exportMCNPAction->setEnabled(false);
    exportXGENAction->setEnabled(false);
    if (license.allowEmbedded)  importAbaqusMeshAction->setEnabled(false);
    toolsMenu->setEnabled(false);
    needsSaved     = false;
    newProjectFlag = true;

    closeFile();
}
// ----------------------------------------------------------------------------
void MainWindow::handleExportCardImage(int which)
{

    if (cardImageView == nullptr)
    {
        cardImageView = new CardImageView(this, true, true, false, true);
        cardImageView->resize(cardImageView->sizeHint());
    }
    whichCardImage = which;
    cardImageView->generateView(&masterData, which);
    cardImageView->show();
    cardImageView->raise();
    cardImageView->activateWindow();
    cardImageView->readSettings();
}

// ----------------------------------------------------------------------------
void MainWindow::handleXGENExportCardImage()
{
    handleExportCardImage(XGEN);
}

// ----------------------------------------------------------------------------
void MainWindow::handleITSExportCardImage()
{
    handleExportCardImage(ITS);
}
// ----------------------------------------------------------------------------
void MainWindow::handleMCNPExportCardImage()
{
    handleExportCardImage(MCNP);
}
// ----------------------------------------------------------------------------
void MainWindow::handleCellChangedSignal(int val, int cell)
{
//    mainModelWindow->updateModel(val, cell);
}
// ----------------------------------------------------------------------------
void MainWindow::handleCellListReady(int val)
{
    needsSaved = true;
    if (view3DModelWindow != nullptr) view3DModelWindow->handleUpdate();
}
// ----------------------------------------------------------------------------
void MainWindow::handleUpdateMeshList()
{
    needsSaved = true;
    if (view3DModelWindow != nullptr) view3DModelWindow->handleUpdate();
}
// ----------------------------------------------------------------------------
void MainWindow::handleModelUpdate()
{
 //   mainModelWindow->updateModel(newProjectFlag);
    modelVtkWindow->updateEntireModel(&modelBounds, &abaqusFiles);

    if (view3DModelWindow != nullptr) view3DModelWindow->updateModelBounds(&modelBounds);
//    modelVtkWindow->updateEntireModel(&modelBounds, &abaqusFiles);

}
// ------------------------------------------------------------------------------------------------
void MainWindow::handleProjectSettings()
{
    if (projectSettings == nullptr)
    {
        projectSettings = new ProjectSettings(&masterData, &license);
        projectSettings->resize(projectSettings->sizeHint());
        connect(projectSettings, SIGNAL(projectSettingsUpdated(int)),
                this,            SLOT(handleProjectSettingsUpdate(int)));
    }

    projectSettings->show();
    projectSettings->raise();
    projectSettings->activateWindow();
    projectSettings->readSettings();

    if (newProjectFlag)
    {
        projectSettings->setDefaults();
        projectFileName = "";
    }
    else
        projectSettings->setValues();

    // By now, either an old project has been opened or project info has
    // been processed for the new project -- hence consider everything old.
    newProjectFlag = false;
}

// ------------------------------------------------------------------------------------------------
void MainWindow::handleProjectSettingsUpdate(int val)
{
    if (val == 1  &&  projectFileName == "") handleSaveAs(1);
    projectSettingsChanged = (bool) val;
    updateMenus();
    if (cardImageView      != nullptr) cardImageView->generateView(&masterData, whichCardImage);
    if (buildCellWindow    != nullptr) buildCellWindow->resetForProject();
    if (view3DModelWindow  != nullptr) view3DModelWindow->resetForProject();
    if (embeddedMeshWindow != nullptr) embeddedMeshWindow->resetForProject();
    if (modelVtkWindow     != nullptr) modelVtkWindow->resetForProject();

}
// ------------------------------------------------------------------------------------------------
void MainWindow::updateMenus()
{
    if (projectSettingsChanged)
    {
        //qDebug() << "Project title: " << text;
        toolsMenu->setEnabled(true);
        saveProjectAction->setEnabled(true);
        saveAsProjectAction->setEnabled(true);
        closeProjectAction->setEnabled(true);
        closeQuitProjectAction->setEnabled(true);
        exportITSAction->setEnabled(true);
        exportMCNPAction->setEnabled(true);
        if (license.allowEmbedded)  importAbaqusMeshAction->setEnabled(true);
        newProject->setEnabled(false);
        openProject->setEnabled(false);
        needsSaved = true;

        if (masterData.projectType == MCNP)
        {
            buildCellZone->setText("Cell Builder");
        }
        else if (masterData.projectType == ITS)
        {
            buildCellZone->setText("Zone Builder");
        }
        else
        {
            buildCellZone->setText("Cell / Zone Builder");
        }

        if (masterData.projectType == ITS)
        {
            exportXGENAction->setEnabled(true);
        }
        else
        {
            exportXGENAction->setEnabled(false);
        }
    }
}
// ------------------------------------------------------------------------------------------------
void MainWindow::createStatusBar()
{
    QPalette pal2;
    pal2.setColor( QPalette::Foreground, QColor(204,0,50) );
    statusFileName = "no file loaded ";
    statusFileNameLabel = new QLabel(statusFileName);
    statusFileNameLabel->setPalette(pal2);
    statusBar()->addPermanentWidget(statusFileNameLabel);
    statusBar()->showMessage(tr("Ready"));
}
// ------------------------------------------------------------------------------------------------
void MainWindow::updateStatusBar(QString fileName)
{
    if (!fileName.startsWith("no project file"))
    {
        QString name;
//        if (tvDefaults->showFullPath)
//        {
            name = fileName;
//        }
//        else
//        {
//            name  = QFileInfo(fileName).baseName();
//        }
        name = name + " ";
        statusFileNameLabel->setText(name);
    }
}
// ------------------------------------------------------------------------------------------------
void MainWindow::showCGTMBversion()
{
    showCGTMBversion(0);
}
// ------------------------------------------------------------------------------------------------
void MainWindow::showCGTMBversion(unsigned char which)
{
    //qDebug() << "-- showTVversion --";

    QString s1, str, t1;

    if (which == 0)
    {
        t1  = QCoreApplication::applicationDirPath();

//        s1.sprintf("%d", license.days2Expire);
        str = "CGTMB Version ";
        str += QApplication::applicationVersion();

//        str += "\n\nLicense expiration date: " + license.expDate.toString();
        str += "\n" + s1 + " days to expiration date.";

        str += "\n\nLicense path: " + t1;

//        str += "\n\nAuthorized Moniker: " + license.moniker;

        str += "\n\nCopyright 2020-2022 Stag Scientific Software, LLC\nAll rights reserved";

        str += "\n\nCGTMB License Agreement\n\n";

        t1 = "About CGTMB";
    }
    else
    {
        t1 = "CGTMB License Agreement";
    }

/*
    QFile FileAgree(":/Resources/license_agreement_march_2021.txt");
    FileAgree.open(QFile::ReadOnly);
    QString agreeString = QLatin1String(FileAgree.readAll());
    FileAgree.close();

    agreeString = str + agreeString;

    agreementWindow = new SSS_TextViewDialog(this, which);
    agreementWindow->setModal(true);
    agreementWindow->setWindowTitle(t1);
    agreementWindow->displayTextString(agreeString);
    connect(agreementWindow, SIGNAL(textWindowAnswerSignal(bool)),  this, SLOT(handleTextWindowAnswerSignal(bool)));
    agreementWindow->exec();
*/
}
// ------------------------------------------------------------------------------------------------
void MainWindow::handleTextWindowAnswerSignal(bool agree)
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    int i = (agree ==  true) ? 1:0;
    settings.setValue("license/accept", i);
    if (!agree)
    {
        //qDebug() << "handleTextWindowAnswerSignal:: agree: " << agree;
        exit(-13);
    }
}
// ------------------------------------------------------------------------------------------------
void MainWindow::handleAbaqusImport()
{
    QString errorString;
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    currentPath = settings.value("currentPath").toString();
    QString initialPath;

    if (currentPath.isEmpty())
    {
        initialPath = QDir::currentPath() + tr("/untitled");
    }
    else
    {
        initialPath = currentPath;
    }

    QString abaqusFileName = QFileDialog::getOpenFileName(this, tr("Open Abaqus Mesh Input File As"),
                                                           initialPath, tr("*.inp;; *.abq;; *.abqinp;; *.abaqus;; *.*"));

    if (!masterData.checkAbaqusFileName(abaqusFileName))
    {
        if (abaqusFileName.isEmpty())
        {
            QString s1 = "File name is empty.  Please try again.";
            QMessageBox *msg = new QMessageBox(this);
            msg->setIcon(QMessageBox::Information);
            msg->setInformativeText(s1);
            msg->setStandardButtons(QMessageBox::Ok);
            msg->setDefaultButton(QMessageBox::Ok);
            msg->show();
            msg->exec();
        }
        else
        {
            currentPath = QFileInfo(abaqusFileName).absolutePath();
            masterData.abaqusFileStringList.push_back(abaqusFileName);
            ReadFileAbaqus *abaqus = new ReadFileAbaqus();

            QString s1 = "Reading Abaqus file " + abaqusFileName;
            s1 += " may take some time.\n\n No progress meter available at this time.";
            QMessageBox *msg = new QMessageBox(this);
            msg->setIcon(QMessageBox::Information);
            msg->setInformativeText(s1);
            msg->setStandardButtons(QMessageBox::Ok);
            msg->setDefaultButton(QMessageBox::Ok);
            msg->show();
            msg->exec();

            int i = abaqus->readAbaqusFile(abaqusFileName, currentPath, errorString);
            if (i == -1)
            {
                QMessageBox::information(this, tr("Unable to open file"), errorString);
            }
            else
            {
                abaqusFiles.push_back(*abaqus);
                QString s = QFileInfo(abaqusFileName).fileName();
                masterData.initialAddToInstanceDataStringList(abaqus, s);

                if (embeddedMeshWindow != nullptr)
                {
                    embeddedMeshWindow->updateEmbeddedMesh();
                }
                if (view3DModelWindow != nullptr)
                {
                    view3DModelWindow->displayMeshDock();
                }
                s1 = "Abaqus file processing complete. \n\nClick OK to continue.";
                msg->setInformativeText(s1);
                msg->setStandardButtons(QMessageBox::Ok);
                msg->setDefaultButton(QMessageBox::Ok);
                msg->show();
                msg->exec();
            }
        }
    }
    else
    {
        QString s1 = "File " + abaqusFileName;
        s1 += " is already part of the project.  Cannot add it again.";
        QMessageBox msg;
        msg.setIcon(QMessageBox::Information);
        msg.setInformativeText(s1);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.show();
        msg.exec();
    }
}
// ------------------------------------------------------------------------------------------------
void MainWindow::doMeshImports()
{
    QString errorString;

    for (int i=0; i < (int) masterData.abaqusFileStringList.size(); ++i)
    {
        ReadFileAbaqus *abaqus = new ReadFileAbaqus();
        QString abaqusFileName = masterData.abaqusFileStringList[i];
        abaqusFileName.trimmed();
        int k = abaqus->readAbaqusFile(abaqusFileName, currentPath, errorString);
        if (k == -1)
        {
            QMessageBox::information(this, tr("Unable to open file"), errorString);
        }
        else
            abaqusFiles.push_back(*abaqus);
    }
}
// ------------------------------------------------------------------------------------------------
void MainWindow::handleShowMeshUniverse(int universe)
{
    modelVtkWindow->showMeshAssembly(&abaqusFiles[universe], &modelBounds, universe);
    if (view3DModelWindow != nullptr) view3DModelWindow->updateModelBounds(&modelBounds);
    else
    {
        saved3DBounds = modelBounds;
    }
}

// ------------------------------------------------------------------------------------------------
void MainWindow::handleMeshUniverseTransparancy(double transparancy)
{
//    mainModelWindow->showMeshUniverse(transparancy);
}

// ------------------------------------------------------------------------------------------------
void MainWindow::handleMeshUniversePersistence(int persistence)
{
//    mainModelWindow->showMeshUniverse(persistence);
}

// ------------------------------------------------------------------------------------------------
void MainWindow::saveXRotation(int angle)
{
//    model3DWidgetAngles[0] = angle;
}
// ------------------------------------------------------------------------------------------------
void MainWindow::saveYRotation(int angle)
{
//    model3DWidgetAngles[1] = angle;
}
// ------------------------------------------------------------------------------------------------
void MainWindow::saveZRotation(int angle)
{
//    model3DWidgetAngles[2] = angle;
}
// ------------------------------------------------------------------------------------------------
void MainWindow::save3DBounds(Bounds b)
{
    saved3DBounds = b;
}
// ------------------------------------------------------------------------------------------------
void MainWindow::handlePtracShowSignal(int sig)
{
//    mainModelWindow->showPtracData(sig, &ptracPointData);
}
// ------------------------------------------------------------------------------------------------
void MainWindow::handlePtracPersistenceSignal(int sig)
{
//    mainModelWindow->handlePtracPersistence(sig);
}
// ------------------------------------------------------------------------------------------------
void MainWindow::handlePtracGhostSignal(int sig)
{
//    mainModelWindow->handleGhostGeometry(sig);
}
// ------------------------------------------------------------------------------------------------
void MainWindow::makeToolBar()
{
    // Create the icons.
    this->setIconSize(QSize(22, 22));

    const QIcon icon5 = QIcon(QString::fromUtf8(":/Icons/camera-100.png"));
    cameraAction->setIcon(icon5);

    const QIcon icon6 = QIcon(QString::fromUtf8(":/Icons/coordinate-system-50.png"));
    orientationAction->setIcon(icon6);

    const QIcon icon7 = QIcon(QString::fromUtf8(":/Icons/flashlight-64.png"));
    lightingAction->setIcon(icon7);

    const QIcon icon1 = QIcon(QString::fromUtf8(":/Icons/filesave.png"));
    saveProjectAction->setIcon(icon1);

    // Create the toolbar.
    toolBar = new QToolBar(this);
    toolBar->setObjectName(QStringLiteral("toolBar"));
    toolBar->setOrientation(Qt::Horizontal);
    toolBar->setIconSize(QSize(50, 50));
    this->addToolBar(Qt::TopToolBarArea, toolBar);

    // Adding actions to the toolbar adds the icons there.

    toolBar->addAction(saveProjectAction);
    toolBar->addSeparator();
    toolBar->addAction(orientationAction);
    toolBar->addAction(cameraAction);
    toolBar->addAction(lightingAction);
}

// ------------------------------------------------------------------------------------------------
void MainWindow::handleLightingDialog()
{
    if (lightSource == nullptr)
    {
        QString s = "Model Window Lighting Control";
        lightSource = new SSSLightSource(s, &lightingControl);

        connect(lightSource, SIGNAL(updateNeededSignal()), this, SLOT(handleLightSource()));
    }
    lightSource->show();
    lightSource->raise();
    lightSource->activateWindow();
}
// ------------------------------------------------------------------------------------------------
void MainWindow::handleLightSource()
{
//    qDebug() << "Ambient: " << lightingControl.source.ambient.x();
//    qDebug() << "Diffuse: " << lightingControl.source.diffuse.x();
    double values[4];
    values[0] = lightingControl.source.ambient.x();
    values[1] = lightingControl.source.diffuse.x();
    values[2] = lightingControl.material.whiteWash;
    values[3] = lightingControl.material.transparancy;
    modelVtkWindow->updateLight(values, lightingControl.control.renderStyle);
}
// ------------------------------------------------------------------------------------------------
void MainWindow::handleOrientationAction()
{
    modelVtkWindow->toggleOrientationWidget();
}
// ------------------------------------------------------------------------------------------------
void MainWindow::handleCameraAction()
{
    modelVtkWindow->toggleCamOrientationWidget();
}
