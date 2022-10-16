#include "buildbodycontrols.h"

BuildBodyControls::BuildBodyControls(MasterData           *masterDataIn,
                                     License              *inLic,
                                     QStandardItemModel   *bodyModelIn,
                                     QVector<CGBodyBasics>   *inCgBodyBasics
                                     )
{
    md = masterDataIn;
    currentBodyNo = 0;
    projectType   = md->projectType;
    buildBodies   = inCgBodyBasics;

    bodyDataInput = &md->bodyDataStringList;
    bodyModel     = bodyModelIn;
    license       = inLic;

    buildBodies->clear();
    bodyNumMap.clear();

//    buildTypeMap();

    buildBodyButtonLayout();
    buildBodyLineEditLayout();
    buildClearAddButtonLayout();
    buildBodyNameLayout();
    buildBodyTreeViewLayout();

    QLabel *topLabel = new QLabel("Body Description");
    adjustQLabel(topLabel, +1, true);
    topLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    QLabel *listLabel = new QLabel("Body List");
    adjustQLabel(listLabel, +1, true);
    listLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(topLabel);
    leftLayout->addLayout(bodyNameLayout);
    leftLayout->addSpacing(30);
    leftLayout->addLayout(buttonGridLayout);
    leftLayout->addSpacing(30);

    leftLayout->addLayout(lineEditGridLayout);
    leftLayout->addSpacing(15);
    leftLayout->addLayout(clearAddLayout);
    leftLayout->addSpacing(5);

    leftLayout->addWidget(listLabel);
    leftLayout->addLayout(bodyTreeViewLayout);

    // ------------------------------------------
    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addLayout(leftLayout);
    //topLayout->addSpacing(30);

    topLayout->addStretch();
    setLayout(topLayout);

    addExistingBodies();
}
// ----------------------------------------------------------------------------
void BuildBodyControls::buildBodyButtonLayout()
{
    bodyButtonGroup = new QButtonGroup;

    BodyButtonInfo bbi;

    for(int i=0; i<bbi.bodyButtonLabels.size(); ++i)
    {
        SSS_PushButton *pb = new SSS_PushButton( );
        pb->setText(bbi.bodyButtonLabels[i]);
        pb->setVal1(i);
        pb->setToolTip(bbi.bodyButtonTips[i]);
        pb->setMaximumWidth(90);
        connect(pb, SIGNAL(released(int, int)), this, SLOT(handleBodyButton(int, int)));
        bodyButtons.push_back(pb);
    }

    buttonGridLayout = new QGridLayout;
    buttonGridLayout->setVerticalSpacing(15);
    buttonGridLayout->setHorizontalSpacing(20);

    /*
    int index = 0;
    for (int i=0; i<3; ++i)
    {
        for (int j=0; j<3; ++j)
        {
            buttonGridLayout->addWidget(bodyButtons[index],     i, j, 1, 1);
            ++index;
        }
    }



//    if (license->devOption1)
//    {
        buttonGridLayout->addWidget(bodyButtons[index],   3, 0, 1, 1);
//    }
    */

    int index = 0;
    for (int i=0; i<2; ++i)
    {
        for (int j=0; j<5; ++j)
        {
                buttonGridLayout->addWidget(bodyButtons[index],     i, j, 1, 1);
                ++index;
        }
    }
}
// ----------------------------------------------------------------------------
void BuildBodyControls::handleBodyButton(int v1, int v2)
{
    BodyButtonInfo bbi;
    bodyData.bodyTag = bbi.bodyTypeTags[v1];
    bodyData.limit   = bbi.skipTo[v1]-1;

    for(int i=0; i<12; ++i)
    {
        bodyEditLabels[i]->setText(bbi.bodyTextLabels[v1][i]);
        bodyData.parameterTags[i] = bbi.bodyTextLabels[v1][i];
    }

    for(unsigned char i = bbi.skipTo[v1]; i<12; ++i)   bodyLineEdits[i]->setEnabled(false);
    for(unsigned char i = 0; i<bbi.skipTo[v1]-1;  ++i) bodyLineEdits[i]->setEnabled(true);

    int istop = 10;
//    if (license->devOption1) istop = 10;

    for(int i=0; i<istop; ++i)
    {
        if (i != v1) bodyButtons[i]->setEnabled(false);
    }
    addPB->setEnabled(true);
}

