#include "meshuniversebuild.h"

MeshUniverseBuild::MeshUniverseBuild(MasterData *mdIn, QVector<ReadFileAbaqus> *abqFiles)
{
    this->setOrientation(Qt::Horizontal);
    md          = mdIn;
    abaqusFiles = abqFiles;
    zoneStart   = 0;

    buildTopGridLayout();
    buildMidGridLayout();
    buildMeshInstanceTable();
    buildRightSideLayout();

    // ------------------------------------------------------------------------
    QVBoxLayout *leftLayout = new QVBoxLayout;

    leftLayout->addLayout(midGridLayout);
    leftLayout->addSpacing(30);
    leftLayout->addLayout(topGridLayout);
    leftLayout->addSpacing(30);
    leftLayout->addWidget(meshInstanceTable);
    leftLayout->addStretch();
    topA = new QWidget();
    topA->setLayout(leftLayout);

    // ------------------------------------------------------------------------
    //QHBoxLayout *mainLayout = new QHBoxLayout;
    //mainLayout->addLayout(leftLayout);
    //mainLayout->addSpacing(15);
    //mainLayout->addLayout(rightLayout);
//    this->setLayout(mainLayout);
    topB = new QWidget();
    topB->setLayout(rightLayout);

    addWidget(topA);
    addWidget(topB);

    readSettings();

    updateAbaqusCombo();
    updateCellCombos();
}
// ------------------------------------------------------------------------------------------------
MeshUniverseBuild::~MeshUniverseBuild()
{
    writeSettings();
}
// ------------------------------------------------------------------------------------------------
void MeshUniverseBuild::resetForProject()
{
    QTableWidgetItem *tItem;

    if (md->projectType == MCNP)
    {
        uName->setText("Universe Name");
        uNum->setText("Universe #");
        cStart->setText("Cell Start");
        fillCellLabel->setText("Fill Cell");
        backgroundCombo->setToolTip("Select background cell number.");
        fillCellCombo->setToolTip("Select a cell number for the fill (container) cell.");
        universeName->setToolTip("Enter short descriptive name for this mesh universe.");
        universeNumber->setToolTip("Enter an universe number for the embedded mesh.");
        cellStart->setToolTip("Eneter starting cell number of mesh pseudo-cells.");
        tItem = meshInstanceTable->horizontalHeaderItem(1);
        tItem->setText("Cell #");
        universeListTable->setToolTip("Table of mesh universes for the 3-D model.");
        topLabel->setText("Mesh Universe List");
        backgroundLabel->show();
        backgroundCombo->show();
    }
    else
    {
        uName->setText("Assembly Name");
        uNum->setText("Assembly #");
        cStart->setText("Zone Start");
        fillCellLabel->setText("Background Zone");
        backgroundCombo->setToolTip("Select background zone number.");
        fillCellCombo->setToolTip("Select a zone number for the background zone.");
        universeName->setToolTip("Enter short descriptive name for this mesh assembly.");
        universeNumber->setToolTip("Enter an assembly number for the embedded mesh.");
        cellStart->setToolTip("Eneter starting zone number of mesh pseudo-zones.");
        tItem = meshInstanceTable->horizontalHeaderItem(1);
        tItem->setText("Zone #");
        universeListTable->setToolTip("Table of mesh assemblies for the 3-D model.");
        topLabel->setText("Mesh Assembly List");
        backgroundLabel->hide();
        backgroundCombo->hide();
    }
}
// ------------------------------------------------------------------------------------------------
void MeshUniverseBuild::writeSettings()
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    settings.setValue("MeshUniverseBuild/size",       this->size());
    settings.setValue("MeshUniverseBuild/Properties", this->saveState());
    settings.setValue("MeshUniverseBuild/Geometry",   this->saveGeometry());
}
// ------------------------------------------------------------------------------------------------
void MeshUniverseBuild::readSettings()
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    this->restoreState(settings.value("MeshUniverseBuild/Properties").toByteArray());
    this->restoreGeometry(settings.value("MeshUniverseBuild/Geometry").toByteArray());
    resize( settings.value("MeshUniverseBuild/size", sizeHint()).toSize() );
}
// ------------------------------------------------------------------------------------------------
void MeshUniverseBuild::buildTopGridLayout()
{
    QLabel *meshFileLabel   = new QLabel("Mesh File");
    QLabel *scaleLabel      = new QLabel("Scale");

    scale = new QLineEdit;
    scale->setText("1.0");

    QLabel *translateLabel  = new QLabel("Translate");
    QLabel *xLabel          = new QLabel("X: ");
    QLabel *yLabel          = new QLabel("Y: ");
    QLabel *zLabel          = new QLabel("Z: ");

    QLabel *rotateLabel     = new QLabel("Rotate");
    QLabel *azimuthalLabel  = new QLabel("Azimuthal:");
    QLabel *polarLabel      = new QLabel("Polar:");

    transx = new QLineEdit;
    transy = new QLineEdit;
    transz = new QLineEdit;

    transx->setText("0.0");
    transy->setText("0.0");
    transz->setText("0.0");

    polarDSB      = new QDoubleSpinBox();
    azimuthalDSB  = new QDoubleSpinBox();

    polarDSB->setRange(0.0,  180.0);
    azimuthalDSB->setRange(-90.0,  90.0);

    polarDSB->setSingleStep(1.0);
    azimuthalDSB->setSingleStep(1.0);

    azimuthalDSB->setValue(0.0);
    polarDSB->setValue(0.0);

    azimuthalDSB->setMinimumWidth(70);
    polarDSB->setMinimumWidth(70);

    abaqusFileCombo = new QComboBox();
    abaqusFileCombo->blockSignals(true);
    abaqusFileCombo->setToolTip("Select mesh input file.");
    abaqusFileCombo->addItem("none");
    abaqusFileCombo->setMinimumWidth(180);
    abaqusFileCombo->setEnabled(false);

    topGridLayout = new QGridLayout;
    topGridLayout->addWidget(meshFileLabel,    0, 0, 1, 1, Qt::AlignHCenter);
    topGridLayout->addWidget(scaleLabel,       0, 4, 1, 1, Qt::AlignHCenter);
    topGridLayout->addWidget(translateLabel,   5, 0, 1, 1, Qt::AlignHCenter);
    topGridLayout->addWidget(xLabel,           6, 0, 1, 1, Qt::AlignRight);
    topGridLayout->addWidget(yLabel,           6, 2, 1, 1, Qt::AlignRight);
    topGridLayout->addWidget(zLabel,           6, 4, 1, 1, Qt::AlignRight);
    topGridLayout->addWidget(transx,           6, 1, 1, 1, Qt::AlignHCenter);
    topGridLayout->addWidget(transy,           6, 3, 1, 1, Qt::AlignHCenter);
    topGridLayout->addWidget(transz,           6, 5, 1, 1, Qt::AlignHCenter);

    topGridLayout->addWidget(rotateLabel,      7, 0, 1, 1, Qt::AlignHCenter);
    topGridLayout->addWidget(azimuthalLabel,   8, 0, 1, 1, Qt::AlignRight);
    topGridLayout->addWidget(polarLabel,       8, 2, 1, 1, Qt::AlignRight);
    topGridLayout->addWidget(azimuthalDSB,     8, 1, 1, 1, Qt::AlignHCenter);
    topGridLayout->addWidget(polarDSB,         8, 3, 1, 1, Qt::AlignHCenter);

    topGridLayout->addWidget(scale,            0, 5, 1, 1, Qt::AlignHCenter);
    topGridLayout->addWidget(abaqusFileCombo,  0, 1, 1, 2, Qt::AlignHCenter);

    topGridLayout->setColumnMinimumWidth(1, 100);
    topGridLayout->setColumnMinimumWidth(3, 100);
    topGridLayout->setColumnMinimumWidth(5, 100);
    topGridLayout->setColumnMinimumWidth(0,  70);
    topGridLayout->setColumnMinimumWidth(2,  70);
    topGridLayout->setColumnMinimumWidth(4,  70);

    connect(abaqusFileCombo, SIGNAL(currentIndexChanged(int)),
            this,            SLOT(handleAbaqusComboChanged(int)));

    abaqusFileCombo->blockSignals(false);
    updateAbaqusCombo();
}
// ------------------------------------------------------------------------------------------------
void MeshUniverseBuild::updateAbaqusCombo()
{
    QVector<QString> fileNames;
    md->getAbaqusNames(&fileNames);
    abaqusFileCombo->blockSignals(true);

    abaqusFileCombo->clear();
    abaqusFileCombo->addItem("none");
    for (int i=0; i < (int) fileNames.size(); ++i)
        abaqusFileCombo->addItem(fileNames[i]);

    abaqusFileCombo->blockSignals(false);
}

