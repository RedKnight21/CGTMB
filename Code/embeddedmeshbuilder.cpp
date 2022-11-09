#include "embeddedmeshbuilder.h"

EmbeddedMeshBuilder::EmbeddedMeshBuilder(MasterData *inMd,
                                         QVector<ReadFileAbaqus> *abqFiles,
                                         QWidget *parent)
{
    md          = inMd;
    abaqusFiles = abqFiles;
    persistence = 0;

    meshUniverseBuild = new MeshUniverseBuild(md, abqFiles);

    buildLowerPanel();

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(meshUniverseBuild);
    mainLayout->addStretch();
    mainLayout->addLayout(lowerLayout);

    QWidget *mainWidget = new QWidget;

    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);
    setWindowTitle("Embedded Mesh Universe Builder");

    readSettings();

    connect(meshUniverseBuild, SIGNAL(showMeshUniverse(int)),
            this,              SLOT(handleShowMeshUniverse(int)));

    connect(meshUniverseBuild, SIGNAL(updateMeshUniverse()),
            this,              SLOT(handleUpdateMeshUniverse()));

}

// ------------------------------------------------------------------------------------------------
EmbeddedMeshBuilder::~EmbeddedMeshBuilder()
{
    doClose();
}
// ------------------------------------------------------------------------------------------------
void EmbeddedMeshBuilder::resetForProject()
{
    if (md->projectType == MCNP)
    {
        setWindowTitle("Embedded Mesh Universe Builder");
    }
    else
    {
        setWindowTitle("Embedded Mesh Assembly Builder");
    }

    meshUniverseBuild->resetForProject();
}
// ------------------------------------------------------------------------------------------------
void EmbeddedMeshBuilder::doClose()
{
    writeSettings();
    close();
}
// ----------------------------------------------------------------------------

QSize EmbeddedMeshBuilder::sizeHint() const
{
#ifdef USING_WINDOWS
    return QSize(1050, 600);
#else
    return QSize(900, 500);
#endif
}
// ------------------------------------------------------------------------------------------------
void EmbeddedMeshBuilder::writeSettings()
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    settings.setValue("EmbeddedMeshBuilder/size",       this->size());
    settings.setValue("EmbeddedMeshBuilder/Properties", this->saveState());
    settings.setValue("EmbeddedMeshBuilder/Geometry",   this->saveGeometry());
    meshUniverseBuild->writeSettings();
}
// ------------------------------------------------------------------------------------------------
void EmbeddedMeshBuilder::readSettings()
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    this->restoreState(settings.value("EmbeddedMeshBuilder/Properties").toByteArray());
    this->restoreGeometry(settings.value("EmbeddedMeshBuilder/Geometry").toByteArray());
    resize( settings.value("EmbeddedMeshBuilder/size", sizeHint()).toSize() );
}
// ----------------------------------------------------------------------------
void EmbeddedMeshBuilder::buildLowerPanel()
{
    /*
    QLabel *transparancyLabel = new QLabel("Transparency");
    QString s1 = "Transparency for the background and fill cells. ";
    transparancyLabel->setToolTip(s1);
    transparancySlider = new QSlider(Qt::Horizontal);
    transparancySlider->setRange(0,100);

    transparancySlider->setValue(50);
    transparancySlider->setTickPosition(QSlider::TicksBelow);
    transparancySlider->setTickInterval(10);
    transparancySlider->setMinimumWidth(150);
    connect(transparancySlider, SIGNAL(valueChanged(int)), this, SLOT(handleTransparancySlider(int)));

    transparancyDSB = new QDoubleSpinBox;
    transparancyDSB->setRange(0.0, 1.0);
    transparancyDSB->setSingleStep(0.05);
    transparancyDSB->setValue(0.5);
    connect(transparancyDSB, SIGNAL(valueChanged(double)), this, SLOT(handleTransparancyDSB(double)));
    */

    persistentMeshCB = new QCheckBox("Persistent Mesh");
    persistentMeshCB->setToolTip("Always show the unstructured mesh.");
    connect(persistentMeshCB, SIGNAL(stateChanged(int)), this, SLOT(handlePersistence(int)));

    closeButton = new QPushButton("CLOSE");
    closeButton->setToolTip("Close this window.");
    connect(closeButton,   SIGNAL(clicked()),     this, SLOT(doClose()));

//    showButton = new QPushButton("SHOW");
//    showButton->setToolTip("Render on Model View Canvas.");
//    connect(showButton,   SIGNAL(clicked()),     this, SLOT(doClose()));

    lowerLayout = new QGridLayout;
    //lowerLayout->addWidget(transparancyLabel,    0, 0, 1, 1, Qt::AlignHCenter);
    //lowerLayout->addWidget(transparancyDSB,      0, 1, 1, 1, Qt::AlignHCenter);
    //lowerLayout->addWidget(transparancySlider,   1, 0, 1, 2, Qt::AlignHCenter);

//    lowerLayout->addWidget(persistentMeshCB,    0, 0, 1, 1, Qt::AlignHCenter);


//    lowerLayout->addWidget(showButton,           1, 3, 1, 1, Qt::AlignHCenter);
    lowerLayout->addWidget(closeButton,          1, 4, 1, 1, Qt::AlignHCenter);

    lowerLayout->setColumnMinimumWidth(2, 200);
}
// ------------------------------------------------------------------------------------------------
void EmbeddedMeshBuilder::handlePersistence(int val)
{
    persistence = val;
    emit showMeshUniverse(val);
}
// ------------------------------------------------------------------------------------------------
void EmbeddedMeshBuilder::handleTransparancySlider(int value)
{
    double val = ((double) value) / 100.0;
    transparancyDSB->setValue(val);
    emit showMeshUniverse(val);
}
// ------------------------------------------------------------------------------------------------
void EmbeddedMeshBuilder::handleTransparancyDSB(double value)
{
    int val = (int) (value * 100.0);
    transparancySlider->setValue(val);
}
// ------------------------------------------------------------------------------------------------
void EmbeddedMeshBuilder::updateEmbeddedMesh()
{
    meshUniverseBuild->updateAbaqusCombo();
}
// ------------------------------------------------------------------------------------------------
void EmbeddedMeshBuilder::handleShowMeshUniverse(int value)
{
    emit showMeshUniverse(value);
}
// ------------------------------------------------------------------------------------------------
void EmbeddedMeshBuilder::handleUpdateMeshUniverse()
{
    emit updateMeshUniverse();
}
