/*=========================================================================

  Program:   Visualization Toolkit
  Module:    sssBooleanOperationCheck.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "sssBooleanOperationCheck.h"

#include "vtkCellData.h"
#include "vtkDistancePolyDataFilter.h"
#include "vtkDoubleArray.h"
#include "vtkFloatArray.h"
#include "vtkGenericCell.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "sssIntersectionPolyDataCheck.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkSmartPointer.h"
#include <vtkCommand.h>
#include <vtkExecutive.h>

vtkStandardNewMacro(sssBooleanOperationCheck);

//------------------------------------------------------------------------------
sssBooleanOperationCheck::sssBooleanOperationCheck()
{
  this->Tolerance = 1e-6;
  this->Operation = VTK_UNION;
  this->ReorientDifferenceCells = 1;

  this->SetNumberOfInputPorts(2);
  this->SetNumberOfOutputPorts(2);
}

//------------------------------------------------------------------------------
sssBooleanOperationCheck::~sssBooleanOperationCheck() = default;

//------------------------------------------------------------------------------
int sssBooleanOperationCheck::RequestData(vtkInformation* vtkNotUsed(request),
  vtkInformationVector** inputVector, vtkInformationVector* outputVector)
{
  vtkInformation* inInfo0 = inputVector[0]->GetInformationObject(0);
  vtkInformation* inInfo1 = inputVector[1]->GetInformationObject(0);
  vtkInformation* outInfo0 = outputVector->GetInformationObject(0);
  vtkInformation* outInfo1 = outputVector->GetInformationObject(1);

  if (!inInfo0 || !inInfo1 || !outInfo0 || !outInfo1)
  {
    return 0;
  }

  vtkPolyData* input0 = vtkPolyData::SafeDownCast(inInfo0->Get(vtkDataObject::DATA_OBJECT()));
  vtkPolyData* input1 = vtkPolyData::SafeDownCast(inInfo1->Get(vtkDataObject::DATA_OBJECT()));

  vtkPolyData* outputSurface =
    vtkPolyData::SafeDownCast(outInfo0->Get(vtkDataObject::DATA_OBJECT()));
  vtkPolyData* outputIntersection =
    vtkPolyData::SafeDownCast(outInfo1->Get(vtkDataObject::DATA_OBJECT()));

  if (!input0 || !input1 || !outputSurface || !outputIntersection)
  {
    return 0;
  }

  // Get intersected versions
  vtkSmartPointer<sssIntersectionPolyDataCheck> PolyDataIntersection =
    vtkSmartPointer<sssIntersectionPolyDataCheck>::New();

  PolyDataIntersection->SetInputConnection(0, this->GetInputConnection(0, 0));
  PolyDataIntersection->SetInputConnection(1, this->GetInputConnection(1, 0));

  PolyDataIntersection->SplitFirstOutputOn();
  PolyDataIntersection->SplitSecondOutputOn();

  PolyDataIntersection->DebugOn();

  bool b1 = PolyDataIntersection->GetExecutive()->Update();

//  printf("sssBooleanOperationCheck:: return %d\n", b1);


  if (b1 != 1) return 0;

  if (PolyDataIntersection->GetStatus() != 1)
  {
//      printf("sssBooleanOperationCheck:: return %d\n", 0);

    return 0;
  }

//  printf("sssBooleanOperationCheck:: return %d\n", 1);

  return 1;
}

//------------------------------------------------------------------------------
void sssBooleanOperationCheck::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);

  os << indent << "Tolerance: " << this->Tolerance << "\n";
  os << indent << "Operation: ";
  switch (this->Operation)
  {
    case VTK_UNION:
      os << "UNION";
      break;

    case VTK_INTERSECTION:
      os << "INTERSECTION";
      break;

    case VTK_DIFFERENCE:
      os << "DIFFERENCE";
      break;
  }
  os << "\n";
  os << indent << "ReorientDifferenceCells: " << this->ReorientDifferenceCells << "\n";
}

//------------------------------------------------------------------------------
int sssBooleanOperationCheck::FillInputPortInformation(int port, vtkInformation* info)
{
  if (!this->Superclass::FillInputPortInformation(port, info))
  {
    return 0;
  }
  if (port == 0)
  {
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkPolyData");
  }
  else if (port == 1)
  {
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkPolyData");
    info->Set(vtkAlgorithm::INPUT_IS_OPTIONAL(), 0);
  }
  return 1;
}

//------------------------------------------------------------------------------
void sssBooleanOperationCheck ::CopyCells(vtkPolyData* in, vtkPolyData* out, int idx,
  vtkDataSetAttributes::FieldList& pointFieldList, vtkDataSetAttributes::FieldList& cellFieldList,
  vtkIdList* cellIds, bool reverseCells)
{
  // Largely copied from vtkPolyData::CopyCells, but modified to
  // use the special form of CopyData that uses a field list to
  // determine which data values to copy over.

  vtkPointData* outPD = out->GetPointData();
  vtkCellData* outCD = out->GetCellData();

  vtkFloatArray* outNormals = nullptr;
  if (reverseCells)
  {
    outNormals = vtkArrayDownCast<vtkFloatArray>(outPD->GetArray("Normals"));
  }

  vtkIdType numPts = in->GetNumberOfPoints();

  if (out->GetPoints() == nullptr)
  {
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    out->SetPoints(points);
  }

  vtkPoints* newPoints = out->GetPoints();

  vtkSmartPointer<vtkIdList> pointMap = vtkSmartPointer<vtkIdList>::New();
  pointMap->SetNumberOfIds(numPts);
  for (vtkIdType i = 0; i < numPts; i++)
  {
    pointMap->SetId(i, -1);
  }

  // Filter the cells
  vtkSmartPointer<vtkGenericCell> cell = vtkSmartPointer<vtkGenericCell>::New();
  vtkSmartPointer<vtkIdList> newCellPts = vtkSmartPointer<vtkIdList>::New();
  for (vtkIdType cellId = 0; cellId < cellIds->GetNumberOfIds(); cellId++)
  {
    in->GetCell(cellIds->GetId(cellId), cell);
    vtkIdList* cellPts = cell->GetPointIds();
    vtkIdType numCellPts = cell->GetNumberOfPoints();

    for (vtkIdType i = 0; i < numCellPts; i++)
    {
      vtkIdType ptId = cellPts->GetId(i);
      vtkIdType newId = pointMap->GetId(ptId);
      if (newId < 0)
      {
        double x[3];
        in->GetPoint(ptId, x);
        newId = newPoints->InsertNextPoint(x);
        pointMap->SetId(ptId, newId);
        outPD->CopyData(pointFieldList, in->GetPointData(), idx, ptId, newId);

        if (reverseCells && outNormals)
        {
          float normal[3];
          outNormals->GetTypedTuple(newId, normal);
          normal[0] *= -1.0;
          normal[1] *= -1.0;
          normal[2] *= -1.0;
          outNormals->SetTypedTuple(newId, normal);
        }
      }
      newCellPts->InsertId(i, newId);
    }
    if (reverseCells)
    {
      for (vtkIdType i = 0; i < newCellPts->GetNumberOfIds() / 2; i++)
      {
        vtkIdType i1 = i;
        vtkIdType i2 = newCellPts->GetNumberOfIds() - i - 1;

        vtkIdType id = newCellPts->GetId(i1);
        newCellPts->SetId(i1, newCellPts->GetId(i2));
        newCellPts->SetId(i2, id);
      }
    }

    vtkIdType newCellId = out->InsertNextCell(cell->GetCellType(), newCellPts);
    outCD->CopyData(cellFieldList, in->GetCellData(), idx, cellIds->GetId(cellId), newCellId);

    newCellPts->Reset();
  } // for all cells
}
