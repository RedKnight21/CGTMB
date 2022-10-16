#include "cutawaywidget.h"

CutAwayWidget::CutAwayWidget(Bounds *inBounds)
{
    bounds = inBounds;

    incrementDialog = nullptr;
    isControlKey    = false;

    xAxisCullDirection = 0;
    yAxisCullDirection = 0;
    zAxisCullDirection = 0;

    xAxisCullFactor = 0.0;
    yAxisCullFactor = 0.0;
    zAxisCullFactor = 0.0;

    spinBoxStepSize = 1.0;

    xPlaneLocation = bounds->nodesMinX;
    yPlaneLocation = bounds->nodesMinY;
    zPlaneLocation = bounds->nodesMinZ;

    setupModel();
    setupSlots();
    setFocusPolicy( Qt::StrongFocus );
}
// ------------------------------------------------------------------------------------------------
void CutAwayWidget::setupModel()
{
    // -------------
    // X-Axis setup.
    xAxisSelectionGroupBox    = new QGroupBox("X-Axis Selection");
    adjustQGroupBox(xAxisSelectionGroupBox, 0, true);
    xAxisSelectionButtonGroup = new QButtonGroup;
    negativeX = new QRadioButton("(-)");
    positiveX = new QRadioButton("(+)");
    noX       = new QRadioButton("Off");
    xReverse  = new QCheckBox("Opposite");
    adjustQRadioButton(negativeX, 0, false);
    adjustQRadioButton(positiveX, 0, false);
    adjustQRadioButton(noX,       0, false);
    adjustQCheckBox(xReverse,     0, false);

    xAxisSelectionButtonGroup->addButton(noX);
    xAxisSelectionButtonGroup->addButton(negativeX);
    xAxisSelectionButtonGroup->addButton(positiveX);
    noX->setChecked(true);
    xReverse->setEnabled(false);

    xAxisSelectionGroupBox->setToolTip("Control clippling plane along the x-axis.");
    negativeX->setToolTip("Turn x-axis clipping plane on from the negative x-axis.");
    positiveX->setToolTip("Turn x-axis clipping plane on from the positive x-axis.");
    noX->setToolTip("Turn x-axis clipping plane off.");
    xReverse->setToolTip("Build the model from the opposite direction.");

    xAxisDoubleSpinBox = new QDoubleSpinBox;
    xAxisDoubleSpinBox->setMinimumWidth(100);
    xAxisDoubleSpinBox->setMaximumWidth(200);
    xAxisDoubleSpinBox->setSingleStep(spinBoxStepSize);
    xAxisDoubleSpinBox->setAlignment(Qt::AlignHCenter);
    xAxisDoubleSpinBox->setEnabled(false);
    xAxisDoubleSpinBox->setToolTip("Position along X-axis.");
    xAxisDoubleSpinBox->setValue(0.0);
//    xAxisDoubleSpinBox->setRange(gl3DWidget->nodesMinX, gl3DWidget->nodesMaxX);

    QHBoxLayout *xRBLayout = new QHBoxLayout;
    xRBLayout->addWidget(noX);
    xRBLayout->addWidget(negativeX);
    xRBLayout->addWidget(positiveX);
    xRBLayout->addStretch();
    //xRBLayout->addWidget(xReverse);

    xSlider = createAxisSlider();
    xSlider->setEnabled(false);
    QVBoxLayout *xAxisLayout = new QVBoxLayout;
    xAxisLayout->addLayout(xRBLayout);
    xAxisLayout->addWidget(xSlider);
    xAxisLayout->addWidget(xAxisDoubleSpinBox);
    xAxisSelectionGroupBox->setLayout(xAxisLayout);
    setXAxisSpinBox();

    // -------------
    // Y-Axis setup.
    yAxisSelectionGroupBox    = new QGroupBox("Y-Axis Selection");
    adjustQGroupBox(yAxisSelectionGroupBox, 0, true);
    yAxisSelectionButtonGroup = new QButtonGroup;
    negativeY = new QRadioButton("(-)");
    positiveY = new QRadioButton("(+)");
    noY       = new QRadioButton("Off");
    yReverse  = new QCheckBox("Opposite");
    adjustQRadioButton(negativeY, 0, false);
    adjustQRadioButton(positiveY, 0, false);
    adjustQRadioButton(noY,       0, false);
    adjustQCheckBox(yReverse,     0, false);

    yAxisSelectionButtonGroup->addButton(noY);
    yAxisSelectionButtonGroup->addButton(negativeY);
    yAxisSelectionButtonGroup->addButton(positiveY);
    noY->setChecked(true);
    yReverse->setEnabled(false);

    yAxisSelectionGroupBox->setToolTip("Control clippling plane along the y-axis.");
    negativeY->setToolTip("Turn y-axis clipping plane on from the negative y-axis.");
    positiveY->setToolTip("Turn y-axis clipping plane on from the positive y-axis.");
    noY->setToolTip("Turn y-axis clipping plane off.");
    yReverse->setToolTip("Build the model from the opposite direction.");

    yAxisDoubleSpinBox = new QDoubleSpinBox;
    yAxisDoubleSpinBox->setMinimumWidth(100);
    yAxisDoubleSpinBox->setMaximumWidth(200);
    yAxisDoubleSpinBox->setSingleStep(spinBoxStepSize);
    yAxisDoubleSpinBox->setAlignment(Qt::AlignHCenter);
    yAxisDoubleSpinBox->setEnabled(false);
    yAxisDoubleSpinBox->setToolTip("Position along Y-axis.");
    yAxisDoubleSpinBox->setValue(0.0);
//    yAxisDoubleSpinBox->setRange(gl3DWidget->nodesMinY, gl3DWidget->nodesMaxY);

    QHBoxLayout *yRBLayout = new QHBoxLayout;
    yRBLayout->addWidget(noY);
    yRBLayout->addWidget(negativeY);
    yRBLayout->addWidget(positiveY);
    yRBLayout->addStretch();
//    yRBLayout->addWidget(yReverse);

    ySlider = createAxisSlider();
    ySlider->setEnabled(false);
    QVBoxLayout *yAxisLayout = new QVBoxLayout;
    yAxisLayout->addLayout(yRBLayout);
    yAxisLayout->addWidget(ySlider);
    yAxisLayout->addWidget(yAxisDoubleSpinBox);
    yAxisSelectionGroupBox->setLayout(yAxisLayout);

    // -------------
    // Z-Axis setup.
    zAxisSelectionGroupBox    = new QGroupBox("Z-Axis Selection");
    adjustQGroupBox(zAxisSelectionGroupBox, 0, true);
    zAxisSelectionButtonGroup = new QButtonGroup;
    negativeZ = new QRadioButton("(-)");
    positiveZ = new QRadioButton("(+)");
    noZ       = new QRadioButton("Off");
    zReverse  = new QCheckBox("Opposite");
    adjustQRadioButton(negativeZ, 0, false);
    adjustQRadioButton(positiveZ, 0, false);
    adjustQRadioButton(noZ,       0, false);
    adjustQCheckBox(zReverse,     0, false);

    zAxisSelectionButtonGroup->addButton(noZ);
    zAxisSelectionButtonGroup->addButton(negativeZ);
    zAxisSelectionButtonGroup->addButton(positiveZ);
    noZ->setChecked(true);
    zReverse->setEnabled(false);

    zAxisSelectionGroupBox->setToolTip("Control clippling plane along the z-axis.");
    negativeZ->setToolTip("Turn z-axis clipping plane on from the negative z-axis.");
    positiveZ->setToolTip("Turn z-axis clipping plane on from the positive z-axis.");
    noZ->setToolTip("Turn z-axis clipping plane off.");
    zReverse->setToolTip("Build the model from the opposite direction.");

    zAxisDoubleSpinBox = new QDoubleSpinBox;
    zAxisDoubleSpinBox->setMinimumWidth(100);
    zAxisDoubleSpinBox->setMaximumWidth(200);
    zAxisDoubleSpinBox->setSingleStep(spinBoxStepSize);
    zAxisDoubleSpinBox->setAlignment(Qt::AlignHCenter);
    zAxisDoubleSpinBox->setEnabled(false);
    zAxisDoubleSpinBox->setToolTip("Position along Z-axis.");
    zAxisDoubleSpinBox->setValue(0.0);
//    zAxisDoubleSpinBox->setRange(gl3DWidget->nodesMinZ, gl3DWidget->nodesMaxZ);

    QHBoxLayout *zRBLayout = new QHBoxLayout;
    zRBLayout->addWidget(noZ);
    zRBLayout->addWidget(negativeZ);
    zRBLayout->addWidget(positiveZ);
    zRBLayout->addStretch();
//    zRBLayout->addWidget(zReverse);

    zSlider = createAxisSlider();
    zSlider->setEnabled(false);
    QVBoxLayout *zAxisLayout = new QVBoxLayout;
    zAxisLayout->addLayout(zRBLayout);
    zAxisLayout->addWidget(zSlider);
    zAxisLayout->addWidget(zAxisDoubleSpinBox);
    zAxisSelectionGroupBox->setLayout(zAxisLayout);
    // ------------------------------------------------------------------------
    QLabel *title = new QLabel("Cutaway Controls");
    adjustQLabel(title, 5, true);

    QHBoxLayout *titleLayout = new QHBoxLayout;
    titleLayout->addWidget(title);

    // ------------------------------------------------------------------------
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addSpacing(17);
    mainLayout->addLayout(titleLayout);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(xAxisSelectionGroupBox);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(yAxisSelectionGroupBox);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(zAxisSelectionGroupBox);

    mainLayout->addStretch();
    this->setLayout(mainLayout);
}
// ------------------------------------------------------------------------------------------------
void CutAwayWidget::setupSlots()
{
    connect(xAxisDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(handleXAxisSpinBox(double)));
    connect(yAxisDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(handleYAxisSpinBox(double)));
    connect(zAxisDoubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(handleZAxisSpinBox(double)));

    connect(noX,            SIGNAL(clicked()),          this, SLOT(setNoXRB()));
    connect(negativeX,      SIGNAL(clicked()),          this, SLOT(setNegativeXRB()));
    connect(positiveX,      SIGNAL(clicked()),          this, SLOT(setPositiveXRB()));
    connect(xReverse,       SIGNAL(stateChanged(int)),  this, SLOT(setXReverse(int)));

    connect(noY,            SIGNAL(clicked()),          this, SLOT(setNoYRB()));
    connect(negativeY,      SIGNAL(clicked()),          this, SLOT(setNegativeYRB()));
    connect(positiveY,      SIGNAL(clicked()),          this, SLOT(setPositiveYRB()));
    connect(yReverse,       SIGNAL(stateChanged(int)),  this, SLOT(setYReverse(int)));

    connect(noZ,            SIGNAL(clicked()),          this, SLOT(setNoZRB()));
    connect(negativeZ,      SIGNAL(clicked()),          this, SLOT(setNegativeZRB()));
    connect(positiveZ,      SIGNAL(clicked()),          this, SLOT(setPositiveZRB()));
    connect(zReverse,       SIGNAL(stateChanged(int)),  this, SLOT(setZReverse(int)));

    connect(xSlider,        SIGNAL(valueChanged(int)),  this, SLOT(setXPlaneValues(int)));
    connect(ySlider,        SIGNAL(valueChanged(int)),  this, SLOT(setYPlaneValues(int)));
    connect(zSlider,        SIGNAL(valueChanged(int)),  this, SLOT(setZPlaneValues(int)));
}
// ------------------------------------------------------------------------------------------------
void CutAwayWidget::setNoXRB()
{
    xSlider->setValue(0);
    xSlider->setEnabled(false);
    xReverse->setEnabled(false);
    xReverse->setChecked(false);
    xAxisDoubleSpinBox->setEnabled(false);
    xAxisCullDirection = 0;
    setXAxisSpinBox();
}
// ------------------------------------------------------------------------------------------------
void CutAwayWidget::setNegativeXRB()
{
    xAxisCullDirection = -1;
    xSlider->setEnabled(true);
    xReverse->setEnabled(true);
    xAxisDoubleSpinBox->setEnabled(true);

    double lower, upper;

    if (bounds->nodesMinX < 0.0)
        lower = bounds->nodesMinX * 1.05;
    else
        lower = bounds->nodesMinX * 0.95;

    if (bounds->nodesMaxX < 0.0)
        upper = bounds->nodesMaxX * 0.95;
    else
        upper = bounds->nodesMaxX * 1.05;

    xAxisDoubleSpinBox->setRange(lower, upper);

    xAxisDoubleSpinBox->setValue(lower);

    xSlider->setValue(0);
    setXAxisSpinBox();
}
// ------------------------------------------------------------------------------------------------
void CutAwayWidget::setPositiveXRB()
{
    xAxisCullDirection = 1;
    xSlider->setEnabled(true);
    xReverse->setEnabled(true);
    xAxisDoubleSpinBox->setEnabled(true);

    double lower, upper;

    if (bounds->nodesMinX < 0.0)
        lower = bounds->nodesMinX * 1.05;
    else
        lower = bounds->nodesMinX * 0.95;

    if (bounds->nodesMaxX < 0.0)
        upper = bounds->nodesMaxX * 0.95;
    else
        upper = bounds->nodesMaxX * 1.05;

    xAxisDoubleSpinBox->setRange(lower, upper);

    xAxisDoubleSpinBox->setValue(upper);

    setXAxisSpinBox();
    xSlider->setValue(100);
}
// ------------------------------------------------------------------------------------------------
void CutAwayWidget::setNoYRB()
{
    ySlider->setValue(0);
    ySlider->setEnabled(false);
    yReverse->setEnabled(false);
    yReverse->setChecked(false);
    yAxisDoubleSpinBox->setEnabled(false);
    yAxisCullDirection = 0;
    setYAxisSpinBox();
}
// ------------------------------------------------------------------------------------------------
void CutAwayWidget::setNegativeYRB()
{
    yAxisCullDirection = -1;
    ySlider->setEnabled(true);
    yReverse->setEnabled(true);
    yAxisDoubleSpinBox->setEnabled(true);

    double lower, upper;

    if (bounds->nodesMinY < 0.0)
        lower = bounds->nodesMinY * 1.05;
    else
        lower = bounds->nodesMinY * 0.95;

    if (bounds->nodesMaxY < 0.0)
        upper = bounds->nodesMaxY * 0.95;
    else
        upper = bounds->nodesMaxY * 1.05;

    yAxisDoubleSpinBox->setRange(lower, upper);

    yAxisDoubleSpinBox->setValue(lower);

    ySlider->setValue(0);
    setYAxisSpinBox();
}

