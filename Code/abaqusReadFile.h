#ifndef ABAQUS_READFILE_H
#define ABAQUS_READFILE_H

#include <QHash>
#include "umObject.h"
#include "rgbColors.h"
#include "skdTree.h"
#include "gridcell.h"
#include "cgutility.h"


class ReadFileAbaqus : public UMObject
{
public:
    int     readAbaqusFile(QString fileName, QString currentPath, QString fileError);
    ReadFileAbaqus();

    ~ReadFileAbaqus();

    QString abaqusFile;

    private:

        char    getPartNodes(QTextStream &in, int &countNode2, QString &line, std::vector<P_nodes> &pNode, QMap<int, int> &nodeMap);
        void    endPartProcessing(ABaqus_part &tPart);
        void    getPartConnectivity(QTextStream &in, int &countNode2, QString &line, ABaqus_part &tPart);
        void    getInstanceInfo(QTextStream &in, QString lineIn);
        void    getMaterialNames(QString line);
        void    finishMaterials();
        void    instanceColors();
        void    calculatePartVolumes();
        void    calculateElementVolumesByPart();
        void    buildInstances();
        void    buildInstancesNew();

        void    buildCTNumbers();
        void    calculateCentroids();
        void    calculateElementNormals();
        void    fprintElements(QString ss);

        void    readFlatAbaqusFile(QTextStream &in);
        void    readNormalAbaqusFile(QTextStream &in);
        void    getInstanceNames(QTextStream &in);
        void    getMaterialNumbers(QTextStream &in);
        void    transferNodes(ABaqus_part &abqPart, ABaqus_part &newPart);
        void    processPartsInstances(QMap<QString, QString> &matElset);
        int     findElsetNumber(const QString stringIn, const QString name, size_t &pos);
        void    finalizeInstances();
        void    reOrderPentSides(short eType, short nSides, int elAdjst, int element, int *sides, std::vector< std::vector<int> > &tetFaces);
        void    reOrderHexSides(short eType, short nSides, int elAdjst, int element, int *sides, std::vector< std::vector<int> > &tetFaces);
        void    makeInstanceTrees();
        void    transferNodes2(ABaqus_part &abqPart, ABaqus_part &newPart);

        void    createSummaryFile();
        short   getNNSides(int nSides, int element,  std::vector<int>  &NND, int *sides);
        void    getFaceNodes(short eType, int tEl, int face, int *nextNodes);
        char    findNodeMatches(unsigned char neighborFace, int thisFace, int *nextNodes, int *thisNodes, int *nodes);


        void data2File(QString name, std::vector<UM_node> &nodes,  std::vector<int> &vertices);


        int     lastEmbeeParticle;
        QString lastEmbeeName;
        QString probID;
        QString dateTime;
        QString inpFile;
        QString outpFile;
        QString runtpeFile;
        QString currentPath;

        char hexOutlineNodes[5][6][2];
        char tetOutlineNodes[4][4][2];
        char pentOutlineNodes[5][5][2];

        QMap<int, int>      pNodeMap;
        QMap<int, int>      pConnectMap;
        QMap<QString, QString> matElsetMap;
        std::vector<UM_nodes>    nodes2;

        std::vector<UM_t1element> t1elements2;
        std::vector<UM_p1element> p1elements2;
        std::vector<UM_h1element> h1elements2;
        std::vector<UM_t2element> t2elements2;
        std::vector<UM_p2element> p2elements2;
        std::vector<UM_h2element> h2elements2;

        std::vector<ABaqus_part> parts;

        std::vector<int>         matNos;
        std::vector<double>      xCentroids;
        std::vector<double>      yCentroids;
        std::vector<double>      zCentroids;
        std::vector<float>       volumes;
        std::vector<float>       densities;
        std::vector<QString>     partNames;
        FILE *fp, *fp2;

        enum    {NONE,     ATTILAGMV,  MCNPEEOUT, ABAQUS  } fileType;
};

#endif // ABAQUS_READFILE_H

