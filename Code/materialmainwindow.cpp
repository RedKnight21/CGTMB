#include "materialmainwindow.h"

MaterialMainWindow::MaterialMainWindow(MasterData *mdIn, QWidget *parent) : QMainWindow(parent)
{
    md = mdIn;
    mtlNumMap.clear();
    customZaidList = nullptr;

    readElementData();
    readXgenData();

    messageWidget = new MaterialMessageWidget(this, true, false, true, false);
    messageWidget->setMinimumHeight(220);
    makeControlWidget();
    if (md->projectType == MCNP) buildZaidListWidget();
    if (md->projectType == ITS)  buildXgenListWidget();
    buildMaterialsListWidget();

    mainSplitter    = new QSplitter(Qt::Horizontal);
    centralSplitter = new QSplitter(Qt::Vertical);
    centralSplitter->setMinimumWidth(400);

    createDockWidget();

    restoreDockWidget(dock2);
    if (md->projectType == ITS)
        restoreDockWidget(dockXgen);
    else
        restoreDockWidget(dock1);


    //if (md->projectType == MCNP)
    centralSplitter->addWidget(messageWidget);
    centralSplitter->addWidget(materialControls);

    mainSplitter->addWidget(centralSplitter);
    setCentralWidget(mainSplitter);

    createActions();
    createMenus();
    setupSlots();

    statusBar()->showMessage(tr("Status Bar"));
    createStatusBar();

    addExistingMaterials();

    if (md->XSDIRfile.length() > 0) readXsdirFile(md->XSDIRfile);

    if (md->projectType == ITS)
    {
        QString s;
        s = "Using XGEN default data";
        statusFileNameLabel->setText(s);
        s = "Read XGEN default data from file.\n";
        messageWidget->displayTextString(s);
    }

    readSettings();
    setWindowTitle("Material Builder Window");
    if (md->projectType == MCNP) dock1->activateWindow();
    if (md->projectType == ITS)  dockXgen->activateWindow();
    dock2->activateWindow();
}
// ----------------------------------------------------------------------------
MaterialMainWindow::~MaterialMainWindow()
{

}
// ----------------------------------------------------------------------------
QSize MaterialMainWindow::minimumSizeHint() const
{
    return QSize(50, 50);
}
// ----------------------------------------------------------------------------
QSize MaterialMainWindow::sizeHint() const
{
#ifdef USING_WINDOWS
    return QSize(1200, 800);
#else
    return QSize(1000, 800);
#endif
}
// ----------------------------------------------------------------------------
void MaterialMainWindow::writeSettings()
{
    //qDebug() << "MaterialMainWindow:: size: " << this->size() << "\n";

    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    settings.setValue("materialMainWindow/size",       this->size());
    settings.setValue("materialMainWindow/Properties", this->saveState());
    settings.setValue("materialMainWindow/Geometry",   this->saveGeometry());
    settings.setValue("materialMainWindow/mainSplitter-geom",     mainSplitter->saveGeometry());
    settings.setValue("materialMainWindow/mainSplitter-state",    mainSplitter->saveState());
    settings.setValue("materialMainWindow/centralSplitter-geom",  centralSplitter->saveGeometry());
    settings.setValue("materialMainWindow/centralSplitter-state", centralSplitter->saveState());
}
// ----------------------------------------------------------------------------
void MaterialMainWindow::readSettings()
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    this->restoreGeometry(settings.value("materialMainWindow/Geometry").toByteArray());
    this->restoreState(settings.value("materialMainWindow/Properties").toByteArray());
 //   resize( settings.value("materialMainWindow/size", sizeHint()).toSize() );

    // Not sure why can't get everything restored to exact dimensions.
