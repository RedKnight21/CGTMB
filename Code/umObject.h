#ifndef UMOBJECT_H
#define UMOBJECT_H

#include "unstructuredmesh.h"
#include "skdTree.h"
#include "gridcell.h"
#include <QOpenGLFunctions>
#include <QtCore>
#include <QApplication>
#include <cfloat>

class UM_instance
{
    public:
        int                 elementLimits[12];
        int                 elNumbers[6];           // # of elements for each element type
        int                 ctNumbers[6];           // cummulative # of elements for each element type
        int                 ctNumbers2[6];
        int                 adder[6];               // to go from inst el # to global type el #
        QString             name;
        QString             partName;
        int                 lowerLimit;
        int                 upperLimit;
        int                 partNo;
        int                 materialNo;
        QColor              color;
        bool                display;
        float               mass;
        float               volume;
        double              centroid[3];
        double              expand[3];
        double              extraExpand[3];
        double              translate[3];
        double              rotate[7];
        double              extents[6];
        std::vector< double >    results;
        std::vector< double >    maxResults;
        std::vector< double >    minResults;
        std::vector< int >       elMin;
        std::vector< int >       elMax;
        std::vector< int >       surfaceTriangles;
        std::vector< UM_node >   surfaceNodes;
        std::vector< UM_node >   surfaceNormals;
        std::vector< UM_node >   nodes;
        std::vector<UM_surface>  surface;
        std::vector<LineSegment> outline;
        std::vector<UM_h1element> h1elements;
        std::vector<UM_p1element> p1elements;
        std::vector<UM_t1element> t1elements;

        Bbox                bounds;     // Bounding box for the instance.
        SkdTree             *tree;      // skdtree of elements in this instance.

        UM_instance()
        {
            expand[0]      = 0.0; expand[1]      = 0.0; expand[2]      = 0.0;
            extraExpand[0] = 0.0; extraExpand[1] = 0.0; extraExpand[2] = 0.0;
            translate[0]   = 0.0; translate[1]   = 0.0; translate[2]   = 0.0;
            rotate[0]      = 0.0; rotate[1]      = 0.0; rotate[2]      = 0.0;
            extents[0]     = 0.0; extents[1]     = 0.0; extents[2]     = 0.0;
            centroid[0]    = 0.0; centroid[1]    = 0.0; centroid[2]    = 0.0;

            display    = true;
            color.setRed(255);
            color.setGreen(255);
            color.setBlue(255);
            color.setAlpha(255);
            lowerLimit  = 0;
            upperLimit  = 0;

            for (int i=0; i<12; i++) elementLimits[i] = 0;
        }
};


class UMObject
{
    public:
    std::vector<UM_nodes>     nodes;
    std::vector<UM_material>  materials;
    std::vector<UM_instance>  instances;
    std::vector<UM_t1element> t1elements;
    std::vector<UM_p1element> p1elements;
    std::vector<UM_h1element> h1elements;
    std::vector<UM_t2element> t2elements;
    std::vector<UM_p2element> p2elements;
    std::vector<UM_h2element> h2elements;
    std::vector<UM_histogramData> umoHistogram;
    std::vector<EmbeeInfo>    edits;
    std::vector<double>       resultMin;
    std::vector<double>       resultMax;
    std::vector<double>       elMaxResults;
    std::vector<double>       elMinResults;
    std::vector<QString>      resultNames;
    std::vector<int>          matMap;
    std::vector< int >        elMinObj;
    std::vector< int >        elMaxObj;
    std::vector<GLfloat>      allNodesPosition;
    std::vector<GLfloat>      allNodesResult;
    std::vector<GLfloat>      allNodesResultColor;

    std::vector<GLfloat>     t1elNodesPosition;
    std::vector<GLfloat>     t1elNodesColor;
    std::vector<GLfloat>     t1elNodesNormal;
    std::vector<GLfloat>     p1elNodesPosition3;
    std::vector<GLfloat>     p1elNodesPosition4;
    std::vector<GLfloat>     p1elNodesColor3;
    std::vector<GLfloat>     p1elNodesNormal3;
    std::vector<GLfloat>     p1elNodesColor4;
    std::vector<GLfloat>     p1elNodesNormal4;
    std::vector<GLfloat>     h1elNodesPosition;
    std::vector<GLfloat>     h1elNodesColor;
    std::vector<GLfloat>     h1elNodesNormal;

    std::vector<GLfloat>     t2elNodesPosition;
    std::vector<GLfloat>     t2elNodesColor;
    std::vector<GLfloat>     t2elNodesNormal;
    std::vector<GLfloat>     p2elNodesPosition3;
    std::vector<GLfloat>     p2elNodesPosition4;
    std::vector<GLfloat>     p2elNodesColor3;
    std::vector<GLfloat>     p2elNodesNormal3;
    std::vector<GLfloat>     p2elNodesColor4;
    std::vector<GLfloat>     p2elNodesNormal4;
    std::vector<GLfloat>     h2elNodesPosition;
    std::vector<GLfloat>     h2elNodesColor;
    std::vector<GLfloat>     h2elNodesNormal;

