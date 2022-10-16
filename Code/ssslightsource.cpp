#include "ssslightsource.h"

// ----------------------------------------------------------------------------
SSSLightSource::SSSLightSource(const QString name, LightingControl *inSource)
{
    pi              = acos(-1.0);
    thisShortName   = name;
    lightingControl = inSource;
    createControlLayout();

    closeButton = new QPushButton("CLOSE");
    closeButton->setToolTip("Close this dialog.");
    connect(closeButton,   SIGNAL(clicked()),     this, SLOT(doClose()));

    QHBoxLayout *closeLayout = new QHBoxLayout;
    closeLayout->addStretch();
    closeLayout->addWidget(closeButton);

    QVBoxLayout *masterLayout = new QVBoxLayout;
 //   masterLayout->addLayout(controlLayout);
    masterLayout->addLayout(controlLayout1);
    masterLayout->addLayout(closeLayout);

    setLayout(masterLayout);
    QString s1 = "Light Source Control for " + name + " Window";
    setWindowTitle(s1);

    thisDialogName = name + "LightSourceDialog";

    readSettings();
}
// ------------------------------------------------------------------------------------------------
SSSLightSource::~SSSLightSource()
{
    doClose();
}

// ------------------------------------------------------------------------------------------------
void SSSLightSource::doClose()
{
    writeSettings();
    close();
}
// ------------------------------------------------------------------------------------------------
QSize SSSLightSource::minimumSizeHint() const
{
    return QSize(300, 200);
}
// ------------------------------------------------------------------------------------------------
void SSSLightSource::writeSettings()
{
    //qDebug() << " slice2Ddialog:: writeSettings" ;
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    settings.setValue(thisDialogName+"/size", this->size());
    settings.setValue(thisDialogName+"/Geometry", this->saveGeometry());
}
// ------------------------------------------------------------------------------------------------
void SSSLightSource::readSettings()
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    resize( settings.value(thisDialogName+"/size", sizeHint()).toSize() );
    this->restoreGeometry(settings.value(thisDialogName+"/Geometry").toByteArray());
}
// ----------------------------------------------------------------------------
void SSSLightSource::createControlLayout()
{
    QLabel *ambientLabel = new QLabel("Ambient");
    ambientLabel->setToolTip("Contribution to the ambient light component.");

    ambientSlider = new QSlider(Qt::Horizontal);
    ambientSlider->setRange(0,100);

    int i = (int) (lightingControl->source.ambient.x() * 100);
    ambientSlider->setValue(i);
    ambientSlider->setTickPosition(QSlider::TicksBelow);
    ambientSlider->setTickInterval(10);
    connect(ambientSlider, SIGNAL(valueChanged(int)), this, SLOT(handleAmbientSlider(int)));
    ambientSlider->setToolTip("Set fraction of light coming from ambient light.");

    ambientDSB = new QDoubleSpinBox;
    ambientDSB->setRange(0.0, 1.0);
    ambientDSB->setSingleStep(0.05);
    ambientDSB->setValue((double) lightingControl->source.ambient.x());
    connect(ambientDSB,   SIGNAL(valueChanged(double)), this, SLOT(handleAmbientDSB(double)));
    ambientDSB->setToolTip("Set fraction of light coming from ambient light.");


    QLabel *diffuseLabel = new QLabel("Diffuse");
    diffuseLabel->setToolTip("Contribution to the diffuse light component.");

    diffuseSlider = new QSlider(Qt::Horizontal);
    diffuseSlider->setRange(0,100);

    i = (int) (lightingControl->source.diffuse.x() * 100);
    diffuseSlider->setValue(i);
    diffuseSlider->setTickPosition(QSlider::TicksBelow);
    diffuseSlider->setTickInterval(10);
    connect(diffuseSlider, SIGNAL(valueChanged(int)), this, SLOT(handleDiffuseSlider(int)));

    diffuseDSB = new QDoubleSpinBox;
    diffuseDSB->setRange(0.0, 1.0);
    diffuseDSB->setSingleStep(0.05);
    diffuseDSB->setValue((double) lightingControl->source.diffuse.x());
    connect(diffuseDSB,   SIGNAL(valueChanged(double)), this, SLOT(handleDiffuseDSB(double)));

    QLabel *specularLabel = new QLabel("Specular");
    specularLabel->setToolTip("Contribution to the specular light component.");

    specularSlider = new QSlider(Qt::Horizontal);
    specularSlider->setRange(0,100);

    i = (int) (lightingControl->source.specular.x() * 100);
    specularSlider->setValue(i);
    specularSlider->setTickPosition(QSlider::TicksBelow);
    specularSlider->setTickInterval(10);
    connect(specularSlider, SIGNAL(valueChanged(int)), this, SLOT(handleSpecularSlider(int)));

    specularDSB = new QDoubleSpinBox;
    specularDSB->setRange(0.0, 1.0);
    specularDSB->setSingleStep(0.05);
    specularDSB->setValue((double) lightingControl->source.specular.x());
    connect(specularDSB,   SIGNAL(valueChanged(double)), this, SLOT(handleSpecularDSB(double)));


    QLabel *transparancyLabel = new QLabel("Transparency");
    QString s1 = "Transparency of the ";
    s1 += thisShortName;
    s1 += " window.";
    transparancyLabel->setToolTip(s1);
    transparancySlider = new QSlider(Qt::Horizontal);
    transparancySlider->setRange(0,100);

    i = (int) (lightingControl->material.transparancy * 100);
    transparancySlider->setValue(i);
    transparancySlider->setTickPosition(QSlider::TicksBelow);
    transparancySlider->setTickInterval(10);
    connect(transparancySlider, SIGNAL(valueChanged(int)), this, SLOT(handleTransparancySlider(int)));

    transparancyDSB = new QDoubleSpinBox;
    transparancyDSB->setRange(0.0, 1.0);
    transparancyDSB->setSingleStep(0.05);
    transparancyDSB->setValue((double) lightingControl->material.transparancy);
    connect(transparancyDSB, SIGNAL(valueChanged(double)), this, SLOT(handleTransparDSB(double)));

    QLabel *polarLabel     = new QLabel("Polar");
    QLabel *azimuthalLabel = new QLabel("Azimuthal");

    polarDSB      = new QDoubleSpinBox();
    azimuthalDSB  = new QDoubleSpinBox();

    polarDSB->setRange(0.0,  180.0);
    azimuthalDSB->setRange(-90.0,  90.0);

    polarDSB->setSingleStep(1.0);
    azimuthalDSB->setSingleStep(1.0);

    polarDSB->setValue((double) lightingControl->source.phi);
    azimuthalDSB->setValue((double) lightingControl->source.theta);
    connect(polarDSB,     SIGNAL(valueChanged(double)), this, SLOT(handlePolarDSB(double)));
    connect(azimuthalDSB, SIGNAL(valueChanged(double)), this, SLOT(handleAzimuthalDSB(double)));


    polarSlider = new QSlider(Qt::Horizontal);
    polarSlider->setRange(0,180);
    i = (int) (lightingControl->source.phi);
    polarSlider->setValue(i);
    polarSlider->setTickPosition(QSlider::TicksBelow);
    polarSlider->setTickInterval(5);
    connect(polarSlider, SIGNAL(valueChanged(int)), this, SLOT(handlePolarSlider(int)));

    azimuthalSlider = new QSlider(Qt::Horizontal);
    azimuthalSlider->setRange(-90,90);
    i = (int) (lightingControl->source.theta);
    azimuthalSlider->setValue(i);
    azimuthalSlider->setTickPosition(QSlider::TicksBelow);
    azimuthalSlider->setTickInterval(5);
    connect(azimuthalSlider, SIGNAL(valueChanged(int)), this, SLOT(handleAzimuthalSlider(int)));


    shaderCombo = new QComboBox();
    shaderCombo->setToolTip("Select shader type.");
    shaderCombo->addItem("3");
    shaderCombo->addItem("4");
    shaderCombo->addItem("5");

    connect(shaderCombo, SIGNAL(currentIndexChanged(int)),
            this,        SLOT(handleShaderComboChanged(int)));

    twoSided = new QCheckBox("Two Sided");
    twoSided->setToolTip("Light both sides of all surfaces.");
    connect(twoSided,  SIGNAL(stateChanged(int)), this, SLOT(handleTwoSided(int)));


    QLabel *viewComboLabel = new QLabel("View Type");

    viewCombo = new QComboBox();
    viewCombo->setToolTip("Select view type.");
//    viewCombo->addItem("One-Sided");
//    viewCombo->addItem("Two-Sided");

    viewCombo->addItem("Normal");
    viewCombo->addItem("White Wash");
    viewCombo->setCurrentIndex(0);

    connect(viewCombo, SIGNAL(currentIndexChanged(int)),
            this,        SLOT(handleViewComboChanged(int)));


    QLabel *whiteWashLabel     = new QLabel("White Wash Factor");

    whiteWashDSB      = new QDoubleSpinBox();
    whiteWashDSB->setRange(0.1, 0.9);
    whiteWashDSB->setSingleStep(0.05);

    whiteWashDSB->setValue((double) lightingControl->material.whiteWash);
    connect(whiteWashDSB, SIGNAL(valueChanged(double)), this, SLOT(handleWhiteWashDSB(double)));
    whiteWashDSB->setToolTip("Select fraction of white to use with the white wash.");





/*
    controlLayout = new QGridLayout;
    controlLayout->addWidget(ambientLabel,      1, 0, 1, 1);
    controlLayout->addWidget(ambientDSB,        1, 1, 1, 1);
    controlLayout->addWidget(ambientSlider,     1, 2, 1, 3);

    controlLayout->addWidget(diffuseLabel,      3, 0, 1, 1);
    controlLayout->addWidget(diffuseDSB,        3, 1, 1, 1);
    controlLayout->addWidget(diffuseSlider,     3, 2, 1, 3);

    controlLayout->addWidget(specularLabel,      4, 0, 1, 1);
    controlLayout->addWidget(specularDSB,        4, 1, 1, 1);
    controlLayout->addWidget(specularSlider,     4, 2, 1, 3);


    controlLayout->addWidget(polarLabel,         5, 0, 1, 1);
    controlLayout->addWidget(polarDSB,           5, 1, 1, 1);
    controlLayout->addWidget(polarSlider,        5, 2, 1, 3);

    controlLayout->addWidget(azimuthalLabel,     7, 0, 1, 1);
    controlLayout->addWidget(azimuthalDSB,       7, 1, 1, 1);
    controlLayout->addWidget(azimuthalSlider,    7, 2, 1, 3);

    controlLayout->addWidget(transparancyLabel,   8, 0, 1, 1);
    controlLayout->addWidget(transparancyDSB,     8, 1, 1, 1);
    controlLayout->addWidget(transparancySlider,  8, 2, 1, 3);

    controlLayout->addWidget(shaderCombo,    9, 1, 1, 1);
    controlLayout->addWidget(twoSided,       9, 3, 1, 1);

    controlLayout->setEnabled(false);
*/
    controlLayout1 = new QGridLayout;
    controlLayout1->addWidget(transparancyLabel,   3, 0, 1, 1);
    controlLayout1->addWidget(transparancyDSB,     3, 1, 1, 1);
    controlLayout1->addWidget(transparancySlider,  3, 2, 1, 3);

    controlLayout1->addWidget(ambientLabel,      1, 0, 1, 1);
    controlLayout1->addWidget(ambientDSB,        1, 1, 1, 1);
    controlLayout1->addWidget(ambientSlider,     1, 2, 1, 3);

    controlLayout1->addWidget(diffuseLabel,       2, 0, 1, 1);
    controlLayout1->addWidget(diffuseDSB,         2, 1, 1, 1);
    controlLayout1->addWidget(diffuseSlider,      2, 2, 1, 3);

    controlLayout1->addWidget(viewComboLabel,    4, 0, 1, 1);
    controlLayout1->addWidget(viewCombo,         4, 1, 1, 1);

    controlLayout1->addWidget(whiteWashLabel,    4, 3, 1, 1);
    controlLayout1->addWidget(whiteWashDSB,      4, 4, 1, 1);

    whiteWashDSB->setEnabled(false);
}
// ------------------------------------------------------------------------------------------------
void SSSLightSource::handleAmbientSlider(int value)
{
    double val = ((double) value) / 100.0;
    ambientDSB->setValue(val);
    lightingControl->source.ambient.setX(val);
    lightingControl->source.ambient.setY(val);
    lightingControl->source.ambient.setZ(val);
    emit updateNeededSignal();

    //qDebug() << "val: " << val;
}
// ------------------------------------------------------------------------------------------------
void SSSLightSource::handleAmbientDSB(double value)
{
    int val = (int) (value * 100.0);
    ambientSlider->setValue(val);
    lightingControl->source.ambient.setX(value);
    lightingControl->source.ambient.setY(value);
    lightingControl->source.ambient.setZ(value);

    //qDebug() << "val: " << val;
}
// ------------------------------------------------------------------------------------------------
void SSSLightSource::handleDiffuseSlider(int value)
{
    double val = ((double) value) / 100.0;
    diffuseDSB->setValue(val);
    lightingControl->source.diffuse.setX(val);
    lightingControl->source.diffuse.setY(val);
    lightingControl->source.diffuse.setZ(val);
    emit updateNeededSignal();

    //qDebug() << "val: " << val;
}
// ------------------------------------------------------------------------------------------------
void SSSLightSource::handleDiffuseDSB(double value)
{
    int val = (int) (value * 100.0);
    diffuseSlider->setValue(val);
    lightingControl->source.diffuse.setX(value);
    lightingControl->source.diffuse.setY(value);
    lightingControl->source.diffuse.setZ(value);

    //qDebug() << "val: " << val;
}

