/*=========================================================================

  Program:   Visualization Toolkit
  Module:    sssSuperCubeSource.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   sssSuperCubeSource
 * @brief   create a polygonal representation of a cube
 *
 * sssSuperCubeSource creates a cube centered at origin. The cube is represented
 * with four-sided polygons. It is possible to specify the length, width,
 * and height of the cube independently.
 */

#ifndef sssSuperCubeSource_h
#define sssSuperCubeSource_h

#include "vtkFiltersSourcesModule.h" // For export macro
#include "vtkPolyDataAlgorithm.h"
#include "vtkFloatArray.h"


class VTKFILTERSSOURCES_EXPORT sssSuperCubeSource : public vtkPolyDataAlgorithm
{
public:

  enum CubeType
  {
    VTK_RPP = 0,
    VTK_BOX,
	VTK_WEDGE
  };

  static sssSuperCubeSource* New();
  vtkTypeMacro(sssSuperCubeSource, vtkPolyDataAlgorithm);
  void PrintSelf(ostream &os);

  ///@{
  /**
   * Set the cube type. Defaults to VTK_BOX.
   */
  vtkSetClampMacro(SuperCube, int, VTK_RPP, VTK_WEDGE);
  vtkGetMacro(SuperCube, int);
  ///@}



  ///@{
  /**
   * Set the center of the cube.
   */
  vtkSetVector3Macro(Center, double);
  vtkGetVectorMacro(Center, double, 3);
  ///@}


  ///@{
  /**
   * Set the anchor point for the cube.
   */
  vtkSetVector3Macro(Anchor, double);
  vtkGetVectorMacro(Anchor, double, 3);
  ///@}


  ///@{
  /**
   * Set the vector for side A1.
   */
  vtkSetVector3Macro(A1, double);
  vtkGetVectorMacro(A1, double, 3);
  ///@}

  ///@{
  /**
   * Set the vector for side A2.
   */
  vtkSetVector3Macro(A2, double);
  vtkGetVectorMacro(A2, double, 3);
  ///@}

  ///@{
  /**
   * Set the vector for side A3.
   */
  vtkSetVector3Macro(A3, double);
  vtkGetVectorMacro(A3, double, 3);
  ///@}

  ///@{
  /**
   * Set the number of resolutions points for side A1.
   */
  vtkSetMacro(ResA1, int);
  vtkGetMacro(ResA1, int);
  ///@}

  ///@{
  /**
   * Set the number of resolutions points for side A2.
   */
  vtkSetMacro(ResA2, int);
  vtkGetMacro(ResA2, int);
  ///@}

  ///@{
  /**
   * Set the number of resolutions points for side A3.
   */
  vtkSetMacro(ResA3, int);
  vtkGetMacro(ResA3, int);
  ///@}

  ///@{
  /**
   * Convenience methods allows creation of cube by specifying bounding box.
   */
  void SetBounds(double xMin, double xMax, double yMin, double yMax, double zMin, double zMax);
  void SetBounds(const double bounds[6]);
  void GetBounds(double bounds[6]);
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
  sssSuperCubeSource();
  ~sssSuperCubeSource() override = default;

  void   setupDirectionLengths();
  int    tesselateGeneralPlane(const int side, const int vadder);
  int    tesselateTrianglePlane(const int side, const int vadder);

  int    RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;

  double Center[3];
  double Anchor[3];
  double A1[3];
  double A2[3];
  double A3[3];
  double A4[3];
  double A1Dir[3];
  double A2Dir[3];
  double A3Dir[3];
  double A4Dir[3];
  double A4Nor[3];
  double A1Length;
  double A2Length;
  double A3Length;
  double A4Length;

  int    ResA1;
  int    ResA2;
  int    ResA3;
  int    ResA4;
  int    OutputPointsPrecision;

  int    SuperCube;

  vtkIdType     pts[4];
  vtkIdType     cell[4];

  vtkPoints     *newPoints;
  vtkFloatArray *newNormals;
  vtkCellArray  *newPolys;

private:
  sssSuperCubeSource(const sssSuperCubeSource&) = delete;
  void operator=(const sssSuperCubeSource&) = delete;
};

#endif
