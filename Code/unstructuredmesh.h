#ifndef UNSTRUCTUREDMESH_H
#define UNSTRUCTUREDMESH_H

#include <vector>
#include <map>
#include <QtCore>
#include <QString>
#include <QColor>
#include <umDataStructures.h>
//#include "utilities.h"

using namespace std;


class LineSegment
{
    public:
        int first;
        int last;
};

class UM_nodes
{
    public:
        vector<double>  results;
        double x;
        double y;
        double z;
        int    count;
        bool   displayed;
};

class UM_node
{
    public:
        double x;
        double y;
        double z;
};

class P_nodes
{
    public:
        double x;
        double y;
        double z;
};

class P_t1element
{
    public:
        float   volume;
//        float   normal[4][3];
        int	connect[4];
        /// Array defining the faces by their nodes.
        int     faces[4][3];
};

class P_p1element
{
    public:
        float   volume;
//        float   normal[5][3];
        int	connect[6];
        /// Array defining the faces by their nodes.
        int     faces[5][4];
};

class P_h1element
{
    public:
        float   volume;
//        float   normal[6][3];
        int	connect[8];
        /// Array defining the faces by their nodes.
        int     faces[6][4];
};

class P_t2element
{
    public:
        float   volume;
        float   normal[4][3];
        int	connect[10];
};

class P_p2element
{
    public:
        float   volume;
        float   normal[5][3];
        int	connect[15];
};

class P_h2element
{
    public:
        float   volume;
        float   normal[6][3];
        int	connect[20];
};

class UM_surface
{
    public:
        int             element;
        int             neighbor;
        unsigned char   face;           // face number range from 0 to 6
        unsigned char   type;
};

class UM_material
{
    public:
        bool    display;
        int     number;
        QString name;
        QColor  color;
};

class ABaqus_part
{
    // Class to support temporary storage of part data from the Abaqus input
    // file before global model construction.
    public:
        double volume;
        int elNumbers[6];           // # of elements for each element type
        //int nEls[6];
        int matNo;
        vector<P_nodes> nodes;
        vector<P_t1element> t1;
        vector<P_p1element> p1;
        vector<P_h1element> h1;
        vector<P_t2element> t2;
        vector<P_p2element> p2;
        vector<P_h2element> h2;

        ///  Vector containing pairs of element number and face numbers that are on the parts surface.
        vector< vector<int> > surfaceEls;
        vector<int> surfaceTriangles;
        vector<P_nodes> surfaceNodes;

        QString name;
        void clearIt();
        void findSurfaceElements();
        void findSurfaceTriangles();

        // Data structure mapping the input file node number to the internal program number for the node.
        // Needed to re-order the node numbers.
        QMap<int, int> 		pNodeMap;

        // Smallest element number in flat model elset.
        int     minElNo;
        // Largest element number in flat model elset.
        int     maxElNo;

        /// Integer array holding the node index scheme belonging to each tet face.
        //  tetFaces[4][3]
        static const int tetFaces[][3];

        /// Integer array holding the node index scheme belonging to each tet face.
        //  pentFaces[5][4]
        static const int pentFaces[][4];

        /// Integer array holding the node index scheme belonging to each hex face.
        //  hexFaces[6][4]
        static const int  hexFaces[][4];
};

class UM_element
{
    public:
        int	eType;
        int	element;
        int	material;
        int     instance;
        bool    displayElement;
        float   density;
        float   volume;
        double  centroid[3];
        double  sBoxes[6];
        vector< double >  results;
};

class UM_t1element : public UM_element
{
    public:
        int	connect[4];
        float   normal[4][3];
};

class UM_p1element : public UM_element
{
    public:
        int	connect[6];
        float   normal[5][3];
};

class UM_h1element : public UM_element
{
    public:
        int	connect[8];
        float   normal[6][3];
};

// 2nd order classes only connect the vertices.
// Mid-points can be added later if needed.
class UM_t2element : public UM_element
{
    public:
        int	connect[4];
        float   normal[4][3];
};

class UM_p2element : public UM_element
{
    public:
        int	connect[6];
        float   normal[5][3];
};

class UM_h2element : public UM_element
{
    public:
        int	connect[8];
        float   normal[6][3];
};

class EmbeeInfo
{
    public:
        int     particle;
        QString type;
        int     tbin;
        int     ebin;
        double  tmult;
        double  emult;
        double  tvalue;
        double  evalue;
        vector< double > results;
};

class UM_histogramData
{
    public:
        QVector<double> hBins;
        QVector<double> limits;
//        int maxEl;
//        int minEl;
};

class IsoObject
{
    public:
        unsigned char   posPts;
        bool            display;
        bool            reverseNormal;
        float           value;
        vector<int>     instances;
        QColor          color;
        vector<Point3>  triangles;
};

class IsoPoint
{
    public:
        unsigned char eType;
        int     elNo;             // element number local to instance
        int     gEl;              // global element number
        float   g;
        float   h;
        float   r;
        float   x, y, z;
        float   value;
        bool    valid;
};

class SectorPoint
{
    public:
        Point   xyz;
        int     element;
        vector<float> results;
        bool    isValid;
};

class SectorRow
{
    public:
        float length;
        int   nPoints;
        vector<SectorPoint> rowVals;
};

class SectorPlane
{
    public:
        int nRows;              // vertical points or number of rows
        vector<int> rowLimits;
        vector<SectorRow> rows;
};

class SectorPart
{
    public:
        unsigned char   nIPoints;
        int             azimuthal;
        int             polar;
        int             instNo;
        vector<Point>   corners;
        SectorPlane     sPlane;
};


class SectorObject
{
    // One object for each sector plane.
    public:
        unsigned char       persistence;
        unsigned char       display;
        unsigned char       nIPoints;
        unsigned char       showPoints;
        QString             name;
        Point               anchor1;
        Point               anchor2;
        Point               dirCosines;
        int                 azimuthal;
        int                 polar;
        int                 nPerCm;
        vector<SectorPart>  part;
        vector<Point>       corners;
        SectorPlane         sPlane;
};


#endif // UNSTRUCTUREDMESH_H
