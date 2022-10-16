#include "cardimageview.h"

// ----------------------------------------------------------------------------
CardImageView::CardImageView(QWidget *parent, bool readFlag, bool saveFlag,
                             bool clrFlag, bool clsFlag)
    : TextViewWindow(parent, readFlag, saveFlag, clrFlag, clsFlag)
{
    ruler1  = "--*----1----*----2----*----3----*----4----*----5----*----6";
    ruler1 += "----*----7----*----8----*----9\n";

    ruler40  = "--*----1----*----2----*----3----*----4";
    ruler50  = ruler40  + "----*----5";
    ruler60  = ruler50  + "----*----6";
    ruler70  = ruler60  + "----*----7";
    ruler80  = ruler70  + "----*----8";
    ruler90  = ruler80  + "----*----9";
    ruler100 = ruler90  + "----*----0";
    ruler110 = ruler100 + "----*----1";
    ruler120 = ruler100 + "----*----2";

    ruler40  += "\n";
    ruler50  += "\n";
    ruler60  += "\n";
    ruler70  += "\n";
    ruler80  += "\n";
    ruler90  += "\n";
    ruler100 += "\n";
    ruler110 += "\n";
    ruler120 += "\n";

    doBody  = false;
    doCell  = false;
    doRuler = false;
    doImp   = false;
    doMtls  = false;

    readSettings();
}
// ----------------------------------------------------------------------------
CardImageView::~CardImageView()
{
    writeSettings();
}
// ----------------------------------------------------------------------------
void CardImageView::saveFile()
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    QString currentPath = settings.value("cardImagePath").toString();
    QString format = ".mcnp";
    QString initialPath;

    if (currentPath.isEmpty())
    {
        initialPath = QDir::currentPath() + tr("/untitled") + format;
    }
    else
    {
        initialPath = currentPath;
    }

    QString projectFileName = QFileDialog::getSaveFileName(this, tr("Save MCNP Card Image File As"),
                                                           initialPath, tr("*.mcnp;; *.*"));

    currentPath = QFileInfo(projectFileName).absolutePath();
    settings.setValue("cardImagePath", currentPath);

//    QFile outFile("projectFileName");
    //outFile.setFileName("projectFileName");

    //qDebug() << "CardImage file: " << projectFileName << "\n";

//    if (!outFile.open((QIODevice::ReadWrite | QIODevice::Text))) qDebug() << "Card image file not opened\n";

//    QTextStream out(&outFile);
    //out << fileOutputString << "\n";