//    centralSplitter->restoreGeometry(settings.value("materialMainWindow/centralSplitter-geom").toByteArray());
//    mainSplitter->restoreGeometry(settings.value("materialMainWindow/mainSplitter-geom").toByteArray());
//    centralSplitter->restoreState(settings.value("materialMainWindow/centralSplitter-state").toByteArray());
//    mainSplitter->restoreState(settings.value("materialMainWindow/mainSplitter-state").toByteArray());

 //   qDebug() << "MaterialMainWindow:: readSettings:: size: " <<
 //               settings.value("MaterialMainWindow/size", settings.value("materialMainWindow/size").toSize());
}
// ----------------------------------------------------------------------------
void MaterialMainWindow::createDockWidget()
{
    if (md->projectType == MCNP)
    {
        dock1 = new QDockWidget(tr("ZAID List"), this);
        dock1->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
        dock1->setWidget(zaidListWidget);
        dock1->setObjectName(tr("ZAID List"));
        dock1->setVisible(true);
        addDockWidget(Qt::LeftDockWidgetArea, dock1);
        zaidAction = dock1->toggleViewAction();
    }

    if (md->projectType == ITS)
    {
        dockXgen = new QDockWidget(tr("XGEN Element List"), this);
        dockXgen->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
        dockXgen->setWidget(xgenListWidget);
        dockXgen->setObjectName(tr("XGEN Element List"));
        dockXgen->setVisible(true);
        addDockWidget(Qt::LeftDockWidgetArea, dockXgen);
        xgenAction = dockXgen->toggleViewAction();
    }

    dock2 = new QDockWidget(tr("Materials List"), this);
    dock2->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock2->setWidget(materialsListWidget);
    dock2->setObjectName(tr("Materials List"));
    dock2->setVisible(true);
    addDockWidget(Qt::RightDockWidgetArea, dock2);
    materialsAction = dock2->toggleViewAction();
}
// ----------------------------------------------------------------------------
void MaterialMainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(zaidAction);
    menu.addAction(materialsAction);
    menu.exec(event->globalPos());
}
// ----------------------------------------------------------------------------
void MaterialMainWindow::makeControlWidget()
{
    QLabel *matName = new QLabel("Name: ");
    materialName = new QLineEdit;
    materialName->setToolTip("Enter short descriptive name for the current material.");

    QLabel *matNumber = new QLabel("Number: ");
    materialNumber = new QLineEdit;
    materialNumber->setMaxLength(80);
    materialNumber->setToolTip("Enter a valid material number for the current material.");

    QLabel *matColor = new QLabel("Color: ");
    materialColor = new QPushButton("COLOR");
    materialColor->setAutoFillBackground(true);
    materialColor->setPalette( QColor( "White" ) );
    materialColor->setToolTip("Choose a material color.");

    QLabel *matDensity = new QLabel("Density: ");
    materialDensity = new QLineEdit;
    materialDensity->setToolTip("Enter a density for the entire material.");

    materialClear = new QPushButton("CLEAR");
    materialAdd   = new QPushButton("ADD");
    materialAdd->setToolTip("Add this current material to the master material list.");
    materialClear->setToolTip("Clear the Current Material widgets including Nuclide List.");

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(matName,         0, 0);
    gridLayout->addWidget(materialName,    0, 1, 1, 3);
    gridLayout->addWidget(matNumber,       1, 0);
    gridLayout->addWidget(materialNumber,  1, 1, 1, 1);
//    gridLayout->addWidget(matColor,        1, 2);
    gridLayout->addWidget(materialColor,   1, 3, 1, 1);

    gridLayout->addWidget(matDensity,      2, 0);
    gridLayout->addWidget(materialDensity, 2, 1, 1, 1);

    gridLayout->addWidget(materialClear,   3, 0);
    gridLayout->addWidget(materialAdd,     3, 3);

    gridLayout->setHorizontalSpacing(30);

    QLabel *controlTitle = new QLabel("Current Material");
    controlTitle->setAlignment(Qt::AlignCenter);
    adjustQLabel(controlTitle, +1, true);

    QLabel *nuclideListTitle = new QLabel("Current Nuclide List");
    nuclideListTitle->setAlignment(Qt::AlignCenter);
    adjustQLabel(nuclideListTitle, +1, true);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(controlTitle);
    mainLayout->addLayout(gridLayout);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(nuclideListTitle);

    buildWorkingListWidget();
    mainLayout->addWidget(workingListWidget);
    buildCloseLayout();
    mainLayout->addLayout(closeLayout);

    materialControls = new QWidget;
    materialControls->setLayout(mainLayout);
    materialControls->setMaximumWidth(400);
    materialControls->setToolTip("Input for current material.");
}
// ----------------------------------------------------------------------------
void MaterialMainWindow::setupSlots()
{
    connect(materialAdd,   SIGNAL(pressed()), this, SLOT(handleMtlAdd()));
    connect(materialClear, SIGNAL(pressed()), this, SLOT(handleMtlClear()));
    connect(materialColor, SIGNAL(pressed()), this, SLOT(handleColorButton()));

    connect(materialDensity, SIGNAL(textChanged(QString)), this, SLOT(handleDensityEdit(QString)));
    connect(materialName,    SIGNAL(textChanged(QString)), this, SLOT(handleNameEdit(QString)));
//    connect(materialNumber,  SIGNAL(editingFinished()),    this, SLOT(handleMtlNoFinish()));
    connect(materialNumber, SIGNAL(textChanged(QString)), this, SLOT(handleMtlNoEdit(QString)));

}
// ----------------------------------------------------------------------------
void MaterialMainWindow::createActions()
{
    xsdirFile = new QAction(tr("&Select XSDIR ..."), this);
    xsdirFile->setStatusTip(tr("Select an XSDIR file to use"));
    connect(xsdirFile, SIGNAL(triggered()), this, SLOT(handleOpenXsdir()));
}
// ----------------------------------------------------------------------------
void MaterialMainWindow::createMenus()
{
    menuBar = new QMenuBar;
    fileMenu  = menuBar->addMenu(tr("&File  "));
    fileMenu->addAction(xsdirFile);
    setMenuBar(menuBar);
}
// ------------------------------------------------------------------------------------------------
void MaterialMainWindow::createStatusBar()
{
    QPalette pal2;
    pal2.setColor( QPalette::Foreground, QColor(204,0,50) );
    statusFileName = "no XSDIR file loaded ";
    statusFileNameLabel = new QLabel(statusFileName);
    statusFileNameLabel->setPalette(pal2);
    statusBar()->addPermanentWidget(statusFileNameLabel);
    statusBar()->showMessage(tr("Ready"));
}
// ------------------------------------------------------------------------------------------------
void MaterialMainWindow::handleOpenXsdir()
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    xsdirPath = settings.value("currentPath").toString();
    QString format = ".*";
    QString initialPath;

    if (xsdirPath.isEmpty())
    {
        initialPath = QDir::currentPath() + tr("/untitled") + format;
    }
    else
    {
        initialPath = xsdirPath;
    }

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open XSDIR Data Library"), initialPath,
                                     tr("xsdir*.*;; *.*"));
    readXsdirFile(fileName);
    md->XSDIRfile = fileName;
}
// ------------------------------------------------------------------------------------------------
void MaterialMainWindow::readXsdirFile(QString fileName)
{
    if (!fileName.isEmpty())
    {
        QString     line;
        QStringList fields;
        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly))
        {
            QString fileError = file.errorString();
            QMessageBox about;
            about.setIcon(QMessageBox::Critical);
            about.setInformativeText(fileError);
            about.setStandardButtons(QMessageBox::Ok);
            about.setDefaultButton(QMessageBox::Ok);
            about.show();
            about.exec();
            return;
        }
        QTextStream in(&file);
        while(!in.atEnd())
        {
            line = in.readLine();
            if (line.startsWith("directory", Qt::CaseInsensitive))
            {
                break;
            }
        }               // -- end while to skip first part

        cNeutron.clear();
        dNeutron.clear();
        photoAtomic.clear();
        photoNuclear.clear();
        dosimetry.clear();
        electron.clear();
        proton.clear();

        // Find zaids and store in appropriate vectors.
        while(!in.atEnd())
        {
            line = in.readLine();
            if (line.startsWith("       ")) continue;
            if (line.startsWith(' ')) line.remove(0, 1);
            fields = line.split(' ');

            if (fields[0].endsWith('c')) cNeutron.push_back(fields[0]);
            if (fields[0].endsWith('d')) dNeutron.push_back(fields[0]);
            if (fields[0].endsWith('p')) photoAtomic.push_back(fields[0]);
            if (fields[0].endsWith('m')) multiGroup.push_back(fields[0]);
            if (fields[0].endsWith('u')) photoNuclear.push_back(fields[0]);
            if (fields[0].endsWith('y')) dosimetry.push_back(fields[0]);
            if (fields[0].endsWith('e')) electron.push_back(fields[0]);
            if (fields[0].endsWith('h')) proton.push_back(fields[0]);
        }

        QString num;
        QString msg = "XSDIR file contains: \n  ";
        num.sprintf("%4d", cNeutron.size());
        msg += num;
        msg += "  continuous energy neutron ZAIDs\n  ";

        num.sprintf("%4d", dNeutron.size());
        msg += num;
        msg += "  discrete energy neutron ZAIDs\n  ";

        num.sprintf("%4d", photoAtomic.size());
        msg += num;
        msg += "  photoatomic ZAIDs\n  ";

        num.sprintf("%4d", multiGroup.size());
        msg += num;
        msg += "  multigroup ZAIDs\n  ";

        num.sprintf("%4d", photoNuclear.size());
        msg += num;
        msg += "  photonuclear ZAIDs\n  ";

        num.sprintf("%4d", dosimetry.size());
        msg += num;
        msg += "  dosimetry ZAIDs\n  ";

        num.sprintf("%4d", electron.size());
        msg += num;
        msg += "  electron ZAIDs\n  ";

        num.sprintf("%4d", proton.size());
        msg += num;
        msg += "  proton ZAIDs\n  ";

        messageWidget->displayTextString(msg);
        zaidType->setEnabled(true);

    }                   // -- end if valid file name

    statusFileNameLabel->setText(fileName);
}
// ------------------------------------------------------------------------------------------------
void MaterialMainWindow::readElementData()
{
    QString     line;
    QStringList fields;
    QFile file(":/Resources/elements.txt");

    if (!file.open(QIODevice::ReadOnly))
    {
        QString fileError = file.errorString();
        fileError += "\nFile: \n";
        fileError += file.fileName();
        QMessageBox about;
        about.setIcon(QMessageBox::Critical);
        about.setInformativeText(fileError);
        about.setStandardButtons(QMessageBox::Ok);
        about.setDefaultButton(QMessageBox::Ok);
        about.show();
        about.exec();
        return;
    }
    elementNames.clear();
    elementSymbols.clear();
    elementNums.clear();
    elementNames.push_back("none");
    elementSymbols.push_back("none");

    QTextStream in(&file);
    int n = 0;
    while(!in.atEnd())
    {
        line   = in.readLine();
        fields = line.split(' ');
        elementSymbols.push_back(fields[0]);
        elementNames.push_back(fields[1]);
        ++n;
        elementNums.push_back(n);
    }
}
// ------------------------------------------------------------------------------------------------
void MaterialMainWindow::buildZaidListWidget()
{
    zaidModel = new QStandardItemModel;

    QStringList hHeaderList;
    hHeaderList << QStringLiteral("Z") << QStringLiteral("Element") << QStringLiteral("Zaid");
    zaidModel->setHorizontalHeaderLabels(hHeaderList);

//    connect(zaidModel, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(bodyModelItemChanged(QStandardItem *)));

    zaidTreeView = new QTreeView;
    zaidTreeView->setModel(zaidModel);
    zaidTreeView->expandAll();

#ifdef USING_WINDOWS
    zaidTreeView->setColumnWidth(0,  80);
    zaidTreeView->setColumnWidth(1, 100);
    zaidTreeView->setColumnWidth(2, 120);
    zaidTreeView->setMinimumSize(230, 500);
#else
    zaidTreeView->setColumnWidth(0, 70);
    zaidTreeView->setColumnWidth(1, 80);
    zaidTreeView->setColumnWidth(2, 70);
    zaidTreeView->setMinimumSize(220, 500);
#endif

    zaidTreeView->sortByColumn(0, Qt::AscendingOrder);
    zaidTreeView->setSortingEnabled(true);
    zaidTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(zaidTreeView, SIGNAL(customContextMenuRequested(const QPoint &)),
            this,         SLOT(onCustomContextMenuZaid(const QPoint &)));


    zaidType = new QComboBox();
    zaidType->setToolTip("Select ZAID type.");
    zaidType->addItem("none");
    zaidType->addItem("(c) Continuous neutron");
    zaidType->addItem("(d) Discrete neutron");
    zaidType->addItem("(p) Photoatomic");
    zaidType->addItem("(m) Multigroup");
    zaidType->addItem("(u) Photonuclear");
    zaidType->addItem("(y) Dosimetry");
    zaidType->addItem("(e) Electron");
    zaidType->addItem("(h) Proton");
    zaidType->addItem("Custom");
    zaidType->setEnabled(false);

    connect(zaidType, SIGNAL(currentIndexChanged(int)),
            this,     SLOT(handleZaidTypeChanged(int)));

    QVBoxLayout *topLayout = new QVBoxLayout;
    topLayout->addWidget(zaidType);
    topLayout->addWidget(zaidTreeView);

    zaidListWidget = new QWidget;
    zaidListWidget->setLayout(topLayout);
    zaidListWidget->setToolTip("List of ZAIDS from the XSDIR file for use in materials.");
}
// ----------------------------------------------------------------------------
void MaterialMainWindow::handleZaidTypeChanged(int index)
{
    if (zaidModel->hasChildren())
    {
        zaidModel->removeRows(0, zaidModel->rowCount());
    }

    switch(index)
    {
        case 1:
            addToZaidList(cNeutron, elementNums) ;
            break;
        case 2:
            addToZaidList(dNeutron, elementNums);
            break;
        case 3:
            addToZaidList(photoAtomic, elementNums);
            break;
        case 4:
            addToZaidList(multiGroup, elementNums);
            break;
        case 5:
            addToZaidList(photoNuclear, elementNums);
            break;
        case 6:
            addToZaidList(dosimetry, elementNums);
            break;
        case 7:
            addToZaidList(electron, elementNums);
            break;
        case 8:
            addToZaidList(proton, elementNums);
            break;
        case 9:
            switch(md->customZaidListType)
            {
                case 0:
                    addToZaidList(cNeutron, md->customZaidIntList) ;
                    break;
                case 1:
                    addToZaidList(dNeutron, md->customZaidIntList) ;
                    break;
                case 2:
                    addToZaidList(photoAtomic, md->customZaidIntList) ;
                    break;
                case 3:
                    addToZaidList(multiGroup, md->customZaidIntList) ;
                    break;
                case 4:
                    addToZaidList(photoNuclear, md->customZaidIntList) ;
                    break;
                case 5:
                    addToZaidList(dosimetry, md->customZaidIntList) ;
                    break;
                case 6:
                    addToZaidList(electron, md->customZaidIntList) ;
                    break;
                case 7:
                    addToZaidList(proton, md->customZaidIntList) ;
                    break;
                default:
                    qDebug() << "not implimented";
                    break;
            }           // -- end switch for custom
            break;
        default:
            qDebug() << "not implimented";
            break;
    }
}
// ----------------------------------------------------------------------------
bool caseInsensitiveLessThan(const QString &s1, const QString &s2)
{
    return s1.toLower() < s2.toLower();
}
// ----------------------------------------------------------------------------
void MaterialMainWindow::addToZaidList(QVector<QString> &zList, QVector<int> &zNums)
{
    QString tmp1, tmp2, elZ, eNum;
    int znum, elem;
    int len1, len2;

    QList<QStandardItem *>  preparedRow;
    QVector<QString> candidates;

    for(int e=0; e < zNums.size(); ++e)
    {
        elem = zNums[e];
        // Determine if there are candidates for this element.
        candidates.clear();

        for(int n=0; n < zList.size(); ++n)
        {
            tmp1  = zList[n];
            len2  = zList[n].length();
            len1  = len2 - 7;
            len2 -= len1;
            tmp2 = tmp1.remove(len1, len2);
            znum = tmp2.toInt();

            if (znum == elem)
            {
                elZ = tmp2;
                candidates.push_back(zList[n]);
            }
        }

        if (candidates.size() > 0)
        {
            eNum.setNum(elem);
            preparedRow = prepareParentRow(eNum, elementSymbols[elem], elementNames[elem]);
            QStandardItem *item2 = zaidModel->invisibleRootItem();
            // adding a row to the invisible root item produces a root element
            item2->appendRow(preparedRow);

            qSort(candidates.begin(), candidates.end(), caseInsensitiveLessThan);

            for(int n=0; n < candidates.size(); ++n)
            {
                // Add child or zaid.
                QList<QStandardItem *> secondRow = prepareParentRow(elZ, elementSymbols[elem], candidates[n]);
                // adding a row to an item starts a subtree
                preparedRow.first()->appendRow(secondRow);
            }
        }
    }
}
// ----------------------------------------------------------------------------
QList<QStandardItem *> MaterialMainWindow::prepareParentRow(const QString &first,
                                                            const QString &second,
                                                            const bool edit)
{
    QList<QStandardItem *> rowItems;

    QStandardItem *a = new QStandardItem(first);
    QStandardItem *b = new QStandardItem(second);
    if (edit) a->setEditable(true);
    else a->setEditable(false);
    if (edit) b->setEditable(true);
    else b->setEditable(false);
    rowItems << a << b;

    return rowItems;
}
// ----------------------------------------------------------------------------
QList<QStandardItem *> MaterialMainWindow::prepareParentRow(const QString &first,
                                                            const QString &second,
                                                            const QString &third)
{
    QList<QStandardItem *> rowItems;

    QStandardItem *a = new QStandardItem(first);
    QStandardItem *b = new QStandardItem(second);
    QStandardItem *c = new QStandardItem(third);

    a->setEditable(false);
    b->setEditable(false);
    c->setEditable(false);
    rowItems << a << b << c;

    return rowItems;
}
// ----------------------------------------------------------------------------
QList<QStandardItem *> MaterialMainWindow::prepareChildRow(const QString &first,
                                                           const QString &second)
{
    QList<QStandardItem *> rowItems;

    QStandardItem *a = new QStandardItem(first);
    QStandardItem *b = new QStandardItem(second);
    a->setEditable(false);
    b->setEditable(false);
    rowItems << a << b;

    return rowItems;
}
// ----------------------------------------------------------------------------
void MaterialMainWindow::onCustomContextMenuZaid(const QPoint &point)
{
    QModelIndex index = zaidTreeView->indexAt(point);
    if (index.isValid() )
    {
//        qDebug() << "index row: " << index.row() <<  "\t column: " << index.column() << "\n";

        QStandardItem *item0;
        item0 = zaidModel->itemFromIndex(index);
//        qDebug() << "data0: " << item0->data(0);
        if (index.column() != 2)
        {
            QMessageBox about;
            QString str = "Please right click on ZAID column for menu.\n";
            about.setIcon(QMessageBox::Information);
            about.setInformativeText(str);
            about.setStandardButtons(QMessageBox::Ok);
            about.setDefaultButton(QMessageBox::Ok);
            about.show();
            about.exec();
            return;
        }

 //       qDebug() << "rowcount: " << item0->rowCount();
 //       qDebug() << "parent  : " << item0->parent();

        if (item0->parent() == nullptr)
        {
            QMessageBox about;
            QString str = "Please right click on ZAID and not the element to see the menu.\n";
            about.setIcon(QMessageBox::Information);
            about.setInformativeText(str);
            about.setStandardButtons(QMessageBox::Ok);
            about.setDefaultButton(QMessageBox::Ok);
            about.show();
            about.exec();
            return;
        }

        QMenu contextMenu;
        QAction *addAction   = contextMenu.addAction(tr("Add this ZAID"));

        QAction *action = contextMenu.exec(zaidTreeView->viewport()->mapToGlobal(point));
        if (action == addAction)
        {
            QString one = "1.0";
            QString zaid = item0->text();
//            qDebug() << "item0: " << item0->text();
            addToWorkingList(zaid, one);
        }
    }               // -- end if for valid index
}
// ------------------------------------------------------------------------------------------------
void MaterialMainWindow::buildWorkingListWidget()
{
    workingModel = new QStandardItemModel;

    QStringList hHeaderList;
    if (md->projectType == MCNP)
        hHeaderList << QStringLiteral("Zaid") << QStringLiteral("Amount");
    if (md->projectType == ITS)
        hHeaderList << QStringLiteral("Element") << QStringLiteral("Weight Fraction");

    workingModel->setHorizontalHeaderLabels(hHeaderList);

//    connect(zaidModel, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(bodyModelItemChanged(QStandardItem *)));

    workingTreeView = new QTreeView;
    workingTreeView->setModel(workingModel);
    workingTreeView->expandAll();

#ifdef USING_WINDOWS
    workingTreeView->setColumnWidth(0, 140);
    workingTreeView->setColumnWidth(1, 120);
#else
    workingTreeView->setColumnWidth(0, 100);
    workingTreeView->setColumnWidth(1, 70);
#endif

    workingTreeView->setMinimumSize(300, 170);
    workingTreeView->sortByColumn(0, Qt::AscendingOrder);
    workingTreeView->setSortingEnabled(true);
    workingTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(workingTreeView, SIGNAL(customContextMenuRequested(const QPoint &)),
            this,            SLOT(onCustomContextMenuWorking(const QPoint &)));

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addWidget(workingTreeView);
    topLayout->addStretch();
    workingListWidget = new QWidget;
    workingListWidget->setLayout(topLayout);
    workingListWidget->setToolTip("Working list of nuclides for the current material being built.");
}
// ------------------------------------------------------------------------------------------------
void MaterialMainWindow::addToWorkingList(QString zaid, QString value)
{
    QList<QStandardItem *>  preparedRow;

    preparedRow = prepareParentRow(zaid, value, true);
    QStandardItem *item = workingModel->invisibleRootItem();
    // adding a row to the invisible root item produces a root element
    item->appendRow(preparedRow);
}

