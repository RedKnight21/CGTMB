#ifndef MESHUNIVERSEBUILD_H
#define MESHUNIVERSEBUILD_H

#include <QObject>
#include <QWidget>
#include <masterdata.h>
#include <abaqusReadFile.h>
#include <sss_pushbutton.h>

class MeshUniverseBuild  : public QSplitter
{
    Q_OBJECT

public:
    MeshUniverseBuild(MasterData *mdIn, QVector<ReadFileAbaqus> *abqFiles);
    ~MeshUniverseBuild();

    void updateAbaqusCombo();
    void updateCellCombos();
    void writeSettings();
    void resetForProject();

private slots:
    void handleFillComboChanged(int);
    void handleBackgroundComboChanged(int);
    void handleAbaqusComboChanged(int);
    void handleColorButton(int row, int col, SSS_PushButton *but);
    void meshInstanceTableChanged(QTableWidgetItem *item);
    void handleAcceptPB();
    void onCustomContextMenu(const QPoint &point);
    void handleCellStartEdit(QString s);

signals:
    void showMeshUniverse(int);
    void updateMeshUniverse();

private:

    void buildTopGridLayout();
    void buildMidGridLayout();
    void displayMessage(QString &msg);
    void buildMeshInstanceTable();
    void clearMeshInstanceTable();
    void addMeshInstanceTableRow(int row, QString name, QString cellNo, QString mtlNo, QString density, QString imp, QColor cellColor);
    void buildRightSideLayout();
    bool addUniverseListRow(QString uNum, QString uuName);
    void saveUniverseData();
    void saveInstanceData();
    void resetWidgets();
    void updateUniverseListTable();
    void populateWidgets(int row);
    void readSettings();

    QWidget *topA;
    QWidget *topB;

    QComboBox *abaqusFileCombo;
    QComboBox *backgroundCombo;
    QComboBox *fillCellCombo;

    QDoubleSpinBox  *azimuthalDSB;
    QDoubleSpinBox  *polarDSB;

    QGridLayout *topGridLayout;
    QGridLayout *midGridLayout;
    QVBoxLayout *rightLayout;

    QLineEdit  *scale;
    QLineEdit  *transx;
    QLineEdit  *transy;
    QLineEdit  *transz;
    QLineEdit  *universeName;
    QLineEdit  *universeNumber;
    QLineEdit  *cellStart;

    QPushButton *acceptPB;

    QTableWidget *meshInstanceTable;
    QTableWidget *universeListTable;

    std::vector<QColor>  instanceColors;
    std::vector<QString> instanceCellNum;
    std::vector<QString> instanceMtlNum;

    MasterData  *md;

    QVector<ReadFileAbaqus> *abaqusFiles;

    std::vector<QMetaObject::Connection> instanceColorConnection;

    int currentAbaqusFileIndex;
    int zoneStart;

    // Widgets used in reset for project type.

    QLabel *uName;
    QLabel *uNum;
    QLabel *cStart;
    QLabel *backgroundLabel;
    QLabel *fillCellLabel;
    QLabel * topLabel;

};

#endif // MESHUNIVERSEBUILD_H