//    out << "This is a test \n";
//    outFile.close();

    const char *c  = projectFileName.toStdString().c_str();
    FILE *fp;
    if ((fp = fopen(c, "w")) != NULL)
    {
        fprintf(fp, "%s\n", fileOutputString.toStdString().c_str());
    }
    fclose(fp);
}
// ----------------------------------------------------------------------------
QSize CardImageView::sizeHint() const
{
    return QSize(700, 400);
}
// ----------------------------------------------------------------------------
void CardImageView::writeSettings()
{
    //qDebug() << "\nCardImageView::writeSettings\n";
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    settings.setValue("cardImageWindow/size",     this->size());
    settings.setValue("cardImageWindow/Geometry", this->saveGeometry());
//    qDebug() << "size: " << this->size();
//    qDebug() << "geom: " << this->saveGeometry();
}
// ----------------------------------------------------------------------------
void CardImageView::readSettings()
{
    QSettings settings(QApplication::organizationName(), QApplication::applicationName());
    this->restoreGeometry(settings.value("cardImageWindow/Geometry").toByteArray());
//    resize( settings.value("cardImageWindow/size", sizeHint()).toSize() );
//    qDebug() << "size: " << this->size();
//    qDebug() << "geom: " << this->saveGeometry();
//    qDebug() << "size: " << settings.value("cardImageWindow/size", sizeHint()).toSize();
//    qDebug() << "geom: " << settings.value("cardImageWindow/Geometry").toByteArray();
}
// ----------------------------------------------------------------------------
void CardImageView::clearText()
{

}
// ----------------------------------------------------------------------------
void CardImageView::generateView(MasterData *masterData, int which)
{
    if      (which == MCNP) generateViewMcnp(masterData);
    else if (which == ITS)  generateViewIts(masterData);
    else if (which == XGEN) generateViewXgen(masterData);
}
// ----------------------------------------------------------------------------
void CardImageView::generateViewMcnp(MasterData *masterData)
{
    lineLength = masterData->inputLineWidth;
    if (masterData->displayImportances > 0) doImp = true;
    else doImp = false;

    if (masterData->displayBodyComments > 0) doBody = true;
    else doBody = false;

    if (masterData->displayCellComments > 0) doCell = true;
    else doCell = false;

    if (masterData->displayRuler > 0) doRuler = true;
    else doRuler = false;

    if (masterData->displayMtlComments > 0) doMtls = true;
    else doMtls = false;

    if (doImp)
    {
        if      (masterData->particleType == 0) impString = "IMP:N=";
        else if (masterData->particleType == 1) impString = "IMP:P=";
        else                                    impString = "IMP:E=";
    }

    int mult = 1;
    if (masterData->projectType == ITS) mult = -1;

    masterData->makeCellList(&masterCellList);
    QString sb, sc, sm, sd;
    generateCellCardImage(sc, mult, masterData);

    masterData->makeBodyBasics(&bodyBasics);
    findUsedBodies();
    generateBodyCardImage(sb);

    generateMtlCardImage(masterData, sm);
    generateMcnpEmbedCardImage(masterData, sd);

    QString tl = masterData->projectTitle;

    fileOutputString = tl + "\n" + sc + "\n" + sb + "\n" + sm +sd;

    this->displayTextString(fileOutputString, true);
}
// ----------------------------------------------------------------------------
void CardImageView::generateCellCardImage(QString &cellString, int mult,
                                          MasterData *md)
{
    QString line;
    QString line2;
    QString s1   = " ";
    QString s2   = "  ";
    QString s5   = "     ";
    QString sc   = "C ";
    QString sd   = "$ ";
    QString sopm = "-";
    QString sopp = "+";
    QString sor  = " : ";

    // Reverse signs if ITS is the project type.
    if (mult == -1)
    {
        sopm = "+";
        sopp = "-";
        sor  = " OR ";
    }

    QVector<QString> lines;
    QString tmp1;

    ruler1 = sc + ruler40;
    if (lineLength > 40)  ruler1 = sc + ruler50;
    if (lineLength > 50)  ruler1 = sc + ruler60;
    if (lineLength > 60)  ruler1 = sc + ruler70;
    if (lineLength > 70)  ruler1 = sc + ruler80;
    if (lineLength > 80)  ruler1 = sc + ruler90;
    if (lineLength > 90)  ruler1 = sc + ruler100;
    if (lineLength > 100) ruler1 = sc + ruler110;
    if (lineLength > 110) ruler1 = sc + ruler120;

    cellString  = "C  -----\n";
    cellString += "C  CELLS\n";
    if (doRuler) cellString += ruler1;
    for(int m=0; m < (int) masterCellList.size(); ++m)
    {
        tmp1  = sss_pad_beginning(5, masterCellList[m].cellNo, ' ');
        line  = tmp1 + s2;
        tmp1  = sss_pad_beginning(5, masterCellList[m].mtlNo, ' ');
        line += tmp1  + s2;
        if (masterCellList[m].density != "0"  && masterCellList[m].density != "0.0")
            line += masterCellList[m].density + s2;

        if (masterCellList[m].zoneOps.size() > 0)
        {
            if (masterCellList[m].zoneOps[0].senseOp == 1) line += sopp;
            else line += sopm;
            tmp1.setNum(masterCellList[m].zoneOps[0].bodyNum);
            line += tmp1;

            lines.clear();
            lines.push_back(line);

            for(int k=1; k < (int) masterCellList[m].zoneOps.size(); ++k)
            {
                line2 = "";

                if (masterCellList[m].zoneOps[k].boolOp == 1) line2 += sor;
                else line2 += s1;

                if (masterCellList[m].zoneOps[k].senseOp == 1) line2 += sopp;
                else line2 += sopm;
                tmp1.setNum(masterCellList[m].zoneOps[k].bodyNum);
                line2 += tmp1;
                lines.push_back(line2);
            }
        }           // -- end if for zoneOps

        line  = lines[0];
        line2 = "";
        int lLength    = line.length();
        int maxSegment = 0;
        for (int k=1; k < (int) lines.size(); ++k)
        {
            lLength += lines[k].length();

            if (lLength > lineLength)
            {
                int n = line.length();
                if (n > maxSegment) maxSegment = n;
                line   += "\n";
                line2   = s5;
                line2  += lines[k];
                lLength = line2.length();
            }
            else
            {
                line2   += lines[k];
                lLength  = line2.length();
            }
        }                       // -- end k-loop over lines

        int n = line2.length();
        if (n < maxSegment)
        {
            n = maxSegment - n;
            for(int k=0; k<n; ++k) line2.append(s1);
        }
        line += line2;

        if (doImp)
        {
            line  = sss_pad_end(lineLength - 10, line, ' ');
            line += impString;
            line += masterCellList[m].importance;
        }

        if (doCell)
        {
            line  = sss_pad_end(lineLength, line, ' ');
            line += sd;
            line += masterCellList[m].name;
        }

        // Next two lines are the last two for building the line.
        line += "\n";
        cellString += line;

    }               // -- end m-loop over cells

    // ------------------------------------------------------------------------
    if (md->instanceDataStringList.size() > 0)
    {

        tmp1  = "C  ------------\n";
        tmp1 += "C  PSEUDO-CELLS\n";
        cellString += tmp1;
        if (doRuler) cellString += ruler1;
    }
    for(int i=0; i < (int) md->instanceDataStringList.size(); ++i)
    {
        QStringList sl = md->instanceDataStringList[i];

        tmp1  = sss_pad_beginning(5, sl[1], ' ');
        line  = tmp1 + s2;

        int m = sl[2].toInt();
        if (m > 0)
        {
            tmp1  = sss_pad_beginning(5, sl[2], ' ');
            line += tmp1  + s5;
            line += sl[3];
        }
        else
        {
            tmp1  = sss_pad_beginning(5, "0", ' ');
            line += tmp1  + s5;
        }
        line = sss_pad_end(lineLength - 30, line, ' ');

        tmp1 = "0   U=" + sl[0];
        line += tmp1;

        if (doImp)
        {
            line  = sss_pad_end(lineLength - 10, line, ' ');
            line += impString;
            line += sl[4];
        }

        if (doCell)
        {
            line  = sss_pad_end(lineLength, line, ' ');
            line += sd;
            line += sl[5];
        }

        // Next two lines are the last two for building the line.
        line += "\n";
        cellString += line;
    }

    // ------------------------------------------------------------------------
    if (md->instanceDataStringList.size() > 0)
    {

        tmp1  = "C  ---------\n";
        tmp1 += "C  FILL CELLS\n";
        cellString += tmp1;
        if (doRuler) cellString += ruler1;
    }

    for(int u=0; u < (int) md->universeDataStringList.size(); ++u)
    {
        QStringList sl = md->universeDataStringList[u];

        tmp1  = sss_pad_beginning(5, sl[1], ' ');
        line  = tmp1 + s5 + " 0";
        line  = sss_pad_end(lineLength - 30, line, ' ');
        line += ("-" + sl[3]);
        line += (s2 + "fill=" + sl[0]);

        if (doImp)
        {
            line  = sss_pad_end(lineLength - 10, line, ' ');
            line += impString;
            line += "1.0";
        }

        if (doCell)
        {
            line  = sss_pad_end(lineLength, line, ' ');
            line += sd;
            line += sl[4];
        }

        // Next two lines are the last two for building the line.
        line += "\n";
        cellString += line;
    }
}                   // -- generateCellCardImage