// ----------------------------------------------------------------------------
void BuildBodyControls::buildBodyLineEditLayout()
{
    for(int i=0; i<12; ++i)
    {
        QLabel *s = new QLabel("* * * *");
        SSS_LineEdit *le = new SSS_LineEdit();
        le->setVal1(i);
        le->setMinimumWidth(100);
        connect(le, SIGNAL(textChanged(QString, int)), this, SLOT(handleBodyLineEdit(QString, int)));
        bodyLineEdits.push_back(le);
        bodyEditLabels.push_back(s);
    }

    lineEditGridLayout = new QGridLayout;
    lineEditGridLayout->setVerticalSpacing(15);
    lineEditGridLayout->setHorizontalSpacing(20);

    int index = 0;

    for (int i=0; i<4; ++i)
    {
        for (int j=0; j<3; ++j)
        {
            lineEditGridLayout->addWidget(bodyEditLabels[index], i, (2*j),   1, 1);
            lineEditGridLayout->addWidget(bodyLineEdits[index],  i, (2*j+1), 1, 1);
            ++index;
        }
    }

    for(int i=0; i<12; ++i) bodyLineEdits[i]->setEnabled(false);
}
// ----------------------------------------------------------------------------
void BuildBodyControls::handleBodyLineEdit(QString s1, int v1)
{
    bodyData.parameterValues[v1] = s1;
}
// ----------------------------------------------------------------------------
void BuildBodyControls::buildClearAddButtonLayout()
{
    clearPB = new QPushButton("CLEAR");
    clearPB->setToolTip("Click to clear body input parameters and reset body push buttons.");
    connect(clearPB,  SIGNAL(clicked()), this, SLOT(handleClearPB()));

    addPB = new QPushButton("ADD BODY");
    addPB->setToolTip("Click to add the body to the list and reset input for next body.");
    connect(addPB,    SIGNAL(clicked()), this, SLOT(handleAddPB()));
    addPB->setEnabled(false);

    clearAddLayout = new QHBoxLayout;
    clearAddLayout->addWidget(clearPB);
    clearAddLayout->addWidget(addPB);
    clearAddLayout->addStretch();
}
// ----------------------------------------------------------------------------
void BuildBodyControls::handleClearPB()
{
    int istop = 10;
//    if (license->devOption1) istop = 10;

    for(int i=0; i<istop; ++i)  bodyButtons[i]->setEnabled(true);
    for(int i=0; i<12; ++i)     bodyEditLabels[i]->setText( "* * * *" );

    for(int i=0; i<12; ++i)
    {
        bodyLineEdits[i]->setEnabled(false);
        bodyLineEdits[i]->clear();
    }
    nameLineEdit->clear();
    bodyData.clear();
}
// ----------------------------------------------------------------------------
void BuildBodyControls::handleAddPB()
{
    //qDebug() << " BuildBodyControls::handleAddRB \n";
    ++currentBodyNo;
    bodyData.bodyNo = currentBodyNo;

    // ------------------------------------------------------------------------
    QString body;
    body.setNum(currentBodyNo);

    // ------------------------------------------------------------------------
    bodyTreeView->setSortingEnabled(false);

    // Set default user body number.
    bodyNumMap[currentBodyNo] = currentBodyNo;

    // Create the individual body data list.
    QStringList sList;
    sList.push_back(body);
    sList.push_back(bodyData.bodyTag);
    sList.push_back(bodyData.bodyName);
    for (unsigned char i=0; i<bodyData.limit; ++i)
    {
        sList.push_back(bodyData.parameterTags[i]);
        sList.push_back(bodyData.parameterValues[i]);
    }

    // Add the individual body data list to the global list.
    bodyDataInput->push_back(sList);

    // Add the body declaration as the parent node to the tree view.
    QList<QStandardItem *>  preparedRow =
                            prepareParentRow(body, bodyData.bodyTag, bodyData.bodyName);
    QStandardItem *item2 = bodyModel->invisibleRootItem();
    // adding a row to the invisible root item produces a root element
    item2->appendRow(preparedRow);

    // Add the body parameters as children.
    for (unsigned char i=0; i<bodyData.limit; ++i)
    {
        QList<QStandardItem *> secondRow =
                               prepareChildRow(body, bodyData.parameterTags[i], bodyData.parameterValues[i]);
        // adding a row to an item starts a subtree
        preparedRow.first()->appendRow(secondRow);
    }

    bodyTreeView->setSortingEnabled(true);
    addPB->setEnabled(false);

    QString s1 = "Adding Body: " + body + " " + bodyData.bodyName + " " + bodyData.bodyTag + "\n";

    CGBodyBasics tmpBody;
    tmpBody.parameters.clear();
    tmpBody.bodyNumber = body.toInt();
    tmpBody.bodyName   = bodyData.bodyName;
    tmpBody.bodyType   = typeMap.bodyNumMap[bodyData.bodyTag];
    double value;
    for (unsigned char i=0; i<bodyData.limit; ++i)
    {
        value = bodyData.parameterValues[i].toDouble();
        tmpBody.parameters.push_back(value);
        s1 += bodyData.parameterValues[i] + " ";
    }
    buildBodies->push_back(tmpBody);

    emit bodyControlsMessage(s1);

    bodyAnalysis(tmpBody);

    // handleClearPb must be last because it resets bodyData.
    handleClearPB();
}
// ----------------------------------------------------------------------------
void BuildBodyControls::bodyAnalysis(CGBodyBasics &tmpBody)
{
    if (tmpBody.bodyType == BOX)
    {
        QString s1, s2, s3, s;
        double len;
        double a1[3], a2[3], a3[3], angles[3], corners[24], v[3];
        a1[0] = tmpBody.parameters[3];
        a1[1] = tmpBody.parameters[4];
        a1[2] = tmpBody.parameters[5];
        len = vectorLength(a1);
        s1 = "\na1 vector length ";
        s2.setNum(len);
        s = s1 + s2;
        emit bodyControlsMessage(s);

        a2[0] = tmpBody.parameters[6];
        a2[1] = tmpBody.parameters[7];
        a2[2] = tmpBody.parameters[8];
        len = vectorLength(a2);
        s1 = "a2 vector length ";
        s2.setNum(len);
        s = s1 + s2;
        emit bodyControlsMessage(s);

        a3[0] = tmpBody.parameters[ 9];
        a3[1] = tmpBody.parameters[10];
        a3[2] = tmpBody.parameters[11];
        len = vectorLength(a3);
        s1 = "a3 vector length ";
        s2.setNum(len);
        s = s1 + s2 + "\n";
        emit bodyControlsMessage(s);

        v[0] = tmpBody.parameters[0];
        v[1] = tmpBody.parameters[1];
        v[2] = tmpBody.parameters[2];

        boxCorners(v, a1, a2, a3, corners);

        int k;
        for (int i=0; i<8; ++i)
        {
            s2.setNum((i+1));
            s = "Corner #" + s2;
            k = i*3;
            s1.sprintf("%12.5e", corners[k]);
            s2.sprintf("%12.5e", corners[k+1]);
            s3.sprintf("%12.5e", corners[k+2]);
            s += "  " + s1 + "  " + s2 + "  " + s3;
            emit bodyControlsMessage(s);
        }

        len = crossProduct(a2, a1, corners);
        s1.sprintf("%7.3f", -len);
        s = "\nAngle between a1 and a2: ";
        s += s1;
        emit bodyControlsMessage(s);

        len = crossProduct(a3, a1, corners);
        s1.sprintf("%7.3f", -len);
        s = "Angle between a1 and a3: ";
        s += s1;
        emit bodyControlsMessage(s);

        len = crossProduct(a2, a3, corners);
        s1.sprintf("%7.3f", -len);
        s = "Angle between a2 and a3: ";
        s += s1 + "\n";
        emit bodyControlsMessage(s);

    }           // -- end BOX

    if (tmpBody.bodyType == TOR)
    {
        double phi = tmpBody.parameters[3] * tmpBody.parameters[3] +
                     tmpBody.parameters[4] * tmpBody.parameters[4] +
                     tmpBody.parameters[5] * tmpBody.parameters[5];

        QString s1 = "\n";
        emit bodyControlsMessage(s1);

        if (phi < 1.0E-10)
        {
            s1 = ">>>>> ERROR: invalid axis of revolution for torus\n";
            emit bodyControlsMessage(s1);
        }
        if (tmpBody.parameters[7] < 1.0E-10)
        {
            s1 = ">>>>> ERROR: invalid RH value for torus\n";
            emit bodyControlsMessage(s1);
        }
        if (tmpBody.parameters[8] < 1.0E-10)
        {
            s1 = ">>>>> ERROR: invalid Rp value for torus\n";
            emit bodyControlsMessage(s1);
        }
        if (tmpBody.parameters[6] < tmpBody.parameters[7])
        {
            s1 = ">>>>> WARNING: torus major radius R < Rp\n";
            emit bodyControlsMessage(s1);
        }
        if (tmpBody.parameters[8] != tmpBody.parameters[7])
        {
            s1 = ">>>>> WARNING: Rh and Rp have unequal lengths\n";
            emit bodyControlsMessage(s1);
        }

        phi = sqrt(phi);

        XYZ2 xyz;

        xyz.x = tmpBody.parameters[3] / phi;
        xyz.y = tmpBody.parameters[4] / phi;
        xyz.z = tmpBody.parameters[5] / phi;

        s1 = "Re-calculated axis of revolution for torus: \n";
        QString s2;
        s2 = QString("%1").arg(xyz.x, 8, 'f', 6);
        s1 += s2 + " ";
        s2 = QString("%1").arg(xyz.y, 8, 'f', 6);
        s1 += s2 + " ";
        s2 = QString("%1").arg(xyz.z, 8, 'f', 6);
        s1 += s2 + " ";

        emit bodyControlsMessage(s1);
    }
}

