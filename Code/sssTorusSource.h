/*=========================================================================

  Program:   Visualization Toolkit
  Module:    sssTorusSource.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   sssTorusSource
 * @brief   create a polygonal torus centered at the origin
 *
 * sssTorusSource creates a torus (represented by polygons) by rotating an ellipse
 * about an axis of revolution.
 * Default is centered at the origin with a rotation radius of 2.0 about the Z-axis. 
 * Radii of the ellipse are 0.5 implying a circle.
 * The resolution (polygonal discretization) in the number of segments about the
 * axis of revolution and the number of intervals in which the ellipse is segmented
 * can be specified.
 *
 */

#ifndef sssTorusSource_h
#define sssTorusSource_h

#include "vtkFiltersSourcesModule.h" // For export macro
#include "vtkPolyDataAlgorithm.h"
#include "vtkFloatArray.h"
#include "data_structs.h"


class  sssTorusSource : public vtkPolyDataAlgorithm
{
public:

  ///@{
  /**
   * Standard methods for obtaining type information, and printing.
   */
  vtkTypeMacro(sssTorusSource, vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os);
  ///@}

  /**
   * Construct sphere with radius=0.5 and default resolution 8 in both Phi
   * and Theta directions. Theta ranges from (0,360) and phi (0,180) degrees.
   */
  static sssTorusSource* New();

  ///@{
  /**
   * Set the radius of sphere. Default is 0.5.
   */
  vtkSetClampMacro(Radius, double, 0.0, VTK_DOUBLE_MAX);
  vtkGetMacro(Radius, double);
  ///@}

  ///@{
  /**
   * Set the radius of sphere. Default is 0.5.
   */
  vtkSetClampMacro(HRadius, double, 0.0, VTK_DOUBLE_MAX);
  vtkGetMacro(HRadius, double);
  ///@}

  ///@{
  /**
   * Set the radius of sphere. Default is 0.5.
   */
  vtkSetClampMacro(PRadius, double, 0.0, VTK_DOUBLE_MAX);
  vtkGetMacro(PRadius, double);
  ///@}


  ///@{
  /**
   * Set the center of the sphere. Default is (0,0,0).
   */
  vtkSetVector3Macro(Center, double);
  vtkGetVectorMacro(Center, double, 3);
  ///@}

  ///@{
  /**
   * Set the number of segments about the axis of revolution.
   */
  vtkSetClampMacro(SegmentsAboutAxis, int, 5, VTK_INT_MAX);
  vtkGetMacro(SegmentsAboutAxis, int);
  ///@}

  ///@{
  /**
   * Set the number of points on the ellipse that defines the
   * surface cross section.
   */
  vtkSetClampMacro(SegmentsOnSurface, int, 10, VTK_INT_MAX);
  vtkGetMacro(SegmentsOnSurface, int);
  ///@}


  ///@{
  /**
   * Set the semi-major height vector. Default is (0,0,0).
   */
  vtkSetVector3Macro(HVector, double);
  vtkGetVectorMacro(HVector, double, 3);
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

  ///@{
  /**
   * Specify whether to generate output point normals. By default this is
   * enabled.
   */
  vtkSetMacro(GenerateNormals, vtkTypeBool);
  vtkGetMacro(GenerateNormals, vtkTypeBool);
  vtkBooleanMacro(GenerateNormals, vtkTypeBool);
  ///@}

protected:
  sssTorusSource();
  ~sssTorusSource() override = default;

  int RequestData(vtkInformation *,        vtkInformationVector **, vtkInformationVector *) override;
  int RequestInformation(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;

  double Radius;
  double HRadius;
  double PRadius;
  double Center[3];
  double HVector[3];

  int    OutputPointsPrecision;
  int    SegmentsAboutAxis;
  int    SegmentsOnSurface;

  vtkTypeBool LatLongTessellation;
  vtkTypeBool GenerateNormals;

  vtkPoints     *newPoints;
  vtkFloatArray *newNormals;
  vtkCellArray  *newPolys;

  vtkIdType     cell[4];

private:
  sssTorusSource(const sssTorusSource &) = delete;
  void operator=(const sssTorusSource &) = delete;
  void tesselateTorus(const int lats, const int lngs);
};

#endif