// ----------------------------------------------------------------------------
void MaterialMainWindow::onCustomContextMenuWorking(const QPoint &point)
{
    QModelIndex index = zaidTreeView->indexAt(point);
    if (index.isValid() )
    {
//        qDebug() << "index row: " << index.row() <<  "\t column: " << index.column() << "\n";

        QStandardItem *itemClicked;
        itemClicked = zaidModel->itemFromIndex(index);

        QMenu contextMenu;
        QAction *deleteAction   = contextMenu.addAction(tr("Delete this ZAID"));

        QAction *action = contextMenu.exec(workingTreeView->viewport()->mapToGlobal(point));
        if (action == deleteAction)
        {
//            qDebug() << "item0: " << itemClicked->text();
            workingModel->removeRow(itemClicked->row());
        }
    }               // -- end if for valid index
}

// ------------------------------------------------------------------------------------------------
void MaterialMainWindow::buildCloseLayout()
{
    closePB = new QPushButton("CLOSE");
    closePB->setToolTip("Click to close this window.");
    connect(closePB, SIGNAL(released()), this, SLOT(doClose()));

    closeLayout = new QHBoxLayout;
    closeLayout->addStretch();
    closeLayout->addWidget(closePB);
}
// ------------------------------------------------------------------------------------------------
void MaterialMainWindow::doClose()
{
    writeSettings();
    close();
}
// ------------------------------------------------------------------------------------------------
void MaterialMainWindow::buildMaterialsListWidget()
{
    materialsModel = new QStandardItemModel;

    QStringList hHeaderList;
    hHeaderList << QStringLiteral("Mtl #") << QStringLiteral("Parameter")
                << QStringLiteral("Value");
    materialsModel->setHorizontalHeaderLabels(hHeaderList);

//    connect(zaidModel, SIGNAL(itemChanged(QStandardItem *)), this, SLOT(bodyModelItemChanged(QStandardItem *)));

    materialsTreeView = new QTreeView;
    materialsTreeView->setModel(materialsModel);
    materialsTreeView->expandAll();

#ifdef USING_WINDOWS
    materialsTreeView->setColumnWidth(0,  80);
    materialsTreeView->setColumnWidth(1, 120);
    materialsTreeView->setColumnWidth(2, 70);
#else
    materialsTreeView->setColumnWidth(0, 80);
    materialsTreeView->setColumnWidth(1, 90);
    materialsTreeView->setColumnWidth(2, 70);
#endif

    materialsTreeView->sortByColumn(0, Qt::AscendingOrder);
    materialsTreeView->setSortingEnabled(true);
    materialsTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(materialsTreeView, SIGNAL(customContextMenuRequested(const QPoint &)),
            this,              SLOT(onCustomContextMenuMaterials(const QPoint &)));

    materialsTreeView->setMinimumSize(90, 500);

//    QLabel *matListLabel = new QLabel("Materials List");
//    matListLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout *topLayout = new QVBoxLayout;
//    topLayout->addWidget(matListLabel);
    topLayout->addWidget(materialsTreeView);

    materialsListWidget = new QWidget;
    materialsListWidget->setLayout(topLayout);
    materialsListWidget->setMinimumWidth(280);
    materialsListWidget->setToolTip("Master material list for the project.");
}
// ----------------------------------------------------------------------------
void MaterialMainWindow::onCustomContextMenuMaterials(const QPoint &point)
{
    QModelIndex index = materialsTreeView->indexAt(point);
    if (index.isValid() )
    {
        if (index.column() > 0)
        {
            QMessageBox about;
            QString str = "Please right click on material number of parent for menu.\n";
            about.setIcon(QMessageBox::Information);
            about.setInformativeText(str);
            about.setStandardButtons(QMessageBox::Ok);
            about.setDefaultButton(QMessageBox::Ok);
            about.show();
            about.exec();
            return;
        }

        QStandardItem *itemClicked;
        itemClicked = materialsModel->itemFromIndex(index);

        if (!itemClicked->hasChildren())
        {
            QMessageBox about;
            QString str = "Please right click on material number of PARENT to see menu.\n";
            about.setIcon(QMessageBox::Information);
            about.setInformativeText(str);
            about.setStandardButtons(QMessageBox::Ok);
            about.setDefaultButton(QMessageBox::Ok);
            about.show();
            about.exec();
            return;
        }

        QMenu contextMenu;
        QAction *deleteAction = contextMenu.addAction(tr("Delete this material"));
        QAction *editAction   = contextMenu.addAction(tr("Edit this material"));

        QString mtlString;
        mtlString = index.data(0).toString();
        int m = mtlString.toInt();
        mtlNumMap[m] = 0;

        QAction *action = contextMenu.exec(materialsTreeView->viewport()->mapToGlobal(point));
        if (action == deleteAction)
        {
        }
        if (action == editAction)
        {
            setEditFields(mtlString);
        }

        if (action != NULL)
        {
            materialsModel->removeRow(itemClicked->row());
            removeFromMtlDataStringList(mtlString);
        }

    }               // -- end if for valid index
}
// ----------------------------------------------------------------------------
void MaterialMainWindow::setEditFields(QString mtl)
{
    QStringList sl;

    mtlData.mtlNo = mtl;

    int i;
    for(i=0; i < (int) md->mtlDataStringList.size(); ++i)
    {
        sl = md->mtlDataStringList[i];
        if (mtl == sl[0])
        {
            break;
        }
    }
    if (i < (int) md->mtlDataStringList.size())
    {
        materialNumber->setText(sl[0]);
        materialName->setText(sl[1]);
        materialDensity->setText(sl[2]);
        QColor color;
        int k = sl[3].toInt();
        color.setRed(k);
        k = sl[4].toInt();
        color.setGreen(k);
        k = sl[5].toInt();
        color.setBlue(k);
        k = sl[6].toInt();
        color.setAlpha(k);
        materialColor->setPalette(color);
        workingModel->removeRows(0, workingModel->rowCount());

        int nZaids = sl[7].toInt();
        for(k = 0; k < nZaids; ++k)
        {
            addToWorkingList(sl[8+2*k], sl[9+2*k]);
        }
    }
}