// ----------------------------------------------------------------------------
void BuildBodyControls::addSingleBodyFromList(int b)
{
    QStringList sl = md->bodyDataStringList[b];

    ++currentBodyNo;
    bodyData.bodyNo = currentBodyNo;
    QString body;
    body.setNum(currentBodyNo);

    // Set default user body number.
    bodyNumMap[currentBodyNo] = currentBodyNo;

    bodyData.bodyTag  = sl[1];
    bodyData.bodyName = sl[2];

    // Add the body declaration as the parent node to the tree view.
    QList<QStandardItem *>  preparedRow =
                            prepareParentRow(body, bodyData.bodyTag, bodyData.bodyName);
    QStandardItem *item2 = bodyModel->invisibleRootItem();
    // adding a row to the invisible root item produces a root element
    item2->appendRow(preparedRow);

    // Extract the body data from the string list.
    bodyData.limit = (sl.size() - 3) / 2;

    int k = 3;
    for (int i=0; i<bodyData.limit; ++i)
    {
        bodyData.parameterTags[i]   = sl[k];
        bodyData.parameterValues[i] = sl[k+1];
        k += 2;
    }

    // Add the body parameters as children.
    for (unsigned char i=0; i<bodyData.limit; ++i)
    {
        QList<QStandardItem *> secondRow =
                               prepareChildRow(body, bodyData.parameterTags[i], bodyData.parameterValues[i]);
        // adding a row to an item starts a subtree
        preparedRow.first()->appendRow(secondRow);
    }

    QString s1 = "Adding Body: " + body + " " + bodyData.bodyName + " " + bodyData.bodyTag + "\n";

    CGBodyBasics tmpBody;
    tmpBody.parameters.clear();
    tmpBody.bodyNumber = body.toInt();
    tmpBody.bodyName   = bodyData.bodyName;
    tmpBody.bodyType   = typeMap.bodyNumMap[bodyData.bodyTag];
    double value;
    for (unsigned char i=0; i<bodyData.limit; ++i)
    {
        value = bodyData.parameterValues[i].toDouble();
        tmpBody.parameters.push_back(value);
        s1 += bodyData.parameterValues[i] + " ";
    }
    buildBodies->push_back(tmpBody);
    emit bodyControlsMessage(s1);

    bodyTreeView->setSortingEnabled(true);
}