// ------------------------------------------------------------------------------------------------
void SSSLightSource::handleSpecularSlider(int value)
{
    double val = ((double) value) / 100.0;
    specularDSB->setValue(val);
    lightingControl->source.specular.setX(val);
    lightingControl->source.specular.setY(val);
    lightingControl->source.specular.setZ(val);
    emit updateNeededSignal();

    //qDebug() << "val: " << val;
}
// ------------------------------------------------------------------------------------------------
void SSSLightSource::handleSpecularDSB(double value)
{
    int val = (int) (value * 100.0);
    specularSlider->setValue(val);
    lightingControl->source.specular.setX(value);
    lightingControl->source.specular.setY(value);
    lightingControl->source.specular.setZ(value);
    //qDebug() << "val: " << val;
}
// ------------------------------------------------------------------------------------------------
void SSSLightSource::handleTransparancySlider(int value)
{
    double val = ((double) value) / 100.0;
    transparancyDSB->setValue(val);
    lightingControl->material.transparancy = val;
    emit updateNeededSignal();

    //qDebug() << "val: " << val;
}
// ------------------------------------------------------------------------------------------------
void SSSLightSource::handleTransparDSB(double value)
{
    int val = (int) (value * 100.0);
    transparancySlider->setValue(val);
    lightingControl->material.transparancy = value;
}
// ------------------------------------------------------------------------------------------------
void SSSLightSource::handlePolarSlider(int value)
{
    double val = ((double) value);
    polarDSB->setValue(val);
    lightingControl->source.phi = val;
    lightSourceDirection();
    emit updateNeededSignal();

    //qDebug() << "val: " << val;
}
// ------------------------------------------------------------------------------------------------
void SSSLightSource::handlePolarDSB(double value)
{
    int val = (int) (value);
    polarSlider->setValue(val);
    lightingControl->source.phi = value;
    lightSourceDirection();

    //qDebug() << "val: " << val;
}
// ------------------------------------------------------------------------------------------------
void SSSLightSource::handleAzimuthalSlider(int value)
{
    double val = ((double) value);
    azimuthalDSB->setValue(val);
    lightingControl->source.theta = val;
    lightSourceDirection();
    emit updateNeededSignal();

    //qDebug() << "val: " << val;
}
// ------------------------------------------------------------------------------------------------
void SSSLightSource::handleWhiteWashDSB(double value)
{
    lightingControl->material.whiteWash = value;
    emit updateNeededSignal();

    //qDebug() << "val: " << val;
}

