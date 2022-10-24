#ifndef BaseVtkWindow_H
#define BaseVtkWindow_H


#include <vtkProperty.h>
#include <vtkCamera.h>

#include "vtkSmartPointer.h" // Required for smart pointer internal ivars.
#include "vtkGenericOpenGLRenderWindow.h"
#include "vtkSmartPointer.h"
#include <vtkElevationFilter.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkQtTableView.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkVectorText.h>
#include <QVTKRenderWidget.h>
#include <vtkNamedColors.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkCameraOrientationWidget.h>
#include <vtkTextProperty.h>
#include <vtkCaptionActor2D.h>


#include <vtkActor.h>
#include <vtkAxesActor.h>
#include <vtkImplicitPlaneRepresentation.h>
#include <vtkImplicitPlaneWidget2.h>
#include <vtkClipPolyData.h>
#include <vtkPlane.h>
#include <vtkAssembly.h>
#include <vtkDoubleArray.h>
#include <vtkPlanes.h>

#include <vtkSphere.h>
#include <vtkSphereSource.h>

#include <vtkCellArrayIterator.h>
#include <vtkDecimatePro.h>
#include <vtkTriangleFilter.h>
#include <vtkRendererCollection.h>
#include <vtkUnstructuredGrid.h>

#include <limits>
#include <QObject>
#include <QtWidgets>
#include "cgbodybasics.h"
#include "cgzoneops.h"
#include "cg_constants.h"
#include "vtkShader.h"
#include "vtkShaderProgram.h"
#include "vtkShaderProperty.h"
#include <vtkOpenGLProperty.h>
#include <vtkOpenGLPolyDataMapper.h>
#include "sssBooleanOperationCheck.h"
#include <vtkExecutive.h>
#include <vtkBooleanOperationPolyDataFilter.h>
#include <vtkAppendPolyData.h>
#include <vtkCleanPolyData.h>
#include <vtkPolyDataNormals.h>
#include "sssEllipsoidSource.h"
#include "sssEllipticalCylinderSource.h"
#include "sssCubeSource.h"
#include "sssTorusSource.h"
#include "masterdata.h"
#include "bounds.h"
#include <vtkDataSetMapper.h>

typedef vtkSmartPointer<vtkPolyDataAlgorithm> BodyPointers;
typedef vtkSmartPointer<vtkActor> BodyActors;
typedef vtkSmartPointer<vtkOpenGLPolyDataMapper> ZoneMappers;
typedef vtkSmartPointer<vtkActor> UMeshActors;
typedef vtkSmartPointer<vtkUnstructuredGrid> UGrids;
typedef vtkSmartPointer<vtkDataSetMapper> GridMappers;

class BaseVtkWindow
{
	public:
		// Constructor/Destructor
        ~BaseVtkWindow();
        BaseVtkWindow(MasterData *mdIn);

        void setNoBooleanOp(bool state);

        void makeVtkWidget(QSplitter *splitter);

        void showOneZone(QVector<CGBodyBasics> *iNcgBodyBasics,
                         QVector<CGZoneOps>    *iNcgZoneOps,
                         std::vector<double> *redF,
                         std::vector<double> *greF,
                         std::vector<double> *bluF, Bounds *inBounds);

        void makeDefaultView();

        void updateClippingSurfaces(const int axes, int dir, double plane);
        void updateClippingSurfaces2(const int axes, int dir, double plane);

        void updateLight(double *values, int style);

        void updateEntireModel(Bounds *inBounds, QVector<ReadFileAbaqus> *abaqus);

        void showMeshAssembly(ReadFileAbaqus *abaqus, Bounds *inBounds, int universe);

        void resetForProject();
        void toggleOrientationWidget();
        void toggleCamOrientationWidget();

    signals:
        void baseVtkTriangleReduction(int, int);


    private:

        void makeUMeshModel(ReadFileAbaqus *abaqus);
        void makeUMeshMappers();