// ------------------------------------------------------------------------------------------------
void CutAwayWidget::setPositiveYRB()
{
    yAxisCullDirection = 1;
    ySlider->setEnabled(true);
    yReverse->setEnabled(true);
    yAxisDoubleSpinBox->setEnabled(true);

    double lower, upper;

    if (bounds->nodesMinY < 0.0)
        lower = bounds->nodesMinY * 1.05;
    else
        lower = bounds->nodesMinY * 0.95;

    if (bounds->nodesMaxY < 0.0)
        upper = bounds->nodesMaxY * 0.95;
    else
        upper = bounds->nodesMaxY * 1.05;

    yAxisDoubleSpinBox->setRange(lower, upper);

    yAxisDoubleSpinBox->setValue(upper);

    setYAxisSpinBox();
    ySlider->setValue(100);
}
// ------------------------------------------------------------------------------------------------
void CutAwayWidget::setNoZRB()
{
    zSlider->setValue(0);
    zSlider->setEnabled(false);
    zReverse->setEnabled(false);
    zReverse->setChecked(false);
    zAxisDoubleSpinBox->setEnabled(false);
    zAxisCullDirection = 0;
    setZAxisSpinBox();
}
// ------------------------------------------------------------------------------------------------
void CutAwayWidget::setNegativeZRB()
{
    zAxisCullDirection = -1;
    zReverse->setEnabled(true);
    zSlider->setEnabled(true);
    zAxisDoubleSpinBox->setEnabled(true);

    double lower, upper;

    if (bounds->nodesMinZ < 0.0)
        lower = bounds->nodesMinZ * 1.05;
    else
        lower = bounds->nodesMinZ * 0.95;

    if (bounds->nodesMaxZ < 0.0)
        upper = bounds->nodesMaxZ * 0.95;
    else
        upper = bounds->nodesMaxZ * 1.05;

    zAxisDoubleSpinBox->setRange(lower, upper);

    zAxisDoubleSpinBox->setValue(lower);

    setZAxisSpinBox();
    zSlider->setValue(0);
}
// ------------------------------------------------------------------------------------------------
void CutAwayWidget::setPositiveZRB()
{
    zAxisCullDirection = 1;
    zReverse->setEnabled(true);
    zSlider->setEnabled(true);
    zAxisDoubleSpinBox->setEnabled(true);

    double lower, upper;

    if (bounds->nodesMinZ < 0.0)
        lower = bounds->nodesMinZ * 1.05;
    else
        lower = bounds->nodesMinZ * 0.95;

    if (bounds->nodesMaxZ < 0.0)
        upper = bounds->nodesMaxZ * 0.95;
    else
        upper = bounds->nodesMaxZ * 1.05;

    zAxisDoubleSpinBox->setRange(lower, upper);

    zAxisDoubleSpinBox->setValue(upper);

    setZAxisSpinBox();
    zSlider->setValue(100);
}

