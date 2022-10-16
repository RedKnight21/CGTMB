#include "buildbodycellcontrols.h"

BuildBodyCellControls::BuildBodyCellControls(MasterData   *mdIn,
                                             License      *inLic,
                                             QProgressBar *pBar,
                                             QVector<CGBodyBasics> *inBodyBasics,
                                             QVector<CGZoneOps>    *inZoneOps,
                                             Bounds                *inBounds)
{
    progressBar  = pBar;
    unclipCell   = false;
    md           = mdIn;
    projectType  = md->projectType;
    license      = inLic;
    cgBodyBasics = inBodyBasics;
    cgZoneOps    = inZoneOps;
    bounds       = inBounds;

    buildTab1();
    buildTab2();

    QVector<QString>    tabWidgetQStrings;

    // Define tab titles.
    QString ss1 = QString("Bodies / Cells");
    if (projectType != MCNP) ss1 = "Bodies / Zones";

    tabWidgetQStrings.push_back(ss1);

    ss1 = QString("Cutaways");
    tabWidgetQStrings.push_back(ss1);

    tabWidget       = new QTabWidget;

    tabIndex1 = tabWidget->addTab(tab1Widget,  tabWidgetQStrings[0]);
    tabIndex2 = tabWidget->addTab(tab2Widget,  tabWidgetQStrings[1]);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(tabWidget);
    setLayout(topLayout);

    tabWidget->setTabEnabled(tabIndex1, true);
    tabWidget->setTabEnabled(tabIndex2, false);

    setupSlots();
}
// ----------------------------------------------------------------------------
BuildBodyCellControls::~BuildBodyCellControls()
{

}
// ----------------------------------------------------------------------------
void BuildBodyCellControls::resetForProject()
{
    projectType = md->projectType;
    if (projectType == MCNP)
    {
        tabWidget->setTabText(0, "Bodies / Cells");
    }
    else
    {
        tabWidget->setTabText(0, "Bodies / Zones");
    }
    cellControls->resetForProject();
}
// ----------------------------------------------------------------------------
void BuildBodyCellControls::buildTab1()
{
    tab1Widget = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout;

    bodyControls    = new BuildBodyControls(md, license, bodyModel, cgBodyBasics);
    cellControls    = new BuildCellControls(md);

    layout->addWidget(bodyControls);
    layout->addSpacing(30);
    layout->addWidget(cellControls);
    layout->addStretch();

    tab1Widget->setLayout(layout);

    connect(bodyControls, SIGNAL( bodyAddSignal(int, QStandardItem *) ),
            this,         SLOT(handleBodyAddSignal(int, QStandardItem *) ));

    connect(cellControls, SIGNAL(buildCellShow()), this, SLOT(handleCellShowSignal()));

    connect(bodyControls, SIGNAL(bodyNumberSignal(int, int)),
            this,         SLOT(handleBodyNumberSignal(int, int)));

    connect(bodyControls, SIGNAL( bodyChangedSignal() ),
            this,         SLOT(handleBodyChangedSignal() ));

    connect(bodyControls, SIGNAL( bodyQueryRequest(int) ),
            this,         SLOT(handleBodyQueryRequestSignal(int) ));

    connect(bodyControls, SIGNAL( bodyRenumberRequest(int) ),
            this,         SLOT(handleBodyRenumberRequestSignal(int) ));
}

// ----------------------------------------------------------------------------
void BuildBodyCellControls::buildTab2()
{
    tab2Widget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;

    cutawayWidget = new CutAwayWidget(bounds);

    layout->addWidget(cutawayWidget);
    layout->addStretch();

    tab2Widget->setLayout(layout);
}

// ----------------------------------------------------------------------------
void BuildBodyCellControls::handleBodyQueryRequestSignal(int bodyNo)
{
    QString msg = cellControls->handleBodyNumberQueryRequest(bodyNo);
    bodyControls->handleBodyNumberQueryRequest(msg);
}

