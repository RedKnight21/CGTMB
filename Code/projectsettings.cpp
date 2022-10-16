#include "projectsettings.h"

ProjectSettings::ProjectSettings(MasterData *md, License *lic)
{
    projectNames.clear();
    // Must be in same order as ProjectType enum's.
    projectNames.push_back("NOPROJECT");
    projectNames.push_back("MCNP");
    projectNames.push_back("ITS");

    masterData = md;
    license    = lic;
    setWindowTitle("Project Settings");
    buildLayout();
    setupSlots();
    setDefaults();
    this->setModal(true);
    projectType->setCurrentIndex(md->projectType);

    handleProjectType(md->projectType);
}
// ------------------------------------------------------------------------------------------------
ProjectSettings::~ProjectSettings()
{
    writeSettings();
}
// ------------------------------------------------------------------------------------------------
void ProjectSettings::doClose()
{
    saveValues();
    writeSettings();
    close();
}

// ------------------------------------------------------------------------------------------------
void ProjectSettings::doCancel()
{
    emit projectSettingsUpdated(0);
    close();
}

// ----------------------------------------------------------------------------
QSize ProjectSettings::sizeHint() const
{
    return QSize(500, 300);
}
// ----------------------------------------------------------------------------
void ProjectSettings::writeSettings()
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    settings.setValue("projectSettings/size",     this->size());
    settings.setValue("projectSettings/Geometry", this->saveGeometry());
}
// ----------------------------------------------------------------------------
void ProjectSettings::readSettings()
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    this->restoreGeometry(settings.value("projectSettings/Geometry").toByteArray());
    resize( settings.value("projectSettings/size", sizeHint()).toSize() );
}
// ----------------------------------------------------------------------------
void ProjectSettings::buildLayout()
{
    QLabel *title  = new QLabel("Title: ");
    projectTitle   = new QLineEdit();
    projectTitle->setToolTip("Input the title for the project.");
    projectTitle->setFixedWidth(400);

    QLabel *dataLine  = new QLabel("Data line: ");
    dataLineLength = new QSpinBox();
    dataLineLength->setToolTip("Set the length of the input line for the card images");
    dataLineLength->setRange(40, 120);
    dataLineLength->setMinimumWidth(70);

    particleLabel  = new QLabel("Particle: ");
    particle = new QComboBox();
    particle->setToolTip("Select the default particle type.");
    particle->addItem("neutrons");
    particle->addItem("photons");
    particle->addItem("electrons");

    QLabel *projLabel = new QLabel("Project Type");
    projectType = new QComboBox();
    projectType->setToolTip("Select a project type.");
    for (int i=0; i < (int) projectNames.size(); ++i)
        projectType->addItem(projectNames[i]);

    cellImportances = new QCheckBox("Cell Importances");
    cellImportances->setToolTip("Add cell importances to cell card lines.");

    lineRuler = new QCheckBox("Column Ruler");
    lineRuler->setToolTip("Add column line ruler to body and cell cards.");

    bodyComment = new QCheckBox("Body Comments");
    bodyComment->setToolTip("Add comments to body lines.");

    cellComment = new QCheckBox("Cell Comments");
    cellComment->setToolTip("Add comments to cell lines.");

    if (license->allowXsdir)
    {
        mtlComment = new QCheckBox("Material Comments");
        mtlComment->setToolTip("Add comments to material lines.");
    }

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(title,           0, 0, 1, 1, Qt::AlignRight);
    gridLayout->addWidget(projectTitle,    0, 1, 1, 3, Qt::AlignLeft);

    gridLayout->addWidget(projLabel,       1, 0, 1, 1, Qt::AlignRight);
    gridLayout->addWidget(projectType,     1, 1, 1, 1, Qt::AlignLeft);

    gridLayout->addWidget(dataLine,        2, 0, 1, 1, Qt::AlignRight);
    gridLayout->addWidget(dataLineLength,  2, 1, 1, 1, Qt::AlignLeft);
    gridLayout->addWidget(particleLabel,   2, 2, 1, 1, Qt::AlignRight);
    gridLayout->addWidget(particle,        2, 3, 1, 1, Qt::AlignLeft);

    gridLayout->addWidget(bodyComment,     3, 0, 1, 1, Qt::AlignLeft);
    gridLayout->addWidget(cellComment,     3, 1, 1, 1, Qt::AlignLeft);
    if (license->allowXsdir)
    {
        gridLayout->addWidget(mtlComment,  3, 2, 1, 1, Qt::AlignLeft);
    }

    gridLayout->addWidget(cellImportances, 4, 0, 1, 1, Qt::AlignLeft);
    gridLayout->addWidget(lineRuler,       4, 1, 1, 1, Qt::AlignLeft);

    gridLayout->setRowMinimumHeight(1,30);
    gridLayout->setRowMinimumHeight(2,30);
    gridLayout->setRowMinimumHeight(3,30);

    // --------------------------------------------------------------
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    cancelPB = new QPushButton("CANCEL");
    connect(cancelPB, SIGNAL(released()),    this, SLOT(doCancel()));

    okPB = new QPushButton("OK");
    connect(okPB, SIGNAL(released()),        this, SLOT(doClose()));
    okPB->setEnabled(false);

    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelPB);
    buttonLayout->addSpacing(20);
    buttonLayout->addWidget(okPB);

    // --------------------------------------------------------------
    QVBoxLayout *finalLayout = new QVBoxLayout();
    finalLayout->addLayout(gridLayout);
    finalLayout->addLayout(buttonLayout);

    this->setLayout(finalLayout);
}
// ----------------------------------------------------------------------------
void ProjectSettings::setupSlots()
{
    connect(projectTitle,   SIGNAL(textChanged(QString)),     this, SLOT(handleProjectTitle(QString)));
    connect(dataLineLength, SIGNAL(valueChanged(int)),        this, SLOT(handleDataLength(int)));
    connect(particle,       SIGNAL(currentIndexChanged(int)), this, SLOT(handleParticle(int)));
    connect(projectType,    SIGNAL(currentIndexChanged(int)), this, SLOT(handleProjectType(int)));

    connect(cellComment,     SIGNAL(stateChanged(int)), this, SLOT(handleCellComment(int)));
    connect(bodyComment,     SIGNAL(stateChanged(int)), this, SLOT(handleBodyComment(int)));
    connect(lineRuler,       SIGNAL(stateChanged(int)), this, SLOT(handleLineRuler(int)));
    connect(cellImportances, SIGNAL(stateChanged(int)), this, SLOT(handleCellImportances(int)));

    if (license->allowXsdir)
    {
        connect(mtlComment,  SIGNAL(stateChanged(int)), this, SLOT(handleMtlComment(int)));
    }
}
// ----------------------------------------------------------------------------
void ProjectSettings::handleProjectTitle(QString s)
{
    tmpProjectTitle = s;
    masterData->projectTitle = projectTitle->text();
}
// ----------------------------------------------------------------------------
void ProjectSettings::handleDataLength(int val)
{
    masterData->inputLineWidth = dataLineLength->value();
}
// ----------------------------------------------------------------------------
void ProjectSettings::handleParticle(int val)
{
    masterData->particleType   = particle->currentIndex();
}
// ----------------------------------------------------------------------------
void ProjectSettings::handleProjectType(int val)
{
    if (val == MCNP)
    {
        cellComment->setText("Cell Comments");
        cellComment->setToolTip("Add comments to cell lines.");
        cellImportances->setText("Cell Importances");
        cellImportances->setToolTip("Add cell importances to cell card lines.");
        cellImportances->setVisible(true);
        particleLabel->setVisible(true);
        particle->setVisible(true);
        cellComment->setVisible(true);
        bodyComment->setVisible(true);
        if (license->allowXsdir)
            mtlComment->setVisible(true);
        lineRuler->setVisible(true);
        okPB->setEnabled(true);
    }
    else if (val == ITS)
    {
        cellComment->setText("Zone Comments");
        cellComment->setToolTip("Add comments to zone lines.");
        cellImportances->setText("Zone Importances");
        cellImportances->setVisible(false);
        particleLabel->setVisible(false);
        particle->setVisible(false);
        cellComment->setVisible(true);
        bodyComment->setVisible(true);
        if (license->allowXsdir)
            mtlComment->setVisible(true);
        lineRuler->setVisible(true);
        okPB->setEnabled(true);
    }
    else
    {
        particleLabel->setVisible(false);
        particle->setVisible(false);
        cellImportances->setVisible(false);
        cellComment->setVisible(false);
        bodyComment->setVisible(false);
        if (license->allowXsdir)
            mtlComment->setVisible(false);
        lineRuler->setVisible(false);
        okPB->setEnabled(false);
    }

    setValues();
}
// ----------------------------------------------------------------------------
void ProjectSettings::handleCellComment(int val)
{
    masterData->displayCellComments = val;
}
// ----------------------------------------------------------------------------
void ProjectSettings::handleBodyComment(int val)
{
    masterData->displayBodyComments = val;
}
// ----------------------------------------------------------------------------
void ProjectSettings::handleMtlComment(int val)
{
    masterData->displayMtlComments = val;
}
// ----------------------------------------------------------------------------
void ProjectSettings::handleLineRuler(int val)
{
    masterData->inputLineWidth = val;
}
// ----------------------------------------------------------------------------
void ProjectSettings::handleCellImportances(int val)
{
    masterData->displayImportances = val;
}
// ----------------------------------------------------------------------------
void ProjectSettings::setDefaults()
{
    projectTitle->setText("");
    dataLineLength->setValue(80);
    particle->setCurrentIndex(0);
    bodyComment->setChecked(true);
    cellComment->setChecked(true);
    lineRuler->setChecked(true);
    cellImportances->setChecked(true);
    if (license->allowXsdir) mtlComment->setChecked(true);
}
// ----------------------------------------------------------------------------
void ProjectSettings::setValues()
{
    projectTitle->setText(masterData->projectTitle);
    particle->setCurrentIndex(masterData->particleType);
    dataLineLength->setValue(masterData->inputLineWidth);

    if (masterData->displayRuler > 0) lineRuler->setChecked(true);
    else lineRuler->setChecked(false);

    if (masterData->displayImportances > 0) cellImportances->setChecked(true);
    else cellImportances->setChecked(false);

    if (masterData->displayBodyComments > 0) bodyComment->setChecked(true);
    else bodyComment->setChecked(false);

    if (masterData->displayCellComments > 0) cellComment->setChecked(true);
    else cellComment->setChecked(false);

    if (license->allowXsdir)
    {
        if (masterData->displayMtlComments > 0) mtlComment->setChecked(true);
        else mtlComment->setChecked(false);
    }
}
// ----------------------------------------------------------------------------
void ProjectSettings::saveValues()
{
    masterData->projectType    = projectType->currentIndex();
    masterData->projectTitle   = projectTitle->text();
    masterData->inputLineWidth = dataLineLength->value();
    masterData->particleType   = particle->currentIndex();

    if (bodyComment->isChecked()) masterData->displayBodyComments = 2;
    else masterData->displayBodyComments = 0;

    if (cellComment->isChecked()) masterData->displayCellComments = 2;
    else masterData->displayCellComments = 0;

    if (lineRuler->isChecked()) masterData->displayRuler = 2;
    else masterData->displayRuler = 0;

    if (cellImportances->isChecked()) masterData->displayImportances = 2;
    else masterData->displayImportances = 0;

    if (license->allowXsdir)
    {
        if (mtlComment->isChecked()) masterData->displayMtlComments = 2;
        else masterData->displayMtlComments = 0;
    }
    emit projectSettingsUpdated(1);
}