// ----------------------------------------------------------------------------
void BuildBodyControls::addExistingBodies()
{
    for(int b=0; b < (int) md->bodyDataStringList.size(); ++b)
    {
        addSingleBodyFromList(b);
    }
}

// ----------------------------------------------------------------------------
void BuildBodyControls::addBodyTreeChild(QTreeWidgetItem *parent, QString body, QString tag, QString value)
{
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();
    treeItem->setText(0, body);
    treeItem->setText(1, tag);
    treeItem->setText(2, value);
    treeItem->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);

    parent->addChild(treeItem);
}
// ----------------------------------------------------------------------------
void BuildBodyControls::buildBodyNameLayout()
{
    QLabel *name = new QLabel("Body Name: ");

    nameLineEdit = new QLineEdit;
    nameLineEdit->setToolTip("Enter short descriptive name for the body to add.");
    connect(nameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(handleBodyNameEdit(QString)));
    nameLineEdit->setMinimumWidth(200);

    bodyNameLayout = new QHBoxLayout;
    bodyNameLayout->addWidget(name);
    bodyNameLayout->addWidget(nameLineEdit);
    bodyNameLayout->addStretch();
}
// ----------------------------------------------------------------------------
void BuildBodyControls::handleBodyNameEdit(QString s)
{
    bodyData.bodyName = s;
}
// ----------------------------------------------------------------------------
void BuildBodyControls::buildBodyTreeViewLayout()
{
    bodyModel = new QStandardItemModel;

    QStringList hHeaderList;
    hHeaderList << QStringLiteral("Body #") << QStringLiteral("Mnemonic") << QStringLiteral("Value");
    bodyModel->setHorizontalHeaderLabels(hHeaderList);

    connect(bodyModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(bodyModelItemChanged(QStandardItem*)));

    bodyTreeView = new QTreeView;
    bodyTreeView->setModel(bodyModel);
    bodyTreeView->expandAll();

#ifdef USING_WINDOWS
    bodyTreeView->setColumnWidth(0,  80);
    bodyTreeView->setColumnWidth(1, 120);
#else
    bodyTreeView->setColumnWidth(0, 70);
    bodyTreeView->setColumnWidth(1, 70);
#endif

    bodyTreeView->sortByColumn(0, Qt::AscendingOrder);
    bodyTreeView->setSortingEnabled(true);
    bodyTreeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(bodyTreeView, SIGNAL(customContextMenuRequested(const QPoint &)),
            this,         SLOT(onCustomContextMenu2(const QPoint &)));

    bodyTreeView->setMinimumSize(250, 200);

    bodyTreeViewLayout = new QVBoxLayout;
//    bodyTreeLayout->addWidget(topLabel);
    bodyTreeViewLayout->addWidget(bodyTreeView);
}

