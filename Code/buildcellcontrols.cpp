#include "buildcellcontrols.h"
#include <QDebug>

BuildCellControls::BuildCellControls(MasterData *masterDataIn)
{
    md                     = masterDataIn;
    projectType            = md->projectType;
    currentBooleanTableRow = 0;
    currentCellNumber      = 1;
    editCellNo             = -1;
    bodiesForCell.clear();
    cbox_display.clear();
    cellNumMap.clear();

//    bodyDataInput = bodyDataInputIn;
//    bodyModel     = bodyModelIn;

    buildCellContructionLayout();
    buildCellDescriptionLayout();
    buildCellTreeLayout();

    QWidget *w1 = new QWidget;
    w1->setLayout(cellConstructionLayout);

    QWidget *w2 = new QWidget;
    w2->setLayout(cellTreeLayout);

    /*
    QVBoxLayout *masterLayout = new QVBoxLayout;

    // Using addStretch will keep the tree views from expanding vertically.
    masterLayout->addWidget(bottomWidget);
    masterLayout->addSpacing(30);
    masterLayout->addLayout(topLayout);
    masterLayout->addSpacing(30);
    masterLayout->addLayout(cellTreeLayout);

    setLayout(masterLayout);
 */

//    QSplitter *splitter;
//    splitter = new QSplitter(Qt::Vertical);
    this->setOrientation(Qt::Vertical);
    this->addWidget(cellDescriptionWidget);
    this->addWidget(w1);
    this->addWidget(w2);

    cellData.clear(currentCellNumber);
    addExistingCells();
}
// ----------------------------------------------------------------------------
void BuildCellControls::resetForProject()
{
    QStandardItem *item;
    projectType = md->projectType;
    if (projectType == MCNP)
    {
        topLabel->setText("Cell Construction Table");
        layoutTitle->setText("Cell Description");
        cellNumberLabel->setText("Cell #:");
        colorPB->setToolTip("Choose cell color.");
        modelListLabel->setText("Model Cell List");
        booleanShowPB->setToolTip("Display the cell in the cell graphics windows.");
        booleanAddPB->setToolTip("Add the cell in the boolean table to the master cell list.");

//        hHeaderList << QStringLiteral("Cell #") << QStringLiteral("Parameter")
//                    << QStringLiteral("Value");
        item = cellModel->horizontalHeaderItem(0);
        item->setText("Cell #");
    }
    else
    {
        topLabel->setText("Zone Construction Table");
        layoutTitle->setText("Zone Description");
        cellNumberLabel->setText("Zone #:");
        colorPB->setToolTip("Choose zone color.");
//        hHeaderList << QStringLiteral("Zone #") << QStringLiteral("Parameter")
//                    << QStringLiteral("Value");
        item = cellModel->horizontalHeaderItem(0);
        item->setText("Zone #");
        modelListLabel->setText("Model Zone List");
        booleanShowPB->setToolTip("Display the zone in the zone graphics windows.");
        booleanAddPB->setToolTip("Add the zone in the boolean table to the master zone list.");
    }
//    cellModel->setHorizontalHeaderLabels(hHeaderList);
}

// ----------------------------------------------------------------------------
void BuildCellControls::buildCellContructionLayout()
{
    cellConstructionLayout = new QVBoxLayout;

    buildBooleanTable();
    buildBooleanButtonLayout();

    topLabel = new QLabel("Cell Construction Table");
    if (projectType != MCNP) topLabel->setText("Zone Construction Table");

    adjustQLabel(topLabel, +1, true);
    topLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    cellConstructionLayout->addWidget(topLabel);
    cellConstructionLayout->addSpacing(10);
    cellConstructionLayout->addWidget(booleanTableWidget);
    cellConstructionLayout->addSpacing(5);
    cellConstructionLayout->addLayout(booleanButtonLayout);
    cellConstructionLayout->addStretch();
}
// ----------------------------------------------------------------------------
void BuildCellControls::buildBooleanTable()
{
    booleanTableWidget = new QTableWidget(0, 4);
    booleanTableWidget->setHorizontalHeaderLabels(QStringList() << tr("AND/OR") << tr("+/-")
                                                                << tr("BODY")   << tr("SHOW"));

#ifdef USING_WINDOWS
    booleanTableWidget->setColumnWidth(0, 95);
    booleanTableWidget->setColumnWidth(1, 60);
    booleanTableWidget->setColumnWidth(2, 80);
    booleanTableWidget->setColumnWidth(3, 80);
#else
    booleanTableWidget->setColumnWidth(0, 70);
    booleanTableWidget->setColumnWidth(1, 50);
    booleanTableWidget->setColumnWidth(2, 70);
    booleanTableWidget->setColumnWidth(3, 70);
#endif
    booleanTableWidget->setMinimumWidth(210);
    booleanTableWidget->setMinimumHeight(200);
    // vHeader = booleanTableWidget->verticalHeader();
    booleanTableWidget->setToolTip("Table of bodies for current cell.");
    booleanTableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(booleanTableWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
            this,               SLOT(onCustomContextMenu(const QPoint &)));

    connect(booleanTableWidget, SIGNAL(itemChanged(QTableWidgetItem *)), this,
                                SLOT(handleBooleanTableChange(QTableWidgetItem *)));
}

