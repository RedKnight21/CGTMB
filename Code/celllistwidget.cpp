#include "celllistwidget.h"

CellListWidget::CellListWidget(MasterData *mdIn, QVector<CellData> *mCellList)
{
    md  = mdIn;
    mcl = mCellList;
    selectState    = true;    // default all instances are selected.

    buildCellTable();
    buildButtonLayout();

    QVBoxLayout *topLayout = new QVBoxLayout;
    topLayout->addWidget(cellListTreeWidget);
    topLayout->addLayout(buttonLayout);

    setLayout(topLayout);

    populateCellTable();
    readSettings();
}
// ----------------------------------------------------------------------------
CellListWidget::~CellListWidget()
{
   writeSettings();
}
// ------------------------------------------------------------------------------------------------
void CellListWidget::resetForProject()
{
    if (md->projectType == MCNP)
    {
        selectPB->setText("UNSELECT ALL CELLS");
    }
    else
    {
        selectPB->setText("UNSELECT ALL ZONES");
    }
}
// ------------------------------------------------------------------------------------------------
void CellListWidget::writeSettings()
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    settings.setValue("cellListWidget/size",       this->size());
    settings.setValue("cellListWidget/Geometry",   this->saveGeometry());
}
// ------------------------------------------------------------------------------------------------
void CellListWidget::readSettings()
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    this->restoreGeometry(settings.value("cellListWidget/Geometry").toByteArray());
//    resize( settings.value("cellListWidget/size", sizeHint()).toSize() );
}
// ----------------------------------------------------------------------------
void CellListWidget::buildCellTable()
{
    cellListTreeWidget = new QTableWidget(0, 3);
    cellListTreeWidget->setHorizontalHeaderLabels(QStringList() << tr("Cell #") << tr("Name")
                                                                << tr("Show"));

#ifdef USING_WINDOWS
    cellListTreeWidget->setColumnWidth(0,  60);
    cellListTreeWidget->setColumnWidth(1, 170);
    cellListTreeWidget->setColumnWidth(2,  70);
    cellListTreeWidget->setMinimumWidth(360);
#else
    cellListTreeWidget->setColumnWidth(0,  80);
    cellListTreeWidget->setColumnWidth(1, 150);
    cellListTreeWidget->setColumnWidth(2,  70);
    cellListTreeWidget->setMinimumWidth(300);
#endif
    // Don't make the following height more than 50% larger than
    // the overall size min height for the CellListWidget.
    cellListTreeWidget->setMinimumHeight(300);
    cellListTreeWidget->setToolTip("Table of cells for the 3-D model.");
}
// ----------------------------------------------------------------------------
void CellListWidget::buildButtonLayout()
{
    selectPB = new QPushButton("UNSELECT ALL CELLS");
    if (md->projectType == ITS) selectPB->setText("UNSELECT ALL ZONES");

    buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(selectPB);

    connect(selectPB, SIGNAL(clicked()), this, SLOT(handleSelectAll()));
}
// ------------------------------------------------------------------------------------------------
void CellListWidget::handleSelectAll()
{
    QString thisText;

    QString s1;
    if (md->projectType == MCNP) s1 = "CELLS";
    else                         s1 = "ZONES";

    if (selectState)
    {
        selectState = false;
        selectPB->setText("SELECT ALL " + s1);
        thisText = "No";
    }
    else
    {
        selectState = true;
        selectPB->setText("UNSELECT ALL " + s1);
        thisText = "Yes";
    }

    for(int row=0; row < (int) cbox_display.size(); ++row)
    {
//     (*mcl)[row].display = state;
        cbox_display[row]->setChecked(selectState);
        cbox_display[row]->setText(thisText);
    }

}
// ------------------------------------------------------------------------------------------------
void CellListWidget::populateCellTable()
{
    for(int i=0; i < (int) md->cellDataStringList.size(); ++i)
    {
        QStringList sl = md->cellDataStringList[i];
//        qDebug() << sl << endl;
//        qDebug() << "1) " << sl[0] << "\t 2)" << sl[1] << endl;
        addRow(sl);
    }
}
// ------------------------------------------------------------------------------------------------
void CellListWidget::addRow(QStringList &sl)
{
    QMetaObject::Connection con;

    int row = cellListTreeWidget->rowCount();
    cellListTreeWidget->insertRow(row);

    QTableWidgetItem *item0 = new QTableWidgetItem;
    item0->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item0->setText(sl[0]);

    item0->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    cellListTreeWidget->setItem(row, 0, item0);

    QTableWidgetItem *item1 = new QTableWidgetItem;
    item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item1->setText(sl[1]);
    item1->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    cellListTreeWidget->setItem(row, 1, item1);

    SSS_CheckBox *ch1 = new SSS_CheckBox();

    int d = sl[9].toInt();
    if (d > 0)
    {
        ch1->setText("YES");
        ch1->setChecked(true);
    }
    else
    {
        ch1->setText("NO");
        ch1->setChecked(false);
    }

    ch1->setVal1(row);
    ch1->setCheck(ch1);
    ch1->setToolTip("Select / unselect this cell for visualization.");
    cbox_display.push_back(ch1);

    con = connect(cbox_display[row],  SIGNAL(pressed(int,int,SSS_CheckBox*)),
                  this,               SLOT(handleCellDisplayCheck(int,int,SSS_CheckBox*)));
    cellListTreeWidget->setCellWidget(row, 2, cbox_display[row]);
    cboxDisplayConnection.push_back(con);
}
// ----------------------------------------------------------------------------
void CellListWidget::handleCellDisplayCheck(int state, int row, SSS_CheckBox *check)
{
    if (state == 0)
    {
        check->setText("No");
    }
    else
    {
        check->setText("Yes");
    }

    // De-reference vector pointers.        [*SKILL*]
    (*mcl)[row].display = state;
    QString s;
    s = QString("%1").arg(state, 0);
    QStringList sl = md->cellDataStringList[row];
    sl[9] = s;
    md->cellDataStringList[row] = sl;

//    zoneOps[row].display = state;
//    qDebug() << "handleCellDisplayCheck:: state: " << state << "\trow: " << row;
//    activateShowButton();
}
// ----------------------------------------------------------------------------
void CellListWidget::clearCellTable()
{
    int nRows = cellListTreeWidget->rowCount();

    for (int i=0; i < nRows; ++i)
    {
        cellListTreeWidget->removeRow(0);
    }
    cbox_display.clear();

    for(size_t i=0; i<cboxDisplayConnection.size(); ++i)
        disconnect(cboxDisplayConnection[i]);
    cboxDisplayConnection.clear();

}
// ----------------------------------------------------------------------------
void CellListWidget::updateTable()
{
    clearCellTable();
    populateCellTable();
}
