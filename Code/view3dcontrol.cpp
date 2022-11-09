#include "view3dcontrol.h"

View3DControl::View3DControl(MasterData *mdIn, QVector<ReadFileAbaqus> *abqFiles,
                             int *m3Dangles)
{
    md = mdIn;
    buildTab2();

    // Define tab titles.
    QVector<QString>    tabWidgetQStrings;
    QString ss1 = QString("Positioning");
    tabWidgetQStrings.push_back(ss1);
    ss1 = QString("Cutaways");
    tabWidgetQStrings.push_back(ss1);
    ss1 = QString("Spatial Effects");
    tabWidgetQStrings.push_back(ss1);

    tabWidget = new QTabWidget;
    tabIndex2 = tabWidget->addTab(tab2Widget, tabWidgetQStrings[1]);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(tabWidget);
    setLayout(topLayout);

    setupSlots();
    this->setMaximumWidth(700);
}
/*
// ----------------------------------------------------------------------------
QSize View3DControl::sizeHint() const
{
    return QSize(600, 700);
}
// ----------------------------------------------------------------------------
QSize View3DControl::minimumSizeHint() const
{
    return QSize(600, 700);
}
*/
// ----------------------------------------------------------------------------
void View3DControl::resetForProject()
{
    cutawayWidget->resetForProject();

    if (md->projectType == MCNP)
    {
//        centerOnWidget->resetForProject("Cell");
    }
    else
    {
//        centerOnWidget->resetForProject("Zone");
    }
}
// ----------------------------------------------------------------------------
void View3DControl::buildTab2()
{
    tab2Widget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout;

    cutawayWidget = new CutAwayWidget(&bounds);

    layout->addWidget(cutawayWidget);
    layout->addStretch();

    tab2Widget->setLayout(layout);
}
// ----------------------------------------------------------------------------
void View3DControl::setupSlots()
{
    connect(cutawayWidget, SIGNAL(xCutawayChanged(double, int)),  this,  SLOT(handleXCutAwayChanged(double, int)));
    connect(cutawayWidget, SIGNAL(yCutawayChanged(double, int)),  this,  SLOT(handleYCutAwayChanged(double, int)));
    connect(cutawayWidget, SIGNAL(zCutawayChanged(double, int)),  this,  SLOT(handleZCutAwayChanged(double, int)));

    connect(cutawayWidget, SIGNAL(xAxisCullDirChange(int)),  this,  SLOT(handleXAxisCullDirChange(int)));
    connect(cutawayWidget, SIGNAL(yAxisCullDirChange(int)),  this,  SLOT(handleYAxisCullDirChange(int)));
    connect(cutawayWidget, SIGNAL(zAxisCullDirChange(int)),  this,  SLOT(handleZAxisCullDirChange(int)));
}

// ------------------------------------------------------------------------------------------------
void View3DControl::handleXCutAwayChanged(double plane, int direction)
{
    emit xCutawayChanged(plane, direction);
}
// ------------------------------------------------------------------------------------------------
void View3DControl::handleYCutAwayChanged(double plane, int direction)
{
    emit yCutawayChanged(plane, direction);
}
// ------------------------------------------------------------------------------------------------
void View3DControl::handleZCutAwayChanged(double plane, int direction)
{
    emit zCutawayChanged(plane, direction);
}
// ------------------------------------------------------------------------------------------------
void View3DControl::handleXAxisCullDirChange(int val)
{
    emit xAxisCullDirChange(val);
}
// ------------------------------------------------------------------------------------------------
void View3DControl::handleYAxisCullDirChange(int val)
{
    emit yAxisCullDirChange(val);
}
// ------------------------------------------------------------------------------------------------
void View3DControl::handleZAxisCullDirChange(int val)
{
    //qDebug() << "BuildBodyCellControls::handleZAxisCullDirChange: " << val;
    emit zAxisCullDirChange(val);
}
// ------------------------------------------------------------------------------------------------
void View3DControl::setBounds(Bounds b)
{
    bounds = b;
    cutawayWidget->resetBounds();
    //qDebug() << "View3DControl setBounds " << bounds.nodesMinY << bounds.nodesMaxY;
}