// ----------------------------------------------------------------------------
void BuildCellControls::addRow(int changeType, QStandardItem *item)
{
    QMetaObject::Connection con;

    // This must be before the row is added to the table so that zoneOps is
    // allocated before itemChanged is called.
    CGZoneOps b4Cell;
    b4Cell.display = true;

    // Set flag for Inside / Outside Sense: -1 -- Inside; 1 -- Outside :: MCNP scheme
    if (changeType == ANDPLUS || changeType == ORPLUS) b4Cell.senseOp = 1;
    else b4Cell.senseOp = -1;

    // Set flag for AND/OR ops: 0 -- AND; 1 -- OR
    if (changeType == ANDPLUS || changeType == ANDMINUS) b4Cell.boolOp = 0;
    else b4Cell.boolOp = 1;
    b4Cell.bodyNum = item->data(0).toInt();

    zoneOps.push_back(b4Cell);

    int row = booleanTableWidget->rowCount();
    booleanTableWidget->insertRow(row);

    QTableWidgetItem *item0 = new QTableWidgetItem;
    item0->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item0->setText(item->data(0).toString());
    booleanTableWidget->setItem(row, 2, item0);

    SSS_ComboBox *cb1 = new SSS_ComboBox();
    cb1->addItem(tr("+"));
    cb1->addItem(tr("-"));
    cb1->setVal1(row);
    cb1->setComboBox(cb1);
    cbox_sense.push_back(cb1);

    if      (changeType == ANDPLUS  || changeType == ORPLUS)  cb1->setCurrentIndex(0);
    else if (changeType == ANDMINUS || changeType == ORMINUS) cb1->setCurrentIndex(1);
    con = connect(cbox_sense[row],  SIGNAL(currentIndexChanged(int, int, int)),
                  this,             SLOT(handleCellSignChanged(int, int, int)));
    booleanTableWidget->setCellWidget(row, 1, cbox_sense[row]);
    cboxSenseConnection.push_back(con);

    SSS_ComboBox *cb2 = new SSS_ComboBox();
    cb2->addItem(tr("AND"));
    cb2->addItem(tr("OR"));
    cb2->setVal1(row);
    cb2->setComboBox(cb2);
    cbox_bool.push_back(cb2);

    if      (changeType == ANDPLUS || changeType == ANDMINUS) cb2->setCurrentIndex(0);
    else if (changeType == ORPLUS  || changeType == ORMINUS)  cb2->setCurrentIndex(1);

    con = connect(cbox_bool[row],   SIGNAL(currentIndexChanged(int, int, int)),
                  this,             SLOT(handleCellOpChanged(int, int, int)));
    booleanTableWidget->setCellWidget(row, 0, cbox_bool[row]);
    cboxBoolConnection.push_back(con);

    if (row == 0) cb2->setDisabled(true);

    SSS_CheckBox *ch1 = new SSS_CheckBox();
    ch1->setText("YES");
    ch1->setChecked(true);
    ch1->setVal1(row);
    ch1->setCheck(ch1);
    ch1->setToolTip("Select / unselect this body for visualization.");
    cbox_display.push_back(ch1);

    con = connect(cbox_display[row],  SIGNAL(pressed(int, int, SSS_CheckBox*)),
                  this,               SLOT(handleCellDisplayCheck(int, int, SSS_CheckBox*)));
    booleanTableWidget->setCellWidget(row, 3, cbox_display[row]);
    cboxDisplayConnection.push_back(con);
}

// ----------------------------------------------------------------------------
void BuildCellControls::addRow(CGZoneOps &zoneOp)
{
    QMetaObject::Connection con;

    int row = booleanTableWidget->rowCount();
    booleanTableWidget->insertRow(row);

    QTableWidgetItem *item0 = new QTableWidgetItem;
    item0->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    QString s1;
    s1.setNum(zoneOp.bodyNum);
    item0->setText(s1);
    booleanTableWidget->setItem(row, 2, item0);

    SSS_ComboBox *cb1 = new SSS_ComboBox();
    cb1->addItem(tr("+"));
    cb1->addItem(tr("-"));
    cb1->setVal1(row);
    cb1->setComboBox(cb1);
    cbox_sense.push_back(cb1);

    if   (zoneOp.senseOp == 1)  cb1->setCurrentIndex(0);
    else                        cb1->setCurrentIndex(1);
    con = connect(cbox_sense[row],  SIGNAL(currentIndexChanged(int,int,int)),
                  this,             SLOT(handleCellSignChanged(int,int,int)));
    booleanTableWidget->setCellWidget(row, 1, cbox_sense[row]);
    cboxSenseConnection.push_back(con);

    SSS_ComboBox *cb2 = new SSS_ComboBox();
    cb2->addItem(tr("AND"));
    cb2->addItem(tr("OR"));
    cb2->setVal1(row);
    cb2->setComboBox(cb2);
    cbox_bool.push_back(cb2);

    if   (zoneOp.boolOp == 0) cb2->setCurrentIndex(0);
    else                      cb2->setCurrentIndex(1);

    con = connect(cbox_bool[row],   SIGNAL(currentIndexChanged(int, int, int)),
                  this,             SLOT(handleCellOpChanged(int, int, int)));
    booleanTableWidget->setCellWidget(row, 0, cbox_bool[row]);
    cboxBoolConnection.push_back(con);

    if (row == 0) cb2->setDisabled(true);

    SSS_CheckBox *ch1 = new SSS_CheckBox();
    ch1->setText("YES");
    ch1->setChecked(zoneOp.display);
    ch1->setVal1(row);
    ch1->setCheck(ch1);
    ch1->setToolTip("Select / unselect this body for visualization.");
    cbox_display.push_back(ch1);

    con = connect(cbox_display[row],  SIGNAL(pressed(int, int, SSS_CheckBox*)),
                  this,               SLOT(handleCellDisplayCheck(int, int, SSS_CheckBox*)));
    booleanTableWidget->setCellWidget(row, 3, cbox_display[row]);
    cboxDisplayConnection.push_back(con);
}

// ----------------------------------------------------------------------------
void BuildCellControls::delRow(QStandardItem *item)
{
    // Removes from cell construction table when a body is deleted from
    // the body list in buildbodycontrols.
    int bodyNum = item->data(0).toInt();

    for (size_t i=0; i < zoneOps.size(); ++i)
    {
        if (bodyNum == zoneOps[i].bodyNum)
        {
            zoneOps.remove(i);
            break;
        }
    }

    int bn;
    QTableWidgetItem *item0;
    for (int row=0; row < booleanTableWidget->rowCount(); ++row)
    {
        item0 = booleanTableWidget->item(row, 2);
        bn    = item0->data(0).toInt();
        if (bn == bodyNum)
        {
            cbox_sense.erase(cbox_sense.begin()+row);
            cbox_bool.erase(cbox_bool.begin()+row);
            cbox_display.erase(cbox_display.begin()+row);
            cboxDisplayConnection.erase(cboxDisplayConnection.begin()+row);
            cboxBoolConnection.erase(cboxBoolConnection.begin()+row);
            cboxSenseConnection.erase(cboxSenseConnection.begin()+row);
            booleanTableWidget->removeRow(row);
            break;
        }
    }
}
// ----------------------------------------------------------------------------
void BuildCellControls::passBodyItem(int changeType, QStandardItem *item)
{
    // Called when buildbodycontrols adds a body to the buildcellcontrols body list or
    // deletes a body form the buildbodycontrols list.
    if (changeType < SSS_DEL)
    {
        addRow(changeType, item);
        ++currentBooleanTableRow;
    }
    else
    {
        delRow(item);
    }
}
// ----------------------------------------------------------------------------
void BuildCellControls::handleCellSignChanged(int value, int row, int val2)
{
//    zoneOps[row].senseOp = value;
    zoneOps[row].senseOp = 1 - 2 * value;
    activateShowButton();
}