// ------------------------------------------------------------------------------------------------
void MeshUniverseBuild::buildMidGridLayout()
{
    uName  = new QLabel("Universe Name");
    uNum   = new QLabel("Universe #");
    cStart = new QLabel("Cell Start");
    backgroundLabel = new QLabel("Background Cell ");
    fillCellLabel   = new QLabel("Fill Cell");

    if (md->projectType == ITS)
    {
        uName->setText("Assembly Name");
        uNum->setText("Assembly #");
        cStart->setText("Zone Start");
        fillCellLabel->setText("Background Zone");
    }

    backgroundCombo = new QComboBox();
    backgroundCombo->setToolTip("Select background cell number.");
    backgroundCombo->addItem("none");
    backgroundCombo->setMinimumWidth(120);

    fillCellCombo = new QComboBox();
    fillCellCombo->setToolTip("Select a cell number for the fill (container) cell.");
    fillCellCombo->addItem("none");
    fillCellCombo->setMinimumWidth(120);

    universeName   = new QLineEdit;
    universeName->setToolTip("Enter short descriptive name for this mesh universe.");

    universeNumber = new QLineEdit;
    universeNumber->setText("0");
    universeNumber->setToolTip("Enter an universe number for the embedded mesh.");

    cellStart      = new QLineEdit;
    cellStart->setText("0");
    cellStart->setToolTip("Eneter starting cell number of mesh pseudo-cells.");
    connect(cellStart,  SIGNAL(textChanged(QString)),
            this,       SLOT(handleCellStartEdit(QString)));

    if (md->projectType == ITS)
    {
        cellStart->setToolTip("Eneter starting zone number of mesh pseudo-zones.");
        universeNumber->setToolTip("Enter an assembly number for the embedded mesh.");
        universeName->setToolTip("Enter short descriptive name for this mesh assembly.");
        fillCellCombo->setToolTip("Select a zone number for the background zone.");
    }

    universeName->setMinimumWidth(280);
    midGridLayout = new QGridLayout;
    midGridLayout->addWidget(uName,            0, 0, Qt::AlignHCenter);
    midGridLayout->addWidget(uNum,             1, 0, Qt::AlignHCenter);
    midGridLayout->addWidget(cStart,           1, 2, Qt::AlignHCenter);

    if (md->projectType == MCNP)
        midGridLayout->addWidget(backgroundLabel,  2, 0, Qt::AlignHCenter);
    midGridLayout->addWidget(fillCellLabel,    2, 2, Qt::AlignHCenter);

    if (md->projectType == MCNP)
        midGridLayout->addWidget(backgroundCombo,  2, 1, Qt::AlignHCenter);
    midGridLayout->addWidget(fillCellCombo,    2, 3, Qt::AlignHCenter);

    midGridLayout->addWidget(universeName,     0, 1, 1, 3, Qt::AlignLeft);
    midGridLayout->addWidget(universeNumber,   1, 1, 1, 1, Qt::AlignLeft);
    midGridLayout->addWidget(cellStart,        1, 3, 1, 1, Qt::AlignLeft);

    connect(fillCellCombo, SIGNAL(currentIndexChanged(int)),
            this,          SLOT(handleFillComboChanged(int)));

    connect(backgroundCombo, SIGNAL(currentIndexChanged(int)),
            this,            SLOT(handleBackgroundComboChanged(int)));
}