// ----------------------------------------------------------------------------
void BuildBodyCellControls::handleBodyRenumberRequestSignal(int bodyNo)
{
    cellControls->handleBodyNumberRenumberRequest(bodyNo);
}

// ----------------------------------------------------------------------------
void BuildBodyCellControls::handleBodyAddSignal(int addType, QStandardItem *item)
{
    //qDebug() << "handleBodyAddSignal:: item rowCount: " << item->rowCount();
    cellControls->passBodyItem(addType, item);
    cellControls->activateShowButton();
}

// ----------------------------------------------------------------------------
void BuildBodyCellControls::handleBodyChangedSignal()
{
    cellControls->activateShowButton();
}

// ----------------------------------------------------------------------------
void BuildBodyCellControls::handleBodyNumberSignal(int oldCell, int newCell)
{
    //qDebug() << "handleBodyNumberSignal:: old: " << oldCell << "\tnew: " << newCell;

    cellControls->updateCellNumbers(oldCell, newCell);
}

// ----------------------------------------------------------------------------
void BuildBodyCellControls::handleCellShowSignal()
{
    /*
    QString s1 = "Building the cell ...";
    QProgressDialog *pd;
    pd = new QProgressDialog(s1, QString(), 0, 1, this);
    pd->setWindowFlag(Qt::Popup);
    pd->show();
    */

    int nZoneOps = (int) cellControls->zoneOps.size();

    progressBar->setVisible(true);
    progressBar->setMinimum(0);
    progressBar->setMaximum(nZoneOps + 1);

    int oldNum, newNum;
//    QVector<CGZoneOps> zoneOps2;
//    zoneOps2.clear();
    CGZoneOps ops;

    cgZoneOps->clear();

    int mult = 1.0;
    if (projectType == ITS) mult = -1.0;

//    qDebug() << "cellControls->zoneOps size: " << cellControls->zoneOps.size();

    for(int i=0; i < nZoneOps; ++i)
    {
        progressBar->setValue(i);
        QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

//        qDebug() << "Body No: " << cellControls->zoneOps[i].bodyNum << "\t sensOp: " <<
//                    cellControls->zoneOps[i].senseOp << "\t boolOp: " << cellControls->zoneOps[i].boolOp << endl;

        // Find the correct entry in the body list.
        oldNum = cellControls->zoneOps[i].bodyNum;
        newNum = bodyControls->bodyNumMap[oldNum];

        // Make the appropriate adjustments.
        ops          = cellControls->zoneOps[i];
        ops.senseOp *= mult;
        ops.meshReduction = cellControls->cellData.meshReduction;
//        ops.bodyNum  = newNum - 1;
        ops.bodyNum  = newNum;
        cgZoneOps->push_back(ops);

        updateZoneBounds(bounds, ops.bodyNum-1);
    }

    progressBar->setValue(nZoneOps);
    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    emit zoneReadyToShow();
    cutawayWidget->resetBounds();
    tabWidget->setTabEnabled(tabIndex2, true);

//    zone.reset();
//    zone.build1Zone(bodyControls->buildBodies, zoneOps2, unclipCell);

//    int nBodies = (int) zoneOps2.size() + 1;
//    emit zoneReadyToShow(&zone, nBodies);
//    tabWidget->setTabEnabled(tabIndex2, true);

    progressBar->setVisible(false);
}

