/*
 * abaqusReadFile.cpp
 *
 * Read the ABAQUS .inp file and save data for processing.
 *
 * Developer: Roger L. Martz
 * Created:   April 21, 2015
 *
 */

#include <fstream>
#include <iostream>
#include <string.h>
#include "abaqusReadFile.h"
#include <stdio.h>

ReadFileAbaqus::~ReadFileAbaqus()
{
    //qDebug() << " ReadFileEeout desctructor called.";
    //fclose(fp);
    //fclose(fp2);
}
// ------------------------------------------------------------------------------------------------
ReadFileAbaqus::ReadFileAbaqus()
{
    //qDebug() << " ReadFileAbaqus constructor called.";
    /*if ((fp = fopen("/Users/roger/Public/Programs/TestFiles/eeoutReadFile_debug4.txt", "w")) == NULL)
    {
        qDebug() << "-- error opening eeoutReadFile_debug file --\n";
    }

    if ((fp2 = fopen("/Users/roger/Public/Programs/TestFiles/eeoutReadFile_debug3.txt", "w")) == NULL)
    {
        qDebug() << "-- error opening eeoutReadFile_debug2 file --\n";
    } */

    // Need fully formed mapping arrays for tets.
    tetOutlineNodes[0][0][0] = -1;
    tetOutlineNodes[0][0][1] = -1;
    tetOutlineNodes[0][1][0] =  0;
    tetOutlineNodes[0][1][1] =  1;
    tetOutlineNodes[0][2][0] =  1;
    tetOutlineNodes[0][2][1] =  2;
    tetOutlineNodes[0][3][0] =  0;
    tetOutlineNodes[0][3][1] =  2;

    tetOutlineNodes[1][0][0] =  0;
    tetOutlineNodes[1][0][1] =  1;
    tetOutlineNodes[1][1][0] = -1;
    tetOutlineNodes[1][1][1] = -1;
    tetOutlineNodes[1][2][0] =  1;
    tetOutlineNodes[1][2][1] =  3;
    tetOutlineNodes[1][3][0] =  0;
    tetOutlineNodes[1][3][1] =  3;

    tetOutlineNodes[2][0][0] =  1;
    tetOutlineNodes[2][0][1] =  2;
    tetOutlineNodes[2][1][0] =  1;
    tetOutlineNodes[2][1][1] =  3;
    tetOutlineNodes[2][2][0] = -1;
    tetOutlineNodes[2][2][1] = -1;
    tetOutlineNodes[2][3][0] =  2;
    tetOutlineNodes[2][3][1] =  3;

    tetOutlineNodes[3][0][0] =  0;
    tetOutlineNodes[3][0][1] =  2;
    tetOutlineNodes[3][1][0] =  0;
    tetOutlineNodes[3][1][1] =  3;
    tetOutlineNodes[3][2][0] =  2;
    tetOutlineNodes[3][2][1] =  3;
    tetOutlineNodes[3][3][0] = -1;
    tetOutlineNodes[3][3][1] = -1;

    // Need fully formed mapping arrays for tets.
    pentOutlineNodes[0][0][0] = -1;
    pentOutlineNodes[0][0][1] = -1;
    pentOutlineNodes[0][1][0] = -1;
    pentOutlineNodes[0][1][1] = -1;
    pentOutlineNodes[0][2][0] =  0;
    pentOutlineNodes[0][2][1] =  1;
    pentOutlineNodes[0][3][0] =  1;
    pentOutlineNodes[0][3][1] =  2;
    pentOutlineNodes[0][4][0] =  0;
    pentOutlineNodes[0][4][1] =  2;

    pentOutlineNodes[1][0][0] = -1;
    pentOutlineNodes[1][0][1] = -1;
    pentOutlineNodes[1][1][0] = -1;
    pentOutlineNodes[1][1][1] = -1;
    pentOutlineNodes[1][2][0] =  3;
    pentOutlineNodes[1][2][1] =  4;
    pentOutlineNodes[1][3][0] =  4;
    pentOutlineNodes[1][3][1] =  5;
    pentOutlineNodes[1][4][0] =  3;
    pentOutlineNodes[1][4][1] =  5;

    pentOutlineNodes[2][0][0] =  0;
    pentOutlineNodes[2][0][1] =  1;
    pentOutlineNodes[2][1][0] =  3;
    pentOutlineNodes[2][1][1] =  4;
    pentOutlineNodes[2][2][0] = -1;
    pentOutlineNodes[2][2][1] = -1;
    pentOutlineNodes[2][3][0] =  1;
    pentOutlineNodes[2][3][1] =  4;
    pentOutlineNodes[2][4][0] =  0;
    pentOutlineNodes[2][4][1] =  3;

    pentOutlineNodes[3][0][0] =  1;
    pentOutlineNodes[3][0][1] =  2;
    pentOutlineNodes[3][1][0] =  4;
    pentOutlineNodes[3][1][1] =  5;
    pentOutlineNodes[3][2][0] =  1;
    pentOutlineNodes[3][2][1] =  4;
    pentOutlineNodes[3][3][0] = -1;
    pentOutlineNodes[3][3][1] = -1;
    pentOutlineNodes[3][4][0] =  2;
    pentOutlineNodes[3][4][1] =  5;

    pentOutlineNodes[4][0][0] =  0;
    pentOutlineNodes[4][0][1] =  2;
    pentOutlineNodes[4][1][0] =  3;
    pentOutlineNodes[4][1][1] =  5;
    pentOutlineNodes[4][2][0] =  0;
    pentOutlineNodes[4][2][1] =  3;
    pentOutlineNodes[4][3][0] =  2;
    pentOutlineNodes[4][3][1] =  5;
    pentOutlineNodes[4][4][0] = -1;
    pentOutlineNodes[4][4][1] = -1;

    // --------------------------------
    hexOutlineNodes[0][1][0] = -1;
    hexOutlineNodes[0][1][1] = -1;
    hexOutlineNodes[0][2][0] =  0;
    hexOutlineNodes[0][2][1] =  1;
    hexOutlineNodes[0][3][0] =  1;
    hexOutlineNodes[0][3][1] =  2;
    hexOutlineNodes[0][4][0] =  2;
    hexOutlineNodes[0][4][1] =  3;
    hexOutlineNodes[0][5][0] =  0;
    hexOutlineNodes[0][5][1] =  3;

    hexOutlineNodes[1][2][0] =  4;
    hexOutlineNodes[1][2][1] =  5;
    hexOutlineNodes[1][3][0] =  5;
    hexOutlineNodes[1][3][1] =  6;
    hexOutlineNodes[1][4][0] =  6;
    hexOutlineNodes[1][4][1] =  7;
    hexOutlineNodes[1][5][0] =  4;
    hexOutlineNodes[1][5][1] =  7;

    hexOutlineNodes[2][3][0] =  1;
    hexOutlineNodes[2][3][1] =  5;
    hexOutlineNodes[2][4][0] = -1;
    hexOutlineNodes[2][4][1] = -1;
    hexOutlineNodes[2][5][0] =  0;
    hexOutlineNodes[2][5][1] =  4;

    hexOutlineNodes[3][4][0] =  2;
    hexOutlineNodes[3][4][1] =  6;
    hexOutlineNodes[3][5][0] = -1;
    hexOutlineNodes[3][5][1] = -1;

    hexOutlineNodes[4][5][0] =  3;
    hexOutlineNodes[4][5][1] =  7;

    annotatePersistence = false;
}
// ------------------------------------------------------------------------------------------------
int ReadFileAbaqus::readAbaqusFile(QString fileName, QString cPath, QString fileError)
{
//    qDebug() << "***** readEeoutFile *****";
//    qDebug() << "fileName:: " << fileName;
//    qDebug() << "cPath   :: " << cPath;

    currentPath = cPath;

    instanceCodesLeft[0] = 0;
    instanceCodesLeft[1] = 2;
    instanceCodesLeft[2] = 4;
    instanceCodesLeft[3] = 6;
    instanceCodesLeft[4] = 8;
    instanceCodesLeft[5] = 10;
    instanceCodesRight[0] = 1;
    instanceCodesRight[1] = 3;
    instanceCodesRight[2] = 5;
    instanceCodesRight[3] = 7;
    instanceCodesRight[4] = 9;
    instanceCodesRight[5] = 11;

    // Tet faces
    tetFaces.clear();
    vector<int> tmp;
    tmp.push_back(0);
    tmp.push_back(2);
    tmp.push_back(1);
    tetFaces.push_back(tmp);
    tmp.clear();
    tmp.push_back(0);
    tmp.push_back(1);
    tmp.push_back(3);
    tetFaces.push_back(tmp);
    tmp.clear();
    tmp.push_back(1);
    tmp.push_back(2);
    tmp.push_back(3);
    tetFaces.push_back(tmp);
    tmp.clear();
    tmp.push_back(2);
    tmp.push_back(0);
    tmp.push_back(3);
    tetFaces.push_back(tmp);
    tmp.clear();

    // Pent faces
    pentFaces.clear();
    tmp.push_back(0);
    tmp.push_back(2);
    tmp.push_back(1);
    tmp.push_back(0);
    pentFaces.push_back(tmp);
    tmp.clear();
    tmp.push_back(3);
    tmp.push_back(4);
    tmp.push_back(5);
    tmp.push_back(3);
    pentFaces.push_back(tmp);
    tmp.clear();
    tmp.push_back(0);
    tmp.push_back(1);
    tmp.push_back(4);
    tmp.push_back(3);
    pentFaces.push_back(tmp);
    tmp.clear();
    tmp.push_back(1);
    tmp.push_back(2);
    tmp.push_back(5);
    tmp.push_back(4);
    pentFaces.push_back(tmp);
    tmp.clear();
    tmp.push_back(2);
    tmp.push_back(0);
    tmp.push_back(3);
    tmp.push_back(5);
    pentFaces.push_back(tmp);
    tmp.clear();

    // Hex faces
    hexFaces.clear();
    tmp.push_back(0);
    tmp.push_back(3);
    tmp.push_back(2);
    tmp.push_back(1);
    hexFaces.push_back(tmp);
    tmp.clear();
    tmp.push_back(4);
    tmp.push_back(5);
    tmp.push_back(6);
    tmp.push_back(7);
    hexFaces.push_back(tmp);
    tmp.clear();
    tmp.push_back(0);
    tmp.push_back(1);
    tmp.push_back(5);
    tmp.push_back(4);
    hexFaces.push_back(tmp);
    tmp.clear();
    tmp.push_back(1);
    tmp.push_back(2);
    tmp.push_back(6);
    tmp.push_back(5);
    hexFaces.push_back(tmp);
    tmp.clear();
    tmp.push_back(2);
    tmp.push_back(3);
    tmp.push_back(7);
    tmp.push_back(6);
    hexFaces.push_back(tmp);
    tmp.clear();
    tmp.push_back(3);
    tmp.push_back(0);
    tmp.push_back(4);
    tmp.push_back(7);
    hexFaces.push_back(tmp);
    tmp.clear();

    QString     key,    s1;
    QString     line,   line2;
    QStringList fields, fields2;

    QFile file(fileName.trimmed());

    if (!file.open(QIODevice::ReadOnly))
    {
        fileError = file.errorString();
        return -1;
    }

    QFileInfo qf(file);
    abaqusFile = qf.fileName();

    QTextStream in(&file);

    // Initialize umObject
    this->nResults    = 0;
    this->nNodes      = 0;
    this->nElements   = 0;
    this->colorChoice = 1;
    instances.clear();
    partNames.clear();
    materials.clear();
    nodes.clear();
    parts.clear();
    fileType = ABAQUS;
    status   = 3;

    UM_material umm;
    umm.display = true;
    umm.color.setRed(255);
    umm.color.setGreen(255);
    umm.color.setBlue(255);
    umm.color.setAlpha(255);

    nodesMinX =  9.9e+39;
    nodesMaxX = -9.9e+39;
    nodesMinY =  9.9e+39;
    nodesMaxY = -9.9e+39;
    nodesMinZ =  9.9e+39;
    nodesMaxZ = -9.9e+39;

    line  = in.readLine();
    line  = in.readLine();

    if (line.startsWith("cubit("))
    {
        readFlatAbaqusFile(in);
    }
    else
    {
        readNormalAbaqusFile(in);
    }

    //qDebug() << "1 readEeoutFile :: h1elNodesIndices.size() "  << this->h1elNodesIndices.size();
    //qDebug() << "1 readEeoutFile :: t2elNodesIndices.size() "  << this->t2elNodesIndices.size();

    bool newBuild = false;

    finishMaterials();
    calculateElementVolumesByPart();
    calculatePartVolumes();

    if (newBuild)
    {
        buildInstancesNew();
        instanceColors();
    }
    else
    {
        buildInstances();
        //qDebug() << "\n ** after buildInstances";
        //QString ss;
        //ss = "/Users/roger/Public/Programs/TestFiles/abaqusReadFile_debug_elements1.txt";
        //fprintElements(ss);

        calculateCentroids();
        //qDebug() << "\n ** after calculateCentroids";

        calculateElementNormals();
        //qDebug() << "\n ** after calculateElementNormals";

        instanceColors();
        //qDebug() << "\n ** after instanceColors";

        finalizeInstances();
        //qDebug() << "\n ** after finalizeInstances";

        buildCTNumbers();
        //qDebug() << "\n ** after buildCTNumbers";

        makeInstanceTrees();
        //qDebug() << "\n ** after makeInstanceTrees";
    }

    // Set the mesh centroid.
    this->nodesCentroid[0] = (this->nodesMinX + this->nodesMaxX) * 0.5;
    extentsX = this->nodesMaxX - this->nodesMinX;
    minExtents[0] = nodesMinX;
    maxExtents[0] = nodesMaxX;

    // Set the mesh centroid.
    this->nodesCentroid[1] = (this->nodesMinY + this->nodesMaxY) * 0.5;
    extentsY = this->nodesMaxY - this->nodesMinY;
    minExtents[1] = nodesMinY;
    maxExtents[1] = nodesMaxY;

    // Set the mesh centroid.
    this->nodesCentroid[2] = (this->nodesMinZ + this->nodesMaxZ) * 0.5;
    extentsZ = this->nodesMaxZ - this->nodesMinZ;
    minExtents[2] = nodesMinZ;
    maxExtents[2] = nodesMaxZ;

    //ss = "/Users/roger/Public/Programs/TestFiles/abaqusReadFile_debug_elements2.txt";
    //fprintElements(ss);
    //for (i=0; i<6; ++i)
    //{
    //    qDebug() << "elAdjust: i: " << i << "\t" << elAdjust[i];
    //}

    //createSummaryFile();
    //qDebug() << "\n ** after createSummaryFile";
    return 0;

}       // -- end of readAbaqusFile

// ------------------------------------------------------------------------------------------------
void ReadFileAbaqus::readNormalAbaqusFile(QTextStream &in)
{
    int		i;
    int         countNode2;
    int         countElement;
    char        loopControl;

    ABaqus_part tPart;

    QString     s1;
    QString     line,   line2;
    QStringList fields, fields2;

    i = 0;
    while (!in.atEnd())
    {
        // Do not change the order of these if tests.
        // This is the expected order in the Abaqus .inp file.
        i++;
        line  = in.readLine();
        line2 = line.toLower();
        //qDebug() << line;
        if (line2.indexOf("*part") > -1)
        {
            tPart.clearIt();
            fields = line2.split('=', QString::SkipEmptyParts);
            s1 = fields.last();
            partNames.push_back(s1);
            tPart.name = s1;
            //qDebug() << "part name: " << fields.last();
        }

        if (line2.indexOf("*node") > -1)
        {
            countNode2 = 0;
            loopControl = getPartNodes(in, countNode2, line, tPart.nodes, tPart.pNodeMap);
            line2 = line.toLower();
            nNodes += countNode2;
            //qDebug() << "<return from getPartNodes> " << line2 << "\tcountNode2: " << countNode2 <<
            //        "\t size of nodes: " << tPart.nodes.size();
            //if (countNode2 >= 837)
            //{
            //    qDebug() << "(1) X: " << tPart.nodes[1].x << "\t Y: " << tPart.nodes[1].y  << "\t Z: " << tPart.nodes[1].z;
            //    qDebug() << "(2) X: " << tPart.nodes[2].x << "\t Y: " << tPart.nodes[2].y  << "\t Z: " << tPart.nodes[2].z;
            //    qDebug() << "(3) X: " << tPart.nodes[3].x << "\t Y: " << tPart.nodes[3].y  << "\t Z: " << tPart.nodes[3].z;

           // }
        }
        if (line2.indexOf("*element") > -1)
        {
            countElement = 0;
            getPartConnectivity(in, countElement, line, tPart);
            line2 = line.toLower();
            nElements += countElement;
            //qDebug() << "<return from getPartConnectivity> " << line2 << "\tcountNode2: " << countNode2;
        }

        if (line2.startsWith("*elset"))
        {
            if (line2.indexOf("material") > -1)
            {
                //qDebug() << "\nline2:\n" << line2;

                fields2 = line2.split(',', QString::SkipEmptyParts);
                //fields2 = fields[1].split(',', QString::SkipEmptyParts);
                //qDebug() << "fields2: " << fields2[0] << "\t::" << fields2[1];
                QRegExp rxlen("(\\d+)");
                int pos = rxlen.indexIn(fields2[1]);
                if (pos > -1)
                {
                    QString value = rxlen.cap(1);
                    tPart.matNo   = value.toInt() - 1;
                    //qDebug() << "part name: " << tPart.name << "\t mat No : " << tPart.matNo;
                }

            }
        }

        if (line2.indexOf("*end part") > -1)
        {
            endPartProcessing(tPart);
            parts.push_back(tPart);
        }
        if (line2.startsWith("*instance"))
        {
            getInstanceInfo(in, line);
        }
        if (line2.startsWith("*material"))
        {
            getMaterialNames(line);
        }
    }
}

