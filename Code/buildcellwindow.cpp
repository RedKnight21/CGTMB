#include "buildcellwindow.h"

BuildCellWindow::BuildCellWindow(MasterData *mdIn, License *inLic, QWidget *parent) : QMainWindow(parent)
{
    progressBar = new QProgressBar(this);
    progressBar->setVisible(false);
    license = inLic;

    md = mdIn;
    projectType = md->projectType;

    lightSource         = nullptr;
    bodyControlsWidget  = nullptr;
    cellVtkWindow       = nullptr;
    bodyControlsWidget  = new BuildBodyCellControls(md, license, progressBar, &cgBodyBasics, &cgZoneOps, &cellBounds);

    buildBottomLeftWidget();

    leftSplitter  = new QSplitter(Qt::Vertical);
    cellVtkWindow = new BaseVtkWindow(md);
    cellVtkWindow->makeVtkWidget(leftSplitter);

//    leftSplitter->addWidget(bodyControlsWidget);
//    leftSplitter->addWidget(bottomLeftWidget);

    rightSplitter = new QSplitter(Qt::Vertical);
    rightSplitter->addWidget(bodyControlsWidget);
    rightSplitter->addWidget(bottomLeftWidget);


    mainSplitter = new QSplitter(Qt::Horizontal);
    mainSplitter->addWidget(leftSplitter);
//    mainSplitter->addWidget(bodyControlsWidget);
    mainSplitter->addWidget(rightSplitter);

    setCentralWidget(mainSplitter);

    readSettings();

    setupSlots();
    handleClearTextEdit();
    setWindowTitle("Cell Builder Window");

    makeActions();
    makeToolBar();

    statusBar()->showMessage(tr("Status bar"));
    statusBar()->addPermanentWidget(progressBar);
}
// ----------------------------------------------------------------------------
void BuildCellWindow::resetForProject()
{
    projectType = md->projectType;
    if (projectType == MCNP)
    {
        setWindowTitle("Cell Builder Window");
    }
    else
    {
        setWindowTitle("Zone Builder Window");
    }
    bodyControlsWidget->resetForProject();
}
// ----------------------------------------------------------------------------
void BuildCellWindow::createDockWidgets()
{    
     QDockWidget *dock2 = new QDockWidget(tr("Cell Message Window"), this);
     dock2->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);
     dock2->setWidget(bottomLeftWidget);
     addDockWidget(Qt::BottomDockWidgetArea, dock2);
     messageAction = dock2->toggleViewAction();

     QDockWidget *dock = new QDockWidget(tr("3-D Cell Window"), this);
     dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
     dock->setWidget(leftSplitter);
     addDockWidget(Qt::LeftDockWidgetArea, dock);
     plotAction = dock->toggleViewAction();
}

// ----------------------------------------------------------------------------
BuildCellWindow::~BuildCellWindow()
{

    if (lightSource != nullptr) lightSource->~SSSLightSource();

//    qDebug() << "BuildCellWindow::~BuildCellWindow" << endl;


//    if (cellGraphicsWidget != nullptr)
//    {
//        cellGraphicsWidget->~BuildCellGraphics();
//    }
//    if (bodyControlsWidget != nullptr)
//    {
//        bodyControlsWidget->~BuildBodyCellControls();
//    }
}
// ----------------------------------------------------------------------------
QSize BuildCellWindow::minimumSizeHint() const
{
    return QSize(500, 500);
}
// ----------------------------------------------------------------------------