// ----------------------------------------------------------------------------
void BuildBodyCellControls::updateZoneBounds(Bounds *zBounds, int bn)
{
 //   double bounds[6];
 //   double x, y, z, r;
   // int    bt;              // body type

 //   bt = (*cgBodyBasics)[bn].bodyType;
/*
    if (bt == SPH)
    {
        x = (*cgBodyBasics)[bn].parameters[0];
        y = (*cgBodyBasics)[bn].parameters[1];
        z = (*cgBodyBasics)[bn].parameters[2];
        r = (*cgBodyBasics)[bn].parameters[3];

        bounds[0] = x - r;
        bounds[1] = x + r;
        bounds[2] = y - r;
        bounds[3] = y + r;
        bounds[4] = z - r;
        bounds[5] = z + r;
    }
*/
    // Enlarge the bounds so that the clipping surfaces don't cut the zone
    // when they aren't being use.  Can't set the clipping surfaces exactly
    // at the bound.
    if (zBounds->nodesMinX < 0.0) zBounds->nodesMinX *= 1.05;
    else zBounds->nodesMinX *= 0.95;

    if (zBounds->nodesMinY < 0.0) zBounds->nodesMinY *= 1.05;
    else zBounds->nodesMinY *= 0.95;

    if (zBounds->nodesMinZ < 0.0) zBounds->nodesMinZ *= 1.05;
    else zBounds->nodesMinZ *= 0.95;

    if (zBounds->nodesMaxX < 0.0) zBounds->nodesMaxX *= 0.95;
    else zBounds->nodesMaxX *= 1.05;

    if (zBounds->nodesMaxY < 0.0) zBounds->nodesMaxY *= 0.95;
    else zBounds->nodesMaxY *= 1.05;

    if (zBounds->nodesMaxZ < 0.0) zBounds->nodesMaxZ *= 0.95;
    else zBounds->nodesMaxZ *= 1.05;
/*
    zBounds.nodesMinX = (bounds[0] < zBounds.nodesMinX ) ? bounds[0] : zBounds.nodesMinX ;
    zBounds.nodesMinY = (bounds[2] < zBounds.nodesMinY ) ? bounds[2] : zBounds.nodesMinY ;
    zBounds.nodesMinZ = (bounds[4] < zBounds.nodesMinZ ) ? bounds[4] : zBounds.nodesMinZ ;

    zBounds.nodesMaxX = (bounds[1] > zBounds.nodesMaxX ) ? bounds[1] : zBounds.nodesMaxX ;
    zBounds.nodesMaxY = (bounds[3] > zBounds.nodesMaxY ) ? bounds[3] : zBounds.nodesMaxY ;
    zBounds.nodesMaxZ = (bounds[5] > zBounds.nodesMaxZ ) ? bounds[5] : zBounds.nodesMaxZ ;
*/
}