// ------------------------------------------------------------------------------------------------
void ReadFileAbaqus::readFlatAbaqusFile(QTextStream &in)
{
    int		i;
    int         countNode2;
    int         countElement;
    char        loopControl;

    ABaqus_part tPart;

    QString     s1;
    QString     line,   line2;
    QStringList fields, fields2;

    bool skipIt = false;
    bool contIt = true;

    //while (!in.atEnd())
    while ( contIt )
    {
        if ( in.atEnd() ) break;

        if (skipIt)
        {
            skipIt = false;
        }
        else
        {
            // Read the next line.
            i++;
            line  = in.readLine();
            line2 = line.toLower();
//            qDebug() << line;
        }
        // ----------------------------------------------------
        // Do not change the order of these if tests.
        // This is the expected order in the Abaqus .inp file.
        if (line2.indexOf("*part") > -1)
        {
            tPart.clearIt();
            fields = line2.split('=', QString::SkipEmptyParts);
            s1 = fields.last();
//            partNames.push_back(s1);
            tPart.name = s1;
            //qDebug() << "part name: " << fields.last();
        }
        else if (line2.indexOf("*node") > -1)
        {
            countNode2 = 0;
            loopControl = getPartNodes(in, countNode2, line, tPart.nodes, tPart.pNodeMap);
            line2 = line.toLower();
            nNodes += countNode2;
            //qDebug() << "<return from getPartNodes> " << line2 << "\tcountNode2: " << countNode2 <<
            //        "\t size of nodes: " << tPart.nodes.size();
            //if (countNode2 >= 837)
            //{
            //    qDebug() << "(1) X: " << tPart.nodes[1].x << "\t Y: " << tPart.nodes[1].y  << "\t Z: " << tPart.nodes[1].z;
            //    qDebug() << "(2) X: " << tPart.nodes[2].x << "\t Y: " << tPart.nodes[2].y  << "\t Z: " << tPart.nodes[2].z;
            //    qDebug() << "(3) X: " << tPart.nodes[3].x << "\t Y: " << tPart.nodes[3].y  << "\t Z: " << tPart.nodes[3].z;

           // }
            skipIt = true;
        }
        else if (line2.indexOf("*element") > -1)
        {
            ABaqus_part flatPart;
            flatPart.clearIt();

            fields = line2.split(',', QString::SkipEmptyParts);
            fields2 = fields[2].split('=', QString::SkipEmptyParts);
            if (fields2[0].indexOf("elset") > -1)
            {
                flatPart.name = fields2[1];
            }

//            qDebug() << "FOUND: *element " << " name: " << flatPart.name;


            countElement = 0;
            line = line.toLower();
            getPartConnectivity(in, countElement, line, flatPart);

            line2 = line.toLower();
            nElements += countElement;

            parts.push_back(flatPart);

//            qDebug() << "<return from getPartConnectivity> " << line2 << "\tcountNode2: " << countNode2;
            skipIt = true;
        }
        else if (line2.startsWith("*solid section"))
        {
            if (line2.indexOf("material") > -1)
            {
                fields = line2.split(',', QString::SkipEmptyParts);
                fields2 = fields[1].split('=', QString::SkipEmptyParts);
//                fields2[0] = trim(fields2[0]);

                size_t st = fields.size();
//                qDebug() << "st: " << st;
//                qDebug() << ">" << fields2[0] << "<";

                if (fields2[0].indexOf("elset") > -1  &&  fields.size() > 2)
                {
                    fields[0] = fields2[1];
                    fields2.clear();
                    fields2 = fields[2].split('=', QString::SkipEmptyParts);
                    matElsetMap[fields[0]] = fields2[1];
                }
            }
        }
        else if (line2.indexOf("*end part") > -1)
        {
            for(i=0; i< (int) parts.size(); ++i)
            {
 //               printf("\nEND PART processing:: part: %d\n", i);
//                if (i == 2)
//                    transferNodes2(tPart, parts[i]);
//                else
                transferNodes(tPart, parts[i]);
                endPartProcessing(parts[i]);
//                qDebug() << "end part: " << parts[i].name <<
//                            " h1 size: " << parts[i].h1.size();
            }
        }
        else if (line2.startsWith("*material"))
        {
            getMaterialNames(line);
        }
    }
    processPartsInstances(matElsetMap);
}

// ----------------------------------------------------------------------------
void ReadFileAbaqus::processPartsInstances(QMap<QString, QString> &matElset)
{
    // Finalize parts from Abaqus flat file.  Create instances.

    int         num;
    QString     name;
    size_t      pos;
    UM_instance umi;

    map<int, int> :: const_iterator ii;


    for(size_t i=0; i<parts.size(); ++i)
    {
        name = matElset[ parts[i].name ];
        num  = findElsetNumber(name, "processPartsInstances", pos);

        if (num < 0)
        {
            cout << " >>>>> ERROR: Invalid material number: " << num << endl;
            cout << " >>>>>        Material tag           : " << name.constData() << endl;
            cout << " >>>>>        Processing parts into instances. \n\n";
        }

        parts[i].matNo = num;

        // Set the material number for each element in the part.
        // **********************************************************************************
        // * CAUTION: this will use the material number input through the Abaqus inp file.  *
        // * This number should not be used to index any array or vector, but should be     *
        // * used entirely as an identifier number.                                         *
        // **********************************************************************************
        //for (ii = parts[i].pConnectMap.begin(); ii != parts[i].pConnectMap.end(); ++ii)
        //{
        //    parts[i].matUse[ii->first] = num;
        //}

        // Finally, register the part name in the object list.
        partNames.push_back(parts[i].name);

        // --------------------------------------
        // Create an instance from the flat part.
        umi.name     = parts[i].name;
        umi.partName = parts[i].name;
        umi.partName.remove(" ");
        umi.partName = umi.partName.trimmed();
        for(int m=0; m<3; ++m) umi.translate[m] = 0.0;
        for(int m=0; m<7; ++m) umi.rotate[m]    = 0.0;
        umi.rotate[3] = 1.0;
        instances.push_back(umi);

    }					// -- end i-loop over parts
}						// -- end processPartsInstances

// ----------------------------------------------------------------------------
int ReadFileAbaqus::findElsetNumber(const QString stringIn, const QString name, size_t &pos)
{
    // Allows '_' or '-' to separate the elset number from the rest of the text.

    int num;
    int j = stringIn.length();

    int found = stringIn.indexOf("_");
    j = j - found;
    QString   s1 = stringIn.mid(found+1, j);

    num = s1.toInt();
    if (num == 0  &&  found < 1)
    {
        found = stringIn.indexOf("-");
        j = j - found;
        s1    = stringIn.mid(found+1, j);

        num = s1.toInt();
        if (num < 0  ||  found < 0)
        {
            cout << "\n >>>>> ERROR: Invalid " << name.toStdString() << " number for elset: "
                 << stringIn.toStdString() << endl;
        }
    }
    pos = found;
    return num;
}

// ----------------------------------------------------------------------------
void ReadFileAbaqus::transferNodes(ABaqus_part &abqPart, ABaqus_part &newPart)
{
    P_nodes tNode;

    tNode.x = 0.0;
    tNode.y = 0.0;
    tNode.z = 0.0;

    // -----------------------------------------------------------------
    // Store zero in the 0'th slot so that the connectivity information
    // won't need to be re-numbered.
    // -----------------------------------------------------------------
    int s = newPart.maxElNo - newPart.minElNo + 1;
    newPart.nodes.clear();
    newPart.nodes.reserve(s);
    newPart.nodes.push_back(tNode);
    newPart.pNodeMap.clear();

    int k;
    int count = 0;

    for(int i=newPart.minElNo; i<newPart.maxElNo+1; ++i)
    {
        k = pNodeMap[i];

        // The following if test skips non-consecutive node numbers.
        if (k == 0)  continue;

        newPart.nodes.push_back( abqPart.nodes[k] );
        ++count;
        newPart.pNodeMap[i] = count;
    }
}

// ----------------------------------------------------------------------------
void ReadFileAbaqus::transferNodes2(ABaqus_part &abqPart, ABaqus_part &newPart)
{
    P_nodes tNode;

    tNode.x = 0.0;
    tNode.y = 0.0;
    tNode.z = 0.0;

    // -----------------------------------------------------------------
    // Store zero in the 0'th slot so that the connectivity information
    // won't need to be re-numbered.
    // -----------------------------------------------------------------
    int s = newPart.maxElNo - newPart.minElNo + 1;
    newPart.nodes.clear();
    newPart.nodes.reserve(s);
    newPart.nodes.push_back(tNode);
    newPart.pNodeMap.clear();

    int k;
    int count = 0;

//    qDebug() << "\n -- transferNodes2 --\n";
//    qDebug() << "size of abqPart nodes: " << abqPart.nodes.size();
//    qDebug() << "min el: " << newPart.minElNo << "max el: " << newPart.maxElNo;
//    qDebug() << "pNodeMap size : "<< pNodeMap.size();

    for(int i=newPart.minElNo; i<newPart.maxElNo+1; ++i)
//    for(int i=newPart.minElNo; i<newPart.minElNo + 2048; ++i)
    {
//        if (i == newPart.minElNo + 2046)
//        {
//            qDebug() << " - - - ";
//        }

        k = pNodeMap[i];

        // The following if test skips non-consecutive node numbers.
        if (k == 0)  continue;

        if (k > (int) abqPart.nodes.size()  ||  k < 0)
            qDebug() << "out of range:: k: " << k;

        newPart.nodes.push_back( abqPart.nodes[k] );
        ++count;
        newPart.pNodeMap[i] = count;
        if (count > 2000)
        {
        qDebug() << "elNo: " << i << "count: " << count << "k: " << k
                 << "node: " << abqPart.nodes[k].x << abqPart.nodes[k].y
                 <<   abqPart.nodes[k].z;
        }
    }

//    qDebug() << "-- transferNodes2:: complete:: nodes size: " << newPart.nodes.size();
//    qDebug() << "-- transferNodes2:: complete::";
}
// ------------------------------------------------------------------------------------------------
void ReadFileAbaqus::getMaterialNames(QString line)
{
    QString     line2;
    QStringList fields;

    int k, n;

    UM_material umm;
    umm.display = true;
    umm.color.setRed(255);
    umm.color.setGreen(255);
    umm.color.setBlue(255);
    umm.color.setAlpha(255);

    //materials.push_back(umm);

    line2  = line.trimmed();
    k = line.lastIndexOf("-", -1);
    n = line.lastIndexOf("_", -1);

    if (k > n)
    {
        fields = line2.split('-', QString::SkipEmptyParts);
    }
    else
    {
        fields = line2.split('_', QString::SkipEmptyParts);
    }
    // Check for all numeric.
    QRegExp rxlen("(\\d+)");
    QString last = fields.last();
    int pos = rxlen.indexIn(last);
    if (pos > -1)
    {
        QString value = rxlen.cap(1);
        k             = value.toInt();
    }
    else
        k = 0;

//    umm.number = fields.last().toInt() - 1;
    umm.number = k;
    fields     = line.split('=', QString::SkipEmptyParts);
    umm.name   = fields[1];
    materials.push_back(umm);

//    qDebug() << "material name: " << umm.name;
}
// ------------------------------------------------------------------------------------------------
void ReadFileAbaqus::finishMaterials()
{
    int i, k;
    int maxK = -2;

    this->nMaterials = materials.size();

    //for(i=0; i<nMaterials; ++i)
    //{
    //    qDebug() << "material:: " << i << "\tnumber: " << materials[i].number << "\t" << materials[i].name;
    //}

    int maxColors = 255;
    vector<int> red(this->nMaterials);
    vector<int> gre(this->nMaterials);
    vector<int> blu(this->nMaterials);
    setRGBcolors(maxColors, this->nMaterials, red, gre, blu);
    // --------------------------------------------------------------

    for(i=0; i<nMaterials; ++i)
    {
        materials[i].color.setRed(red[i]);
        materials[i].color.setGreen(gre[i]);
        materials[i].color.setBlue(blu[i]);
        if (materials[i].number > maxK) maxK = materials[i].number;
    }

    // Initialize the material map array.
    matMap.clear();
    for (i=-2; i<maxK; ++i)
    {
        matMap.push_back(-1);
    }

    for (i=0; i<this->nMaterials; i++)
    {
        k         = materials[i].number;    // Get the material number from the name.
        matMap[k] = i;                      // Store the list number by material number.
        //qDebug() << "list #" << i << "mat no " << k << "\tname: " << materials[i].name;
    }

    //for (i=0; i<matMap.size(); ++i)
    //{
    //    qDebug() << "i: " << i << "\tmatMap: " << matMap[i];
    //}
}
// ------------------------------------------------------------------------------------------------
void ReadFileAbaqus::getInstanceInfo(QTextStream &in, QString lineIn)
{
    QString line;
    QStringList fields, fields2;

    // Move to finish instances
    //int maxColors = 255;
    //vector<int> red(this->nInstances);
    //vector<int> gre(this->nInstances);
    //vector<int> blu(this->nInstances);
    //setRGBcolors(maxColors, this->nInstances, red, gre, blu);
    // --------------------------------------------------------------

    UM_instance umi;

    // -- set defaults
    umi.display    = true;
    umi.color.setRed(255);
    umi.color.setGreen(255);
    umi.color.setBlue(255);
    umi.color.setAlpha(255);
    umi.lowerLimit  = 0;
    umi.upperLimit  = 0;
    umi.expand[0]   = 0.0;   umi.extraExpand[0] = 0.0;  umi.translate[0] = 0.0;
    umi.expand[1]   = 0.0;   umi.extraExpand[1] = 0.0;  umi.translate[1] = 0.0;
    umi.expand[2]   = 0.0;   umi.extraExpand[2] = 0.0;  umi.translate[2] = 0.0;
    umi.extents[0]  = 0.0;
    umi.extents[1]  = 0.0;
    umi.extents[2]  = 0.0;

    umi.rotate[0]   = 0.0;   umi.rotate[1]   = 0.0;   umi.rotate[2]   = 0.0;
    umi.rotate[3]   = 0.0;   umi.rotate[4]   = 0.0;   umi.rotate[5]   = 0.0;   umi.rotate[6]   = 0.0;

    umi.centroid[0] = 0.0;
    umi.centroid[1] = 0.0;
    umi.centroid[2] = 0.0;
    for (int i=0; i<12; i++) umi.elementLimits[i] = 0;

    // Grab the instance name and associated part name.
    fields       = lineIn.split(',', QString::SkipEmptyParts);
    int i = fields[1].indexOf("=");
    if (i > 0)
    {
        ++i;
        fields[1].remove(0, i);
    }
    umi.name     = fields[1].trimmed();

    if (fields.count() > 2)
    {
        fields2      = fields[2].split('=', QString::SkipEmptyParts);
    }
    else
    {
        fields2      = fields[1].split('=', QString::SkipEmptyParts);
    }
    fields2[1]   = fields2[1].toLower();
    umi.partName = fields2[1].trimmed();

    // Grab the next line and check for translation.
    line = in.readLine();
    line = line.toLower();

    //qDebug() << line;

    if (!line.startsWith("*end"))
    {
        //qDebug() << " -- not end --";
        fields        = line.split(',', QString::SkipEmptyParts);
        //qDebug() << "fields: " << fields[0] << fields[1] << fields[2];

        umi.translate[0] = fields.takeFirst().toDouble();
        umi.translate[1] = fields.takeFirst().toDouble();
        umi.translate[2] = fields.takeFirst().toDouble();
        line             = in.readLine();
        line             = line.toLower();

        if (!line.startsWith("*end"))
        {
            fields        = line.split(',', QString::SkipEmptyParts);
            for(int i=0; i<7; ++i)
            {
                umi.rotate[i] = fields.takeFirst().toDouble();
            }
        }
    }

    //qDebug() << "instance name: " << umi.name;
    //qDebug() << "instance part: " << umi.partName;
    //qDebug() << "translate: " << umi.translate[0] << "\t" << umi.translate[1] << "\t" << umi.translate[2];

    this->instances.push_back(umi);
}

