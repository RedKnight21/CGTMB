#ifndef CELLLISTWIDGET_H
#define CELLLISTWIDGET_H

#include <QObject>
#include <QWidget>

#include "masterdata.h"
#include "sss_checkbox.h"


class CellListWidget : public QWidget
{
    Q_OBJECT

public:
    CellListWidget(MasterData *mdIn, QVector<CellData> *mCellList);
    ~CellListWidget();

    void updateTable();

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
    void resetForProject();

private slots:

    void handleSelectAll();
    void handleCellDisplayCheck(int state, int row, SSS_CheckBox *check);

private:

    void    addRow(QStringList &sl);
    void    buildCellTable();
    void    buildButtonLayout();
    void    populateCellTable();
    void    clearCellTable();
    void    readSettings();
    void    writeSettings();

    QTableWidget        *cellListTreeWidget;
    QPushButton         *selectPB;
    QHBoxLayout         *buttonLayout;
    MasterData          *md;
    QVector<CellData>   *mcl;

    bool    selectState;

    std::vector<SSS_CheckBox *> cbox_display;
    std::vector<QMetaObject::Connection> cboxDisplayConnection;


};

#endif // CELLLISTWIDGET_H