// ----------------------------------------------------------------------------
void MaterialMainWindow::removeFromMtlDataStringList(QString mtl)
{
    // Given a cell number, remove the QStringList from the master data vector.
    QStringList sl;

    mtlData.mtlNo = mtl;

    for(int i=0; i < (int) md->mtlDataStringList.size(); ++i)
    {
        sl = md->mtlDataStringList[i];
        if (mtl == sl[0])
        {
            md->mtlDataStringList.removeAt(i);
            break;
        }
    }
}
// ----------------------------------------------------------------------------
void MaterialMainWindow::handleColorButton()
{
    QString s1("Material Color Chooser");

    QColor colorOld(mtlData.color);

    QColor color = QColorDialog::getColor(colorOld, this, s1, QColorDialog::ShowAlphaChannel);
    if (color.isValid())
    {
        materialColor->setPalette(color);
        mtlData.color = color;
    }
}

// ----------------------------------------------------------------------------
void MaterialMainWindow::handleMtlClear()
{
    materialName->setText("");
    materialNumber->setText("");
    materialDensity->setText("");
    mtlData.clear();

    materialColor->setPalette(QColor( "White" ));
    materialColor->setText("COLOR");

    int limit = workingModel->rowCount() - 1;
    for (int i=limit; i != -1; --i)
    {
        workingModel->removeRow(i);
    }
}