// ------------------------------------------------------------------------------------------------
void ReadFileAbaqus::buildInstances()
{
    double rPoint1[3];
    double rPoint2[3];
    double angle;
    double oldPt[3], newPt[3];
    int currentT1, currentP1, currentH1, currentT2, currentP2, currentH2;
    int i, j, k, p;
    int elAccuml[6];
    int nodeAdjust = 0;
    int *nAdjust;
    int nInst = instances.size();
    int pSize = parts.size();

    nAdjust = new int[nInst];
    UM_nodes tNode;
    UM_node  sNode;

    tNode.x = 0.0;
    tNode.y = 0.0;
    tNode.z = 0.0;
    tNode.displayed = false;

    // Store zero in the 0'th slot so that the connectivity information
    // won't need to be re-numbered.
    this->nodes.clear();
    this->nodes.push_back(tNode);

    // ---------------------------
    // Process the 1st order tets.
    UM_t1element umt1;
    umt1.eType          = 4;
    umt1.instance       = 0;
    umt1.displayElement = true;
    t1elements.clear();

    UM_t1element umT1;
    umT1.eType          = 4;
    umT1.instance       = 0;
    umT1.displayElement = true;

    // ----------------------------
    // Process the 1st order pents.
    UM_p1element ump1;
    ump1.eType          = 5;
    ump1.instance       = 0;
    ump1.displayElement = true;
    this->p1elements.clear();

    UM_p1element umP1;
    umP1.eType          = 5;
    umP1.instance       = 0;
    umP1.displayElement = true;

    // ---------------------------
    // Process the 1st order hexs.
    UM_h1element umh1;
    umh1.eType          = 6;
    umh1.instance       = 0;
    umh1.displayElement = true;
    this->h1elements.clear();

    UM_h1element umH1;
    umH1.eType          = 6;
    umH1.instance       = 0;
    umH1.displayElement = true;

    // ---------------------------
    // Process the 2nd order tets.
    UM_t2element umt2;
    umt2.eType          = 14;
    umt2.instance       = 0;
    umt2.displayElement = true;
    this->t2elements.clear();

    // ----------------------------
    // Process the 2nd order pents.
    UM_p2element ump2;
    ump2.eType          = 15;
    ump2.instance       = 0;
    ump2.displayElement = true;
    this->p2elements.clear();

    // ---------------------------
    // Process the 2nd order hexs.
    UM_h2element umh2;
    umh2.eType          = 16;
    umh2.instance       = 0;
    umh2.displayElement = true;
    this->h2elements.clear();

    for (j=0; j<6; ++j)
    {
        elAdjust[j] = 0;
        //qDebug() << "j: " << j << "\t elAdjust: " << elAdjust[j];
    }

    int rnodes = 0;

    // -----------------------------------------------------------------------------
    // Translate and rotate the nodes into position and add them to the global list.
    for (i=0; i<nInst; ++i)
    {
//        qDebug() << "instance from part: " << instances[i].partName;

 /*       qDebug() << "inst: " << (i+1) << "\t expand: " << instances[i].expand[0] << "\t" << instances[i].expand[1] << "\t"
                 << instances[i].expand[2] ;
        qDebug() << "\t rotate: " << instances[i].rotate[0] << "\t" << instances[i].rotate[1] << "\t" << instances[i].rotate[2];
        qDebug() << "\t rotate: " << instances[i].rotate[3] << "\t" << instances[i].rotate[4] << "\t" << instances[i].rotate[5];
        qDebug() << "\t rotate: " << instances[i].rotate[6] ;
*/
        angle      = instances[i].rotate[6];
        rPoint1[0] = instances[i].rotate[0];  rPoint1[1] = instances[i].rotate[1];  rPoint1[2] = instances[i].rotate[2];
        rPoint2[0] = instances[i].rotate[3];  rPoint2[1] = instances[i].rotate[4];  rPoint2[2] = instances[i].rotate[5];

        //qDebug() << "angle: " << angle;
        //qDebug() << "rPoint1: " << rPoint1[0] << rPoint1[1] << rPoint1[2];
        //qDebug() << "rPoint2: " << rPoint2[0] << rPoint2[1] << rPoint2[2];

        for (j=0; j<3; ++j)
        {
            instances[i].extents[j]   =  9.9e+39;
            instances[i].extents[j+3] = -9.9e+39;
        }
        for (j=0; j<12; ++j) instances[i].elementLimits[j] = 0;
        for (j=0; j<6; ++j)  instances[i].adder[j] = 0;

        // Find the part number.
        for (p=0; p<pSize; ++p)
        {
            if (partNames[p].startsWith(instances[i].partName)) break;
        }
        if (p >= pSize) qDebug() << ">>>>> ERROR: part number not found in buildInstances";
        //qDebug() << "part number: " << p;

        for (j=0; j<6; ++j)
        {
            this->elAdjust[j] += parts[p].elNumbers[j];
            //qDebug() << "j: " << j << "\t nEls: " << parts[p].elNumbers[j];
        }

        nAdjust[i] = parts[p].nodes.size() - 1;
        //qDebug() << i << "\t nAdjust: " << nAdjust[i];
                //for (j=1; j<2; ++j)

//        qDebug() << "translate: " << instances[i].translate[0] << "\t" << instances[i].translate[1] <<
//                    "\t" << instances[i].translate[2];

//        double t1 = instances[i].translate[0];
//        double t2 = instances[i].translate[1];
//        double t3 = instances[i].translate[2];

        instances[i].nodes.clear();

        sNode.x = 0.0;
        sNode.y = 0.0;
        sNode.z = 0.0;

        instances[i].nodes.push_back(sNode);

        for (j=1; j < (int) parts[p].nodes.size(); ++j)
        {
            // The j=0 value is just a place holder and is already in the global list.
            //qDebug() << "node: " << j << "\t" << parts[p].nodes[j].x << "\t" << parts[p].nodes[j].y << "\t" << parts[p].nodes[j].z;

            oldPt[0] = parts[p].nodes[j].x + instances[i].translate[0];
            oldPt[1] = parts[p].nodes[j].y + instances[i].translate[1];
            oldPt[2] = parts[p].nodes[j].z + instances[i].translate[2];

            getRotatedPoint(rPoint1, rPoint2,  angle, oldPt, newPt);
            tNode.x = sNode.x = newPt[0];
            tNode.y = sNode.y = newPt[1];
            tNode.z = sNode.z = newPt[2];

            this->nodes.push_back(tNode);

            instances[i].nodes.push_back(sNode);
            ++rnodes;
            //qDebug() << "Node: " << rnodes << "\tx: " << tNode.x << "\ty: " << tNode.y << "\tz: " << tNode.z ;

            // Extremes for this instance.
            if ( tNode.x < instances[i].extents[0] )  instances[i].extents[0] = tNode.x;
            if ( tNode.x > instances[i].extents[3] )  instances[i].extents[3] = tNode.x;
            if ( tNode.y < instances[i].extents[1] )  instances[i].extents[1] = tNode.y;
            if ( tNode.y > instances[i].extents[4] )  instances[i].extents[4] = tNode.y;
            if ( tNode.z < instances[i].extents[2] )  instances[i].extents[2] = tNode.z;
            if ( tNode.z > instances[i].extents[5] )  instances[i].extents[5] = tNode.z;
            // Extremes for the entire problem.
            if ( tNode.x < this->nodesMinX )  this->nodesMinX = tNode.x;
            if ( tNode.x > this->nodesMaxX )  this->nodesMaxX = tNode.x;
            if ( tNode.y < this->nodesMinY )  this->nodesMinY = tNode.y;
            if ( tNode.y > this->nodesMaxY )  this->nodesMaxY = tNode.y;
            if ( tNode.z < this->nodesMinZ )  this->nodesMinZ = tNode.z;
            if ( tNode.z > this->nodesMaxZ )  this->nodesMaxZ = tNode.z;
        }
    }                                           // -- end i-loop over instances

//    qDebug() << "== build Instances ==";
//    qDebug() << "nodes min X : " << nodesMinX << "nodes max X : " << nodesMaxX;
//    qDebug() << "nodes min Y : " << nodesMinY << "nodes max Y : " << nodesMaxY;
//    qDebug() << "nodes min Z : " << nodesMinZ << "nodes max Z : " << nodesMaxZ;

    elAccuml[0] = 0;
    for (j=1; j<6; ++j)
    {
        //qDebug() << "j: " << j << "\t elAdjust: " << elAdjust[j];
        elAccuml[j] = elAccuml[j-1] + elAdjust[j-1];
    }

    for (j=0; j<6; ++j)
    {
        elAdjust[j] = elAccuml[j];
        //qDebug() << "j: " << j << "\t elAccuml: " << elAccuml[j];
    }

    currentT1 = 0;            currentP1 = elAccuml[1];  currentH1 = elAccuml[2];
    currentT2 = elAccuml[3];  currentP2 = elAccuml[4];  currentH2 = elAccuml[5];

    // --------------------------------------------------------------------------------------------
    // Adjust the node connectivity for each element and add the element to the appropriate vector.
    // Set each element's properties as it is added to the instance.
    for (i=0; i<nInst; ++i)
    {
        for (j=0; j<6; ++j) instances[i].elNumbers[j] = 0;
        // Find the part number.
        for (p=0; p<pSize; ++p)
        {
            if (partNames[p].startsWith(instances[i].partName)) break;
        }
        if (p >= pSize) qDebug() << ">>>>> ERROR: part number not found in buildInstances";
        //qDebug() << "build instance: " << i << "\tpart: " << instances[i].partName;
        //qDebug() << "part number: " << p << "\t node adjust: " << nAdjust[i] << "\t nodeAdjust: " << nodeAdjust;

        if (parts[p].elNumbers[0] > 0 )
        {
            umt1.material = parts[p].matNo;
            umt1.density  = 1.0;
            umt1.instance = i;

            umT1.material = parts[p].matNo;
            umT1.density  = 1.0;
            umT1.instance = i;

            instances[i].elNumbers[0] = parts[p].elNumbers[0];

            //qDebug() << "\n -- parts elNumbers: " << parts[p].elNumbers[0] << "\t p: " << p;
            //qDebug() << "\n -- inst  elNumbers: " << instances[i].elNumbers[0] << "\t i: " << i;

            instances[i].elementLimits[0] = currentT1 + 1;  instances[i].elementLimits[1] = currentT1 + parts[p].elNumbers[0];
            for (j=0; j<parts[p].elNumbers[0]; ++j)
            {
                for (k=0; k<4; ++k)
                {
                    //if (j==0) qDebug() << k << "\t t1" << parts[p].t1[j].connect[k] << "\t" << (parts[p].t1[j].connect[k] + nodeAdjust);
                    umt1.connect[k] = parts[p].t1[j].connect[k] + nodeAdjust;
                    umT1.connect[k] = parts[p].t1[j].connect[k];
                }
                umt1.volume   = parts[p].t1[j].volume;
                umT1.volume   = parts[p].t1[j].volume;
                t1elements.push_back(umt1);
                instances[i].t1elements.push_back(umT1);
                ++currentT1;
            }
        }

        if (parts[p].elNumbers[1] > 0 )
        {
            instances[i].elNumbers[1] = parts[p].elNumbers[1];
            ump1.instance = i;
            ump1.material = parts[p].matNo;
            ump1.density  = 1.0;

            umP1.instance = i;
            umP1.material = parts[p].matNo;
            umP1.density  = 1.0;

            instances[i].elementLimits[2] = currentP1 + 1;  instances[i].elementLimits[3] = currentP1 + parts[p].elNumbers[1];
            for (j=0; j<parts[p].elNumbers[1]; ++j)
            {
                for (k=0; k<6; ++k)
                {
                    //qDebug() << k << "\t p1" << parts[p].p1[j].connect[k] << "\t" << (parts[p].p1[j].connect[k] + nodeAdjust);
                    ump1.connect[k] = parts[p].p1[j].connect[k] + nodeAdjust;
                    umP1.connect[k] = parts[p].p1[j].connect[k];
                }
                ump1.volume   = parts[p].p1[j].volume;
                umP1.volume   = parts[p].p1[j].volume;
                p1elements.push_back(ump1);
                instances[i].p1elements.push_back(umP1);
                ++currentP1;
            }
        }

        if (parts[p].elNumbers[2] > 0 )
        {
            //qDebug() << "i: " << i << "\tparts[p].elNumbers[2]" << parts[p].elNumbers[2];
            //qDebug() << "size of parts   : " << parts.size();

            //qDebug() << "size of parts h1: " << parts[p].h1.size();
            //qDebug() << " h1 volume: " << parts[p].h1[i].volume;

            instances[i].elNumbers[2] = parts[p].elNumbers[2];
            umh1.instance = i;
            umh1.material = parts[p].matNo;
            umh1.density  = 1.0;

            umH1.instance = i;
            umH1.material = parts[p].matNo;
            umH1.density  = 1.0;

            instances[i].elementLimits[4] = currentH1 + 1;  instances[i].elementLimits[5] = currentH1 + parts[p].elNumbers[2];

            //qDebug() << "\n -- parts elNumbers: " << parts[p].elNumbers[2] << "\t p: " << p;
            //qDebug() << "\n -- inst  elNumbers: " << instances[i].elNumbers[2] << "\t i: " << i;

            for (j=0; j<parts[p].elNumbers[2]; ++j)
            {
                for (k=0; k<8; ++k)
                {
                    //qDebug() << k << "\t h1" << parts[p].h1[j].connect[k] << "\t" << (parts[p].h1[j].connect[k] + nodeAdjust);
                    umh1.connect[k] = parts[p].h1[j].connect[k] + nodeAdjust;
                    umH1.connect[k] = parts[p].h1[j].connect[k];
                }
                umh1.volume   = parts[p].h1[j].volume;
                umH1.volume   = parts[p].h1[j].volume;
                h1elements.push_back(umh1);
                instances[i].h1elements.push_back(umH1);
                ++currentH1;
            }
        }

        if (parts[p].elNumbers[3] > 0 )
        {
            instances[i].elNumbers[3] = parts[p].elNumbers[3];
            umt2.instance = i;
            umt2.material = parts[p].matNo;
            umt2.density  = 1.0;
            instances[i].elementLimits[6] = currentT2 + 1;  instances[i].elementLimits[7] = currentT2 + parts[p].elNumbers[3];
            for (j=0; j<parts[p].elNumbers[3]; ++j)
            {
                for (k=0; k<10; ++k)
                {
                    //qDebug() << k << "\t t2" << parts[p].t2[j].connect[k] << "\t" << (parts[p].t2[j].connect[k] + nodeAdjust);
                    umt2.connect[k] = parts[p].t2[j].connect[k] + nodeAdjust;
                }
                umt2.volume   = parts[p].t2[j].volume;
                t2elements.push_back(umt2);
                ++currentT2;
            }
        }

        if (parts[p].elNumbers[4] > 0 )
        {
            instances[i].elNumbers[4] = parts[p].elNumbers[4];
            ump2.instance = i;
            ump2.material =parts[p].matNo;
            ump2.density  = 1.0;
            instances[i].elementLimits[8] = currentP2 + 1;  instances[i].elementLimits[9] = currentP2 + parts[p].elNumbers[4];
            for (j=0; j<parts[p].elNumbers[4]; ++j)
            {
                for (k=0; k<15; ++k)
                {
                    //qDebug() << k << "\t p2" << parts[p].p2[j].connect[k] << "\t" << (parts[p].p2[j].connect[k] + nodeAdjust);
                    ump2.connect[k] = parts[p].p2[j].connect[k] + nodeAdjust;
                }
                ump2.volume   = parts[p].p2[j].volume;
                p2elements.push_back(ump2);
                ++currentP2;
            }
        }

        if (parts[p].elNumbers[5] > 0 )
        {
            instances[i].elNumbers[5] = parts[p].elNumbers[5];
            umh2.instance = i;
            umh2.material = parts[p].matNo;
            umh2.density  = 1.0;
            instances[i].elementLimits[10] = currentH2 + 1;  instances[i].elementLimits[11] = currentH2 + parts[p].elNumbers[5];
            for (j=0; j<parts[p].elNumbers[5]; ++j)
            {
                for (k=0; k<20; ++k)
                {
                    //qDebug() << k << "\t h2" << parts[p].h2[j].connect[k] << "\t" << (parts[p].h2[j].connect[k] + nodeAdjust);
                    umh2.connect[k] = parts[p].h2[j].connect[k] + nodeAdjust;
                }
                umh2.volume   = parts[p].h2[j].volume;
                h2elements.push_back(umh2);
                ++currentH2;
            }
        }

        //for(j=0; j<12; ++j)
        //{
        //    qDebug() << "i: " << i << "elementLimits: " << instances[i].elementLimits[j];
        //}

        if (i > 0)
        {
            for (j = 0; j < 6; ++j)
            {
                instances[i].adder[j] = instances[i-1].adder[j] + instances[i-1].elNumbers[j];
            }
        }
        nodeAdjust += nAdjust[i];
    }                                           // -- end i-loop over instances

    nEls[0] = t1elements.size();
    nEls[1] = p1elements.size();
    nEls[2] = h1elements.size();
    nEls[3] = t2elements.size();
    nEls[4] = p2elements.size();
    nEls[5] = h2elements.size();

    delete [] nAdjust;

//    qDebug() << "-- # of nodes: " << nodes.size();
}                                               // -- end buildInstances

