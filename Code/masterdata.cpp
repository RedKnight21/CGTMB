#include "masterdata.h"

MasterData::MasterData()
{

}
// ----------------------------------------------------------------------------
MasterData::~MasterData()
{
    bodyDataStringList.empty();
    cellDataStringList.empty();
    mtlDataStringList.empty();
    customZaidIntList.empty();
    abaqusFileStringList.empty();
    umeshDataStringList.empty();
    instanceDataStringList.empty();
    usedUniverseNumbers.empty();
    umObjNameUniverse.empty();
    universeDataStringList.empty();
    ptracEventColors.empty();
    ptracBatchesStringList.empty();
}
// ----------------------------------------------------------------------------
void MasterData::clear()
{
    projectTitle        = "";
    displayRuler        = 1;
    displayImportances  = 1;
    displayBodyComments = 1;
    displayCellComments = 1;
    displayMtlComments  = 1;
    inputLineWidth      = 80;
    customZaidListType  = 0;
    lastUniverseNumber  = 0;
    projectType  = NOPROJECT;
    bodyDataStringList.clear();
    cellDataStringList.clear();
    mtlDataStringList.clear();
    customZaidIntList.clear();
    abaqusFileStringList.clear();
    umeshDataStringList.clear();
    instanceDataStringList.clear();
    usedUniverseNumbers.clear();
    umObjNameUniverse.clear();
    universeDataStringList.clear();
    ptracBatchesStringList.clear();
    setDefaults();
}
// ----------------------------------------------------------------------------
void MasterData::setDefaults()
{
    ptracEventColors.clear();
    QColor c = QColor("red");
    ptracEventColors.push_back(c);
    c = QColor("cyan");
    ptracEventColors.push_back(c);
    c = QColor("green");
    ptracEventColors.push_back(c);
    c = QColor("yellow");
    ptracEventColors.push_back(c);
    c = QColor("black");
    ptracEventColors.push_back(c);

    // Ghost geometry color
    c.setRed(255);
    c.setGreen(253);
    c.setBlue(208);
    c.setAlpha(200);
    ptracEventColors.push_back(c);

#ifdef USING_WINDOWS
    ptracEventPointSize = 6;
#else
    ptracEventPointSize = 10;
#endif
}
// ----------------------------------------------------------------------------
bool MasterData::saveToFile(QString fName)
{
    int     sListSize;
    QString s1;
    QStringList sl;

//    if (cellDataStringList.size() < 1) return false;

//    qDebug() << "MasterData::saveToFile: " << fName;

//    qDebug() << "Size of bodyDataStringList = " << bodyDataStringList.size() << endl;

    if (!fName.isEmpty())
    {
        QFile file(fName);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream fout(&file);

        fout << "CGTMB PROJECT FILE" << "\n";
        fout << "PROJECT-TYPE  : " << projectType               << "\n";
        fout << "PROJECT-TITLE : " << projectTitle              << "\n";
        fout << "PARTICLE      : " << particleType              << "\n";
        fout << "IMPORTANCES   : " << displayImportances        << "\n";
        fout << "BODY-COMMENTS : " << displayBodyComments       << "\n";
        fout << "CELL-COMMENTS : " << displayCellComments       << "\n";
        fout << "MTL-COMMENTS  : " << displayMtlComments        << "\n";
        fout << "RULER         : " << displayRuler              << "\n";
        fout << "LINE-LENGTH   : " << inputLineWidth            << "\n";
        QString type = QSysInfo::productType();
        if (type == "windows")
            fout << "XSDIRFILE     : win : " << XSDIRfile                 << "\n";
        else
            fout << "XSDIRFILE     : nix : " << XSDIRfile                 << "\n";

        fout << "CUSTOM ZAID ID: " << customZaidListType        << "\n";
        fout << "LAST UNIVERSE : " << lastUniverseNumber        << "\n";

        fout << "BODIES        : " << bodyDataStringList.size() << "\n";
        for (int b=0; b < (int) bodyDataStringList.size(); ++b)
        {
            sl = bodyDataStringList[b];
            sListSize = sl.size();
            for (int i=0; i<sListSize; ++i)
            {
                s1 = sl[i];
                fout << s1 << ";" ;  // Don't add extra spaces between entries.
            }
            fout << "\n";
        }

        fout << "CELLS        : " << cellDataStringList.size() << "\n";
        for (int b=0; b < (int) cellDataStringList.size(); ++b)
        {
            sl = cellDataStringList[b];
            sListSize = sl.size();
            for (int i=0; i<sListSize; ++i)
            {
                s1 = sl[i];
                fout << s1 << ";" ;  // Don't add extra spaces between entries.
            }
            fout << "\n";
        }

        fout << "MATERIALS    : " << mtlDataStringList.size() << "\n";

        for (int b=0; b < (int) mtlDataStringList.size(); ++b)
        {
            sl = mtlDataStringList[b];
            sListSize = sl.size();
            for (int i=0; i<sListSize; ++i)
            {
                s1 = sl[i];
                fout << s1 << ";" ;  // Don't add extra spaces between entries.
            }
            fout << "\n";
        }

        fout << "CUSTOM ZAID LIST: " << customZaidIntList.size() << "\n";
        for (int b=0; b < (int) customZaidIntList.size(); ++b)
        {
            s1.setNum(customZaidIntList[b]);
            fout << s1 << ";" ;  // Don't add extra spaces between entries.
        }
        if (customZaidIntList.size() > 0) fout << "\n";

        fout << "ABAQUS FILE LIST: " << abaqusFileStringList.size() << "\n";

        for (int b=0; b < (int) abaqusFileStringList.size(); ++b)
        {
            if (type == "windows")
                fout << "ABAQUS FILE     : win : " << abaqusFileStringList[b]                 << "\n";
            else
                fout << "ABAQUS FILE     : nix : " << abaqusFileStringList[b]                 << "\n";
        }

        fout << "INSTANCE DATA LIST: " << instanceDataStringList.size() << "\n";
        for (int b=0; b < (int) instanceDataStringList.size(); ++b)
        {
            sl = instanceDataStringList[b];
            sListSize = sl.size();
            for (int i=0; i<sListSize; ++i)
            {
                s1 = sl[i];
                fout << s1 << ";" ;  // Don't add extra spaces between entries.
            }
            fout << "\n";
        }

        fout << "UNIVERSE DATA LIST: " << universeDataStringList.size() << "\n";
        for (int b=0; b < (int) universeDataStringList.size(); ++b)
        {
            sl = universeDataStringList[b];
            sListSize = sl.size();
            for (int i=0; i<sListSize; ++i)
            {
                s1 = sl[i];
                fout << s1 << ";" ;  // Don't add extra spaces between entries.
            }
            fout << "\n";
        }

        fout << "MESH NAME MAP: " << umObjNameUniverse.size() << "\n";
        QMap<QString, QString>::const_iterator it = umObjNameUniverse.begin();
        while( it != umObjNameUniverse.constEnd() )
        {
            fout << it.key() << ";" << it.value() << "\n";
            ++it;
        }

        if (ptracFile.length() > 0)
        {
            if (type == "windows")
                fout << "PTRAC FILE     : win : " << ptracFile << "\n";
            else
                fout << "PTRAC FILE     : nix : " << ptracFile << "\n";
        }

        fout << "PTRAC BATCHES    : " << ptracBatchesStringList.size() << "\n";

        for (int b=0; b < (int) ptracBatchesStringList.size(); ++b)
        {
            sl = ptracBatchesStringList[b];
            sListSize = sl.size();
            for (int i=0; i<sListSize; ++i)
            {
                s1 = sl[i];
                fout << s1 << ";" ;  // Don't add extra spaces between entries.
            }
            fout << "\n";
        }

        fout << "PTRAC COLORS    : " << ptracEventColors.size() << "\n";

        QVector4D c;
        for (int b=0; b < (int) ptracEventColors.size(); ++b)
        {
            c[0] = ptracEventColors[b].red();
            c[1] = ptracEventColors[b].green();
            c[2] = ptracEventColors[b].blue();
            c[3] = ptracEventColors[b].alpha();

            for (int i=0; i < 4; ++i)
            {
                fout << c[i] << ";" ;  // Don't add extra spaces between entries.
            }
            fout << "\n";
        }
        fout << "PTRAC ENERGY    : " << ptracEnergyStringList.size() << "\n";

        for (int b=0; b < (int) ptracEnergyStringList.size(); ++b)
        {
            sl = ptracEnergyStringList[b];
            sListSize = sl.size();
            for (int i=0; i<sListSize; ++i)
            {
                s1 = sl[i];
                fout << s1 << ";" ;  // Don't add extra spaces between entries.
            }
            fout << "\n";
        }

        fout << "PTRAC POINT SIZE : " << ptracEventPointSize << "\n";


        fout << "END CGMTB PROJECT FILE" << "\n";
        file.close();
    }
    return true;
}
// ----------------------------------------------------------------------------
bool MasterData::readFromFile(QString fName)
{
    bool retVal = false;

    if (!fName.isEmpty())
    {
        QString     line;
        QStringList fields;
        QFile file(fName);

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
            return retVal;
        }

        bodyDataStringList.clear();
        cellDataStringList.clear();
        mtlDataStringList.clear();

        QTextStream in(&file);
        line   = in.readLine();
        if (!line.startsWith("CGTMB PROJECT FILE"))
        {
            QMessageBox about;
            QString str = ": Not a CGTMB Project file.\n";
            str = fName + str;
            about.setIcon(QMessageBox::Critical);
            about.setInformativeText(str);
            about.setStandardButtons(QMessageBox::Ok);
            about.setDefaultButton(QMessageBox::Ok);
            about.setBaseSize(400, 150);
            about.show();
            about.exec();
            return retVal;
        }
        while(!in.atEnd())
        {
            line = in.readLine();
            if (line.startsWith("  ")) continue;
            if (line.startsWith("PROJECT-TYPE"))
            {
                fields = line.split(':');
                projectType = fields[1].toInt();
            }
            else if (line.startsWith("PROJECT-TITLE"))
            {
                fields = line.split(':');
                projectTitle = fields[1].trimmed();
            }
            else if (line.startsWith("PARTICLE"))
            {
                fields = line.split(':');
                particleType = fields[1].toInt();
            }
            else if (line.startsWith("IMPORTANCES"))
            {
                fields = line.split(':');
                displayImportances = fields[1].toInt();
            }
            else if (line.startsWith("BODY-COMMENTS"))
            {
                fields = line.split(':');
                displayBodyComments = fields[1].toInt();
            }
            else if (line.startsWith("CELL-COMMENTS"))
            {
                fields = line.split(':');
                displayCellComments = fields[1].toInt();
            }
            else if (line.startsWith("MTL-COMMENTS"))
            {
                fields = line.split(':');
                displayMtlComments = fields[1].toInt();
            }
            else if (line.startsWith("RULER"))
            {
                fields = line.split(':');
                displayRuler = fields[1].toInt();
            }
            else if (line.startsWith("LINE-LENGTH"))
            {
                fields = line.split(':');
                inputLineWidth = fields[1].toInt();
            }
            else if (line.startsWith("XSDIRFILE"))
            {
                fields = line.split(':');

#ifdef USING_WINDOWS
                if (fields[1].trimmed() == "win"  &&  fields.size() == 4)
                {
                    XSDIRfile = fields[2].trimmed() + ":" + fields[3].trimmed();
                }
#else
                if (fields[1].trimmed() == "nix"   &&  fields.size() == 3)
                {
                    XSDIRfile = fields[2].trimmed();
                }
#endif
            }
            else if (line.startsWith("CUSTOM ZAID ID"))
            {
                fields = line.split(':');
                customZaidListType = fields[1].toInt();
            }
            else if (line.startsWith("BODIES"))
            {
                fields = line.split(':');
                int nBodies = fields[1].toInt();
                for(int n=0; n<nBodies; ++n)
                {
                    line   = in.readLine();
                    fields = line.split(';');
                    fields.removeLast();
        //            qDebug() << "size of BODY fields: " << fields.size() << endl;
                    bodyDataStringList.push_back(fields);
                }
            }
            else if (line.startsWith("CELLS"))
            {
                fields = line.split(':');
                int nCells = fields[1].toInt();
                for(int n=0; n<nCells; ++n)
                {
                    line   = in.readLine();
                    fields = line.split(';');
                    fields.removeLast();
        //            qDebug() << "size of CELL fields: " << fields.size() << endl;
                    cellDataStringList.push_back(fields);
                }
            }
            else if (line.startsWith("MATERIALS"))
            {
                fields = line.split(':');
                int nMtls = fields[1].toInt();

                for(int n=0; n<nMtls; ++n)
                {
                    line   = in.readLine();
                    fields = line.split(';');
                    fields.removeLast();
//                    qDebug() << "size of MTL fields: " << fields.size() << endl;
                    mtlDataStringList.push_back(fields);
                }
            }
            else if (line.startsWith("CUSTOM ZAID LIST"))
            {
                fields = line.split(':');
                int nZaids = fields[1].toInt();
                if (nZaids > 0)
                {
                    line   = in.readLine();
                    fields = line.split(';');
                    fields.removeLast();

                    for(int n=0; n<nZaids; ++n)
                    {
                        customZaidIntList.push_back(fields[n].toInt());
  //                  qDebug() << "custom:: n: " << n << "\t Z: " << customZaidIntList[n];
                    }
                }
            }
            else if (line.startsWith("ABAQUS FILE LIST"))
            {
                fields    = line.split(':');
                int nFile = fields[1].toInt();

                for(int n=0; n<nFile; ++n)
                {
                    line      = in.readLine();
                    fields    = line.split(':');
#ifdef USING_WINDOWS
                    if (fields[1].trimmed() == "win"  &&  fields.size() == 4)
                    {
                        QString s = fields[2] + ":" + fields[3];
                        abaqusFileStringList.push_back(s);
                    }
#else
                    if ((fields[1].trimmed() == "nix" || fields[1].trimmed() == "inx")  &&  fields.size() == 3)
                    {
                        abaqusFileStringList.push_back(fields[2]);
                    }
#endif
//                    fields.removeLast();
                }
            }
            else if (line.startsWith("INSTANCE DATA LIST"))
            {
                fields = line.split(':');
                int nInsts = fields[1].toInt();
                for(int n=0; n<nInsts; ++n)
                {
                    line   = in.readLine();
                    fields = line.split(';');
                    fields.removeLast();
                    instanceDataStringList.push_back(fields);
                }
            }
            else if (line.startsWith("UNIVERSE DATA LIST"))
            {
                fields = line.split(':');
                int nInsts = fields[1].toInt();
                for(int n=0; n<nInsts; ++n)
                {
                    line   = in.readLine();
                    fields = line.split(';');
                    fields.removeLast();
                    universeDataStringList.push_back(fields);
                }
            }
            else if (line.startsWith("MESH NAME MAP"))
            {
                fields = line.split(':');
                int nObjs = fields[1].toInt();
                for(int n=0; n<nObjs; ++n)
                {
                    line   = in.readLine();
                    fields = line.split(';');
                    umObjNameUniverse.insert(fields[0].trimmed(), fields[1].trimmed());
                }
            }  
            else if (line.startsWith("PTRAC FILE"))
            {
                fields = line.split(':');

#ifdef USING_WINDOWS
                if (fields[1].trimmed() == "win"  &&  fields.size() == 4)
                {
                    ptracFile = fields[2].trimmed() + ":" + fields[3].trimmed();
                }
#else
                if (fields[1].trimmed() == "nix"   &&  fields.size() == 3)
                {
                    ptracFile = fields[2].trimmed();
                }
#endif
            }
            else if (line.startsWith("PTRAC BATCHES"))
            {
                fields = line.split(':');
                int nBatches = fields[1].toInt();

                for(int n=0; n<nBatches; ++n)
                {
                    line   = in.readLine();
                    fields = line.split(';');
                    fields.removeLast();
                    ptracBatchesStringList.push_back(fields);
                }
            }
            else if (line.startsWith("PTRAC COLORS"))
            {
                ptracEventColors.clear();
                QColor c;
                fields = line.split(':');
                int nColors = fields[1].toInt();
                for(int n=0; n<nColors; ++n)
                {
                    line   = in.readLine();
                    fields = line.split(';');
                    fields.removeLast();
                    c.setRed( fields[0].toInt() );
                    c.setGreen( fields[1].toInt() );
                    c.setBlue( fields[2].toInt() );
                    c.setAlpha( fields[3].toInt() );

                    ptracEventColors.push_back(c);
                }
            }
            else if (line.startsWith("PTRAC ENERGY"))
            {
                fields = line.split(':');
                int nEnergy = fields[1].toInt();

                for(int n=0; n<nEnergy; ++n)
                {
                    line   = in.readLine();
                    fields = line.split(';');
                    fields.removeLast();
                    ptracEnergyStringList.push_back(fields);
                }
            }
            else if (line.startsWith("PTRAC POINT SIZE"))
            {
                fields = line.split(':');
                ptracEventPointSize = fields[1].toInt();
            }
            else if (line.startsWith("END CGMTB"))
            {
                break;
            }
        }               // -- end while

        retVal = true;
    }                   // -- end if for fName not empty

    return  retVal;
}                       // -- end readFromFile
// ----------------------------------------------------------------------------
void MasterData::makeBodyBasics(QVector<CGBodyBasics> *bodyBasics)
{
    int     sListSize;
    double  value;
    QStringList sl;
    CGBodyBasics tmpBody;
    BodyTypeMap  tmap;

    bodyBasics->clear();

//    qDebug() << "size: " << bodyDataStringList.size() << endl;
    for (int b=0; b < (int) bodyDataStringList.size(); ++b)
    {
//        qDebug() << "b: " << b << endl;
        tmpBody.parameters.clear();

        sl = bodyDataStringList[b];
        sListSize = sl.size();

        tmpBody.bodyNumber = sl[0].toInt();
        tmpBody.bodyName   = sl[2];
        tmpBody.bodyType   = tmap.bodyNumMap[ sl[1] ];

//        printf("makeBodyBaiscs:: sl[1] <%s>]\n", sl[1].toLatin1().data());

        for (int i=4; i<sListSize; i += 2)
        {
            value = sl[i].toDouble();
            tmpBody.parameters.push_back(value);
        }
        bodyBasics->push_back(tmpBody);
    }
}
// ----------------------------------------------------------------------------
void MasterData::getCellNumbers(QVector<QString> &list)
{
    int i;
    QString name;
    QString reduced;
    list.clear();
    for(int b=0; b < (int) cellDataStringList.size(); ++b)
    {
        QStringList sl = cellDataStringList[b];
//        i = (sl[1].size() < 17) ? sl[1].size() : 16;
        i = (sl[1].size() < 20) ? sl[1].size() : 19;
        reduced = sl[1];
        reduced.truncate(i);
        name = sl[0] + " / " + reduced;
        list.push_back(name);
    }
}
// ----------------------------------------------------------------------------
void MasterData::makeCellList(QVector<CellData> *cellList)
{
    int i;
    CellData cellData;

    cellList->clear();

    for(int b=0; b < (int) cellDataStringList.size(); ++b)
    {
        QStringList sl = cellDataStringList[b];

        cellData.clear(0);

        cellData.cellNo     = sl[0];
        cellData.name       = sl[1];
        cellData.mtlNo      = sl[2];
        cellData.density    = sl[3];
        cellData.importance = sl[4];

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
//        i = sl[11].toInt();
//        cellData.yRes = i;
//        i = sl[12].toInt();
//        cellData.zRes = i;


        int nOps = sl[11].toInt();
        int k = 11;

//        qDebug() << "\nmakeCellList:: cell no: " << sl[0];

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

            zo.meshReduction = cellData.meshReduction;

            cellData.zoneOps.push_back(zo);
        }
        cellList->push_back(cellData);
    }
}
// ----------------------------------------------------------------------------
void MasterData::getAbaqusNames(QVector<QString> *list)
{
    QString name;
    list->clear();

    for(int i=0; i < (int) abaqusFileStringList.size(); ++i)
    {
        name = strippedName(abaqusFileStringList[i]);
        list->push_back(name);
    }
}
// ------------------------------------------------------------------------------------------------
QString MasterData::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
// ------------------------------------------------------------------------------------------------
bool MasterData::checkAbaqusFileName(QString &name)
{
    bool retValue = false;

    for(int i=0; i < (int) abaqusFileStringList.size(); ++i)
    {
        if (name == abaqusFileStringList[i])
        {
            retValue = true;
        }
    }
    return retValue;
}
// ------------------------------------------------------------------------------------------------