// ----------------------------------------------------------------------------
void MeshUniverseBuild::handleCellStartEdit(QString s)
{
    zoneStart = s.toInt();
    if (zoneStart > 0) abaqusFileCombo->setEnabled(true);
}

// ------------------------------------------------------------------------------------------------
void MeshUniverseBuild::updateCellCombos()
{
    QVector<QString> cellNumbers;
    md->getCellNumbers(cellNumbers);
    fillCellCombo->clear();
    fillCellCombo->addItem("none");
    backgroundCombo->clear();
    backgroundCombo->addItem("none");

    for (int i=0; i < (int) cellNumbers.size(); ++i)
    {
        fillCellCombo->addItem(cellNumbers[i]);
        backgroundCombo->addItem(cellNumbers[i]);
    }
}
// ------------------------------------------------------------------------------------------------
void MeshUniverseBuild::handleFillComboChanged(int index)
{
    if (index > 0  &&  index == backgroundCombo->currentIndex())
    {
        QString msg = "Fill cell cannot be the same as the background cell. ";
        msg += "Try another cell number.";
        displayMessage(msg);
        fillCellCombo->setCurrentIndex(0);
    }
}
// ------------------------------------------------------------------------------------------------
void MeshUniverseBuild::handleBackgroundComboChanged(int index)
{
    if (index > 0  &&  index == fillCellCombo->currentIndex())
    {
        QString msg = "Background cell cannot be the same as the fill cell. ";
        msg += "Try another cell number.";
        displayMessage(msg);
        backgroundCombo->setCurrentIndex(0);
    }
}
// ------------------------------------------------------------------------------------------------
void MeshUniverseBuild::displayMessage(QString &msg)
{
    QMessageBox about;
    about.setIcon(QMessageBox::Critical);
    about.setInformativeText(msg);
    about.setStandardButtons(QMessageBox::Ok);
    about.setDefaultButton(QMessageBox::Ok);
    about.show();
    about.exec();
}
// ------------------------------------------------------------------------------------------------
void MeshUniverseBuild::buildMeshInstanceTable()
{
    meshInstanceTable = new QTableWidget(0, 6);
#ifdef USING_WINDOWS
    meshInstanceTable->setColumnWidth(0, 200);
    meshInstanceTable->setColumnWidth(1,  75);
    meshInstanceTable->setColumnWidth(2, 110);
    meshInstanceTable->setColumnWidth(3,  80);
    meshInstanceTable->setColumnWidth(4, 100);
    meshInstanceTable->setColumnWidth(5,  80);
#else
    meshInstanceTable->setColumnWidth(0, 200);
    meshInstanceTable->setColumnWidth(1,  70);
    meshInstanceTable->setColumnWidth(2,  80);
    meshInstanceTable->setColumnWidth(3,  80);
    meshInstanceTable->setColumnWidth(4,  80);
    meshInstanceTable->setColumnWidth(5,  80);
#endif

    QString lab1;
    if (md->projectType == MCNP)
        lab1 = "Cell #";
    else
        lab1 = "Zone #";

    meshInstanceTable->setHorizontalHeaderLabels(
      QStringList() << tr("Part/Instance") << lab1 << tr("Material #")
                << tr("Density")
                << tr("Color")   << tr("Imp"));

    connect(meshInstanceTable, SIGNAL(itemChanged(QTableWidgetItem *)),
            this,              SLOT(meshInstanceTableChanged(QTableWidgetItem *)));
}
// ------------------------------------------------------------------------------------------------
void MeshUniverseBuild::handleAbaqusComboChanged(int index)
{
    QString name;
    QString cellNo;
    QString mtlNo;
    QString one = "1.0";

//    printf("index: %d\n", index);
    if (index > 0)
    {
        instanceColors.clear();
        instanceCellNum.clear();
        instanceMtlNum.clear();

        clearMeshInstanceTable();
        currentAbaqusFileIndex = index;
        // De-reference vector pointers.                [*SKILL*]
        ReadFileAbaqus *rfa = &(*abaqusFiles)[index-1];

        QString s1 = cellStart->text();
        int cellAdder = s1.toInt();

        meshInstanceTable->blockSignals(true);

        for(int n=0; n < rfa->nInstances; ++n)
        {
            printf("instance name: %s\n", rfa->instances[n].name.toLatin1().data());
            name = rfa->instances[n].name;
            cellNo.setNum(n+cellAdder+1);
            mtlNo.setNum( rfa->instances[n].materialNo+1 );
            addMeshInstanceTableRow(n, name, cellNo, mtlNo, one, one, rfa->instances[n].color);
        }

        meshInstanceTable->blockSignals(false);
    }
    else
        clearMeshInstanceTable();
}
// ------------------------------------------------------------------------------------------------
void MeshUniverseBuild::clearMeshInstanceTable()
{
    meshInstanceTable->blockSignals(true);

    for(size_t i=0; i<instanceColorConnection.size(); ++i)
    {
        disconnect(instanceColorConnection[i]);
    }
    instanceColorConnection.clear();

 //   meshInstanceTable->clearContents();

    int nRows = meshInstanceTable->rowCount();
    for(int i=nRows - 1; i > -1; --i)
        meshInstanceTable->removeRow(i);

    meshInstanceTable->blockSignals(false);
}
// ------------------------------------------------------------------------------------------------
void MeshUniverseBuild::addMeshInstanceTableRow(int row, QString name, QString cellNo,
                                                QString mtlNo, QString density, QString imp, QColor cellColor)
{
    instanceColors.push_back(cellColor);
    instanceCellNum.push_back(cellNo);
    instanceMtlNum.push_back(mtlNo);

    meshInstanceTable->insertRow(row);
    QColor color = QColor( 255, 0, 0, 255);
//    QBrush brush = QBrush (color, Qt::SolidPattern );

    QTableWidgetItem *item0 = new QTableWidgetItem();
    item0->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
//    item0->setForeground(color);
    item0->setText(name);
    meshInstanceTable->setItem(row, 0, item0);

    QTableWidgetItem *item1 = new QTableWidgetItem();
    item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
//    item1->setForeground(color);
    item1->setText(cellNo);
    meshInstanceTable->setItem(row, 1, item1);

    QTableWidgetItem *item2 = new QTableWidgetItem();
    item2->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item2->setForeground(color);
    item2->setText(mtlNo);
    meshInstanceTable->setItem(row, 2, item2);

    QTableWidgetItem *item3 = new QTableWidgetItem();
    item3->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item3->setForeground(color);
    item3->setText(density);
    meshInstanceTable->setItem(row, 3, item3);

    QTableWidgetItem *item4 = new QTableWidgetItem();
    item4->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    item4->setForeground(color);
    item4->setText(imp);
    meshInstanceTable->setItem(row, 5, item4);

    // Make and add the color button.
    SSS_PushButton *but = new SSS_PushButton();
    but->setAutoFillBackground(true);

    but->setPalette(cellColor);
    but->setVal1(row);
    but->setVal2(0);
    but->setButton(but);
    meshInstanceTable->setCellWidget(row, 4, but);
    but->setToolTip("Set the pseudo-cell/zone color.");

    QMetaObject::Connection con;

    con = connect(but,  SIGNAL(pressed(int,int,SSS_PushButton*)),
                  this, SLOT(handleColorButton(int,int,SSS_PushButton*)));

    instanceColorConnection.push_back(con);

}
// ------------------------------------------------------------------------------------------------
void MeshUniverseBuild::handleColorButton(int row, int col, SSS_PushButton *but)
{
    QString s1("Pseudo-Cell Chooser");
    if (md->projectType != MCNP) s1 = "Pseudo-Zone Chooser";

    QPalette p = but->palette();
    QColor   c = p.color(QPalette::Window);

    QColor color = QColorDialog::getColor(c, this, s1, QColorDialog::ShowAlphaChannel);
    if (color.isValid())
    {
            but->setPalette(color);
            instanceColors[row] = color;
    }
}