// ----------------------------------------------------------------------------
void CardImageView::findUsedBodies()
{
    int bn;

    for(int b=0; b < (int) bodyBasics.size(); ++b)
        bodyBasics[b].used = false;

    for(int m=0; m < (int) masterCellList.size(); ++m)
    {
        for(int k=0; k < (int) masterCellList[m].zoneOps.size(); ++k)
        {
            bn = masterCellList[m].zoneOps[k].bodyNum;

            for(int b=0; b < (int) bodyBasics.size(); ++b)
            {
                if (bn == bodyBasics[b].bodyNumber)
                {
                    bodyBasics[b].used = true;
                    break;
                }
            }
        }               // -- end k-loop
    }                   // -- end m-loop
}                       // -- end findUsedBodies

// ----------------------------------------------------------------------------
void CardImageView::generateBodyCardImage(QString &bodyString)
{
    int bn;
    QString line;
    QString tmp1;
    QString s2   = "  ";

    BodyNameMap bName;

    bodyString  = "C  ------\n";
    bodyString += "C  BODIES\n";
    if (doRuler) bodyString += ruler1;

    // This may need to be modified to check for input line length
    // before adding more parameters.
    for(int b=0; b < (int) bodyBasics.size(); ++b)
    {
        if (bodyBasics[b].used)
        {
            bn = bodyBasics[b].bodyNumber;
            tmp1.setNum(bn);
            line = sss_pad_beginning(5, tmp1, ' ');
            line += s2;

            if (bodyBasics[b].bodyType != ELR)
            {
                if (bodyBasics[b].bodyType == TOR)
                {
                    if (bodyBasics[b].parameters[5] > 0.9)
                        tmp1 = "TZ ";
                    else if (bodyBasics[b].parameters[4] > 0.9)
                        tmp1 = "TY ";
                    else
                        tmp1 = "TX ";

                    line += tmp1;
                    line += s2;

                    for(int p=0; p < 3; ++p)
                    {
                        // A value of 10 or more in the setNum will only yield
                        // 9 decimal digits. One value of the 10 is needed for
                        // the deicmal place!
                        tmp1.setNum(bodyBasics[b].parameters[p], 'g', 10);
                        line += tmp1;
                        line += s2;
                    }
                    for(int p=6; p < 9; ++p)
                    {
                        // A value of 10 or more in the setNum will only yield
                        // 9 decimal digits. One value of the 10 is needed for
                        // the deicmal place!
                        tmp1.setNum(bodyBasics[b].parameters[p], 'g', 10);
                        line += tmp1;
                        line += s2;
                    }
                }
                else
                {
                    tmp1 = bName.bodyNameMap[ bodyBasics[b].bodyType ];

                    line += tmp1;
                    line += s2;

                    for(int p=0; p < (int) bodyBasics[b].parameters.size(); ++p)
                    {
                        // A value of 10 or more in the setNum will only yield
                        // 9 decimal digits. One value of the 10 is needed for
                        // the deicmal place!
                        tmp1.setNum(bodyBasics[b].parameters[p], 'g', 10);
                        line += tmp1;
                        line += s2;
                    }
                }
            }
            else
            {
                // Convert ITS ELR specification to MCNP/4/5/6's specification.
                tmp1 = "ELL";
                line += tmp1;
                line += s2;

                int p;
                for(p=0; p < (int) bodyBasics[b].parameters.size()-1; ++p)
                {
                    tmp1.setNum( bodyBasics[b].parameters[p], 'g', 10 );
                    line += tmp1;
                    line += s2;
                }
                tmp1.setNum( -bodyBasics[b].parameters[p], 'g', 10);
                line += tmp1;
                line += s2;
            }

            if (doBody)
            {
                line = sss_pad_end(lineLength, line, ' ');
                line += "$ ";
                line += bodyBasics[b].bodyName;
            }
            line += "\n";
            bodyString += line;
        }
    }                   // -- end b-loop
}                       // -- end generateBodyCardImage