QSize BuildCellWindow::sizeHint() const
{
#ifdef USING_WINDOWS
    return QSize(1500, 500);
#else
    return QSize(2000, 1000);
#endif
}
// ------------------------------------------------------------------------------------------------
void BuildCellWindow::writeSettings()
{
    //qDebug() << "\n  BuildCellWindow::writeSettings \n";
    //qDebug() << "BuildCellWindow:: size: " << this->size() << "\n";

    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    settings.setValue("buildCellWindow/size",       this->size());
    settings.setValue("buildCellWindow/Properties", this->saveState());
    settings.setValue("buildCellWindow/Geometry",   this->saveGeometry());
    settings.setValue("buildCellWindow/mainSplitter-geom",  mainSplitter->saveGeometry());
    settings.setValue("buildCellWindow/mainSplitter-state", mainSplitter->saveState());
    settings.setValue("buildCellWindow/leftSplitter-state", leftSplitter->saveState());
    settings.setValue("buildCellWindow/leftSplitter-geom",  leftSplitter->saveGeometry());
}
// ------------------------------------------------------------------------------------------------
void BuildCellWindow::readSettings()
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    this->restoreState(settings.value("buildCellWindow/Properties").toByteArray());
    this->restoreGeometry(settings.value("buildCellWindow/Geometry").toByteArray());
    resize( settings.value("buildCellWindow/size", sizeHint()).toSize() );

    leftSplitter->restoreState(settings.value("buildCellWindow/leftSplitter-state").toByteArray());
    mainSplitter->restoreState(settings.value("buildCellWindow/mainSplitter-state").toByteArray());

    leftSplitter->restoreGeometry(settings.value("buildCellWindow/leftSplitter-geom").toByteArray());
    mainSplitter->restoreGeometry(settings.value("buildCellWindow/mainSplitter-geom").toByteArray());

    //qDebug() << "BuildCellWindow:: readSettings:: size: " <<
    //            settings.value("buildCellWindow/size", sizeHint()).toSize();
}
// ------------------------------------------------------------------------------------------------
void BuildCellWindow::doClose()
{
 //   qDebug() << "\n ** BuildCellWindow:: doClose \n\n";

    writeSettings();
    close();
}
// ------------------------------------------------------------------------------------------------
void BuildCellWindow::handleZoneReadyToShow()
{
    //qDebug() << "BuildCellWindow ::  " <<  "\t MaxX: " << zone->nodesMaxX << "\t minX: " << zone->nodesMinX;

    int nBodies = (int) cgBodyBasics.size() + 1;

    int maxColors = 255;
    vector<int> red(nBodies);
    vector<int> gre(nBodies);
    vector<int> blu(nBodies);
    setRGBcolors(maxColors, nBodies, red, gre, blu);

    vector<double> redF(nBodies);
    vector<double> greF(nBodies);
    vector<double> bluF(nBodies);

    float norm = 1.0 / ((float) maxColors);
    for(int n=0; n<nBodies; ++n)
    {
        redF[n] = ((double) red[n]) * norm;
        greF[n] = ((double) gre[n]) * norm;
        bluF[n] = ((double) blu[n]) * norm;
    }

    cellVtkWindow->showOneZone(&cgBodyBasics, &cgZoneOps, &redF, &greF, &bluF, &cellBounds);
}
// ------------------------------------------------------------------------------------------------
void BuildCellWindow::buildBottomLeftWidget()
{
    closePB = new QPushButton("CLOSE");
    closePB->setToolTip("Click to close this window.");
    connect(closePB, SIGNAL(released()), this, SLOT(doClose()));

    clearPB = new QPushButton("CLEAR");
    clearPB->setToolTip("Click to clear the text pane window.");

    unclipCB = new QCheckBox("Unclip Cell");
    unclipCB->setToolTip("Select to unclip all bodies in the cell");
    unclipCB->setChecked(false);

    QHBoxLayout *hLayout = new QHBoxLayout;

    hLayout->addWidget(clearPB);
    hLayout->addStretch();
    hLayout->addWidget(unclipCB);
    hLayout->addStretch();
    hLayout->addWidget(closePB);

    bottomLeftTextEdit = new QTextEdit;
    bottomLeftTextEdit->setReadOnly(false);

#ifdef USING_WINDOWS
    int fontSize       = 10;
#else
    int fontSize       = 14;
 #endif
    QFont courierFont("Courier", fontSize);
    bottomLeftTextEdit->setCurrentFont(courierFont);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(bottomLeftTextEdit);
    vLayout->addLayout(hLayout);

    bottomLeftWidget = new QWidget;
    bottomLeftWidget->setLayout(vLayout);
}
// ------------------------------------------------------------------------------------------------
void BuildCellWindow::setupSlots()
{

    connect(bodyControlsWidget, SIGNAL(zoneReadyToShow()),
            this,               SLOT(handleZoneReadyToShow()));
    connect(bodyControlsWidget, SIGNAL(cellListReady(int)), this, SLOT(handleCellListReady(int)));

    connect(bodyControlsWidget, SIGNAL(xCutawayChanged(double, int)), this, SLOT(handleXCutAwayChanged(double, int)));
    connect(bodyControlsWidget, SIGNAL(yCutawayChanged(double, int)), this, SLOT(handleYCutAwayChanged(double, int)));
    connect(bodyControlsWidget, SIGNAL(zCutawayChanged(double, int)), this, SLOT(handleZCutAwayChanged(double, int)));



/*
    connect(bodyControlsWidget, SIGNAL(zoneReadyToShow(CGZone2 *, int)),
            this,               SLOT(handleZoneReadyToShow(CGZone2 *, int)));

    connect(bodyControlsWidget, SIGNAL(cellChangedSignal(int, int)),
            this,               SLOT(handleCellChangedSignal(int, int)));

    connect(bodyControlsWidget,  SIGNAL(xRotationChanged(int)),  cellGraphicsWidget,  SLOT(setXRotation(int)));
    connect(bodyControlsWidget,  SIGNAL(yRotationChanged(int)),  cellGraphicsWidget,  SLOT(setYRotation(int)));
    connect(bodyControlsWidget,  SIGNAL(zRotationChanged(int)),  cellGraphicsWidget,  SLOT(setZRotation(int)));


    connect(bodyControlsWidget, SIGNAL(xAxisCullDirChange(int)), this, SLOT(handleXAxisCullDirChange(int)));
    connect(bodyControlsWidget, SIGNAL(yAxisCullDirChange(int)), this, SLOT(handleYAxisCullDirChange(int)));
    connect(bodyControlsWidget, SIGNAL(zAxisCullDirChange(int)), this, SLOT(handleZAxisCullDirChange(int)));

    connect(bodyControlsWidget, SIGNAL(bodyCellControlMessage(QString)), this, SLOT(handleMessages(QString)));

    connect(bodyControlsWidget, SIGNAL(centerOnXYZPosition(int, double, double, double)),
            this,               SLOT(handleCenterOnXYZ(int, double, double, double)));

    connect(cellGraphicsWidget,  SIGNAL(xRotationChanged(int)),  this,  SLOT(handleXRotation(int)));
    connect(cellGraphicsWidget,  SIGNAL(yRotationChanged(int)),  this,  SLOT(handleYRotation(int)));
    connect(cellGraphicsWidget,  SIGNAL(zRotationChanged(int)),  this,  SLOT(handleZRotation(int)));

    connect(cellGraphicsWidget, SIGNAL(windowSizeChanged(const int)),
            this, SLOT(handleSizeChange(const int)));

    connect(cellGraphicsWidget, SIGNAL(graphicsWidgetMessage(QString)),
            this, SLOT(handleMessages(QString)));
*/
    connect(clearPB,  SIGNAL(pressed()),         this, SLOT(handleClearTextEdit()) );
    connect(unclipCB, SIGNAL(stateChanged(int)), this, SLOT(handleUnclipCB(int)) );

}
// ------------------------------------------------------------------------------------------------
void BuildCellWindow::handleSizeChange(const int value)
{
    int width = this->width();
    int height = this->height();

    width  += value;
    height += value;
    this->resize(width, height);
}
// ------------------------------------------------------------------------------------------------
void BuildCellWindow::handleCellChangedSignal(int val, int cell)
{
    emit(cellChangedSignal(val, cell));
}