// ------------------------------------------------------------------------------------------------
void SSSLightSource::handleAzimuthalDSB(double value)
{
    int val = (int) (value);
    azimuthalSlider->setValue(val);
    lightingControl->source.theta = value;
    lightSourceDirection();

    //qDebug() << "val: " << val;
}
// ------------------------------------------------------------------------------------------------
void SSSLightSource::lightSourceDirection()
{
    /*
    float x = lightSource->position.x();
    float y = lightSource->position.y();
    float z = lightSource->position.z();
    float a = x*x + y*y + z*z;
    a = sqrt(a);

    qDebug() << "light direction: " << (x/a) << "\t" << (y/a) << "\t" << (z/a);
    */
    double phirad   =  (lightingControl->source.phi)  * pi / 180.0;
    double thetarad =  lightingControl->source.theta * pi / 180.0;

    double cosphi = cos(phirad);
    double sinphi = sin(phirad);
    double costheta = cos(thetarad);
    double sintheta = sin(thetarad);

//    qDebug() << "phi: " << lightingControl->source.phi << "\tphirad: " << phirad;
//    qDebug() << "cos phi: " << cosphi;
//    qDebug() << "sin phi: " << sinphi;
//    qDebug() << "theta: " << lightingControl->source.theta << "\tthetarad: " << thetarad;
//    qDebug() << "cos theta: " << costheta;
//    qDebug() << "sin theta: " << sintheta;

    if (abs(cosphi) < 1.e-8) cosphi = 0.0;

    double dist = 1000.;

    lightingControl->source.position.setX( sinphi * costheta * dist );
    lightingControl->source.position.setY( sinphi * sintheta * dist );
    lightingControl->source.position.setZ( cosphi * dist );

//    qDebug() << "lightSourceDirection: " << lightingControl->source.position.x() << "\t"
//             << lightingControl->source.position.y() << "\t" << lightingControl->source.position.z();
}
// ------------------------------------------------------------------------------------------------
void SSSLightSource::handleShaderComboChanged(int index)
{
    if (index == 0)
        lightingControl->control.whichShader = 3;
    else if (index == 1)
        lightingControl->control.whichShader = 4;
    else if (index == 2)
        lightingControl->control.whichShader = 5;
}

// ------------------------------------------------------------------------------------------------
void SSSLightSource::handleViewComboChanged(int index)
{
    lightingControl->control.renderStyle = index;
//    lightingControl->control.whichShader = 7;
    if (index == 1)
        whiteWashDSB->setEnabled(true);
    else
        whiteWashDSB->setEnabled(false);

    emit updateNeededSignal();
}

// ----------------------------------------------------------------------------
void SSSLightSource::handleTwoSided(int val)
{
    lightingControl->control.twoSided = val;
}