// ------------------------------------------------------------------------------------------------
void ReadFileAbaqus::buildInstancesNew()
{
    // This routine is for building the instances so that everything is handled at the instance level
    // and not a big, global model as initially done.
    // Currently, store the nodes by instance and set the surface elements for rendering the geometry.

    double rPoint1[3];
    double rPoint2[3];
    double angle;
    double oldPt[3], newPt[3];
    double dir1[3];
    double dir2[3];
    double dir3[3];
    int i0, i1, i2;
    int i, j, k, p;
    int nInst = instances.size();
    int pSize = parts.size();

    UM_node tNode;

    // ---------------------------
    // Process the 1st order tets.
    UM_t1element umt1;
    umt1.eType          = 4;
    umt1.instance       = 0;
    umt1.displayElement = true;
    t1elements.clear();

    // ----------------------------
    // Process the 1st order pents.
    UM_p1element ump1;
    ump1.eType          = 5;
    ump1.instance       = 0;
    ump1.displayElement = true;
    this->p1elements.clear();

    // ---------------------------
    // Process the 1st order hexs.
    UM_h1element umh1;
    umh1.eType          = 6;
    umh1.instance       = 0;
    umh1.displayElement = true;
    this->h1elements.clear();

    for (j=0; j<6; ++j)
    {
        elAdjust[j] = 0;
    }

    //QString name = "normals.txt";
    //const char *c  = name.toStdString().c_str();

    //FILE *fp;
    //(fp = fopen(c, "w"));

    // -----------------------------------------------------------------------------
    // Translate and rotate the nodes into position and add them to the global list.
    for (i=0; i<nInst; ++i)
    {
        angle      = instances[i].rotate[6];
        rPoint1[0] = instances[i].rotate[0];  rPoint1[1] = instances[i].rotate[1];  rPoint1[2] = instances[i].rotate[2];
        rPoint2[0] = instances[i].rotate[3];  rPoint2[1] = instances[i].rotate[4];  rPoint2[2] = instances[i].rotate[5];

        for (j=0; j<3; ++j)
        {
            instances[i].extents[j]   =  9.9e+39;
            instances[i].extents[j+3] = -9.9e+39;
        }
        for (j=0; j<12; ++j) instances[i].elementLimits[j] = 0;
        for (j=0; j<6; ++j)  instances[i].adder[j] = 0;

        // Find the part number.
        for (p=0; p<pSize; ++p)
        {
            if (partNames[p].startsWith(instances[i].partName)) break;
        }
        if (p >= pSize) qDebug() << ">>>>> ERROR: part number not found in buildInstances";

        for (j=0; j<6; ++j)
        {
            this->elAdjust[j] += parts[p].elNumbers[j];
        }

        instances[i].materialNo = parts[p].matNo;

        tNode.x = 0.0;
        tNode.y = 0.0;
        tNode.z = 0.0;

        // Store zero in the 0'th slot so that the connectivity information
        // won't need to be re-numbered.
        instances[i].nodes.clear();
        //instances[i].nodes.push_back(tNode);
        instances[i].surfaceNodes.clear();

        for (j=1; j < (int) parts[p].nodes.size(); ++j)
        {
            // The j=0 value is just a place holder and is already in the global list.
            //qDebug() << "node: " << j << "\t" << parts[p].nodes[j].x << "\t" << parts[p].nodes[j].y << "\t" << parts[p].nodes[j].z;

            oldPt[0] = parts[p].nodes[j].x + instances[i].translate[0];
            oldPt[1] = parts[p].nodes[j].y + instances[i].translate[1];
            oldPt[2] = parts[p].nodes[j].z + instances[i].translate[2];

            getRotatedPoint(rPoint1, rPoint2,  angle, oldPt, newPt);
            tNode.x = newPt[0];  tNode.y = newPt[1];  tNode.z = newPt[2];

            instances[i].nodes.push_back(tNode) ;

            // Extremes for this instance.
            if ( tNode.x < instances[i].extents[0] )  instances[i].extents[0] = tNode.x;
            if ( tNode.x > instances[i].extents[3] )  instances[i].extents[3] = tNode.x;
            if ( tNode.y < instances[i].extents[1] )  instances[i].extents[1] = tNode.y;
            if ( tNode.y > instances[i].extents[4] )  instances[i].extents[4] = tNode.y;
            if ( tNode.z < instances[i].extents[2] )  instances[i].extents[2] = tNode.z;
            if ( tNode.z > instances[i].extents[5] )  instances[i].extents[5] = tNode.z;
            // Extremes for the entire problem.
            if ( tNode.x < this->nodesMinX )  this->nodesMinX = tNode.x;
            if ( tNode.x > this->nodesMaxX )  this->nodesMaxX = tNode.x;
            if ( tNode.y < this->nodesMinY )  this->nodesMinY = tNode.y;
            if ( tNode.y > this->nodesMaxY )  this->nodesMaxY = tNode.y;
            if ( tNode.z < this->nodesMinZ )  this->nodesMinZ = tNode.z;
            if ( tNode.z > this->nodesMaxZ )  this->nodesMaxZ = tNode.z;
        }

        for (j=0; j < (int) parts[p].surfaceNodes.size(); ++j)
        {
            // The j=0 value is just a place holder and is already in the global list.
            //qDebug() << "node: " << j << "\t" << parts[p].nodes[j].x << "\t" << parts[p].nodes[j].y << "\t" << parts[p].nodes[j].z;

            oldPt[0] = parts[p].surfaceNodes[j].x + instances[i].translate[0];
            oldPt[1] = parts[p].surfaceNodes[j].y + instances[i].translate[1];
            oldPt[2] = parts[p].surfaceNodes[j].z + instances[i].translate[2];

            getRotatedPoint(rPoint1, rPoint2,  angle, oldPt, newPt);
            tNode.x = newPt[0];  tNode.y = newPt[1];  tNode.z = newPt[2];

            instances[i].surfaceNodes.push_back(tNode);
        }

        // Transfer the surface node information for rendering with triangles.
        instances[i].surfaceTriangles.clear();
        for (j=0; j < (int) parts[p].surfaceTriangles.size(); ++j)
        {
            k = parts[p].surfaceTriangles[j];
            instances[i].surfaceTriangles.push_back( k );
        }

//        QString ss = "instance_data.txt";
//        data2File(ss, instances[i].surfaceNodes, instances[i].surfaceTriangles);

        instances[i].surfaceNormals.clear();
        for (j=0; j < (int) instances[i].surfaceTriangles.size(); j += 3)
        {
            // Get the indices for the triangle.
            i0 = instances[i].surfaceTriangles[j];
            i1 = instances[i].surfaceTriangles[j+1];
            i2 = instances[i].surfaceTriangles[j+2];

            // Create the vector along the 1st edge.
            dir1[0] = instances[i].surfaceNodes[i1].x - instances[i].surfaceNodes[i0].x;
            dir1[1] = instances[i].surfaceNodes[i1].y - instances[i].surfaceNodes[i0].y;
            dir1[2] = instances[i].surfaceNodes[i1].z - instances[i].surfaceNodes[i0].z;

            // Create the vector along the 3rd edge.
            dir2[0] = instances[i].surfaceNodes[i2].x - instances[i].surfaceNodes[i0].x;
            dir2[1] = instances[i].surfaceNodes[i2].y - instances[i].surfaceNodes[i0].y;
            dir2[2] = instances[i].surfaceNodes[i2].z - instances[i].surfaceNodes[i0].z;

            // Find the normal.
            angle = crossProduct(dir1, dir2, dir3);
            tNode.x = dir3[0];
            tNode.y = dir3[1];
            tNode.z = dir3[2];

            if (100 < 24)
            {
                fprintf(fp, "i0: %4d  i1: %4d  i2: %4d\n", i0, i1, i2);

                fprintf(fp, "Node: %4d\t Point 1: %10.6f\t %10.6f\t %10.6f\n",
                        i0, instances[i].surfaceNodes[i0].x,
                        instances[i].surfaceNodes[i0].y, instances[i].surfaceNodes[i0].z);

                fprintf(fp, "Node: %4d\t Point 2: %10.6f\t %10.6f\t %10.6f\n",
                        i1, instances[i].surfaceNodes[i1].x,
                        instances[i].surfaceNodes[i1].y, instances[i].surfaceNodes[i1].z);

                fprintf(fp, "Node: %4d\t Point 3: %10.6f\t %10.6f\t %10.6f\n",
                        i2, instances[i].surfaceNodes[i2].x,
                        instances[i].surfaceNodes[i2].y, instances[i].surfaceNodes[i2].z);

            fprintf(fp, "Node: %4d\t Vector 1: %10.6f\t %10.6f\t %10.6f\n",
                    j, dir1[0], dir1[1], dir1[2]);

            fprintf(fp, "Node: %4d\t Vector 2: %10.6f\t %10.6f\t %10.6f\n",
                    j, dir2[0], dir2[1], dir2[2]);

            fprintf(fp, "Node: %4d\t Normal  : %10.6f\t %10.6f\t %10.6f\n\n",
                    j, tNode.x, tNode.y, tNode.z);
            }

            // Once for each node of the triangle.
            instances[i].surfaceNormals.push_back(tNode);
            instances[i].surfaceNormals.push_back(tNode);
            instances[i].surfaceNormals.push_back(tNode);
        }
    }                                           // -- end i-loop over instances

    nEls[0] = t1elements.size();
    nEls[1] = p1elements.size();
    nEls[2] = h1elements.size();

    //fclose(fp);
}                                               // -- end buildInstancesNew


// ------------------------------------------------------------------------------------------------
void ReadFileAbaqus::data2File(QString name, vector<UM_node> &nodes,  vector<int> &vertices)
{
    FILE *fp;

    const char *c  = name.toStdString().c_str();

    if ((fp = fopen(c, "w")) != NULL)
    {
       for(size_t i=0; i < (int) nodes.size(); ++i)
       {
           fprintf(fp, "Node: %4ld\t Loc: %10.6f\t %10.6f\t %10.6f\n", i, nodes[i].x, nodes[i].y, nodes[i].z);
       }
       fprintf(fp, "\n");
       for(size_t i=0; i < (int) vertices.size(); )
       {
           fprintf(fp, "Connect: %4ld\t Loc: %8d\t %8d\t %8d\n",
                   i, vertices[i], vertices[i+1], vertices[i+2]);
           i += 3;
       }
    }
    fclose(fp);
}
// ------------------------------------------------------------------------------------------------
void ReadFileAbaqus::fprintElements(QString ss)
{
    if ((fp = fopen(ss.toLatin1(), "w")) == NULL)
    {
        qDebug() << "-- error opening ReadFileAbaqus debug file :" << ss.toLatin1() << "\n";
    }
    int i, last1;
    last1 = -1;
    for(i=0; i<nEls[0]; ++i)
    {
        if (t1elements[i].instance != last1)
        {
            fprintf(fp, "tet: %5d\t element: %6d\t inst: %d\t mat: %d\n", i, t1elements[i].element, t1elements[i].instance,
                t1elements[i].material);
            last1 = t1elements[i].instance;
        }
    }
    last1 = -1;
    for(i=0; i<nEls[1]; ++i)
    {
        if (p1elements[i].instance != last1)
        {
            fprintf(fp, "tet: %5d\t element: %6d\t inst: %d\t mat: %d\n", i, p1elements[i].element, p1elements[i].instance,
                p1elements[i].material);
            last1 = p1elements[i].instance;
        }
    }

    last1 = -1;
    for(i=0; i<nEls[2]; ++i)
    {
        if (h1elements[i].instance != last1)
        {
            fprintf(fp, "hex: %5d\t element: %6d\t inst: %d\t mat: %d\n", i, h1elements[i].element, h1elements[i].instance,
                h1elements[i].material);
                last1 = h1elements[i].instance;
        }
    }

    for(i=0; i<nEls[0]; ++i)
    {
        {
            fprintf(fp, "tet: %5d\t element: %6d\t inst: %d\t mat: %d\n", i, t1elements[i].element, t1elements[i].instance,
                t1elements[i].material);
        }
    }
    for(i=0; i<nEls[1]; ++i)
    {
        {
            fprintf(fp, "tpent: %5d\t element: %6d\t inst: %d\t mat: %d\n", i, p1elements[i].element, p1elements[i].instance,
                p1elements[i].material);
        }
    }

    for(i=0; i<nEls[2]; ++i)
    {
        {
            fprintf(fp, "hex: %5d\t element: %6d\t inst: %d\t mat: %d\n", i, h1elements[i].element, h1elements[i].instance,
                h1elements[i].material);
        }
    }
    fclose(fp);
}
// ------------------------------------------------------------------------------------------------
void ReadFileAbaqus::calculateCentroids()
{
    size_t  i;
    int     j, node;
    double  x, y, z;

    for (i=0; i<t1elements.size(); ++i)
    {
        x = 0.0;    y = 0.0;    z = 0.0;
        for (j=0; j<4; ++j)
        {
            node = t1elements[i].connect[j];
            x += nodes[node].x;
            y += nodes[node].y;
            z += nodes[node].z;
            //qDebug() << "node: " << node << "\tx: " << x << "\ty: " << y << "\tz: " << z ;
        }
        x *= 0.25;  y *= 0.25;  z *= 0.25;
        t1elements[i].centroid[0] = x;
        t1elements[i].centroid[1] = y;
        t1elements[i].centroid[2] = z;
        //qDebug() << "t1 centroid:: " << i << "\t" << x << "\t" << y << "\t" << z;
    }

    for (i=0; i<p1elements.size(); ++i)
    {
        x = 0.0;    y = 0.0;    z = 0.0;
        for (j=0; j<6; ++j)
        {
            node = p1elements[i].connect[j];
            x += nodes[node].x;
            y += nodes[node].y;
            z += nodes[node].z;
            //qDebug() << "node: " << node << "\tx: " << x << "\ty: " << y << "\tz: " << z ;
        }
        x = x / 6.0;  y = y / 6.0;  z = z / 6.0;
        p1elements[i].centroid[0] = x;
        p1elements[i].centroid[1] = y;
        p1elements[i].centroid[2] = z;
        //qDebug() << "p1 centroid:: " << i << "\t" << x << "\t" << y << "\t" << z;
    }

    for (i=0; i<h1elements.size(); ++i)
    {
        x = 0.0;    y = 0.0;    z = 0.0;
        for (j=0; j<8; ++j)
        {
            node = h1elements[i].connect[j];
            x += nodes[node].x;
            y += nodes[node].y;
            z += nodes[node].z;
            //qDebug() << "node: " << node << "\tx: " << x << "\ty: " << y << "\tz: " << z ;
        }
        x *= 0.125;  y *= 0.125;  z *= 0.125;
        h1elements[i].centroid[0] = x;
        h1elements[i].centroid[1] = y;
        h1elements[i].centroid[2] = z;
        //qDebug() << "h1 centroid:: " << i << "\t" << x << "\t" << y << "\t" << z;
    }

    for (i=0; i<t2elements.size(); ++i)
    {
        x = 0.0;    y = 0.0;    z = 0.0;
        for (j=0; j<10; ++j)
        {
            node = t2elements[i].connect[j];
            x += nodes[node].x;
            y += nodes[node].y;
            z += nodes[node].z;
            //qDebug() << "node: " << node << "\tx: " << x << "\ty: " << y << "\tz: " << z ;
        }
        x *= 0.1;  y *= 0.1;  z *= 0.1;
        t2elements[i].centroid[0] = x;
        t2elements[i].centroid[1] = y;
        t2elements[i].centroid[2] = z;
        //qDebug() << "t2 centroid:: " << i << "\t" << x << "\t" << y << "\t" << z;
    }

    for (i=0; i<p2elements.size(); ++i)
    {
        x = 0.0;    y = 0.0;    z = 0.0;
        for (j=0; j<15; ++j)
        {
            node = p2elements[i].connect[j];
            x += nodes[node].x;
            y += nodes[node].y;
            z += nodes[node].z;
            //qDebug() << "node: " << node << "\tx: " << x << "\ty: " << y << "\tz: " << z ;
        }
        x = x / 15.0;  y = y / 15.0;  z = z / 15.0;
        p2elements[i].centroid[0] = x;
        p2elements[i].centroid[1] = y;
        p2elements[i].centroid[2] = z;
        //qDebug() << "p2 centroid:: " << i << "\t" << x << "\t" << y << "\t" << z;
    }

    for (i=0; i<h2elements.size(); ++i)
    {
        x = 0.0;    y = 0.0;    z = 0.0;
        for (j=0; j<20; ++j)
        {
            node = h2elements[i].connect[j];
            x += nodes[node].x;
            y += nodes[node].y;
            z += nodes[node].z;
            //qDebug() << "node: " << node << "\tx: " << x << "\ty: " << y << "\tz: " << z ;
        }
        x *= 0.05;  y *= 0.05;  z *= 0.05;
        h2elements[i].centroid[0] = x;
        h2elements[i].centroid[1] = y;
        h2elements[i].centroid[2] = z;
        //qDebug() << "h2 centroid:: " << i << "\t" << x << "\t" << y << "\t" << z;
    }
}

// ------------------------------------------------------------------------------------------------
void ReadFileAbaqus::instanceColors()
{
    int i;
    nInstances = instances.size();

    int maxColors = 255;
    vector<int> red(nInstances);
    vector<int> gre(nInstances);
    vector<int> blu(nInstances);
    setRGBcolors(maxColors, nInstances, red, gre, blu);
    // --------------------------------------------------------------

    for(i=0; i<nInstances; ++i)
    {
        instances[i].color.setRed(red[i]);
        instances[i].color.setGreen(gre[i]);
        instances[i].color.setBlue(blu[i]);

 //       qDebug() << "Instance color: " << red[i] << gre[i] << blu[i];
    }
}                                               // -- instanceColors
// ------------------------------------------------------------------------------------------------

