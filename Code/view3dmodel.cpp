#include "view3dmodel.h"

View3DModel::View3DModel(MasterData *mdIn, QVector<CGBodyBasics> *bBasics,
                         QVector<CellData> *mCellList,
                         QVector<ReadFileAbaqus> *abqFiles,
                         int *m3Dangles,
                         QWidget *parent) : QMainWindow(parent)
{
    md = mdIn;
    masterCellList = mCellList;
    bodyBasics     = bBasics;

    md->makeCellList(masterCellList);
    md->makeBodyBasics(bodyBasics);

    QWidget *central = new QWidget;
    view3DControl    = new View3DControl(mdIn, abqFiles, m3Dangles);
    cellListWidget   = new CellListWidget(md, masterCellList);
    meshListWidget   = new MeshListWidget(md, masterCellList);

    createDockWidget();
    buildCloseButton();
    buildMessageWindow();

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addWidget(messageWindow);
    centralLayout->addWidget(view3DControl);
    centralLayout->addLayout(closeLayout);

    central->setLayout(centralLayout);
    setCentralWidget(central);

    setWindowTitle(tr("Model View Window Controls"));
    readSettings();
    setupSlots();
    if (md->abaqusFileStringList.size() == 0)
        dock2->hide();
}
// ----------------------------------------------------------------------------
View3DModel::~View3DModel()
{
    writeSettings();
}
// ----------------------------------------------------------------------------
void View3DModel::resetForProject()
{
    view3DControl->resetForProject();
    cellListWidget->resetForProject();
    meshListWidget->resetForProject();
}
// ----------------------------------------------------------------------------
void View3DModel::createDockWidget()
{
//    QDockWidget *dock2 = new QDockWidget(tr("3-D Message Window"), this);
//    dock2->setAllowedAreas(Qt::TopDockWidgetArea | Qt::BottomDockWidgetArea);
//    dock2->setWidget(messageWindow);
//    addDockWidget(Qt::TopDockWidgetArea, dock2);
//    messageAction = dock2->toggleViewAction();

    QDockWidget *dock;
    QString s1;
    if (md->projectType == MCNP) s1 = "Cell List";
    else                         s1 = "Zone List";

    dock = new QDockWidget((s1), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setWidget(cellListWidget);
    dock->setObjectName((s1));

    addDockWidget(Qt::LeftDockWidgetArea, dock);
    cellZoneAction = dock->toggleViewAction();

    if (md->projectType == MCNP) s1 = "Mesh Universe List";
    else                         s1 = "Mesh Assembly List";

    dock2 = new QDockWidget((s1), this);
    dock2->setAllowedAreas(Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea);
    dock2->setWidget(meshListWidget);

    dock2->setObjectName((s1));
    meshUniverseAction = dock2->toggleViewAction();
    if (md->abaqusFileStringList.size()> 0)
    {
        addDockWidget(Qt::RightDockWidgetArea, dock2);
    }
}
// ----------------------------------------------------------------------------
void View3DModel::displayMeshDock()
{
    addDockWidget(Qt::RightDockWidgetArea, dock2);
}
// ----------------------------------------------------------------------------
void View3DModel::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(cellZoneAction);
    menu.addAction(meshUniverseAction);
    menu.exec(event->globalPos());
}
// ----------------------------------------------------------------------------
QSize View3DModel::sizeHint() const
{
#ifdef USING_WINDOWS
    return QSize(1500, 1000);
#else
    return QSize(1200, 700);
#endif
}

// ------------------------------------------------------------------------------------------------
void View3DModel::writeSettings()
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    settings.setValue("view3DModelWindow/size",       this->size());
    settings.setValue("view3DModelWindow/Properties", this->saveState());
    settings.setValue("view3DModelWindow/Geometry",   this->saveGeometry());

    meshListWidget->writeSettings();
    cellListWidget->writeSettings();
}
// ------------------------------------------------------------------------------------------------
void View3DModel::readSettings()
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    this->restoreState(settings.value("view3DModelWindow/Properties").toByteArray());
    this->restoreGeometry(settings.value("view3DModelWindow/Geometry").toByteArray());
