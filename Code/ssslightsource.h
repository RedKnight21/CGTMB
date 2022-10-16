#ifndef SSSLIGHTSOURCE_H
#define SSSLIGHTSOURCE_H

#include <QtWidgets>
#include <QObject>
#include <QWidget>
#include "opengl_lighting.h"

class SSSLightSource : public QWidget
{
    Q_OBJECT

public:
    SSSLightSource(const QString name, LightingControl *inSource);
    ~SSSLightSource();

    void lightSourceDirection();

private slots:
    void doClose();
    void handleAmbientSlider(int value);
    void handleDiffuseSlider(int value);
    void handlePolarSlider(int value);
    void handleAzimuthalSlider(int value);
    void handleSpecularSlider(int value);
    void handleTransparancySlider(int value);

    void handleAmbientDSB(double value);
    void handleDiffuseDSB(double value);
    void handlePolarDSB(double value);
    void handleAzimuthalDSB(double value);
    void handleSpecularDSB(double value);
    void handleTransparDSB(double value);
    void handleWhiteWashDSB(double value);

    void handleShaderComboChanged(int index);
    void handleViewComboChanged(int index);
    void handleTwoSided(int val);


signals:
    void updateNeededSignal();

private:
    QSize   minimumSizeHint() const;
    void    writeSettings();
    void    readSettings();
    void    createControlLayout();

    LightingControl *lightingControl;

    QPushButton     *closeButton;

    QGridLayout     *controlLayout;
    QGridLayout     *controlLayout1;

    QSlider         *ambientSlider;
    QSlider         *diffuseSlider;
    QSlider         *polarSlider;
    QSlider         *azimuthalSlider;
    QSlider         *specularSlider;
    QSlider         *transparancySlider;

    QComboBox       *shaderCombo;
    QComboBox       *viewCombo;

    QCheckBox       *twoSided;

    QDoubleSpinBox  *ambientDSB;
    QDoubleSpinBox  *diffuseDSB;
    QDoubleSpinBox  *polarDSB;
    QDoubleSpinBox  *azimuthalDSB;
    QDoubleSpinBox  *specularDSB;
    QDoubleSpinBox  *transparancyDSB;
    QDoubleSpinBox  *whiteWashDSB;

    QString         thisDialogName;
    QString         thisShortName;

    double pi;
};

#endif // SSSLIGHTSOURCE_H
