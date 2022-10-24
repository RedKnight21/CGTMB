#include "unstructuredmesh.h"

const int ABaqus_part::tetFaces[][3]   = {{0, 2, 1},  {0, 1, 3},  {1, 2, 3},  {2, 0, 3}};
const int ABaqus_part::hexFaces[][4]   = {{0, 3, 2, 1},  {4, 5, 6, 7},  {0, 1, 5, 4},  {1, 2, 6, 5},  {2, 3, 7, 6},  {3, 0, 4, 7}};
const int ABaqus_part::pentFaces[][4]  = {{0, 2, 1, 0},  {3, 4, 5, 0},  {0, 1, 4, 3},  {1, 2, 5, 4},  {2, 0, 3, 5}};

void ABaqus_part::clearIt()
{
    nodes.clear();
    t1.clear();
    p1.clear();
    h1.clear();
    t2.clear();
    p2.clear();
    h2.clear();
    for(int i=0; i<6; ++i) elNumbers[i] = 0;
    minElNo = INT_MAX;
    maxElNo = -1;
}
// ----------------------------------------------------------------------------
void ABaqus_part::findSurfaceElements()
{
    // Routine to find all elements on the part's surface.  Handles mixed element types.
    // In the surfaceEls array, store coded element number and face number for later use.

    int k;
    int e1, e2;
    std::vector<int> t;
    t.push_back(0);
    t.push_back(0);
    t.push_back(0);
    t.push_back(0);
    t.push_back(0);
    t.push_back(0);

    std::vector<int> t2;
    t2.push_back(0);
    t2.push_back(0);

    int hsize = (int) h1.size();
    int tsize = (int) t1.size();
    int psize = (int) p1.size();
    int tpsiz = psize + tsize;
    int total = tsize + psize + hsize;


#ifdef BUILD_OPENMP

    int hThreads = 1;
    int pThreads = 1;
    int tThreads = 1;

    int nt = omp_get_max_threads();
    printf("\nPART:: findNeighbors:: max threads: %d\n", nt);

    hThreads = hsize / nt + 1;
    hThreads = min(hThreads, nt);

    pThreads = psize / nt + 1;
    pThreads = min(pThreads, nt);

    tThreads = tsize / nt + 1;
    tThreads = min(tThreads, nt);

#endif


    // Define the faces for each element in the part.
    // 1st order hexs.
#ifdef BUILD_OPENMP
    #pragma omp parallel for num_threads(hThreads) private(k)
#endif
    for(int h=0; h<hsize; ++h)
    {
        // Process each of the 6 faces for this hex.
        for(int f=0; f<6; ++f)
        {
            for(int s=0; s<4; ++s)
            {
                // Store the 4 nodes numbers for this face.
                k                 = hexFaces[f][s];
                h1[h].faces[f][s] = h1[h].connect[k];
            }
            // Sort into ascending order.
            std::sort(h1[h].faces[f], h1[h].faces[f]+4);
        }
    }				// -- end i-loop over hex elements

    // 1st order pents.
#ifdef BUILD_OPENMP
    #pragma omp parallel for num_threads(pThreads) private(k)
#endif
    for(int p=0; p<psize; ++p)
    {
        // Process each of the 5 faces for this pent.
        for(int f=0; f<2; ++f)
        {
            for(int s=0; s<3; ++s)
            {
                // Store the 3 nodes numbers for this face.
                k                 = pentFaces[f][s];
                p1[p].faces[f][s] = p1[p].connect[k];
            }
            // Sort into ascending order.
            std::sort(p1[p].faces[f], p1[p].faces[f]+3);
        }

        for(int f=2; f<5; ++f)
        {
            for(int s=0; s<4; ++s)
            {
                // Store the 4 nodes numbers for this face.
                k                 = pentFaces[f][s];
                p1[p].faces[f][s] = p1[p].connect[k];
            }
            // Sort into ascending order.
            std::sort(p1[p].faces[f], p1[p].faces[f]+4);
        }
    }				// -- end i-loop over pent elements

    // 1st order tets.
#ifdef BUILD_OPENMP
    #pragma omp parallel for num_threads(tThreads) private(k)
#endif
    for(int h=0; h<tsize; ++h)
    {
        // Process each of the 4 faces for this tet.
        for(int f=0; f<4; ++f)
        {
            for(int s=0; s<3; ++s)
            {
                // Store the 3 nodes numbers for this face.
                k                 = tetFaces[f][s];
                t1[h].faces[f][s] = t1[h].connect[k];
            }
            // Sort into ascending order.
            std::sort(t1[h].faces[f], t1[h].faces[f]+3);
        }
    }				// -- end i-loop over tet elements

    // ---------------------------------------------------------------
    // Setup scratch array for this part.
    std::vector< std::vector<int> > scratch;
    for(int h=0; h < total; ++h)  scratch.push_back( t );

    // ---------------------------------------------------------------
    // Search only hexs.
    // Find which elements share a face.
#ifdef BUILD_OPENMP
    #pragma omp parallel for num_threads(hThreads) private(e1, e2)
#endif
    for(int ho=0; ho<hsize; ++ho)
    {
        for(int hi=ho+1; hi<hsize; ++hi)
        {
            for(int n=0; n<6; ++n)
            {
                for(int m=0; m<6; ++m)
                {
                    if (h1[ho].faces[n][0] == h1[hi].faces[m][0]  &&
                        h1[ho].faces[n][1] == h1[hi].faces[m][1]  &&
                        h1[ho].faces[n][2] == h1[hi].faces[m][2]  &&
                        h1[ho].faces[n][3] == h1[hi].faces[m][3])
                    {
                        e1 = ho + tpsiz;
                        e2 = hi + tpsiz;

                        scratch[e1][n] = 1;
                        scratch[e2][m] = 1;
                    }
                }		// -- end m-loop
            }			// -- end n-loop
        }				// -- end hi-loop
    }					// -- end ho-loop

    // Search only pents.
    // Find which elements share a face.
#ifdef BUILD_OPENMP
    #pragma omp parallel for num_threads(pThreads) private(e1, e2)
#endif
    for(int ho=0; ho<psize; ++ho)
    {
        for(int hi=ho+1; hi<psize; ++hi)
        {
            for(int n=2; n<5; ++n)
            {
                for(int m=2; m<5; ++m)
                {
                    if (p1[ho].faces[n][0] == p1[hi].faces[m][0]  &&
                        p1[ho].faces[n][1] == p1[hi].faces[m][1]  &&
                        p1[ho].faces[n][2] == p1[hi].faces[m][2]  &&
                        p1[ho].faces[n][3] == p1[hi].faces[m][3])
                    {
                        e1 = ho + tsize;
                        e2 = hi + tsize;

                        scratch[e1][n] = 1;
                        scratch[e2][m] = 1;
                    }
                }		// -- end m-loop
            }			// -- end n-loop

            for(int n=0; n<2; ++n)
            {
                for(int m=0; m<2; ++m)
                {
                    if (p1[ho].faces[n][0] == p1[hi].faces[m][0]  &&
                        p1[ho].faces[n][1] == p1[hi].faces[m][1]  &&
                        p1[ho].faces[n][2] == p1[hi].faces[m][2])
                    {
                        e1 = ho + tsize;
                        e2 = hi + tsize;

                        scratch[e1][n] = 1;
                        scratch[e2][m] = 1;
                    }
                }		// -- end m-loop
            }			// -- end n-loop
        }				// -- end hi-loop
    }					// -- end ho-loop

    // Search only tets.
    // Find which elements share a face.
#ifdef BUILD_OPENMP
    #pragma omp parallel for num_threads(tThreads) private(e1, e2)
#endif
    for(int ho=0; ho<tsize; ++ho)
    {
        for(int hi=ho+1; hi<tsize; ++hi)
        {
            for(int n=0; n<4; ++n)
            {
                for(int m=0; m<4; ++m)
                {
                    if (t1[ho].faces[n][0] == t1[hi].faces[m][0]  &&
                        t1[ho].faces[n][1] == t1[hi].faces[m][1]  &&
                        t1[ho].faces[n][2] == t1[hi].faces[m][2])
                    {
                        scratch[ho][n] = 1;
                        scratch[hi][m] = 1;
                    }
                }		// -- end m-loop
            }			// -- end n-loop
        }				// -- end hi-loop
    }					// -- end ho-loop

    // 1st order pents & hexs together.
    if (psize > 0  &&  hsize > 0)
    {
#ifdef BUILD_OPENMP
        #pragma omp parallel for num_threads(hThreads) private(e1, e2)
#endif
        for(int ho=0; ho<hsize; ++ho)
        {
            for(int hi=0; hi<psize; ++hi)
            {
                for(int n=0; n<6; ++n)
                {
                    for(int m=2; m<5; ++m)
                    {
                        if (h1[ho].faces[n][0] == p1[hi].faces[m][0]  &&
                            h1[ho].faces[n][1] == p1[hi].faces[m][1]  &&
                            h1[ho].faces[n][2] == p1[hi].faces[m][2]  &&
                            h1[ho].faces[n][3] == p1[hi].faces[m][3])
                        {
                            e1 = ho + tpsiz;
                            e2 = hi + tsize;

                            scratch[e1][n] = 1;
                            scratch[e2][m] = 1;
                        }
                    }		// -- end m-loop
                }			// -- end n-loop
            }				// -- end h1-loop over pents
        }					// -- end ho-loop over hexs
    }						// -- end if for both pents & hexs

    // 1st order tets & pents together.
    if (psize > 0  &&  tsize > 0)
    {
#ifdef BUILD_OPENMP
        #pragma omp parallel for num_threads(tThreads) private(e1, e2)
#endif
        for(int ho=0; ho<tsize; ++ho)
        {
            for(int hi=0; hi<psize; ++hi)
            {
                for(int n=0; n<4; ++n)
                {
                    for(int m=0; m<2; ++m)
                    {
                        if (t1[ho].faces[n][0] == p1[hi].faces[m][0]  &&
                            t1[ho].faces[n][1] == p1[hi].faces[m][1]  &&
                            t1[ho].faces[n][2] == p1[hi].faces[m][2])
                        {
                            e1 = ho;
                            e2 = hi + tsize;

                            scratch[e1][n] = 1;
                            scratch[e2][m] = 1;
                        }
                    }		// -- end m-loop
                }			// -- end n-loop
            }				// -- end h1-loop over pents
        }					// -- end ho-loop over hexs
    }						// -- end if for both pents & hexs

    // ---------------------------------------------------------------
    // Gather the surface elements and corresponding face numbers.
    // 1st order hexs
    for(int ho=tpsiz; ho<total; ++ho)
    {
        for(int n=0; n<6; ++n)
        {
            if (scratch[ho][n] == 0)
            {
                t2[0] = ho - tpsiz;				// element #
                t2[1] = n + 20;					// face #
                surfaceEls.push_back( t2 );
            }
        }
    }					// -- end ho-loop

    // 1st order pents
    for(int ho=tsize; ho<tpsiz; ++ho)
    {
        for(int n=0; n<5; ++n)
        {
            if (scratch[ho][n] == 0)
            {
                t2[0] = ho - tsize;				// element #
                t2[1] = n + 10;					// face #
                surfaceEls.push_back( t2 );
            }
        }
    }					// -- end ho-loop

    // 1st order tets
    for(int ho=0; ho<tsize; ++ho)
    {
        for(int n=0; n<4; ++n)
        {
            if (scratch[ho][n] == 0)
            {
                t2[0] = ho;						// element #
                t2[1] = n;						// face #
                surfaceEls.push_back( t2 );
            }
        }
    }					// -- end ho-loop
}						// -- end findSurfaceElements

