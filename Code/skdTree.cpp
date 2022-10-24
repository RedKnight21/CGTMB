#include "skdTree.h"

void SkdTree::printDebugLinkp()
{
    qDebug() << "-- linkp values --";
    for (int i = 0; i<number*2; i++)
    {
        qDebug() << "i: " << i << "\t linkp: " << linkp[i];
    }
}

void SkdTree::printDebugCutDir()
{
    qDebug() << "-- cutDir values --";
    for (int i = 0; i<number*2; i++)
    {
        qDebug() << "i: " << i << "\t cutDir: " << cutDir[i];
    }
}

void SkdTree::printDebugSbox()
{
    qDebug() << "-- sbox values --";
    for (int i = 0; i<number*2; i++)
    {
        qDebug() << "i: " << i << "\t sbox:\t" << sbox[i]._min << "\t" << sbox[i]._max;
    }
}
// --------------------------------------------------------------------------------------------
void SkdTree::createTree(std::vector<Point> &centroids)
{
        // qDebug() << "size of centroids: " << centroids.size() << eBoxes.size();
        int num = number = centroids.size();
        // Allocate memory for the tree
        this->linkp  = new int[num*2];
        this->sbox   = new AlignedBox[num*2];
        this->cutDir = new unsigned char[num*2];

            //if ((fp = fopen("/Users/roger/Public/Programs/TestFiles/skdtree_build.txt", "w")) == NULL)
            //{
             //   qDebug() << "-- error opening debug file --\n";
            //}

        // Allocate temporary working memory.
        this->mIndex = new int[num];

        if (num == 1)
        {
            linkp[0] = -1;
        }
        else
        {
            //Initialize indexs for the entire tree.
            // mIndex contains the permutations of the element ID's.
            for(int i = 0; i<num; i++)
                mIndex[i] = i;
            // Build the tree starting from the 1st node.
            //num--;
            int nextp = 0;
            buildBranch(centroids, nextp, 0, num-1, 0);
        }
        delete [] mIndex;
            //fclose(fp);
}                       // -- end createTree
// --------------------------------------------------------------------------------------------
    void SkdTree::buildBranch(std::vector<Point> &centroids, int &nextp, int ileft, int iright, int jnode)
    {
            //qDebug() << "\n\nbuildBranch:: size of centroids: " << centroids.size() << eBoxes.size();
            unsigned char   localCutDir;
            int             imedian;
            int             imedianr;
            int             jnodeL;
            int             jnodeR;
            int             jleft;
            int             jright;
            int             i, j, k;
            float           minVal;
            float           maxVal;

            //int irog = eBoxes.size();

            //fprintf(fp, "\n -- buildBranch --\n");

            // Find the cutting direction.
            localCutDir   = find_centroid_coord_extremes(centroids, ileft, iright);
            cutDir[jnode] = localCutDir;
            //qDebug() << "localCutDir is " << localCutDir;
            //qDebug() << "ileft: " << ileft << "\tiright: " << iright;

            //fprintf(fp, "buildBranch:: jnode: %d \t cutDir: %d\n", jnode, cutDir[jnode]);

            // Find the safety box for the cutting direction.
            minVal =  1.0e+33;
            maxVal = -1.0e+33;
            for (j=ileft; j<=iright; j++)
            {
                //if (irog > 20000) qDebug() << "(1) j: " << j;
                //if ((j < 0) || (mIndex[j] < 0))
                //{
                    //qDebug() << "j: " << j << "\tmIndex: " << mIndex[j];
                    //qDebug() << "eBoxes: " << eBoxes[mIndex[j]].box[localCutDir] << eBoxes[mIndex[j]].box[localCutDir+3];
                //}
                //if (j > -1)
                //{
                    if (eBoxes[mIndex[j]].box[localCutDir] < minVal)
                        minVal = eBoxes[mIndex[j]].box[localCutDir];
                    if (eBoxes[mIndex[j]].box[localCutDir+3] > maxVal)
                        maxVal = eBoxes[mIndex[j]].box[localCutDir+3];
                //}
                //if (irog > 20000) qDebug() << "(2) j: " << j << "\t" << ileft << "\t" << iright;
            }

            //if (irog > 20000) qDebug() << " ******************";

            sbox[jnode]._min = minVal;
            sbox[jnode]._max = maxVal;
            //qDebug() << "jnode: " << jnode << "\t min: " << minVal << "\t max: " << maxVal;
            //fprintf(fp, "\tminVal: %f\t maxVal: %f\n", minVal, maxVal);

            // Increment the indexes for the next set of nodes.
            jnodeL = nextp + 1;
            jnodeR = nextp + 2;
            //qDebug() << "jnode: " << jnode << "\t nextp: " << nextp << "\t jnodeL: " << jnodeL << "\t jnodeR: " << jnodeR;

            // Store the index to the next available array location.
            linkp[jnode] = jnodeL;

            // Select the exact median to have a fully balanced tree.
            imedian = (ileft + iright) / 2;
            //qDebug() << "imedian: " << imedian << "\t ileft: " << ileft << "\t iright: " << iright;

            // Re-order the indexes in mIndex so that the left box has centroids <= median
            // and the right box has centroids >= median.
            sort_coordinates(centroids, localCutDir, imedian, ileft, iright);

            //fprintf(fp, "-- Left Bucket --\n");
            //for(int k=ileft; k<=imedian; k++)
            //{
                //fprintf(fp, "\t%d\t %d\t %f\t %f\t %f\n", k, mIndex[k],
                //        centroids[mIndex[k]].coord[0], centroids[mIndex[k]].coord[1], centroids[mIndex[k]].coord[2] );
            //}
            //fprintf(fp, "-- Right Bucket --\n");
            //for(int k=imedian; k<=iright; k++)
            //{
                //fprintf(fp, "\t%d\t %d\t %f\t %f\t %f\n", k, mIndex[k],
                //        centroids[mIndex[k]].coord[0], centroids[mIndex[k]].coord[1], centroids[mIndex[k]].coord[2] );
            //}

            //for (int n = 0; n < number; n++)
            //{
            //    qDebug() << "mIndex: " << n << "\t " << mIndex[n];
            //}

            // Process those to the left of center and the center.
            if (imedian == ileft)
            {
                // Leaf Node.  Leave sbox values set to 0 for leaf nodes.
                //qDebug() << " {*}{*} LEFT :: jnodeL: " << jnodeL << "\t ileft: " << ileft << "\t mIndex: " << mIndex[ileft];
                linkp[jnodeL] = -mIndex[ileft] - 1;
                nextp        += 1;
            }
            else
            {
                nextp = jnodeR;
                jleft = ileft;
                //qDebug() << "+ going LEFT :: nextp: " << nextp<< "\t imedian: " << imedian << "\t jleft: " << jleft
                //        << "\t jnodeL: " << jnodeL;
                buildBranch(centroids, nextp, jleft, imedian, jnodeL);
                //qDebug() << "\n*** Return from LEFT:: jnodeL: " << jnodeL << "\t nextp: " << nextp;
                //this->printDebugLinkp();
            }

            //qDebug() << "imedian: " << imedian << "\t iright: " << iright;
            // Process those to the right of center.
            imedianr = imedian + 1;
            if (imedianr == iright)
            {
                // Leaf Node.  Leave sbox values set to 0 for leaf nodes.
                //qDebug() << " {*}{*} RIGHT:: jnodeR: " << jnodeR << "\t iright: " << iright<< "\t mIndex: " << mIndex[iright];

                linkp[jnodeR] = -mIndex[iright] - 1;
            }
            else
            {
                nextp += 1;
                jright = iright;
                //qDebug() << "\n+ going RIGHT:: nextp: " << nextp << "\t imedianr: " << imedianr << "\t jright: " << jright
                //        << "\t jnodeR: " << jnodeR;
                buildBranch(centroids, nextp, imedianr, jright, jnodeR);
                //qDebug() << "*** Return from RIGHT:: jnodeR: " << jnodeR << "\t nextp: " << nextp;
            }
    }

    // ----------------------------------------------------------------------------------------

        unsigned char SkdTree::find_centroid_coord_extremes(std::vector<Point> &centroids, int blow, int bupp)
        {
            float           maxVal;
            float           minVal;
            float           tmp[3];

            int             i, j;

            //fprintf(fp, "-- find_centroid_coord_extremes -- ");
            //fprintf(fp, "\tblow: %d\t bupp: %d\n" , blow, bupp);

            for(i = 0; i < 3; i++)
            {
                //qDebug() << " ------------\n" << " Direction: " << i;
                //qDebug() << " blow: " << blow << " bupp: " << bupp;
                maxVal = -1.0e+33;  minVal = 1.0e+33;
                for (j = blow; j <= bupp; j++)
                {
                    //qDebug() << "j: " << j << "\tX: " << centroids[j].coord[i] << "\tmaxVal: "
                    //         << maxVal << "\tminVale: " << minVal;
                    //fprintf(fp, "\t\t index: %d\t max: %f\t\t coord: %f\n", j, maxVal, centroids[mIndex[j]].coord[i]);
                    if (centroids[mIndex[j]].coord[i] > maxVal)
                        maxVal = centroids[mIndex[j]].coord[i];
                    if (centroids[mIndex[j]].coord[i] < minVal)
                        minVal = centroids[mIndex[j]].coord[i];
                }
                tmp[i] = maxVal - minVal;
                //qDebug() << "tmp distance is " << tmp[i];
                //fprintf(fp, "i: %d\t extents: %f\t min: %f\t max: %f\n", i, tmp[i], minVal, maxVal);
            }
            maxVal = -1.0e+33;
            j      = -1;
            for(i = 0; i < 3; i++)
            {
                if (tmp[i] > maxVal)
                {
                    j      = i;
                    maxVal = tmp[i];
                }
            }
            ////fprintf(fp, "\t returning direction: %d\n", j);
            return ((unsigned char) j);
        }

        // ----------------------------------------------------------------------------------------

        void SkdTree::sort_coordinates(std::vector<Point> &xyz, int coord, int center, int lindx, int rindx)
        {
            // Sort locatins relative to the median cut-point.  This will re-order the indexes
            // along coordinate coord so that center is >= those below and <= those above.
            // This does not guarantee that all positions in the respective sub-domains will
            // be in order.

            int     i, m, s, t;
            int     lleft;          // local left index
            int     lright;         // local right index

            // Local setup.
            lleft  = lindx;
            lright = rindx;

            // Cycle through the locations from left to right.
            while (lleft < lright)
            {
                t = mIndex[lleft];
                m = lleft;

                // Cycle through indexes to the right of the current location.
                // Switch indexes to put locations in order.
                for (i = lleft + 1; i <= lright; i++)
                {
                    if (xyz[mIndex[i]].coord[coord] < xyz[t].coord[coord])
                    {
                        m = m + 1;
                        s         = mIndex[m];
                        mIndex[m] = mIndex[i];
                        mIndex[i] = s;
                    }
                }           // -- end i-loop
                s             = mIndex[lleft];
                mIndex[lleft] = mIndex[m];
                mIndex[m]     = s;

                // Avoid moving the median point.
                if (m <= center)
                {
                    lleft = m + 1;
                }
                if (m >= center)
                {
                    lright = m - 1;
                }
            }
        }

    // ------------------------------------------------------------------------
    int SkdTree::findInstanceElement(Point &point, Point &ghr, int &gEl, unsigned char &eType)
    {
        int elNo;
        unsigned char iType;

        // Get the list of potential elements containing the point.
        // nodesFound contain local element numbers for the instance.
        nodesFound.clear();
        findNodeBox(point, 0);
        //Point ghr;

        /*if (iflag)
        {
            qDebug() << "\n * findInstanceElement *" ;
            qDebug() << "nodesFound size is " << nodesFound.size();
            qDebug() << "point: " << point.coord[0] << "\t" << point.coord[1] << "\t"  << point.coord[2];
        }*/

        elNo = -1; gEl = -1;

        if (nodesFound.size() > 0)
        {
            bool found = false;
            int  i     = 0;
            //if (iflag) qDebug() << "nodesFound.size is " << nodesFound.size();
            while  ((i<nodesFound.size()) && (!found))
            {
                // Get global element number and its type.
                gEl  = getGlobalTypeElementNumber(nodesFound[i], iType, eType);
                //elNo = nodesFound[i];
                elNo = nodesFound[i] + ctNumbers2[iType];
                //if (iflag)
                //{
                //    qDebug() << "nodesFound is : " << nodesFound[i];
                //    qDebug() << "\t type: " << this->eType[nodesFound[i]];
                //    qDebug() << "elNo: " << elNo << "\ti: " << i;
                //}

                if ((eType == 4) || (eType == 14))
                {
                    found = isInside1stOrderTet(elNo, point, ghr, eType);
                    //if (elNo == 538) qDebug() << "ghr: " << ghr.coord[0] << ghr.coord[1] << ghr.coord[2];
                }
                else if ((eType == 6) || (eType == 16))
                {
                    found = isInside1stOrderHex(elNo, point, ghr, eType, false);
                    //qDebug() << "ghr: " << ghr.coord[0] << ghr.coord[1] << ghr.coord[2];
                }
                else if ((eType == 5) || (eType == 15))
                {
                    found = isInside1stOrderPent(elNo, point, ghr, eType);
                    //if (iflag) qDebug() << "found: " << found << "\tghr: " << ghr.coord[0] << ghr.coord[1] << ghr.coord[2];
                    if (!found) elNo = -elNo;
                }
                i++;
                //qDebug() << "i: " << i << "size is " << nodesFound.size();
            }
        }
        //qDebug() << "findInstanceElement:: returning elNo:" << elNo;

        return elNo;
    }

    // --------------------------------------------------------------------------------------------
    int SkdTree::findInstanceElement(Point &point, Point &ghr, int &gEl, unsigned char &eType, bool pFlag)
    {
        int elNo;
        unsigned char iType;

        // Get the list of potential elements containing the point.
        // nodesFound contain local element numbers for the instance.
        nodesFound.clear();
        findNodeBox(point, 0);

        if (pFlag)
        {
            qDebug() << "\n * findInstanceElement *" ;
            qDebug() << "nodesFound size is " << nodesFound.size();
            qDebug() << "point: " << point.coord[0] << "\t" << point.coord[1] << "\t"  << point.coord[2];
            for(int k=0; k<nodesFound.size(); ++k)
            {
                qDebug() << "\t k: " << k << "\t local el: " << nodesFound[k];
            }
        }

        elNo = -1; gEl = -1;

        if (nodesFound.size() > 0)
        {
            bool found = false;
            int  i     = 0;
            if (pFlag) qDebug() << "\nnodesFound.size is " << nodesFound.size();
            while  ((i<nodesFound.size()) && (!found))
            {
                // Get global element number and its type.
                gEl  = getGlobalTypeElementNumber(nodesFound[i], iType, eType);
                gEl += ctNumbers2[iType];
                elNo = nodesFound[i] + ctNumbers2[iType];
                if (pFlag)
                {
                    qDebug() << "nodesFound is : " << nodesFound[i]
                             << "\t type: " << this->eType[nodesFound[i]]
                             << "\telNo: " << elNo << "\ti: " << i << "\t gEl: " << gEl;
                    qDebug() << "iType: " << iType << "\tctNumbers2: " << ctNumbers2[iType]
                             << "\telNumbers: " << elNumbers[iType] << "\n";
                }

                if ((eType == 4) || (eType == 14))
                {
                    found = isInside1stOrderTet(elNo, point, ghr, eType);
                    //if (pFlag) qDebug() << "found: " << found << "\telNo: " << elNo << "\tgEl: " << gEl
                    //        << "\tghr: " << ghr.coord[0] << ghr.coord[1] << ghr.coord[2];
                }
                else if ((eType == 6) || (eType == 16))
                {
                    found = isInside1stOrderHex(elNo, point, ghr, eType, pFlag);
                    if (pFlag)
                    {
                        qDebug() << "ghr: " << ghr.coord[0] << ghr.coord[1] << ghr.coord[2] << "\n";
                    }
                }
                else if ((eType == 5) || (eType == 15))
                {
                    found = isInside1stOrderPent(elNo, point, ghr, eType);
                    //if (iflag) qDebug() << "found: " << found << "\tghr: " << ghr.coord[0] << ghr.coord[1] << ghr.coord[2];
                }
                if (!found)
                {
                    elNo = -elNo;
                    gEl  = -gEl;
                }
                i++;
                //qDebug() << "i: " << i << "size is " << nodesFound.size();
            }
        }
        //qDebug() << "findInstanceElement:: returning elNo:" << elNo;

        return elNo;
    }

    // ------------------------------------------------------------------------
    int SkdTree::getGlobalTypeElementNumber(int localEl, unsigned char &iType, unsigned char &eType)
    {
        // Return global element number and the element type.

        int gElNo;          // global type element #
        unsigned char i;

        gElNo = 0;
        for (i=0; i<6; i++)
        {
            //qDebug() << "getGlobalTypeElementNumber:: localEl " << localEl << "\t ctNumbers1: " << ctNumbers1[i];
            if (localEl < ctNumbers1[i])
            {
                gElNo = localEl + elAdjust[i];
                break;
            }
        }
        //qDebug() << "i for eType: " << i;
        iType = i;
        switch(iType)
        {
            case 0:
                eType = 4;
                //gElNo = localEl + 1;
                break;
             case 1:
                eType = 5;
                //gElNo = localEl + elAdjust[1] + 1;
                break;
             case 2:
                eType = 6;
                ///gElNo = localEl + elAdjust[2] + 1;
                break;
             case 3:
                eType = 14;
                //gElNo = localEl + elAdjust[3] + 1;
                break;
             case 4:
                eType = 15;
                //gElNo = localEl + elAdjust[4] + 1;
                break;
             case 5:
                eType = 16;
                //gElNo = localEl + elAdjust[5] + 1;
                break;
             default:
                eType = 0;
        }
        //qDebug() << " getGlobalTypeElementNumber:: localEl: " << localEl << "\t global El: " << gElNo << "\t eType: " << eType;
        return gElNo;
    }
    // --------------------------------------------------------------------------------------------
    void SkdTree::findNodeBox(Point &point, int node)
    {
        int i, iFlag, inode;

        if (linkp[node] < 0)
        {
            inode = -linkp[node] - 1;

            //qDebug() << "inode: " << inode << "\tpoint: " << point.coord[0] << point.coord[1] << point.coord[2];

            // check to see if the point is within the leaf node box.
            iFlag = 0;
            for (i=0; i<3; i++)
            {
                //qDebug() << "leaf:: " << i << "box: " << eBoxes[inode].box[i]  << "box: " << eBoxes[inode].box[i+3];
                if ((point.coord[i] >= eBoxes[inode].box[i]) && (point.coord[i] <= eBoxes[inode].box[i+3]))
                {
                    iFlag = iFlag + 1;
                }
            }
            //qDebug() << "iFlag: " << iFlag;
            if (iFlag == 3)
            {
                nodesFound.push_back(inode);
            }
        }
        else
        {
            //qDebug() << "node: " << node << "\tcutDir: " << cutDir[node];
            //qDebug() << "\tmax: " << sbox[node]._max << "\tmin: " << sbox[node]._min ;
            // The following two checks are needed for overlapping domains.
            // See if the point is in the left sub-domain.
            if (point.coord[cutDir[node]] <= sbox[node]._max)
            {
                //qDebug() << "-- Going left --";
                findNodeBox(point, linkp[node]);
                //qDebug() << "-- Return left --";
            }

            // See if the point is in the left sub-domain.
            if (point.coord[cutDir[node]] >= sbox[node]._min)
            {
                //qDebug() << "-- Going right --";
                findNodeBox(point, linkp[node]+1);
                //qDebug() << "-- Return left --";
            }
        }
    }

    // ------------------------------------------------------------------------
    bool SkdTree::isInside1stOrderHex(int hex, Point &point, Point &ghr, unsigned char eType, bool pFlag)
    {
        bool    returnValue;
        float   deltas[3];
        float   guess[3];
        float   funcResults[3];
        float   dervResults[3][3];
        float   verts[8][3];

        float   tr, th;

        int     i, j;
        int     ii, jj, rr, mm, gg, hh;
        int     quitFlag;

        //qDebug() << "Search point: " << point.coord[0] << point.coord[1] << point.coord[2];
        // Gather the element's node locations.
        if (eType == 6)
        {
            for (i = 0; i<8; i++)
            {
                // De-reference vector pointers.
                j = (*h1elements)[hex].connect[i];
                verts[i][0] = (*nodes)[j].x;
                verts[i][1] = (*nodes)[j].y;
                verts[i][2] = (*nodes)[j].z;
                if (pFlag) qDebug() << "j: " << j << "x: " << verts[i][0] << "y: " << verts[i][1] << "z: " << verts[i][2];
            }
        }
        else
        {
            for (i = 0; i<8; i++)
            {
                j = (*h2elements)[hex].connect[i];
                verts[i][0] = (*nodes)[j].x;
                verts[i][1] = (*nodes)[j].y;
                verts[i][2] = (*nodes)[j].z;
                //qDebug() << "j: " << j << "x: " << verts[i][0] << "y: " << verts[i][1] << "z: " << verts[i][2];
            }
        }
        //print8x3Array(verts, 8);

        // Set the g,h,r guess.
        guess[0] = 0.0;  guess[1] = 0.0;  guess[2] = 0.0;

        j = quitFlag = 0;
        while ((j<20) && (quitFlag < 3))
        {
            // ------------------------------------------------------------------------------------
            // eval1stOrderHexFunc
            funcResults[0] = 0.0;  funcResults[1] = 0.0;  funcResults[2] = 0.0;
            // Loop over each direction.
            for (ii = 0; ii < 3; ii++)
            {
                jj = -1;
                for (rr = 0; rr < 2; rr++)
                {
                    tr = 1.0 + asign[rr] * guess[2];
                    mm = -1;
                    for (hh = 0; hh < 2; hh++)
                    {
                        th = tr * (1.0 + asign[hh] * guess[1]);
                        for (gg = 0; gg < 2; gg++)
                        {
                            jj++;
                            mm++;
                            funcResults[ii] += th * (1.0 + gsign[mm] * guess[0]) * verts[jj][ii];
                            //qDebug() << "ii: " << ii << "\tth: " << th << "\tgsign: " << gsign[mm] << "\tverts: " << verts[jj][ii];
                            //qDebug() << "guess[0]: " << guess[1] << "\tfuncResults: " << funcResults[ii];
                        }
                    }
                }           // rr-loop end
                funcResults[ii] = point.coord[ii] - (0.125 * funcResults[ii]);
                //qDebug() << "funcResults:: ii: " << ii << "\t" << funcResults[ii] << "\n";
            }
            // ------------------------------------------------------------------------------------
            // eval1stOrderHexDerivatives
            for (ii = 0; ii < 3; ii++)
            {
                for (jj = 0; jj < 3; jj++)
                {
                    dervResults[ii][jj] = 0.0;
                }
            }
            // Loop over each direction.
            for (ii = 0; ii < 3; ii++)
            {
                // Derivative w.r.t. the 1st master co-ordinate.  Put in column #1 (0).
                dervResults[ii][0]  = (1.0 - guess[1]) * (1.0 - guess[2]) * (verts[1][ii] - verts[0][ii]);
                dervResults[ii][0] += (1.0 + guess[1]) * (1.0 - guess[2]) * (verts[2][ii] - verts[3][ii]);
                dervResults[ii][0] += (1.0 - guess[1]) * (1.0 + guess[2]) * (verts[5][ii] - verts[4][ii]);
                dervResults[ii][0] += (1.0 + guess[1]) * (1.0 + guess[2]) * (verts[6][ii] - verts[7][ii]);
                dervResults[ii][0] *= 0.125;

                // Derivative w.r.t. the 2nd master co-ordinate.  Put in column #2 (1).
                dervResults[ii][1]  = (1.0 - guess[0]) * (1.0 - guess[2]) * (verts[3][ii] - verts[0][ii]);
                dervResults[ii][1] += (1.0 + guess[0]) * (1.0 - guess[2]) * (verts[2][ii] - verts[1][ii]);
                dervResults[ii][1] += (1.0 - guess[0]) * (1.0 + guess[2]) * (verts[7][ii] - verts[4][ii]);
                dervResults[ii][1] += (1.0 + guess[0]) * (1.0 + guess[2]) * (verts[6][ii] - verts[5][ii]);
                dervResults[ii][1] *= 0.125;

                // Derivative w.r.t. the 3rd master co-ordinate.  Put in column #3 (2).
                dervResults[ii][2]  = (1.0 - guess[0]) * (1.0 - guess[1]) * (verts[4][ii] - verts[0][ii]);
                dervResults[ii][2] += (1.0 + guess[0]) * (1.0 - guess[1]) * (verts[5][ii] - verts[1][ii]);
                dervResults[ii][2] += (1.0 - guess[0]) * (1.0 + guess[1]) * (verts[7][ii] - verts[3][ii]);
                dervResults[ii][2] += (1.0 + guess[0]) * (1.0 + guess[1]) * (verts[6][ii] - verts[2][ii]);
                dervResults[ii][2] *= 0.125;
            }
            // ------------------------------------------------------------------------------------
            //qDebug() << " -- dervResults --";
            //print8x3Array(dervResults, 3);

            ii = matrixInverse3x3(dervResults);

            //qDebug() << " -- inverse dervResults --";
            //print8x3Array(dervResults, 3);

            matMult3(dervResults, funcResults, deltas);
            //qDebug() << "Deltas: " << deltas[0] << "\t" << deltas[1] << " \t" << deltas[2];
            for (jj = 0; jj < 3; jj++)
            {
                guess[jj] += deltas[jj];
            }
            quitFlag = 0;
            j++;
            for (jj = 0; jj < 3; jj++)
            {
                if (fabs(deltas[jj]) < 1.0e-6)
                {
                    quitFlag += 1;
                }
            }
        }           // -- end while loop

        //qDebug() << "-- j: " << j;
        //qDebug() << "guess: " << guess[0] << guess[1] << guess[2];
        quitFlag = 0;
        for (jj = 0; jj < 3; jj++)
        {
            ghr.coord[jj] = guess[jj];
            if ((guess[jj] >= -1.00001) && (guess[jj] <= 1.00001))
                quitFlag++;

            if (pFlag)
            {
                qDebug() << "isInside: " << guess[jj] << "\tquitFlag :" << quitFlag << "\t " << (-1.0 - guess[jj]);
            }
        }
        returnValue = false;
        if (quitFlag >= 3) returnValue = true;
        //qDebug() << "quitFlag is " << quitFlag;

        return returnValue;
    }