char ReadFileAbaqus::getPartNodes(QTextStream &in, int &countNode2, QString &lineOut,
                                  vector<P_nodes> &pNodes, QMap<int, int> &nodeMap)
{
    QString     line;
    QStringList fields;

    //int     count;
    int     num;
    P_nodes tNode;
    char    returnValue;

    returnValue = 0;
    //count = 0;
    tNode.x = 0.0;
    tNode.y = 0.0;
    tNode.z = 0.0;

    // Store zero in the 0'th slot so that the connectivity information
    // won't need to be re-numbered.
    pNodes.push_back(tNode);
    pNodeMap.clear();
    nodeMap.clear();

    line   = in.readLine();
    //qDebug() << "getPartNodes::> " << line;

    while (!line.startsWith("*"))
    {
        fields = line.split(',', QString::SkipEmptyParts);
        if (fields.size() != 4)
        {
            qDebug() << ">>>>> More than 4 entries on node line.";
        }

        num     = fields.takeFirst().toInt();
        tNode.x = fields.takeFirst().toDouble();
        tNode.y = fields.takeFirst().toDouble();
        tNode.z = fields.takeFirst().toDouble();

        //qDebug() << "countNode2: " << countNode2 << "\t" << tNode.x << "\t" << tNode.y << "\t" << tNode.z;
        ++countNode2;

        pNodeMap.insert(num, countNode2);
        nodeMap.insert(num, countNode2);

        // -- Put this in assembly processing
        //if ( tNode.x < this->nodesMinX )  this->nodesMinX = tNode.x;
        //if ( tNode.x > this->nodesMaxX )  this->nodesMaxX = tNode.x;
        //if ( tNode.y < this->nodesMinY )  this->nodesMinY = tNode.y;
        //if ( tNode.y > this->nodesMaxY )  this->nodesMaxY = tNode.y;
        //if ( tNode.z < this->nodesMinZ )  this->nodesMinZ = tNode.z;
        //if ( tNode.z > this->nodesMaxZ )  this->nodesMaxZ = tNode.z;

        pNodes.push_back(tNode);

        line   = in.readLine();
    }

    lineOut = line;
    return returnValue;
}
// ------------------------------------------------------------------------------------------------
void ReadFileAbaqus::getPartConnectivity(QTextStream &in, int &countElement, QString &lineOut, ABaqus_part &tPart)
{
    QString     line;
    QStringList fields;
    int         i;
    int         elNo;
    int         num;
    int         fSize1, fSize2;

    //    qDebug() << "getPartConnectivity:: lineOut: " << lineOut;

    lineOut = lineOut.toLower();
    fields = lineOut.split('=', QString::SkipEmptyParts);
    line   = fields.takeLast().toLower();

    //qDebug() << "getPartConnectivity:: line to lower " << line;

//    if (line.startsWith("c3d8"))
    if (lineOut.indexOf("c3d8") > 0)
    {
        P_h1element tEl;
        line = in.readLine();

        while (!line.startsWith("*"))
        {
            fields = line.split(',', QString::SkipEmptyParts);
            if (fields.size() != 9)
            {
                qDebug() << ">>>>> 9 entries NOT on 1st order hex connectivity line.";
                qDebug() << line;
            }

            ++countElement;
            num = fields.takeFirst().toInt();
            pConnectMap.insert(num, countElement);
            for (i=0; i<8; ++i)
            {
                elNo  = fields.takeFirst().toInt();
                tEl.connect[i] = elNo;
                if (elNo < tPart.minElNo) tPart.minElNo = elNo;
                if (elNo > tPart.maxElNo) tPart.maxElNo = elNo;
            }
            tPart.h1.push_back(tEl);
            line = in.readLine();
        }           // end while loop
    }               // end c3d8

//    if (line.startsWith("c3d6"))
    if (lineOut.indexOf("c3d6") > 0)
    {
        P_p1element tEl;
        line = in.readLine();

        while (!line.startsWith("*"))
        {
            fields = line.split(',', QString::SkipEmptyParts);
            if (fields.size() != 7)
            {
                qDebug() << ">>>>> 7 entries NOT on 1st order pent connectivity line.";
                qDebug() << line;
            }

            ++countElement;
            num = fields.takeFirst().toInt();
            pConnectMap.insert(num, countElement);
            for (i=0; i<6; ++i)
            {
                elNo  = fields.takeFirst().toInt();
                tEl.connect[i] = elNo;
                if (elNo < tPart.minElNo) tPart.minElNo = elNo;
                if (elNo > tPart.maxElNo) tPart.maxElNo = elNo;
            }
            tPart.p1.push_back(tEl);
            line = in.readLine();
        }                           // end while loop

        //qDebug() << "** c3d6 ** number of nodes: " << tPart.nodes.size();
        //for(i=0; i<tPart.nodes.size(); ++i)
        //{
        //    qDebug() << "i: " << i << "\t" << tPart.nodes[i].x << "\t" << tPart.nodes[i].y << "\t" << tPart.nodes[i].z;
        //}
    }               // end c3d6

//    if (line.startsWith("c3d4"))
    if (lineOut.indexOf("c3d4") > 0)
    {
        P_t1element tEl;
        line = in.readLine();

        while (!line.startsWith("*"))
        {
            fields = line.split(',', QString::SkipEmptyParts);
            if (fields.size() != 5)
            {
                qDebug() << ">>>>> 5 entries NOT on 1st order tet connectivity line.";
                qDebug() << line;
            }

            ++countElement;
            num = fields.takeFirst().toInt();
            pConnectMap.insert(num, countElement);
            for (i=0; i<4; ++i)
            {
                elNo  = fields.takeFirst().toInt();
                tEl.connect[i] = elNo;
                if (elNo < tPart.minElNo) tPart.minElNo = elNo;
                if (elNo > tPart.maxElNo) tPart.maxElNo = elNo;
            }
            tPart.t1.push_back(tEl);
            line = in.readLine();
        }           // end while loop
    }               // end c3d4

//    if (line.startsWith("c3d10"))
    if (lineOut.indexOf("c3d10") > 0)
    {
        P_t2element tEl;
        line = in.readLine();

        while (!line.startsWith("*"))
        {
            fields = line.split(',', QString::SkipEmptyParts);
            if (fields.size() != 11)
            {
                qDebug() << ">>>>> 11 entries NOT on 2nd order tet connectivity line.";
                qDebug() << line;
            }

            ++countElement;
            num = fields.takeFirst().toInt();
            pConnectMap.insert(num, countElement);
            for (i=0; i<10; ++i)
            {
                tEl.connect[i] = fields.takeFirst().toInt();
            }
            tPart.t2.push_back(tEl);
            line = in.readLine();
        }           // end while loop
    }               // end c3d8

//    if (line.startsWith("c3d15"))
    if (lineOut.indexOf("c3d15") > 0)
    {
        P_p2element tEl;
        line = in.readLine();

        while (!line.startsWith("*"))
        {
            fields = line.split(',', QString::SkipEmptyParts);
            if (fields.size() != 16)
            {
                qDebug() << ">>>>> 11 entries NOT on 2nd order pent connectivity line.";
                qDebug() << line;
            }

            ++countElement;
            num = fields.takeFirst().toInt();
            pConnectMap.insert(num, countElement);
            for (i=0; i<15; ++i)
            {
                tEl.connect[i] = fields.takeFirst().toInt();
            }
            tPart.p2.push_back(tEl);
            line = in.readLine();
        }           // end while loop
    }               // end c3d10

//    if (line.startsWith("c3d20"))
    if (lineOut.indexOf("c3d20") > 0)
    {
        P_h2element tEl;
        line = in.readLine();

        while (!line.startsWith("*"))
        {
            fields = line.split(',', QString::SkipEmptyParts);
            fSize1 = fields.size();

            ++countElement;
            num = fields.takeFirst().toInt();
            pConnectMap.insert(num, countElement);

            --fSize1;
            for (i=0; i<fSize1; ++i)
            {
                tEl.connect[i] = fields.takeFirst().toInt();
            }   

            if (fSize1 != 20)
            {
                line   = in.readLine();
                fields = line.split(',', QString::SkipEmptyParts);
                fSize2 = fields.size();
                fSize2 += fSize1;
                if (fSize2 != 20)
                {
                    qDebug() << ">>>>> 21 entries NOT on 2nd order hex connectivity line.";
                    qDebug() << line;
                }
                else
                {
                    for (i=(fSize1); i<fSize2; ++i)
                    {
                        tEl.connect[i] = fields.takeFirst().toInt();
                    }
                }
            }
            tPart.h2.push_back(tEl);
            line = in.readLine();
        }           // end while loop

        //qDebug() << "get connectivity:: h2 size: " << tPart.h2.size();
    }               // end c3d60

    //qDebug() << "# of t1 elements: " << tPart.t1.size();
    //qDebug() << "# of p1 elements: " << tPart.p1.size();
    //qDebug() << "# of h1 elements: " << tPart.h1.size();
    tPart.elNumbers[0] = tPart.t1.size();
    tPart.elNumbers[1] = tPart.p1.size();
    tPart.elNumbers[2] = tPart.h1.size();
    tPart.elNumbers[3] = tPart.t2.size();
    tPart.elNumbers[4] = tPart.p2.size();
    tPart.elNumbers[5] = tPart.h2.size();
    lineOut = line;
}
// ------------------------------------------------------------------------------------------------
void ReadFileAbaqus::endPartProcessing(ABaqus_part &tPart)
{
    //qDebug() << " -- start endPartProcessing --";

    // ****************************************************************************
    // * For the Abaqus flat file, the ABaqus_part pNodeMap will convert from
    // * The "master" part list of nodes to the part specific list of nodes.
    // * For a part differentiated Abaqus file, this is probably a direct
    // * mapping, but need the machinary in place for both types of Abaqus files.
    // ****************************************************************************

    int i, j;
    int nel1, nel2;
    int nSize = tPart.t1.size();
    //qDebug() << " number of 1st order tets: " << nSize;
    // Re-order the node indices in the connectivity lists.
    for (i=0; i<nSize; ++i)
    {
        for (j=0; j<4; ++j)
        {
            nel1 = tPart.t1[i].connect[j];
            nel2 = tPart.pNodeMap.value(nel1);
            tPart.t1[i].connect[j] = nel2;
        }
    }

    nSize = tPart.p1.size();
    //qDebug() << " number of 1st order pents: " << nSize;
    // Re-order the node indices in the connectivity lists.
    for (i=0; i<nSize; ++i)
    {
        for (j=0; j<6; ++j)
        {
            nel1 = tPart.p1[i].connect[j];
            nel2 = tPart.pNodeMap.value(nel1);
            tPart.p1[i].connect[j] = nel2;
        }
    }
    nSize = tPart.h1.size();
    //qDebug() << " number of 1st order hexs: " << nSize;
    // Re-order the node indices in the connectivity lists.
    for (i=0; i<nSize; ++i)
    {
        for (j=0; j<8; ++j)
        {
            nel1 = tPart.h1[i].connect[j];
            nel2 = tPart.pNodeMap.value(nel1);
            tPart.h1[i].connect[j] = nel2;
        }
    }

    nSize = tPart.t2.size();
    //qDebug() << " number of 2nd order tets: " << nSize;
    // Re-order the node indices in the connectivity lists.
    for (i=0; i<nSize; ++i)
    {
        for (j=0; j<10; ++j)
        {
            nel1 = tPart.t2[i].connect[j];
            nel2 = tPart.pNodeMap.value(nel1);
            tPart.t2[i].connect[j] = nel2;
        }
    }

    nSize = tPart.p2.size();
    //qDebug() << " number of 2nd order pents: " << nSize;
    // Re-order the node indices in the connectivity lists.
    for (i=0; i<nSize; ++i)
    {
        for (j=0; j<15; ++j)
        {
            nel1 = tPart.p2[i].connect[j];
            nel2 = tPart.pNodeMap.value(nel1);
            tPart.p2[i].connect[j] = nel2;
        }
    }
    nSize = tPart.h2.size();
    //qDebug() << " number of 2nd order hexs: " << nSize;
    // Re-order the node indices in the connectivity lists.
    for (i=0; i<nSize; ++i)
    {
        for (j=0; j<20; ++j)
        {
            nel1 = tPart.h2[i].connect[j];
            nel2 = tPart.pNodeMap.value(nel1);
            tPart.h2[i].connect[j] = nel2;
        }
    }

    tPart.findSurfaceElements();
    tPart.findSurfaceTriangles();
    //qDebug() << "-- end of endPartProcessing --";
}