void MasterData::initialAddToInstanceDataStringList(ReadFileAbaqus *rfa, QString &objName)
{
    int         color;
    QString     unum;
    QString     s;
    QStringList sl;
    const QString one  = "1.0";
    const QString ione = "1";

    // Set a universe number that hasn't been used.
    ++lastUniverseNumber;
    for( ; ; ++lastUniverseNumber)
    {
        unum.setNum(lastUniverseNumber);
        if (!usedUniverseNumbers.contains(unum)) break;
    }
    usedUniverseNumbers.push_back(unum);
    umObjNameUniverse[objName] = unum;

    for(int i=0; i < rfa->nInstances; ++i)
    {
        sl.clear();
        sl.push_back(unum);                         // universe #
        s.setNum(i+1);
        sl.push_back(s);                            // instance # (w/i the umobject)
        s.setNum( rfa->instances[i].materialNo );
        sl.push_back(s);                            // material #
        sl.push_back(one);                          // denisty
        sl.push_back(one);                          // importance

        sl.push_back(rfa->instances[i].name);
        instanceDataStringList.push_back(sl);

        color = rfa->instances[i].color.red();
        s.setNum(color);
        sl.push_back(s);
        color = rfa->instances[i].color.green();
        s.setNum(color);
        sl.push_back(s);
        color = rfa->instances[i].color.blue();
        s.setNum(color);
        sl.push_back(s);
        color = rfa->instances[i].color.alpha();
        s.setNum(color);
        sl.push_back(s);
        sl.push_back(ione);
    }
}