// ------------------------------------------------------------------------------------------------
void BuildCellWindow::handleCellListReady(int val)
{
    emit cellListReady(val);
    if (val != 1)
    {
        cellVtkWindow->makeDefaultView();
    }
//    cellGraphicsWidget->reset();
}
// ------------------------------------------------------------------------------------------------
void BuildCellWindow::handleMessages(QString msg)
{
    bottomLeftTextEdit->append(msg);
    bottomLeftTextEdit->verticalScrollBar()->setValue( bottomLeftTextEdit->verticalScrollBar()->maximum() );
}
// ------------------------------------------------------------------------------------------------
void BuildCellWindow::handleClearTextEdit()
{
    bottomLeftTextEdit->clear();
    bottomLeftTextEdit->setText("CGTMB: Cell / zone builder messages\n\n");
}

// ------------------------------------------------------------------------------------------------
void BuildCellWindow::handleXRotation(int angle)
{
    //qDebug() << "BuildCellWindow:: X change: " << angle;
//    bodyControlsWidget->setXRotation(angle);
}
// ------------------------------------------------------------------------------------------------
void BuildCellWindow::handleYRotation(int angle)
{
    //qDebug() << "BuildCellWindow:: Y change: " << angle;
//    bodyControlsWidget->setYRotation(angle);
}
// ------------------------------------------------------------------------------------------------
void BuildCellWindow::handleZRotation(int angle)
{
    //qDebug() << "BuildCellWindow:: Z change: " << angle;
//    bodyControlsWidget->setZRotation(angle);
}
// ------------------------------------------------------------------------------------------------
void BuildCellWindow::handleXCutAwayChanged(double plane, int direction)
{
 //   cout << "X cut plane: " << plane << "\t direction " << direction << endl;
//    cellGraphicsWidget->setXCutawayPlane(plane);
    cellVtkWindow->updateClippingSurfaces(1, direction, plane);
}
// ------------------------------------------------------------------------------------------------
void BuildCellWindow::handleYCutAwayChanged(double plane, int direction)
{
//    cellGraphicsWidget->setYCutawayPlane(plane);
    cellVtkWindow->updateClippingSurfaces(2, direction, plane);
}
// ------------------------------------------------------------------------------------------------
void BuildCellWindow::handleZCutAwayChanged(double plane, int direction)
{
//    cellGraphicsWidget->setZCutawayPlane(plane);
    cellVtkWindow->updateClippingSurfaces(3, direction, plane);
}
// ------------------------------------------------------------------------------------------------
void BuildCellWindow::handleXAxisCullDirChange(int val)
{
//    cellGraphicsWidget->setXAxisCullDir(val);
}
// ------------------------------------------------------------------------------------------------
void BuildCellWindow::handleYAxisCullDirChange(int val)
{
//    cellGraphicsWidget->setYAxisCullDir(val);
}
// ------------------------------------------------------------------------------------------------
void BuildCellWindow::handleZAxisCullDirChange(int val)
{
    //qDebug() << "BuildCellWindow::handleZAxisCullDirChange: " << val;

//    cellGraphicsWidget->setZAxisCullDir(val);
}
// ------------------------------------------------------------------------------------------------
void BuildCellWindow::handleCenterOnXYZ(int i, double x, double y, double z)
{
//    cellGraphicsWidget->setModelCenter(i, x, y, z);
}
// ------------------------------------------------------------------------------------------------
void BuildCellWindow::handleUnclipCB(int state)
{
    //qDebug() << "BuildCellWindow::handleUnclipCB: " << state;
    if (state > 0) cellVtkWindow->setNoBooleanOp(true);
    else           cellVtkWindow->setNoBooleanOp(false);
}
// ------------------------------------------------------------------------------------------------
void BuildCellWindow::closeFileOperations()
{
//    bodyControlsWidget->closeFileOperations();
}
// ------------------------------------------------------------------------------------------------
void BuildCellWindow::makeActions()
{
    openAction = new QAction(tr("&Open File..."), this);
    openAction->setStatusTip(tr("Open an existng file"));

    printAction = new QAction(tr("&Print"), this);
    printAction->setStatusTip(tr("Print"));

    helpAction = new QAction(tr("Help"), this);
    helpAction->setStatusTip(tr("Get help."));

    saveAction = new QAction(tr("Save"), this);
    saveAction->setStatusTip(tr("Save"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(handleSaveNeeded()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setStatusTip(tr("Exit the application"));
    exitAction->setShortcut(tr("Ctrl+X"));

    clipAction = new QAction(tr("Clipping Plane"), this);
    clipAction->setStatusTip(tr("Show / Hide clipping plane."));

    cameraAction = new QAction(tr("Camera Orientation"), this);
    cameraAction->setStatusTip(tr("Show / Hide camera orientation widget."));
    connect(cameraAction, SIGNAL(triggered()), this, SLOT(handleCameraAction()));

    orientationAction = new QAction(tr("Orientation Axes"), this);
    orientationAction->setStatusTip(tr("Show / Hide moveable orientation axes."));
    connect(orientationAction, SIGNAL(triggered()), this, SLOT(handleOrientationAction()));

    lightingAction = new QAction(tr("Color (light) control"), this);
    lightingAction->setStatusTip("Show lighting dialog.");
    connect(lightingAction, SIGNAL(triggered()), this, SLOT(handleLightingDialog()));
}
// ------------------------------------------------------------------------------------------------
void BuildCellWindow::makeToolBar()
{
    // Create the icons.
    this->setIconSize(QSize(22, 22));

    const QIcon icon = QIcon(QString::fromUtf8(":/Icons/help.png"));
    helpAction->setIcon(icon);

    const QIcon icon1 = QIcon(QString::fromUtf8(":/Icons/fileopen.png"));
    openAction->setIcon(icon1);

    const QIcon icon2 = QIcon(QString::fromUtf8(":/Icons/clipping_plane.gif"));
    printAction->setIcon(icon2);

    const QIcon icon3 = QIcon(QString::fromUtf8(":/Icons/filesave.png"));
    saveAction->setIcon(icon3);

    const QIcon icon4 = QIcon(QString::fromUtf8(":/Icons/clipping_plane.gif"));
    clipAction->setIcon(icon4);
    connect(clipAction, SIGNAL(triggered()), this, SLOT(handleClipAction()));


    const QIcon icon5 = QIcon(QString::fromUtf8(":/Icons/camera-100.png"));
    cameraAction->setIcon(icon5);

    const QIcon icon6 = QIcon(QString::fromUtf8(":/Icons/coordinate-system-50.png"));
    orientationAction->setIcon(icon6);

    const QIcon icon7 = QIcon(QString::fromUtf8(":/Icons/flashlight-64.png"));
    lightingAction->setIcon(icon7);


    // Create the toolbar.
    toolBar = new QToolBar(this);
    toolBar->setObjectName(QStringLiteral("toolBar"));
    toolBar->setOrientation(Qt::Horizontal);
    toolBar->setIconSize(QSize(50, 50));
    this->addToolBar(Qt::TopToolBarArea, toolBar);

    // Adding actions to the toolbar adds the icons there.
    toolBar->addAction(saveAction);
    toolBar->addSeparator();
    toolBar->addAction(orientationAction);
    toolBar->addAction(cameraAction);
    toolBar->addAction(lightingAction);

}
// ------------------------------------------------------------------------------------------------
void BuildCellWindow::handleOrientationAction()
{
    cellVtkWindow->toggleOrientationWidget();
}
// ------------------------------------------------------------------------------------------------
void BuildCellWindow::handleCameraAction()
{
    cellVtkWindow->toggleCamOrientationWidget();
}
// ------------------------------------------------------------------------------------------------
void BuildCellWindow::handleClipAction()
{

}
// ------------------------------------------------------------------------------------------------
void BuildCellWindow::handleLightingDialog()
{
    if (lightSource == nullptr)
    {
        QString s = "Cell Builder Window Lighting Control";
        lightSource = new SSSLightSource(s, &lightingControl);

        connect(lightSource, SIGNAL(updateNeededSignal()), this, SLOT(handleLightSource()));

//        buildCellWindow = new BuildCellWindow(&masterData, &license);
//        buildCellWindow->resize(buildCellWindow->sizeHint());

 //       connect(buildCellWindow, SIGNAL(cellListReady(int)), this, SLOT(handleCellListReady(int)));
//        connect(buildCellWindow, SIGNAL(cellChangedSignal(int, int)),
 //               this,            SLOT(handleCellChangedSignal(int, int)));
    }
    lightSource->show();
    lightSource->raise();
    lightSource->activateWindow();
}
// ------------------------------------------------------------------------------------------------
void BuildCellWindow::handleLightSource()
{
//    qDebug() << "Ambient: " << lightingControl.source.ambient.x();
//    qDebug() << "Diffuse: " << lightingControl.source.diffuse.x();
    double values[4];
    values[0] = lightingControl.source.ambient.x();
    values[1] = lightingControl.source.diffuse.x();
    values[2] = lightingControl.material.whiteWash;
    values[3] = lightingControl.material.transparancy;
    cellVtkWindow->updateLight(values, lightingControl.control.renderStyle);
}
// ------------------------------------------------------------------------------------------------
void BuildCellWindow::handleSaveNeeded()
{
    emit saveNeeded();
}
