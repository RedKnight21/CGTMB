/*=========================================================================

  Program:   Visualization Toolkit
  Module:    sssSuperCubeSource.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "sssCubeSource.h"
#include "sssGeometryHelpers.h"
#include "cgutility.h"

#include "vtkCellArray.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkStreamingDemandDrivenPipeline.h"

#include <cmath>
#include <vector>

using namespace std;

vtkStandardNewMacro(sssSuperCubeSource);

//------------------------------------------------------------------------------
sssSuperCubeSource::sssSuperCubeSource()
{
  this->Center[0] = 0.5;
  this->Center[1] = 0.5;
  this->Center[2] = 0.5;

  this->Anchor[0] = 0.0;
  this->Anchor[1] = 0.0;
  this->Anchor[2] = 0.0;

  this->A1[0] = 1.0;
  this->A1[1] = 0.0;
  this->A1[2] = 0.0;

  this->A2[0] = 0.0;
  this->A2[1] = 1.0;
  this->A2[2] = 0.0;

  this->A3[0] = 0.0;
  this->A3[1] = 0.0;
  this->A3[2] = 1.0;

  this->A1Dir[0] = 1.0;
  this->A1Dir[1] = 0.0;
  this->A1Dir[2] = 0.0;

  this->A2Dir[0] = 0.0;
  this->A2Dir[1] = 1.0;
  this->A2Dir[2] = 0.0;

  this->A3Dir[0] = 0.0;
  this->A3Dir[1] = 0.0;
  this->A3Dir[2] = 1.0;

  this->ResA1 = 10;
  this->ResA2 = 10;
  this->ResA3 = 10;
  this->ResA4 = 10;

  this->A1Length = 1.0;
  this->A2Length = 1.0;
  this->A3Length = 1.0;
  this->A4Length = 1.0;

  this->SuperCube = VTK_RPP;

  this->OutputPointsPrecision = DOUBLE_PRECISION;

  this->SetNumberOfInputPorts(0);
}

//------------------------------------------------------------------------------
int sssSuperCubeSource::RequestData(vtkInformation       *vtkNotUsed(request),
                                    vtkInformationVector **vtkNotUsed(inputVector), 
                                    vtkInformationVector *outputVector)
{
  // get the info object
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // get the output
  vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  int i, j, k;

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


  setupDirectionLengths();

  int    num = 0;
  int    vadder = 0;



  if (SuperCube == VTK_WEDGE)
  {
    int  s4[]   = {0,4,6};        		// mapping to box sides

    num     = tesselateTrianglePlane(0, vadder);
	vadder += num;

    num     = tesselateTrianglePlane(1, vadder);
	vadder += num;

    for (int i=0; i<3; ++i)
    {
        num     = tesselateGeneralPlane(s4[i], vadder);
		vadder += num;
    }
  }
  else
  {
    for (int i=0; i<6; ++i)
    {
        num     = tesselateGeneralPlane(i, vadder);
		vadder += num;
    }
  }

  // ---------------------------------------------
  // Update ourselves and release memory
  //
  output->SetPoints(newPoints);
  newPoints->Delete();

  output->GetPointData()->SetNormals(newNormals);
  newNormals->Delete();

  // since we've estimated size; reclaim some space.
  newPolys->Squeeze();
  output->SetPolys(newPolys);
  newPolys->Delete();

  return 1;

}



//------------------------------------------------------------------------------
void sssSuperCubeSource::setupDirectionLengths()
{
	double x;
	double norm;

	if ((norm = vtkMath::Norm(this->A1)) == 0.0)
	{
          norm = 1.0;
	}
	x = this->A1[0] * this->A1[0] + this->A1[1] * this->A1[1] + this->A1[2] * this->A1[2];
	A1Length = sqrt(x);

	this->A1Dir[0] = this->A1[0] / norm;
	this->A1Dir[1] = this->A1[1] / norm;
	this->A1Dir[2] = this->A1[2] / norm;

	if ((norm = vtkMath::Norm(this->A2)) == 0.0)
	{
          norm = 1.0;
	}
	x = this->A2[0] * this->A2[0] + this->A2[1] * this->A2[1] + this->A2[2] * this->A2[2];
	A2Length = sqrt(x);

	this->A2Dir[0] = this->A2[0] / norm;
	this->A2Dir[1] = this->A2[1] / norm;
	this->A2Dir[2] = this->A2[2] / norm;

	if ((norm = vtkMath::Norm(this->A3)) == 0.0)
	{
          norm = 1.0;
	}
	x = this->A3[0] * this->A3[0] + this->A3[1] * this->A3[1] + this->A3[2] * this->A3[2];
	A3Length = sqrt(x);

	this->A3Dir[0] = this->A3[0] / norm;
	this->A3Dir[1] = this->A3[1] / norm;
	this->A3Dir[2] = this->A3[2] / norm;

	// -------------------------------------
	// For wedge side if present.
	this->A4[0] = this->A1[0] - this->A3[0];
	this->A4[1] = this->A1[1] - this->A3[1];
	this->A4[2] = this->A1[2] - this->A3[2];

	if ((norm = vtkMath::Norm(this->A4)) == 0.0)
	{
          norm = 1.0;
	}
	x = this->A4[0] * this->A4[0] + this->A4[1] * this->A4[1] + this->A4[2] * this->A4[2];
	A4Length = sqrt(x);

	this->A4Dir[0] = this->A4[0] / norm;
	this->A4Dir[1] = this->A4[1] / norm;
	this->A4Dir[2] = this->A4[2] / norm;

	double angle, a[3];
    angle = crossProduct(this->A4Dir, this->A2Dir, &a[0]);

	if ((norm = vtkMath::Norm(a)) == 0.0)
	{
          norm = 1.0;
	}

	this->A4Nor[0] = a[0] / norm;
	this->A4Nor[1] = a[1] / norm;
	this->A4Nor[2] = a[2] / norm;
}

//------------------------------------------------------------------------------

int sssSuperCubeSource::tesselateGeneralPlane(const int side, const int vadder)
{
	double anchor[3];
	double normal[3];
    SSS_ABC         abc;
    vector<SSS_ABC> odds;
    vector<SSS_ABC> evens;

    double dist1, dist2, dirv1[3], dirv2[3];
    bool reverse = false;
	int  n1, n2;
	int  count = 0;

	// Setup the build parameters.
    switch(side)
    {
        case (2):                                       // Box Side 1258 -- Wed Side 1465

            anchor[0] = this->Anchor[0];
            anchor[1] = this->Anchor[1];
            anchor[2] = this->Anchor[2];
            dist1     = A1Length;						// Length A1
            dist2     = A3Length;						// Length A3
            dirv1[0]  =  this->A1Dir[0];				// A1 direction
            dirv1[1]  =  this->A1Dir[1];
            dirv1[2]  =  this->A1Dir[2];
            dirv2[0]  =  this->A3Dir[0];				// A3 direction
            dirv2[1]  =  this->A3Dir[1];
            dirv2[2]  =  this->A3Dir[2];
			normal[0] = -this->A2Dir[0];
			normal[1] = -this->A2Dir[1];
			normal[2] = -this->A2Dir[2];
            reverse   = false;
			n1        = this->ResA1;
			n2        = this->ResA3;
            break;
        case(1):                                        // Box Side 2365

            anchor[0] = this->Anchor[0] + this->A1[0];
            anchor[1] = this->Anchor[1] + this->A1[1];
            anchor[2] = this->Anchor[2] + this->A1[2];
            dist1     = A2Length;						// Length A2
            dist2     = A3Length;						// Length A3
            dirv1[0]  = this->A2Dir[0];					// A2 direction
            dirv1[1]  = this->A2Dir[1];
            dirv1[2]  = this->A2Dir[2];
            dirv2[0]  = this->A3Dir[0];					// A3 direction
            dirv2[1]  = this->A3Dir[1];
            dirv2[2]  = this->A3Dir[2];
			normal[0] = this->A1Dir[0];
			normal[1] = this->A1Dir[1];
			normal[2] = this->A1Dir[2];
            reverse   = false;
			n1        = this->ResA2;
			n2        = this->ResA3;
            break;
        case(3):                                        // Box Side 4367

            anchor[0] = this->Anchor[0] + this->A2[0];
            anchor[1] = this->Anchor[1] + this->A2[1];
            anchor[2] = this->Anchor[2] + this->A2[2];
            dist1     = A1Length;						// Length A1
            dist2     = A3Length;						// Length A3
            dirv1[0]  = this->A1Dir[0];					// A1 direction
            dirv1[1]  = this->A1Dir[1];
            dirv1[2]  = this->A1Dir[2];
            dirv2[0]  = this->A3Dir[0];					// A3 direction
            dirv2[1]  = this->A3Dir[1];
            dirv2[2]  = this->A3Dir[2];
			normal[0] = this->A2Dir[0];
			normal[1] = this->A2Dir[1];
			normal[2] = this->A2Dir[2];
            reverse   = true;
			n1        = this->ResA1;
			n2        = this->ResA3;
            break;
        case(0):                                        // Box Side 1478

            anchor[0] = this->Anchor[0];
            anchor[1] = this->Anchor[1];
            anchor[2] = this->Anchor[2];
            dist1     = A2Length;						// Length A2
            dist2     = A3Length;						// Length A3
            dirv1[0]  =  this->A2Dir[0];				// A2 direction
            dirv1[1]  =  this->A2Dir[1];
            dirv1[2]  =  this->A2Dir[2];
            dirv2[0]  =  this->A3Dir[0];				// A3 direction
            dirv2[1]  =  this->A3Dir[1];
            dirv2[2]  =  this->A3Dir[2];
			normal[0] = -this->A1Dir[0];
			normal[1] = -this->A1Dir[1];
			normal[2] = -this->A1Dir[2];
            reverse   = true;
			n1        = this->ResA2;
			n2        = this->ResA3;
            break;
        case(4):                                        // Box Side 1234 -- Wed Side 1234

            anchor[0] = this->Anchor[0];
            anchor[1] = this->Anchor[1];
            anchor[2] = this->Anchor[2];
            dist1     = A1Length;						// Length A1
            dist2     = A2Length;						// Length A2
            dirv1[0]  =  this->A1Dir[0];				// A1 direction
            dirv1[1]  =  this->A1Dir[1];
            dirv1[2]  =  this->A1Dir[2];
            dirv2[0]  =  this->A2Dir[0];				// A2 direction
            dirv2[1]  =  this->A2Dir[1];
            dirv2[2]  =  this->A2Dir[2];
			normal[0] = -this->A3Dir[0];
			normal[1] = -this->A3Dir[1];
			normal[2] = -this->A3Dir[2];
            reverse   = true;
			n1        = this->ResA1;
			n2        = this->ResA2;
            break;
        case(5):                                        // Box Side 8567

            anchor[0] = this->Anchor[0] + this->A3[0];
            anchor[1] = this->Anchor[1] + this->A3[1];
            anchor[2] = this->Anchor[2] + this->A3[2];
            dist1     = A1Length;						// Length A1
            dist2     = A2Length;						// Length A2
            dirv1[0]  = this->A1Dir[0];					// A1 direction
            dirv1[1]  = this->A1Dir[1];
            dirv1[2]  = this->A1Dir[2];
            dirv2[0]  = this->A2Dir[0];					// A2 direction
            dirv2[1]  = this->A2Dir[1];
            dirv2[2]  = this->A2Dir[2];
			normal[0] = this->A3Dir[0];
			normal[1] = this->A3Dir[1];
			normal[2] = this->A3Dir[2];
            reverse   = false;
			n1        = this->ResA1;
			n2        = this->ResA2;
            break;
        case (6):                                       // Wed Side 5236
            anchor[0] = this->Anchor[0] + this->A3[0];
            anchor[1] = this->Anchor[1] + this->A3[1];
            anchor[2] = this->Anchor[2] + this->A3[2];
            dist1     = A4Length;						// Length A4
            dist2     = A2Length;						// Length A2
            dirv1[0]  = this->A4Dir[0];					// A4 direction
            dirv1[1]  = this->A4Dir[1];
            dirv1[2]  = this->A4Dir[2];
            dirv2[0]  = this->A2Dir[0];					// A2 direction
            dirv2[1]  = this->A2Dir[1];
            dirv2[2]  = this->A2Dir[2];
			normal[0] = this->A4Nor[0];
			normal[1] = this->A4Nor[1];
			normal[2] = this->A4Nor[2];
            reverse   = false;
			n1        = this->ResA4;
			n2        = this->ResA2;
            break;
    }

    double d1 = dist1 / ((double) n1);
    double d2 = dist2 / ((double) n2);
    ++n1;
    ++n2;
	double a[3];
	double node[3];

    // Build in place along direction vectors.
    for(int j=0; j<n2; ++j)
    {
        a[0] = anchor[0] + ((double) j) * d2 * dirv2[0];
        a[1] = anchor[1] + ((double) j) * d2 * dirv2[1];
        a[2] = anchor[2] + ((double) j) * d2 * dirv2[2];

        for(int i=0; i<n1; ++i)
        {
			++count;
            node[0] = a[0] + ((double) i) * d1 * dirv1[0];
            node[1] = a[1] + ((double) i) * d1 * dirv1[1];
            node[2] = a[2] + ((double) i) * d1 * dirv1[2];

			// Insert node.
			newPoints->InsertNextPoint(node);
			newNormals->InsertNextTuple(normal);

        }
    }           // -- end j-loop

    // ------------------------------------------
    // Construct the triangle connectivities.
    // Lower left half by row.

	odds.clear();
	evens.clear();

    for(int j=0; j < n2-1; ++j)
    {
        for(int i=0; i < n1-1; ++i)
        {
            int a = (n1 * j) + i + vadder;
            abc.a = a;
            if (reverse)
            {
                abc.c = a + 1;
                abc.b = a + n1;
            }
            else
            {
                abc.b = a + 1;
                abc.c = a + n1;
            }
            evens.push_back(abc);
        }
    }

    // Upper right half by row.
    for(int j=0; j < n2-1; ++j)
    {
        for(int i=1; i < n1; ++i)
        {
            int a = (n1 * j) + i + vadder;
            abc.a = a;
            if (reverse)
            {
                abc.c = a + n1;
                abc.b = a + n1 - 1;
            }
            else
            {
                abc.b = a + n1;
                abc.c = a + n1 - 1;
            }
            odds.push_back(abc);
        }
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

//------------------------------------------------------------------------------

int sssSuperCubeSource::tesselateTrianglePlane(const int side, const int vadder)
{
	double anchor[3];
	double normal[3];
	double anode[3];
	double bnode[3];
	double cnode[3];

    XYZ2            tnode1, tnode2;
    SSS_ABC         abc;
    vector<SSS_ABC> odds;
    vector<SSS_ABC> evens;
    double dist3, dirv3[3];
	double dirv1[3], dirv2[3];

	anchor[0] = this->Anchor[0] + this->A3[0];
	anchor[1] = this->Anchor[1] + this->A3[1];
	anchor[2] = this->Anchor[2] + this->A3[2];

	// Direction vector along diagonal surface.
	dirv3[0]  = this->A4Dir[0];					// A4 direction
	dirv3[1]  = this->A4Dir[1];
	dirv3[2]  = this->A4Dir[2];

	dist3 = this->A4Length;

	dirv1[0]  =  this->A1Dir[0];				// A1 direction
	dirv1[1]  =  this->A1Dir[1];
	dirv1[2]  =  this->A1Dir[2];
	dirv2[0]  =  this->A3Dir[0];				// A3 direction
	dirv2[1]  =  this->A3Dir[1];
	dirv2[2]  =  this->A3Dir[2];

    bool reverse = false;
    if (side == 1)
    {
        anchor[0] += this->A2[0];
        anchor[1] += this->A2[1];
        anchor[2] += this->A2[2];
        normal[0]  = this->A2[0];
        normal[1]  = this->A2[1];
        normal[2]  = this->A2[2];
        reverse    = true;
    }
	else
	{
        normal[0]  = -this->A2[0];
        normal[1]  = -this->A2[1];
        normal[2]  = -this->A2[2];
	}

    int n1, n2;
	int n3 = this->ResA4;
	double d3 = this->A4Length / ((double) n3);
	++n3;


    // Find the points along the diagonal surface.
    vector<XYZ2> p3;
	p3.clear();
    for(int j=0; j<n3; ++j)
    {
        tnode2.x = anchor[0] + ((double) j) * d3 * dirv3[0];
        tnode2.y = anchor[1] + ((double) j) * d3 * dirv3[1];
        tnode2.z = anchor[2] + ((double) j) * d3 * dirv3[2];
        p3.push_back(tnode2);
    }

	anchor[0] = this->Anchor[0];
	anchor[1] = this->Anchor[1];
	anchor[2] = this->Anchor[2];

    if (side == 1)
    {
        anchor[0] += this->A2[0];
        anchor[1] += this->A2[1];
        anchor[2] += this->A2[2];
    }

    vector<XYZ2> p1;
	p1.clear();

    for (size_t n=0; n < p3.size(); ++n)
    {
		anode[0] = p3[n].x;
		anode[1] = p3[n].y;
		anode[2] = p3[n].z;

        bool yes = linesIntersect(anode, dirv2, anchor, dirv1, bnode);
        if (yes)
		{
			tnode1.x = bnode[0];
			tnode1.y = bnode[1];
			tnode1.z = bnode[2];

            p1.push_back( tnode1 );
		}
    }

    dirv1[0] *= -1.0;  dirv1[1] *= -1.0;  dirv1[2] *= -1.0;
    dirv2[0] *= -1.0;  dirv2[1] *= -1.0;  dirv2[2] *= -1.0;

    vector<XYZ2> p2;
	p2.clear();

    for (size_t n=0; n < p3.size(); ++n)
    {
		anode[0] = p3[n].x;
		anode[1] = p3[n].y;
		anode[2] = p3[n].z;

        bool yes = linesIntersect(anode, dirv1, anchor, dirv2, bnode);
        if (yes)
		{
			tnode1.x = bnode[0];
			tnode1.y = bnode[1];
			tnode1.z = bnode[2];

            p2.push_back( tnode1 );
		}
    }

    dirv1[0] *= -1.0;  dirv1[1] *= -1.0;  dirv1[2] *= -1.0;

	int count = 0;

	for (size_t n = 0; n < p1.size(); ++n)
	{
		++count;
		anode[0] = p1[n].x;
		anode[1] = p1[n].y;
		anode[2] = p1[n].z;

		// Insert node.
		newPoints->InsertNextPoint(anode);
		newNormals->InsertNextTuple(normal);
	} 	

    int num = (int) p3.size() - 1;

    for (size_t n= p2.size() - 2; n > 0; --n)
    {
        for (int c=0; c<num; ++c)
        {
			anode[0] = p1[c].x;
			anode[1] = p1[c].y;
			anode[2] = p1[c].z;

			bnode[0] = p2[n].x;
			bnode[1] = p2[n].y;
			bnode[2] = p2[n].z;

            bool yes = linesIntersect(anode, dirv2, bnode, dirv1, cnode);
			if (yes)
			{
				++count;

				// Insert node.
				newPoints->InsertNextPoint(cnode);
				newNormals->InsertNextTuple(normal);
			}
        }
        --num;
    }

	cnode[0] = p3[0].x;
	cnode[1] = p3[0].y;
	cnode[2] = p3[0].z;

	++count;
	newPoints->InsertNextPoint(cnode);
	newNormals->InsertNextTuple(normal);


    // ------------------------------------------
    // Construct the triangle connectivities.
    // Lower left half by row.
    n2 = 0;
    n1 = 0;
    for(int j=0; j < n3-1; ++j)
    {
        n2 = (n3-j);
        for(int i=0; i < n3-j-1; ++i)
        {
            int a = n1 + i + vadder;
            abc.a = a;
            if (reverse)
            {
                abc.c = a + 1;
                abc.b = a + n2;
            }
            else
            {
                abc.b = a + 1;
                abc.c = a + n2;
            }
            evens.push_back(abc);
        }
        n1 += n3 - j;
    }

    // Upper right half by row.
    n1 = 0;
    for(int j=0; j < n3-1; ++j)
    {
        n2 = (n3-j);
        for(int i=1; i < n3-j-1; ++i)
        {
            int a = n1 + i + vadder;
            abc.a = a;
            if (reverse)
            {
                abc.c = a + n2;
                abc.b = a + n2 - 1;
            }
            else
            {
                abc.b = a + n2;
                abc.c = a + n2 - 1;
            }
            odds.push_back(abc);
        }
        n1 += n3 - j;
    }

    --n3;
    n1 = 0;
    n2 = 0;

    // Re-order the vertices into the master vector.
    for(int n=0; n < n3; ++n)
    {
        for(int k=0; k < n3-n-1 ; ++k)
        {
            cell[0] = evens[n1].a;
            cell[1] = evens[n1].b;
            cell[2] = evens[n1].c;
			newPolys->InsertNextCell(3, cell);

            cell[0] = odds[n2].a;
            cell[1] = odds[n2].b;
            cell[2] = odds[n2].c;
			newPolys->InsertNextCell(3, cell);

            ++n1;
            ++n2;
        }
        cell[0] = evens[n1].a;
        cell[1] = evens[n1].b;
        cell[2] = evens[n1].c;
		newPolys->InsertNextCell(3, cell);
        ++n1;
    }

	return count;
}


//------------------------------------------------------------------------------
// Convenience method allows creation of cube by specifying bounding box.
void sssSuperCubeSource::SetBounds(double xMin, double xMax, double yMin, double yMax, double zMin, double zMax)
{
  double bounds[6];
  bounds[0] = xMin;
  bounds[1] = xMax;
  bounds[2] = yMin;
  bounds[3] = yMax;
  bounds[4] = zMin;
  bounds[5] = zMax;
  this->SetBounds(bounds);
}

//------------------------------------------------------------------------------
void sssSuperCubeSource::SetBounds(const double bounds[6])
{
  this->SetA1(bounds[1] - bounds[0], 0.0, 0.0);
  this->SetA2(0.0, bounds[3] - bounds[2], 0.0);
  this->SetA3(0.0, 0.0, bounds[5] - bounds[4]);

  this->SetCenter(
    (bounds[1] + bounds[0]) / 2.0, (bounds[3] + bounds[2]) / 2.0, (bounds[5] + bounds[4]) / 2.0);

  this->SetAnchor(bounds[0], bounds[2], bounds[4]);
}

//------------------------------------------------------------------------------
void sssSuperCubeSource::GetBounds(double bounds[6])
{
  bounds[0] = this->Center[0] - (this->A1Length / 2.0);
  bounds[1] = this->Center[0] + (this->A1Length / 2.0);
  bounds[2] = this->Center[1] - (this->A2Length / 2.0);
  bounds[3] = this->Center[1] + (this->A2Length / 2.0);
  bounds[4] = this->Center[2] - (this->A3Length / 2.0);
  bounds[5] = this->Center[2] + (this->A3Length / 2.0);
}

//------------------------------------------------------------------------------
void sssSuperCubeSource::PrintSelf(ostream& os)
{
  // this->Superclass::PrintSelf(os, indent);

  os << "Output Points Precision: " << this->OutputPointsPrecision << "\n";
  os << "X Length: " << this->A1Length << "\n";
  os << "Y Length: " << this->A2Length << "\n";
  os << "Z Length: " << this->A3Length << "\n";
  os << "Diagonal: " << this->A4Length << "\n";

  os << "Anchor  : (" << this->Anchor[0] << ", " << this->Anchor[1] << ", " << this->Anchor[2] << ")\n";
  os << "A1      : (" << this->A1[0] << ", " << this->A1[1] << ", " << this->A1[2] << ")\n";
  os << "A2      : (" << this->A2[0] << ", " << this->A2[1] << ", " << this->A2[2] << ")\n";
  os << "A3      : (" << this->A3[0] << ", " << this->A3[1] << ", " << this->A3[2] << ")\n";
  if (this->SuperCube == VTK_WEDGE)  
  	os << "A4      : (" << this->A4[0] << ", " << this->A4[1] << ", " << this->A4[2] << ")\n";

  os << "ResA1   : (" << this->ResA1 << ")\n";
  os << "ResA2   : (" << this->ResA2 << ")\n";
  os << "ResA3   : (" << this->ResA3 << ")\n";
  if (this->SuperCube == VTK_WEDGE)  os << "ResA4   : (" << this->ResA4 << ")\n";


}
