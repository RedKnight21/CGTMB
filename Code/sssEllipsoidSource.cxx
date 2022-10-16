/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkEllipsoidSource.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "sssEllipsoidSource.h"
#include "sssGeometryHelpers.h"

#include "vtkCellArray.h"
#include "vtkFloatArray.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkMath.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkStreamingDemandDrivenPipeline.h"

#include <cmath>

vtkStandardNewMacro(sssEllipsoidSource);

//------------------------------------------------------------------------------
// Construct sphere with radius=0.5 and default resolution 8 in both Phi
// and Theta directions. Theta ranges from (0,360) and phi (0,180) degrees.
// Normals are generated.
sssEllipsoidSource::sssEllipsoidSource(int res)
{
  res = res < 4 ? 4 : res;
  this->Radius    = 2.0;
  this->Minor     = 0.5;
  this->Center[0] = 0.0;
  this->Center[1] = 0.0;
  this->Center[2] = 0.0;
  this->Height[0] = 0.0;
  this->Height[1] = 0.0;
  this->Height[2] = 1.0;

  this->Ellipsoid           = VTK_ELR;
  this->ThetaResolution     = res;
  this->PhiResolution       = res;
  this->StartTheta          = 0.0;
  this->EndTheta            = 360.0;
  this->StartPhi            = 0.0;
  this->EndPhi              = 180.0;
  this->LatLongTessellation = 0;

  this->OutputPointsPrecision = vtkAlgorithm::SINGLE_PRECISION;

  this->GenerateNormals = true;

  this->SetNumberOfInputPorts(0);
}