// ------------------------------------------------------------------------------------------------
void ReadFileAbaqus::getMaterialNumbers(QTextStream &in)
{
    QString     line;
    QStringList fields;

    int         j;
    int         count;
    int         num;

    count = 0;
    this->matNos.clear();
    while (count < this->nElements)
    {
        line   = in.readLine();
        fields = line.split(' ', QString::SkipEmptyParts);
        count += fields.size();
        num    = fields.size();
        for (j=0; j<num; j++)
        {
            this->matNos.push_back(fields.takeFirst().toInt() - 1);
        }
    }
    //qDebug() << "last mat no: " << this->matNos[count-1] << "count: " << count;
    //qDebug() << "nElement: " << this->nElements;
}
// ----------------------------------------------------------------------------
void ReadFileAbaqus::finalizeInstances()
{
    int i, j, k, n;
    int numEdits;
    int node;
    int *instElMin, *instElMax;
    int objMinEl,   objMaxEl;
    int currentEl[6];

    double  tValue;
    double  *minValue, *maxValue;
    double  minX, minY, minZ;
    double  maxX, maxY, maxZ;
    double  xArray[20], yArray[20], zArray[20];

    vector<double>  tempX, tempY, tempZ;

    //qDebug() << "* finalizeInstances * \t nInstances:" << nInstances;

    numEdits  = this->edits.size();
    minValue  = new double[numEdits];
    maxValue  = new double[numEdits];
    instElMin = new int[numEdits];
    instElMax = new int[numEdits];

    //QString ss;
    //ss = "/Users/roger/Public/Programs/TestFiles/abaqusReadFile_debug_elements3.txt";
    //if ((fp = fopen(ss.toLatin1(), "w")) == NULL)
    //{
    //    qDebug() << "-- error opening ReadFileAbaqus debug file :" << ss.toLatin1() << "\n";
    //}

    //for (i=0; i<5; ++i)
    //    qDebug() << "elAdjust: " << elAdjust[i];
    for (i=0; i<5; ++i)
        currentEl[i] = elAdjust[i];

    for (i=0; i<this->nInstances; i++)
    {
        for(n=0; n<numEdits; ++n) minValue[n] =  9.9e+39;
        for(n=0; n<numEdits; ++n) maxValue[n] = -9.9e+39;
        tempX.clear();
        tempY.clear();
        tempZ.clear();
        instances[i].volume = 0.0;
        instances[i].mass   = 0.0;
        instances[i].minResults.clear();
        instances[i].maxResults.clear();
        instances[i].elMin.clear();
        instances[i].elMax.clear();
        this->instances[i].surface.clear();
        this->instances[i].outline.clear();
        this->instances[i].results.clear();
        this->instances[i].results.reserve(numEdits);
        for (n=0; n <numEdits; n++)
        {
            this->instances[i].results.push_back(0.0);
        }

        //fprintf(fp, "\n ** Instance: %3d\n", i);

        //qDebug() << "instance: " << i;

        // Set the instance # for its 1st order tets.
        if (this->instances[i].elementLimits[0] > 0)
        {
            k = instances[i].elementLimits[0] - 1;
            instances[i].materialNo = this->matMap[ t1elements[k].material ];

            //qDebug() << " ++++ i: " << i << "\t materialNo: " << t1elements[k].material << "\t\t k: " << k;
            //fprintf(fp, "TETS:: elementLimits: %6d\t %6d\n", instances[i].elementLimits[0], instances[i].elementLimits[1]);
            
            for (j=this->instances[i].elementLimits[0]; j<=this->instances[i].elementLimits[1]; j++)
            {
                k = j - 1;
                this->t1elements[k].element  = currentEl[0];
                this->t1elements[k].instance = i;

                //fprintf(fp, "\t element: %6d \t instance: %3d\t tet: %6d\n", currentEl[0], i, k);
                ++currentEl[0];

                this->instances[i].volume += this->t1elements[k].volume;
                this->instances[i].mass   += this->t1elements[k].volume * this->t1elements[k].density;

                //qDebug() << "tet: " << k << "\t instance: " << i;

                // Collect the results by instance.
                for(int n=0; n<numEdits; n++)
                {
                    this->instances[i].results[n] += this->t1elements[k].results[n] * this->t1elements[k].volume;
                    if (t1elements[k].results[n] > maxValue[n])
                    {
                        maxValue[n]  = t1elements[k].results[n];
                        instElMax[n] = j;
                    }
                    if (t1elements[k].results[n] < minValue[n])
                    {
                        minValue[n]  = t1elements[k].results[n];
                        instElMin[n] = j;
                    }
                }

                for(int n=0; n<4; n++)
                {
                    node = this->t1elements[k].connect[n];
                    tempX.push_back(this->nodes[node].x);
                    tempY.push_back(this->nodes[node].y);
                    tempZ.push_back(this->nodes[node].z);
                    xArray[n] = this->nodes[node].x;
                    yArray[n] = this->nodes[node].y;
                    zArray[n] = this->nodes[node].z;
                }
                tValue = Min(xArray, 4);
                t1elements[k].sBoxes[0] = tValue;
                //qDebug() << "hex element: " << k << "\t min x: " << tValue;
                tValue = Min(yArray, 4);
                t1elements[k].sBoxes[1] = tValue;
                //qDebug() << "hex element: " << k << "\t min y: " << tValue;

                tValue = Min(zArray, 4);
                t1elements[k].sBoxes[2] = tValue;
                //qDebug() << "hex element: " << k << "\t min z: " << tValue;

                tValue = Max(xArray, 4);
                t1elements[k].sBoxes[3] = tValue;
                //qDebug() << "hex element: " << k << "\t max x: " << tValue;

                tValue = Max(yArray, 4);
                t1elements[k].sBoxes[4] = tValue;
                //qDebug() << "hex element: " << k << "\t max y: " << tValue;

                tValue = Max(zArray, 4);
                t1elements[k].sBoxes[5] = tValue;
                //qDebug() << "hex element: " << k << "\t max z: " << tValue;
            }
        }
        // Set the instance # for its 1st order pents.
        if (this->instances[i].elementLimits[2] > 0)
        {
            k = instances[i].elementLimits[2] - elAdjust[1] - 1;
            instances[i].materialNo = this->matMap[ p1elements[k].material ];

            //fprintf(fp, "PENTS:: elementLimits: %6d\t %6d\n", instances[i].elementLimits[2], instances[i].elementLimits[3]);

            //qDebug() << " ++++ i: " << i << "\t materialNo: " << p1elements[k].material << "\t\t k: " << k;
            //qDebug() << "el limits: " << instances[i].elementLimits[2] << "\t" << instances[i].elementLimits[3];
            //qDebug() << "size of p1elements: " << p1elements.size();
            //qDebug() << "elAdjust          : " << elAdjust[1];
            
            for (j=this->instances[i].elementLimits[2]; j<=this->instances[i].elementLimits[3]; j++)
            {
                k = j - elAdjust[1] - 1;
                this->p1elements[k].element  = currentEl[1];
                this->p1elements[k].instance = i;

               // fprintf(fp, "\t element: %6d \t instance: %3d\t pent: %6d\n", currentEl[1], i, k);

                this->instances[i].volume += this->p1elements[k].volume;
                this->instances[i].mass   += this->p1elements[k].volume * this->p1elements[k].density;
                ++currentEl[1];

                for(int n=0; n<6; n++)
                {
                    //qDebug() << "k: " << k << "\t n: " << n << "\tnode: " << node;

                    node = this->p1elements[k].connect[n];
                    tempX.push_back(this->nodes[node].x);
                    tempY.push_back(this->nodes[node].y);
                    tempZ.push_back(this->nodes[node].z);
                    xArray[n] = this->nodes[node].x;
                    yArray[n] = this->nodes[node].y;
                    zArray[n] = this->nodes[node].z;
                }
                tValue = Min(xArray, 6);
                p1elements[k].sBoxes[0] = tValue;
                //qDebug() << "hex element: " << k << "\t min x: " << tValue;
                tValue = Min(yArray, 6);
                p1elements[k].sBoxes[1] = tValue;
                //qDebug() << "hex element: " << k << "\t min y: " << tValue;

                tValue = Min(zArray, 6);
                p1elements[k].sBoxes[2] = tValue;
                //qDebug() << "hex element: " << k << "\t min z: " << tValue;

                tValue = Max(xArray, 6);
                p1elements[k].sBoxes[3] = tValue;
                //qDebug() << "hex element: " << k << "\t max x: " << tValue;

                tValue = Max(yArray, 6);
                p1elements[k].sBoxes[4] = tValue;
                //qDebug() << "hex element: " << k << "\t max y: " << tValue;

                tValue = Max(zArray, 6);
                p1elements[k].sBoxes[5] = tValue;
                //qDebug() << "hex element: " << k << "\t max z: " << tValue;
            }
        }

        //qDebug() << "Limits: " << this->instances[i].elementLimits[4] << this->instances[i].elementLimits[5];
        // Set the instance # for its 1st order hexs.
        if (this->instances[i].elementLimits[4] > 0)
        {
            //fprintf(fp, "HEXS:: elementLimits: %6d\t %6d\n", instances[i].elementLimits[4], instances[i].elementLimits[5]);

            k = instances[i].elementLimits[4] - elAdjust[2] - 1;
            instances[i].materialNo = this->matMap[ h1elements[k].material ];

            //qDebug() << " ++++ i: " << i << "\t materialNo: " << h1elements[k].material << "\t\t k: " << k;

            for (j=this->instances[i].elementLimits[4]; j<=this->instances[i].elementLimits[5]; j++)
            {
                k = j - elAdjust[2] - 1;
                this->h1elements[k].element  = currentEl[2];
                this->h1elements[k].instance = i;

                //fprintf(fp, "\t element: %6d \t instance: %3d\t hex: %6d\n", currentEl[2], i, k);
                ++currentEl[2];

                this->instances[i].volume += this->h1elements[k].volume;
                this->instances[i].mass   += this->h1elements[k].volume * this->h1elements[k].density;

                for(int n=0; n<8; n++)
                {
                    node = this->h1elements[k].connect[n];
                    tempX.push_back(this->nodes[node].x);
                    tempY.push_back(this->nodes[node].y);
                    tempZ.push_back(this->nodes[node].z);
                    xArray[n] = this->nodes[node].x;
                    yArray[n] = this->nodes[node].y;
                    zArray[n] = this->nodes[node].z;
                }
                tValue = Min(xArray, 8);
                h1elements[k].sBoxes[0] = tValue;
                //qDebug() << "hex element: " << k << "\t min x: " << tValue;
                tValue = Min(yArray, 8);
                h1elements[k].sBoxes[1] = tValue;
                //qDebug() << "hex element: " << k << "\t min y: " << tValue;

                tValue = Min(zArray, 8);
                h1elements[k].sBoxes[2] = tValue;
                //qDebug() << "hex element: " << k << "\t min z: " << tValue;

                tValue = Max(xArray, 8);
                h1elements[k].sBoxes[3] = tValue;
                //qDebug() << "hex element: " << k << "\t max x: " << tValue;

                tValue = Max(yArray, 8);
                h1elements[k].sBoxes[4] = tValue;
                //qDebug() << "hex element: " << k << "\t max y: " << tValue;

                tValue = Max(zArray, 8);
                h1elements[k].sBoxes[5] = tValue;
                //qDebug() << "hex element: " << k << "\t max z: " << tValue;
            }
        }
        // Set the instance # for its 2nd order tets.
        if (this->instances[i].elementLimits[6] > 0)
        {
            k = instances[i].elementLimits[6] - elAdjust[3] - 1;
            instances[i].materialNo = this->matMap[ t2elements[k].material ];

            //qDebug() << " ++++ i: " << i << "\t materialNo: " << p1elements[k].material << "\t\t k: " << k;
            
            for (j=this->instances[i].elementLimits[6]; j<=this->instances[i].elementLimits[7]; j++)
            {
                k = j - elAdjust[3] - 1;
                this->t2elements[k].element  = currentEl[3];
                this->t2elements[k].instance = i;
                this->instances[i].volume += this->t2elements[k].volume;
                this->instances[i].mass   += this->t2elements[k].volume * this->t2elements[k].density;
                ++currentEl[3];

                for(int n=0; n<4; n++)
                {
                    node = this->t2elements[k].connect[n];
                    tempX.push_back(this->nodes[node].x);
                    tempY.push_back(this->nodes[node].y);
                    tempZ.push_back(this->nodes[node].z);
                    xArray[n] = this->nodes[node].x;
                    yArray[n] = this->nodes[node].y;
                    zArray[n] = this->nodes[node].z;
                }
                tValue = Min(xArray, 4);
                t2elements[k].sBoxes[0] = tValue;
                //qDebug() << "hex element: " << k << "\t min x: " << tValue;
                tValue = Min(yArray, 4);
                t2elements[k].sBoxes[1] = tValue;
                //qDebug() << "hex element: " << k << "\t min y: " << tValue;

                tValue = Min(zArray, 4);
                t2elements[k].sBoxes[2] = tValue;
                //qDebug() << "hex element: " << k << "\t min z: " << tValue;

                tValue = Max(xArray, 4);
                t2elements[k].sBoxes[3] = tValue;
                //qDebug() << "hex element: " << k << "\t max x: " << tValue;

                tValue = Max(yArray, 4);
                t2elements[k].sBoxes[4] = tValue;
                //qDebug() << "hex element: " << k << "\t max y: " << tValue;

                tValue = Max(zArray, 4);
                t2elements[k].sBoxes[5] = tValue;
                //qDebug() << "hex element: " << k << "\t max z: " << tValue;
            }
        }
        // Set the instance # for its 2nd order pents.
        if (this->instances[i].elementLimits[8] > 0)
        {
            k = instances[i].elementLimits[8] - elAdjust[4] - 1;
            instances[i].materialNo = this->matMap[ p2elements[k].material ];

            //qDebug() << " ++++ i: " << i << "\t materialNo: " << p2elements[k].material << "\t\t k: " << k;

            for (j=this->instances[i].elementLimits[8]; j<=this->instances[i].elementLimits[9]; j++)
            {
                k = j - elAdjust[4] - 1;
                this->p2elements[k].element  = currentEl[4];
                this->p2elements[k].instance = i;
                this->instances[i].volume += this->p2elements[k].volume;
                this->instances[i].mass   += this->p2elements[k].volume * this->p2elements[k].density;
                ++currentEl[4];

                for(int n=0; n<6; n++)
                {
                    node = this->p2elements[k].connect[n];
                    tempX.push_back(this->nodes[node].x);
                    tempY.push_back(this->nodes[node].y);
                    tempZ.push_back(this->nodes[node].z);
                    xArray[n] = this->nodes[node].x;
                    yArray[n] = this->nodes[node].y;
                    zArray[n] = this->nodes[node].z;
                }
                tValue = Min(xArray, 6);
                p2elements[k].sBoxes[0] = tValue;
                //qDebug() << "hex element: " << k << "\t min x: " << tValue;
                tValue = Min(yArray, 6);
                p2elements[k].sBoxes[1] = tValue;
                //qDebug() << "hex element: " << k << "\t min y: " << tValue;

                tValue = Min(zArray, 6);
                p2elements[k].sBoxes[2] = tValue;
                //qDebug() << "hex element: " << k << "\t min z: " << tValue;

                tValue = Max(xArray, 6);
                p2elements[k].sBoxes[3] = tValue;
                //qDebug() << "hex element: " << k << "\t max x: " << tValue;

                tValue = Max(yArray, 6);
                p2elements[k].sBoxes[4] = tValue;
                //qDebug() << "hex element: " << k << "\t max y: " << tValue;

                tValue = Max(zArray, 6);
                p2elements[k].sBoxes[5] = tValue;
                //qDebug() << "hex element: " << k << "\t max z: " << tValue;
            }
        }

        // Set the instance # for its 2nd order hexs.
        if (this->instances[i].elementLimits[10] > 0)
        {
            k = instances[i].elementLimits[10] - elAdjust[5] - 1;
            instances[i].materialNo = this->matMap[ h2elements[k].material ];

            //qDebug() << " ++++ i: " << i << "\t materialNo: " << h2elements[k].material << "\t\t k: " << k;
            //qDebug() << "el limits: " << instances[i].elementLimits[10] << "\t" << instances[i].elementLimits[11];
            //qDebug() << "size of h2elements: " << h2elements.size();
            //qDebug() << "elAdjust          : " << elAdjust[5];

            for (j=this->instances[i].elementLimits[10]; j<=this->instances[i].elementLimits[11]; ++j)
            {
                k = j - elAdjust[5] - 1;
                //qDebug() << "k: " << k << "\t j: " << j;
                this->h2elements[k].element  = currentEl[5];
                this->h2elements[k].instance = i;
                this->instances[i].volume += this->h2elements[k].volume;
                this->instances[i].mass   += this->h2elements[k].volume * this->h2elements[k].density;
                ++currentEl[5];

                for(int n=0; n<8; n++)
                {
                    node = this->h2elements[k].connect[n];

                    //qDebug() << "k: " << k << "\t n: " << n << "\tnode: " << node;

                    tempX.push_back(this->nodes[node].x);
                    tempY.push_back(this->nodes[node].y);
                    tempZ.push_back(this->nodes[node].z);
                    xArray[n] = this->nodes[node].x;
                    yArray[n] = this->nodes[node].y;
                    zArray[n] = this->nodes[node].z;
                }
                tValue = Min(xArray, 8);
                h2elements[k].sBoxes[0] = tValue;
                //qDebug() << "hex element: " << k << "\t min x: " << tValue;
                tValue = Min(yArray, 8);
                h2elements[k].sBoxes[1] = tValue;
                //qDebug() << "hex element: " << k << "\t min y: " << tValue;

                tValue = Min(zArray, 8);
                h2elements[k].sBoxes[2] = tValue;
                //qDebug() << "hex element: " << k << "\t min z: " << tValue;

                tValue = Max(xArray, 8);
                h2elements[k].sBoxes[3] = tValue;

                //qDebug() << "hex element: " << k << "\t max x: " << tValue;

                tValue = Max(yArray, 8);
                h2elements[k].sBoxes[4] = tValue;
                //qDebug() << "hex element: " << k << "\t max y: " << tValue;

                tValue = Max(zArray, 8);
                h2elements[k].sBoxes[5] = tValue;
                //qDebug() << "hex element: " << k << "\t max z: " << tValue;
            }
        }
        minX = Min(tempX);
        maxX = Max(tempX);
        this->instances[i].centroid[0]   = (minX+maxX)/2.0;
        this->instances[i].bounds.box[0] = minX;
        this->instances[i].bounds.box[3] = maxX;
        minY = Min(tempY);
        maxY = Max(tempY);
        this->instances[i].centroid[1]   = (minY+maxY)/2.0;
        this->instances[i].bounds.box[1] = minY;
        this->instances[i].bounds.box[4] = maxY;
        minZ = Min(tempZ);
        maxZ = Max(tempZ);
        this->instances[i].centroid[2]   = (minZ+maxZ)/2.0;
        this->instances[i].bounds.box[2] = minZ;
        this->instances[i].bounds.box[5] = maxZ;

        //qDebug() << "inst: " << i << "\t centroid: " << instances[i].centroid[0]  << instances[i].centroid[1]  << instances[i].centroid[2];

        for(n=0; n<numEdits; ++n)
        {
            instances[i].minResults.push_back(minValue[n]);
            instances[i].maxResults.push_back(maxValue[n]);
            instances[i].elMin.push_back(instElMin[n]-1);
            instances[i].elMax.push_back(instElMax[n]-1);
        }
    }
    delete minValue;
    delete maxValue;

    this->elMaxResults.clear();
    this->elMinResults.clear();
    this->elMaxObj.clear();
    this->elMinObj.clear();

    for(n=0; n<numEdits; ++n)
    {
        minX =  9.9e+39;
        maxX = -9.9e+39;
        for (i=0; i<this->nInstances; i++)
        {
            if (instances[i].maxResults[n] > maxX)
            {
                maxX     = instances[i].maxResults[n];
                objMaxEl = instances[i].elMax[n];        // temp store the element number
            }
            if (instances[i].minResults[n] < minX)
            {
                minX     = instances[i].minResults[n];
                objMinEl = instances[i].elMin[n];        // temp store the element number
            }
        }
        //qDebug() << "edit: " << n << "\t max: " << maxX << "\t min: " << minX;
        this->elMaxResults.push_back(maxX);
        this->elMinResults.push_back(minX);
        this->elMaxObj.push_back(objMaxEl);
        this->elMinObj.push_back(objMinEl);
    }
    //qDebug() << " end finalize instances \n\n\n";

    //fclose(fp);
}
// ------------------------------------------------------------------------------------------------
void ReadFileAbaqus::buildCTNumbers()
{
    int i;
    for (i=0; i < this->nInstances; ++i)
    {
        instances[i].ctNumbers[0] = instances[i].elNumbers[0];
        for (int j = 1; j < 6; ++j)
        {
            instances[i].ctNumbers[j] = instances[i].ctNumbers[j-1] + instances[i].elNumbers[j] ;
        }

        //qDebug() << "Instance Adder & elNumbers: " << i;
        //for (int j = 0; j < 6; j++)
        //{
        //    qDebug() << "\t" << j << "\tadder: " <<  this->instances[i].adder[j]
        //            << "\telNumber:" << this->instances[i].elNumbers[j] << "\t ctNumbers: " << instances[i].ctNumbers[j];
        //}
    }

    for (int j = 0; j < 6; ++j)
    {
        instances[0].ctNumbers2[j] = 0;
    }

    for (i=1; i < this->nInstances; ++i)
    {
        //qDebug() << "-- instance: " << i;
        for (int j = 0; j < 6; ++j)
        {
            instances[i].ctNumbers2[j] = instances[i-1].ctNumbers2[j] + instances[i-1].elNumbers[j];
            //qDebug() << "j: " << j << "\t: ctNumbers2: " << instances[i].ctNumbers2[j];
        }
    }
}
// ------------------------------------------------------------------------------------------------
void ReadFileAbaqus::calculateElementVolumesByPart()
{
    int     i, j, k;
    size_t  p;
    double  t1points[3][4];
    double  p1points[3][6];
    double  h1points[3][8];
    double  t2points[3][10];
    double  p2points[3][15];
    double  h2points[3][20];

    //qDebug() << "* calculateVolumesByPart *\t" << parts.size();

    for (p=0; p<parts.size(); ++p)
    {
        for (i=0; i<parts[p].elNumbers[0]; ++i)
        {
            // Grab the node positions.
            for (j=0; j<4; ++j)
            {
                k = parts[p].t1[i].connect[j];             // node index
                t1points[0][j] = parts[p].nodes[k].x;
                t1points[1][j] = parts[p].nodes[k].y;
                t1points[2][j] = parts[p].nodes[k].z;
                //if (i==0) qDebug() << "index k: " << k << "\t XYZ: " << p1points[0][j] << "\t" << p1points[1][j] << "\t" << p1points[2][j];
            }
            parts[p].t1[i].volume = find1TetVolume(t1points);
            //qDebug() << "1st tet volume for " << i << "\t: " << parts[p].t1[i].volume;
            //exit(-999);

        }           // -- end i-loop for 1st order tets

        //qDebug() << "num of p1 elements: " << parts[p].elNumbers[1];
        for (i=0; i<parts[p].elNumbers[1]; ++i)
        {
            // Grab the node positions.
            for (j=0; j<6; ++j)
            {
                k = parts[p].p1[i].connect[j];             // node index
                p1points[0][j] = parts[p].nodes[k].x;
                p1points[1][j] = parts[p].nodes[k].y;
                p1points[2][j] = parts[p].nodes[k].z;
                //qDebug() << "index k: " << k << "\t XYZ: " << p1points[0][j] << "\t" << p1points[1][j] << "\t" << p1points[2][j];
            }
            parts[p].p1[i].volume = find1PentVolume(p1points);
            //qDebug() << "1st pent volume for " << i << "\t: " << parts[p].p1[i].volume;

        }           // -- end i-loop for 1st order pents

        //qDebug() << "num of h1 elements: " << parts[p].elNumbers[2];
        for (i=0; i<parts[p].elNumbers[2]; ++i)
        {
            // Grab the node positions.
            for (j=0; j<8; ++j)
            {
                k = parts[p].h1[i].connect[j];             // node index
                h1points[0][j] = parts[p].nodes[k].x;
                h1points[1][j] = parts[p].nodes[k].y;
                h1points[2][j] = parts[p].nodes[k].z;
                //qDebug() << "index k: " << k << "\t XYZ: " << p1points[0][j] << "\t" << p1points[1][j] << "\t" << p1points[2][j];
            }
            parts[p].h1[i].volume = find1HexVolume(h1points);
            //qDebug() << "1st hex volume for " << i << "\t: " << parts[p].h1[i].volume;

        }           // -- end i-loop for 1st order hexs

        //qDebug() << "num of t2 elements: " << parts[p].elNumbers[3];

        for (i=0; i<parts[p].elNumbers[3]; ++i)
        //for (i=0; i<1; ++i)
        {
            // Grab the node positions.
            for (j=0; j<10; ++j)
            {
                k = parts[p].t2[i].connect[j];             // node index
                t2points[0][j] = parts[p].nodes[k].x;
                t2points[1][j] = parts[p].nodes[k].y;
                t2points[2][j] = parts[p].nodes[k].z;
                //if (i==0) qDebug() << (j+1) << "index k: " << k << "\t XYZ: " << t2points[0][j] << "\t" << t2points[1][j] << "\t" << t2points[2][j];
            }
            parts[p].t2[i].volume = find2TetVolume(t2points);
            //qDebug() << "2nd tet volume for " << i << "\t: " << parts[p].t2[i].volume;

        }           // -- end i-loop for 2nd order tets

        for (i=0; i<parts[p].elNumbers[4]; ++i)
        {
            // Grab the node positions.
            for (j=0; j<15; ++j)
            {
                k = parts[p].p2[i].connect[j];             // node index
                p2points[0][j] = parts[p].nodes[k].x;
                p2points[1][j] = parts[p].nodes[k].y;
                p2points[2][j] = parts[p].nodes[k].z;
                //if (i==0) qDebug() << (j+1) << "index k: " << k << "\t XYZ: " << t2points[0][j] << "\t" << t2points[1][j] << "\t" << t2points[2][j];
            }
            parts[p].p2[i].volume = find2PentVolume(p2points);
            //qDebug() << "2nd pent volume for " << i << "\t: " << parts[p].p2[i].volume;

        }           // -- end i-loop for 2nd order tets

        //qDebug() << "num of h2 elements: " << parts[p].elNumbers[5];

        for (i=0; i<parts[p].elNumbers[5]; ++i)
        {
            // Grab the node positions.
            for (j=0; j<20; ++j)
            {
                k = parts[p].h2[i].connect[j];             // node index
                h2points[0][j] = parts[p].nodes[k].x;
                h2points[1][j] = parts[p].nodes[k].y;
                h2points[2][j] = parts[p].nodes[k].z;
                //qDebug() << (j+1) <<"\tindex k: " << k << "\t XYZ: " << h2points[0][j] << "\t" << h2points[1][j] << "\t" << h2points[2][j];
            }
            parts[p].h2[i].volume = find2HexVolume(h2points);
            //qDebug() << "2nd hex volume for " << i << "\t: " << parts[p].h2[i].volume;

        }           // -- end i-loop for 2nd order hexs
    }
}
// ----------------------------------------------------------------------------
void ReadFileAbaqus::calculatePartVolumes()
{
    int     i;
    size_t  p;

    //qDebug() << "* calculatePartVolumes *\t" << parts.size();

    for (p=0; p<parts.size(); ++p)
    {
        parts[p].volume = 0.0;
        for (i=0; i<parts[p].elNumbers[0]; ++i)
        {
            parts[p].volume += parts[p].t1[i].volume;
        }
        for (i=0; i<parts[p].elNumbers[1]; ++i)
        {
            parts[p].volume += parts[p].p1[i].volume;
        }
        for (i=0; i<parts[p].elNumbers[2]; ++i)
        {
            parts[p].volume += parts[p].h1[i].volume;
        }
        for (i=0; i<parts[p].elNumbers[3]; ++i)
        {
            parts[p].volume += parts[p].t2[i].volume;
        }
        for (i=0; i<parts[p].elNumbers[4]; ++i)
        {
            parts[p].volume += parts[p].p2[i].volume;
        }
        for (i=0; i<parts[p].elNumbers[5]; ++i)
        {
            parts[p].volume += parts[p].h2[i].volume;
        }

        //qDebug() << "-- part: " << p << "\t volume: " << parts[p].volume;
    }
}           // end calculatePartVolumes
// ------------------------------------------------------------------------------------------------

