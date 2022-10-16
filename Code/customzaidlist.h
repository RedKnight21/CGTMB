#ifndef ELEMENTDIALOG_H
#define ELEMENTDIALOG_H

#include <QMainWindow>
#include <QDialog>
#include <QTableWidget>
#include "masterdata.h"
#include <iostream>

class QCheckBox;
class QSlider;
class QLabel;
class QLCDNumber;
class QSpinBox;
class QPushButton;
class QTableWidget;

class CustomZaidList : public QDialog
{
  Q_OBJECT
  
  public:
  
    CustomZaidList(MasterData *mdIn, QWidget *parent = 0);
//    ~CustomZaidList();
    void updateTable();

  private slots:
   
    void clearTable();
    void doClose();
    void applySettings();
    void insertTableRow();
    void somethingChanged(QTableWidgetItem *);
    void handleZaidTypeChanged(int index);

  signals:
    void geometryChanged(int);

  private:

    void setupViews();
    void addRow(int);
    void writeSettings();

    QTableWidget *tableWidget;

    QComboBox *zaidType;

    QSize    sizeHint() const;
     
    QPushButton *addButton;
    QPushButton *applyButton;
    QPushButton *clearButton;
    QPushButton *quitButton;
    QPushButton *resetButton;

    QVector<int> elMap;

    int  totalRows;
    int  whichList;

    MasterData *md;

};

#endif