// ----------------------------------------------------------------------------
void BuildBodyControls::updateBuildBodies(QStandardItem *item)
{
    QStandardItem *child;
    double value;

    int bodyNo = item->parent()->data(0).toInt();

    //qDebug() << "updateBuildBodies   :: data(1) : " << item->parent()->data(1);
    //qDebug() << "updateBuildBodies   :: data(2) : " << item->parent()->data(2);

    int bIndex = -1;
    for(int k=0; k < (int) buildBodies->size(); ++k)
    {
        if (bodyNo == (*buildBodies)[k].bodyNumber)
        {
//            qDebug() << "bodyNo " << bodyNo << "\t found: " << k << endl;

            for ( int i = 0; i < item->parent()->rowCount(); ++i )
            {
                child = item->parent()->child(i, 2);
                value = child->data(0).toDouble();
                (*buildBodies)[k].parameters[i] = value;
//                qDebug() << "Row: " << i << "\tchild data: " << child->data(0);
            }
            bIndex = k;
            break;
        }
    }

    QString body;
    QString param;
    body.setNum(bodyNo);
    for(int k=0; k < (int) md->bodyDataStringList.size(); ++k)
    {
        QStringList sl = md->bodyDataStringList[k];
        if (body == sl[0])
        {
//            qDebug() << "body number found in bodyDataStringList: " << body;
//            qDebug() << "Size of sl: " << sl.size();

            int m = 0;
            for(int i=4; i < (int) sl.size(); i += 2)
            {
                param.setNum( (*buildBodies)[bIndex].parameters[m], 'e', 10 );
                sl[i] = param;
                ++m;
            }
//            qDebug() << "new list: " << sl;
            md->bodyDataStringList.replace(k, sl);
            break;
        }
    }
}

