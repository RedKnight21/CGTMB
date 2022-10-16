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
        vector< double >    results;
        vector< double >    maxResults;
        vector< double >    minResults;
        vector< int >       elMin;
        vector< int >       elMax;
        vector< int >       surfaceTriangles;
        vector< UM_node >   surfaceNodes;
        vector< UM_node >   surfaceNormals;
        vector< UM_node >   nodes;
        vector<UM_surface>  surface;
        vector<LineSegment> outline;
        vector<UM_h1element> h1elements;
        vector<UM_p1element> p1elements;
        vector<UM_t1element> t1elements;

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
    vector<UM_nodes>     nodes;
    vector<UM_material>  materials;
    vector<UM_instance>  instances;
    vector<UM_t1element> t1elements;
    vector<UM_p1element> p1elements;
    vector<UM_h1element> h1elements;
    vector<UM_t2element> t2elements;
    vector<UM_p2element> p2elements;
    vector<UM_h2element> h2elements;
    vector<UM_histogramData> umoHistogram;
    vector<EmbeeInfo>    edits;
    vector<double>       resultMin;
    vector<double>       resultMax;
    vector<double>       elMaxResults;
    vector<double>       elMinResults;
    vector<QString>      resultNames;
    vector<int>          matMap;
    vector< int >        elMinObj;
    vector< int >        elMaxObj;
    vector<GLfloat>      allNodesPosition;
    vector<GLfloat>      allNodesResult;
    vector<GLfloat>      allNodesResultColor;

    vector<GLfloat>     t1elNodesPosition;
    vector<GLfloat>     t1elNodesColor;
    vector<GLfloat>     t1elNodesNormal;
    vector<GLfloat>     p1elNodesPosition3;
    vector<GLfloat>     p1elNodesPosition4;
    vector<GLfloat>     p1elNodesColor3;
    vector<GLfloat>     p1elNodesNormal3;
    vector<GLfloat>     p1elNodesColor4;
    vector<GLfloat>     p1elNodesNormal4;
    vector<GLfloat>     h1elNodesPosition;
    vector<GLfloat>     h1elNodesColor;
    vector<GLfloat>     h1elNodesNormal;

    vector<GLfloat>     t2elNodesPosition;
    vector<GLfloat>     t2elNodesColor;
    vector<GLfloat>     t2elNodesNormal;
    vector<GLfloat>     p2elNodesPosition3;
    vector<GLfloat>     p2elNodesPosition4;
    vector<GLfloat>     p2elNodesColor3;
    vector<GLfloat>     p2elNodesNormal3;
    vector<GLfloat>     p2elNodesColor4;
    vector<GLfloat>     p2elNodesNormal4;
    vector<GLfloat>     h2elNodesPosition;
    vector<GLfloat>     h2elNodesColor;
    vector<GLfloat>     h2elNodesNormal;

    vector<GLuint>      t1elNodesIndices;
    vector<GLuint>      t2elNodesIndices;
    vector<GLuint>      h1elNodesIndices;
    vector<GLuint>      h2elNodesIndices;
    vector<GLuint>      p1elNodesIndices3;
    vector<GLuint>      p2elNodesIndices3;
    vector<GLuint>      p1elNodesIndices4;
    vector<GLuint>      p2elNodesIndices4;

    vector<GLfloat>     surfaceNodePosition3, surfaceNodePosition4;
    vector<GLfloat>     surfaceNodeNormal3,   surfaceNodeNormal4;
    vector<GLfloat>     surfaceNodeColor3,    surfaceNodeColor4;

    vector<GLuint>      surfaceIndices3,   surfaceIndices4;
    vector<GLuint>      surfaceInstances3, surfaceInstances4;   // boundary indices for wireframe

    vector<QColor>      resultColors;
    vector<double>      resultBins;

    vector< vector<int> > tetFaces;
    vector< vector<int> > pentFaces;
    vector< vector<int> > hexFaces;

    vector<AnnotatePoint>   annotatePoints;
    vector<AnnotateLine>    annotateLines;
    vector<AnnotateString>  annotateStrings;

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
    void    findLineOutvalues(int plotType, vector<IsoPoint> &lineOutData);
    void    processSectorPointValues(Point &pt, vector<float> &results, int &elNo, bool &isValid, bool pFlag);
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
    void    findAllPointValues(IsoPoint &thisData, vector<float> &results);
    //~UMObject();

    private:
        int    xResValueIso;
        int    yResValueIso;
        int    zResValueIso;
        int    boxesPerPlane;
        int    pointsPerPlane;

        float   deltaX, deltaY, deltaZ;     // deltas for constructing the iso grid.

        vector<float> isoXmesh;
        vector<float> isoYmesh;
        vector<float> isoZmesh;
        vector<IsoPoint> isoGrid;

//        FILE *fp, *fp2, *fp3, *fp4;
};

#endif // UMOBJECT_H