// ----------------------------------------------------------------------------
void CardImageView::generateMtlCardImage(MasterData *md, QString &mtlString)
{
    int k;
    int nZaids;
    int nLim;
    int nSpace;
    QString line;
    QString tmp1;
    QString s2   = "  ";
    QString s5   = "     ";
    QStringList sl;

    mtlString  = "C  ---------\n";
    mtlString += "C  MATERIALS\n";
    if (doRuler) mtlString += ruler1;

    for(int m=0; m < (int) md->mtlDataStringList.size(); ++m)
    {
        sl = md->mtlDataStringList[m];
        line  = "M" + sl[0];
        for(int i = line.length(); i < 6; ++i) line += " ";

        line  += s2;
        nZaids = sl[7].toInt();
        nLim   = (nZaids < 2) ? nZaids : 2;

        if (lineLength < 50) nLim = 1;
        for(int n=0; n < nLim; ++n)
        {
            k = 2*n;
            nSpace = 10 - sl[8+k].length();

            if (n%2 == 0) nSpace += 10;
            else nSpace += 32;
            line = sss_pad_end(nSpace, line, ' ');
            tmp1 = sl[8+k] + s2 + sl[9+k];
            line += tmp1;
        }

        if (doMtls)
        {
                line = sss_pad_end(lineLength, line, ' ');
                line += "$ ";
                tmp1  = sl[1];
                tmp1 = sss_pad_end(10, tmp1, ' ');
                line += tmp1;
                line += s2;
                line += "Density: ";
                line += sl[2];
        }
        line += "\n";
        mtlString += line;

        line = "";
        for(int n=nLim; n < nZaids; ++n)
        {
            if (n%2 == 0  ||  lineLength < 50) line = s5;

            k = 2*n;
            nSpace = 10 - sl[8+k].length();


            if (n%2 == 0  ||  lineLength < 50) nSpace += 10;
            else nSpace += 32;


            line = sss_pad_end(nSpace, line, ' ');
            tmp1 = sl[8+k] + s2 + sl[9+k];
            line += tmp1;
            if (n%2 == 1  ||  n == (nZaids-1)  ||  lineLength < 50)
            {
                line += "\n";
                mtlString += line;
            }
        }
    }                   // -- end b-loop
}                       // -- end generateMtlCardImage

