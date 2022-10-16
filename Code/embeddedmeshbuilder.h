#ifndef EMBEDDEDMESHBUILDER_H
#define EMBEDDEDMESHBUILDER_H

#include <QMainWindow>
#include <QObject>
#include <QtWidgets>
#include <meshuniversebuild.h>

class MasterData;


class EmbeddedMeshBuilder : public QMainWindow
{
    Q_OBJECT

public:
    EmbeddedMeshBuilder(MasterData *inMd, QVector<ReadFileAbaqus> *abqFiles, QWidget *parent = nullptr);
    ~EmbeddedMeshBuilder();

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    void updateEmbeddedMesh();
    void resetForProject();

signals:

    void showMeshUniverse(int, int);
    void showMeshUniverse(double);
    void showMeshUniverse(int);
    void updateMeshUniverse();
 //   void updateMeshUniverseTransparancySignal(double);

private slots:

    void doClose();
    void handleTransparancyDSB(double value);
    void handleTransparancySlider(int value);
    void handleShowMeshUniverse(int value);
    void handlePersistence(int val);
    void handleUpdateMeshUniverse();

private:

    void buildLowerPanel();
    void writeSettings();
    void readSettings();

    MasterData        *md;
    MeshUniverseBuild *meshUniverseBuild;

    QVector<ReadFileAbaqus> *abaqusFiles;

    QCheckBox       *persistentMeshCB;

    QSlider         *transparancySlider;
    QDoubleSpinBox  *transparancyDSB;

    QPushButton     *closeButton;
    QPushButton     *showButton;

    QGridLayout     *lowerLayout;
    QVBoxLayout     *mainLayout;

    int persistence;

};

#endif // EMBEDDEDMESHBUILDER_H