// ----------------------------------------------------------------------------
void BuildCellControls::handleCellOpChanged(int value, int row, int val2)
{
    zoneOps[row].boolOp = value;
    activateShowButton();
}
// ----------------------------------------------------------------------------
void BuildCellControls::handleCellDisplayCheck(int state, int row, SSS_CheckBox *check)
{
    if (state == 0)
    {
        check->setText("NO");
    }
    else
    {
        check->setText("YES");
    }

    zoneOps[row].display = state;
    activateShowButton();
}
// ----------------------------------------------------------------------------
void BuildCellControls::handleBooleanTableChange(QTableWidgetItem *item)
{
    //qDebug() << "handleBooleanTableChange:: item: " << item->data(0) << "\t col" << item->column()
    //         << "\t row: " << item->row();
    //qDebug() << "size of zoneOps: " << zoneOps.size();
    if (item->column() == 2)
    {
        // Need to add checking for valid body number.

        zoneOps[item->row()].bodyNum = item->data(0).toInt();
    }

    // Check that the body number is valid.  If not, QDialog -- Error.
}

// ----------------------------------------------------------------------------
void BuildCellControls::onCustomContextMenu(const QPoint &point)
{
    //qDebug() << "** onCustomContextMenu **";
    QModelIndex index = booleanTableWidget->indexAt(point);
    if (index.isValid() )
    {
        //qDebug() << "index row: " << index.row() <<   "\n";
        QMenu contextMenu;
        QAction *rowDeleteAction  = contextMenu.addAction(tr("DELETE ROW"));

        QAction *action = contextMenu.exec(booleanTableWidget->viewport()->mapToGlobal(point));

        if (action == rowDeleteAction)
        {
            int row = index.row();
            //qDebug() << "+ DEL :: item: " << index.data(0) << index.data(1);

            cbox_sense.erase(cbox_sense.begin()+row);
            cbox_bool.erase(cbox_bool.begin()+row);
            cbox_display.erase(cbox_display.begin()+row);
            zoneOps.remove(row);

            cboxDisplayConnection.erase(cboxDisplayConnection.begin()+row);
            cboxBoolConnection.erase(cboxBoolConnection.begin()+row);
            cboxSenseConnection.erase(cboxSenseConnection.begin()+row);
            booleanTableWidget->removeRow(row);
            activateShowButton();
        }
    }
}
// ----------------------------------------------------------------------------
void BuildCellControls::updateCellNumbers(int oldCell, int newCell)
{
    QTableWidgetItem *item;
    QString s1, s2;
    int     cell;

    s2.sprintf("%d", newCell);

    for (int row=0; row<booleanTableWidget->rowCount(); ++row)
    {
       item = booleanTableWidget->item(row, 2);
       s1 = item->text();
       cell = s1.toInt();
       if (cell == oldCell)
       booleanTableWidget->item(row, 2)->setText(s2);
    }
}
// ----------------------------------------------------------------------------
void BuildCellControls::buildBooleanButtonLayout()
{
    booleanClearPB = new QPushButton("CLEAR");
    booleanShowPB  = new QPushButton("SHOW");
    booleanAddPB   = new QPushButton("ADD");

    booleanClearPB->setToolTip("Clear the boolean construction table of ALL rows.");
    booleanShowPB->setToolTip("Display the cell in the cell graphics windows.");
    booleanAddPB->setToolTip("Add the cell in the boolean table to the master cell list.");

    connect(booleanClearPB, SIGNAL(pressed()), this, SLOT(handleClearBooleanTable()));
    connect(booleanShowPB,  SIGNAL(pressed()), this, SLOT(handleCellShow()));
    connect(booleanAddPB,   SIGNAL(pressed()), this, SLOT(handleCellAdd()));

    booleanButtonLayout = new QHBoxLayout;
    booleanButtonLayout->addWidget(booleanClearPB);
    booleanButtonLayout->addWidget(booleanShowPB);
    booleanButtonLayout->addWidget(booleanAddPB);

    booleanClearPB->setEnabled(false);
    booleanShowPB->setEnabled(false);
    booleanAddPB->setEnabled(false);
}
// ----------------------------------------------------------------------------
void BuildCellControls::handleClearBooleanTable()
{
    int nRows = booleanTableWidget->rowCount();

    for (int i=0; i < nRows; ++i)
    {
        booleanTableWidget->removeRow(0);
    }
    zoneOps.clear();
    cbox_display.clear();
    cbox_bool.clear();
    cbox_sense.clear();

    for(size_t i=0; i<cboxBoolConnection.size(); ++i)
        disconnect(cboxBoolConnection[i]);
    cboxBoolConnection.clear();

    for(size_t i=0; i<cboxSenseConnection.size(); ++i)
        disconnect(cboxSenseConnection[i]);
    cboxSenseConnection.clear();

    for(size_t i=0; i<cboxDisplayConnection.size(); ++i)
        disconnect(cboxDisplayConnection[i]);
    cboxDisplayConnection.clear();

    cellData.clear(0);
    setCellDescription();

    booleanClearPB->setEnabled(false);
    booleanShowPB->setEnabled(false);
    booleanAddPB->setEnabled(false);
    disableCellWidgets();
    emit cellListReady(0);
}
// ----------------------------------------------------------------------------
void BuildCellControls::handleCellShow()
{
    emit buildCellShow();
}