// ------------------------------------------------------------------------------------------------
    bool SkdTree::isInside1stOrderTet(int tet, Point &point, Point &ghr, unsigned char eType)
    {
        float   deltas[3];
        float   guess[3];
        float   funcResults[3];
        float   dervResults[3][3];
        float   verts[4][3];
        float   tghr;

        int     i, j;
        int     ii, jj;
        int     quitFlag;

        bool    returnValue;

        //qDebug() << "Search point: " << point.coord[0] << point.coord[1] << point.coord[2];
        // Gather the element's node locations.
        if (eType == 4)
        {
            for (i = 0; i<4; i++)
            {
                j = (*t1elements)[tet].connect[i];
                verts[i][0] = (*nodes)[j].x;
                verts[i][1] = (*nodes)[j].y;
                verts[i][2] = (*nodes)[j].z;
                //qDebug() << "j: " << j << "x: " << verts[i][0] << "y: " << verts[i][1] << "z: " << verts[i][2];
            }
        }
        else
        {
            for (i = 0; i<4; i++)
            {
                j = (*t2elements)[tet].connect[i];
                verts[i][0] = (*nodes)[j].x;
                verts[i][1] = (*nodes)[j].y;
                verts[i][2] = (*nodes)[j].z;
                //qDebug() << "j: " << j << "x: " << verts[i][0] << "y: " << verts[i][1] << "z: " << verts[i][2];
            }
        }
        //print8x3Array(verts, 4);

        // Set the g,h,r guess.
        guess[0] = 0.0;  guess[1] = 0.0;  guess[2] = 0.0;

        j = quitFlag = 0;
        while ((j<20) && (quitFlag < 3))
        {
            // ------------------------------------------------------------------------------------
            // eval1stOrderTetFunc
            tghr = 1.0 - guess[0] - guess[1] - guess[2];
            // Loop over each direction.
            for (ii = 0; ii < 3; ii++)
            {
                funcResults[ii] = tghr * verts[0][ii];
                for (jj = 1; jj < 4; jj++)
                {    
                    funcResults[ii] += guess[jj-1] * verts[jj][ii];
                }
                funcResults[ii] = point.coord[ii] - funcResults[ii];

                //qDebug() << "funcResults:: ii: " << ii << "\t" << funcResults[ii];
            }
            // ------------------------------------------------------------------------------------
            // eval1stOrderTetDerivatives
            // --------------------------
            // Loop over each direction.
            for (ii = 0; ii < 3; ii++)
            {
                // Derivative w.r.t. the 1st master co-ordinate.  Put in column #1 (0).
                dervResults[ii][0]  = (verts[1][ii] - verts[0][ii]);

                // Derivative w.r.t. the 2nd master co-ordinate.  Put in column #2 (1).
                dervResults[ii][1]  = (verts[2][ii] - verts[0][ii]);

                // Derivative w.r.t. the 3rd master co-ordinate.  Put in column #3 (2).
                dervResults[ii][2]  = (verts[3][ii] - verts[0][ii]);
            }
            // ------------------------------------------------------------------------------------
            //qDebug() << " -- dervResults --";
            //print8x3Array(dervResults, 3);

            //ii = matrixInverse3x3(dervResults);

            //qDebug() << " -- inverse dervResults --";
            //print8x3Array(dervResults, 3);

            matMult3(dervResults, funcResults, deltas);
            //qDebug() << "Deltas: " << deltas[0] << "\t" << deltas[1] << " \t" << deltas[2];
            for (jj = 0; jj < 3; jj++)
            {
                guess[jj] += deltas[jj];
            }
            quitFlag = 0;
            j++;
            for (jj = 0; jj < 3; jj++)
            {
                if (fabs(deltas[jj]) < 1.0e-6)
                {
                    quitFlag += 1;
                }
            }
        }           // -- end while loop

        //qDebug() << "-- j: " << j;
        //qDebug() << "guess: " << guess[0] << guess[1] << guess[2];
        quitFlag = 0;
        for (jj = 0; jj < 3; jj++)
        {
            ghr.coord[jj] = guess[jj];
            if ((guess[jj] >= 0.0) && (guess[jj] <= 1.0)) ++quitFlag;
        }

        tghr = fabs(guess[0]) + fabs(guess[1]) + fabs(guess[2]);
        if (tghr <= 1.0) ++quitFlag;

        returnValue = false;
        // There are 4 conditions that must be satisfed for a point to be inside a tet.
        if (quitFlag >= 4) returnValue = true;
        //qDebug() << "quitFlag is " << quitFlag;

        return returnValue;
    }

    // ------------------------------------------------------------------------
    bool SkdTree::isInside1stOrderPent(int pent, Point &point, Point &ghr, unsigned char eType)
    {
        float   deltas[3];
        float   guess[3];
        float   funcResults[3];
        float   dervResults[3][3];
        float   verts[6][3];
        float   tgh;
        float   tr[2];
        float   testValue;

        int     i, j;
        int     ii, jj, rr;
        int     quitFlag;

        bool    returnValue;

        //qDebug() << "Search point: " << point.coord[0] << point.coord[1] << point.coord[2];
        // Gather the element's node locations.
        if (eType == 5)
        {
            for (i = 0; i<6; i++)
            {
                j = (*p1elements)[pent].connect[i];
                verts[i][0] = (*nodes)[j].x;
                verts[i][1] = (*nodes)[j].y;
                verts[i][2] = (*nodes)[j].z;
                //if (iflag) qDebug() << "pent:: j: " << j << "\tx: " << verts[i][0] << "\ty: " << verts[i][1] << "\tz: " << verts[i][2];
            }
        }
        else
        {
            for (i = 0; i<6; i++)
            {
                j = (*p2elements)[pent].connect[i];
                verts[i][0] = (*nodes)[j].x;
                verts[i][1] = (*nodes)[j].y;
                verts[i][2] = (*nodes)[j].z;
                //qDebug() << "j: " << j << "x: " << verts[i][0] << "y: " << verts[i][1] << "z: " << verts[i][2];
            }
        }
        //print8x3Array(verts, 4);

        // Set the g,h,r guess.
        guess[0] = 0.0;  guess[1] = 0.0;  guess[2] = 0.0;

        j = quitFlag = 0;
        while ((j<20) && (quitFlag < 3))
        {
            // ------------------------------------------------------------------------------------
            // eval1stOrderPentFunc
            tgh   = 1.0 - guess[0] - guess[1];
            tr[0] = 1.0 - guess[2];
            tr[1] = 1.0 + guess[2];
            funcResults[0] = 0.0;  funcResults[1] = 0.0;  funcResults[2] = 0.0;
            // Loop over each direction.
            for (ii = 0; ii < 3; ii++)
            {
                jj = -1;
                for (rr = 0; rr < 2; rr++)
                {
                    jj++;
                    funcResults[ii] +=      tgh * tr[rr] * verts[jj][ii];
                    jj++;
                    funcResults[ii] += guess[0] * tr[rr] * verts[jj][ii];
                    jj++;
                    funcResults[ii] += guess[1] * tr[rr] * verts[jj][ii];
                }
                funcResults[ii] = point.coord[ii] - 0.5 * funcResults[ii];

                //qDebug() << "funcResults:: ii: " << ii << "\t" << funcResults[ii];
            }
            // ------------------------------------------------------------------------------------
            // eval1stOrderPentDerivatives
            // ---------------------------
            // Loop over each direction.
            for (ii = 0; ii < 3; ii++)
            {
                // Derivative w.r.t. the 1st master co-ordinate.  Put in column #1 (0).
                dervResults[ii][0]  = (1.0 - guess[2]) * (verts[1][ii] - verts[0][ii]);
                dervResults[ii][0] += (1.0 + guess[2]) * (verts[4][ii] - verts[3][ii]);
                dervResults[ii][0] *= 0.5;

                // Derivative w.r.t. the 2nd master co-ordinate.  Put in column #2 (1).
                dervResults[ii][1]  = (1.0 - guess[2]) * (verts[2][ii] - verts[0][ii]);
                dervResults[ii][1] += (1.0 + guess[2]) * (verts[5][ii] - verts[3][ii]);
                dervResults[ii][1] *= 0.5;

                // Derivative w.r.t. the 3rd master co-ordinate.  Put in column #3 (2).
                dervResults[ii][2]  = (1.0 - guess[0] - guess[1]) * (verts[3][ii] - verts[0][ii]);
                dervResults[ii][2] +=                   guess[0]  * (verts[4][ii] - verts[1][ii]);
                dervResults[ii][2] +=                   guess[1]  * (verts[5][ii] - verts[2][ii]);
                dervResults[ii][2] *= 0.5;
            }
            // ------------------------------------------------------------------------------------
            //qDebug() << " -- dervResults --";
            //print8x3Array(dervResults, 3);

            //ii = matrixInverse3x3(dervResults);

            //qDebug() << " -- inverse dervResults --";
            //print8x3Array(dervResults, 3);

            matMult3(dervResults, funcResults, deltas);

            //qDebug() << " return from matMult3\n";
            for (jj = 0; jj < 3; jj++)
            {
                guess[jj] += deltas[jj];
            }

            //if (iflag) qDebug() << "----Deltas: " << deltas[0] << "\t" << deltas[1] << " \t" << deltas[2];
            //if (iflag) qDebug() << "----guess : " << guess[0] << "\t" << guess[1] << " \t" << guess[2];

            quitFlag = 0;
            j++;
            for (jj = 0; jj < 3; jj++)
            {
                testValue = fabs(deltas[jj]);
                if (testValue < 1.0e-6)
                {
                    quitFlag += 1;
                }
                if (testValue > 10000.) quitFlag = 99;
            }
        }           // -- end while loop

        //if (iflag) qDebug() << "-- j: " << j;
        //if (iflag) qDebug() << "guess: " << guess[0] << guess[1] << guess[2];

        quitFlag = 0;
        for (jj = 0; jj < 2; jj++)
        {
            ghr.coord[jj] = guess[jj];
            if ((guess[jj] >= 0.0) && (guess[jj] <= 1.0)) ++quitFlag;
        }
        ghr.coord[2] = guess[2];

        if ((guess[2] >= -1.0) && (guess[2] <= 1.0)) ++quitFlag;
        testValue = fabs(guess[0]) + fabs(guess[1]);
        if (testValue <= 1.0) ++quitFlag;

        // There are 4 conditions that must be satisfed for a point to be inside a pent.
        returnValue = false;
        if (quitFlag >= 4) returnValue = true;
        //if (iflag) qDebug() << "returnValue quitFlag is " << quitFlag;

        return returnValue;
    }
