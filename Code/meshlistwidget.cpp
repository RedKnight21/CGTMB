#include "meshlistwidget.h"

MeshListWidget::MeshListWidget(MasterData *mdIn, QVector<CellData> *mCellList)
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
MeshListWidget::~MeshListWidget()
{
   writeSettings();
}
// ----------------------------------------------------------------------------
QSize MeshListWidget::minimumSizeHint() const
{
    return QSize(350, 300);
}
// ----------------------------------------------------------------------------
QSize MeshListWidget::sizeHint() const
{
    return QSize(350, 600);
}
// ------------------------------------------------------------------------------------------------
void MeshListWidget::writeSettings()
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    settings.setValue("meshListWidget/size",       this->size());
    settings.setValue("meshListWidget/Geometry",   this->saveGeometry());
}
// ------------------------------------------------------------------------------------------------
void MeshListWidget::readSettings()
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    this->restoreGeometry(settings.value("meshListWidget/Geometry").toByteArray());
    resize( settings.value("meshListWidget/size", sizeHint()).toSize() );
}
// ------------------------------------------------------------------------------------------------
void MeshListWidget::resetForProject()
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
// ----------------------------------------------------------------------------
void MeshListWidget::buildCellTable()
{
    QString s1;
    if (md->projectType == MCNP) s1 = "Cell #";
    else                         s1 = "Zone #";


    cellListTreeWidget = new QTableWidget(0, 4);
    cellListTreeWidget->setHorizontalHeaderLabels(QStringList() << tr("Univ. #")
                                                                << s1 << tr("Name")
                                                                << tr("SHOW"));
#ifdef USING_WINDOWS
    cellListTreeWidget->setColumnWidth(0,  90);
    cellListTreeWidget->setColumnWidth(1,  90);
    cellListTreeWidget->setColumnWidth(2, 170);
    cellListTreeWidget->setColumnWidth(3,  90);
    cellListTreeWidget->setMinimumWidth(450);
#else
    cellListTreeWidget->setColumnWidth(0,  60);
    cellListTreeWidget->setColumnWidth(1,  70);
    cellListTreeWidget->setColumnWidth(2, 150);
    cellListTreeWidget->setColumnWidth(3,  60);
    cellListTreeWidget->setMinimumWidth(370);
#endif

    cellListTreeWidget->setMinimumHeight(300);
    if (md->projectType == MCNP)
        cellListTreeWidget->setToolTip("Table of cells for the 3-D model.");
    else
        cellListTreeWidget->setToolTip("Table of zones for the 3-D model.");
}
// ----------------------------------------------------------------------------
void MeshListWidget::buildButtonLayout()
{
    selectPB = new QPushButton("UNSELECT ALL CELLS");
    if (md->projectType == ITS) selectPB->setText("UNSELECT ALL ZONES");

    buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(selectPB);

    connect(selectPB, SIGNAL(clicked()), this, SLOT(handleSelectAll()));
}
// ------------------------------------------------------------------------------------------------
void MeshListWidget::handleSelectAll()
{
    QString thisText;
    //qDebug() << "PartChooserDialog::handleSelectAll:" << selectState;

    QString s1;
    if (md->projectType == MCNP) s1 = "CELLS";
    else                         s1 = "ZONES";

    if (selectState)
    {
        selectState = false;
        selectPB->setText("SELECT ALL " + s1);
        thisText = "NO";
    }
    else
    {
        selectState = true;
        selectPB->setText("UNSELECT ALL " + s1);
        thisText = "YES";
    }

    for(int row=0; row < (int) cbox_display.size(); ++row)
    {
//     (*mcl)[row].display = state;
        cbox_display[row]->setChecked(selectState);
        cbox_display[row]->setText(thisText);
    }

}
// ------------------------------------------------------------------------------------------------
void MeshListWidget::populateCellTable()
{
    for(int i=0; i < (int) md->instanceDataStringList.size(); ++i)
    {
        QStringList sl = md->instanceDataStringList[i];
//        qDebug() << sl << endl;
//        qDebug() << "1) " << sl[0] << "\t 2)" << sl[1] << endl;
        addRow(sl);
    }
    cellListTreeWidget->sortByColumn(0,Qt::AscendingOrder);
}
// ------------------------------------------------------------------------------------------------
void MeshListWidget::addRow(QStringList &sl)
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

    QTableWidgetItem *item2 = new QTableWidgetItem;
    item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item2->setText(sl[5]);
    item2->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    cellListTreeWidget->setItem(row, 2, item2);

    SSS_CheckBox *ch1 = new SSS_CheckBox();
    ch1->setText("YES");
    ch1->setChecked(true);
    int k;
    k = sl[0].toInt();
    ch1->setVal1(k);
    k = sl[1].toInt();
    ch1->setVal2(k);
    ch1->setCheck(ch1);
    ch1->setToolTip("Select / unselect this cell / zone for visualization.");
    cbox_display.push_back(ch1);

    con = connect(cbox_display[row],  SIGNAL(pressed(int, int, int, SSS_CheckBox*)),
                  this,               SLOT(handleCellDisplayCheck(int, int, int, SSS_CheckBox*)));
    cellListTreeWidget->setCellWidget(row, 3, cbox_display[row]);
    cboxDisplayConnection.push_back(con);
}
// ----------------------------------------------------------------------------
void MeshListWidget::handleCellDisplayCheck(int state, int univ, int cell, SSS_CheckBox *check)
{
    if (state == 0)
    {
        check->setText("NO");
    }
    else
    {
        check->setText("YES");
    }

    md->setInstanceDisplay(univ, cell, state);

    // De-reference vector pointers.        [*SKILL*]
    //(*mcl)[row].display = state;

//    zoneOps[row].display = state;
//    qDebug() << "handleCellDisplayCheck:: state: " << state << "\trow: " << row;
//    activateShowButton();
}
// ----------------------------------------------------------------------------
void MeshListWidget::clearCellTable()
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
void MeshListWidget::updateTable()
{
    //qDebug() << "MeshListWidget::updateTable";
    clearCellTable();
    populateCellTable();
}
