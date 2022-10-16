#include "mainmodelwindow.h"

MainModelWindow::MainModelWindow(MasterData *mdIn, QVector<CGBodyBasics> *bBasics,
                                 QVector<CellData> *mCellList, QVector<ReadFileAbaqus> *abqFiles)
{
    md = mdIn;
    masterCellList = mCellList;
    bodyBasics     = bBasics;
    model3DWidget  = new Model3DWidget(mdIn, abqFiles, this);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(model3DWidget);

    setLayout(mainLayout);

    setWindowTitle(tr("3-D Model Window"));
    setupSlots();
}
// ------------------------------------------------------------------------------------------------
MainModelWindow::~MainModelWindow()
{
    if (model3DWidget != nullptr) model3DWidget->~Model3DWidget();
}
// ------------------------------------------------------------------------------------------------
void MainModelWindow::updateModel(bool newProjectFlag)
{
//    qDebug() << "- MainModelWindow::updateModel -";
    model3DWidget->makeEntireModel(masterCellList, bodyBasics, newProjectFlag);
    model3DWidget->makeEntireMesh();
    model3DWidget->updateViewParameters();
}

// ------------------------------------------------------------------------------------------------
void MainModelWindow::updateModel(int which, int cell)
{
    if (which == 1)
    {
        model3DWidget->deleteModelZone(cell);
    }
    else if (which == 2)
    {
        model3DWidget->updateModelZones(cell);
    }
}

// ------------------------------------------------------------------------------------------------
void MainModelWindow::buildModelZones()
{
    model3DWidget->buildModelZones();
}

// ------------------------------------------------------------------------------------------------
void MainModelWindow::setXRotation(int angle)
{
    model3DWidget->setXRotation(angle);
}
// ------------------------------------------------------------------------------------------------
void MainModelWindow::setYRotation(int angle)
{
    model3DWidget->setYRotation(angle);
}
// ------------------------------------------------------------------------------------------------
void MainModelWindow::setZRotation(int angle)
{
    model3DWidget->setZRotation(angle);
}
// ------------------------------------------------------------------------------------------------
void MainModelWindow::setupSlots()
{
    connect(model3DWidget, SIGNAL(xRotationChanged(int)),   this, SLOT(handleXRotation(int)));
    connect(model3DWidget, SIGNAL(yRotationChanged(int)),   this, SLOT(handleYRotation(int)));
    connect(model3DWidget, SIGNAL(zRotationChanged(int)),   this, SLOT(handleZRotation(int)));

    connect(model3DWidget, SIGNAL(boundsReady(Bounds)),     this, SLOT(handleBounds(Bounds)));
    connect(model3DWidget, SIGNAL(model3DMessage(QString)), this, SLOT(handleModel3DWidgetMessage(QString)));

    connect(model3DWidget, SIGNAL(windowSizeChanged(const int)),
            this, SLOT(handleSizeChange(const int)));

    connect(model3DWidget, SIGNAL(squareWindowSize(const int)),
            this, SLOT(handleSquareWindow(const int)));

    connect(model3DWidget, SIGNAL(graphicsWidgetMessage(QString)),
            this, SLOT(handleModel3DWidgetMessage(QString)));
}
// ------------------------------------------------------------------------------------------------
void MainModelWindow::handleSquareWindow(const int value)
{
    int width = this->width();
    int height = this->height();

    int newSize;
    newSize = (width < height) ? width : height;

//    qDebug() << "MMW:: newSize: " << newSize << width << height;

    this->resize(newSize, newSize);

    emit squareWindowSize(newSize);
}