// ----------------------------------------------------------------------------
void CardImageView::generateMcnpEmbedCardImage(MasterData *md, QString &embString)
{
    int len;
    int univ1, univ2;

    QString line;
    QString tmp1;
    QString s2   = "  ";
    QString s5   = "     ";

    for(int u=0; u < (int) md->universeDataStringList.size(); ++u)
    {
        embString += "C\n";
        line = "embed";
        QStringList sl = md->universeDataStringList[u];

        line += sl[0];
        line += s2;
        len = line.length();

        line += "meshgeo=abaqus\n";
        embString += line;

        line  = sss_pad_beginning(len+7, "mgeoin=", ' ');
        line += sl[11] + "\n";
        embString += line;

        line  = sss_pad_beginning(len+7, "length=", ' ');
        line += sl[5] + "\n";
        embString += line;

        line  = sss_pad_beginning(len+11, "background=", ' ');
        line += sl[2] + "\n";
        embString += line;

        univ1 = sl[0].toInt();
        int count = 0;

        for(int i=0; i < (int) md->instanceDataStringList.size(); ++i)
        {
            QStringList sli = md->instanceDataStringList[i];
            univ2 = sli[0].toInt();

            if (univ1 == univ2)
            {
                if (count == 0)
                {
                    line  = sss_pad_beginning(len+9, "matcell= ", ' ');
                    len += 9;
                }
                else
                {
                    line  = sss_pad_beginning(len, " ", ' ');
                }
                ++count;
                tmp1  = QString("%1").arg(count, 4);
                line += tmp1;
                tmp1  = sss_pad_beginning(10, sli[1], ' ');
                line += tmp1 + "\n";
                embString += line;
            }
        }                       // -- end i-loop
    }                           // -- end u-loop
}
// ----------------------------------------------------------------------------
void CardImageView::generateViewIts(MasterData *masterData)
{
    lineLength = masterData->inputLineWidth;
    if (masterData->displayImportances > 0) doImp = true;
    else doImp = false;

    if (masterData->displayBodyComments > 0) doBody = true;
    else doBody = false;

    if (masterData->displayCellComments > 0) doCell = true;
    else doCell = false;

    if (masterData->displayRuler > 0) doRuler = true;
    else doRuler = false;

    if (masterData->displayMtlComments > 0) doMtls = true;
    else doMtls = false;

    if (doImp)
    {
        if      (masterData->particleType == 0) impString = "IMP:N=";
        else if (masterData->particleType == 1) impString = "IMP:P=";
        else                                    impString = "IMP:E=";
    }

    int mult = 1;
    if (masterData->projectType == MCNP) mult = -1;

    masterData->makeCellList(&masterCellList);

    zoneMap.clear();
    materialMap.clear();
    addMeshZonesToMap(masterData);
    addCGZonesToMap(mult);

    QString sb, sc, sm;
    // Do zones before bodies.
    generateItsZoneImage2(sc);

    masterData->makeBodyBasics(&bodyBasics);
    findUsedBodies();

    generateItsBodyCardImage(sb, masterData);
    generateItsMtlCardImage(sm);

    QString tl = masterData->projectTitle;
    tl = "echo 1\ntitle\n" + tl + "\n*";

    fileOutputString = tl + "\n" + sb + "\n" + sc + "\n" + sm + "\n";

    this->displayTextString(fileOutputString, true);
}
// ----------------------------------------------------------------------------
void CardImageView::generateViewXgen(MasterData *masterData)
{
    int k;
    int numElements;
    QString s;
    QString sc   = "* ";
    QString s2   = "  ";
    QString s9   = "         ";
    QString sDensity;
    QString line;

    QString tl = masterData->projectTitle;
    tl = "title\nXGEN input for " + tl + "\n*";

    fileOutputString = tl + "\n";

    for(auto i=0; i < masterData->mtlDataStringList.size(); ++i)
    {
        QStringList sl = masterData->mtlDataStringList[i];
        sDensity = sl[2];
        numElements = sl[7].toInt();
        s = "";
        for(auto n=0; n < numElements; ++n)
        {
            k = 2 * n;
            if (n == 0)
            {
                line = "material ";
            }
            else
            {
                line = s9;
            }

            line += sl[8+k];
            line  = sss_pad_end(12, line, ' ');
            s    += line + (s2 + sl[9+k] + "\n");
        }
        s += (s2 + "density " + s2 + sDensity + "\n" + sc + "\n");

        fileOutputString += s;
    }

    this->displayTextString(fileOutputString, true);
}

