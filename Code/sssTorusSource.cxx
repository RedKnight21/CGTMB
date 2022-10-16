/*=========================================================================

  Program:   Visualization Toolkit
  Module:    sssTorusSource.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "sssTorusSource.h"
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
#include "vtkStreamingDemandDrivenPipeline.h"

#include <cmath>

using namespace std;

vtkStandardNewMacro(sssTorusSource);

//------------------------------------------------------------------------------
// Construct torus with radius=2.0 and circular cross section of radius 0.5
// Normals are generated.
sssTorusSource::sssTorusSource()
{
  this->Radius     = 2.0;
  this->HRadius    = 0.5;
  this->PRadius    = 0.5;
  this->Center[0]  = 0.0;
  this->Center[1]  = 0.0;
  this->Center[2]  = 0.0;
  this->HVector[0] = 0.0;
  this->HVector[1] = 0.0;
  this->HVector[2] = 1.0;

  this->SegmentsOnSurface   = 10;
  this->SegmentsAboutAxis   = 10;

  this->OutputPointsPrecision = vtkAlgorithm::SINGLE_PRECISION;

  this->GenerateNormals = true;

  this->SetNumberOfInputPorts(0);
}

//------------------------------------------------------------------------------
int sssTorusSource::RequestData(vtkInformation *vtkNotUsed(request),
  vtkInformationVector **vtkNotUsed(inputVector), vtkInformationVector *outputVector)
{
  // get the info object
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // get the output
  vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  double a2, c2;

  // ---------------------------------------------------------------------------
  double x = this->HVector[0];
  double y = this->HVector[1];
  double z = this->HVector[2];

  c2 = x*x + y*y + z*z;
  a2 = sqrt(c2);

  this->HVector[0] = this->HVector[0] / a2;
  this->HVector[1] = this->HVector[1] / a2;
  this->HVector[2] = this->HVector[2] / a2;

  // ---------------------------------------------------------------------------
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

  tesselateTorus(this->SegmentsAboutAxis, this->SegmentsOnSurface);

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

//------------------------------------------------------------------------------
void sssTorusSource::tesselateTorus(const int lats, const int lngs)
{
    // lats -- number of segments of torus about H.
    // lngs -- number of segments about Rh X Rp cross-product vector

	// Build with H-vector parallel along the Z-axis.

    double pi = acos(-1.0);
    int i, j;
    double theta, phi, phi2;

	double node[3];
	double normal[3];

    vector<XYZ2> t_nodes;
    vector<XYZ2> t_normals;

    SSS_ABC  abc;
    vector<SSS_ABC> odds;
    vector<SSS_ABC> evens;

    vector<int> v;
    v.reserve(3);
    v.push_back(0);
    v.push_back(0);
    v.push_back(0);

    XYZ2 buildCenter;
    XYZ2 xyz;
    XYZ2 xyz2;
    XYZ2 uvw;
    vector< vector<XYZ2>> points;
    vector< vector<XYZ2>> norms;

    points.resize(lats, vector<XYZ2>(lngs));
    norms.resize(lats, vector<XYZ2>(lngs));

    buildCenter.x = this->Radius;
    buildCenter.y = 0.0;
    buildCenter.z = 0.0;

    double delta_lng = (2.0 * pi) / ((double) lngs);
    double lng_start = -1.5;
    double cos_theta;
    double sin_theta;
    double cos_phi;
    double sin_phi;
    double rh = this->HRadius;
    double rp = this->PRadius;
    double d;

    // Calculate the 1st set of points on the cross section
	// for the starting segment.
    xyz.y = 0.0;
    uvw.y = 0.0;
    for (j=0; j < lngs; ++j)
    {
        cos_theta = ((double) j * delta_lng);
        xyz.x = rp * cos(cos_theta);
        xyz.z = rh * sin(cos_theta);
        if (fabs(xyz.x) < 1.0e-10) xyz.x = 0.0;
        if (fabs(xyz.z) < 1.0e-10) xyz.z = 0.0;

        uvw.x = xyz.x;
        uvw.z = xyz.z;

        xyz.x += buildCenter.x;
        points[0][j] = xyz;

        t_nodes.push_back(xyz);

        d = uvw.x * uvw.x + uvw.z * uvw.z;
        d = sqrt(d);
        uvw.x = uvw.x / d;
        uvw.z = uvw.z / d;
        norms[0][j] = uvw;

        t_normals.push_back(uvw);
    }

	// Find the segment "length" in radians.
    double delta_lat = (2.0 * pi) / ((double) lats);

	// Create the other cross-section points for the other segments.
    for (i=1; i < lats; ++i)
    {
        phi = ((double) i * delta_lat);
        sin_phi = sin(phi);
        cos_phi = cos(phi);
        if (fabs(sin_phi) < 1.0E-10) sin_phi = 0.0;
        if (fabs(cos_phi) < 1.0E-10) cos_phi = 0.0;

        for (j=0; j < lngs; ++j)
        {
            xyz = points[0][j];
            // Rotate about Z-axis.
            // Because xyz.y is always 0, this is simplified.
            // xyz2.z does not change.
            xyz2.x = ( xyz.x) * cos_phi;
            xyz2.y = ( xyz.x) * sin_phi;
            xyz2.z = xyz.z;
            points[i][j] = xyz2;

            t_nodes.push_back(xyz2);

            xyz = norms[0][j];
            // Rotate about Z-axis.
            // Because xyz.y is always 0, this is simplified.
            // xyz2.z does not change.
            xyz2.x = ( xyz.x) * cos_phi;
            xyz2.y = ( xyz.x) * sin_phi;
            xyz2.z = xyz.z;
            norms[i][j] = xyz2;

            t_normals.push_back(xyz2);
        }
    }

	xyz.x = this->HVector[0];
	xyz.y = this->HVector[1];
	xyz.z = this->HVector[2];

	// Rotate the torus and translate into position.

    if (xyz.z < 1.0)
    {
        cos_theta = xyz.z;
        sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        sin_phi = xyz.y / sin_theta;
        cos_phi = xyz.x / sin_theta;

        theta = acos(cos_theta);
        phi   = asin(sin_phi);
        phi2  = acos(cos_phi);
        if (std:: isnan(phi)) phi = phi2;
        for (int i=0; i < (int) t_nodes.size(); ++i)
        {
            xyz     = t_nodes[i];
			node[0] = xyz.x;
			node[1] = xyz.y;
			node[2] = xyz.z;

            rotateToGlobal(node, phi, theta);

            // Translate
            node[0] += this->Center[0];
            node[1] += this->Center[1];
            node[2] += this->Center[2];

            xyz       = t_normals[i];
			normal[0] = xyz.x;
			normal[1] = xyz.y;
			normal[2] = xyz.z;

            rotateToGlobal(normal, phi, theta);

			// Insert node.
			newPoints->InsertNextPoint(node);
			newNormals->InsertNextTuple(normal);
        }
    }
    else
    {
        // No rotation needed.
        for (int i=0; i < (int) t_nodes.size(); ++i)
        {
            xyz     = t_nodes[i];
			node[0] = xyz.x;
			node[1] = xyz.y;
			node[2] = xyz.z;

            // Translate
            node[0] += this->Center[0];
            node[1] += this->Center[1];
            node[2] += this->Center[2];

            xyz       = t_normals[i];
			normal[0] = xyz.x;
			normal[1] = xyz.y;
			normal[2] = xyz.z;

			// Insert node.
			newPoints->InsertNextPoint(node);
			newNormals->InsertNextTuple(normal);
        }
    }

	// Set the even connectivities.
    int b;

    for(int j=0; j < lats-1; ++j)
    {
        b = lngs * j;

        for(int i=0; i < lngs-1; ++i)
        {
            int a = b + i;
            abc.a = a;
            abc.c = a + 1;
            abc.b = a + lngs;
            evens.push_back(abc);
        }
        abc.c = b;
        abc.a = b + lngs - 1;
        abc.b = abc.a + lngs;
        evens.push_back(abc);
    }

    b = lngs * (lats - 1);

    for(int i=0; i < lngs-1; ++i)
    {
        abc.a = b + i;
        abc.c = abc.a + 1;
        abc.b = i;
        evens.push_back(abc);
    }

    abc.b = lngs - 1;
    abc.c = b;
    abc.a = b + lngs - 1;
    evens.push_back(abc);


	// Set the odd connectivities
    for(int j=0; j < lats-1; ++j)
    {
        b = lngs * j;

        for(int i=0; i < lngs-1; ++i)
        {
            int a = b + i;
            abc.c = a + 1;
            abc.a = a + lngs;
            abc.b = abc.a + 1;
            odds.push_back(abc);
        }
        abc.c = b;
        abc.b = b + lngs;
        abc.a = abc.b + lngs - 1;
        odds.push_back(abc);
    }

    b = lngs * (lats - 1) + 1;

    for(int i=0; i < lngs-1; ++i)
    {
        abc.b = i;
        abc.c = abc.b + 1;
        abc.a = b + i;
        odds.push_back(abc);
    }

    abc.a = b - 1;
    abc.c = 0;
    abc.b = lngs - 1;
    odds.push_back(abc);

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
}
//------------------------------------------------------------------------------
void sssTorusSource::PrintSelf(ostream &os)
{
  //this->Superclass::PrintSelf(os, indent);

  os << "\nTorus: \n";
  os << "Center              : (" << this->Center[0]  << ", " << this->Center[1]  << ", " << this->Center[2]  << ")\n";
  os << "H Vector            : (" << this->HVector[0] << ", " << this->HVector[1] << ", " << this->HVector[2] << ")\n";
  os << "Radius              : "  << this->Radius            << "\n";
  os << "H Radius            : "  << this->HRadius           << "\n";
  os << "P Radius            : "  << this->PRadius           << "\n";
  os << "Segments About Axis : "  << this->SegmentsAboutAxis << "\n";
  os << "Segments On Surface : "  << this->SegmentsOnSurface << "\n";
}
//------------------------------------------------------------------------------
int sssTorusSource::RequestInformation(vtkInformation *vtkNotUsed(request),
  vtkInformationVector **vtkNotUsed(inputVector), vtkInformationVector *outputVector)
{
  // get the info object
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  outInfo->Set(CAN_HANDLE_PIECE_REQUEST(), 1);

  return 1;
}