//------------------------------------------------------------------------------
int sssEllipsoidSource::RequestData(vtkInformation *vtkNotUsed(request),
  vtkInformationVector **vtkNotUsed(inputVector), vtkInformationVector *outputVector)
{
  // get the info object
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // get the output
  vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  double a2, c2;

  // ---------------------------------------------------------------------------
  if (Ellipsoid == VTK_ELR)
  {
    double x = this->Height[0];
    double y = this->Height[1];
    double z = this->Height[2];

    c2 = x*x + y*y + z*z;
    this->Major = sqrt(c2);

    this->Minor = this->Radius;
    a2 = this->Minor * this->Minor;

    this->Orient[0] = this->Height[0] / this->Major;
    this->Orient[1] = this->Height[1] / this->Major;
    this->Orient[2] = this->Height[2] / this->Major;

    x = sqrt(c2 - a2);

    this->Loci[0] = this->Center[0] - this->Orient[0] * x;
    this->Loci[1] = this->Center[1] - this->Orient[1] * x;
    this->Loci[2] = this->Center[2] - this->Orient[2] * x;

    this->Loci[3] = this->Center[0] + this->Orient[0] * x;
    this->Loci[4] = this->Center[1] + this->Orient[1] * x;
    this->Loci[5] = this->Center[2] + this->Orient[2] * x;
  }
  else
  {
    this->Center[0] = (this->Loci[3] + this->Loci[0]) * 0.5;
    this->Center[1] = (this->Loci[4] + this->Loci[1]) * 0.5;
    this->Center[2] = (this->Loci[5] + this->Loci[2]) * 0.5;

    double x = this->Loci[3] - this->Loci[0];
    double y = this->Loci[4] - this->Loci[1];
    double z = this->Loci[5] - this->Loci[2];

    a2  = sqrt(x*x + y*y + z*z);
    this->Orient[0] = x / a2;
    this->Orient[1] = y / a2;
    this->Orient[2] = z / a2;

    a2 /= 2.0;

    this->Major = this->Radius * 0.5;
    c2 = this->Major * this->Major;

    if (this->Major < a2)
    {
      printf("\n>>>>> ERROR: Semi-major axis < focal radii\n");
    }

    a2 *= a2;
    this->Minor = sqrt(c2 - a2);
    a2 = this->Minor * this->Minor;

    this->Height[0] = this->Orient[0] * this->Major;
    this->Height[1] = this->Orient[1] * this->Major;
    this->Height[2] = this->Orient[2] * this->Major;
  }

  double orientTheta, orientPhi;
  findThetaPhi(this->Height, orientTheta, orientPhi);

//  printf("orientTheta: %f   orientPhi: %f \n", orientTheta, orientPhi);

  // ---------------------------------------------------------------------------

  vtkPoints     *newPoints;
  vtkFloatArray *newNormals = nullptr;
  vtkCellArray  *newPolys;
  vtkIdType     pts[4];
  double        x[3], n[3], deltaPhi, deltaTheta, phi, theta, radius, norm;
  double        startTheta, endTheta, startPhi, endPhi;
  int           base, numPoles = 0, thetaResolution, phiResolution;
  int           i, j;
  int           jStart, jEnd, numOffset;
  int           numPts, numPolys;

  int piece     = outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_PIECE_NUMBER());
  int numPieces = outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_PIECES());

  if (numPieces > this->ThetaResolution)
  {
    numPieces = this->ThetaResolution;
  }
  if (piece >= numPieces)
  {
    // Although the super class should take care of this,
    // it cannot hurt to check here.
    return 1;
  }

  // I want to modify the ivars resoultion start theta and end theta,
  // so I will make local copies of them.  THese might be able to be merged
  // with the other copies of them, ...
  int localThetaResolution = this->ThetaResolution;
  double localStartTheta   = this->StartTheta;
  double localEndTheta     = this->EndTheta;

  while (localEndTheta < localStartTheta)
  {
    localEndTheta += 360.0;
  }
  deltaTheta = (localEndTheta - localStartTheta) / localThetaResolution;

  // Change the ivars based on pieces.
  int start, end;
  start = piece * localThetaResolution / numPieces;
  end   = (piece + 1) * localThetaResolution / numPieces;

  localEndTheta   = localStartTheta + (double)(end)*deltaTheta;
  localStartTheta = localStartTheta + (double)(start)*deltaTheta;

  localThetaResolution = end - start;

  // Set things up; allocate memory
  //
  vtkDebugMacro("EllipsoidSource Executing piece index " << piece << " of " << numPieces << " pieces.");

  numPts = this->PhiResolution * localThetaResolution + 2;
  // creating triangles
  numPolys = this->PhiResolution * 2 * localThetaResolution;

  newPoints = vtkPoints::New();

  // Set the desired precision for the points in the output.
  if (this->OutputPointsPrecision == vtkAlgorithm::DOUBLE_PRECISION)
  {
    newPoints->SetDataType(VTK_DOUBLE);
  }
  else
  {
    newPoints->SetDataType(VTK_FLOAT);
  }
  newPoints->Allocate(numPts);

  if (this->GenerateNormals)
  {
    newNormals = vtkFloatArray::New();
    newNormals->SetNumberOfComponents(3);
    newNormals->Allocate(3 * numPts);
    newNormals->SetName("Normals");
  }

  newPolys = vtkCellArray::New();
  newPolys->AllocateEstimate(numPolys, 3);

  // -----------------------------------
  // Create Ellipsoid
  // Build with semi-major axis along Z.
  //
  // Create north pole if needed
  if (this->StartPhi <= 0.0)
  {
    x[0] = 0.0;
    x[1] = 0.0;
    x[2] = this->Major;

    // Rotate to final orientation.
    rotateToGlobal(x, orientPhi, orientTheta);

    // Translate into position.
    x[0] += this->Center[0];
    x[1] += this->Center[1];
    x[2] += this->Center[2];

    newPoints->InsertPoint(numPoles, x);

    if (newNormals)
    {
      x[0] = x[1] = 0.0;
      x[2] = 1.0;
      rotateToGlobal(x, orientPhi, orientTheta);
      newNormals->InsertTuple(numPoles, x);
    }
    numPoles++;
  }

  // Create south pole if needed
  if (this->EndPhi >= 180.0)
  {
    x[0] = 0.0;
    x[1] = 0.0;
    x[2] = -this->Major;

    // Rotate to final orientation.
    rotateToGlobal(x, orientPhi, orientTheta);

    // Translate into position.
    x[0] += this->Center[0];
    x[1] += this->Center[1];
    x[2] += this->Center[2];

    newPoints->InsertPoint(numPoles, x);

    if (newNormals)
    {
      x[0] = x[1] = 0.0;
      x[2] = -1.0;
      rotateToGlobal(x, orientPhi, orientTheta);
      newNormals->InsertTuple(numPoles, x);
    }
    numPoles++;
  }

  // Check data, determine increments, and convert to radians
  startTheta  = (localStartTheta < localEndTheta ? localStartTheta : localEndTheta);
  startTheta *= vtkMath::Pi() / 180.0;

  endTheta    = (localEndTheta > localStartTheta ? localEndTheta : localStartTheta);
  endTheta   *= vtkMath::Pi() / 180.0;

  startPhi  = (this->StartPhi < this->EndPhi ? this->StartPhi : this->EndPhi);
  startPhi *= vtkMath::Pi() / 180.0;
  endPhi    = (this->EndPhi > this->StartPhi ? this->EndPhi : this->StartPhi);
  endPhi   *= vtkMath::Pi() / 180.0;

  phiResolution   = this->PhiResolution - numPoles;
  deltaPhi        = (endPhi - startPhi) / (this->PhiResolution - 1);
  thetaResolution = localThetaResolution;
  if (fabs(localStartTheta - localEndTheta) < 360.0)
  {
    ++localThetaResolution;
  }
  deltaTheta = (endTheta - startTheta) / thetaResolution;

  jStart = (this->StartPhi <= 0.0 ? 1 : 0);
  jEnd   = (this->EndPhi >= 180.0 ? this->PhiResolution - 1 : this->PhiResolution);

  this->UpdateProgress(0.1);

  // Create intermediate points
  for (i = 0; i < localThetaResolution; ++i)
  {
    theta = localStartTheta * vtkMath::Pi() / 180.0 + i * deltaTheta;

    for (j = jStart; j < jEnd; ++j)
    {
      phi    = startPhi + j * deltaPhi;
      radius = this->Minor * sin((double)phi);

      x[0] = n[0] = radius * cos((double)theta);
      x[1] = n[1] = radius * sin((double)theta);
      x[2] = n[2] = this->Major * cos((double)phi);

      // Rotate to final orientation.
      rotateToGlobal(x, orientPhi, orientTheta);

      // Translate into position.
      x[0] += this->Center[0];
      x[1] += this->Center[1];
      x[2] += this->Center[2];

      newPoints->InsertNextPoint(x);

      // Finalize the normals.
      n[0] /= a2;
      n[1] /= a2;
      n[3] /= c2;

      // Rotate to final orientation.
      rotateToGlobal(n, orientPhi, orientTheta);

      if (newNormals)
      {
        if ((norm = vtkMath::Norm(n)) == 0.0)
        {
          norm = 1.0;
        }
        n[0] /= norm;
        n[1] /= norm;
        n[2] /= norm;

        newNormals->InsertNextTuple(n);
      }
    }
    this->UpdateProgress(0.10 + 0.50 * i / static_cast<float>(localThetaResolution));
  }

  // Generate mesh connectivity
  base = phiResolution * localThetaResolution;

  if (fabs(localStartTheta - localEndTheta) < 360.0)
  {
    --localThetaResolution;
  }

  if (this->StartPhi <= 0.0) // around north pole
  {
    for (i = 0; i < localThetaResolution; i++)
    {
      pts[0] = phiResolution * i + numPoles;
      pts[1] = (phiResolution * (i + 1) % base) + numPoles;
      pts[2] = 0;
      newPolys->InsertNextCell(3, pts);
    }
  }

  if (this->EndPhi >= 180.0) // around south pole
  {
    numOffset = phiResolution - 1 + numPoles;

    for (i = 0; i < localThetaResolution; i++)
    {
      pts[0] = phiResolution * i + numOffset;
      pts[2] = ((phiResolution * (i + 1)) % base) + numOffset;
      pts[1] = numPoles - 1;
      newPolys->InsertNextCell(3, pts);
    }
  }
  this->UpdateProgress(0.70);

  // bands in-between poles
  for (i = 0; i < localThetaResolution; i++)
  {
    for (j = 0; j < (phiResolution - 1); j++)
    {
      pts[0] = phiResolution * i + j + numPoles;
      pts[1] = pts[0] + 1;
      pts[2] = ((phiResolution * (i + 1) + j) % base) + numPoles + 1;
      if (!this->LatLongTessellation)
      {
        newPolys->InsertNextCell(3, pts);
        pts[1] = pts[2];
        pts[2] = pts[1] - 1;
        newPolys->InsertNextCell(3, pts);
      }
      else
      {
        pts[3] = pts[2] - 1;
        newPolys->InsertNextCell(4, pts);
      }
    }
    this->UpdateProgress(0.70 + 0.30 * i / static_cast<double>(localThetaResolution));
  }

  // Update ourselves and release memory
  //
  newPoints->Squeeze();
  output->SetPoints(newPoints);
  newPoints->Delete();

  if (newNormals)
  {
    newNormals->Squeeze();
    output->GetPointData()->SetNormals(newNormals);
    newNormals->Delete();
  }

  newPolys->Squeeze();
  output->SetPolys(newPolys);
  newPolys->Delete();

  return 1;
}