// ----------------------------------------------------------------------------
void CardImageView::addMeshZonesToMap(MasterData *masterData)
{
    int univ;
    int zoneNo;
    QString s;
    QString sm;
    QString sc   = "* ";
    QString s2   = "  ";
    QString bn;
    QString zn;
    QString tmp1;

    int zoneAdder = (int) masterCellList.size();

    for(auto i=0; i < masterData->instanceDataStringList.size(); ++i)
    {
        QStringList sl = masterData->instanceDataStringList[i];
        zoneNo = zoneAdder + i + 1;
        univ   = sl[0].toInt();

        bn = QString("%1").arg( (zoneAdder + univ), 0);
        zn = QString("%1").arg( zoneNo, 0);

        s  = "Z" + zn;
        s  = sss_pad_end(9, s, ' ');
        s += bn;

        sm = " " + sl[2];

        if (doCell)
        {
            s  = sss_pad_end(lineLength, s, ' ');
            s += sc;
            tmp1  = sss_pad_beginning(5, "UM", ' ');
            s += tmp1;
            s += s2;
            s += sl[5];

            sm = sss_pad_end(lineLength, sm, ' ');
            sm += sc;
            sm += tmp1;
            sm += s2;
            sm += sl[5];
        }

        zoneMap[zoneNo]      = s;
        materialMap[zoneNo] = sm;
    }
}
// ----------------------------------------------------------------------------
void CardImageView::addCGZonesToMap(int mult)
{
    QString line;
    QString line2;
    QString sm;
    QString s1   = " ";
    QString s2   = "  ";
    QString s5   = "     ";
    QString sc   = "* ";
    QString sopm = "-";
    QString sopp = "+";
    QString sor  = " : ";
    QVector<QString> lines;
    QString tmp1;

    // Reverse signs if ITS is the project type.
    if (mult == -1)
    {
        sopm = "+";
        sopp = "-";
        sor  = " OR ";
    }

    int zoneNum;

    for(int m=0; m < (int) masterCellList.size(); ++m)
    {
        tmp1.setNum(m+1);
        tmp1  = sss_pad_end(5, tmp1, ' ');
        line  = "Z" + tmp1 + s2;

        if (masterCellList[m].zoneOps.size() > 0)
        {
            if (masterCellList[m].zoneOps[0].senseOp == 1) line += sopp;
            else line += sopm;
            tmp1.setNum(masterCellList[m].zoneOps[0].bodyNum);
            line += tmp1;

            lines.clear();
            lines.push_back(line);

            for(int k=1; k < (int) masterCellList[m].zoneOps.size(); ++k)
            {
                line2 = "";

                if (masterCellList[m].zoneOps[k].boolOp == 1) line2 += sor;
                else line2 += s1;

                if (masterCellList[m].zoneOps[k].senseOp == 1) line2 += sopp;
                else line2 += sopm;
                tmp1.setNum(masterCellList[m].zoneOps[k].bodyNum);
                line2 += tmp1;
                lines.push_back(line2);
            }
        }           // -- end if for zoneOps

        line  = lines[0];
        line2 = "";
        int lLength    = line.length();
        int maxSegment = 0;
        for (int k=1; k < (int) lines.size(); ++k)
        {
            lLength += lines[k].length();

            if (lLength > lineLength)
            {
                int n = line.length();
                if (n > maxSegment) maxSegment = n;
                line   += "\n";
                line2   = s5;
                line2  += lines[k];
                lLength = line2.length();
            }
            else
            {
                line2   += lines[k];
                lLength  = line2.length();
            }
        }                       // -- end k-loop over lines

        int n = line2.length();
        if (n < maxSegment)
        {
            n = maxSegment - n;
            for(int k=0; k<n; ++k) line2.append(s1);
        }
        line += line2;

        sm = " " + masterCellList[m].mtlNo;

        if (doCell)
        {
            line  = sss_pad_end(lineLength, line, ' ');
            line += sc;
            tmp1  = sss_pad_beginning(5, masterCellList[m].cellNo, ' ');
            line += tmp1;
            line += s2;
            line += masterCellList[m].name;

            sm = sss_pad_end(lineLength, sm, ' ');
            sm += sc;
            sm += tmp1;
            sm += s2;
            sm += masterCellList[m].name;
        }
        zoneNum              = m+1;
        zoneMap[zoneNum]     = line;
        materialMap[zoneNum] = sm;
    }                                                   // -- end m-loop over cells
}