    std::vector<GLuint>      t1elNodesIndices;
    std::vector<GLuint>      t2elNodesIndices;
    std::vector<GLuint>      h1elNodesIndices;
    std::vector<GLuint>      h2elNodesIndices;
    std::vector<GLuint>      p1elNodesIndices3;
    std::vector<GLuint>      p2elNodesIndices3;
    std::vector<GLuint>      p1elNodesIndices4;
    std::vector<GLuint>      p2elNodesIndices4;

    std::vector<GLfloat>     surfaceNodePosition3, surfaceNodePosition4;
    std::vector<GLfloat>     surfaceNodeNormal3,   surfaceNodeNormal4;
    std::vector<GLfloat>     surfaceNodeColor3,    surfaceNodeColor4;

    std::vector<GLuint>      surfaceIndices3,   surfaceIndices4;
    std::vector<GLuint>      surfaceInstances3, surfaceInstances4;   // boundary indices for wireframe

    std::vector<QColor>      resultColors;
    std::vector<double>      resultBins;

    std::vector< std::vector<int> > tetFaces;
    std::vector< std::vector<int> > pentFaces;
    std::vector< std::vector<int> > hexFaces;

    std::vector<AnnotatePoint>   annotatePoints;
    std::vector<AnnotateLine>    annotateLines;
    std::vector<AnnotateString>  annotateStrings;

    double  maxElCentroid[3];  // maximum element location from histogram window
    double  minElCentroid[3];
    int     maxElement;
    int     minElement;

    int elAdjust[6];

    int nMaterials;
    int nNodes;
    int nInstances;
    int nElements;
    int nParticles;
    int nResults;
    int status;
    int version;

    int   nEls[6];
    short instanceCodesLeft[6];
    short instanceCodesRight[6];
    short colorChoice;

    bool  annotatePersistence;

    double minExtents[3];
    double maxExtents[3];

    double  nodesMinX;
    double  nodesMaxX;
    double  nodesMinY;
    double  nodesMaxY;
    double  nodesMinZ;
    double  nodesMaxZ;
    double  nodesCentroid[3];

    float   extentsX;
    float   extentsY;
    float   extentsZ;

    // ----------
    // Functions.
    //int     readEeoutFile(QString fileName, QString currentPath, QString fileError);
    void    createNewEdit(int op, int edit1, int edit2, double mult1, double mult2, QString name);
    void    createIsoGrid(int nx, int ny, int nz);
    void    createIsoSurface(int whichPlot, float isoValue, bool doGrid, IsoObject &iso);
    double  eval1stOrderHexResults(int whichPlot, IsoPoint &isoGrid);
    double  eval1stOrderTetResults(int whichPlot, IsoPoint &isoGrid);
    double  eval1stOrderPentResults(int whichPlot, IsoPoint &isoGrid);
    int     setupIsoGridPoint(float cox, float coy, float coz, IsoPoint &iso);
    void    setupIsoGridPoint(IsoPoint &iso, bool iflag, bool pFlag);
    void    findLineOutvalues(int plotType, std::vector<IsoPoint> &lineOutData);
    void    processSectorPointValues(Point &pt, std::vector<float> &results, int &elNo, bool &isValid, bool pFlag);
    void    findSectorPointValue(int pType, IsoPoint &isoData);
    void    findPointGHRetc(Point &pt, Point &ghr, int &elNo, int &gEl, unsigned char &eType, bool pFlag);
    int     findElementData(int elNo, int &inst, int &matNo, short &eType);
    int     findMaterialNumber(int elNo, short eType);
    void    generateHistogramBins(int nBins, int whichEdit, int intervalType);
    bool    retreiveCentroids(int elNo, short eType, double *cent);
    int     findElementType(int elNo, int &inst, short &eType);
    void    makeAllNodesPosition();
    void    makeAllNodesResult(int which);
    void    makeAllNodesResultColor(int which, int intervalType);
    void    setupVolumeGeometryNodesIndices(int colorChoice);
    void    setupVolumeGeometryNodesColors(int intervalType, int plotType, int colorChoice, int contourType);
    void    setupVolumeGeometryNodes();
    bool    isElementDisplayed(int elNo, int colorChoice);

    void    setupSurfaceNodePositions();
    void    setupSurfaceNodeNormals();
    void    setupSurfaceNodeColors(int intervalType, int whichPlot, int colorChoice, int contourType);
    void    setupSurfaceIndices(int colorChoice);
    void    clearVectors();
    void    findAllPointValues(IsoPoint &thisData, std::vector<float> &results);
    //~UMObject();

    private:
        int    xResValueIso;
        int    yResValueIso;
        int    zResValueIso;
        int    boxesPerPlane;
        int    pointsPerPlane;

        float   deltaX, deltaY, deltaZ;     // deltas for constructing the iso grid.

        std::vector<float> isoXmesh;
        std::vector<float> isoYmesh;
        std::vector<float> isoZmesh;
        std::vector<IsoPoint> isoGrid;

//        FILE *fp, *fp2, *fp3, *fp4;
};

#endif // UMOBJECT_H