// ----------------------------------------------------------------------------
void MaterialMainWindow::handleMtlAdd()
{
/*
    qDebug() << "materialName   : " << materialName->text();
    qDebug() << "materialNumber : " << materialNumber->text();
    qDebug() << "materialDensity: " << materialDensity->text().toDouble();

    qDebug() << "material color red: " << mtlData.color.red();
    qDebug() << "material color grn: " << mtlData.color.green();
    qDebug() << "material color blu: " << mtlData.color.blue();
*/

    int mtlNo = materialNumber->text().toInt();

    if ((mtlNumMap[mtlNo] > 0))
    {
        QString s = materialNumber->text();
        QMessageBox msg;
        QString str = "Material number " + s + " is already in use.  Please select another.\n";
        msg.setIcon(QMessageBox::Information);
        msg.setInformativeText(str);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.show();
        msg.exec();
        return;
    }
    if (mtlNo == 0)
    {
        QMessageBox msg;
        QString str = "Please enter an unique material number.\n";
        msg.setIcon(QMessageBox::Information);
        msg.setInformativeText(str);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.show();
        msg.exec();
        return;
    }
    if (materialDensity->text().toDouble() == 0.0)
    {
        QMessageBox msg;
        QString str = "Please enter a non-zero material density.\n";
        msg.setIcon(QMessageBox::Information);
        msg.setInformativeText(str);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.show();
        msg.exec();
        return;
    }
    if (workingModel->rowCount() < 1)
    {
        QMessageBox msg;
        QString str = "Please enter a ZAID.\n";
        msg.setIcon(QMessageBox::Information);
        msg.setInformativeText(str);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.show();
        msg.exec();
        return;
    }

    int num = materialNumber->text().toInt();
    mtlNumMap[num] = num;

    // Add to QTreeView
    addToMaterialsList();
    addToMtlDataStringList();

    mtlData.color.setNamedColor("White");
    handleMtlClear();
}
// ----------------------------------------------------------------------------
void MaterialMainWindow::addToMaterialsList()
{
    QString tmp1, tmp2;
    int znum;
    int len1, len2;
    double wfSum;

    QList<QStandardItem *>  preparedRow;
    QList<QStandardItem *>  secondRow;

    QString s1 = mtlData.mtlNo;
    QString s2 = "name";
    QString s3 = mtlData.name;
    QString s4;
    preparedRow = prepareParentRow(s1, s2, s3);

    QStandardItem *item = materialsModel->invisibleRootItem();
    // adding a row to the invisible root item produces a root element
    item->appendRow(preparedRow);

    // Append the density row
    s2 = "density";
    s3 = mtlData.density;
    secondRow = prepareParentRow(s1, s2, s3);
    preparedRow.first()->appendRow(secondRow);

    wfSum = 0.0;
    QStandardItem *zaid;
    for (int i=0; i < workingModel->rowCount(); ++i)
    {
        zaid = workingModel->item(i,0);
        s2 = zaid->text();

        // Find the symbol and set it to s1.
        /*
        tmp1  = s2;
        len2  = s2.length();
        len1  = len2 - 7;
        len2 -= len1;
        tmp2 = tmp1.remove(len1, len2);
        znum = tmp2.toInt();
        */
        if (md->projectType == MCNP)
        {
            len1 = s2.length();
            if (len1 > 3)
            {
                len2 = s2.indexOf(".");
                tmp1 = s2;
                if (len2 > 0) len2 -= 3;
                else          len2  = 2;
                tmp2 = tmp1.remove(len2, len1);
                znum = tmp2.toInt();
                s1   = elementSymbols[znum];
                zaid = workingModel->item(i,1);
                s3   = zaid->text();
                secondRow = prepareParentRow(s1, s2, s3);
                preparedRow.first()->appendRow(secondRow);
            }
            else
            {
                QString s1 = "Improper ZAID string: " + s2 + "\n";
                s1 += "Not adding this one.  Please enter a proper ZAID.";
                QMessageBox about;
                about.setIcon(QMessageBox::Warning);
                about.setInformativeText(s1);
                about.setStandardButtons(QMessageBox::Ok);
                about.setDefaultButton(QMessageBox::Ok);
                about.show();
                about.exec();
            }
        }               // -- end if
        else
        {
            zaid   = workingModel->item(i, 1);
            s4     = zaid->text();
            wfSum += s4.toDouble();

            secondRow = prepareParentRow(s1, s2, s4);
            preparedRow.first()->appendRow(secondRow);

        }               // -- end else
    }                   // -- end i-loop

    if (md->projectType == ITS)
    {
        QString msg = mtlData.name + "\n";
        QString sw;
        sw = QString("%1").arg(wfSum, 8, 'f', 6);
        if (wfSum > 1.0000001)
        {
            msg += "sum of weight fractions is greater than 1.0 : ";
            msg +=  sw;
        }
        else if (wfSum < 0.999999)
        {
            msg += "sum of weight fractions is less than 1.0 : ";
            msg +=  sw;
        }
        else
            msg += ("sum of weight fractions looks like 1.0 : " + sw);

        messageWidget->displayTextString(msg);
    }
}