// ----------------------------------------------------------------------------
void BuildCellControls::activateShowButton()
{
    booleanClearPB->setEnabled(true);
    booleanShowPB->setEnabled(true);
    booleanAddPB->setEnabled(true);
    enableCellWidgets();
}
// ----------------------------------------------------------------------------
void BuildCellControls::handleCellAdd()
{
    editCellNo = cellNoLE->text().toInt();

    if (cellNumMap[editCellNo] > 0)
    {
        QString s;
        s.setNum(editCellNo);
        QMessageBox msg;
        QString str = "Cell number " + s + " is already in use.  Please select another.\n";
        if (projectType != MCNP)
        {
            str = "Zone number " + s + " is already in use.  Please select another.\n";
        }
        msg.setIcon(QMessageBox::Information);
        msg.setInformativeText(str);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.show();
        msg.exec();
        return;
    }
    if (editCellNo == 0)
    {
        QMessageBox msg;
        QString str = "Please enter an unique cell number.\n";
        if (projectType != MCNP) str = "Please enter an unique zone number.\n";

        msg.setIcon(QMessageBox::Information);
        msg.setInformativeText(str);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.show();
        msg.exec();
        return;
    }
    cellNumMap[editCellNo] = editCellNo;

    if (cellData.name.isEmpty())
    {
        QString s1;
        s1.setNum(editCellNo);
        cellData.name = "Cell-" + s1;
        if (projectType != MCNP) cellData.name = "Zone-" + s1;
    }
    if (cellData.mtlNo.isEmpty())
    {
        cellData.mtlNo.setNum(0);
    }
    if (cellData.density.isEmpty())
    {
        cellData.density.setNum(0);
    }

    QString cell = cellData.cellNo;

    cellData.zoneOps.clear();
    for (int i=0; i < zoneOps.size(); ++i)
    {
        cellData.zoneOps.push_back(zoneOps[i]);
    }

    // Data describing the cell should be set, either by user input or default.
    // Store in the master vector.
    buildCells.push_back(cellData);

    cellTreeView->setSortingEnabled(false);

    // Add the body declaration as the parent node to the tree view.
    QList<QStandardItem *>  preparedRow = prepareParentRow(cell, "Name", cellData.name);

    QStandardItem *item2 = cellModel->invisibleRootItem();
    // adding a row to the invisible root item produces a root element
    item2->appendRow(preparedRow);

    QList<QStandardItem *> secondRow;

    secondRow = prepareChildRow(cell, "mtl. no.", cellData.mtlNo);
    preparedRow.first()->appendRow(secondRow);

    secondRow = prepareChildRow(cell, "density", cellData.density);
    preparedRow.first()->appendRow(secondRow);

    if (projectType == MCNP)
    {
        secondRow = prepareChildRow(cell, "importance", cellData.importance);
        preparedRow.first()->appendRow(secondRow);
    }

    QString s1 = "Adding Cell: " + cell + " " + cellData.name;
    if (projectType != MCNP) s1 = "Adding Zone: " + cell + " " + cellData.name;

    s1 += "\n     Mtl No: " + cellData.mtlNo;
    s1 += "  Density: " + cellData.density;
    s1 += "\n";

    cellTreeView->setSortingEnabled(true);

    emit cellControlsMessage(s1);

    // ----------------------------------------------------------------
    // addToCellDataStringList MUST be before handleClearBooleanTable
    addToCellDataStringList();
    // ----------------------------------------------------------------

    handleClearBooleanTable();
    booleanClearPB->setEnabled(false);
    booleanShowPB->setEnabled(false);
    booleanAddPB->setEnabled(false);
    meshReduction->setEnabled(false);
//    xResolution->setEnabled(false);
//    yResolution->setEnabled(false);
//    zResolution->setEnabled(false);

    cellData.color.setNamedColor("White");

    setCellDescription();
    disableCellWidgets();
    emit cellListReady(2);
    emit cellChangedSignal(2, editCellNo);
}
// ----------------------------------------------------------------------------
QList<QStandardItem *> BuildCellControls::prepareParentRow(const QString &first,
                                                           const QString &second,
                                                           const QString &third)
{
    QList<QStandardItem *> rowItems;

    QStandardItem *a = new QStandardItem(first);
    QStandardItem *b = new QStandardItem(second);
    QStandardItem *c = new QStandardItem(third);

    if (projectType == MCNP) a->setEditable(true);
    else a->setEditable(false);
    b->setEditable(false);
    c->setEditable(true);
    rowItems << a << b << c;

    return rowItems;
}
// ----------------------------------------------------------------------------
QList<QStandardItem *> BuildCellControls::prepareChildRow(const QString &first,
                                                          const QString &second,
                                                          const QString &third)
{
    QList<QStandardItem *> rowItems;

    QStandardItem *a = new QStandardItem(first);
    QStandardItem *b = new QStandardItem(second);
    QStandardItem *c = new QStandardItem(third);
    a->setEditable(false);
    b->setEditable(false);
    c->setEditable(true);
    rowItems << a << b << c;

    return rowItems;
}
// ----------------------------------------------------------------------------
void BuildCellControls::addToCellDataStringList()
{
    int         i;
    int         n1;
    int         n2;
    QString     s1;
    QStringList sl;

    sl.push_back(cellData.cellNo);
    sl.push_back(cellData.name);
    sl.push_back(cellData.mtlNo);
    sl.push_back(cellData.density);
    sl.push_back(cellData.importance);

    // Store the color
    i = cellData.color.red();
    s1.setNum(i);
    sl.push_back(s1);
    i = cellData.color.green();
    s1.setNum(i);
    sl.push_back(s1);
    i = cellData.color.blue();
    s1.setNum(i);
    sl.push_back(s1);
    i = cellData.color.alpha();    
    s1.setNum(i);
    sl.push_back(s1);

    // Display indicator.
    s1.setNum(2);
    sl.push_back(s1);

    // Mesh reduction.
    s1.setNum(cellData.meshReduction);
    sl.push_back(s1);

//    s1.setNum(cellData.yRes);
//    sl.push_back(s1);
//    s1.setNum(cellData.zRes);
//    sl.push_back(s1);

    // Store the zone ops
    i = (int) cellData.zoneOps.size();
    s1.setNum(i);
    sl.push_back(s1);

    int klim = i;
    for(int k=0; k<klim; ++k)
    {
        i = cellData.zoneOps[k].boolOp;
        s1.setNum(i);
        sl.push_back(s1);

        i = cellData.zoneOps[k].senseOp;
        s1.setNum(i);
        sl.push_back(s1);

        i = cellData.zoneOps[k].bodyNum;
        s1.setNum(i);
        sl.push_back(s1);

        i = (int) cellData.zoneOps[k].display;
        s1.setNum(i);
        sl.push_back(s1);
    }

    // Insert in order by cell number.
    QStringList l;
    klim = (int) md->cellDataStringList.size();

    if (klim > 0)
    {
        n1 = sl[0].toInt();
        for(i=0; i<klim; ++i)
        {
            l = md->cellDataStringList[i];
            n2 = l[0].toInt();

            if (n2 > n1) break;
        }
        md->cellDataStringList.insert(i, sl);
    }
    else
        md->cellDataStringList.push_back(sl);
}
// ----------------------------------------------------------------------------
void BuildCellControls::addExistingCells()
{
    for(int b=0; b < (int) md->cellDataStringList.size(); ++b)
    {
        addSingleCellFromList(b);
    }
    cellData.color.setNamedColor("White");
    emit cellListReady(0);
}
// ----------------------------------------------------------------------------
void BuildCellControls::addSingleCellFromList(int b)
{
    int i;
    QStringList sl = md->cellDataStringList[b];

    cellData.clear(0);

    cellData.cellNo     = sl[0];
    cellData.name       = sl[1];
    cellData.mtlNo      = sl[2];
    cellData.density    = sl[3];
    cellData.importance = sl[4];

    int num = cellData.cellNo.toInt();
    cellNumMap[num] = num;

    i = sl[5].toInt();
    cellData.color.setRed(i);
    i = sl[6].toInt();
    cellData.color.setGreen(i);
    i = sl[7].toInt();
    cellData.color.setBlue(i);
    i = sl[8].toInt();
    cellData.color.setAlpha(i);

    i = sl[9].toInt();
    cellData.display = i;

    i = sl[10].toInt();
    cellData.meshReduction = i;

//    cellData.xRes = i;
//    i = sl[11].toInt();
//    cellData.yRes = i;
//    i = sl[12].toInt();
//    cellData.zRes = i;


    int nOps = sl[11].toInt();
    int k = 11;
    for (int n=0; n<nOps; ++n)
    {
        CGZoneOps zo;
        ++k;
        zo.boolOp = sl[k].toInt();

        ++k;
        zo.senseOp = sl[k].toInt();

        ++k;
        zo.bodyNum = sl[k].toInt();

        ++k;
        zo.display = (bool) sl[k].toInt();

        cellData.zoneOps.push_back(zo);
    }

    // Data describing the cell should be set, either by user input or default.
    // Store in the master vector.
    buildCells.push_back(cellData);

    cellTreeView->setSortingEnabled(false);

    // Add the body declaration as the parent node to the tree view.
    QList<QStandardItem *>  preparedRow = prepareParentRow(cellData.cellNo, "Name", cellData.name);

    QStandardItem *item2 = cellModel->invisibleRootItem();
    // adding a row to the invisible root item produces a root element
    item2->appendRow(preparedRow);

    QList<QStandardItem *> secondRow;

    secondRow = prepareChildRow(cellData.cellNo, "mtl. no.", cellData.mtlNo);
    preparedRow.first()->appendRow(secondRow);

    secondRow = prepareChildRow(cellData.cellNo, "density", cellData.density);
    preparedRow.first()->appendRow(secondRow);

    if (projectType == MCNP)
    {
        secondRow = prepareChildRow(cellData.cellNo, "importance", cellData.importance);
        preparedRow.first()->appendRow(secondRow);
    }

    QString s1 = "Adding Cell: " + cellData.cellNo + " " + cellData.name;
    if (projectType != MCNP) s1 = "Adding Zone: " + cellData.cellNo + " " + cellData.name;

    s1 += "     Mtl No: " + cellData.mtlNo;
    s1 += "  Density: " + cellData.density;

    cellTreeView->setSortingEnabled(true);

    emit cellControlsMessage(s1);
}
// ----------------------------------------------------------------------------