// ------------------------------------------------------------------------------------------------
void MeshUniverseBuild::meshInstanceTableChanged(QTableWidgetItem *item)
{
    // Block signals so that "somethingChanged" routine is not called.
    meshInstanceTable->blockSignals(true);

//    qDebug() << "item col: " << item->column() << "\t row: " << item->row() << "\t " << item->text();
    int col = item->column();
    if (col == 1)      instanceCellNum[item->row()] = item->text();
    else if (col == 2) instanceMtlNum[item->row()]  = item->text();

    meshInstanceTable->blockSignals(false);
}
// ------------------------------------------------------------------------------------------------
void MeshUniverseBuild::buildRightSideLayout()
{
    topLabel = new QLabel("Mesh Universe List");
    if (md->projectType == ITS)
        topLabel->setText("Mesh Assembly List");
    topLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    // ------------------------------------------------------------------------
    universeListTable = new QTableWidget(0, 2);
    universeListTable->setHorizontalHeaderLabels(QStringList() << tr("No.") << tr("Name"));

#ifdef USING_WINDOWS
    universeListTable->setColumnWidth(0,  60);
    universeListTable->setColumnWidth(1, 170);
    universeListTable->setMinimumWidth(240);
#else
    universeListTable->setColumnWidth(0,  50);
    universeListTable->setColumnWidth(1, 150);
    universeListTable->setMinimumWidth(220);
#endif

    universeListTable->setMinimumHeight(300);
    universeListTable->setToolTip("Table of mesh universes for the 3-D model.");

    universeListTable->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(universeListTable, SIGNAL(customContextMenuRequested(const QPoint &)),
            this,              SLOT(onCustomContextMenu(const QPoint &)));

    // ------------------------------------------------------------------------
    acceptPB = new QPushButton("ACCEPT");
    acceptPB->setToolTip("Accept the current mesh universe and add to the table.");
    connect(acceptPB, SIGNAL(pressed()), this, SLOT(handleAcceptPB()));
    QHBoxLayout *acceptLayout = new QHBoxLayout;
    acceptLayout->addStretch();
    acceptLayout->addWidget(acceptPB);

    rightLayout = new QVBoxLayout;
    rightLayout->addWidget(topLabel);
    rightLayout->addSpacing(5);
    rightLayout->addWidget(universeListTable);
    rightLayout->addStretch();
    rightLayout->addLayout(acceptLayout);

    updateUniverseListTable();
}
// ------------------------------------------------------------------------------------------------
void MeshUniverseBuild::updateUniverseListTable()
{
    QStringList sl;
    for (int i=0; i < (int) md->universeDataStringList.size(); ++i)
    {
        sl = md->universeDataStringList[i];
        addUniverseListRow(sl[0], sl[4]);
    }
}