// ----------------------------------------------------------------------------
void ABaqus_part::findSurfaceTriangles()
{
    int k;
    int face;
    int el;
    int node[4];
    int nCount = 0;

    //  ***********************************************************************
    //  * nodes array is ABaqus_part nodes array specific to the part.
    //  * nodes array is not the "master" part array from the Abaqus flat file.
    //  * Connectivity indices must've been converted from flat file values
    //  * to indices in the part through using ABaqus_part pNodeMap when
    //  * extracting the part info from the "master" part.
    //  ***********************************************************************

    surfaceNodes.clear();
    surfaceTriangles.clear();

    for(int f=0; f < (int) surfaceEls.size(); ++f)
    //for(int f=0; f < 4; ++f)
    {
        el   = surfaceEls[f][0];
        face = surfaceEls[f][1];

        if (face > 19)
        {
            // 1st order hexs
            face -= 20;
            for (int i=0; i<4; ++i)
            {
                k       = ABaqus_part::hexFaces[face][i];
                node[i] = h1[el].connect[k];
            }
            // Must duplicate in order to intermix triangles made
            // from hex-quads with triangles from tet-triangles.
            surfaceNodes.push_back( nodes[node[0]] );
            surfaceNodes.push_back( nodes[node[1]] );
            surfaceNodes.push_back( nodes[node[2]] );
            surfaceNodes.push_back( nodes[node[0]] );
            surfaceNodes.push_back( nodes[node[2]] );
            surfaceNodes.push_back( nodes[node[3]] );
            // 1st triangle
            surfaceTriangles.push_back( nCount );
            surfaceTriangles.push_back( nCount + 1 );
            surfaceTriangles.push_back( nCount + 2 );
            // 2nd triangle
            surfaceTriangles.push_back( nCount + 3 );
            surfaceTriangles.push_back( nCount + 4 );
            surfaceTriangles.push_back( nCount + 5 );
            nCount += 6;
        }
        else if (face > 9)
        {
            // 1st order pents
            face -= 10;
            if (face < 2)
            {
                for (int i=0; i<3; ++i)
                {
                    k       = ABaqus_part::pentFaces[face][i];
                    node[i] = p1[el].connect[k];
                    surfaceNodes.push_back( nodes[node[i]] );
                }
                // 1st triangle
                surfaceTriangles.push_back( nCount );
                surfaceTriangles.push_back( nCount + 1 );
                surfaceTriangles.push_back( nCount + 2 );
                nCount += 3;
            }
            else
            {
                for (int i=0; i<4; ++i)
                {
                    k       = ABaqus_part::pentFaces[face][i];
                    node[i] = p1[el].connect[k];
                    surfaceNodes.push_back( nodes[node[i]] );
                }
                // 1st triangle
                surfaceTriangles.push_back( nCount );
                surfaceTriangles.push_back( nCount + 1 );
                surfaceTriangles.push_back( nCount + 2 );
                // 2nd triangle
                surfaceTriangles.push_back( nCount );
                surfaceTriangles.push_back( nCount + 2 );
                surfaceTriangles.push_back( nCount + 3 );
                nCount += 4;
            }
        }
        else
        {
            // 1st order tets
            for (int i=0; i<3; ++i)
            {
                k       = ABaqus_part::tetFaces[face][i];
                node[i] = t1[el].connect[k];
            }
            // 1st triangle
            surfaceTriangles.push_back( nCount );
            surfaceTriangles.push_back( nCount + 1 );
            surfaceTriangles.push_back( nCount + 2 );
            nCount += 3;
        }
    }							// -- end f-loop

}