void BuildCellControls::buildCellDescriptionLayout()
{
    if (projectType== MCNP)
        layoutTitle     = new QLabel("Cell Description");
    else
        layoutTitle     = new QLabel("Zone Description");

    QLabel *cellComment     = new QLabel("Name:");
    QLabel *materialLabel   = new QLabel("Material #:");

    if (projectType== MCNP)
        cellNumberLabel = new QLabel("Cell #:");
    else
        cellNumberLabel = new QLabel("Zone #:");

    QLabel *densityLabel    = new QLabel("Density:");
    QLabel *importanceLabel = new QLabel("Importance:");
//    QLabel *colorLabel      = new QLabel("Color:");
    QLabel *resolutionLabel = new QLabel("Reduction:");


    adjustQLabel(layoutTitle, +1, true);

    nameLE       = new QLineEdit;
    cellNoLE     = new QLineEdit;
    materialNoLE = new QLineEdit;
    densityLE    = new QLineEdit;
    importanceLE = new QLineEdit;

    importanceLE->setText("1.0");

    cellNoLE->setMinimumWidth(70);

    colorPB = new QPushButton("COLOR");
    colorPB->setMinimumWidth(100);
    colorPB->setAutoFillBackground(true);
    colorPB->setPalette( QColor( "White" ) );
    if (projectType== MCNP)
        colorPB->setToolTip("Choose cell color.");
    else
        colorPB->setToolTip("Choose zone color.");

    meshReduction = new QSpinBox();
    meshReduction->setRange(0,90);
    meshReduction->setValue(0);
    meshReduction->setMinimumWidth(55);
    meshReduction->setSingleStep(5);
    meshReduction->setToolTip("Set % reduction in number of mesh used to render.");

/*
    xResolution = new QSpinBox();
    xResolution->setRange(10,200);
    xResolution->setValue(50);
    xResolution->setMinimumWidth(55);
    xResolution->setSingleStep(5);
    xResolution->setToolTip("Set number of evalulation points along X for rendering evalulation.");

    yResolution = new QSpinBox();
    yResolution->setRange(10,200);
    yResolution->setValue(50);
    yResolution->setMinimumWidth(55);
    yResolution->setSingleStep(5);
    yResolution->setToolTip("Set number of evalulation points along Y for rendering evalulation.");

    zResolution = new QSpinBox();
    zResolution->setRange(10,200);
    zResolution->setValue(50);
    zResolution->setMinimumWidth(55);
    zResolution->setSingleStep(5);
    zResolution->setToolTip("Set number of evalulation points along Z for rendering evalulation.");
*/
    QGridLayout *bottomLayout = new QGridLayout;
    bottomLayout->addWidget(layoutTitle,     0, 0, 1, 4, Qt::AlignHCenter);
    bottomLayout->addWidget(cellComment,     1, 0, 1, 1, Qt::AlignHCenter);
    bottomLayout->addWidget(cellNumberLabel, 2, 0, 1, 1, Qt::AlignHCenter);
    bottomLayout->addWidget(materialLabel,   3, 0, 1, 1, Qt::AlignHCenter);
    bottomLayout->addWidget(densityLabel,    3, 2, 1, 1, Qt::AlignHCenter);
    if (projectType == MCNP)
        bottomLayout->addWidget(importanceLabel, 5, 0, 1, 1, Qt::AlignHCenter);
//    bottomLayout->addWidget(colorLabel,      2, 2, 1, 1, Qt::AlignHCenter);
    bottomLayout->addWidget(resolutionLabel, 5, 2, 1, 1, Qt::AlignHCenter);
//    bottomLayout->addWidget(xResolution,     6, 1, 1, 1, Qt::AlignHCenter);
//    bottomLayout->addWidget(yResolution,     6, 2, 1, 1, Qt::AlignHCenter);
//    bottomLayout->addWidget(zResolution,     6, 3, 1, 1, Qt::AlignHCenter);

    bottomLayout->addWidget(meshReduction,     5, 3, 1, 1, Qt::AlignHCenter);


    bottomLayout->addWidget(nameLE,          1, 1, 1, 3);
    bottomLayout->addWidget(cellNoLE,        2, 1, 1, 1);
    bottomLayout->addWidget(materialNoLE,    3, 1, 1, 1);
    bottomLayout->addWidget(densityLE,       3, 3, 1, 1);
    if (projectType == MCNP)
        bottomLayout->addWidget(importanceLE,    5, 1, 1, 1);
    bottomLayout->addWidget(colorPB,         2, 3, 1, 1);

    cellDescriptionWidget = new QWidget;
    cellDescriptionWidget->setLayout(bottomLayout);

    connect(nameLE,        SIGNAL(textChanged(QString)), this, SLOT(handleNameEdit(QString)));
    connect(materialNoLE,  SIGNAL(textChanged(QString)), this, SLOT(handleMaterialEdit(QString)));
    connect(cellNoLE,      SIGNAL(textChanged(QString)), this, SLOT(handleCellNoEdit(QString)));
    connect(cellNoLE,      SIGNAL(editingFinished()),    this, SLOT(handleCellNoFinish()));
    connect(densityLE,     SIGNAL(textChanged(QString)), this, SLOT(handleDensityEdit(QString)));
    if (projectType == MCNP)
        connect(importanceLE,  SIGNAL(textChanged(QString)), this, SLOT(handleImportanceEdit(QString)));
    connect(colorPB,       SIGNAL(clicked()),            this, SLOT(handleColorButton()));

//    connect(xResolution,   SIGNAL(valueChanged(int)),    this, SLOT(xResChange(int)));
//    connect(yResolution,   SIGNAL(valueChanged(int)),    this, SLOT(yResChange(int)));
//    connect(zResolution,   SIGNAL(valueChanged(int)),    this, SLOT(zResChange(int)));

    connect(meshReduction,   SIGNAL(valueChanged(int)),    this, SLOT(meshReductionChange(int)));


    disableCellWidgets();
}