// ------------------------------------------------------------------------------------------------
void MainModelWindow::handleSizeChange(const int value)
{
    int width = this->width();
    int height = this->height();

    width  += value;
    height += value;
    this->resize(width, height);

    emit windowSizeChanged(value);
}
// ------------------------------------------------------------------------------------------------
void MainModelWindow::handleXRotation(int angle)
{
    //qDebug() << "MainModelWindow::handelXRotation: " << angle;
    emit xRotationChanged(angle);
}
// ------------------------------------------------------------------------------------------------
void MainModelWindow::handleYRotation(int angle)
{
    emit yRotationChanged(angle);
}
// ------------------------------------------------------------------------------------------------
void MainModelWindow::handleZRotation(int angle)
{
    emit zRotationChanged(angle);
}
// ------------------------------------------------------------------------------------------------
void MainModelWindow::setXCutAwayPlane(double plane)
{
    model3DWidget->setXCutawayPlane(plane);
}
// ------------------------------------------------------------------------------------------------
void MainModelWindow::setYCutAwayPlane(double plane)
{
    model3DWidget->setYCutawayPlane(plane);
}
// ------------------------------------------------------------------------------------------------
void MainModelWindow::setZCutAwayPlane(double plane)
{
    model3DWidget->setZCutawayPlane(plane);
}
// ------------------------------------------------------------------------------------------------
void MainModelWindow::setXAxisCullDirChange(int dir)
{
    model3DWidget->setXAxisCullDir(dir);
}
// ------------------------------------------------------------------------------------------------
void MainModelWindow::setYAxisCullDirChange(int dir)
{
    model3DWidget->setYAxisCullDir(dir);
}
// ------------------------------------------------------------------------------------------------
void MainModelWindow::setZAxisCullDirChange(int dir)
{
    //qDebug() << "MainModelWindow::setZAxisCullDirChange" << dir;
    model3DWidget->setZAxisCullDir(dir);
}
// ------------------------------------------------------------------------------------------------
void MainModelWindow::handleBounds(Bounds b)
{
    emit boundsChanged(b);
//    Bounds bb = b;
//    qDebug() << "MainModelWindow:: handleBounds " << b.nodesMinY << b.nodesMaxY;
}
// ------------------------------------------------------------------------------------------------
void MainModelWindow::setOnXYZPosition(int i, double x, double y, double z)
{
//    qDebug() << "View3DModel::handleCenterOnXY: " << x << "\t" << y << "\t" << z << "\n";

    model3DWidget->setModelCenter(i, x, y, z);
}
// ------------------------------------------------------------------------------------------------
void MainModelWindow::setAutoRotateAngle(int axis, double angle)
{
    model3DWidget->setAutoRotateAngle(axis, angle);
}
// ------------------------------------------------------------------------------------------------
void MainModelWindow::saveRotationAngles()
{
    model3DWidget->saveRotationAngles();
}
// ------------------------------------------------------------------------------------------------
void MainModelWindow::restoreRotationAngles()
{
    model3DWidget->restoreRotationAngles();
}
// ------------------------------------------------------------------------------------------------
void MainModelWindow::handleModel3DWidgetMessage(QString s)
{
    emit mainModelWindowMessage(s);
}
// ------------------------------------------------------------------------------------------------
void MainModelWindow::handleCloseProject()
{
//    qDebug() << "MainModelWindow::handleCloseProject";
    model3DWidget->handleProjectClose();
}
// ------------------------------------------------------------------------------------------------
void MainModelWindow::showMeshUniverse(int universe, int persistence)
{
//    qDebug() << "MainModelWindow:: showMeshUniverse:: universe: " << universe << transparancy;
    model3DWidget->controlMeshUniverseModel(universe, persistence);
}
// ------------------------------------------------------------------------------------------------
void MainModelWindow::showMeshUniverse(int persistence)
{
    model3DWidget->controlMeshUniverseModel(persistence);
}
// ------------------------------------------------------------------------------------------------
void MainModelWindow::showPtracData(int sig, QVector<GLfloat> *data)
{
    model3DWidget->showPtracData(sig, data);
}
// ------------------------------------------------------------------------------------------------
void MainModelWindow::handlePtracPersistence(int sig)
{
    model3DWidget->handlePtracPersistenceSignal(sig);
}
// ------------------------------------------------------------------------------------------------
void MainModelWindow::handleGhostGeometry(int sig)
{
    model3DWidget->handlePtracGhostGeometry(sig);
}