        void clearAllActors();
        void makeAxisTriad();
        void makeCameraOrientationWidget();

        void makeZoneMappersBoolean(QVector<CGZoneOps>  *cgZoneOpsLocal);
        void makeZoneMappersBoolean(QVector<CGZoneOps>  *cgZoneOpsLocal,
                                    QColor &color,  const int cell, const bool display);

        void makeZoneMappersNoBoolean(QVector<CGZoneOps>  *cgZoneOpsLoca);
        void makeZoneActors();
        void makeZoneActors2();
        void makeZoneActorsWithGrid();

        void checkCellBooleanOperations2(QVector<CGZoneOps>  *cgZoneOpsLocal);
        void checkCellBooleanOperations(QVector<CGZoneOps>  *cgZoneOpsLocal);

        void makeBody(CGBodyBasics &body);

        void resetAllVectors();

        void userWarning(const int code, const int bodyNum);

        void setupClipPlanes();
        void setupClipPlanes(vtkSmartPointer<vtkPlanes> clipPlanes);
        void addCGZoneActors(vtkSmartPointer<vtkPlanes> clipPlanes);

        void addUGridZoneActors(vtkSmartPointer<vtkPlanes> clipPlanes);
        void updateAllUGrids(QVector<ReadFileAbaqus> *abaqus);

        void finalizeRenderWindow();

        void updateBounds(double *bounds);

        std::vector < vtkSmartPointer < sssBooleanOperationCheck >> checkBools;
        std::vector < vtkSmartPointer < vtkPolyDataAlgorithm >> srcs;
        std::vector < vtkSmartPointer < vtkBooleanOperationPolyDataFilter >> bools;

        MasterData *md;
        Bounds *mBounds;

    QVector<CGBodyBasics> *cgBodyBasics;
    QVector<CGZoneOps>    *cgZoneOps;
    std::vector<double>        colorR;
    std::vector<double>        colorG;
    std::vector<double>        colorB;
    std::vector<double>        ugridColorR;
    std::vector<double>        ugridColorG;
    std::vector<double>        ugridColorB;
    std::vector<bool>          zoneDisplay;
    std::vector<bool>          ugridDisplay;

    std::vector< std::vector<int>> zoneSpecs;
    std::vector< std::vector<int>> codeZoneIn;
    std::vector< std::vector<int>> codeZoneOr;
    std::vector<int> numCodeZones;

    int viewMode;
    int backFaceStyle;
    double whiteWashFactor;
    double transparancy;

    bool noBooleanOps;
    // Normal gives direction that will be kept when clipped.
    double xnorm[3] = {1., 0., 0.};
    double ynorm[3] = {0., 1., 0.};
    double znorm[3] = {0., 0., 1.};
    // Specify large negative position in repective direction
    // to avoid initial clipping.
    double xpt[3]   = {-10000., 0., 0.};
    double ypt[3]   = {0., -10000., 0.};
    double zpt[3]   = {0., 0., -10000.};

	QVTKRenderWidget		*qvtkWidget;

    // VTK Renderer
    vtkNew<vtkRenderer> ren;

    // VTK OpenGL Render Window
    vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;

    vtkRenderWindowInteractor *iRen;

    vtkNew<vtkOrientationMarkerWidget> orientationWidget;

    vtkNew<vtkAxesActor> axes;

    vtkNew<vtkCameraOrientationWidget> camOrientManipulator;

    vtkNew<vtkTextProperty> textProperty1;
    vtkNew<vtkProperty> zoneProperty1;

    vtkNew<vtkNamedColors> colors;

    std::vector<ZoneMappers> zoneMappers;
    std::vector<GridMappers> ugridMappers;
    std::vector<UGrids> ugrids;

    bool goOrientationMarkerWidget;
    bool goCamOrientationWidget;

};



#endif // BaseVtkWindow_H