// ------------------------------------------------------------------------------------------------
void MeshUniverseBuild::resetWidgets()
{
    // Remove rows from the meshInstanceTable first.
//    int nRows = meshInstanceTable->rowCount() - 1;
//    for(int n=nRows; n > -1; --n)
//    {
//        meshInstanceTable->removeRow(n);
//    }

    clearMeshInstanceTable();

    abaqusFileCombo->setCurrentIndex(0);
    backgroundCombo->setCurrentIndex(0);
    fillCellCombo->setCurrentIndex(0);

    scale->setText("");
    transx->setText("");
    transy->setText("");
    transz->setText("");
    universeName->setText("");
    universeNumber->setText("");
    cellStart->setText("");
    abaqusFileCombo->setEnabled(false);

    azimuthalDSB->setValue(0.0);
    polarDSB->setValue(0.0);
}
// ------------------------------------------------------------------------------------------------
void MeshUniverseBuild::populateWidgets(int row)
{
    QString universe;
    clearMeshInstanceTable();
    QStringList sl = md->universeDataStringList[row];

    scale->setText(sl[5]);
    transx->setText(sl[6]);
    transy->setText(sl[7]);
    transz->setText(sl[8]);
    universeName->setText(sl[4]);
    universeNumber->setText(sl[0]);
    universe = sl[0];
    cellStart->setText(sl[1]);

    double angle = sl[9].toDouble();
    azimuthalDSB->setValue(angle);
    angle = sl[10].toDouble();
    polarDSB->setValue(angle);

    QStringList fields;
    QString line;
    int ct;

    ct = backgroundCombo->count();
    int c;
    for(c=0; c < ct; ++c)
    {
        line = backgroundCombo->itemText(c);
        fields = line.split('/');
        if (sl[2].trimmed() == fields[0].trimmed()) break;
    }
    backgroundCombo->setCurrentIndex(c);

    ct = fillCellCombo->count();
    for(c=0; c < ct; ++c)
    {
        line = fillCellCombo->itemText(c);
        fields = line.split('/');
        if (sl[3].trimmed() == fields[0].trimmed()) break;
    }
    fillCellCombo->setCurrentIndex(c);

    ct = abaqusFileCombo->count();
    for(c=0; c < ct; ++c)
    {
        line = abaqusFileCombo->itemText(c);
        if (sl[11].trimmed() == line.trimmed()) break;
    }
    abaqusFileCombo->blockSignals(true);
    abaqusFileCombo->setCurrentIndex(c);
    abaqusFileCombo->blockSignals(false);

    // Now, re-populate the instance table.
    QColor color;
    ct = 0;
    for(c = 0; c < (int) md->instanceDataStringList.size(); ++c)
    {
        sl = md->instanceDataStringList[c];
        if (universe.trimmed() == sl[0].trimmed())
        {
            color.setRed(sl[6].toInt());
            color.setGreen(sl[7].toInt());
            color.setBlue(sl[8].toInt());
            color.setAlpha(sl[9].toInt());

            addMeshInstanceTableRow(ct, sl[5], sl[1], sl[2], sl[3], sl[4], color);
            ++ct;
        }
    }
}               // -- end populateWidgets