// ----------------------------------------------------------------------------
void BuildBodyCellControls::setupSlots()
{
    connect(cutawayWidget, SIGNAL(xCutawayChanged(double, int)), this, SLOT(handleXCutAwayChanged(double, int)));
    connect(cutawayWidget, SIGNAL(yCutawayChanged(double, int)), this, SLOT(handleYCutAwayChanged(double, int)));
    connect(cutawayWidget, SIGNAL(zCutawayChanged(double, int)), this, SLOT(handleZCutAwayChanged(double, int)));

/*
    connect(spatialControls, SIGNAL(xRotationChanged(int)),     this,    SLOT(handleXRotation(int)));
    connect(spatialControls, SIGNAL(yRotationChanged(int)),     this,    SLOT(handleYRotation(int)));
    connect(spatialControls, SIGNAL(zRotationChanged(int)),     this,    SLOT(handleZRotation(int)));


    connect(cutawayWidget, SIGNAL(xAxisCullDirChange(int)), this, SLOT(handleXAxisCullDirChange(int)));
    connect(cutawayWidget, SIGNAL(yAxisCullDirChange(int)), this, SLOT(handleYAxisCullDirChange(int)));
    connect(cutawayWidget, SIGNAL(zAxisCullDirChange(int)), this, SLOT(handleZAxisCullDirChange(int)));
*/

    connect(bodyControls, SIGNAL(bodyControlsMessage(QString)), this, SLOT(handleMessages(QString)));
    connect(cellControls, SIGNAL(cellControlsMessage(QString)), this, SLOT(handleMessages(QString)));
    connect(cellControls, SIGNAL(cellListReady(int)),           this, SLOT(handleCellListReady(int)));
    connect(cellControls, SIGNAL(cellChangedSignal(int, int)),  this, SLOT(handleCellChangedSignal(int, int)));

//    connect(spatialControls, SIGNAL(centerOnXYZPosition(int, double, double, double)),
//            this,            SLOT(handleCenterOnXYZ(int, double, double, double)));
}
// ------------------------------------------------------------------------------------------------
void BuildBodyCellControls::handleCellChangedSignal(int which, int cell)
{
    emit cellChangedSignal(which, cell);
}
// ------------------------------------------------------------------------------------------------
void BuildBodyCellControls::handleCellListReady(int val)
{
//    qDebug() << "BuildBodyCellControls::handleCellListReady";
    emit cellListReady(val);
    if (val != 1)
    {
        tabWidget->setTabEnabled(tabIndex2, false);
    }
}
// ------------------------------------------------------------------------------------------------
void BuildBodyCellControls::handleMessages(QString msg)
{
    emit bodyCellControlMessage(msg);
}
// ------------------------------------------------------------------------------------------------
void BuildBodyCellControls::handleXRotation(int angle)
{
    emit xRotationChanged(angle);
}
// ------------------------------------------------------------------------------------------------
void BuildBodyCellControls::handleYRotation(int angle)
{
    emit yRotationChanged(angle);
}
// ------------------------------------------------------------------------------------------------
void BuildBodyCellControls::handleZRotation(int angle)
{
    emit zRotationChanged(angle);
}
// ------------------------------------------------------------------------------------------------
void BuildBodyCellControls::handleXCutAwayChanged(double plane, int direction)
{
    emit xCutawayChanged(plane, direction);
}
// ------------------------------------------------------------------------------------------------
void BuildBodyCellControls::handleYCutAwayChanged(double plane, int direction)
{
    emit yCutawayChanged(plane, direction);
}
// ------------------------------------------------------------------------------------------------
void BuildBodyCellControls::handleZCutAwayChanged(double plane, int direction)
{
    emit zCutawayChanged(plane, direction);
}
// ------------------------------------------------------------------------------------------------
void BuildBodyCellControls::handleXAxisCullDirChange(int val)
{
    emit xAxisCullDirChange(val);
}
// ------------------------------------------------------------------------------------------------
void BuildBodyCellControls::handleYAxisCullDirChange(int val)
{
    emit yAxisCullDirChange(val);
}
// ------------------------------------------------------------------------------------------------
void BuildBodyCellControls::handleZAxisCullDirChange(int val)
{
    //qDebug() << "BuildBodyCellControls::handleZAxisCullDirChange: " << val;
    emit zAxisCullDirChange(val);
}
// ------------------------------------------------------------------------------------------------
void BuildBodyCellControls::setXRotation(int angle)
{
    //qDebug() << "BuildBodyCellControls:: X change: " << angle;

//    spatialControls->setXRotation(angle);
}
// ------------------------------------------------------------------------------------------------
void BuildBodyCellControls::setYRotation(int angle)
{
    //qDebug() << "BuildBodyCellControls:: Y change: " << angle;
//    spatialControls->setYRotation(angle);
}
// ------------------------------------------------------------------------------------------------
void BuildBodyCellControls::setZRotation(int angle)
{
    //qDebug() << "BuildBodyCellControls:: Z change: " << angle;
//    spatialControls->setZRotation(angle);
}
// ------------------------------------------------------------------------------------------------
void BuildBodyCellControls::handleCenterOnXYZ(int i, double x, double y, double z)
{
    emit centerOnXYZPosition(i, x, y, z);
}
// ------------------------------------------------------------------------------------------------
void BuildBodyCellControls::setUnclippedFlag(bool value)
{
    unclipCell = value;
}
// ------------------------------------------------------------------------------------------------
void BuildBodyCellControls::closeFileOperations()
{

}
