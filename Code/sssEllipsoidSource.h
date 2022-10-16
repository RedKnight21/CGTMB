/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkEllipsoidSource.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   sssEllipsoidSource
 * @brief   create a polygonal sphere centered at the origin
 *
 * sssEllipsoidSource creates an ellipsoid (represented by polygons) of specified
 * by a either two loci and a major axis OR center, orientation vector, and minor radius.
 * Default is centered at the origin with semi-major axis (radius) of 1.0 along Z and a 
 * semi-minor axis (radius) of 0.5
 * The resolution (polygonal discretization)
 * in both the latitude (phi) and longitude (theta) directions can be
 * specified. It also is possible to create partial ellipsoids by specifying
 * maximum phi and theta angles. By default, the surface tessellation of
 * the ellipsoid uses triangles; however you can set LatLongTessellation to
 * produce a tessellation using quadrilaterals.
 *
 * @warning
 * Resolution means the number of latitude or longitude lines for a complete
 * ellipsoid. If you create partial ellipsoids the number of latitude/longitude
 * lines may be off by one.
 */

#ifndef sssEllipsoidSource_h
#define sssEllipsoidSource_h

#include "vtkFiltersSourcesModule.h" // For export macro
#include "vtkPolyDataAlgorithm.h"

class VTKFILTERSSOURCES_EXPORT sssEllipsoidSource : public vtkPolyDataAlgorithm
{
public:

  enum EllipsoidType
  {
    VTK_ELL = 0,
    VTK_ELR
  };

  ///@{
  /**
   * Standard methods for obtaining type information, and printing.
   */
  vtkTypeMacro(sssEllipsoidSource, vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os);
  ///@}

  /**
   * Construct sphere with radius=0.5 and default resolution 8 in both Phi
   * and Theta directions. Theta ranges from (0,360) and phi (0,180) degrees.
   */
  static sssEllipsoidSource* New();

  ///@{
  /**
   * Set the radius of sphere. Default is 0.5.
   */
  vtkSetClampMacro(Radius, double, 0.0, VTK_DOUBLE_MAX);
  vtkGetMacro(Radius, double);
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
   * Set the number of points in the longitude direction (ranging from
   * StartTheta to EndTheta).
   */
  vtkSetClampMacro(ThetaResolution, int, 3, VTK_INT_MAX);
  vtkGetMacro(ThetaResolution, int);
  ///@}

  ///@{
  /**
   * Set the number of points in the latitude direction (ranging
   * from StartPhi to EndPhi).
   */
  vtkSetClampMacro(PhiResolution, int, 3, VTK_INT_MAX);
  vtkGetMacro(PhiResolution, int);
  ///@}

  ///@{
  /**
   * Set the starting longitude angle. By default StartTheta=0 degrees.
   */
  vtkSetClampMacro(StartTheta, double, 0.0, 360.0);
  vtkGetMacro(StartTheta, double);
  ///@}

  ///@{
  /**
   * Set the ending longitude angle. By default EndTheta=360 degrees.
   */
  vtkSetClampMacro(EndTheta, double, 0.0, 360.0);
  vtkGetMacro(EndTheta, double);
  ///@}

  ///@{
  /**
   * Set the starting latitude angle (0 is at north pole). By default
   * StartPhi=0 degrees.
   */
  vtkSetClampMacro(StartPhi, double, 0.0, 360.0);
  vtkGetMacro(StartPhi, double);
  ///@}

  ///@{
  /**
   * Set the ending latitude angle. By default EndPhi=180 degrees.
   */
  vtkSetClampMacro(EndPhi, double, 0.0, 360.0);
  vtkGetMacro(EndPhi, double);
  ///@}

  ///@{
  /**
   * Set the semi-major height vector. Default is (0,0,0).
   */
  vtkSetVector3Macro(Height, double);
  vtkGetVectorMacro(Height, double, 3);
  ///@}

  ///@{
  /**
   * Set the loci points. Default is (0,0,0).
   */
  vtkSetVector6Macro(Loci, double);
  vtkGetVector6Macro(Loci, double);
  ///@}

  ///@{
  /**
   * Set the boolean operation to perform. Defaults to VTK_ELL.
   */
  vtkSetClampMacro(Ellipsoid, int, VTK_ELL, VTK_ELR);
  vtkGetMacro(Ellipsoid, int);
  ///@}
  ///
  ///@{
  /**
   * Cause the sphere to be tessellated with edges along the latitude
   * and longitude lines. If off, triangles are generated at non-polar
   * regions, which results in edges that are not parallel to latitude and
   * longitude lines. If on, quadrilaterals are generated everywhere
   * except at the poles. This can be useful for generating a wireframe
   * sphere with natural latitude and longitude lines.
   */
  vtkSetMacro(LatLongTessellation, vtkTypeBool);
  vtkGetMacro(LatLongTessellation, vtkTypeBool);
  vtkBooleanMacro(LatLongTessellation, vtkTypeBool);
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
  sssEllipsoidSource(int res = 8);
  ~sssEllipsoidSource() override = default;

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
  int RequestInformation(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

  double Radius;
  double Major;
  double Minor;
  double Center[3];
  double Height[3];
  double Orient[3];
  double Loci[6];

  double StartTheta;
  double EndTheta;
  double StartPhi;
  double EndPhi;

  int    Ellipsoid;
  int    OutputPointsPrecision;
  int    ThetaResolution;
  int    PhiResolution;

  vtkTypeBool LatLongTessellation;
  vtkTypeBool GenerateNormals;

private:
  sssEllipsoidSource(const sssEllipsoidSource &) = delete;
  void operator=(const sssEllipsoidSource &) = delete;
};

#endif