//------------------------------------------------------------------------------
void sssEllipsoidSource::PrintSelf(ostream &os)
{
  //this->Superclass::PrintSelf(os, indent);

  os << "\nEllipsoid type: "        << Ellipsoid                   << "\n";
  os << "LatLong Tessellation: "    << this->LatLongTessellation   << "\n";
  os << "Output Points Precision: " << this->OutputPointsPrecision << "\n";
  os << "Generate Normals: "        << this->GenerateNormals       << "\n";

  os << "Theta Resolution: " << this->ThetaResolution << "\n";
  os << "Phi Resolution: "   << this->PhiResolution   << "\n";
  os << "Theta Start: "      << this->StartTheta      << "\n";
  os << "Phi Start: "        << this->StartPhi        << "\n";
  os << "Theta End: "        << this->EndTheta        << "\n";
  os << "Phi End: "          << this->EndPhi          << "\n";
  os << "Radius: "           << this->Radius          << "\n";
  os << "Major : "           << this->Major           << "\n";
  os << "Minor : "           << this->Minor           << "\n";

  os << "Center: (" << this->Center[0] << ", " << this->Center[1] << ", " << this->Center[2] << ")\n";
  os << "Orient: (" << this->Orient[0] << ", " << this->Orient[1] << ", " << this->Orient[2] << ")\n";
  os << "Height: (" << this->Height[0] << ", " << this->Height[1] << ", " << this->Height[2] << ")\n";
  os << "Locus #1: (" << this->Loci[0] << ", " << this->Loci[1]   << ", " << this->Loci[2]   << ")\n";
  os << "Locus #2: (" << this->Loci[3] << ", " << this->Loci[4]   << ", " << this->Loci[5]   << ")\n";
}

//------------------------------------------------------------------------------
int sssEllipsoidSource::RequestInformation(vtkInformation *vtkNotUsed(request),
  vtkInformationVector **vtkNotUsed(inputVector), vtkInformationVector *outputVector)
{
  // get the info object
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  outInfo->Set(CAN_HANDLE_PIECE_REQUEST(), 1);

  return 1;
}
