#include <QtGui>
#include <math.h>
#include <QFont>
#include "customzaidlist.h"

CustomZaidList::CustomZaidList(MasterData *mdIn, QWidget *parent)
{
    md = mdIn;
    elMap.clear();
    totalRows = 4;
    whichList = 0;
    setupViews();

    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    resize( settings.value("customZaildListDialog/size", sizeHint()).toSize() );
    this->restoreGeometry(settings.value("customZaildListDialog/Geometry").toByteArray());
}
// ------------------------------------------------------------------------------------------------
//CustomZaidList::~CustomZaidList()
//{
//    writeSettings();
//}
// ------------------------------------------------------------------------------------------------
void CustomZaidList::writeSettings()
{
    //qDebug() << " ElementDialog:: writeSettings" ;
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    settings.setValue("customZaildListDialog/size", this->size());
    settings.setValue("customZaildListDialog/Geometry", this->saveGeometry());
}
// ------------------------------------------------------------------------------------------------
void CustomZaidList::setupViews()
{
    // Table Widget & Upper layout
    tableWidget = new QTableWidget(0, 5);
    tableWidget->setFont(QFont("Courier", 14, 65));
    tableWidget->setToolTip("Enter element Z-numbers.");
    for (int row = 0; row < totalRows; row++)
    {
        tableWidget->insertRow(row);
        addRow(row);
    }

    QHBoxLayout *upperLayout = new QHBoxLayout;
    upperLayout->addWidget(tableWidget);

    //  Bottom control buttons 
    addButton    = new QPushButton("ADD ROW");
    applyButton  = new QPushButton("APPLY");
    quitButton   = new QPushButton("CLOSE");
    clearButton  = new QPushButton("CLEAR LIST");
    addButton->setToolTip("Add a row to the Z-number table.");
    clearButton->setToolTip("Clear the Z-numbers from the table.");
    QString s1 = "Create custom ZAID list by using the requested element / Z-numbers.";
    applyButton->setToolTip(s1);
    quitButton->setToolTip("Close this window.");

    zaidType = new QComboBox();
    zaidType->setToolTip("Select ZAID type.");
    zaidType->addItem("(c) Continuous neutron");
    zaidType->addItem("(d) Discrete neutron");
    zaidType->addItem("(p) Photoatomic");
    zaidType->addItem("(m) Multigroup");
    zaidType->addItem("(u) Photonuclear");
    zaidType->addItem("(y) Dosimetry");
    zaidType->addItem("(e) Electron");
    zaidType->addItem("(h) Proton");

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(zaidType);
    bottomLayout->addWidget(addButton);
    bottomLayout->addWidget(clearButton);
    bottomLayout->addWidget(applyButton);
    bottomLayout->addWidget(quitButton);

    QWidget *bottomControls = new QWidget;
    bottomControls->setLayout(bottomLayout);

    //  Construct final dialog
    QGridLayout *topLayout = new QGridLayout;
    topLayout->addLayout(upperLayout,    0, 0);
    topLayout->addWidget(bottomControls, 1, 0, 1, 1);
    setLayout(topLayout);

    setWindowTitle(tr("Control For Custom ZAID List"));

    //  Connect SLOTS & SIGNALS
    connect(clearButton, SIGNAL(clicked()), this, SLOT(clearTable()));
    connect(applyButton, SIGNAL(clicked()), this, SLOT(applySettings()));
    connect(quitButton,  SIGNAL(clicked()), this, SLOT(doClose()));
    connect(addButton,   SIGNAL(clicked()), this, SLOT(insertTableRow()));
    connect(tableWidget, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(somethingChanged(QTableWidgetItem *)));

    connect(zaidType, SIGNAL(currentIndexChanged(int)),
            this,     SLOT(handleZaidTypeChanged(int)));
}
// ------------------------------------------------------------------------------------------------
void CustomZaidList::doClose()
{
    writeSettings();
    close();
}
// ------------------------------------------------------------------------------------------------
void CustomZaidList::insertTableRow()
{
    totalRows += 1;

    QTableWidgetItem *newItem = tableWidget->item(totalRows-1, 0);
    tableWidget->setRowCount(totalRows);
    addRow(totalRows-1);
}
// ------------------------------------------------------------------------------------------------
void CustomZaidList::addRow(int row)
{
    QColor color = QColor( 255, 0, 0, 255);
    QBrush brush = QBrush (color, Qt::SolidPattern );

    QTableWidgetItem *item0 = new QTableWidgetItem();
    item0->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item0->setForeground(color);
    tableWidget->setItem(row, 0, item0);

    QTableWidgetItem *item1 = new QTableWidgetItem();
    item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item1->setForeground(color);
    tableWidget->setItem(row, 1, item1);

    QTableWidgetItem *item2 = new QTableWidgetItem();
    item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item2->setForeground(color);
    tableWidget->setItem(row, 2, item2);

    QTableWidgetItem *item3 = new QTableWidgetItem();
    item3->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item3->setForeground(color);
    tableWidget->setItem(row, 3, item3);

    QTableWidgetItem *item4 = new QTableWidgetItem();
    item4->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item4->setForeground(color);
    tableWidget->setItem(row, 4, item4);
}
// ------------------------------------------------------------------------------------------------
void CustomZaidList::somethingChanged(QTableWidgetItem *item)
{
    // Block signals so that "somethingChanged" routine is not called.
    tableWidget->blockSignals(true);

    QString s0 = item->data(Qt::EditRole).toString();

    QRegExp re("\\d*");

    //qDebug() << "\n ElementDialog::somethingChanged";
    if (s0 != "")
    {
        if (!re.exactMatch(s0))
        {
            QMessageBox mb;
            QString str = "Ignoring illegal character: " + s0;
            str += "\nIs NOT a number.";
            mb.setIcon(QMessageBox::Critical);
            mb.setInformativeText(str);
            mb.setStandardButtons(QMessageBox::Ok);
            mb.setDefaultButton(QMessageBox::Ok);
            mb.show();
            mb.exec();
            return;
        }
        int elNo = s0.toInt();

        //qDebug() << "elNo : " << elNo << "\t nElement: " << umObject->nElements << endl;

        if ((elNo > 0) && (elNo <= 100))
        {
            int i;
            int aSize = (int) elMap.size();

            for (i=0; i < aSize; ++i)
            {
                if (elMap[i] == elNo) break;
            }
            if (i >= aSize) elMap.push_back(elNo);
        }
        else
        {
            QMessageBox mb;
            QString str = "Illegal element number: " + QString::number(elNo);
            str += "\nOut of range.";
            mb.setIcon(QMessageBox::Critical);
            mb.setInformativeText(str);
            mb.setStandardButtons(QMessageBox::Ok);
            mb.setDefaultButton(QMessageBox::Ok);
            mb.show();
            mb.exec();
        }
    }
    tableWidget->blockSignals(false);
}
// ------------------------------------------------------------------------------------------------
QSize CustomZaidList::sizeHint() const
{
    return QSize(550, 250);
}
// ------------------------------------------------------------------------------------------------
void CustomZaidList::applySettings()
{
    md->customZaidIntList.clear();
    for(int i=0; i<elMap.size(); ++i)
    {
//        qDebug() << "i: " << i << "\t Z: " << elMap[i];
        md->customZaidIntList.push_back(elMap[i]);
    }
    md->customZaidListType = whichList;
}
// ------------------------------------------------------------------------------------------------
void CustomZaidList::updateTable()
{
    int row;
    int col;
    QString s1;
    QTableWidgetItem *item;

    int nRows = (md->customZaidIntList.size() - (totalRows * 5)) / 5 + 1;
    for(int n=0; n<nRows; ++n)
    {
        addRow(totalRows);
        ++totalRows;
    }

    int nItems = 5 * tableWidget->rowCount();

    for(int n=0; n<md->customZaidIntList.size(); ++n)
    {
        if (n < nItems)
        {
            row = n / 5;
            col = n % 5;
            s1.setNum(md->customZaidIntList[n]);
            item = tableWidget->item(row,col);
            item->setText(s1);
        }
    }
}
// ------------------------------------------------------------------------------------------------
void CustomZaidList::clearTable()
{
  int row;
  int col;

  QString s1 = "";
  QTableWidgetItem *item;

  int nRows = tableWidget->rowCount();
  for (row = 0; row < nRows; ++row)
  {
     for (col = 0; col < 5; ++col)
     {
        //cout << " row " << row << " col " << col << endl;
        item = tableWidget->item(row,col);
        item->setText(QString(s1));
     }
  }
  elMap.clear();
}
// ------------------------------------------------------------------------------------------------
void CustomZaidList::handleZaidTypeChanged(int index)
{
    whichList = index;
}