// ----------------------------------------------------------------------------
void CardImageView::generateItsBodyCardImage(QString &bodyString, MasterData *masterData)
{
    int bn;
    QString line;
    QString tmp1;
    QString s2   = "  ";

    BodyNameMap bName;

    bodyString = "*        GEOMETRY\n*\nGEOMETRY  0  0\n*\n";
    bodyString += "* BODIES\n";
    if (doRuler) bodyString += ruler1;

    // This may need to be modified to check for input line length
    // before adding more parameters.
    for(int b=0; b < (int) bodyBasics.size(); ++b)
    {
        if (bodyBasics[b].used)
        {
            bn = bodyBasics[b].bodyNumber;
            line = " ";

            tmp1 = bName.bodyNameMap[ bodyBasics[b].bodyType ];

            line += tmp1;
            line += s2;

            for(int p=0; p < (int) bodyBasics[b].parameters.size(); ++p)
            {
                // A value of 10 or more in the setNum will only yield
                // 9 decimal digits. One value of the 10 is needed for
                // the deicmal place!
                tmp1.setNum(bodyBasics[b].parameters[p], 'g', 10);
                line += tmp1;
                line += s2;
            }

            if (doBody)
            {
                tmp1 = QString("%1").arg(bn, 4);
                line = sss_pad_end(lineLength, line, ' ');
                line += "* " + tmp1 + s2;
                line += bodyBasics[b].bodyName;
            }
            line += "\n";
            bodyString += line;
        }
    }                           // -- end b-loop

    int adder = (int) bodyBasics.size();
    QStringList sl;

    for(int m=0; m < (int) masterData->universeDataStringList.size(); ++m)
    {
        sl = masterData->universeDataStringList[m];
        line = " UME " + sl[0];

        if (doBody)
        {
            tmp1 = QString("%1").arg( (adder + m), 4);
            line = sss_pad_end(lineLength, line, ' ');
            line += "* " + tmp1 + s2;
            line += sl[4];
        }
        line += "\n";
        bodyString += line;
    }

    bodyString += "*\nEND";
}                               // -- end generateItsBodyCardImage