// ----------------------------------------------------------------------------
void BuildBodyControls::updateBuildBodies(int oldBody, int column, QStandardItem *item)
{
    //qDebug() << "updateBuildBodies2   :: data(0) : " << item->data(0);
    //qDebug() << "updateBuildBodies2   :: data(1) : " << item->data(1);
    //qDebug() << "updateBuildBodies2   :: data(2) : " << item->data(2);

    if (column == 2)
    {
        //qDebug() << "changing the body name\t oldBody: " << oldBody << endl;

        QStandardItem *it = bodyModel->item(oldBody, 0);
        int        bodyNo = it->data(0).toInt();

        for(int k=0; k<buildBodies->size(); ++k)
        {
            if (bodyNo == (*buildBodies)[k].bodyNumber)
            {
                (*buildBodies)[k].bodyName = item->data(0).toString();
                //qDebug() << "new name: " << buildBodies[k].bodyName  << endl;
            }
        }
    }
    else
    {
        int bodyNo = item->data(0).toInt();
        //qDebug() << "bodyNo : " << bodyNo << "\t oldBody: " << oldBody << endl;

        for(int k=0; k<buildBodies->size(); ++k)
        {
            if (oldBody == (*buildBodies)[k].bodyNumber)
            {
                (*buildBodies)[k].bodyNumber = bodyNo;
                //qDebug() << "new bodyNumber: " << buildBodies[k].bodyNumber  << endl;
            }
        }
    }
}