char ReadFileAbaqus::findNodeMatches(unsigned char neighborFace, int thisFace, int *nextNodes, int *thisNodes, int *nodes)
{
    int nMatches;
    int i, j;

    nMatches = 0;
    if ((neighborFace < 2) && (thisFace < 2))
    {
        for(i=0; i<3; ++i)
        {
            for(j=0; j<3; ++j)
            {
                if (nextNodes[i] == thisNodes[j])
                {
                    nodes[nMatches] = nextNodes[i];
                    ++nMatches;
                }
            }
            if (nMatches == 2) break;
        }
    }
    else if ((neighborFace > 1) && (thisFace > 1))
    {
        for(i=0; i<4; ++i)
        {
            for(j=0; j<4; ++j)
            {
                if (nextNodes[i] == thisNodes[j])
                {
                    nodes[nMatches] = nextNodes[i];
                    ++nMatches;
                }
            }
            if (nMatches == 2) break;
        }
    }
    return nMatches;
}

// ------------------------------------------------------------------------------------------------
void ReadFileAbaqus::getFaceNodes(short eType, int tEl, int face, int *nextNodes)
{
    int i;

    switch (eType)
    {
        case 0:
        // 1st Order tets
        for (i=0; i<4; ++i)
        {
            nextNodes[i] = t1elements[tEl].connect[ tetFaces[face][i] ];
        }
        break;

        case 3:
        // 2nd Order tets
        for (i=0; i<4; ++i)
        {
            nextNodes[i] = t2elements[tEl].connect[ tetFaces[face][i] ];
        }
        break;

        case 1:
        // 1st Order pents
        for (i=0; i<4; ++i)
        {
            nextNodes[i] = p1elements[tEl].connect[ pentFaces[face][i] ];
        }
        break;

        case 4:
        // 2nd Order pents
        for (i=0; i<4; ++i)
        {
            nextNodes[i] = p2elements[tEl].connect[ pentFaces[face][i] ];
        }
        break;
    }
}