// ----------------------------------------------------------------------------
void MaterialMainWindow::addToMtlDataStringList()
{
    int         i;
    int        n1;
    int        n2;
    int      klim;
    QString     s1;
    QStringList sl;

    sl.push_back(mtlData.mtlNo);
    sl.push_back(mtlData.name);
    sl.push_back(mtlData.density);

    // Store the color
    i = mtlData.color.red();
    s1.setNum(i);
    sl.push_back(s1);
    i = mtlData.color.green();
    s1.setNum(i);
    sl.push_back(s1);
    i = mtlData.color.blue();
    s1.setNum(i);
    sl.push_back(s1);
    i = mtlData.color.alpha();
    s1.setNum(i);
    sl.push_back(s1);

    // Store the number of zaids in the material.
    i = (int) workingModel->rowCount();
    s1.setNum(i);
    sl.push_back(s1);

    QStandardItem *zaid;
    for (int i=0; i < workingModel->rowCount(); ++i)
    {
        zaid = workingModel->item(i,0);
        s1 = zaid->text();
        sl.push_back(s1);

        zaid = workingModel->item(i,1);
        s1 = zaid->text();
        sl.push_back(s1);
    }

    // Insert in order by material number.
    QStringList l;
    klim = (int) md->mtlDataStringList.size();

    if (klim > 0)
    {
        n1 = sl[0].toInt();
        for(i=0; i<klim; ++i)
        {
            l = md->mtlDataStringList[i];
            n2 = l[0].toInt();

            if (n2 > n1) break;
        }

        md->mtlDataStringList.insert(i, sl);
    }
    else
        md->mtlDataStringList.push_back(sl);
}
// ----------------------------------------------------------------------------
void MaterialMainWindow::handleMtlNoEdit(QString s)
{
    mtlData.mtlNo  = s;
}
// ----------------------------------------------------------------------------
void MaterialMainWindow::handleNameEdit(QString s)
{
    mtlData.name = s;
}
// ----------------------------------------------------------------------------
void MaterialMainWindow::handleDensityEdit(QString s)
{
    mtlData.density = s;
}
// ----------------------------------------------------------------------------
void MaterialMainWindow::addExistingMaterials()
{
    mtlNumMap.clear();
    for(int b=0; b < (int) md->mtlDataStringList.size(); ++b)
    {
        addSingleMtlFromList(b);
    }
}
// ----------------------------------------------------------------------------
void MaterialMainWindow::addSingleMtlFromList(int b)
{
    QString tmp1, tmp2;
    int znum;
    int len1, len2;
    int i;
    QStringList sl = md->mtlDataStringList[b];

    mtlData.clear();
    mtlData.mtlNo = sl[0];
    mtlData.name  = sl[1];
    mtlData.density = sl[2];

    i = mtlData.mtlNo.toInt();
    mtlNumMap[i] = i;

    materialsTreeView->setSortingEnabled(false);

    QList<QStandardItem *>  preparedRow;
    QList<QStandardItem *>  secondRow;

    QString s1 = mtlData.mtlNo;
    QString s2 = "name";
    QString s3 = mtlData.name;
    preparedRow = prepareParentRow(s1, s2, s3);

    QStandardItem *item = materialsModel->invisibleRootItem();
    // adding a row to the invisible root item produces a root element
    item->appendRow(preparedRow);

    s2 = "density";
    s3 = mtlData.density;
    secondRow = prepareParentRow(s1, s2, s3);
    preparedRow.first()->appendRow(secondRow);

    int nZaids = sl[7].toInt();

    int k = 7;
    for (i=0; i<nZaids; ++i)
    {
        ++k;
        s2    = sl[k];
        len2  = s2.length();
        int d = s2.indexOf('.');
        tmp1  = s2;

        if (md->projectType == MCNP)
        {
            if (d > 0)
            {
                // Find the symbol and set it to s1.
                len1  = d - 3;
                len2  = len2 - len1;
                tmp2 = tmp1.remove(len1, len2);
                znum = tmp2.toInt();
            }
            else
            {
                len1 = len2 - 3;
                tmp2 = tmp1.remove(len1, 3);
                znum = tmp2.toInt();
            }
            s1 = elementSymbols[znum];
        }
        ++k;
        s3 = sl[k];
        secondRow = prepareParentRow(s1, s2, s3);
        preparedRow.first()->appendRow(secondRow);
    }
    materialsTreeView->setSortingEnabled(true);
}
// ----------------------------------------------------------------------------
void MaterialMainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Alt)
    {
        isAltKey = true;
        //qDebug() << " Key_Alt keys\n";
    }

    if (event->key() == Qt::Key_C)
    {
        if (isAltKey)
        {
            if (customZaidList == nullptr)
            {
                customZaidList = new CustomZaidList(md, this);
//                connect(customZaidList, SIGNAL(updateNeededSignal()),
//                        this,              SLOT(handleUpdateSignal()));
            }
            customZaidList->show();
            customZaidList->update();
            customZaidList->raise();
            customZaidList->activateWindow();
            customZaidList->updateTable();
        }
    }
}
// ------------------------------------------------------------------------------------------------
void MaterialMainWindow::readXgenData()
{
    QString     line;
    QStringList fields;
    QFile file(":/Resources/xgen_data.txt");

    if (!file.open(QIODevice::ReadOnly))
    {
        QString fileError = file.errorString();
        fileError += "\nFile: \n";
        fileError += file.fileName();
        QMessageBox about;
        about.setIcon(QMessageBox::Critical);
        about.setInformativeText(fileError);
        about.setStandardButtons(QMessageBox::Ok);
        about.setDefaultButton(QMessageBox::Ok);
        about.show();
        about.exec();
        return;
    }
    xgenElNumber.clear();
    xgenElName.clear();
    xgenElSymbol.clear();
    xgenElDensity.clear();

    QTextStream in(&file);
    int n = 0;
    while(!in.atEnd())
    {
        line   = in.readLine();
        line   = sss_single_space_separator(line);
        fields = line.split(' ');
        xgenElNumber.push_back(fields[0]);
        xgenElName.push_back(fields[1]);
        xgenElSymbol.push_back(fields[2]);
        xgenElDensity.push_back(fields[3]);
    }
}