// ------------------------------------------------------------------------------------------------
void CutAwayWidget::setXPlaneValues(int value)
{
    xSliderValue = value;
    double x = (double) value;
    xAxisCullFactor = x / 100.0;
    setXAxisSpinBox();
}
// ------------------------------------------------------------------------------------------------
void CutAwayWidget::setYPlaneValues(int value)
{
    ySliderValue = value;
    double x = (double) value;
    yAxisCullFactor = x / 100.0;
    setYAxisSpinBox();
}
// ------------------------------------------------------------------------------------------------
void CutAwayWidget::setZPlaneValues(int value)
{
    zSliderValue = value;
    double x = (double) value;
    zAxisCullFactor = x / 100.0;
    setZAxisSpinBox();
}

// ------------------------------------------------------------------------------------------------
void CutAwayWidget::handleXAxisSpinBox(double planeLoc)
{
    int    plane;
    double dPlane;
    double extentsX = bounds->nodesMaxX - bounds->nodesMinX;

    dPlane = 0.0;
    if (xAxisCullDirection != 0)
    {
        dPlane = (planeLoc - bounds->nodesMinX) / extentsX;
    }

    plane = (int) (dPlane * 100.0);

    xAxisDoubleSpinBox->blockSignals(true);
    xSlider->setValue(plane);
    xAxisDoubleSpinBox->blockSignals(false);
}
// ------------------------------------------------------------------------------------------------
void CutAwayWidget::setXAxisSpinBox()
{
    double plane;
    double lower, upper;

    if (bounds->nodesMinX < 0.0)
        lower = bounds->nodesMinX * 1.05;
    else
        lower = bounds->nodesMinX * 0.95;

    if (bounds->nodesMaxX < 0.0)
        upper = bounds->nodesMaxX * 0.95;
    else
        upper = bounds->nodesMaxX * 1.05;

    xAxisDoubleSpinBox->setRange(lower, upper);

    double extentsX = upper - lower;

    emit xAxisCullDirChange(xAxisCullDirection);

    if (xAxisCullDirection != 0)
    {
        plane = lower + extentsX * xAxisCullFactor;
    }
    else
    {
        // Ensure plane does not cut the geometry in this direction by
        // setting it out of range.
        plane = lower;
    }

    xSlider->blockSignals(true);
    xAxisDoubleSpinBox->setValue(plane);
    xSlider->blockSignals(false);

    emit xCutawayChanged(plane, xAxisCullDirection);
}
// ------------------------------------------------------------------------------------------------
void CutAwayWidget::handleYAxisSpinBox(double planeLoc)
{
    int    plane;
    double dPlane;
    double extentsY = bounds->nodesMaxY - bounds->nodesMinY;

    dPlane = 0.0;
    if (yAxisCullDirection != 0)
    {
        dPlane = (planeLoc - bounds->nodesMinY) / extentsY;
    }

    plane = (int) (dPlane * 100.0);
    yAxisDoubleSpinBox->blockSignals(true);
    ySlider->setValue(plane);
    yAxisDoubleSpinBox->blockSignals(false);
}
// ------------------------------------------------------------------------------------------------
void CutAwayWidget::setYAxisSpinBox()
{
    double plane;    
    double lower, upper;

    if (bounds->nodesMinY < 0.0)
        lower = bounds->nodesMinY * 1.05;
    else
        lower = bounds->nodesMinY * 0.95;

    if (bounds->nodesMaxY < 0.0)
        upper = bounds->nodesMaxY * 0.95;
    else
        upper = bounds->nodesMaxY * 1.05;

    yAxisDoubleSpinBox->setRange(lower, upper);

    double extentsY = upper - lower;

    emit yAxisCullDirChange(yAxisCullDirection);

    if (yAxisCullDirection != 0)
    {
        plane = lower + extentsY * yAxisCullFactor;
    }
    else
    {
        // Ensure plane does not cut the geometry in this direction by
        // setting it out of range.
        plane = lower;
    }

    ySlider->blockSignals(true);
    yAxisDoubleSpinBox->setValue(plane);
    ySlider->blockSignals(false);

    emit yCutawayChanged(plane, yAxisCullDirection);
}
// ------------------------------------------------------------------------------------------------
void CutAwayWidget::handleZAxisSpinBox(double planeLoc)
{
    int    plane;
    double dPlane;
    double extentsZ = bounds->nodesMaxZ - bounds->nodesMinZ;

    dPlane = 0.0;
    if (zAxisCullDirection != 0)
    {      
        dPlane = (planeLoc - bounds->nodesMinZ) / extentsZ;
    }

    plane = (int) (dPlane * 100.0);

    zAxisDoubleSpinBox->blockSignals(true);
    zSlider->setValue(plane);
    zAxisDoubleSpinBox->blockSignals(false);
}
// ------------------------------------------------------------------------------------------------
void CutAwayWidget::setZAxisSpinBox()
{
    double plane;
    double lower, upper;

    if (bounds->nodesMinZ < 0.0)
        lower = bounds->nodesMinZ * 1.05;
    else
        lower = bounds->nodesMinZ * 0.95;

    if (bounds->nodesMaxZ < 0.0)
        upper = bounds->nodesMaxZ * 0.95;
    else
        upper = bounds->nodesMaxZ * 1.05;

    zAxisDoubleSpinBox->setRange(lower, upper);

    double extentsZ = upper - lower;

    emit zAxisCullDirChange(zAxisCullDirection);

    if (zAxisCullDirection != 0)
    {
        plane = lower + extentsZ * zAxisCullFactor;
    }
    else
    {
        // Ensure plane does not cut the geometry in this direction by
        // setting it out of range.
        plane = lower;
    }

    zSlider->blockSignals(true);
    zAxisDoubleSpinBox->setValue(plane);
    zSlider->blockSignals(false);

    emit zCutawayChanged(plane, zAxisCullDirection);
}
// ------------------------------------------------------------------------------------------------
void CutAwayWidget::setZReverse(int value)
{
    if (value == Qt::Checked)
    {
        zAxisCullDirection *= 2;
    }
    else
    {
        zAxisCullDirection /= 2;
    }
    setZAxisSpinBox();
}
// ------------------------------------------------------------------------------------------------
void CutAwayWidget::setYReverse(int value)
{
    if (value == Qt::Checked)
    {
        yAxisCullDirection *= 2;
    }
    else
    {
        yAxisCullDirection /= 2;
    }
    setYAxisSpinBox();
}
// ------------------------------------------------------------------------------------------------
void CutAwayWidget::setXReverse(int value)
{
    if (value == Qt::Checked)
    {
        xAxisCullDirection *= 2;
    }
    else
    {
        xAxisCullDirection /= 2;
    }
    setXAxisSpinBox();
}
// ------------------------------------------------------------------------------------------------
QSlider *CutAwayWidget::createAxisSlider()
{
    QSlider *slider = new QSlider(Qt::Horizontal);
    slider->setRange(0, 100);
    slider->setTickPosition(QSlider::TicksBelow);
    slider->setTickInterval(10);
    slider->setValue(0);
    return slider;
}
// ------------------------------------------------------------------------------------------------
void CutAwayWidget::resetBounds()
{
//    bounds = b;
    xPlaneLocation = bounds->nodesMinX;
    yPlaneLocation = bounds->nodesMinY;
    zPlaneLocation = bounds->nodesMinZ;

 //   setXAxisSpinBox();
 //   setYAxisSpinBox();
 //   setZAxisSpinBox();
}