// ----------------------------------------------------------------------------
void BuildBodyControls::bodyModelItemChanged(QStandardItem *bodyModelItem)
{
    // Does execution of this function update the body and model 3-D graphics?

    //qDebug() << "bodyModelItemChanged:: data        :  " << bodyModelItem->data(0) << endl;
    //qDebug() << "bodyModelItemChanged:: row          :  " << bodyModelItem->row() << endl;

    //qDebug() << "bodyModelItemChanged:: parent       :  " << bodyModelItem->parent();
    //qDebug() << "bodyModelItemChanged:: has children :  " << bodyModelItem->hasChildren();

    QStandardItem *child;

    if (bodyModelItem->parent() != nullptr)
    {
        // Child rows changed.
        updateBuildBodies(bodyModelItem);
        QStandardItem *itemChanged = bodyModelItem;
        emit bodyChangedSignal();
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
        //qDebug() << "parent data for body declaration    : " ;
        //qDebug() << "row count: " << bodyModelItem->rowCount();
        //qDebug() << "row #    : " << bodyModelItem->row();
        //qDebug() << "col #    : " << bodyModelItem->column();

        /*
        if (bodyTreeView->rootIndex().isValid() )
        {
            QModelIndex qm = bodyTreeView->rootIndex();
            qDebug << "tree root: row: " << qm.row();
        }
        */

        //qDebug() << "bodyModel rowCount: " << bodyModel->rowCount();

        if (bodyModelItem->column() == 0)
        {
            int pBody = bodyModelItem->data(0).toInt() ;                // parent body no.
            int cBody = bodyModelItem->child(0, 0)->data(0).toInt();    // child body no.

            // Is the user body number number already in use?
            if (bodyNumMap.value(pBody, -1) > -1)
            {
                // Re-set body number before issuing error message.
                QString s1 = bodyModelItem->child(0, 0)->data(0).toString();

                bodyTreeView->blockSignals(true);
                bodyModel->blockSignals(true);
                QStandardItem *si2 = bodyModel->item(bodyModelItem->row(), 0);
                si2->setText(s1);
                bodyModel->blockSignals(false);
                bodyTreeView->blockSignals(false);

                QMessageBox msg;
                QString str = "body number is already in use.  Please select another.\n";
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
                int k = bodyNumMap[cBody];
                bodyNumMap[pBody] = k;
                bodyNumMap.remove(cBody);

                // Re-set the children to the new user body number.
                bodyTreeView->blockSignals(true);
                bodyModel->blockSignals(true);
                for ( int i = 0; i < bodyModelItem->rowCount(); ++i )
                {
                    child = bodyModelItem->child(i, 0);
                    //qDebug() << "-- Row: " << i << "\tchild data: " << child->data(0);
                    child->setData(bodyModelItem->data(0) , 0);
                }
                bodyModel->blockSignals(false);
                bodyTreeView->blockSignals(false);
                emit bodyNumberSignal(cBody, pBody);

                updateBuildBodies(cBody, bodyModelItem->column(), bodyModelItem);
            }
        }                   // -- end parent column stuff
        else if (bodyModelItem->column() == 2)
        {
            //qDebug() << "column 2 " << bodyModelItem->data(0);

            updateBuildBodies(bodyModelItem->row(), bodyModelItem->column(), bodyModelItem);
        }
    }
}