// ------------------------------------------------------------------------------------------------
short ReadFileAbaqus::getNNSides(int nSides, int element, vector<int> &NND, int *sides)
{
    char    nZeros;
    int     n, k;

    nZeros = 0;
    for(n=0; n<nSides; ++n)
    {
        k = (element * nSides) + n;
        sides[n] = NND[k];
        if (sides[n] == 0) ++nZeros;
    }
    //if (element == 39197) qDebug() << "getNNSides:: nZeros: " << nZeros;

    return nZeros;
}
// ------------------------------------------------------------------------------------------------
void ReadFileAbaqus::reOrderPentSides(short eType, short nSides, int elAdjst, int element, int *others, vector< vector<int> > &faces)
{
    bool fFlag;
    int i, j, k, f;
    int count;
    int kLim;
    int newEl;
    int nPoints;
    int temp[6];

    nPoints = 6;
    kLim    = 4;

    // Loop over element's faces.
    for(f=0; f<nSides; f++) temp[f] = 0;

    // Loop over the nearest neighbors.
    for(i=0; i<nSides; i++)
    {
        //if (eType == 1) qDebug() << "--i: " << i << "others[i]: " << others[i];

        if (others[i] == 0) break;
        newEl = others[i] - 1 - elAdjst;

        fFlag = false;
        // Loop over element's faces.
        for(f=0; f<nSides; f++)
        {
            if (f<2) kLim =3;
            else kLim=4;
            //qDebug() << "---___--- FAce: " << f;
            for (count=0, k=0; k<kLim; k++)
            {
                switch (eType)
                {
                // Check against all nodes from the nearest neighbor.

                    case 1:

                        //qDebug() << "f: " << f << "i: " << i << "k: " << k << " newEl: " << newEl << " g el: " << others[i];

                        for(j=0; j<nPoints; j++)
                        {
                            //qDebug() << "--el 1:: " << p1elements[element].connect[faces[f][k]] <<
                            //        " -- newel 2: " << p1elements[newEl].connect[j];
                            if (p1elements[element].connect[faces[f][k]] == p1elements[newEl].connect[j])
                            {
                                //qDebug() << "** found: " << p1elements[newEl].connect[j];
                                count++;
                                //break;
                            }
                        }
                        //qDebug() << "count: " << count;
                        break;
                    case 4:

                        //qDebug() << "f: " << f << "i: " << i << "k: " << k << " newEl: " << newEl << " g el: " << others[i];

                        for(j=0; j<nPoints; j++)
                        {
                            //qDebug() << "--el 1:: " << p1elements[element].connect[faces[f][k]] <<
                            //        " -- newel 2: " << p1elements[newEl].connect[j];
                            if (p2elements[element].connect[faces[f][k]] == p2elements[newEl].connect[j])
                            {
                                //qDebug() << "** found: " << p1elements[newEl].connect[j];
                                count++;
                                //break;
                            }
                        }
                        //qDebug() << "count: " << count;
                        break;
                }

                if (count == 3)
                {
                    if (eType == 4)
                    {
                        //qDebug() << "********* count == 3 ******";
                        //qDebug() << "Setting face: " << f << "others:" << others[i] << "\n";
                    }
                    temp[f] = others[i];
                    fFlag = true;
                    break;
                }
            }       // -- end k-loop
            if (fFlag) break;
        }
    }   // -- end i-loop over nearest neighbors.

    for (f=0; f<nSides; f++)
    {
        others[f] = temp[f];
    }
}
// ------------------------------------------------------------------------------------------------
void ReadFileAbaqus::reOrderHexSides(short eType, short nSides, int elAdjst, int element, int *others, vector< vector<int> > &faces)
{
    bool fFlag;
    int i, j, k, f;
    int count;
    int kLim;
    int newEl;
    int nPoints;
    int temp[6];

    //short nSides = 4;
    //int   elAdjst = 0;

    if ((eType == 2) || (eType == 5))
    {
        nPoints = 8;
        kLim    = 4;
    }
    else
    {
        nPoints = 4;
        kLim    = 3;
    }

    // Loop over element's faces.
    for(f=0; f<nSides; f++) temp[f] = 0;

    // Loop over the nearest neighbors.
    for(i=0; i<nSides; i++)
    {
        //if (eType == 1) qDebug() << "--i: " << i << "others[i]: " << others[i];

        if (others[i] == 0) break;
        newEl = others[i] - 1 - elAdjst;

        fFlag = false;
        // Loop over element's faces.
        for(f=0; f<nSides; f++)
        {
            //qDebug() << "---___--- FAce: " << f;
            for (count=0, k=0; k<kLim; k++)
            {
                switch (eType)
                {
                // Check against all nodes from the nearest neighbor.

                    case 0:

                        //qDebug() << "f: " << f << "i: " << i << "k: " << k << " newEl: " << newEl << " g el: " << others[i];

                        for(j=0; j<nPoints; j++)
                        {
                            //qDebug() << "--el 1:: " << p1elements[element].connect[faces[f][k]] <<
                            //        " -- newel 2: " << p1elements[newEl].connect[j];
                            if (t1elements[element].connect[faces[f][k]] == t1elements[newEl].connect[j])
                            {
                                //qDebug() << "** found: " << p1elements[newEl].connect[j];
                                count++;
                                //break;
                            }
                        }
                        //qDebug() << "count: " << count;
                        break;

                    case 2:

                        //qDebug() << "f: " << f << "i: " << i << "k: " << k << " newEl: " << newEl << " g el: " << others[i];

                        for(j=0; j<nPoints; j++)
                        {
                            //qDebug() << "--el 1:: " << p1elements[element].connect[faces[f][k]] <<
                            //        " -- newel 2: " << p1elements[newEl].connect[j];
                            if (h1elements[element].connect[faces[f][k]] == h1elements[newEl].connect[j])
                            {
                                //qDebug() << "** found: " << p1elements[newEl].connect[j];
                                count++;
                                //break;
                            }
                        }
                        //qDebug() << "count: " << count;
                        break;

                    case 3:

                        //qDebug() << "---___--- FAce: " << f;
                        //qDebug() << "f: " << f << "i: " << i << "k: " << k << " newEl: " << newEl << " g el: " << others[i];

                        for(j=0; j<nPoints; j++)
                        {
                            //qDebug() << "--el 1:: " << p1elements[element].connect[faces[f][k]] <<
                            //        " -- newel 2: " << p1elements[newEl].connect[j];
                            if (t2elements[element].connect[faces[f][k]] == t2elements[newEl].connect[j])
                            {
                                //qDebug() << "** found: " <<t21elements[newEl].connect[j];
                                count++;
                            }
                        }
                        break;

                    case 5:

                        //qDebug() << "f: " << f << "i: " << i << "k: " << k << " newEl: " << newEl << " g el: " << others[i];

                        for(j=0; j<nPoints; j++)
                        {
                            //qDebug() << "--el 1:: " << p1elements[element].connect[faces[f][k]] <<
                            //        " -- newel 2: " << p1elements[newEl].connect[j];
                            if (h2elements[element].connect[faces[f][k]] == h2elements[newEl].connect[j])
                            {
                                //qDebug() << "** found: " << p1elements[newEl].connect[j];
                                count++;
                                //break;
                            }
                        }
                        //qDebug() << "count: " << count;
                        break;
                }

                if (count == 3)
                {
                    if (eType == 3)
                    {
                        //qDebug() << "********* count == 3 ******";
                        //qDebug() << "Setting face: " << f << "others:" << others[i] << "\n";
                    }
                    temp[f] = others[i];
                    fFlag = true;
                    break;
                }
            }       // -- end k-loop
            if (fFlag) break;
        }
    }   // -- end i-loop over nearest neighbors.

    for (f=0; f<nSides; f++)
    {
        others[f] = temp[f];
    }
}
// ------------------------------------------------------------------------------------------------
void ReadFileAbaqus::makeInstanceTrees()
{
    int             i, j, k, n;
    int             eBegin, eEnd;
    vector<Point>   centroids;    // Centroids for the elements.
    //vector<Bbox>    eBoxes;       // Bounding boxes for the elements.
    Point           pt;
    Bbox            b;

    //qDebug() << "* makeInstanceTrees *";
    //qDebug() << "size of t1: " << t1elements.size();
    //qDebug() << "size of p1: " << p1elements.size();
    //qDebug() << "size of h1: " << h1elements.size();
    //qDebug() << "size of t2: " << t2elements.size();
    //qDebug() << "size of p2: " << p2elements.size();
    //qDebug() << "size of h2: " << h2elements.size();
    //exit(99);
    int num = this->instances.size();

    for(i=0; i < num; i++)
    {
        centroids.clear();
        // Pass references to the global lists of each element type.
        instances[i].tree = new SkdTree(&t1elements, &p1elements, &h1elements,
                                        &t2elements, &p2elements, &h2elements,
                                        &nodes,      instances[i].ctNumbers, instances[i].ctNumbers2,
                                        instances[i].elNumbers, elAdjust);
        instances[i].tree->eBoxes.clear();

        // Gather the elements
        // 1st order Tets
        eBegin = instances[i].elementLimits[0];
        eEnd   = instances[i].elementLimits[1];
        //qDebug() << "1st tets:: eBegin: " << eBegin << " eEnd: " << eEnd;
        if (eBegin < eEnd)
        {
            eBegin--;
            for (j = eBegin; j < eEnd; j++)
            {
                pt.coord[0] = t1elements[j].centroid[0];
                pt.coord[1] = t1elements[j].centroid[1];
                pt.coord[2] = t1elements[j].centroid[2];
                centroids.push_back(pt);
                for (n=0; n<6; n++)
                {
                    b.box[n] = t1elements[j].sBoxes[n];
                }
                instances[i].tree->eBoxes.push_back(b);
                instances[i].tree->eType.push_back(4);
                //fprintf(fp, "1st tet: %d\t %f\t %f\t %f\n",  j, pt.coord[0], pt.coord[2], pt.coord[2]);
                //fprintf(fp, "1st tet: %d\t %f\t %f\t %f\n",  j, t1elements[j].centroid[0], t1elements[j].centroid[1],
                //        t1elements[j].centroid[2]);
            }
        }

        // 1st order Pents
        eBegin = instances[i].elementLimits[2];
        eEnd   = instances[i].elementLimits[3];
        //qDebug() << "eBegin: " << eBegin << " eEnd: " << eEnd;
        if (eBegin < eEnd)
        {
            eBegin--;
            for (j = eBegin; j < eEnd; j++)
            {
                k = j - elAdjust[1];
                pt.coord[0] = p1elements[k].centroid[0];
                pt.coord[1] = p1elements[k].centroid[1];
                pt.coord[2] = p1elements[k].centroid[2];
                centroids.push_back(pt);
                for (n=0; n<6; n++)
                {
                    b.box[n] = p1elements[k].sBoxes[n];
                }
                instances[i].tree->eBoxes.push_back(b);
                instances[i].tree->eType.push_back(5);
                //fprintf(fp, "1st pent: %d\t %f\t %f\t %f\n",  j, pt.coord[0], pt.coord[2], pt.coord[2]);
                //fprintf(fp, "1st pent: %d\t %f\t %f\t %f\n",  j, p1elements[k].centroid[0], p1elements[k].centroid[1],
                //        p1elements[k].centroid[2]);
            }
        }
        // 1st order Hexs
        eBegin = instances[i].elementLimits[4];
        eEnd   = instances[i].elementLimits[5];
        //qDebug() << "e1st hexs:: Begin: " << eBegin << " eEnd: " << eEnd;
        if (eBegin < eEnd)
        {
            eBegin--;
            for (j = eBegin; j < eEnd; j++)
            {
                k = j - elAdjust[2];
                pt.coord[0] = h1elements[k].centroid[0];
                pt.coord[1] = h1elements[k].centroid[1];
                pt.coord[2] = h1elements[k].centroid[2];
                centroids.push_back(pt);
                for (n=0; n<6; n++)
                {
                    b.box[n] = h1elements[k].sBoxes[n];
                }
                instances[i].tree->eBoxes.push_back(b);
                instances[i].tree->eType.push_back(6);
                //fprintf(fp, "1st hex: %d\t %f\t %f\t %f\n",  j, pt.coord[0], pt.coord[2], pt.coord[2]);
                //fprintf(fp, "1st hex: %d\t %f\t %f\t %f\n",  j, h1elements[k].centroid[0], h1elements[k].centroid[1],
                //        h1elements[k].centroid[2]);
            }
        }

        // 2nd order Tets
        eBegin = instances[i].elementLimits[6];
        eEnd   = instances[i].elementLimits[7];
        //qDebug() << "eBegin: " << eBegin << " eEnd: " << eEnd;
        if (eBegin < eEnd)
        {
            eBegin--;
            for (j = eBegin; j < eEnd; j++)
            {
                k = j - elAdjust[3];
                pt.coord[0] = t2elements[k].centroid[0];
                pt.coord[1] = t2elements[k].centroid[1];
                pt.coord[2] = t2elements[k].centroid[2];
                centroids.push_back(pt);
                for (n=0; n<6; n++)
                {
                    b.box[n] = t2elements[k].sBoxes[n];
                }
                instances[i].tree->eBoxes.push_back(b);
                instances[i].tree->eType.push_back(14);
                //fprintf(fp, "2nd tet: %d\t %f\t %f\t %f\n",  j, pt.coord[0], pt.coord[2], pt.coord[2]);
                //fprintf(fp, "2nd tet: %d\t %f\t %f\t %f\n",  j, t2elements[k].centroid[0], t2elements[k].centroid[1],
                 //       t2elements[k].centroid[2]);
            }
        }

        // 2nd order Pents
        eBegin = instances[i].elementLimits[8];
        eEnd   = instances[i].elementLimits[9];
        //qDebug() << "eBegin: " << eBegin << " eEnd: " << eEnd;
        if (eBegin < eEnd)
        {
            eBegin--;
            for (j = eBegin; j < eEnd; j++)
            {
                k = j - elAdjust[4];
                pt.coord[0] = p2elements[k].centroid[0];
                pt.coord[1] = p2elements[k].centroid[1];
                pt.coord[2] = p2elements[k].centroid[2];
                centroids.push_back(pt);
                for (n=0; n<6; n++)
                {
                    b.box[n] = p2elements[k].sBoxes[n];
                }
                instances[i].tree->eBoxes.push_back(b);
                instances[i].tree->eType.push_back(15);
                //fprintf(fp, "2nd pent: %d\t %f\t %f\t %f\n",  j, pt.coord[0], pt.coord[2], pt.coord[2]);
                //fprintf(fp, "2nd pents: %d\t %f\t %f\t %f\n",  j, p2elements[k].centroid[0], p2elements[k].centroid[1],
                //        p2elements[k].centroid[2]);
            }
        }
        // 2nd order Hexs
        eBegin = instances[i].elementLimits[10];
        eEnd   = instances[i].elementLimits[11];
        //qDebug() << "eBegin: " << eBegin << " eEnd: " << eEnd;
        if (eBegin < eEnd)
        {
            eBegin--;
            for (j = eBegin; j < eEnd; j++)
            {
                k = j - elAdjust[5];
                pt.coord[0] = h2elements[k].centroid[0];
                pt.coord[1] = h2elements[k].centroid[1];
                pt.coord[2] = h2elements[k].centroid[2];
                centroids.push_back(pt);
                for (n=0; n<6; n++)
                {
                    b.box[n] = h2elements[k].sBoxes[n];
                }
                instances[i].tree->eBoxes.push_back(b);
                instances[i].tree->eType.push_back(16);
                //fprintf(fp, "2nd hex: %d\t %f\t %f\t %f\n",  j, pt.coord[0], pt.coord[2], pt.coord[2]);
                //fprintf(fp, "2nd hex: %d\t %f\t %f\t %f\n",  j, h2elements[k].centroid[0], h2elements[k].centroid[1],
                //        h2elements[k].centroid[2]);

            }
        }

        //qDebug() << "\n\n -----------------------------------------------------------\n";
        //qDebug() << " Instance: " << i << "\t size of centroids: " << centroids.size();

    /*    fprintf(fp, "\n -- instances[i].eBoxes: bounding planes --\n");
        for(j=0; j<instances[i].tree->eBoxes.size(); j++)
        {
            fprintf(fp, "%2d\t %f\t %f\t %f\t %f\t %f\t %f \n", j, instances[i].tree->eBoxes[j].box[0], instances[i].tree->eBoxes[j].box[1],
                    instances[i].tree->eBoxes[j].box[2],
                        instances[i].tree->eBoxes[j].box[3], instances[i].tree->eBoxes[j].box[4], instances[i].tree->eBoxes[j].box[5]);
        }*/


        instances[i].tree->createTree(centroids);
        //instances[i].tree->printDebugLinkp();
        //instances[i].tree->printDebugCutDir();
        //instances[i].tree->printDebugSbox();
    }
}
// ------------------------------------------------------------------------------------------------
void ReadFileAbaqus::createSummaryFile()
{
    size_t i;
    int    j;
    int nResults;
    QString sumFileName = currentPath + "/eeout_summary.txt";

    //qDebug() << "* createSummaryFile  *";

    if ((fp = fopen(sumFileName.toLatin1() , "w")) == NULL)
    {
        qDebug() << "-- error opening eeout summary text file --\n";
        qDebug() << "-- summary file: " << sumFileName;
    }

    fprintf(fp, "\t\t-- Abaqus file summary information --\n\n");
    //fprintf(fp, "%s\n", probID.toLatin1().data());
    //fprintf(fp, "%s\n", dateTime.toLatin1().data());
    //fprintf(fp, "%s\n", inpFile.toLatin1().data());
    //fprintf(fp, "%s\n", outpFile.toLatin1().data());
    //fprintf(fp, "%s\n", runtpeFile.toLatin1().data());
    //fprintf(fp, "%s\n", abaqusFile.toLatin1().data());

    //fprintf(fp, "\n");
    //fprintf(fp, "Number of particles: %d\n", nParticles);
    fprintf(fp, "Number of edits    : %d\n", (int) edits.size());
    fprintf(fp, "Number of nodes    : %d\n", nNodes);
    fprintf(fp, "Number of materials: %d\n", nMaterials);
    fprintf(fp, "Number of instances: %d\n", nInstances);
    fprintf(fp, "Number of elements : %d\n", nElements);
    fprintf(fp, "\n");
    fprintf(fp, "Number of 1st tets : %d\n", nEls[0]);
    fprintf(fp, "Number of 1st pents: %d\n", nEls[1]);
    fprintf(fp, "Number of 1st hexs : %d\n", nEls[2]);
    fprintf(fp, "Number of 2nd tets : %d\n", nEls[3]);
    fprintf(fp, "Number of 2nd pents: %d\n", nEls[4]);
    fprintf(fp, "Number of 2nd hexs : %d\n", nEls[5]);
    fprintf(fp, "\n");

    if (edits.size() > 0) fprintf(fp, "\t\t-- Edit Descriptions --\n");

    for (i=0; i<edits.size(); ++i)
    {
        fprintf(fp, "\nEdit #%d\t Type: %s\n", (i+1), resultNames[i].toLatin1().data());
        fprintf(fp, "Particle %d\n", edits[i].particle);
        fprintf(fp, "Energy bin: %d\t Value: %13.6e\t Multiplier: %13.6e\n", edits[i].ebin, edits[i].evalue, edits[i].emult);
        fprintf(fp, "  Time bin: %d\t Value: %13.6e\t Multiplier: %13.6e\n", edits[i].tbin, edits[i].tvalue, edits[i].tmult);
    }

    fprintf(fp, "\n\t\t-- Model Extents --\n\n");
    fprintf(fp, "Min X: %+2.5e \t Max X: %+2.5e \n", minExtents[0], maxExtents[0]);
    fprintf(fp, "Min Y: %+2.5e \t Max Y: %+2.5e \n", minExtents[1], maxExtents[1]);
    fprintf(fp, "Min Z: %+2.5e \t Max Z: %+2.5e \n", minExtents[2], maxExtents[2]);

    fprintf(fp, "\n\n\t\t-- Material Descriptions --\n\n");
    for (i=0; i<nMaterials; ++i)
    {
        fprintf(fp, "Material #%d\t %s\n", (i+1), materials[i].name.toLatin1().data());
    }

    fprintf(fp, "\n\n\t\t-- Instance Summary --");

    for (i=0; i<nInstances; ++i)
    {
        fprintf(fp, "\n\nInstance/Part #%d\t %s\n", (i+1), instances[i].name.toLatin1().data());
        fprintf(fp, "\n");

        if (instances[i].elNumbers[0] > 0)
            fprintf(fp, "\tNumber of 1st Order Tets : %d\n", instances[i].elNumbers[0]);
        if (instances[i].elNumbers[1] > 0)
            fprintf(fp, "\tNumber of 1st Order Pents: %d\n", instances[i].elNumbers[1]);
        if (instances[i].elNumbers[2] > 0)
            fprintf(fp, "\tNumber of 1st Order Hexs : %d\n", instances[i].elNumbers[2]);
        if (instances[i].elNumbers[3] > 0)
            fprintf(fp, "\tNumber of 2nd Order Tets : %d\n", instances[i].elNumbers[3]);
        if (instances[i].elNumbers[4] > 0)
            fprintf(fp, "\tNumber of 2nd Order Pents: %d\n", instances[i].elNumbers[4]);
        if (instances[i].elNumbers[5] > 0)
            fprintf(fp, "\tNumber of 2nd Order Hexs : %d\n", instances[i].elNumbers[5]);
        fprintf(fp, "\n");

        fprintf(fp, "\tMaterial: %13d\n", instances[i].materialNo);
        fprintf(fp, "\tDensity : %13.6e\n", (instances[i].mass / instances[i].volume));
        fprintf(fp, "\tMass    : %13.6e\n", instances[i].mass);
        fprintf(fp, "\tVolume  : %13.6e\n", instances[i].volume);
        fprintf(fp, "\tCentroid: %13.6e\t %13.6e\t %13.6e\n", instances[i].centroid[0], instances[i].centroid[1], instances[i].centroid[2]);
        fprintf(fp, "\n");

        fprintf(fp, "\tMin X   : %13.6e \tMax X   : %13.6e\n", instances[i].bounds.box[0], instances[i].bounds.box[3]);
        fprintf(fp, "\tMin Y   : %13.6e \tMax Y   : %13.6e\n", instances[i].bounds.box[1], instances[i].bounds.box[4]);
        fprintf(fp, "\tMin Z   : %13.6e \tMax Z   : %13.6e\n", instances[i].bounds.box[2], instances[i].bounds.box[5]);

        fprintf(fp, "\n");
        nResults = instances[i].results.size();
        for (j=0; j<nResults; ++j)
        {
            fprintf(fp, "\tEdit #%d %s\t%13.6e\n", (j+1), resultNames[j].toLatin1().data(), (instances[i].results[j] / instances[i].volume));
        }
    }
    fclose(fp);
}
// ----------------------------------------------------------------------------
void ReadFileAbaqus::calculateElementNormals()
{
    int     i, j, k;
    int     node;
    double  points[3][3];   // 1st index: x, y, z ; 2nd index: which point

    //qDebug() << "* calculateElementNormals *";

    for (i=0; i<this->nEls[0]; i++)
    {
        // Loop over the faces.
        for (j=0; j<4; ++j)
        {
            //qDebug() << "FACE: " << j;
            // Gather the nodes for the current face.
            for (k=0; k<3; ++k)
            {
                //qDebug() << "hexFaces: " << hexFaces[j][k];
                node = t1elements[i].connect[ tetFaces[j][k] ];
                points[0][k] = nodes[node].x;
                points[1][k] = nodes[node].y;
                points[2][k] = nodes[node].z;
            }
            calculateFaceNormal(points, t1elements[i].normal[j]);
            //qDebug() << "Normal Vector";
            //qDebug() << h1elements[i].normal[j][0];
            //qDebug() << h1elements[i].normal[j][1];
            //qDebug() << h1elements[i].normal[j][2];
        }
    }
    for (i=0; i<this->nEls[1]; i++)
    {
        // Loop over the faces.
        for (j=0; j<5; ++j)
        {
            //qDebug() << "FACE: " << j;
            // Gather the nodes for the current face.
            for (k=0; k<3; ++k)
            {
                //qDebug() << "hexFaces: " << hexFaces[j][k];
                node = p1elements[i].connect[ pentFaces[j][k] ];
                points[0][k] = nodes[node].x;
                points[1][k] = nodes[node].y;
                points[2][k] = nodes[node].z;
            }
            calculateFaceNormal(points, p1elements[i].normal[j]);
            //qDebug() << "Normal Vector";
            //qDebug() << h1elements[i].normal[j][0];
            //qDebug() << h1elements[i].normal[j][1];
            //qDebug() << h1elements[i].normal[j][2];
        }
    }
    for (i=0; i<this->nEls[2]; i++)
    {
        // Loop over the faces.
        for (j=0; j<6; ++j)
        {
            //qDebug() << "FACE: " << j;
            // Gather the nodes for the current face.
            for (k=0; k<3; ++k)
            {
                //qDebug() << "hexFaces: " << hexFaces[j][k];
                node = h1elements[i].connect[ hexFaces[j][k] ];
                points[0][k] = nodes[node].x;
                points[1][k] = nodes[node].y;
                points[2][k] = nodes[node].z;
            }
            calculateFaceNormal(points, h1elements[i].normal[j]);
            //qDebug() << "Normal Vector";
            //qDebug() << h1elements[i].normal[j][0];
            //qDebug() << h1elements[i].normal[j][1];
            //qDebug() << h1elements[i].normal[j][2];
        }
    }
    for (i=0; i<this->nEls[3]; i++)
    {
        // Loop over the faces.
        for (j=0; j<4; ++j)
        {
            //qDebug() << "FACE: " << j;
            // Gather the nodes for the current face.
            for (k=0; k<3; ++k)
            {
                //qDebug() << "hexFaces: " << hexFaces[j][k];
                node = t2elements[i].connect[ tetFaces[j][k] ];
                points[0][k] = nodes[node].x;
                points[1][k] = nodes[node].y;
                points[2][k] = nodes[node].z;
            }
            calculateFaceNormal(points, t2elements[i].normal[j]);
            //qDebug() << "Normal Vector";
            //qDebug() << h1elements[i].normal[j][0];
            //qDebug() << h1elements[i].normal[j][1];
            //qDebug() << h1elements[i].normal[j][2];
        }
    }
    for (i=0; i<this->nEls[4]; i++)
    {
        // Loop over the faces.
        for (j=0; j<5; ++j)
        {
            //qDebug() << "FACE: " << j;
            // Gather the nodes for the current face.
            for (k=0; k<3; ++k)
            {
                //qDebug() << "hexFaces: " << hexFaces[j][k];
                node = p2elements[i].connect[ pentFaces[j][k] ];
                points[0][k] = nodes[node].x;
                points[1][k] = nodes[node].y;
                points[2][k] = nodes[node].z;
            }
            calculateFaceNormal(points, p2elements[i].normal[j]);
            //qDebug() << "Normal Vector";
            //qDebug() << h1elements[i].normal[j][0];
            //qDebug() << h1elements[i].normal[j][1];
            //qDebug() << h1elements[i].normal[j][2];
        }
    }
    for (i=0; i<this->nEls[5]; i++)
    {
        // Loop over the faces.
        for (j=0; j<6; ++j)
        {
            //qDebug() << "FACE: " << j;
            // Gather the nodes for the current face.
            for (k=0; k<3; ++k)
            {
                //qDebug() << "hexFaces: " << hexFaces[j][k];
                node = h2elements[i].connect[ hexFaces[j][k] ];
                points[0][k] = nodes[node].x;
                points[1][k] = nodes[node].y;
                points[2][k] = nodes[node].z;
            }
            calculateFaceNormal(points, h2elements[i].normal[j]);
            //qDebug() << "Normal Vector";
            //qDebug() << h1elements[i].normal[j][0];
            //qDebug() << h1elements[i].normal[j][1];
            //qDebug() << h1elements[i].normal[j][2];
        }
    }
}
