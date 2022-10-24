/*=========================================================================

  Program:   Visualization Toolkit
  Module:    sssBooleanOperationCheck.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
/**
 * @class   sssBooleanOperationCheck
 *
 *
 * This class is a modification of vtkBooleanOperationPolyDataFilter that returns
 * indicators as to whether or not the inputs intersect so that boolean operations
 * can take place without all of the other operations taking place -- to avoid
 * any other errors and warnings to take place.
 * Therefore, other operations perfomred by vtkBooleanOperationPolyDataFilter
 * do not take place with this class.
 *
 * It is intended to call this class to see if vtkBooleanOperationPolyDataFilter
 * should be called later. Uses sssIntersectionPolyDataCheck.
 *
 * Unfortunately, at this time, a lot of the baggage from vtkIntersectionPolyDataCheck
 * is left in place -- to be dealt with later.
 *
 *
 * Computes the boundary of the union, intersection, or difference
 * volume computed from the volumes defined by two input surfaces. The
 * two surfaces do not need to be manifold, but if they are not,
 * unexpected results may be obtained. The resulting surface is
 * available in the first output of the filter. The second output
 * contains a set of polylines that represent the intersection between
 * the two input surfaces.
 *
 * @warning This filter is not designed to perform 2D boolean operations,
 * and in fact relies on the inputs having no co-planar, overlapping cells.
 *
 * This code was contributed in the VTK Journal paper:
 * "Boolean Operations on Surfaces in VTK Without External Libraries"
 * by Cory Quammen, Chris Weigle C., Russ Taylor
 * http://hdl.handle.net/10380/3262
 * http://www.midasjournal.org/browse/publication/797
 */

#ifndef sssBooleanOperationCheck_h
#define sssBooleanOperationCheck_h

#include "vtkFiltersGeneralModule.h" // For export macro
#include "vtkPolyDataAlgorithm.h"

#include "vtkDataSetAttributes.h" // Needed for CopyCells() method

class vtkIdList;

class sssBooleanOperationCheck : public vtkPolyDataAlgorithm
{
public:
  /**
   * Construct object that computes the boolean surface.
   */
  static sssBooleanOperationCheck* New();

  vtkTypeMacro(sssBooleanOperationCheck, vtkPolyDataAlgorithm);

  void PrintSelf(ostream& os, vtkIndent indent) override;

  enum OperationType
  {
    VTK_UNION = 0,
    VTK_INTERSECTION,
    VTK_DIFFERENCE
  };

  ///@{
  /**
   * Set the boolean operation to perform. Defaults to union.
   */
  vtkSetClampMacro(Operation, int, VTK_UNION, VTK_DIFFERENCE);
  vtkGetMacro(Operation, int);
  void SetOperationToUnion() { this->SetOperation(VTK_UNION); }
  void SetOperationToIntersection() { this->SetOperation(VTK_INTERSECTION); }
  void SetOperationToDifference() { this->SetOperation(VTK_DIFFERENCE); }
  ///@}

  ///@{
  /**
   * Turn on/off cell reorientation of the intersection portion of the
   * surface when the operation is set to DIFFERENCE. Defaults to on.
   */
  vtkSetMacro(ReorientDifferenceCells, vtkTypeBool);
  vtkGetMacro(ReorientDifferenceCells, vtkTypeBool);
  vtkBooleanMacro(ReorientDifferenceCells, vtkTypeBool);
  ///@}

  ///@{
  /**
   * Set/get the tolerance used to determine when a point's absolute
   * distance is considered to be zero. Defaults to 1e-6.
   */
  vtkSetMacro(Tolerance, double);
  vtkGetMacro(Tolerance, double);
  ///@}

protected:
  sssBooleanOperationCheck();
  ~sssBooleanOperationCheck() override;

  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;
  int FillInputPortInformation(int, vtkInformation*) override;

private:
  sssBooleanOperationCheck(const sssBooleanOperationCheck&) = delete;
  void operator=(const sssBooleanOperationCheck&) = delete;

  /**
   * Copies cells with indices given by from one vtkPolyData to
   * another. The point and cell field lists are used to determine
   * which fields should be copied.
   */
  void CopyCells(vtkPolyData* in, vtkPolyData* out, int idx,
    vtkDataSetAttributes::FieldList& pointFieldList, vtkDataSetAttributes::FieldList& cellFieldList,
    vtkIdList* cellIds, bool reverseCells);

  /**
   * Tolerance used to determine when a point's absolute
   * distance is considered to be zero.
   */
  double Tolerance;

  /**
   * Which operation to perform.
   * Can be VTK_UNION, VTK_INTERSECTION, or VTK_DIFFERENCE.
   */
  int Operation;

  ///@{
  /**
   * Determines if cells from the intersection surface should be
   * reversed in the difference surface.
   */
  vtkTypeBool ReorientDifferenceCells;
  ///@}
};

#endif