// ------------------------------------------------------------------------------------------------
void MeshUniverseBuild::handleAcceptPB()
{
    QString s1;
    if (md->projectType == MCNP) s1 = "cell";
    else s1 = "zone";

    if (0 == fillCellCombo->currentIndex())
    {
        QString msg;
        if (md->projectType == MCNP)
            msg = "Fill cell was not set.\n\nPlease select a valid cell.";
        else
            msg = "Background zone was not set.\n\nPlease select a valid zone.";

        displayMessage(msg);
        return;
    }
    if (0 == backgroundCombo->currentIndex()  &&  (md->projectType == MCNP))
    {
        QString msg = "Background " + s1 + " was not set.\n\n";
        msg += "Please select a valid " + s1 + " number.";
        displayMessage(msg);
        return;
    }
    if (zoneStart < 1)
    {
        QString msg = "Starting " + s1 + " must be greater than 0.";
        displayMessage(msg);
        return;
    }
    QString uNum  = universeNumber->text();
    int num = uNum.toInt();
    if (num < 1)
    {
        QString msg = "Universe number must be greater than 0.";
        displayMessage(msg);
        return;
    }
    if (0 == abaqusFileCombo->currentIndex())
    {
        QString msg = "No Abaqus file selecred.\n\n";
        msg += "Please select a valid mesh file.";
        displayMessage(msg);
        return;
    }

    QString uuName = universeName->text();

    if ( addUniverseListRow(uNum, uuName) )
    {
        saveUniverseData();
        saveInstanceData();
        emit updateMeshUniverse();
    }
    resetWidgets();
}
// ------------------------------------------------------------------------------------------------
void MeshUniverseBuild::saveUniverseData()
{
    QStringList fields;

//    qDebug() << "universeName: " << universeName->text();
//    qDebug() << "universeNumber: " << universeNumber->text();

    QString line = fillCellCombo->currentText();
    fields = line.split('/');
//    qDebug() << "fillCellCombo: " <<  fillCellCombo->currentText() << "\t" << fields[0].trimmed();

    line = backgroundCombo->currentText();
    fields = line.split('/');
//    qDebug() << "backgroundCombo: " <<  backgroundCombo->currentText() << "\t" << fields[0].trimmed();

//    qDebug() << "abaqusFileCombo: " <<  abaqusFileCombo->currentText() ;

//    qDebug() << "cellStart: " <<  cellStart->text();

//    qDebug() << "scale: " <<  scale->text();
//    qDebug() << "transx: " <<  transx->text();
//    qDebug() << "transy: " <<  transy->text();
//    qDebug() << "transz: " <<  transz->text();

//    qDebug() << "azimuthalDSB: " <<  azimuthalDSB->text();
//    qDebug() << "polarDSB: " <<  polarDSB->text();

    QStringList sl;
    sl.push_back(universeNumber->text());
    sl.push_back(cellStart->text());
    line = backgroundCombo->currentText();
    fields = line.split('/');
    sl.push_back(fields[0].trimmed());
    line = fillCellCombo->currentText();
    fields = line.split('/');
    sl.push_back(fields[0].trimmed());
    sl.push_back(universeName->text());

    sl.push_back(scale->text());
    sl.push_back(transx->text());
    sl.push_back(transy->text());
    sl.push_back(transz->text());
    sl.push_back(azimuthalDSB->text());
    sl.push_back(polarDSB->text());
    sl.push_back(abaqusFileCombo->currentText());

    // Insert in order by universe number.
    int i, n1, n2;
    QStringList l;
    int klim = (int) md->universeDataStringList.size();

    if (klim > 0)
    {
        n1 = sl[0].toInt();
        for(i=0; i<klim; ++i)
        {
            l = md->universeDataStringList[i];
            n2 = l[0].toInt();

            if (n2 > n1) break;
        }

        md->universeDataStringList.insert(i, sl);

    }
    else
        md->universeDataStringList.push_back(sl);
}

