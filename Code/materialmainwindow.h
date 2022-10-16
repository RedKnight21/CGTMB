#ifndef MATERIALMAINWINDOW_H
#define MATERIALMAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QWidget>
#include "masterdata.h"
#include "materialmessagewidget.h"
#include "sss_guiutility.h"
#include "sss_string_utils.h"
#include "materialdata.h"
#include "customzaidlist.h"

class MaterialMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MaterialMainWindow(MasterData *mdIn, QWidget *parent = nullptr);
    ~MaterialMainWindow();

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
    void writeSettings();

signals:

public slots:

private slots:
    void handleOpenXsdir();
    void handleZaidTypeChanged(int val);
    void onCustomContextMenuZaid(const QPoint &point);
    void onCustomContextMenuWorking(const QPoint &point);
    void onCustomContextMenuMaterials(const QPoint &point);
    void onCustomContextMenuXgen(const QPoint &point);
    void doClose();
    void handleMtlAdd();
    void handleColorButton();
    void handleNameEdit(QString s);
    void handleDensityEdit(QString s);
    void handleMtlNoEdit(QString s);
    void handleMtlClear();

private:
    void makeControlWidget();
    void createActions();
    void createMenus();
    void createStatusBar();
    void readXsdirFile(QString fileName);
    void readElementData();
    void createDockWidget();
    void buildZaidListWidget();
    void buildXgenListWidget();
    void addToZaidList(QVector<QString> &zList, QVector<int> &zNums);
    void addZaidToWorking(QString zaid);
    void buildWorkingListWidget();
    void addToWorkingList(QString zaid, QString value);
    void buildCloseLayout();
    void buildMaterialsListWidget();
    void setupSlots();
    void addToMaterialsList();
    void addToMtlDataStringList();
    void addExistingMaterials();
    void addSingleMtlFromList(int b);
    void removeFromMtlDataStringList(QString mtl);
    void setEditFields(QString mtl);
    void readSettings();
    void readXgenData();
    void addToXgenList();

    MtlData mtlData;
    QMap<int, int>  mtlNumMap;
    MasterData *md;
    CustomZaidList *customZaidList;

    bool editFlag;
    bool isAltKey;


    QList<QStandardItem *> prepareParentRow(const QString &first,
                                            const QString &second, const bool edit);
    QList<QStandardItem *> prepareParentRow(const QString &first,
                                            const QString &second,
                                            const QString &third);
    QList<QStandardItem *> prepareChildRow(const QString &first,
                                           const QString &second);

    QSplitter               *mainSplitter;
    QSplitter               *centralSplitter;
    MaterialMessageWidget   *messageWidget;
    QDockWidget             *dock1;
    QDockWidget             *dock2;
    QDockWidget             *dockXgen;

    QWidget  *materialControls;
    QWidget  *zaidListWidget;
    QWidget  *xgenListWidget;

    QAction            *zaidAction;
    QStandardItemModel *zaidModel;
    QTreeView          *zaidTreeView;

    QAction            *xgenAction;
    QStandardItemModel *xgenModel;
    QTreeView          *xgenTreeView;

    QWidget            *workingListWidget;
    QStandardItemModel *workingModel;
    QTreeView          *workingTreeView;

    QWidget            *materialsListWidget;
    QStandardItemModel *materialsModel;
    QTreeView          *materialsTreeView;
    QAction            *materialsAction;

    QComboBox *zaidType;
    QLineEdit *materialName;
    QLineEdit *materialNumber;
    QLineEdit *materialDensity;
    QPushButton *materialColor;
    QPushButton *materialAdd;
    QPushButton *materialClear;
    QPushButton *closePB;

    QHBoxLayout     *closeLayout;

    QMenu    *fileMenu;
    QMenuBar *menuBar;
    QAction  *xsdirFile;
    QLabel   *statusFileNameLabel;
    QString  statusFileName;
    QString  xsdirPath;

    QVector<QString> cNeutron;
    QVector<QString> dNeutron;
    QVector<QString> photoAtomic;
    QVector<QString> multiGroup;
    QVector<QString> photoNuclear;
    QVector<QString> dosimetry;
    QVector<QString> electron;
    QVector<QString> proton;
    QVector<QString> elementNames;
    QVector<QString> elementSymbols;
    QVector<int>     elementNums;
    QVector<QString> xgenElNumber;
    QVector<QString> xgenElName;
    QVector<QString> xgenElSymbol;
    QVector<QString> xgenElDensity;


protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

};

#endif // MATERIALMAINWINDOW_H
