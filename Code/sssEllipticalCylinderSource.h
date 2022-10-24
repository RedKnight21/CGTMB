/*=========================================================================

  Program:   Visualization Toolkit
  Module:    sssSuperCylinderSource.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   sssSuperCylinderSource
 * @brief   generate a cylinder centered at origin
 *
 * sssSuperCylinderSource creates a polygonal cylinder centered at Center;
 * The axis of the cylinder is aligned along the global Z-axis.
 * The height and radius of the cylinder can be specified, as well as the
 * number of sides. It is also possible to control whether the cylinder is
 * open-ended or capped. If you have the end points of the cylinder, you
 * should use a vtkLineSource followed by a vtkTubeFilter instead of the
 * sssSuperCylinderSource.
 */

#ifndef sssSuperCylinderSource_h
#define sssSuperCylinderSource_h

#include "vtkFiltersSourcesModule.h" // For export macro
#include "vtkPolyDataAlgorithm.h"
#include "vtkFloatArray.h"
#include "data_structs.h"
#include "vtkCell.h" // Needed for VTK_CELL_SIZE

class  sssSuperCylinderSource : public vtkPolyDataAlgorithm
{
public:

    enum CylinderType
    {
        VTK_RCC = 0,
        VTK_REC,
        VTK_TRC
    };

  static sssSuperCylinderSource* New();
  vtkTypeMacro(sssSuperCylinderSource, vtkPolyDataAlgorithm);
  void PrintSelf(ostream &os);

  ///@{
  /**
   * Set the cube type. Defaults to VTK_RCC.
   */
  vtkSetClampMacro(SuperCylinder, int, VTK_RCC, VTK_TRC);
  vtkGetMacro(SuperCylinder, int);
  ///@}

  ///@{
  /**
   * Set the top radius t of the cylinder/TRC. Initial value is 1.
   */
  vtkSetClampMacro(RadiusTop, double, 0.0, VTK_DOUBLE_MAX);
  vtkGetMacro(RadiusTop, double);
  ///@}

  ///@{
  /**
   * Set the radius of the cylinder/RCC/TRC. Initial value is 0.5
   */
  vtkSetClampMacro(RadiusBottom, double, 0.0, VTK_DOUBLE_MAX);
  vtkGetMacro(RadiusBottom, double);
  ///@}

  ///@{
  /**
   * Set/Get cylinder anchor. Initial value is (0.0,0.0,0.0)
   */
  vtkSetVector3Macro(Anchor, double);
  vtkGetVectorMacro(Anchor, double, 3);
  ///@}

  ///@{
  /**
   * Set/Get cylinder height vector. Initial value is (0.0,0.0,0.0)
   */
  vtkSetVector3Macro(HVector, double);
  vtkGetVectorMacro(HVector, double, 3);
  ///@}

  ///@{
  /**
   * Set/Get cylinder/REC major radius vector. Initial value is (0.0,0.0,0.0)
   */
  vtkSetVector3Macro(RMajor, double);
  vtkGetVectorMacro(RMajor, double, 3);
  ///@}

  ///@{
  /**
   * Set/Get cylinder/REC minor radius vector. Initial value is (0.0,0.0,0.0)
   */
  vtkSetVector3Macro(RMinor, double);
  vtkGetVectorMacro(RMinor, double, 3);
  ///@}

  ///@{
  /**
   * Set the number of facets used to define cylinder. Initial value is 6.
   */
  vtkSetClampMacro(Resolution, int, 3, VTK_CELL_SIZE);
  vtkGetMacro(Resolution, int);
  ///@}

  ///@{
  /**
   * Set the number of facets used to define cylinder. Initial value is 6.
   */
  vtkSetClampMacro(ResHeight, int, 2, VTK_CELL_SIZE);
  vtkGetMacro(ResHeight, int);
  ///@}

  ///@{
  /**
   * Set the number of facets used to define cylinder. Initial value is 6.
   */
  vtkSetClampMacro(ResRadius, int, 2, 360);
  vtkGetMacro(ResRadius, int);
  ///@}

  ///@{
  /**
   * Turn on/off whether to cap cylinder with polygons. Initial value is true.
   */
  vtkSetMacro(Capping, vtkTypeBool);
  vtkGetMacro(Capping, vtkTypeBool);
  vtkBooleanMacro(Capping, vtkTypeBool);
  ///@}

  ///@{
  /**
   * Set/get the desired precision for the output points.
   * vtkAlgorithm::SINGLE_PRECISION - Output single-precision floating point.
   * vtkAlgorithm::DOUBLE_PRECISION - Output double-precision floating point.
   */
  vtkSetMacro(OutputPointsPrecision, int);
  vtkGetMacro(OutputPointsPrecision, int);
  ///@}

protected:
  sssSuperCylinderSource(int res = 6);
  ~sssSuperCylinderSource() override = default;

  void setupBuildParameters();
  int  tesselateCylinder();
  int  tesselateCylEndCap(const int topBottom, int vadder);

  vtkTypeBool Capping;

  int    RequestData(vtkInformation *, vtkInformationVector  **, vtkInformationVector *) override;
  double Height;
  int    Resolution;
  int    ResHeight;
  int    ResRadius;
  int    OutputPointsPrecision;
  int    SuperCylinder;

  double Anchor[3];
  double HVector[3];
  double RMajor[3];
  double RMinor[3];
  double RadiusBottom;
  double RadiusTop;
  double orientTheta;
  double orientPhi;

  vtkIdType     pts[4];
  vtkIdType     cell[4];

  vtkPoints     *newPoints;
  vtkFloatArray *newNormals;
  vtkCellArray  *newPolys;

private:
  sssSuperCylinderSource(const sssSuperCylinderSource &) = delete;
  void operator=(const sssSuperCylinderSource &) = delete;
};

#endif