// ----------------------------------------------------------------------------
QList<QStandardItem *> BuildBodyControls::prepareChildRow(const QString &first,
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
QList<QStandardItem *> BuildBodyControls::prepareParentRow(const QString &first,
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
void BuildBodyControls::onCustomContextMenu2(const QPoint &point)
{
    QModelIndex index = bodyTreeView->indexAt(point);
    if (index.isValid() )
    {
        QStandardItem *itemClicked = bodyModel->itemFromIndex(index);
//        qDebug() << "itemClicked: " << itemClicked->parent()->data();

        if (index.column() > 0)
        {
            QMessageBox about;
            QString str = "Please right click on body column for menu.\n";
            about.setIcon(QMessageBox::Information);
            about.setInformativeText(str);
            about.setStandardButtons(QMessageBox::Ok);
            about.setDefaultButton(QMessageBox::Ok);
            about.show();
            about.exec();
            return;
        }

//        QStandardItem *itemClicked = bodyModel->itemFromIndex(index);
        //qDebug() << "itemClicked rowCount: " << itemClicked->rowCount();

        QMenu contextMenu;
        QAction *andPlusAction   = contextMenu.addAction(tr("+ AND"));
        QAction *andMinusAction  = contextMenu.addAction(tr("- AND"));
        QAction *orPlusAction    = contextMenu.addAction(tr("+ OR"));
        QAction *orMinusAction   = contextMenu.addAction(tr("- OR"));
        QAction *orDeleteAction  = contextMenu.addAction(tr("DELETE"));
        QAction *queryAction     = contextMenu.addAction(tr("QUERY"));

        QAction *action = contextMenu.exec(bodyTreeView->viewport()->mapToGlobal(point));

        int actionType;

        if (action == andPlusAction)
        {
            //qDebug() << "+ ADD :: item: " << index.data(0) << index.data(1);
            actionType = ANDPLUS;
            emit bodyAddSignal(actionType, itemClicked);
        }
        else if (action == andMinusAction)
        {
            //qDebug() << "- ADD :: item: " << index.data(0) << index.data(1);
            actionType = ANDMINUS;
            emit bodyAddSignal(actionType, itemClicked);
        }
        else if (action == orPlusAction)
        {
            //qDebug() << "+ OR :: item: " << index.data(0) << index.data(1);
            actionType = ORPLUS;
            emit bodyAddSignal(actionType, itemClicked);
        }
        else if (action == orMinusAction)
        {
            //qDebug() << "- OR :: item: " << index.data(0) << index.data(1);
            actionType = ORMINUS;
            emit bodyAddSignal(actionType, itemClicked);
        }
        else if (action == orDeleteAction)
        {
            actionType = SSS_DEL;
            emit bodyAddSignal(actionType, itemClicked);

 //           qDebug() << "DELETE :: item: " << index.data(0) << index.data(1);

            // 1st delete child rows then parent.
//            bodyModel->removeRows(itemClicked->row(), itemClicked->rowCount(), index);
//            bodyModel->removeRow(itemClicked->row());

            // The order of the following statements is important!
            QString s = index.data(0).toString();       // (1)
            removeFromBodyDataStringList(s);            // (2)
            int bodyNo = index.data(0).toInt();         // (3)
            resetBodyTree();                            // (4)
            renumberBodyDataStringList(bodyNo);         // (5)
            emit(bodyRenumberRequest(bodyNo));          // (6)
        }
        else if (action == queryAction)
        {
//            actionType = QUERY;
            int bodyNo = index.data(0).toInt();
            emit bodyQueryRequest(bodyNo);
        }

        /*
         * This next cast sequence was a pain to find!
         *
        const QStandardItemModel *model = static_cast<QStandardItemModel * >
                 (const_cast<QAbstractItemModel *> (index.model() ));

        QStandardItem *sItem = model->itemFromIndex(index);
        */
    }
}

// ----------------------------------------------------------------------------
void BuildBodyControls::resetBodyTree()
{
    currentBodyNo = 0;
    bodyData.clear();
    bodyNumMap.clear();
    bodyModel->clear();
    buildBodies->clear();

    QStringList hHeaderList;
    hHeaderList << QStringLiteral("Body #") << QStringLiteral("Mnemonic") << QStringLiteral("Value");
    bodyModel->setHorizontalHeaderLabels(hHeaderList);

    addExistingBodies();
}
// ----------------------------------------------------------------------------
void BuildBodyControls::removeFromBodyDataStringList(QString body)
{
    // Given a body number, remove the QStringList from the master data vector.
    QStringList sl;

    for(int i=0; i < (int) md->bodyDataStringList.size(); ++i)
    {
        sl = md->bodyDataStringList[i];
        if (body == sl[0])
        {
            md->bodyDataStringList.removeAt(i);
            break;
        }
    }
}
// ----------------------------------------------------------------------------
void BuildBodyControls::renumberBodyDataStringList(int bodyNum)
{
    // Given a body number (that has been removed), renumber the existing
    // body numbers that are greater thanit.
    QString sb;
    QStringList sl;
    int body;

    for(int i=0; i < (int) md->bodyDataStringList.size(); ++i)
    {
        sl = md->bodyDataStringList[i];
        body = sl[0].toInt();
        if (body > bodyNum)
        {
            sb.setNum(body - 1);
            sl[0] = sb;
            md->bodyDataStringList[i] = sl;
        }
    }
}
// ----------------------------------------------------------------------------
void BuildBodyControls::handleBodyNumberQueryRequest(QString msg)
{
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setInformativeText(msg);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.show();
    msgBox.exec();
}
// ----------------------------------------------------------------------------
void BuildBodyControls::closeFileOperations()
{

}
