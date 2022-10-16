/*=========================================================================

  Program:   Visualization Toolkit
  Module:    sssSuperCylinderSource.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "sssEllipticalCylinderSource.h"
#include "sssGeometryHelpers.h"

#include "vtkCellArray.h"
#include "vtkFloatArray.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"

#include <cmath>
#include <vector>

using namespace std;

vtkStandardNewMacro(sssSuperCylinderSource);

sssSuperCylinderSource::sssSuperCylinderSource(int res)
{
  this->Resolution   = res;
  this->Height       = 1.0;
  this->RadiusBottom = 0.5;
  this->RadiusTop    = 0.4;
  this->Capping      = 1;
  this->Anchor[0]    = 0.0;
  this->Anchor[1]    = 0.0;
  this->Anchor[2]    = 0.0;

  this->HVector[0]  = 0.0;
  this->HVector[1]  = 0.0;
  this->HVector[2]  = 1.0;

  this->RMajor[0]   = 0.5;
  this->RMajor[1]   = 0.0;
  this->RMajor[2]   = 0.0;

  this->RMinor[0]   = 0.0;
  this->RMinor[1]   = 0.4;
  this->RMinor[2]   = 0.0;

  this->ResHeight   = 5;
  this->ResRadius   = 30;

  this->OutputPointsPrecision = SINGLE_PRECISION;

  this->SetNumberOfInputPorts(0);

  this->SuperCylinder = VTK_RCC;
}
// -----------------------------------------------------------------------------
int sssSuperCylinderSource::RequestData(vtkInformation *vtkNotUsed(request),
  vtkInformationVector** vtkNotUsed(inputVector), vtkInformationVector *outputVector)
{
  // get the info object
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // get the output
  vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  double angle = 2.0 * vtkMath::Pi() / this->Resolution;

  //
  // Set things up; allocate memory
  //
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

  newNormals = vtkFloatArray::New();
  newNormals->SetNumberOfComponents(3);
  newNormals->SetName("Normals");

  newPolys = vtkCellArray::New();


  setupBuildParameters();
  int    num = 0;
  int    vadder = 0;

  setupBuildParameters();
  num = tesselateCylinder();

  if (this->Capping)
  {
	vadder += num;
	num     = tesselateCylEndCap(0, vadder);

	vadder += num;
	num     = tesselateCylEndCap(1, vadder);
}

  // -----------------------------------
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

// -----------------------------------------------------------------------------

void sssSuperCylinderSource::setupBuildParameters()
{
	Height = HVector[0]*HVector[0] + HVector[1]*HVector[1] + HVector[2]*HVector[2];
	Height = sqrt(Height);

	findThetaPhi(this->HVector, orientTheta, orientPhi);

	if (SuperCylinder == VTK_REC)
	{
		RadiusBottom = RMajor[0]*RMajor[0] + RMajor[1]*RMajor[1] + RMajor[2]*RMajor[2];
		RadiusBottom = sqrt(RadiusBottom);
		RadiusTop    = RMinor[0]*RMinor[0] + RMinor[1]*RMinor[1] + RMinor[2]*RMinor[2];
		RadiusTop    = sqrt(RadiusTop);

	}
}
// -----------------------------------------------------------------------------

int sssSuperCylinderSource::tesselateCylinder()
{
    double theta2, phi2, d;

	double trcH = 0.0;
    double ar;
    double br;

	double node[3];
	double normal[3];

    SSS_ABC         abc;
    XYZ2            xyz, xyz2;
    vector<SSS_ABC> odds;
    vector<SSS_ABC> evens;

	int  count = 0;


  if (SuperCylinder == VTK_RCC)
  {
	ar = RadiusBottom;
	br = RadiusBottom;
  }
  else if (SuperCylinder == VTK_REC)
  {
	ar = RadiusBottom;
	br = RadiusTop;
  }
  else if (SuperCylinder == VTK_TRC)
  {
	ar = RadiusBottom;
	br = RadiusTop;
    trcH    = ar  * Height / (ar - br);
  }

  int lats = max(2, ResHeight);
  int lngs = ResRadius;

  double delta_lat = Height / ((double) lats);
  double delta_lng = (2.0 * vtkMath::Pi()) / ((double) lngs);
  double lat_start = 0.0;


//  printf("lats: %d\n", lats);
//  printf("lngs: %d\n", lngs);

//	printf("delta_lat  : %e\n", delta_lat);
//	printf("delta_lng  : %e\n", delta_lng);


    ++lats;

    xyz2.z = 0.0;
    abc.c  = 0;

    for (int i=0; i < lats; ++i)
    {
        // Start at the bottom.
        theta2 = lat_start + ((double) i) * delta_lat;
        xyz.z  = theta2;

        if (SuperCylinder == VTK_TRC)
        {
            ar = RadiusBottom * (trcH - (delta_lat * i)) / trcH;
            br = ar;
        }

        for (int j=0; j < lngs; ++j)
        {
            phi2   = ((double) j * delta_lng);
            xyz.x = ar * cos(phi2);
            xyz.y = br * sin(phi2);

            if (fabs(xyz.x) < 1.0e-10) xyz.x = 0.0;
            if (fabs(xyz.y) < 1.0e-10) xyz.y = 0.0;

            // Normals point outward in XY-plane.
            // Will be re-oriented in reconcileObjects.
            d = xyz.x * xyz.x + xyz.y * xyz.y;
            d = sqrt(d);
            xyz2.x = xyz.x / d;
            xyz2.y = xyz.y / d;

			++count;
			node[0] = xyz.x;
			node[1] = xyz.y;
			node[2] = xyz.z;
      		rotateToGlobal(node, orientPhi, orientTheta);

			node[0] += this->Anchor[0];
			node[1] += this->Anchor[1];
			node[2] += this->Anchor[2];

			normal[0] = xyz2.x;
			normal[1] = xyz2.y;
			normal[2] = xyz2.z;
      		rotateToGlobal(normal, orientPhi, orientTheta);

			// Insert node.
			newPoints->InsertNextPoint(node);
			newNormals->InsertNextTuple(normal);
        }
    }               // -- end i-loop

	// --------------------------------------
    // Construct the triangle connectivities.
    // Lower left half by row.

	odds.clear();
	evens.clear();

    for(int j=0; j < lats-1; ++j)
    {
        int a = 0;
        for(int i=0; i < lngs-1; ++i)
        {
            a     = (lngs * j) + i;
            abc.a = a;
            abc.b = a + 1;
            abc.c = a + lngs;
            evens.push_back(abc);
        }
        abc.a = a + 1;
        abc.b = abc.a - lngs + 1;
        abc.c = abc.c + 1;
        evens.push_back(abc);
    }

    // Upper right half by row.
    for(int j=0; j < lats-1; ++j)
    {
        int a;
        for(int i=1; i < lngs; ++i)
        {
            a = (lngs * j) + i;
            abc.a = a;
            abc.b = a + lngs;
            abc.c = a + lngs - 1;
            odds.push_back(abc);
        }
        abc.a = abc.a - lngs + 1;
        abc.c = abc.b;
        abc.b = a + 1;
        odds.push_back(abc);
    }

    // Re-order the vertices into the master vector.
    for(size_t i=0; i < evens.size(); ++i)
    {
        cell[0] = evens[i].a;
        cell[1] = evens[i].b;
        cell[2] = evens[i].c;
		newPolys->InsertNextCell(3, cell);

        cell[0] = odds[i].a;
        cell[1] = odds[i].b;
        cell[2] = odds[i].c;
		newPolys->InsertNextCell(3, cell);
	}
	return count;
}
// -----------------------------------------------------------------------------

int sssSuperCylinderSource::tesselateCylEndCap(const int topBottom, int vadder)
{
	double node[3];
	double normal[3];
	double radius1;
	double radius2;
	double theta1;
	double theta2;
	double phi;

    SSS_ABC         abc;
    XYZ2            xyz, xyz2;
    vector<SSS_ABC> odds;
    vector<SSS_ABC> evens;

    xyz2.x =  0.0;
    xyz2.y =  0.0;

	if (topBottom == 1) 
	{
		xyz2.z = 1.0;
		xyz.z  = this->Height;
	}
	else
	{
		xyz2.z = -1.0;
		xyz.z  =  0.0;
	}

	normal[0] = xyz2.x;
	normal[1] = xyz2.y;
	normal[2] = xyz2.z;
	rotateToGlobal(normal, orientPhi, orientTheta);


	if (SuperCylinder == VTK_RCC)
	{
		radius1 = RadiusBottom;
		radius2 = RadiusBottom;
	}
	else if (SuperCylinder == VTK_REC)
	{
		radius1 = RadiusBottom;
		radius2 = RadiusTop;
	}
	else if (SuperCylinder == VTK_TRC)
	{
		if (topBottom == 1) 
		{
			radius1 = RadiusTop;
			radius2 = RadiusTop;
		}
		else
		{
			radius1 = RadiusBottom;
			radius2 = RadiusBottom;
		}
	}

	int lats = max(2, ResHeight);
	int lngs = ResRadius;

    // Set the starting location from center for the 1st ring.
    double lat_start1 = radius1 / this->ResHeight;
    double lat_start2 = radius2 / this->ResHeight;

    double delta_lat1 = (radius1 - lat_start1) / ((double) (lats-1));
    double delta_lat2 = (radius2 - lat_start2) / ((double) (lats-1));
    double delta_lng  = (2.0 * vtkMath::Pi() ) / ((double) lngs);

	int count = 0;

    for (int i=0; i < lats; ++i)
    {
        // Start on the positive X-axis and build counterclockwise.
        theta1 = lat_start1 + ((double) i) * delta_lat1;
        theta2 = lat_start2 + ((double) i) * delta_lat2;

        for (int j=0; j < lngs; ++j)
        {
            phi   = ((double) j * delta_lng);
            xyz.x = theta1 * cos(phi);
            xyz.y = theta2 * sin(phi);

            if (fabs(xyz.x) < 1.0e-10) xyz.x = 0.0;
            if (fabs(xyz.y) < 1.0e-10) xyz.y = 0.0;

			++count;
			node[0] = xyz.x;
			node[1] = xyz.y;
			node[2] = xyz.z;
      		rotateToGlobal(node, orientPhi, orientTheta);

			node[0] += this->Anchor[0];
			node[1] += this->Anchor[1];
			node[2] += this->Anchor[2];

			// Insert node.
			newPoints->InsertNextPoint(node);
			newNormals->InsertNextTuple(normal);
        }
    }               // -- end i-loop

	// Center node.
	node[0] = 0.0;
	node[1] = 0.0;
	node[2] = xyz.z;
	rotateToGlobal(node, orientPhi, orientTheta);

	node[0] += this->Anchor[0];
	node[1] += this->Anchor[1];
	node[2] += this->Anchor[2];
	newPoints->InsertNextPoint(node);
	newNormals->InsertNextTuple(normal);

	odds.clear();
	evens.clear();

	// --------------------------------------
    // Construct the triangle connectivities.
    // Lower left half by row.
    for(int j=0; j < lats-1; ++j)
    {
        int a;
        for(int i=0; i < lngs-1; ++i)
        {
            a     = (lngs * j) + i + vadder;
            abc.a = a;
            abc.b = a + 1;
            abc.c = a + lngs;
            evens.push_back(abc);
        }
        abc.a = a + 1;
        abc.b = abc.a - lngs + 1;
        abc.c = abc.c + 1;
        evens.push_back(abc);
    }

    // Upper right half by row.
    for(int j=0; j < lats-1; ++j)
    {
        int a;
        for(int i=1; i < lngs; ++i)
        {
            a = (lngs * j) + i + vadder;
            abc.a = a;
            abc.b = a + lngs;
            abc.c = a + lngs - 1;
            odds.push_back(abc);
        }
        abc.a = abc.a - lngs + 1;
        abc.c = abc.b;
        abc.b = a + 1;
        odds.push_back(abc);
    }

    // Re-order the vertices into the master vector.
    for(size_t i=0; i < evens.size(); ++i)
    {
        cell[0] = evens[i].a;

		if (topBottom == 1) 
        {
            cell[1] = evens[i].c;
            cell[2] = evens[i].b;
        }
        else
        {
            cell[1] = evens[i].b;
            cell[2] = evens[i].c;
        }

		newPolys->InsertNextCell(3, cell);

        cell[0] = odds[i].a;
		if (topBottom == 1) 
        {
            cell[1] = odds[i].c;
            cell[2] = odds[i].b;
        }
        else
        {
            cell[1] = odds[i].b;
            cell[2] = odds[i].c;
        }

		newPolys->InsertNextCell(3, cell);
	}

    // Build the center.
    int cStart = vadder;
    int cEnd   = cStart + lngs - 1;

	cell[2] = count + vadder;

    for (int j=cStart; j < cEnd; ++j)
    {
        if (topBottom == 1)
        {
            cell[1] = j+1;
            cell[0] = j;
        }
        else
        {
            cell[0] = j+1;
            cell[1] = j;
        }
		newPolys->InsertNextCell(3, cell);
    }


	if (topBottom == 1)
	{
		cell[1] = cEnd;
		cell[0] = cStart;
	}
	else
	{
		cell[0] = cEnd;
		cell[1] = cStart;
	}
	newPolys->InsertNextCell(3, cell);

	++count;

	return count;
}

// -----------------------------------------------------------------------------

void sssSuperCylinderSource::PrintSelf(ostream &os)
{
  // this->Superclass::PrintSelf(os, indent);

  os << "Type        : " << this->SuperCylinder << "\n";
  os << "ResHeight   : " << this->ResHeight << "\n";
  os << "ResRadius   : " << this->ResRadius << "\n";
  os << "Height      : " << this->Height << "\n";
  os << "RadiusBottom: " << this->RadiusBottom << "\n";
  os << "RadiusTop   : " << this->RadiusTop << "\n";
  os << "Anchor      : (" << this->Anchor[0] << ", " << this->Anchor[1] << ", " << this->Anchor[2] << " )\n";
  os << "RMajor      : (" << this->RMajor[0] << ", " << this->RMajor[1] << ", " << this->RMajor[2] << " )\n";
  os << "RMinor      : (" << this->RMinor[0] << ", " << this->RMinor[1] << ", " << this->RMinor[2] << " )\n";
  os << "HVector     : (" << this->HVector[0] << ", " << this->HVector[1] << ", " << this->HVector[2] << " )\n";


  os << "Capping     : " << (this->Capping ? "On\n" : "Off\n");
  os << "Output Points Precision: " << this->OutputPointsPrecision << "\n";
}
