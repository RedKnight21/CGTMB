#ifndef UTILITIES_H
#define UTILITIES_H

#include <QVector3D>
#include <math.h>

struct LightSource
{
    LightSource()
    {
        ambient.setX(0.1);
        ambient.setY(0.1);
        ambient.setZ(0.1);

        diffuse.setX(0.8);
        diffuse.setY(0.8);
        diffuse.setZ(0.8);

        color.setX(1.0);
        color.setY(1.0);
        color.setZ(1.0);

        position.setX(0.0);
        position.setY(0.0);
        position.setZ(1000.0);

        specular.setX(0.0);
        specular.setY(0.0);
        specular.setZ(0.0);
        phi   =  0.0;
        theta =  0.0;
        shininess = 20.0;
        ambientContribution  = 0.6;
        specularContribution = 0.3;
    }
    QVector3D ambient;
    QVector3D diffuse;
    QVector3D specular;
    QVector3D position;
    QVector3D color;
    float     phi;
    float     theta;
    float     shininess;
    float     specularContribution;
    float     ambientContribution;
};
struct Material
{
    Material()
    {
        transparancy = 1.0;
        whiteWash    = 0.3;
    }
    QVector3D ambient;
    QVector3D diffuse;
    QVector3D emission;
    QVector3D specular;
    float     shininess;
    float     transparancy;
    float     whiteWash;
};
struct Control
{
    Control()
    {
        whichShader = 7;
        renderStyle = 1;
    }
    bool flat;
    bool perVertex;
    bool halfAngle;
    bool colorCorners;
    bool twoSided;
    int  whichShader;
    int  whichGeometry;
    int  renderStyle;
};

struct LightingControl
{
    Control     control;
    Material    material;
    LightSource source;
};


#endif // UTILITIES_H