// ------------------------------------------------------------------------------------------------
void MeshUniverseBuild::saveInstanceData()
{
    QTableWidgetItem *item0;

    QString s0;
    QColor color;
    QString red, green, blue, alpha;
    QString one = "1";
    QString density;
    QString imp;

    QString s = universeNumber->text();
    md->removeInstanceFromList(s);

    int nRows = meshInstanceTable->rowCount();
    for(int n=0; n < nRows; ++n)
    {
        item0 = meshInstanceTable->item(n, 0);
        s0 = item0->text();

        item0 = meshInstanceTable->item(n, 3);
        density = item0->text();

        item0 = meshInstanceTable->item(n, 5);
        imp = item0->text();


//       qDebug() << "row: " << n << "\ts0: " << s0 << "\ts1: " << instanceCellNum[n]
//                    << "\ts2: " << instanceMtlNum[n] <<
//                    "\tcolor: " << instanceColors[n];

        QStringList sl;
        sl.push_back(universeNumber->text());
        sl.push_back(instanceCellNum[n]);
        sl.push_back(instanceMtlNum[n]);
        sl.push_back(density);
        sl.push_back(imp);
        sl.push_back(s0);
        color = instanceColors[n];
        red.setNum( color.red() );
        green.setNum( color.green() );
        blue.setNum( color.blue() );
        alpha.setNum( color.alpha() );
        sl.push_back(red);
        sl.push_back(green);
        sl.push_back(blue);
        sl.push_back(alpha);
        sl.push_back(one);          // Default display indicator

        // instanceDataStringList has the data ordered in blocks by the universe number.
        md->instanceDataStringList.push_back(sl);
    }
}