// ------------------------------------------------------------------------------------------------
void MaterialMainWindow::buildXgenListWidget()
{
    xgenModel = new QStandardItemModel;

    QStringList hHeaderList;
    hHeaderList << QStringLiteral("Z") << QStringLiteral("Element") << QStringLiteral("Density");
    xgenModel->setHorizontalHeaderLabels(hHeaderList);

    xgenTreeView = new QTreeView;
    xgenTreeView->setModel(xgenModel);
    xgenTreeView->expandAll();

#ifdef USING_WINDOWS
    xgenTreeView->setColumnWidth(0,  80);
    xgenTreeView->setColumnWidth(1, 100);
    xgenTreeView->setColumnWidth(2, 120);
    xgenTreeView->setMinimumSize(230, 500);
#else
    xgenTreeView->setColumnWidth(0, 70);
    xgenTreeView->setColumnWidth(1, 80);
    xgenTreeView->setColumnWidth(2, 70);
    xgenTreeView->setMinimumSize(220, 500);
#endif

    xgenTreeView->sortByColumn(0, Qt::AscendingOrder);
    xgenTreeView->setSortingEnabled(true);
    xgenTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(xgenTreeView, SIGNAL(customContextMenuRequested(const QPoint &)),
            this,         SLOT(onCustomContextMenuXgen(const QPoint &)));

    QVBoxLayout *topLayout = new QVBoxLayout;
    topLayout->addWidget(xgenTreeView);

    xgenListWidget = new QWidget;
    xgenListWidget->setLayout(topLayout);
    xgenListWidget->setToolTip("List of element for use in XGEN materials.");

    addToXgenList();
}