// ------------------------------------------------------------------------------------------------
void CutAwayWidget::resetForProject()
{
    xAxisCullFactor = 0.0;
    yAxisCullFactor = 0.0;
    zAxisCullFactor = 0.0;

    setNoXRB();
    setNoYRB();
    setNoZRB();
}

// ----------------------------------------------------------------------------
void CutAwayWidget::keyPressEvent(QKeyEvent *event)
{
 //   qDebug() << "keypress from cutaway " << event->key();
//    if (event->key() == Qt::Key_Meta)
//    {
        // Mac OSX: control key on keyboard
//        isControlKey = true;
 //       qDebug() << " Key_Meta / Control keys\n";
//    }

    if ((event->modifiers() & Qt::MetaModifier) && (event->key() == Qt::Key_I))
    {
//        qDebug() << " Key_Meta / Control keys / I Key\n";
        launchIncrementDialog();
    }
}
// ----------------------------------------------------------------------------
void CutAwayWidget::keyReleaseEvent(QKeyEvent *event)
{
    /*
    if (event->key() == Qt::Key_Meta)
    {
        // Mac control key
        isControlKey = false;
    }
    */
}
// ----------------------------------------------------------------------------
void CutAwayWidget::launchIncrementDialog()
{
    if (!incrementDialog)
    {
             incrementDialog = new IncrementChangeDialog();
             connect(incrementDialog, SIGNAL(incrementValue(double)),
                     this,            SLOT(handleNewIncrement(double)));
    }

    incrementDialog->setValue(spinBoxStepSize);
    incrementDialog->show();
    incrementDialog->raise();
    incrementDialog->activateWindow();
}
// ----------------------------------------------------------------------------
void CutAwayWidget::handleNewIncrement(double value)
{
    spinBoxStepSize = value;
    xAxisDoubleSpinBox->setSingleStep(spinBoxStepSize);
    yAxisDoubleSpinBox->setSingleStep(spinBoxStepSize);
    zAxisDoubleSpinBox->setSingleStep(spinBoxStepSize);
}
// ----------------------------------------------------------------------------
IncrementChangeDialog::IncrementChangeDialog()
{
    this->setWindowTitle("Set cutaway spin box increment");

    QString s = "Enter a new value to use when incrementing the spin boxes.";
    incrementLineEdit = new QLineEdit;
    incrementLineEdit->setMinimumWidth(40);
    incrementLineEdit->setMaximumWidth(80);
    incrementLineEdit->setToolTip(s);

    acceptButton = new QPushButton("OK");
    cancelButton = new QPushButton("CANCEL");

    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(acceptButton, SIGNAL(clicked()), this, SLOT(handleAcceptButton()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(incrementLineEdit);
    buttonLayout->addSpacing(20);
    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(acceptButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addSpacing(15);
    mainLayout->addLayout(buttonLayout);

    mainLayout->addStretch();
    setLayout(mainLayout);
    setModal(true);
}
// ----------------------------------------------------------------------------
void IncrementChangeDialog::handleAcceptButton()
{
    QString s1 = incrementLineEdit->text();
    double val = s1.toDouble();
    emit incrementValue(val);
    close();
}
// ----------------------------------------------------------------------------
void IncrementChangeDialog::setValue(double val)
{
    QString s1;
    s1.setNum(val);
    incrementLineEdit->setText(s1);
}