// ------------------------------------------------------------------------------------------------
bool MeshUniverseBuild::addUniverseListRow(QString uNum, QString uuName)
{
    bool flag = true;

    QTableWidgetItem *itemx;
    QString test;
    int row = universeListTable->rowCount();
    for(int i=0; i<row; ++i)
    {
        itemx = universeListTable->itemAt(i, 0);
        test = itemx->text();
        if (test.trimmed() == uNum.trimmed())
        {
            flag = false;
            break;
        }
    }
    if (flag)
    {
        universeListTable->insertRow(row);

        QTableWidgetItem *item0 = new QTableWidgetItem;
        item0->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        item0->setText(uNum);
        universeListTable->setItem(row, 0, item0);

        QTableWidgetItem *item1 = new QTableWidgetItem;
        item1->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        item1->setText(uuName);
        universeListTable->setItem(row, 1, item1);

        universeListTable->sortByColumn(0, Qt::AscendingOrder);
    }
    else
    {
        test = "Universe " + uNum + " already exists.  Try another.";
        displayMessage(test);
    }
    return flag;
}

// ----------------------------------------------------------------------------
void MeshUniverseBuild::onCustomContextMenu(const QPoint &point)
{
    //qDebug() << "** onCustomContextMenu **";
    QModelIndex index = universeListTable->indexAt(point);
    if (index.isValid() )
    {
        //qDebug() << "index row: " << index.row() <<   "\n";
        QMenu contextMenu;
        QAction *rowDeleteAction  = contextMenu.addAction(tr("DELETE"));
        QAction *rowEditAction  = contextMenu.addAction(tr("EDIT"));
        QAction *rowShowAction  = contextMenu.addAction(tr("SHOW"));

        QAction *action = contextMenu.exec(universeListTable->viewport()->mapToGlobal(point));

        if (action == rowDeleteAction  ||  action == rowEditAction)
        {
            int row = index.row();
            int i;
            QTableWidgetItem *item = universeListTable->item(row, 0);
            QString s = item->text();
            for(i=0; i < (int) md->universeDataStringList.size(); ++i)
            {
                QStringList sl = md->universeDataStringList[i];
                if (s == sl[0]) break;
            }

            if (action == rowEditAction) populateWidgets(i);

            md->removeInstanceFromList(s);
            md->universeDataStringList.remove(i);
            universeListTable->removeRow(row);
//            qDebug() << "universeListTable size " << universeListTable->size();
        }
        else if (action == rowShowAction)
        {
            int row = index.row();
            int i;
            QTableWidgetItem *item = universeListTable->item(row, 0);
            QString s = item->text();
            for(i=0; i < (int) md->universeDataStringList.size(); ++i)
            {
                QStringList sl = md->universeDataStringList[i];
                if (s == sl[0]) break;
            }
            emit showMeshUniverse(i);
        }
    }
}