// ----------------------------------------------------------------------------
void MaterialMainWindow::addToXgenList()
{
    if (xgenModel->hasChildren())
    {
        xgenModel->removeRows(0, xgenModel->rowCount());
    }

    QList<QStandardItem *>  preparedRow;

    for(int e=0; e < xgenElNumber.size(); ++e)
    {
        preparedRow = prepareParentRow(xgenElNumber[e], xgenElSymbol[e], xgenElDensity[e]);
        QStandardItem *item2 = xgenModel->invisibleRootItem();
        // adding a row to the invisible root item produces a root element
        item2->appendRow(preparedRow);
    }
}

// ----------------------------------------------------------------------------
void MaterialMainWindow::onCustomContextMenuXgen(const QPoint &point)
{
    QModelIndex index = xgenTreeView->indexAt(point);
    if (index.isValid() )
    {
        QStandardItem *item0;
        item0 = xgenModel->itemFromIndex(index);
//        qDebug() << "data0: " << item0->data(0);
        if (index.column() != 1)
        {
            QMessageBox about;
            QString str = "Please right click on Element column for menu.\n";
            about.setIcon(QMessageBox::Information);
            about.setInformativeText(str);
            about.setStandardButtons(QMessageBox::Ok);
            about.setDefaultButton(QMessageBox::Ok);
            about.show();
            about.exec();
            return;
        }

        /*
        if (item0->parent() == nullptr)
        {
            QMessageBox about;
            QString str = "Please right click on Z and not the element symbol to see the menu.\n";
            about.setIcon(QMessageBox::Information);
            about.setInformativeText(str);
            about.setStandardButtons(QMessageBox::Ok);
            about.setDefaultButton(QMessageBox::Ok);
            about.show();
            about.exec();
            return;
        }
        */

        QMenu contextMenu;
        QAction *addAction   = contextMenu.addAction(tr("Add this element"));

        QAction *action = contextMenu.exec(xgenTreeView->viewport()->mapToGlobal(point));
        if (action == addAction)
        {
            QString one = "1.0";
            QString zaid = item0->text();
//            qDebug() << "item0: " << item0->text();
            addToWorkingList(zaid, one);
        }
    }               // -- end if for valid index
}
