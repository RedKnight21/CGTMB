#ifndef BUILDBODYCONTROLS_H
#define BUILDBODYCONTROLS_H

#include <QObject>
#include <QWidget>
#include <QButtonGroup>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QMap>

#include "bodybuttoninfo.h"
#include "sss_pushbutton.h"
#include "sss_linedit.h"
#include "cgtmb_constants.h"
#include "cgbodybasics.h"
#include "masterdata.h"
#include "bodydata.h"
#include "celldata.h"
#include "bodytypemap.h"
#include "cgutility.h"
#include "sss_guiutility.h"
#include "license.h"

class BuildBodyControls   : public QWidget
{
    Q_OBJECT

public:
    BuildBodyControls(MasterData *masterDataIn, License *inLic,
                      QStandardItemModel      *bodyModelIn,
                      QVector<CGBodyBasics>   *inCgBodyBasics
                      );

    void handleBodyNumberQueryRequest(QString msg);
    void closeFileOperations();

    QTreeView               *bodyTreeView;
    QMap<int, int>          bodyNumMap;


signals:
    void bodyAddSignal(int, QStandardItem *);
    void bodyChangedSignal();
    void bodyNumberSignal(int, int);
    void bodyControlsMessage(QString);
    void bodyQueryRequest(int);
    void bodyRenumberRequest(int);

private slots:
    void handleBodyButton(int v1, int v2);
    void handleBodyLineEdit(QString s1, int v1);
    void handleClearPB();
    void handleAddPB();
    void handleBodyNameEdit(QString);
    void onCustomContextMenu2(const QPoint &point);
    void bodyModelItemChanged(QStandardItem *bodyModelItem);

private:
    QButtonGroup    *bodyButtonGroup;
    QGridLayout     *buttonGridLayout;
    QGridLayout     *lineEditGridLayout;
    QHBoxLayout     *clearAddLayout;
    QHBoxLayout     *bodyNameLayout;
    QVBoxLayout     *bodyTreeLayout;
    QVBoxLayout     *bodyTreeViewLayout;

    QLineEdit       *nameLineEdit;
    QPushButton     *clearPB;
    QPushButton     *addPB;
    QTreeWidget     *bodyTree;
    QStandardItemModel *bodyModel;

    QPushButton     *andPlusPB;
    QPushButton     *andMinusPB;
    QPushButton     *orPlusPB;
    QPushButton     *orMinusPB;
    QHBoxLayout     *cellBooleanButtonLayout;

    QVector<QStringList> *bodyDataInput;


    QVector<SSS_PushButton *> bodyButtons;
    QVector<SSS_LineEdit *>   bodyLineEdits;
    QVector<QLabel *>         bodyEditLabels;

    BodyTypeMap  typeMap;

    void buildBodyButtonLayout();
    void buildBodyLineEditLayout();
    void buildClearAddButtonLayout();
    void buildBodyNameLayout();
    void buildBodyTreeViewLayout();
    void buildTypeMap();
    void updateBuildBodies(QStandardItem *item);
    void updateBuildBodies(int oldBody, int column, QStandardItem *item);
    void addExistingBodies();
    void addSingleBodyFromList(int b);
    void removeFromBodyDataStringList(QString body);
    void bodyAnalysis(CGBodyBasics &tmpBody);

    QList<QStandardItem *> prepareChildRow( const QString &first,
                                            const QString &second,
                                            const QString &third );

    QList<QStandardItem *> prepareParentRow( const QString &first,
                                             const QString &second,
                                             const QString &third );

    void addBodyTreeChild(QTreeWidgetItem *parent, QString cell, QString tag, QString value);
    void resetBodyTree();
    void renumberBodyDataStringList(int bodyNum);


    BodyData    bodyData;
    MasterData  *md;
    License     *license;

    QVector<CGBodyBasics>   *buildBodies;

    int currentBodyNo;
    int projectType;

};

#endif // BUILDBODYCONTROLS_H
