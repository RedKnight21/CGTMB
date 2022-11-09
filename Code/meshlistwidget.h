#ifndef MESHLISTWIDGET_H
#define MESHLISTWIDGET_H

#include <QObject>
#include <QWidget>

#include "masterdata.h"
#include "sss_checkbox.h"


class MeshListWidget : public QWidget
{
    Q_OBJECT

public:
    MeshListWidget(MasterData *mdIn, QVector<CellData> *mCellList);
    ~MeshListWidget();

    void updateTable();

    void resetForProject();
    void    writeSettings();

private slots:

    void handleSelectAll();
    void handleCellDisplayCheck(int state, int univ, int cell, SSS_CheckBox *check);

private:

    void    addRow(QStringList &sl);
    void    buildCellTable();
    void    buildButtonLayout();
    void    populateCellTable();
    void    clearCellTable();
    void    readSettings();

    QTableWidget        *cellListTreeWidget;
    QPushButton         *selectPB;
    QHBoxLayout         *buttonLayout;
    MasterData          *md;
    QVector<CellData>   *mcl;

    bool    selectState;

    std::vector<SSS_CheckBox *> cbox_display;
    std::vector<QMetaObject::Connection> cboxDisplayConnection;

};

#endif // MESHLISTWIDGET_H