// ------------------------------------------------------------------------------------------------
void BuildCellControls::xResChange(int val)
{
    cellData.xRes = val;
}
// ------------------------------------------------------------------------------------------------
void BuildCellControls::yResChange(int val)
{
    cellData.yRes= val;
}
// ------------------------------------------------------------------------------------------------
void BuildCellControls::zResChange(int val)
{
    cellData.zRes = val;
}

// ------------------------------------------------------------------------------------------------
void BuildCellControls::meshReductionChange(int val)
{
    cellData.meshReduction = val;
}

// ----------------------------------------------------------------------------
void BuildCellControls::setCellDescription()
{
    nameLE->setText(cellData.name);
    cellNoLE->setText(cellData.cellNo);
    materialNoLE->setText(cellData.mtlNo);
    densityLE->setText(cellData.density);
    importanceLE->setText(cellData.importance);
    colorPB->setPalette(cellData.color);
//    xResolution->setValue(cellData.xRes);
//    yResolution->setValue(cellData.yRes);
//    zResolution->setValue(cellData.zRes);
    meshReduction->setValue(cellData.meshReduction);
}
// ----------------------------------------------------------------------------
void BuildCellControls::enableCellWidgets()
{
    nameLE->setEnabled(true);
    cellNoLE->setEnabled(true);
    materialNoLE->setEnabled(true);
    densityLE->setEnabled(true);
    if (projectType == MCNP)
        importanceLE->setEnabled(true);
    colorPB->setEnabled(true);
//    xResolution->setEnabled(true);
//    yResolution->setEnabled(true);
//    zResolution->setEnabled(true);
    meshReduction->setEnabled(true);
}