// ----------------------------------------------------------------------------
void CardImageView::generateItsZoneImage(QString &cellString, int mult)
{
    QString line;
    QString line2;
    QString s1   = " ";
    QString s2   = "  ";
    QString s5   = "     ";
    QString sc   = "* ";
    QString sopm = "-";
    QString sopp = "+";
    QString sor  = " : ";

    // Reverse signs if ITS is the project type.
    if (mult == -1)
    {
        sopm = "+";
        sopp = "-";
        sor  = " OR ";
    }

    QVector<QString> lines;
    QString tmp1;

    ruler1 = sc + ruler40;
    if (lineLength > 40)  ruler1 = sc + ruler50;
    if (lineLength > 50)  ruler1 = sc + ruler60;
    if (lineLength > 60)  ruler1 = sc + ruler70;
    if (lineLength > 70)  ruler1 = sc + ruler80;
    if (lineLength > 80)  ruler1 = sc + ruler90;
    if (lineLength > 90)  ruler1 = sc + ruler100;
    if (lineLength > 100) ruler1 = sc + ruler110;
    if (lineLength > 110) ruler1 = sc + ruler120;

    cellString  = "*\n";
    cellString += "*  ZONES\n";
    if (doRuler) cellString += ruler1;
    for(int m=0; m < (int) masterCellList.size(); ++m)
    {
        tmp1.setNum(m+1);
        tmp1  = sss_pad_end(5, tmp1, ' ');
        line  = "Z" + tmp1 + s2;

        if (masterCellList[m].zoneOps.size() > 0)
        {
            if (masterCellList[m].zoneOps[0].senseOp == 1) line += sopp;
            else line += sopm;
            tmp1.setNum(masterCellList[m].zoneOps[0].bodyNum);
            line += tmp1;

            lines.clear();
            lines.push_back(line);

            for(int k=1; k < (int) masterCellList[m].zoneOps.size(); ++k)
            {
                line2 = "";

                if (masterCellList[m].zoneOps[k].boolOp == 1) line2 += sor;
                else line2 += s1;

                if (masterCellList[m].zoneOps[k].senseOp == 1) line2 += sopp;
                else line2 += sopm;
                tmp1.setNum(masterCellList[m].zoneOps[k].bodyNum);
                line2 += tmp1;
                lines.push_back(line2);
            }
        }           // -- end if for zoneOps

        line  = lines[0];
        line2 = "";
        int lLength    = line.length();
        int maxSegment = 0;
        for (int k=1; k < (int) lines.size(); ++k)
        {
            lLength += lines[k].length();

            if (lLength > lineLength)
            {
                int n = line.length();
                if (n > maxSegment) maxSegment = n;
                line   += "\n";
                line2   = s5;
                line2  += lines[k];
                lLength = line2.length();
            }
            else
            {
                line2   += lines[k];
                lLength  = line2.length();
            }
        }                       // -- end k-loop over lines

        int n = line2.length();
        if (n < maxSegment)
        {
            n = maxSegment - n;
            for(int k=0; k<n; ++k) line2.append(s1);
        }
        line += line2;

        if (doCell)
        {
            line  = sss_pad_end(lineLength, line, ' ');
            line += sc;
            tmp1  = sss_pad_beginning(5, masterCellList[m].cellNo, ' ');
            line += tmp1;
            line += s2;
            line += masterCellList[m].name;
        }

        // Next two lines are the last two for building the line.
        line += "\n";
        cellString += line;

    }                                                   // -- end m-loop over cells
    cellString += "*\nEND\n*\n* SUB-ZONES\n*\nEND";
}                                                       // -- generateItsZoneImage


// ----------------------------------------------------------------------------
void CardImageView::generateItsZoneImage2(QString &cellString)
{
    QString sc   = "* ";
    QString line;

    ruler1 = sc + ruler40;
    if (lineLength > 40)  ruler1 = sc + ruler50;
    if (lineLength > 50)  ruler1 = sc + ruler60;
    if (lineLength > 60)  ruler1 = sc + ruler70;
    if (lineLength > 70)  ruler1 = sc + ruler80;
    if (lineLength > 80)  ruler1 = sc + ruler90;
    if (lineLength > 90)  ruler1 = sc + ruler100;
    if (lineLength > 100) ruler1 = sc + ruler110;
    if (lineLength > 110) ruler1 = sc + ruler120;

    cellString  = "*\n";
    cellString += "*  ZONES\n";
    if (doRuler) cellString += ruler1;

    QMap<int, QString>::iterator mi;

    for (mi = zoneMap.begin(); mi != zoneMap.end(); ++mi)
    {
        line = mi.value();
        // Next two lines are the last two for building the line.
        line += "\n";
        cellString += line;
    }                                                   // -- end m-loop over cells
    cellString += "*\nEND\n*\n* SUB-ZONES\n*\nEND";
}
// ----------------------------------------------------------------------------
void CardImageView::generateItsMtlCardImage(QString &cellString)
{
    QString line;
    QString sc   = "* ";

    ruler1 = sc + ruler40;
    if (lineLength > 40)  ruler1 = sc + ruler50;
    if (lineLength > 50)  ruler1 = sc + ruler60;
    if (lineLength > 60)  ruler1 = sc + ruler70;
    if (lineLength > 70)  ruler1 = sc + ruler80;
    if (lineLength > 80)  ruler1 = sc + ruler90;
    if (lineLength > 90)  ruler1 = sc + ruler100;
    if (lineLength > 100) ruler1 = sc + ruler110;
    if (lineLength > 110) ruler1 = sc + ruler120;

    cellString  = "*  \n";
    cellString += "*  MATERIAL ASSIGNMENT\n";
    if (doRuler) cellString += ruler1;

    QMap<int, QString>::iterator mi;

    for (mi = materialMap.begin(); mi != materialMap.end(); ++mi)
    {
        line = mi.value();
        // Next two lines are the last two for building the line.
        line += "\n";
        cellString += line;
    }

    cellString += "*";
}                               // -- generateItsMtlCardImage