// ------------------------------------------------------------------------------------------------
int MasterData::getAbaqusFileIndex(QString &name)
{
    QString fName;
    int     indx = -1;

    for (int i=0; i < (int) abaqusFileStringList.size(); ++i)
    {
        fName = strippedName(abaqusFileStringList[i]);
        if (fName.trimmed() == name.trimmed())
        {
            indx = i;
            break;
        }
    }
    return indx;
}
// ------------------------------------------------------------------------------------------------
void MasterData::removeInstanceFromList(QString &univ)
{
    QVector<QStringList>::iterator it= instanceDataStringList.begin();

    for( ; it != instanceDataStringList.end() ; )
    {
 //       QString s = it->takeFirst();
        QString s = it->first();

        //if (s == univ) qDebug() << "found it " << s;
        if (s == univ)
        {
            it = instanceDataStringList.erase(it);
        }
        else
            ++it;
    }
}
// ------------------------------------------------------------------------------------------------
int MasterData::getTypeEnergyBins(const int index, QVector<double> &bins)
{
    bins.clear();
    QStringList sl;
    sl = ptracEnergyStringList[index];

    int typ = sl[1].toInt();

    for(int i=2; i< sl.size(); ++i)
    {
        bins.push_back( sl[i].toDouble() );
    }

    return typ;
}
// ------------------------------------------------------------------------------------------------
int  MasterData::getStringListIndex(QVector<QStringList> &vsl, const QString key)
{
    QStringList sl;
    int indx;
    int vsize = (int) vsl.size();
    for(indx=0; indx < vsize; ++indx)
    {
        sl = vsl[indx];
        if (sl[0].contains(key)) break;
        if (key.contains(sl[0])) break;
    }
    if (indx >= vsize) indx = 0;
    return indx;
}
// ------------------------------------------------------------------------------------------------
void MasterData::setInstanceDisplay(int univ, int cell, int state)
{
    int c, u;

    for(int i=0; i < (int) instanceDataStringList.size(); ++i)
    {
        QStringList sl = instanceDataStringList[i];
        u = sl[0].toInt();
        c = sl[1].toInt();
        if (u == univ  &&  c == cell)
        {
            QString s;
            s.setNum(state);
            sl[10] = s;
            instanceDataStringList[i] = sl;
            break;
        }
    }
}
// ------------------------------------------------------------------------------------------------
int MasterData::getInstanceDisplay(int univ, int cell)
{
    int val, c, u;

    val = 1;
    for(int i=0; i < (int) instanceDataStringList.size(); ++i)
    {
        QStringList sl = instanceDataStringList[i];
        u = sl[0].toInt();
        c = sl[1].toInt();
        if (u == univ  &&  c == cell)
        {
            val = sl[10].toInt();
        }
    }
    return val;
}