//    resize( settings.value("view3DModelWindow/size", sizeHint()).toSize() );
}
// ------------------------------------------------------------------------------------------------
void View3DModel::buildCloseButton()
{
    closePB = new QPushButton("CLOSE");
    closePB->setToolTip("Click to close this window.");
    connect(closePB, SIGNAL(released()), this, SLOT(doClose()));

    QString s1;
    if (md->projectType == MCNP) s1 = "Click to update the cell list.";
    else                         s1 = "Click to update the zone list.";

    updatePB = new QPushButton("UPDATE");
    updatePB->setToolTip(s1);
    connect(updatePB, SIGNAL(released()), this, SLOT(handleUpdate()));

    showPB = new QPushButton("SHOW");
    showPB->setToolTip("Click to re-draw the model in the 3-D graphics window.");
    connect(showPB, SIGNAL(released()), this, SLOT(handleShow()));

    closeLayout = new QHBoxLayout;
//    closeLayout->addWidget(updatePB);
    closeLayout->addStretch();
    closeLayout->addWidget(showPB);
    closeLayout->addStretch();
    closeLayout->addWidget(closePB);
}
// ------------------------------------------------------------------------------------------------
void View3DModel::doClose()
{
    writeSettings();
    close();
}
// ------------------------------------------------------------------------------------------------
void View3DModel::buildMessageWindow()
{
    messageTextWindow = new QTextEdit;
    messageTextWindow->setReadOnly(false);
    messageTextWindow->setMinimumSize(400, 100);

#ifdef USING_WINDOWS
    int fontSize       = 11;
#else
    int fontSize       = 14;
#endif

    QFont courierFont("Courier", fontSize);
    messageTextWindow->setCurrentFont(courierFont);

    clearPB = new QPushButton("CLEAR");
    clearPB->setToolTip("Click to clear the text pane window.");
    connect(clearPB, SIGNAL(pressed()), this, SLOT(handleClearTextEdit()) );

    QHBoxLayout *clearLayout = new QHBoxLayout;
    clearLayout->addStretch();
    clearLayout->addWidget(clearPB);

    messageLayout = new QVBoxLayout;
    messageLayout->addWidget(messageTextWindow);
    messageLayout->addLayout(clearLayout);

    messageWindow = new QWidget;
    messageWindow->setLayout(messageLayout);
}
// ------------------------------------------------------------------------------------------------
void View3DModel::handleClearTextEdit()
{
    messageTextWindow->clear();
    messageTextWindow->setText("3-D Model display messages\n\n");
}
// ------------------------------------------------------------------------------------------------
void View3DModel::handleUpdate()
{
    md->makeCellList(masterCellList);
    md->makeBodyBasics(bodyBasics);

    cellListWidget->updateTable();
    meshListWidget->updateTable();
}
// ------------------------------------------------------------------------------------------------
void View3DModel::handleShow()
{
    emit modelUpdate();
}
// ------------------------------------------------------------------------------------------------
void View3DModel::setupSlots()
{
    connect(view3DControl, SIGNAL(xCutawayChanged(double, int)), this, SLOT(handleXCutAwayChanged(double, int)));
    connect(view3DControl, SIGNAL(yCutawayChanged(double, int)), this, SLOT(handleYCutAwayChanged(double, int)));
    connect(view3DControl, SIGNAL(zCutawayChanged(double, int)), this, SLOT(handleZCutAwayChanged(double, int)));

    connect(view3DControl, SIGNAL(xAxisCullDirChange(int)), this, SLOT(handleXAxisCullDirChange(int)));
    connect(view3DControl, SIGNAL(yAxisCullDirChange(int)), this, SLOT(handleYAxisCullDirChange(int)));
    connect(view3DControl, SIGNAL(zAxisCullDirChange(int)), this, SLOT(handleZAxisCullDirChange(int)));
}

// ------------------------------------------------------------------------------------------------
void View3DModel::handleXCutAwayChanged(double plane, int direction)
{
    emit xCutawayChanged(plane, direction);
}
// ------------------------------------------------------------------------------------------------
void View3DModel::handleYCutAwayChanged(double plane, int direction)
{
    emit yCutawayChanged(plane, direction);
}
// ------------------------------------------------------------------------------------------------
void View3DModel::handleZCutAwayChanged(double plane, int direction)
{
    emit zCutawayChanged(plane, direction);
}
// ------------------------------------------------------------------------------------------------
void View3DModel::handleXAxisCullDirChange(int val)
{
    xAxisCullDirChange(val);
}
// ------------------------------------------------------------------------------------------------
void View3DModel::handleYAxisCullDirChange(int val)
{
    yAxisCullDirChange(val);
}
// ------------------------------------------------------------------------------------------------
void View3DModel::handleZAxisCullDirChange(int val)
{
    zAxisCullDirChange(val);
}
// ------------------------------------------------------------------------------------------------
void View3DModel::setBounds(Bounds b)
{
    //Bounds bb = b;
    //qDebug() << "View3DModel setBounds";
    //qDebug() << "View3DModel:: setBounds " << b.nodesMinY << b.nodesMaxY;
    view3DControl->setBounds(b);
}
// ------------------------------------------------------------------------------------------------
void View3DModel::setMessage(QString s)
{
    messageTextWindow->insertPlainText(s);
}
// ----------------------------------------------------------------------------
void View3DModel::keyPressEvent(QKeyEvent *event)
{
    if ((event->modifiers() & Qt::AltModifier) && (event->key() == Qt::Key_U))
    {
        handleUpdate();
    }
}
// ----------------------------------------------------------------------------
void View3DModel::updateModelBounds(Bounds *inBounds)
{
    Bounds b = *inBounds;
    view3DControl->setBounds(b);
}












