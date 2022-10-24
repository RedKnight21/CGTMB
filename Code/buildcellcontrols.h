#ifndef BUILDCELLCONTROLS_H
#define BUILDCELLCONTROLS_H

#include <QObject>
#include <QWidget>
#include <QTreeView>
#include <QStandardItem>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <vector>

#include "cgtmb_constants.h"
#include "sss_combobox.h"
#include "sss_checkbox.h"
// #include "cgzone.h"
#include "masterdata.h"
#include "bodydata.h"
#include "celldata.h"
#include "sss_guiutility.h"

class BuildCellControls   : public QSplitter
{
    Q_OBJECT

public:
    BuildCellControls(MasterData *masterDataIn);

    void passBodyItem(int changeType, QStandardItem *item);
    void updateCellNumbers(int oldCell, int newCell);
    void activateShowButton();
    void closeFileOperations();
    QString handleBodyNumberQueryRequest(int bodyNo);
    void    handleBodyNumberRenumberRequest(int bodyNo);
    void    resetForProject();

    QTableWidget            *booleanTableWidget;

    QVector<BodiesForCell>  bodiesForCell;
    QVector<CGZoneOps>      zoneOps;

    QTreeView               *cellTreeView;

    QVector<CellData>       buildCells;
    CellData                cellData;


signals:
    void buildCellShow();
    void cellControlsMessage(QString);
    void cellChangedSignal(int, int);
    void cellListReady(int);


private slots:
    void handleBooleanTableChange(QTableWidgetItem *item);
    void onCustomContextMenu(const QPoint &point);
    void onCustomContextMenu2(const QPoint &point);
    void handleClearBooleanTable();
    void handleCellShow();
    void handleCellSignChanged(int value, int row, int val2);
    void handleCellOpChanged(int value, int row, int val2);
    void handleCellDisplayCheck(int state, int row, SSS_CheckBox *check);

    void handleNameEdit(QString s);
    void handleMaterialEdit(QString s);
    void handleCellNoEdit(QString s);
    void handleDensityEdit(QString s);
    void handleImportanceEdit(QString s);
    void handleColorButton();
    void handleCellAdd();
    void handleCellNoFinish();
    void cellModelItemChanged(QStandardItem *cellModelItem);

    void xResChange(int val);
    void yResChange(int val);
    void zResChange(int val);
    void meshReductionChange(int val);

private:

    void buildCellContructionLayout();
    void buildCellDescriptionLayout();
    void buildBooleanTable();
    void addRow(int changeType, QStandardItem *item);
    void addRow(CGZoneOps &zoneOps);
    void delRow(QStandardItem *item);
    void buildBooleanButtonLayout();
    void buildCellTreeLayout();
    void setCellDescription();
    void setEditFields(QStandardItem *itemClicked, QModelIndex index);

    void updateBuildCellsChild(int cell, int row, QStandardItem *item);
    void updateBuildCells(int oldBody, int column, QStandardItem *item);
    void addToCellDataStringList();
    void removeFromCellDataStringList(QString cell);
    void addExistingCells();
    void addSingleCellFromList(int b);
    void enableCellWidgets();
    void disableCellWidgets();

    QList<QStandardItem *> prepareParentRow(const QString &first, const QString &second, const QString &third);
    QList<QStandardItem *> prepareChildRow(const QString &first,  const QString &second, const QString &third);

    QVector<QStringList> *bodyDataInput;

    QStandardItemModel *cellModel;

    MasterData          *md;


    QStandardItemModel  *booleanModel;

    QHeaderView         *vHeader;

    QTreeWidget     *cellTree;
    QVBoxLayout     *cellTreeLayout;

    QVBoxLayout *cellConstructionLayout;
    QHBoxLayout *booleanButtonLayout;

    QPushButton *booleanClearPB;
    QPushButton *booleanShowPB;
    QPushButton *booleanAddPB;


    QGridLayout *bottomLayout;
    QWidget     *cellDescriptionWidget;

    QLineEdit   *nameLE;
    QLineEdit   *cellNoLE;
    QLineEdit   *materialNoLE;
    QLineEdit   *densityLE;
    QLineEdit   *importanceLE;
    QPushButton *colorPB;
    QSpinBox    *xResolution;
    QSpinBox    *yResolution;
    QSpinBox    *zResolution;
    QSpinBox    *meshReduction;

    int         currentBooleanTableRow;
    int         currentCellNumber;
    int         projectType;
    int         editCellNo;
    bool        editFlag;

    QMap<int, int>  cellNumMap;


    std::vector<SSS_CheckBox *> cbox_display;
    std::vector<SSS_ComboBox *> cbox_sense;
    std::vector<SSS_ComboBox *> cbox_bool;

    std::vector<QMetaObject::Connection> cboxDisplayConnection;
    std::vector<QMetaObject::Connection> cboxSenseConnection;
    std::vector<QMetaObject::Connection> cboxBoolConnection;

    QLabel *topLabel;
    QLabel *layoutTitle;
    QLabel *cellNumberLabel;
    QLabel *modelListLabel;
    QStringList hHeaderList;

};

#endif // BUILDCELLCONTROLS_H