// ------------------------------------------------------------------------------------------------
int MasterData::getInstanceCellNumberByIndex(int univ, int index)
{
    int u;
    int cell = -1;

    int count = 0;

    for(int i=0; i < (int) instanceDataStringList.size(); ++i)
    {
        QStringList sl = instanceDataStringList[i];
        u = sl[0].toInt();
        if (u == univ)
        {
            if (count == index)
            {
                cell  = sl[1].toInt();
                break;
            }
            else
                ++count;
        }
    }
    return cell;
}

// ------------------------------------------------------------------------------------------------
int MasterData::getCellNumberIndex(int cell)
{
    int     tCell;
    int     indx = -1;

    for (int i=0; i < (int) cellDataStringList.size(); ++i)
    {
        QStringList sl = cellDataStringList[i];
        tCell = sl[0].toInt();
        if (tCell == cell)
        {
            indx = i;
            break;
        }
    }
    return indx;
}

// ------------------------------------------------------------------------------------------------
void MasterData::getInstanceDataColors(std::vector<double> *r, std::vector<double> *g, std::vector<double> *b)
{

    for (auto n=0; n < instanceDataStringList.size(); ++n)
    {
        QStringList sl = instanceDataStringList[n];
        QColor color;
        color.setRed(sl[6].toInt());
        color.setGreen(sl[7].toInt());
        color.setBlue(sl[8].toInt());

        r->push_back( color.redF() );
        g->push_back( color.greenF() );
        b->push_back( color.blueF() );
    }
}
// ------------------------------------------------------------------------------------------------
void MasterData::getMeshModParameters(QString *name, double *params)
{
    int index;

    for (auto n=0; n < universeDataStringList.size(); ++n)
    {
        QStringList sl = universeDataStringList[n];
        if (name == sl[11])
        {
            index = n;
            break;
        }
    }

    QStringList sl = universeDataStringList[index];
    params[0] = sl[ 5].toDouble();
    params[1] = sl[ 6].toDouble();
    params[2] = sl[ 7].toDouble();
    params[3] = sl[ 8].toDouble();
    params[4] = sl[ 9].toDouble();
    params[5] = sl[10].toDouble();
}
// ------------------------------------------------------------------------------------------------
int MasterData::getMeshUniverseNumber(QString *name)
{
    int index = -1;

    for (auto n=0; n < universeDataStringList.size(); ++n)
    {
        QStringList sl = universeDataStringList[n];
        if (name == sl[11])
        {
            index = sl[0].toInt();
            break;
        }
    }
    return index;
}