// ----------------------------------------------------------------------------
void BuildCellControls::disableCellWidgets()
{
    nameLE->setEnabled(false);
    cellNoLE->setEnabled(false);
    materialNoLE->setEnabled(false);
    densityLE->setEnabled(false);
    if (projectType == MCNP)
        importanceLE->setEnabled(false);
    colorPB->setEnabled(false);
//    xResolution->setEnabled(false);
//    yResolution->setEnabled(false);
//    zResolution->setEnabled(false);
    meshReduction->setEnabled(false);
}
// ----------------------------------------------------------------------------
void BuildCellControls::handleNameEdit(QString s)
{
    cellData.name = s;
}
// ----------------------------------------------------------------------------
void BuildCellControls::handleMaterialEdit(QString s)
{
    cellData.mtlNo = s;
}
// ----------------------------------------------------------------------------
void BuildCellControls::handleCellNoEdit(QString s)
{
    cellData.cellNo = s;
}
// ----------------------------------------------------------------------------
void BuildCellControls::handleCellNoFinish()
{
    QString s = cellNoLE->text();
    int num   = s.toInt();
    int num2  = cellNumMap.value(num);

    if (num2 > 0)
    {
        QMessageBox msg;
        QString str;
        if (projectType == MCNP)
            str = "Cell number " + s + " is already in use.  Please select another.\n";
        else
            str = "Zone number " + s + " is already in use.  Please select another.\n";

        msg.setIcon(QMessageBox::Information);
        msg.setInformativeText(str);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.show();
        msg.exec();
        cellNoLE->setText("");
    }
    else
    {
        cellData.cellNo = s;
        editCellNo = num;
    }
}
// ----------------------------------------------------------------------------
void BuildCellControls::handleDensityEdit(QString s)
{
    cellData.density = s;
}
// ----------------------------------------------------------------------------
void BuildCellControls::handleImportanceEdit(QString s)
{
    cellData.importance = s;
}
// ----------------------------------------------------------------------------
void BuildCellControls::handleColorButton()
{
    QString s1;

    if (projectType == MCNP)  s1 = "Cell Color Chooser";
    else s1 = "Zone Color Chooser";

    QColor colorOld(cellData.color);

    QColor color = QColorDialog::getColor(colorOld, this, s1, QColorDialog::ShowAlphaChannel);
    if (color.isValid())
    {
        colorPB->setPalette(color);
        cellData.color = color;
    }
}
// ----------------------------------------------------------------------------
void BuildCellControls::buildCellTreeLayout()
{
    cellModel = new QStandardItemModel;

    if (projectType == MCNP)
        hHeaderList << QStringLiteral("Cell #") << QStringLiteral("Parameter") << QStringLiteral("Value");
    else
        hHeaderList << QStringLiteral("Zone #") << QStringLiteral("Parameter") << QStringLiteral("Value");

    cellModel->setHorizontalHeaderLabels(hHeaderList);

    connect(cellModel, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(cellModelItemChanged(QStandardItem *)));

    cellTreeView = new QTreeView;
    cellTreeView->setModel(cellModel);
    cellTreeView->expandAll();

#ifdef USING_WINDOWS
    cellTreeView->setColumnWidth(0,  80);
    cellTreeView->setColumnWidth(1, 120);
    cellTreeView->setColumnWidth(2, 100);
#else
    cellTreeView->setColumnWidth(0, 70);
    cellTreeView->setColumnWidth(1, 90);
    cellTreeView->setColumnWidth(2, 90);
#endif

    cellTreeView->sortByColumn(0, Qt::AscendingOrder);
    cellTreeView->setSortingEnabled(true);
    cellTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(cellTreeView, SIGNAL(customContextMenuRequested(const QPoint &)),
            this,         SLOT(onCustomContextMenu2(const QPoint &)));

    modelListLabel = new QLabel("Model Cell List");
    if (projectType != MCNP) modelListLabel->setText("Model Zone List");

//    QFont f = label1->font();
//    qDebug() << "label1 font: " << label1->font();
//    qDebug() << "family: " << f.family();
//    qDebug() << "weight: " << f.weight();
//    qDebug() << "bold  : " << f.bold();
//    qDebug() << "size  : " << f.pointSize();

    modelListLabel->setAlignment(Qt::AlignHCenter);
    adjustQLabel(modelListLabel, +1, true);
    cellTreeView->setMinimumSize(250, 200);
    cellTreeLayout = new QVBoxLayout;
    cellTreeLayout->addWidget(modelListLabel);
    cellTreeLayout->addWidget(cellTreeView);
}
// ----------------------------------------------------------------------------
void BuildCellControls::updateBuildCellsChild(int cell, int row, QStandardItem *item)
{
    for(int k=0; k<buildCells.size(); ++k)
    {
        if (cell == buildCells[k].cellNo.toInt())
        {
            if      (row == 0) buildCells[k].mtlNo      = item->data(0).toString();
            else if (row == 1) buildCells[k].density    = item->data(0).toString();
            else if (row == 2) buildCells[k].importance = item->data(0).toString();
        }
    }
}
// ----------------------------------------------------------------------------
void BuildCellControls::updateBuildCells(int oldBody, int column, QStandardItem *item)
{
    //qDebug() << "updateBuildBodies2   :: data(0) : " << item->data(0);
    //qDebug() << "updateBuildBodies2   :: data(1) : " << item->data(1);
    //qDebug() << "updateBuildBodies2   :: data(2) : " << item->data(2);

    if (column == 2)
    {
        //qDebug() << "changing the body name\t oldBody: " << oldBody << endl;

        QStandardItem *it = cellModel->item(oldBody, 0);
        int        cellNo = it->data(0).toInt();

        for(size_t k=0; k<buildCells.size(); ++k)
        {
            if (cellNo == buildCells[k].cellNo.toInt())
            {
                buildCells[k].name = item->data(0).toString();
                //qDebug() << "new name: " << buildBodies[k].bodyName  << endl;
            }
        }
    }
    else
    {
        int bodyNo = item->data(0).toInt();
        //qDebug() << "bodyNo : " << bodyNo << "\t oldBody: " << oldBody << endl;

        for(size_t k=0; k<buildCells.size(); ++k)
        {
            if (oldBody == buildCells[k].cellNo)
            {
                buildCells[k].cellNo = bodyNo;
                //qDebug() << "new bodyNumber: " << buildBodies[k].bodyNumber  << endl;
            }
        }
    }
}
// ----------------------------------------------------------------------------
void BuildCellControls::cellModelItemChanged(QStandardItem *cellModelItem)
{
    // Does execution of this function update the body and model 3-D graphics?

    //qDebug() << "cellModelItemChanged:: data         :  " << cellModelItem->data(0);
    //qDebug() << "cellModelItemChanged:: row          :  " << cellModelItem->row();
    //qDebug() << "cellModelItemChanged:: parent       :  " << cellModelItem->parent();
    //qDebug() << "cellModelItemChanged:: has children :  " << cellModelItem->hasChildren();

    QStandardItem *child;

    if (cellModelItem->parent() != nullptr)
    {
        //qDebug() << "Parent row: " << cellModelItem->parent()->row() << "\t data: " << cellModelItem->parent()->data(0) << endl;
        // Child rows changed.
        int cellNo = cellModelItem->parent()->data(0).toInt();
        updateBuildCellsChild(cellNo, cellModelItem->row(), cellModelItem);
        QStandardItem *itemChanged = cellModelItem;
/*
        qDebug() << "parent data     : " << bodyModelItem->parent()->data(0) << endl;
        qDebug() << "parent row count: " << bodyModelItem->parent()->rowCount() << endl;

        for ( int i = 0; i < bodyModelItem->parent()->rowCount(); ++i )
        {
            child = bodyModelItem->parent()->child(i, 2);
            qDebug() << "Row: " << i << "\tchild data: " << child->data(0);
        }
*/
    }
    else
    {
        if (cellModelItem->column() == 0)
        {
            int pCell = cellModelItem->data(0).toInt() ;                // parent body no.
            int cCell = cellModelItem->child(0, 0)->data(0).toInt();    // child body no.

            //qDebug() << "cellNumMap values: " << cellNumMap.values() << endl;

            // Is the user body number number already in use?
            if (cellNumMap.value(pCell, -1) > -1)
            {
                // Re-set body number before issuing error message.
                QString s1 = cellModelItem->child(0, 0)->data(0).toString();

                cellTreeView->blockSignals(true);
                cellModel->blockSignals(true);
                QStandardItem *si2 = cellModel->item(cellModelItem->row(), 0);
                si2->setText(s1);
                cellModel->blockSignals(false);
                cellTreeView->blockSignals(false);

                QMessageBox msg;
                QString str = "Cell number is already in use.  Please select another.\n";
                msg.setIcon(QMessageBox::Information);
                msg.setInformativeText(str);
                msg.setStandardButtons(QMessageBox::Ok);
                msg.setDefaultButton(QMessageBox::Ok);
                msg.show();
                msg.exec();
                return;
            }
            else
            {
                // New user body number is okay.
                int k = cellNumMap[cCell];
                cellNumMap[pCell] = k;
                cellNumMap.remove(cCell);

                // Re-set the children to the new user body number.
                cellTreeView->blockSignals(true);
                cellModel->blockSignals(true);
                for ( int i = 0; i < cellModelItem->rowCount(); ++i )
                {
                    child = cellModelItem->child(i, 0);
                    //qDebug() << "-- Row: " << i << "\tchild data: " << child->data(0);
                    child->setData(cellModelItem->data(0) , 0);
                }
                cellModel->blockSignals(false);
                cellTreeView->blockSignals(false);

//                emit bodyNumberSignal(cBody, pBody);
            }
        }                   // -- end parent column stuff
        else if (cellModelItem->column() == 2)
        {
            updateBuildCells(cellModelItem->row(), cellModelItem->column(), cellModelItem);
        }
    }
}
// ----------------------------------------------------------------------------
void BuildCellControls::onCustomContextMenu2(const QPoint &point)
{
    //qDebug() << "** onCustomContextMenu2 **";
    QModelIndex index = cellTreeView->indexAt(point);
    if (index.isValid() )
    {
        QString cellString;
        cellString = index.data(0).toString();
        //qDebug() << "index row: " << index.row() <<  "\t columne: " << index.column() << "\n";

        if (index.column() > 0)
        {
            QMessageBox about;
            QString str = "Please right click on cell column for menu.\n";
            if (projectType != MCNP)
                    str = "Please right click on zone column for menu.\n";
            about.setIcon(QMessageBox::Information);
            about.setInformativeText(str);
            about.setStandardButtons(QMessageBox::Ok);
            about.setDefaultButton(QMessageBox::Ok);
            about.show();
            about.exec();
            return;
        }

        QStandardItem *itemClicked = cellModel->itemFromIndex(index);
        //qDebug() << "itemClicked rowCount: " << itemClicked->rowCount();

        QMenu contextMenu;

        QAction *editAction      = contextMenu.addAction(tr("EDIT"));
        QAction *orDeleteAction  = contextMenu.addAction(tr("DELETE"));

        QAction *action = contextMenu.exec(cellTreeView->viewport()->mapToGlobal(point));

        int actionType;

        int cell = index.data(0).toInt();
        if (action == editAction)
        {
            actionType = EDIT;
            handleClearBooleanTable();
            for(size_t k=0; k<buildCells.size(); ++k)
            {
                if (cell == buildCells[k].cellNo.toInt())
                {
                    editCellNo = k;
                    cellData   = buildCells[k];
                    buildCells.remove(k);
                    cellNumMap[cell] = 0;
                    break;
                }
            }
            setEditFields(itemClicked, index);
            editFlag = true;
        }
        else if (action == orDeleteAction)
        {
            actionType = SSS_DEL;
 //           emit bodyAddSignal(actionType, itemClicked);

//            qDebug() << "DELETE :: item: " << index.data(0) << index.data(1);
            // 1st delete child rows then parent.
            cellModel->removeRows(itemClicked->row(), itemClicked->rowCount(), index);
            cellModel->removeRow(itemClicked->row());
        }

        // For both actions, remove from the master data vector.
        removeFromCellDataStringList(cellString);

        emit cellListReady(1);
        emit cellChangedSignal(1, cell);

        /*
         * This next cast sequence was a pain to find!
         *
        const QStandardItemModel *model = static_cast<QStandardItemModel * >
                 (const_cast<QAbstractItemModel *> (index.model() ));

        QStandardItem *sItem = model->itemFromIndex(index);
        */
    }           // -- end index is valid
}
// ----------------------------------------------------------------------------
void BuildCellControls::removeFromCellDataStringList(QString cell)
{
    // Given a cell number, remove the QStringList from the master data vector.
    QStringList sl;

    cellData.cellNo = cell;

    for(int i=0; i < (int) md->cellDataStringList.size(); ++i)
    {
        sl = md->cellDataStringList[i];
        if (cell == sl[0])
        {
            md->cellDataStringList.removeAt(i);
            break;
        }
    }
}
// ----------------------------------------------------------------------------
void BuildCellControls::setEditFields(QStandardItem *itemClicked, QModelIndex index)
{
    zoneOps.clear();
    setCellDescription();
    booleanTableWidget->blockSignals(true);
    for(size_t k=0; k < (int) cellData.zoneOps.size(); ++k)
    {
        addRow(cellData.zoneOps[k]);
        zoneOps.push_back(cellData.zoneOps[k]);
    }
    booleanTableWidget->blockSignals(false);

    cellTreeView->blockSignals(true);
    // 1st delete child rows then parent.
    cellModel->removeRows(itemClicked->row(), itemClicked->rowCount(), index);
    cellModel->removeRow(itemClicked->row());
    cellTreeView->blockSignals(false);
    cellNoLE->setEnabled(false);
    activateShowButton();
}
// ----------------------------------------------------------------------------
QString BuildCellControls::handleBodyNumberQueryRequest(int bodyNo)
{
    QString s1;
    s1.setNum(bodyNo);
    QString msg;
    QVector<QString> cns;
    cns.clear();

    for(int k=0; k < (int) buildCells.size(); ++k)
    {
        for(int i=0; i < (int) buildCells[k].zoneOps.size(); ++i)
        {
            if (bodyNo == buildCells[k].zoneOps[i].bodyNum)
            {
                cns.push_back(buildCells[k].cellNo);
                break;
            }
        }
    }

    msg = "Body: " + s1;
    if (cns.size() == 0)
    {
        if (projectType == MCNP)
            msg += " not in use by any cell.";
        else
            msg += " not in use by any zone.";
    }
    else
    {
        if (projectType == MCNP)
            msg += " used by the following cells: ";
        else
            msg += " used by the following zones: ";

        for(int i=0; i < (int) cns.size(); ++i)
            msg += (cns[i] + "  ");
    }
    return  msg;
}

// ----------------------------------------------------------------------------
void BuildCellControls::handleBodyNumberRenumberRequest(int bodyNo)
{
    QString s1;
    s1.setNum(bodyNo);

    for(int k=0; k < (int) buildCells.size(); ++k)
    {
        for(int i=0; i < (int) buildCells[k].zoneOps.size(); ++i)
        {
            if (buildCells[k].zoneOps[i].bodyNum >= bodyNo)
            {
//                qDebug() << "Cell: " << buildCells[k].cellNo
//                         << "Renumber " << buildCells[k].zoneOps[i].bodyNum;
                --buildCells[k].zoneOps[i].bodyNum;
            }
        }
    }

    // Now, renumber the body numbers in the cell descriptions.
    QString s;
    QStringList sl;
    for(int k=0; k < (int) md->cellDataStringList.size(); ++k)
    {
        sl = md->cellDataStringList[k];
        int nOps = sl[11].toInt();
        int bNum;
        int m;
        for(int n=0; n < nOps; ++n)
        {
            m = 14 + n * 4;
            bNum = sl[m].toInt();
            if (bNum > bodyNo)
            {
                --bNum;
                s.setNum(bNum);
                sl[m] = s;
            }
        }
        md->cellDataStringList[k] = sl;
    }
}
