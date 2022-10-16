#include "umObject.h"

// ----------------------------------------------------------------------------
//UMObject::~UMObject()
//{
//}

void UMObject::createNewEdit(int op, int edit1, int edit2, double mult1, double mult2, QString name)
{
    int    i;
    double x;
    double thisMax;
    double thisMin;

    int nNodes = this->nodes.size();

    edit1--;
    edit2--;

    this->resultNames.push_back(name);
    thisMax = -DBL_MAX;
    thisMin =  DBL_MAX;

    switch(op)
    {
        case 0:     // -- scaling
            for(i=0; i < this->nEls[0]; i++)
            {
                x = mult1 * this->t1elements[i].results[edit1];
                this->t1elements[i].results.push_back(x);
                if (x > thisMax)
                {
                    thisMax = x;
                }
                if (x < thisMin)
                {
                    thisMin = x;
                }
            }
            for(i=0; i < this->nEls[1]; i++)
            {
                x = mult1 * this->p1elements[i].results[edit1];
                this->p1elements[i].results.push_back(x);
                if (x > thisMax)
                {
                    thisMax = x;
                }
                if (x < thisMin)
                {
                    thisMin = x;
                }
            }
            for(i=0; i < this->nEls[2]; i++)
            {
                x = mult1 * this->h1elements[i].results[edit1];
                this->h1elements[i].results.push_back(x);
                if (x > thisMax)
                {
                    thisMax = x;
                }
                if (x < thisMin)
                {
                    thisMin = x;
                }
            }
            for(i=0; i < this->nEls[3]; i++)
            {
                x = mult1 * this->t2elements[i].results[edit1];
                this->t2elements[i].results.push_back(x);
                if (x > thisMax)
                {
                    thisMax = x;
                }
                if (x < thisMin)
                {
                    thisMin = x;
                }
            }
            for(i=0; i < this->nEls[4]; i++)
            {
                x = mult1 * this->p2elements[i].results[edit1];
                this->p2elements[i].results.push_back(x);
                if (x > thisMax)
                {
                    thisMax = x;
                }
                if (x < thisMin)
                {
                    thisMin = x;
                }
            }
            for(i=0; i < this->nEls[5]; i++)
            {
                x = mult1 * this->h2elements[i].results[edit1];
                this->h2elements[i].results.push_back(x);
                if (x > thisMax)
                {
                    thisMax = x;
                }
                if (x < thisMin)
                {
                    thisMin = x;
                }
            }

            for(i = 0; i < nNodes; i++)
            {
                x = mult1 * this->nodes[i].results[edit1];
                this->nodes[i].results.push_back(x);
            }
            break;

        case 1:     // -- add
            for(i=0; i < this->nEls[0]; i++)
            {
                x = mult1 * this->t1elements[i].results[edit1] + (mult2 * this->t1elements[i].results[edit2]);
                this->t1elements[i].results.push_back(x);
                if (x > thisMax)
                {
                    thisMax = x;
                }
                if (x < thisMin)
                {
                    thisMin = x;
                }
            }
            for(i=0; i < this->nEls[1]; i++)
            {
                x = mult1 * this->p1elements[i].results[edit1] + (mult2 * this->p1elements[i].results[edit2]);
                this->p1elements[i].results.push_back(x);
                if (x > thisMax)
                {
                    thisMax = x;
                }
                if (x < thisMin)
                {
                    thisMin = x;
                }
            }
            for(i=0; i < this->nEls[2]; i++)
            {
                x = mult1 * this->h1elements[i].results[edit1] + (mult2 * this->h1elements[i].results[edit2]);
                this->h1elements[i].results.push_back(x);
                if (x > thisMax)
                {
                    thisMax = x;
                }
                if (x < thisMin)
                {
                    thisMin = x;
                }
            }
            for(i=0; i < this->nEls[3]; i++)
            {
                x = mult1 * this->t2elements[i].results[edit1] + (mult2 * this->t2elements[i].results[edit2]);
                this->t2elements[i].results.push_back(x);
                if (x > thisMax)
                {
                    thisMax = x;
                }
                if (x < thisMin)
                {
                    thisMin = x;
                }
            }
            for(i=0; i < this->nEls[4]; i++)
            {
                x = mult1 * this->p2elements[i].results[edit1] + (mult2 * this->p2elements[i].results[edit2]);
                this->p2elements[i].results.push_back(x);
                if (x > thisMax)
                {
                    thisMax = x;
                }
                if (x < thisMin)
                {
                    thisMin = x;
                }
            }
            for(i=0; i < this->nEls[5]; i++)
            {
                x = mult1 * this->h2elements[i].results[edit1] + (mult2 * this->h2elements[i].results[edit2]);
                this->h2elements[i].results.push_back(x);
                if (x > thisMax)
                {
                    thisMax = x;
                }
                if (x < thisMin)
                {
                    thisMin = x;
                }
            }

            for(i = 0; i < nNodes; i++)
            {
                x = mult1 * this->nodes[i].results[edit1] / (mult2 * this->nodes[i].results[edit2]);
                this->nodes[i].results.push_back(x);
            }
            break;

        case 2:     // -- subtract
            for(i=0; i < this->nEls[0]; i++)
            {
                x = mult1 * this->t1elements[i].results[edit1] - (mult2 * this->t1elements[i].results[edit2]);
                this->t1elements[i].results.push_back(x);
                if (x > thisMax)
                {
                    thisMax = x;
                }
                if (x < thisMin)
                {
                    thisMin = x;
                }
            }
            for(i=0; i < this->nEls[1]; i++)
            {
                x = mult1 * this->p1elements[i].results[edit1] - (mult2 * this->p1elements[i].results[edit2]);
                this->p1elements[i].results.push_back(x);
                if (x > thisMax)
                {
                    thisMax = x;
                }
                if (x < thisMin)
                {
                    thisMin = x;
                }
            }
            for(i=0; i < this->nEls[2]; i++)
            {
                x = mult1 * this->h1elements[i].results[edit1] - (mult2 * this->h1elements[i].results[edit2]);
                this->h1elements[i].results.push_back(x);
                if (x > thisMax)
                {
                    thisMax = x;
                }
                if (x < thisMin)
                {
                    thisMin = x;
                }
            }
            for(i=0; i < this->nEls[3]; i++)
            {
                x = mult1 * this->t2elements[i].results[edit1] - (mult2 * this->t2elements[i].results[edit2]);
                this->t2elements[i].results.push_back(x);
                if (x > thisMax)
                {
                    thisMax = x;
                }
                if (x < thisMin)
                {
                    thisMin = x;
                }
            }
            for(i=0; i < this->nEls[4]; i++)
            {
                x = mult1 * this->p2elements[i].results[edit1] - (mult2 * this->p2elements[i].results[edit2]);
                this->p2elements[i].results.push_back(x);
                if (x > thisMax)
                {
                    thisMax = x;
                }
                if (x < thisMin)
                {
                    thisMin = x;
                }
            }
            for(i=0; i < this->nEls[5]; i++)
            {
                x = mult1 * this->h2elements[i].results[edit1] - (mult2 * this->h2elements[i].results[edit2]);
                this->h2elements[i].results.push_back(x);
                if (x > thisMax)
                {
                    thisMax = x;
                }
                if (x < thisMin)
                {
                    thisMin = x;
                }
            }
            for(i = 0; i < nNodes; i++)
            {
                x = mult1 * this->nodes[i].results[edit1] - (mult2 * this->nodes[i].results[edit2]);
                this->nodes[i].results.push_back(x);
            }
            break;

        case 3:     // -- multiply
            for(i=0; i < this->nEls[0]; i++)
            {
                x = mult1 * this->t1elements[i].results[edit1] * (mult2 * this->t1elements[i].results[edit2]);
                this->t1elements[i].results.push_back(x);
                if (x > thisMax)
                {
                    thisMax = x;
                }
                if (x < thisMin)
                {
                    thisMin = x;
                }
            }
            for(i=0; i < this->nEls[1]; i++)
            {
                x = mult1 * this->p1elements[i].results[edit1] * (mult2 * this->p1elements[i].results[edit2]);
                this->p1elements[i].results.push_back(x);
                if (x > thisMax)
                {
                    thisMax = x;
                }
                if (x < thisMin)
                {
                    thisMin = x;
                }
            }
            for(i=0; i < this->nEls[2]; i++)
            {
                x = mult1 * this->h1elements[i].results[edit1] * (mult2 * this->h1elements[i].results[edit2]);
                this->h1elements[i].results.push_back(x);
                if (x > thisMax)
                {
                    thisMax = x;
                }
                if (x < thisMin)
                {
                    thisMin = x;
                }
            }
            for(i=0; i < this->nEls[3]; i++)
            {
                x = mult1 * this->t2elements[i].results[edit1] * (mult2 * this->t2elements[i].results[edit2]);
                this->t2elements[i].results.push_back(x);
                if (x > thisMax)
                {
                    thisMax = x;
                }
                if (x < thisMin)
                {
                    thisMin = x;
                }
            }
            for(i=0; i < this->nEls[4]; i++)
            {
                x = mult1 * this->p2elements[i].results[edit1] * (mult2 * this->p2elements[i].results[edit2]);
                this->p2elements[i].results.push_back(x);
                if (x > thisMax)
                {
                    thisMax = x;
                }
                if (x < thisMin)
                {
                    thisMin = x;
                }
            }
            for(i=0; i < this->nEls[5]; i++)
            {
                x = mult1 * this->h2elements[i].results[edit1] * (mult2 * this->h2elements[i].results[edit2]);
                this->h2elements[i].results.push_back(x);
                if (x > thisMax)
                {
                    thisMax = x;
                }
                if (x < thisMin)
                {
                    thisMin = x;
                }
            }


            for(i = 0; i < nNodes; i++)
            {
                x = mult1 * this->nodes[i].results[edit1] * (mult2 * this->nodes[i].results[edit2]);
                this->nodes[i].results.push_back(x);
            }
            break;

        case 4:     // -- divide

            for(i=0; i < this->nEls[0]; i++)
            {
                x = mult1 * this->t1elements[i].results[edit1] / (mult2 * this->t1elements[i].results[edit2]);
                this->t1elements[i].results.push_back(x);
                if (x > thisMax)
                {
                    thisMax = x;
                }
                if (x < thisMin)
                {
                    thisMin = x;
                }
            }
            for(i=0; i < this->nEls[1]; i++)
            {
                x = mult1 * this->p1elements[i].results[edit1] / (mult2 * this->p1elements[i].results[edit2]);
                this->p1elements[i].results.push_back(x);
                if (x > thisMax)
                {
                    thisMax = x;
                }
                if (x < thisMin)
                {
                    thisMin = x;
                }
            }
            for(i=0; i < this->nEls[2]; i++)
            {
                x = mult1 * this->h1elements[i].results[edit1] / (mult2 * this->h1elements[i].results[edit2]);
                this->h1elements[i].results.push_back(x);
                if (x > thisMax)
                {
                    thisMax = x;
                }
                if (x < thisMin)
                {
                    thisMin = x;
                }
            }
            for(i=0; i < this->nEls[3]; i++)
            {
                x = mult1 * this->t2elements[i].results[edit1] / (mult2 * this->t2elements[i].results[edit2]);
                this->t2elements[i].results.push_back(x);
                if (x > thisMax)
                {
                    thisMax = x;
                }
                if (x < thisMin)
                {
                    thisMin = x;
                }
            }
            for(i=0; i < this->nEls[4]; i++)
            {
                x = mult1 * this->p2elements[i].results[edit1] / (mult2 * this->p2elements[i].results[edit2]);
                this->p2elements[i].results.push_back(x);
                if (x > thisMax)
                {
                    thisMax = x;
                }
                if (x < thisMin)
                {
                    thisMin = x;
                }
            }
            for(i=0; i < this->nEls[5]; i++)
            {
                x = mult1 * this->h2elements[i].results[edit1] / (mult2 * this->h2elements[i].results[edit2]);
                this->h2elements[i].results.push_back(x);
                if (x > thisMax)
                {
                    thisMax = x;
                }
                if (x < thisMin)
                {
                    thisMin = x;
                }
            }
            for(i = 0; i < nNodes; i++)
            {
                x = mult1 * this->nodes[i].results[edit1] / (mult2 * this->nodes[i].results[edit2]);
                this->nodes[i].results.push_back(x);
            }
            break;

    }   // -- end switch

    this->resultMax.push_back(thisMax);
    if (thisMin < DBL_MIN)  thisMin = DBL_MIN;
    this->resultMin.push_back(thisMin);
    this->nResults++;
}                       // -- end createNewEdit
// ------------------------------------------------------------------------------------------------
int UMObject::findElementData(int elNo, int &inst, int &matNo, short &eType)
{
    // Given the global element number (elNo),
    // return the instance number (to which it belongs), its element type, and its material number.

    short left;
    short right;
    int   elNoAdjust;

    elNoAdjust = elNo + 1;
   // elNoAdjust = elNo;

    for (eType=0; eType<6; ++eType)
    {
        left  = instanceCodesLeft[eType];
        right = instanceCodesRight[eType];
        for (inst=0; inst<nInstances; ++inst)
        {
            //qDebug() << inst << "  limits: " << instances[inst].elementLimits[left] << instances[inst].elementLimits[right];

            if ((elNoAdjust >= instances[inst].elementLimits[left]) && (elNoAdjust <= instances[inst].elementLimits[right]))
            {
                matNo = findMaterialNumber(elNo, eType);
                return 0;
            }
        }
    }
    //qDebug() << "-- instance number not found: " << eType << "\telNO: " << elNo << " left: " << left << " right: " << right;
    return -1;
}
// ------------------------------------------------------------------------------------------------
bool UMObject::isElementDisplayed(int elNo, int colorChoice)
{
    // Given just the element number determined if it is to be displayed based on a number of criteria.
    int lel;        // local element number
    int inst;
    int matNo;
    short eType;
    bool retValue;
    bool masterDisplay;

    retValue = false;
    if (findElementData(elNo, inst, matNo, eType) > -1)
    {
        if (colorChoice == 1)  masterDisplay = this->instances[inst].display;
        else  masterDisplay = this->materials[matNo].display;

        //qDebug() << "elNo: " << elNo << "\tinst: " << inst << "\teType:" << eType << "\tmatNo: "
        //         << matNo << "\t master: " << masterDisplay;


        switch(eType)
        {
            case 0:                 // -- 1st tets
                lel = elNo - elAdjust[0];
                if (masterDisplay && this->t1elements[lel].displayElement)  retValue = true;
                break;

            case 1:                 // -- 1st pents
                lel = elNo - elAdjust[1];
                if (masterDisplay && this->p1elements[lel].displayElement)  retValue = true;
                break;

            case 2:                 // -- 1st hexs
                lel = elNo - elAdjust[2];
                if (masterDisplay && this->h1elements[lel].displayElement)  retValue = true;
                break;

            case 3:                 // -- 2nd tets
                lel = elNo - elAdjust[3];
                if (masterDisplay && this->t2elements[lel].displayElement)  retValue = true;
                break;

            case 4:                 // -- 2nd pents
                lel = elNo - elAdjust[4];
                if (masterDisplay && this->p2elements[lel].displayElement)  retValue = true;
                break;

            case 5:                 // -- 2nd hexs
                lel = elNo - elAdjust[5];
                if (masterDisplay && this->h2elements[lel].displayElement)  retValue = true;
                break;
            }
    }
            //if (retValue) qDebug() << "elNo: " << elNo << "\t retValue: " << retValue;
    return retValue;
}
// ------------------------------------------------------------------------------------------------
int UMObject::findMaterialNumber(int elNo, short eType)
{
    int k, matNo;

    switch(eType)
    {
        case 0:                 // -- 1st tets
            k = elNo - elAdjust[0];
            //qDebug() << "k: " << k << "\telNo: " << elNo << "\t elAdjust: " << elAdjust[0];
            //qDebug() << "t1elements: " << t1elements.size();
            matNo  = this->t1elements[k].material;
            break;
        case 1:                 // -- 1st pents
            k = elNo - elAdjust[1];
            matNo  = this->p1elements[k].material;
            break;
        case 2:                 // -- 1st hexs
            k = elNo - elAdjust[2];
            matNo  = this->h1elements[k].material;
            break;
        case 3:                 // -- 2nd tets
            k = elNo - elAdjust[3];
            matNo  = this->t2elements[k].material;
            break;
        case 4:                 // -- 2nd pents
            k = elNo - elAdjust[4];
            matNo  = this->p2elements[k].material;
            break;
        case 5:                 // -- 2nd hexs
            k = elNo - elAdjust[5];
            matNo  = this->h2elements[k].material;
            break;
    }
    matNo  = this->matMap[matNo];
    return matNo;
}
// ------------------------------------------------------------------------------------------------
bool UMObject::retreiveCentroids(int elNo, short eType, double *cent)
{
    int k;

    switch(eType)
    {
        case 0:                 // -- 1st tets
            k = elNo - elAdjust[0];
            //qDebug() << "k: " << k << "\telNo: " << elNo << "\t elAdjust: " << elAdjust[0];
            //qDebug() << "t1elements: " << t1elements.size();
            cent[0]  = this->t1elements[k].centroid[0];
            cent[1]  = this->t1elements[k].centroid[1];
            cent[2]  = this->t1elements[k].centroid[2];
            break;
        case 1:                 // -- 1st pents
            k = elNo - elAdjust[1];
            cent[0]  = this->p1elements[k].centroid[0];
            cent[1]  = this->p1elements[k].centroid[1];
            cent[2]  = this->p1elements[k].centroid[2];
            break;
        case 2:                 // -- 1st hexs
            k = elNo - elAdjust[2];
            cent[0]  = this->h1elements[k].centroid[0];
            cent[1]  = this->h1elements[k].centroid[1];
            cent[2]  = this->h1elements[k].centroid[2];
            break;
        case 3:                 // -- 2nd tets
            k = elNo - elAdjust[3];
            cent[0]  = this->t2elements[k].centroid[0];
            cent[1]  = this->t2elements[k].centroid[1];
            cent[2]  = this->t2elements[k].centroid[2];
            break;
        case 4:                 // -- 2nd pents
            k = elNo - elAdjust[4];
            cent[0]  = this->p2elements[k].centroid[0];
            cent[1]  = this->p2elements[k].centroid[1];
            cent[2]  = this->p2elements[k].centroid[2];
            break;
        case 5:                 // -- 2nd hexs
            k = elNo - elAdjust[5];
            cent[0]  = this->h2elements[k].centroid[0];
            cent[1]  = this->h2elements[k].centroid[1];
            cent[2]  = this->h2elements[k].centroid[2];
            break;
        default:
            return false;
    }
    return true;
}
// ------------------------------------------------------------------------------------------------
int UMObject::findElementType(int elNo, int &inst, short &eType)
{
    // Given the global element number (elNo),
    // return the instance number (to which it belongs), its element type, and its material number.

    short left;
    short right;
    int   elNoAdjust;

    elNoAdjust = elNo + 1;

    for (eType=0; eType<6; ++eType)
    {
        left  = instanceCodesLeft[eType];
        right = instanceCodesRight[eType];
        for (inst=0; inst<nInstances; ++inst)
        {
            //qDebug() << inst << "  limits: " << instances[inst].elementLimits[left] << instances[inst].elementLimits[right];

            if ((elNoAdjust >= instances[inst].elementLimits[left]) && (elNoAdjust <= instances[inst].elementLimits[right]))
            {
                return 0;
            }
        }
    }
    //qDebug() << "-- instance number not found: " << eType << "\telNO: " << elNo << " left: " << left << " right: " << right;
    return -1;
}

// ------------------------------------------------------------------------------------------------

void UMObject::createIsoGrid(int nx, int ny, int nz)
{
    int   i, j, k, m;
    float cox, coy, coz;

    xResValueIso = nx;
    yResValueIso = ny;
    zResValueIso = nz;
    deltaX = extentsX / ((float) xResValueIso);
    deltaY = extentsY / ((float) yResValueIso);
    deltaZ = extentsZ / ((float) zResValueIso);

    /*qDebug() << " --- createIsoGrid called ---";
    qDebug() << "xResValueIso is " << xResValueIso << "\textents X: " << extentsX;
    qDebug() << "yResValueIso is " << yResValueIso << "\textents Y: " << extentsY;
    qDebug() << "zResValueIso is " << zResValueIso << "\textents Z: " << extentsZ; */

    // Create and store the grid values.
    isoXmesh.clear();
    isoYmesh.clear();
    isoZmesh.clear();

    for (k=0; k <= zResValueIso; k++)
    {
        coz = nodesMinZ + deltaZ * ((float) k);
        isoZmesh.push_back(coz);
    }
    for (j=0; j <= yResValueIso; j++)
    {
        coy = nodesMinY + deltaY * ((float) j);
        isoYmesh.push_back(coy);
    }
    for (i=0; i<= xResValueIso; i++)
    {
        cox = nodesMinX + deltaX * ((float) i);
        isoXmesh.push_back(cox);
    }

    // -------------------------------------------------
    // Create the new look-up grid for all iso-surfaces.
    IsoPoint iso;
    isoGrid.clear();
    nx++; ny++; nz++;
    isoGrid.reserve(nx*ny*nz);

    m = 0;
    for (k=0; k <= zResValueIso; k++)
    {
        coz = isoZmesh[k];
        iso.z = coz;
        for (j=0; j <= yResValueIso; j++)
        {
            coy = isoYmesh[j];
            iso.y = coy;
            for (i=0; i<= xResValueIso; i++)
            {
                cox = isoXmesh[i];
                iso.x = cox;
                //qDebug() << "\n -- createIsoGrid --";
                //qDebug() << "cox coy coz: " << cox << coy << coz;
                bool iflag = false;
                if (m == 11) iflag = true;
                m++;
                setupIsoGridPoint(cox, coy, coz, iso);
                if (iso.elNo < 0)
                {
                    iso.g = iso.h = iso.r = -9.;
                }

                isoGrid.push_back(iso);
            }
        }
    }           // -- end k-loop
}
// ------------------------------------------------------------------------------------------------

int UMObject::setupIsoGridPoint(float cox, float coy, float coz, IsoPoint &iso)
{
    size_t        i;
    int           inst, gEl;
    bool          flag;
    Point         test, ghr;
    unsigned char eType;

    inst     = -1;
    iso.elNo = -1;
    flag     = false;

    for (i=0; i < instances.size(); i++)
    {
        if ((coz >= instances[i].bounds.box[2]) && (coz <= instances[i].bounds.box[5]))
        {
            if ((coy >= instances[i].bounds.box[1]) && (coy <= instances[i].bounds.box[4]))
            {
                if ((cox >= instances[i].bounds.box[0]) && (cox <= instances[i].bounds.box[3]))
                {
                    inst = i;
                    test.coord[0] = cox;  test.coord[1] = coy;  test.coord[2] = coz;
                    iso.elNo  = instances[i].tree->findInstanceElement(test, ghr, gEl, eType);
                    iso.g     = ghr.coord[0];
                    iso.h     = ghr.coord[1];
                    iso.r     = ghr.coord[2];
                    iso.gEl   = gEl;
                    iso.eType = eType;
                    if (iso.elNo > -1) break;
                }
            }
        }
    }               // -- end i-loop
    return inst;
}
// ------------------------------------------------------------------------------------------------
void UMObject::setupIsoGridPoint(IsoPoint &iso, bool iflag, bool pFlag)
{
    size_t        i;
    int           gEl;
    //bool          flag;
    Point         test, ghr;
    unsigned char eType;

    iso.elNo = -1;
    //flag     = false;
    if (pFlag)
    {
        qDebug() << "setupIsoGridPoint:: x: " << iso.x << "\t y: " << iso.y << "\tz: " << iso.z;
    }

    for (i=0; i < instances.size(); i++)
    {
        if ((iso.z >= instances[i].bounds.box[2]) && (iso.z <= instances[i].bounds.box[5]))
        {
            if ((iso.y >= instances[i].bounds.box[1]) && (iso.y <= instances[i].bounds.box[4]))
            {
                if ((iso.x >= instances[i].bounds.box[0]) && (iso.x <= instances[i].bounds.box[3]))
                {
                    if (pFlag)
                    {
                        qDebug() << "setupIsoGridPoint:: instance: " << i;
                        qDebug() << "X: " << instances[i].bounds.box[0] << "\t" << instances[i].bounds.box[3];
                        qDebug() << "Y: " << instances[i].bounds.box[1] << "\t" << instances[i].bounds.box[4];
                        qDebug() << "Z: " << instances[i].bounds.box[2] << "\t" << instances[i].bounds.box[5];
                    }
                    test.coord[0] = iso.x;  test.coord[1] = iso.y;  test.coord[2] = iso.z;
                    iso.elNo  = instances[i].tree->findInstanceElement(test, ghr, gEl, eType, pFlag);
                    iso.g     = ghr.coord[0];
                    iso.h     = ghr.coord[1];
                    iso.r     = ghr.coord[2];
                    iso.gEl   = gEl;
                    iso.eType = eType;
                    if (iso.elNo > -1) break;
                }
            }
        }
    }
}

// ------------------------------------------------------------------------------------------------
void UMObject::createIsoSurface(int whichPlot, float isoValue, bool doGrid, IsoObject &isoObject)
{
    int i, j, k, m, n, q;
    int instNo;
    int numTris;
    int hashIndexY;
    int hashIndexZ;
    float cox, coy, coz;
    unsigned char vCount;

    GRIDCELL  myGrid;
    TRIANGLE  tris[5];
    Point3    tempTri;

   /* qDebug() << "createIsoSurface:: which plot: " << whichPlot;
    qDebug() << "isoObject value is " << isoValue;
    qDebug() << "isoGrid  size is " << isoGrid.size();
    qDebug() << "isoXmesh size is " << isoXmesh.size();
    qDebug() << "isoYmesh size is " << isoYmesh.size();
    qDebug() << "isoZmesh size is " << isoZmesh.size();
    qDebug() << "isoGrid  size is " << isoGrid.size();
    qDebug() << "deltaX is " << deltaX;
    qDebug() << "deltaY is " << deltaY;
    qDebug() << "deltaZ is " << deltaZ;
    qDebug() << "nodesMinX is " << nodesMinX;
    qDebug() << "nodesMinY is " << nodesMinY;
    qDebug() << "nodesMinZ is " << nodesMinZ; */

    // ------------------------------
    // Initialize the isoList vector.
    i = xResValueIso * yResValueIso * zResValueIso + 1;
    //QVector<int> isoList(i, -1);
    //qDebug() << "isoList size is " << isoList.size();

    boxesPerPlane  = xResValueIso * yResValueIso;
    pointsPerPlane = (xResValueIso + 1) * (yResValueIso + 1);

    //qDebug() << "boxesPerPlane size is " << boxesPerPlane;

    // ------------------------------------------------------------------------
    // Generate the values at the grid points.
    if (doGrid)
    {
        for (i = 0; i < (int) isoGrid.size(); i++)
        {
            if (isoGrid[i].elNo > -1)
            {
                isoGrid[i].valid = true;
                if ((isoGrid[i].eType == 4) || (isoGrid[i].eType == 14))
                {
                    isoGrid[i].value = eval1stOrderTetResults(whichPlot, isoGrid[i]);
                }
                else if ((isoGrid[i].eType == 6) || (isoGrid[i].eType == 16))
                {
                    isoGrid[i].value = eval1stOrderHexResults(whichPlot, isoGrid[i]);
                }
                else if ((isoGrid[i].eType == 5) || (isoGrid[i].eType == 15))
                {
                    //bool iflag = false;
                    //if (i == 0) iflag = true;
                    isoGrid[i].value = eval1stOrderPentResults(whichPlot, isoGrid[i]);
                }

                //if (isoGrid[i].elNo > 6) fprintf(fp2, "%4d\t %d\t %f\n", i, isoGrid[i].elNo, isoGrid[i].value);

                //int k = i;
                //if (isoGrid[i].value > .199)
                /*if (i < 606)
                {
                fprintf(fp2,"  %5d\t%8d\t%7.4f\t%7.4f\t%7.4f \t%9.3f\t%9.3f\t%9.3f\t %e\n", k, isoGrid[k].elNo, isoGrid[k].g, isoGrid[k].h, isoGrid[k].r,
                isoGrid[k].x, isoGrid[k].y, isoGrid[k].z, isoGrid[i].value);
                }*/
            }
            else
            {
                isoGrid[i].value = 0.0;
                isoGrid[i].valid = false;
            }
        }       // end i-loop
    }           // end doGrid

    isoObject.triangles.clear();

    // -- Testing -------------------------------------------------------------
//    Point    tmpPoint;
    IsoPoint dummyIso;

    for (k = 0; k < zResValueIso; k++)
    {
        // Set the Z-position
        hashIndexZ = k * pointsPerPlane;

        myGrid.p[0].z = myGrid.p[1].z = myGrid.p[2].z = myGrid.p[3].z = isoZmesh[k];
        myGrid.p[4].z = myGrid.p[5].z = myGrid.p[6].z = myGrid.p[7].z = isoZmesh[k+1];

        //qDebug() << "low Z  " << isoZmesh[k] << "\t high Z: " << isoZmesh[k+1] << "\t k: " << k<< "\thashIndexZ is " << hashIndexZ;
        // --------------------------------------------------------------------
        // Sweep the (Y) j-rows
        for (j = 0; j < yResValueIso; j++)
        {
            hashIndexY = j * (xResValueIso + 1);

            myGrid.p[2].y = myGrid.p[3].y = myGrid.p[6].y = myGrid.p[7].y = isoYmesh[j];
            myGrid.p[0].y = myGrid.p[1].y = myGrid.p[4].y = myGrid.p[5].y = isoYmesh[j+1];

            //qDebug() << "\nlow Y  " << isoYmesh[j] << "\t high Y: " << isoYmesh[j+1] << "\t j: " << j<< "\thashIndexY is " << hashIndexY;

            // Look at the X along this row.
            for (i = 0; i < xResValueIso; i++)
            {
                vCount = 0;
                //qDebug() << "low X  " << isoXmesh[i] << "\t high X: " << isoXmesh[i+1] << "\t i: " << i;
                myGrid.p[0].x = myGrid.p[3].x = myGrid.p[4].x = myGrid.p[7].x = isoXmesh[i];
                myGrid.p[1].x = myGrid.p[2].x = myGrid.p[5].x = myGrid.p[6].x = isoXmesh[i+1];

                n = m = hashIndexZ + hashIndexY + i;
                //qDebug() << "m: " << m;
                myGrid.val[3] = isoGrid[m].value;
                if (isoGrid[m].valid) ++vCount;
                m++;
                //qDebug() << "m: " << m;
                myGrid.val[2] = isoGrid[m].value;
                if (isoGrid[m].valid) ++vCount;

                m += (xResValueIso + 1);
                //qDebug() << "m: " << m;
                myGrid.val[1] = isoGrid[m].value;
                if (isoGrid[m].valid) ++vCount;
                m--;
                //qDebug() << "m: " << m;
                myGrid.val[0] = isoGrid[m].value;
                if (isoGrid[m].valid) ++vCount;

                n += pointsPerPlane;
                //qDebug() << "n: " << n;
                myGrid.val[7] = isoGrid[n].value;
                if (isoGrid[n].valid) ++vCount;
                n++;
                //qDebug() << "n: " << n;
                myGrid.val[6] = isoGrid[n].value;
                if (isoGrid[n].valid) ++vCount;

                n += (xResValueIso + 1);
                //qDebug() << "n: " << n;
                myGrid.val[5] = isoGrid[n].value;
                if (isoGrid[n].valid) ++vCount;
                n--;
                //qDebug() << "n: " << n;
                myGrid.val[4] = isoGrid[n].value;
                if (isoGrid[n].valid) ++vCount;

                //qDebug() << "posPts: " << isoObject.posPts;

                if (vCount > (isoObject.posPts - 1))
                {
                    numTris = myGrid.Polygonise(isoValue, tris);

                    /*if ((i < 5) && (j < 1) && (k < 3))
                    {
                        fprintf(fp2, "\ni: %d\t j: %d\t k: %d\t hashY: %d\t hashZ: %d\n", i, j, k, hashIndexY, hashIndexZ);
                        fprintf(fp2, "numTris: %d\n", numTris);
                        for (n=0; n<8; n++)
                        {
                            fprintf(fp2, "\tmyGrid.val[%d]: %e\n", n, myGrid.val[n]);
                        }
                    }*/

                    //if (numTris > 0) qDebug() << "Number of triangles: " << numTris;
                    for (n=0; n < numTris; n++)
                    {
                        cox = coy = coz = 0.0;
                        for (q = 0, m = 0; m < 3; m++, q +=3)
                        {
                            tempTri.coord[q]   = tris[n].p[m].x;
                            tempTri.coord[q+1] = tris[n].p[m].y;
                            tempTri.coord[q+2] = tris[n].p[m].z;
                            cox               += tris[n].p[m].x;
                            coy               += tris[n].p[m].y;
                            coz               += tris[n].p[m].z;
                        }
                        isoObject.triangles.push_back(tempTri);
                        cox / 3.0;
                        coy / 3.0;
                        coz / 3.0;
                        instNo = setupIsoGridPoint(cox, coy, coz, dummyIso);
                        // Store the instance number cooresponding to the center of thr triangle.
                        isoObject.instances.push_back(instNo);
                    }
                }               // -- end if for vCount
            }                   // -- end for loop over X (i-loop)
        }                       // -- end for loop over Y (j-loop)
    }                           // -- end for loop over Z (k-loop)

    /*
    for (i=0; i<isoObject.triangles.size(); i++)
    {
        qDebug() << "i: " << i;
        qDebug() << "\t Point: " << isoObject.triangles[i].coord[0] << "\t"
                                 << isoObject.triangles[i].coord[1] << "\t"
                                 << isoObject.triangles[i].coord[2];

        qDebug() << "\t Point: " << isoObject.triangles[i].coord[3] << "\t"
                                 << isoObject.triangles[i].coord[4] << "\t"
                                 << isoObject.triangles[i].coord[5];

        qDebug() << "\t Point: " << isoObject.triangles[i].coord[6] << "\t"
                                 << isoObject.triangles[i].coord[7] << "\t"
                                 << isoObject.triangles[i].coord[8];


         fprintf(fp2, "Isosurface triangle: %d\n",i);
         fprintf(fp2, "\t Point: %f\t %f\t %f\t  %f\t %f\t %f\t  %f\t %f\t %f\n",
                 isoObject.triangles[i].coord[0], isoObject.triangles[i].coord[1], isoObject.triangles[i].coord[2],
                 isoObject.triangles[i].coord[3], isoObject.triangles[i].coord[4], isoObject.triangles[i].coord[5],
                 isoObject.triangles[i].coord[6], isoObject.triangles[i].coord[7], isoObject.triangles[i].coord[8]);
    } */

    //qDebug() << "isoObject triangle size is " << isoObject.triangles.size();

}                               // -- end of function createIsoSurface

// ----------------------------------------------------------------------------
double  UMObject::eval1stOrderHexResults(int whichPlot, IsoPoint &isoGrid)
{
    int   node;
    float u;
    float coef[8];

    coef[0] = (1.0 - isoGrid.g) * (1.0 - isoGrid.h) * (1.0 - isoGrid.r);
    coef[1] = (1.0 + isoGrid.g) * (1.0 - isoGrid.h) * (1.0 - isoGrid.r);
    coef[2] = (1.0 + isoGrid.g) * (1.0 + isoGrid.h) * (1.0 - isoGrid.r);
    coef[3] = (1.0 - isoGrid.g) * (1.0 + isoGrid.h) * (1.0 - isoGrid.r);
    coef[4] = (1.0 - isoGrid.g) * (1.0 - isoGrid.h) * (1.0 + isoGrid.r);
    coef[5] = (1.0 + isoGrid.g) * (1.0 - isoGrid.h) * (1.0 + isoGrid.r);
    coef[6] = (1.0 + isoGrid.g) * (1.0 + isoGrid.h) * (1.0 + isoGrid.r);
    coef[7] = (1.0 - isoGrid.g) * (1.0 + isoGrid.h) * (1.0 + isoGrid.r);

    //qDebug() << "eval1stOrderHexResults:: isoGrid.eType" << isoGrid.eType;

    u = 0.0;
    if (isoGrid.eType == 6)
    {
        //qDebug() << "isoGrid.elNo:" << isoGrid.elNo << "whichPlot:" << whichPlot;

        for (int i=0; i< 8; i++)
        {
            node = h1elements[isoGrid.elNo].connect[i];
            u   += coef[i] * nodes[node].results[whichPlot];
        }
    }
    else
    {
        //qDebug() << "isoGrid.elNo:" << isoGrid.elNo << "\tgEl:" << isoGrid.gEl << "\t whichPlot: " << whichPlot;

        for (int i=0; i< 8; i++)
        {
            node = h2elements[isoGrid.elNo].connect[i];
            u   += coef[i] * nodes[node].results[whichPlot];
            //if (isoGrid.gEl == 211)
            //{
            //    qDebug() << "eval1stOrderHexResults:: 211: node results: " << nodes[node].results[whichPlot] << "\t node" << node;
            //}
        }
    }
    u *= 0.125;
    //if (isoGrid.gEl == 211)
    //{
    //    qDebug() << "final average: " << u;
    //}

    return u;
}

// ----------------------------------------------------------------------------
double  UMObject::eval1stOrderTetResults(int whichPlot, IsoPoint &isoGrid)
{
    int   node;
    float u;
    float coef[4];

    coef[0] = (1.0 - isoGrid.g - isoGrid.h - isoGrid.r);
    coef[1] = isoGrid.g;
    coef[2] = isoGrid.h;
    coef[3] = isoGrid.r;

    //qDebug() << "eval1stOrderTetResults:: elNo:" << isoGrid.elNo << "\t g: " << coef[1] << "\th: " << coef[2] << "\tr" << coef[3];

    u = 0.0;
    if (isoGrid.eType == 4)
    {
        for (int i=0; i< 4; i++)
        {
            node = t1elements[isoGrid.elNo].connect[i];
            u   += coef[i] * nodes[node].results[whichPlot];
            //if (isoGrid.elNo == 538)
            //{
            //    qDebug() << "node: " << node << "\tcoef: " << coef[i] << "\t result: " << nodes[node].results[whichPlot]
           //             << "\tu: " << u;
            //}
        }

        if (u < 0.0)
        {
            //qDebug() << "g: " << isoGrid.g << "\t h: " << isoGrid.h << "\t r: " << isoGrid.r;
            for (int i=0; i< 4; i++)
            {
                node = t1elements[isoGrid.elNo].connect[i];
                //qDebug() << "i: " << i << "\t node: " << node << "\tcoef:" << coef[i]
                //        << "\t result: " << nodes[node].results[whichPlot] << "\tu: " << u;
            }
        }
    }
    else
    {
        for (int i=0; i< 4; i++)
        {
            node = t2elements[isoGrid.elNo].connect[i];
            u   += coef[i] * nodes[node].results[whichPlot];
        }
    }
    return u;
}

// ----------------------------------------------------------------------------
double  UMObject::eval1stOrderPentResults(int whichPlot, IsoPoint &isoGrid)
{
    int   node;
    float u;
    float coef[6];
    float omgh;
    float omr;
    float opr;

    omgh = 1.0 - isoGrid.g - isoGrid.h;
    omr  = 1.0 - isoGrid.r;
    opr  = 1.0 + isoGrid.r;

    coef[0] = omgh * omr;
    coef[1] = omr  * isoGrid.g;
    coef[2] = omr  * isoGrid.h;
    coef[3] = omgh * opr;
    coef[4] = opr  * isoGrid.g;
    coef[5] = opr  * isoGrid.h;

    u = 0.0;
    if (isoGrid.eType == 5)
    {
        for (int i=0; i < 6; i++)
        {
            node = p1elements[isoGrid.elNo].connect[i];
            u   += coef[i] * nodes[node].results[whichPlot];
        }
    }
    else
    {
        for (int i=0; i < 6; i++)
        {
            node = p2elements[isoGrid.elNo].connect[i];
            u   += coef[i] * nodes[node].results[whichPlot];
        }
    }
    u *= 0.5;
    return u;
}
// ------------------------------------------------------------------------------------------------
void UMObject::findLineOutvalues(int pType, vector<IsoPoint> &lineOutData)
{
    int  i;
    int  loSize;
    bool iflag = false;
    bool pFlag = false;

    loSize = lineOutData.size();
    for(i = 0; i<loSize; i++)
    {
        //qDebug() << "findLineOutvalues:: " << i;
        //if (i == 94) pFlag = true;
        //else pFlag = false;
        setupIsoGridPoint(lineOutData[i], iflag, pFlag);
        if (lineOutData[i].elNo > -1)
        {
            lineOutData[i].valid = true;
            if ((lineOutData[i].eType == 4) || (lineOutData[i].eType == 14))
            {
                lineOutData[i].value = eval1stOrderTetResults(pType, lineOutData[i]);
            }
            else if ((lineOutData[i].eType == 6) || (lineOutData[i].eType == 16))
            {
                lineOutData[i].value = eval1stOrderHexResults(pType, lineOutData[i]);
            }
            else if ((lineOutData[i].eType == 5) || (lineOutData[i].eType == 15))
            {
                isoGrid[i].value = eval1stOrderPentResults(pType, lineOutData[i]);
            }
        }
        else
        {
            lineOutData[i].value = 0.0;
            lineOutData[i].valid = false;
        }
    }           // -- end i-loop over lineOutData
}

// ------------------------------------------------------------------------------------------------
void UMObject::findAllPointValues(IsoPoint &thisData, vector<float> &results)
{
    int  i;
    bool iflag = false;
    bool pFlag = false;

    setupIsoGridPoint(thisData, iflag, pFlag);


    //qDebug() << "findAllPointValues:: elNo" << thisData.elNo;

    if (thisData.elNo > -1)
    {
        results.clear();
        thisData.valid = true;
        for(i=0; i < this->nResults; ++i)
        {
            if ((thisData.eType == 4) || (thisData.eType == 14))
            {
                thisData.value = eval1stOrderTetResults(i, thisData);
            }
            else if ((thisData.eType == 6) || (thisData.eType == 16))
            {
                thisData.value = eval1stOrderHexResults(i, thisData);
            }
            else if ((thisData.eType == 5) || (thisData.eType == 15))
            {
                thisData.value = eval1stOrderPentResults(i, thisData);
            }
            results.push_back( (float) thisData.value );
        }
    }
    else
    {
        thisData.value = 0.0;
        thisData.valid = false;
    }
}           // -- end findAllPointValues
// ------------------------------------------------------------------------------------------------
void UMObject::processSectorPointValues(Point &pt, vector<float> &results, int &elNo, bool &isValid, bool pFlag)
{
    int             i;
    //int             elNo;
    int             nResults;
    Point           ghr;
    float           tmp;
    IsoPoint        iso;

    pFlag = false;

    nResults = this->resultNames.size();
    //qDebug() << "UMObject::processSectorPointValues" << nResults;
    //for (i=0; i<nResults; ++i)
    //{
    //    qDebug() << "resultNames: " << resultNames[i];
    //}

    findPointGHRetc(pt, ghr, iso.elNo, iso.gEl, iso.eType, pFlag);
    elNo = iso.gEl;

    if (pFlag)
    {
        qDebug() << "\n** processSectorPointValues **"
                 << "\npt : " << pt.coord[0] << "\t" << pt.coord[1] << "\t" << pt.coord[2]
                 << "\nghr: " << ghr.coord[0] << "\t" << ghr.coord[1] << "\t" << ghr.coord[2]
                 << "\nelNo: " << iso.elNo   << "gEl: " << iso.gEl;
    }


    // Encode the element number and store in the results for the geometry.
    tmp = (float) iso.elNo;
    results.push_back(tmp);

    iso.g = ghr.coord[0];
    iso.h = ghr.coord[1];
    iso.r = ghr.coord[2];

    // Find the results values for all known edits.
    for (i=0; i<nResults; ++i)
    {
        findSectorPointValue(i, iso);
        results.push_back(iso.value);
    }
}

// ------------------------------------------------------------------------------------------------
void UMObject::findSectorPointValue(int whichPlot, IsoPoint &isoData)
{
    if (isoData.elNo > -1)
    {
        isoData.valid = true;
        if ((isoData.eType == 4) || (isoData.eType == 14))
        {
            isoData.value = eval1stOrderTetResults(whichPlot, isoData);
        }
        else if ((isoData.eType == 6) || (isoData.eType == 16))
        {
            isoData.value = eval1stOrderHexResults(whichPlot, isoData);
        }
        else if ((isoData.eType == 5) || (isoData.eType == 15))
        {
            isoData.value = eval1stOrderPentResults(whichPlot, isoData);
        }
    }
    else
    {
        isoData.value = 0.0;
        isoData.valid = false;
    }
}

// ------------------------------------------------------------------------------------------------
void UMObject::findPointGHRetc(Point &pt, Point &ghr, int &elNo, int &gEl, unsigned char &eType, bool pFlag)
{
    // elNo  -- local element number for the instance
    // gEl   -- global element number for the umObject
    size_t i;

    elNo = -1;
    gEl  = 0;
    ghr.coord[0] = -99.;
    ghr.coord[1] = -99.;
    ghr.coord[2] = -99.;

    if (pFlag)
    {
        qDebug() << "\n Entering findPointGHRetc \n";
    }

    for (i=0; i < instances.size(); ++i)
    {
        if (pFlag)
        {
            qDebug() << i << "\tcoord[2] " << pt.coord[2] <<
                        "\t bounds: " << instances[i].bounds.box[2] << instances[i].bounds.box[5];
        }
        if ((pt.coord[2] >= instances[i].bounds.box[2]) && (pt.coord[2] <= instances[i].bounds.box[5]))
        {
            if (pFlag)
            {
                qDebug() << i << "\tcoord[1] " << pt.coord[1] <<
                            "\t bounds: " << instances[i].bounds.box[1] << instances[i].bounds.box[4];
            }
            if ((pt.coord[1] >= instances[i].bounds.box[1]) && (pt.coord[1] <= instances[i].bounds.box[4]))
            {
                if (pFlag)
                {
                    qDebug() << i << "\tcoord[0] " << pt.coord[0] <<
                                "\t bounds: " << instances[i].bounds.box[0] << instances[i].bounds.box[3];
                }
                if ((pt.coord[0] >= instances[i].bounds.box[0]) && (pt.coord[0] <= instances[i].bounds.box[3]))
                {
                    elNo  = instances[i].tree->findInstanceElement(pt, ghr, gEl, eType, pFlag) ;
                    if (elNo > -1) break;
                }
            }
        }
    }
    if (pFlag)
    {
        qDebug() << "elNo: " << elNo;
        qDebug() << " Exiting findPointGHRetc \n";
    }
}
// ----------------------------------------------------------------------------
void UMObject::generateHistogramBins(int nBins, int whichEdit, int intervalType)
{
    int i, j, k, n;

    double  tValue, delta1, delta2;
    double  objMinValue, instMinValue;
    int     *hData,    *hDataAll;

    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Set the histogram upper bin limits.
    double *limits1, *limits2;
    limits1 = new double[nBins];
    limits2 = new double[nBins];

    // Do the umObject upper bin limits.
    if (intervalType == 0)
    {
        delta1 = this->elMaxResults[whichEdit] - this->elMinResults[whichEdit];
        objMinValue = this->elMinResults[whichEdit];
    }
    else if (intervalType == 1)
    {
        delta1 = log(elMaxResults[whichEdit]) - log(elMinResults[whichEdit]);
        objMinValue = log(this->elMinResults[whichEdit]);
    }
    else
    {
        delta1 = log10(elMaxResults[whichEdit]) - log10(elMinResults[whichEdit]);
        objMinValue = log10(this->elMinResults[whichEdit]);
    }
    delta1 = delta1 / ((double) nBins);

    //qDebug() << "elMaxResults: " << this->elMaxResults[whichEdit];
    //qDebug() << "elMinResults: " << this->elMinResults[whichEdit];

    for (n=0; n<nBins; ++n)
    {
        limits1[n] = objMinValue + delta1 * ((double) (n + 1));
    }
    delta1   = 1.0 / delta1;

    hData    = new int[nBins];
    hDataAll = new int[nBins];

    for (n=0; n<nBins; ++n) hDataAll[n] = 0;

    this->umoHistogram.clear();
    UM_histogramData um_hData;

    for (i=0; i<this->nInstances; i++)
    {
        // Set the upper limits for the instance bins.
        if (intervalType == 0)
        {
            delta2 = this->instances[i].maxResults[whichEdit] - this->instances[i].minResults[whichEdit];
            instMinValue = this->instances[i].minResults[whichEdit];
        }
        else if (intervalType == 1)
        {
            delta2 = log(this->instances[i].maxResults[whichEdit]) - log(this->instances[i].minResults[whichEdit]);
            instMinValue = log(this->instances[i].minResults[whichEdit]);
        }
        else
        {
            delta2 = log10(this->instances[i].maxResults[whichEdit]) - log10(this->instances[i].minResults[whichEdit]);
            instMinValue = log10(this->instances[i].minResults[whichEdit]);
        }
        delta2 = delta2 / ((double) nBins);

        for (n=0; n<nBins; ++n)
        {
            limits2[n] = instMinValue + delta2 * ((double) (n + 1));
        }
        delta2 = 1.0 / delta2;
        // Clear the temp array.
        for (n=0; n<nBins; ++n) hData[n] = 0;

        // ----------------------------------------------------------------------------------------
        // Set the instance # for its 1st order tets.
        if (this->instances[i].elementLimits[0] > 0)
        {
            for (j=this->instances[i].elementLimits[0]; j<=this->instances[i].elementLimits[1]; j++)
            {
                k = j - 1;
                tValue = this->t1elements[k].results[whichEdit];
                if (intervalType == 1) tValue = log(tValue);
                else if (intervalType == 2) tValue = log10(tValue);
                n = (int) ((tValue - instMinValue) * delta2);
                if (n >= nBins) n = nBins - 1;
                ++hData[n];
                n = (int) ((tValue - objMinValue) * delta1);
                if (n >= nBins) n = nBins - 1;
                ++hDataAll[n];
            }
        }
        // ----------------------------------------------------------------------------------------
        // Set the instance # for its 1st order pents.
        if (this->instances[i].elementLimits[2] > 0)
        {
            for (j=this->instances[i].elementLimits[2]; j<=this->instances[i].elementLimits[3]; j++)
            {
                k = j - elAdjust[1] - 1; 
                tValue = this->p1elements[k].results[whichEdit];
                if (intervalType == 1) tValue = log(tValue);
                else if (intervalType == 2) tValue = log10(tValue);
                n = (int) ((tValue - instMinValue) * delta2);
                if (n >= nBins) n = nBins - 1;
                ++hData[n];
                n = (int) ((tValue - objMinValue) * delta1);
                if (n >= nBins) n = nBins - 1;
                ++hDataAll[n];
            }
        }
        // ----------------------------------------------------------------------------------------
        // Set the instance # for its 1st order hexs.
        if (this->instances[i].elementLimits[4] > 0)
        {
            //qDebug() << "maxResults:" << this->instances[i].maxResults[whichEdit];
            //qDebug() << "minResults:" << this->instances[i].minResults[whichEdit];
            //qDebug() << "delta2: " << delta2;

            //qDebug() << "el minResults:" << this->elMinResults[whichEdit];
            //qDebug() << "delta1: " << delta1;

            for (j=this->instances[i].elementLimits[4]; j<=this->instances[i].elementLimits[5]; j++)
            {
                k = j - elAdjust[2] - 1;
                tValue = this->h1elements[k].results[whichEdit];
                if (intervalType == 1) tValue = log(tValue);
                else if (intervalType == 2) tValue = log10(tValue);
                n = (int) ((tValue - instMinValue) * delta2);

                if (n >= nBins) n = nBins - 1;
                //qDebug() << "j: " << j << "\tValue:" << tValue << "\tn: " << n;

                ++hData[n];

                n = (int) ((tValue - objMinValue) * delta1);
                if (n >= nBins) n = nBins - 1;
                ++hDataAll[n];
            }
        }
        // ----------------------------------------------------------------------------------------
        // Set the instance # for its 2nd order tets.
        if (this->instances[i].elementLimits[6] > 0)
        {
            for (j=this->instances[i].elementLimits[6]; j<=this->instances[i].elementLimits[7]; j++)
            {
                k = j - elAdjust[3] - 1;
                tValue = this->t2elements[k].results[whichEdit];
                if (intervalType == 1) tValue = log(tValue);
                else if (intervalType == 2) tValue = log10(tValue);
                n = (int) ((tValue - instMinValue) * delta2);
                if (n >= nBins) n = nBins - 1;
                ++hData[n];
                n = (int) ((tValue - objMinValue) * delta1);
                if (n >= nBins) n = nBins - 1;
                ++hDataAll[n];
            }
        }
        // ----------------------------------------------------------------------------------------
        // Set the instance # for its 2nd order pents.
        if (this->instances[i].elementLimits[8] > 0)
        {
            for (j=this->instances[i].elementLimits[8]; j<=this->instances[i].elementLimits[9]; j++)
            {
                k = j - elAdjust[4] - 1;
                tValue = this->p2elements[k].results[whichEdit];
                n = (int) ((tValue - instMinValue) * delta2);
                if (n >= nBins) n = nBins - 1;
                ++hData[n];
                n = (int) ((tValue - objMinValue) * delta1);
                if (n >= nBins) n = nBins - 1;
                ++hDataAll[n];
            }
        }
        // ----------------------------------------------------------------------------------------
        // Set the instance # for its 2nd order hexs.
        if (this->instances[i].elementLimits[10] > 0)
        {
            for (j=this->instances[i].elementLimits[10]; j<=this->instances[i].elementLimits[11]; j++)
            {
                k = j - elAdjust[5] - 1;
                tValue = this->h2elements[k].results[whichEdit];
                if (intervalType == 1) tValue = log(tValue);
                else if (intervalType == 2) tValue = log10(tValue);
                n = (int) ((tValue - instMinValue) * delta2);
                if (n >= nBins) n = nBins - 1;
                ++hData[n];
                n = (int) ((tValue - objMinValue) * delta1);
                if (n >= nBins) n = nBins - 1;
                ++hDataAll[n];
            }
        }
        // -------------------------
        // Store the intance values.
        um_hData.hBins.clear();
        um_hData.limits.push_back(instMinValue);
        for(n=0; n<nBins; ++n)
        {
            um_hData.hBins.push_back((double) hData[n]);
            um_hData.limits.push_back(limits2[n]);
        }
        this->umoHistogram.push_back(um_hData);
    }
    // --------------------------------------------------------------------------------------------
    // Store the object values.
    um_hData.hBins.clear();
    um_hData.limits.push_back(objMinValue);
    for(n=0; n<nBins; ++n)
    {
        um_hData.hBins.push_back((double) hData[n]);

        //qDebug() << "n: " << n << "\t hData:" << hData[n] << "\t : " << ((double) hData[n]);

        um_hData.limits.push_back(limits1[n]);
    }
    this->umoHistogram.push_back(um_hData);

    // Free the temp arrays.
    delete [] hData;
    delete [] hDataAll;
    delete [] limits1;
    delete [] limits2;
    QApplication::restoreOverrideCursor();
}
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
void UMObject::makeAllNodesPosition()
{
    size_t i;
    allNodesPosition.clear();

    for (i=0; i<nodes.size(); ++i)
    {
        allNodesPosition.push_back((GLfloat) nodes[i].x);
        allNodesPosition.push_back((GLfloat) nodes[i].y);
        allNodesPosition.push_back((GLfloat) nodes[i].z);
    }
}

// ------------------------------------------------------------------------------------------------
void UMObject::makeAllNodesResult(int which)
{
    size_t i;
    allNodesResult.clear();

    for (i=0; i<nodes.size(); ++i)
    {
        allNodesResult.push_back((GLfloat) nodes[i].results[which]);
    }
}

// ------------------------------------------------------------------------------------------------
void UMObject::makeAllNodesResultColor(int which, int intervalType)
{
    size_t  i;
    int     index;
    double  x;
    int aSize = this->resultBins.size();

    allNodesResultColor.clear();

    for (i=0; i<nodes.size(); ++i)
    {
        x = this->nodes[i].results[which];
        if (intervalType == 1)
        {
            x = log(x);
        }
        else if (intervalType == 2)
        {
            x = log10(x);
        }
        index = searchBins(x, this->resultBins, aSize);
        allNodesResultColor.push_back((GLfloat) this->resultColors[index].redF());
        allNodesResultColor.push_back((GLfloat) this->resultColors[index].greenF());
        allNodesResultColor.push_back((GLfloat) this->resultColors[index].blueF());
    }
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
void UMObject::setupVolumeGeometryNodesIndices(int colorChoice)
{
    // Setup the vertex arrays for the indices of those elements that are to be displayed.

    int    j;
    int    inst;
    int    matNo;
    GLint  nodet1, nodet2, nodeh1, nodeh2, nodep13, nodep14, nodep23, nodep24;
    bool   masterDisplay;

    nodet1  = -1;
    nodet2  = -1;
    nodep13 = -1;
    nodep14 = -1;
    nodep23 = -1;
    nodep24 = -1;
    nodeh1  = -1;
    nodeh2  = -1;

    //qDebug() << "-- setupVolumeGeometryNodesIndices --";

    //for (j=0; j<materials.size(); ++j)
    //{
    //    qDebug() << "mat: " << j << "\t display: " << materials[j].display << "\t red: " << materials[j].color.red()
    //            << "\t green: " << materials[j].color.green() << "\t blue: " << materials[j].color.blue();
    //}

    //qDebug() << "h1elNodesIndices.size() "  << this->h1elNodesIndices.size();
    //qDebug() << "t2elNodesIndices.size() "  << this->t2elNodesIndices.size();

    //qDebug() << "nEls[0] : " << nEls[0];
    //qDebug() << "nEls[1] : " << nEls[1];
    //qDebug() << "nEls[2] : " << nEls[2];
    //qDebug() << "nEls[3] : " << nEls[3];
    //qDebug() << "nEls[4] : " << nEls[4];
    //qDebug() << "nEls[5] : " << nEls[5];

    /*if ((fp2 = fopen("/Users/roger/Public/Programs/TestFiles/umobject_vol_indices_debug.txt", "w")) == NULL)
    {
        qDebug() << "-- error opening eeoutReadFile_debug2 file --\n";
    }*/

    if (nEls[0] > 0)
    {
        t1elNodesIndices.clear();
    }
    if (nEls[3] > 0)
    {
        t2elNodesIndices.clear();
    }
    if (nEls[1] > 0)
    {
        p1elNodesIndices3.clear();
        p1elNodesIndices4.clear();
    }
    if (nEls[4] > 0)
    {
        p2elNodesIndices3.clear();
        p2elNodesIndices4.clear();
    }
    if (nEls[2] > 0)
    {
        h1elNodesIndices.clear();
    }
    if (nEls[5] > 0)
    {
        h2elNodesIndices.clear();
    }
    // ----------------------
    //    Add 1st order tets.
    for (int k = 0; k < nEls[0]; ++k)
    {
        inst   = t1elements[k].instance;
        matNo  = t1elements[k].material;
        matNo  = matMap[matNo];

    //    if ((inst < 2) || (inst > 950))
    //        qDebug() << "inst: " << inst << "\t instances display: " << instances[inst].display
    //                << "\telement display: " << t1elements[k].displayElement << "\t matNo: " << matNo
    //                << "t1 material: " << t1elements[j].material;




        if (colorChoice == 1)
            masterDisplay = instances[inst].display;
        else
            masterDisplay = materials[matNo].display;

        if (masterDisplay && t1elements[k].displayElement)
        {
            for (j = 0; j < 12; ++j)
            {
                ++nodet1;
                // Store the node index.
                t1elNodesIndices.push_back(nodet1);
            }
        }                 // end if for displayMaterials
        else
        {
            nodet1 += 12;
        }
    }                     // end k-loop for 1st order tets

    // ----------------------
    //    Add 2nd order tets.
    for (int k = 0; k < nEls[3]; ++k)
    {
        inst   = t2elements[k].instance;
        matNo  = t2elements[k].material;
        matNo  = matMap[matNo];
        if (colorChoice == 1)
            masterDisplay = instances[inst].display;
        else
            masterDisplay = materials[matNo].display;

        //qDebug() << "tet: " << k << "\t displayElement: " << t2elements[k].displayElement;

        if (masterDisplay && t2elements[k].displayElement)
        {
            for (j = 0; j < 12; ++j)
            {
                ++nodet2;
                // Store the node index.
                t2elNodesIndices.push_back(nodet2);
                //qDebug() << "j: " << j << "\t node: " << nodet2;
            }
        }                 // end if for displayMaterials
        else
        {
            nodet2 += 12;
        }
    }                     // end k-loop for 2nd order tets

    // ----------------------
    //    Add 1st order hexs.
    for (int k = 0; k < nEls[2]; ++k)
    {
        inst   = h1elements[k].instance;
        matNo  = h1elements[k].material;
        matNo  = matMap[matNo];

        //fprintf(fp2, "hex: %5d  inst: %2d  matNo: %d\n", k, inst, matNo);

    //    if ((inst < 2) || (inst > 950))
    //        qDebug() << "inst: " << inst << "\t instances display: " << instances[inst].display
    //                << "\telement display: " << h1elements[k].displayElement << "\t matNo: " << matNo
    //                << "h1 material: " << h1elements[j].material;


        if (colorChoice == 1)
            masterDisplay = instances[inst].display;
        else
            masterDisplay = materials[matNo].display;

        if (masterDisplay && h1elements[k].displayElement)
        {
            for (j = 0; j < 24; ++j)
            {
                ++nodeh1;
                // Store the node index.
                h1elNodesIndices.push_back(nodeh1);
                //fprintf(fp2, "\tnodeh1: %d\n", nodeh1);
            }
        }                 // end if for displayMaterials
        else
        {
            nodeh1 += 24;
        }
    }                     // end k-loop for 1st order hexs

    // ----------------------
    //    Add 2nd order hexs.
    for (int k = 0; k < nEls[5]; ++k)
    {
        inst   = h2elements[k].instance;
        matNo  = h2elements[k].material;
        matNo  = matMap[matNo];
        if (colorChoice == 1)
            masterDisplay = instances[inst].display;
        else
            masterDisplay = materials[matNo].display;

        if (masterDisplay && h2elements[k].displayElement)
        {
            for (j = 0; j < 24; ++j)
            {
                ++nodeh2;
                // Store the node index.
                h2elNodesIndices.push_back(nodeh2);
            }
        }                 // end if for displayMaterials
        else
        {
            nodeh2 += 24;
        }
    }                     // end k-loop for 2nd order hexs

    // -----------------------
    //    Add 1st order pents.
    for (int k = 0; k < nEls[1]; ++k)
    {
        inst   = p1elements[k].instance;
        matNo  = p1elements[k].material;
        matNo  = matMap[matNo];
        if (colorChoice == 1)
            masterDisplay = instances[inst].display;
        else
            masterDisplay = materials[matNo].display;

        if (masterDisplay && p1elements[k].displayElement)
        {
            for (j = 0; j < 6; ++j)
            {
                ++nodep13;
                // Store the node index.
                p1elNodesIndices3.push_back(nodep13);
            }
            for (j = 0; j < 12; ++j)
            {
                ++nodep14;
                // Store the node index.
                p1elNodesIndices4.push_back(nodep14);
            }
        }                 // end if for displayMaterials
        else
        {
            nodep13 +=  6;
            nodep14 += 12;
        }
    }                     // end k-loop for 1st order pents

    // -----------------------
    //    Add 2nd order pents.
    for (int k = 0; k < nEls[4]; ++k)
    {
        inst   = p2elements[k].instance;
        matNo  = p2elements[k].material;
        matNo  = matMap[matNo];
        if (colorChoice == 1)
            masterDisplay = instances[inst].display;
        else
            masterDisplay = materials[matNo].display;

        if (masterDisplay && p2elements[k].displayElement)
        {
            for (j = 0; j < 6; ++j)
            {
                ++nodep23;
                // Store the node index.
                p2elNodesIndices3.push_back(nodep23);
            }
            for (j = 0; j < 12; ++j)
            {
                ++nodep24;
                // Store the node index.
                p2elNodesIndices4.push_back(nodep24);
            }
        }                 // end if for displayMaterials
        else
        {
            nodep23 +=  6;
            nodep24 += 12;
        }
    }                     // end k-loop for 1st order pents
    //fprintf(fp2, "h1elNodesIndices -- size: %d\n", h1elNodesIndices.size());
    //fprintf(fp2, "h2elNodesIndices -- size: %d\n", h2elNodesIndices.size());

    //fclose(fp2);

    //qDebug() << " ** END setupVolumeGeometryNodesIndices **";

    //qDebug() << "h1elNodesIndices.size() "  << this->h1elNodesIndices.size();
    //qDebug() << "t2elNodesIndices.size() "  << this->t2elNodesIndices.size();
}

// ------------------------------------------------------------------------------------------------
void UMObject::setupVolumeGeometryNodesColors(int intervalType, int plotType, int colorChoice, int contourType)
{
    // Setup the vertex arrays for quilted view where the homogeneous values for all nodes in an
    // element may be for either geometry or a result.

    QColor color1;
    int which = plotType - 1;
    int inst;
    int index;
    int matNo;
    int node;
    int aSize = resultBins.size();
    double x;

    //qDebug() << "--- setupVolumeGeometryNodesColors ---";

    /*if ((fp3 = fopen("/Users/roger/Public/Programs/TestFiles/umobject_vol_colors_debug.txt", "w")) == NULL)
    {
        qDebug() << "-- error opening umobject_vol_colors_debug file --\n";
    }*/

    if (nEls[0] > 0)
    {
        t1elNodesColor.clear();
    }
    if (nEls[3] > 0)
    {
        t2elNodesColor.clear();
    }
    if (nEls[1] > 0)
    {
        p1elNodesColor3.clear();
        p1elNodesColor4.clear();
    }
    if (nEls[4] > 0)
    {
        p2elNodesColor3.clear();
        p2elNodesColor4.clear();
    }
    if (nEls[2] > 0)
    {
        h1elNodesColor.clear();
    }
    if (nEls[5] > 0)
    {
        h2elNodesColor.clear();
    }
    // ----------------------
    //    Add 1st order tets.
    for (int k = 0; k < nEls[0]; ++k)
    {
        inst   = t1elements[k].instance;
        matNo  = t1elements[k].material;
        matNo  = matMap[matNo];

        if (contourType == 0)
        {
            // ------------------------------------
            // - QUILTED results color assignment -
            // ------------------------------------
            if (which > - 1)
            {
                x = t1elements[k].results[which];
                if (intervalType == 1)
                {
                    x = log(x);
                }
                else if (intervalType == 2)
                {
                    x = log10(x);
                }
                index  = searchBins(x, this->resultBins, aSize);
                color1 = resultColors[index];
            }
            // -----------------------------
            // - GEOMTERY color assignment -
            // -----------------------------
            else
            {
                if (colorChoice == 0)
                    color1 = materials[matNo].color;
                else
                    color1 = instances[inst].color;
            }
        }                   // - end contourType == 0

        // Loop over the faces.
        for (int i = 0; i < 4; ++i)
        {
            if (contourType == 1)
            {
                // ------------------------------------
                // - BLENDED results color assignment -
                // ------------------------------------
                for (int j = 0; j < 3; ++j)
                {
                    node  = t1elements[k].connect[ tetFaces[i][j] ];
                    x     = nodes[node].results[which];
                    if (intervalType == 1)
                    {
                        x = log(x);
                    }
                    else if (intervalType == 2)
                    {
                        x = log10(x);
                    }
                    index = searchBins(x, this->resultBins, aSize);
                    color1 = resultColors[index];
                    // Store the node color.
                    t1elNodesColor.push_back((GLfloat) color1.redF());
                    t1elNodesColor.push_back((GLfloat) color1.greenF());
                    t1elNodesColor.push_back((GLfloat) color1.blueF());
                    t1elNodesColor.push_back((GLfloat) color1.alphaF());
                }
            }
            else
            {
                // ---------------------------------------
                // - One color assignment for all nodes. -
                // - Geometry or quilted view.           -
                // ---------------------------------------
                for (int j = 0; j < 3; ++j)
                {
                    // Store the node color.
                    t1elNodesColor.push_back((GLfloat) color1.redF());
                    t1elNodesColor.push_back((GLfloat) color1.greenF());
                    t1elNodesColor.push_back((GLfloat) color1.blueF());
                    t1elNodesColor.push_back((GLfloat) color1.alphaF());
                }
            }               // end else -- contourType == 0
        }                   // end i-loop
    }                       // end k-loop for 1st order tets

    // ----------------------
    //    Add 2nd order tets.
    for (int k = 0; k < nEls[3]; ++k)
    {
        inst   = t2elements[k].instance;
        matNo  = t2elements[k].material;
        matNo  = matMap[matNo];

        if (contourType == 0)
        {
            // ------------------------------------
            // - QUILTED results color assignment -
            // ------------------------------------
            if (which > - 1)
            {
                x = t2elements[k].results[which];
                if (intervalType == 1)
                {
                    x = log(x);
                }
                else if (intervalType == 2)
                {
                    x = log10(x);
                }
                index  = searchBins(x, this->resultBins, aSize);
                color1 = resultColors[index];
            }
            // -----------------------------
            // - GEOMTERY color assignment -
            // -----------------------------
            else
            {
                if (colorChoice == 0)
                    color1 = materials[matNo].color;
                else
                    color1 = instances[inst].color;
            }
        }                   // - end contourType == 0

        // Loop over the faces.
        for (int i = 0; i < 4; ++i)
        {
            if (contourType == 1)
            {
                // ------------------------------------
                // - BLENDED results color assignment -
                // ------------------------------------
                for (int j = 0; j < 3; ++j)
                {
                    node  = t2elements[k].connect[ tetFaces[i][j] ];
                    x     = nodes[node].results[which];
                    if (intervalType == 1)
                    {
                        x = log(x);
                    }
                    else if (intervalType == 2)
                    {
                        x = log10(x);
                    }
                    index = searchBins(x, this->resultBins, aSize);
                    color1 = resultColors[index];
                    // Store the node color.
                    t2elNodesColor.push_back((GLfloat) color1.redF());
                    t2elNodesColor.push_back((GLfloat) color1.greenF());
                    t2elNodesColor.push_back((GLfloat) color1.blueF());
                    t2elNodesColor.push_back((GLfloat) color1.alphaF());
                }
            }
            else
            {
                // ---------------------------------------
                // - One color assignment for all nodes. -
                // - Geometry or quilted view.           -
                // ---------------------------------------
                for (int j = 0; j < 3; ++j)
                {
                    // Store the node color.
                    t2elNodesColor.push_back((GLfloat) color1.redF());
                    t2elNodesColor.push_back((GLfloat) color1.greenF());
                    t2elNodesColor.push_back((GLfloat) color1.blueF());
                    t2elNodesColor.push_back((GLfloat) color1.alphaF());
                }
            }               // end else -- contourType == 0
        }                   // end i-loop
    }                       // end k-loop for 2nd order tets

    // ----------------------
    //    Add 1st order hexs.
    for (int k = 0; k < nEls[2]; ++k)
    {
        inst   = h1elements[k].instance;
        matNo  = h1elements[k].material;
        matNo  = matMap[matNo];

        //fprintf(fp3, "hex no: %6d  inst: %4d  matNo: %4d\n", k, inst, matNo);

        if (contourType == 0)
        {
            // ------------------------------------
            // - QUILTED results color assignment -
            // ------------------------------------
            if (which > - 1)
            {
                x = h1elements[k].results[which];
                if (intervalType == 1)
                {
                    x = log(x);
                }
                else if (intervalType == 2)
                {
                    x = log10(x);
                }
                index  = searchBins(x, this->resultBins, aSize);
                color1 = resultColors[index];
            }
            // -----------------------------
            // - GEOMTERY color assignment -
            // -----------------------------
            else
            {
                if (colorChoice == 0)
                    color1 = materials[matNo].color;
                else
                    color1 = instances[inst].color;
            }
        }                   // - end contourType == 0

        // Loop over the faces.
        for (int i = 0; i < 6; ++i)
        {
            if (contourType == 1)
            {
                // ------------------------------------
                // - BLENDED results color assignment -
                // ------------------------------------
                for (int j = 0; j < 4; ++j)
                {
                    node  = h1elements[k].connect[ hexFaces[i][j] ];
                    x     = nodes[node].results[which];
                    if (intervalType == 1)
                    {
                        x = log(x);
                    }
                    else if (intervalType == 2)
                    {
                        x = log10(x);
                    }
                    index = searchBins(x, this->resultBins, aSize);
                    color1 = resultColors[index];
                    // Store the node color.
                    h1elNodesColor.push_back((GLfloat) color1.redF());
                    h1elNodesColor.push_back((GLfloat) color1.greenF());
                    h1elNodesColor.push_back((GLfloat) color1.blueF());
                    h1elNodesColor.push_back((GLfloat) color1.alphaF());

                    //fprintf(fp3, "%4d %4d\t%f\t%f\t%f\n", i, j, color1.redF(), color1.greenF(), color1.blueF());
                }
            }
            else
            {
                // ---------------------------------------
                // - One color assignment for all nodes. -
                // - Geometry or quilted view.           -
                // ---------------------------------------
                for (int j = 0; j < 4; ++j)
                {
                    // Store the node color.
                    h1elNodesColor.push_back((GLfloat) color1.redF());
                    h1elNodesColor.push_back((GLfloat) color1.greenF());
                    h1elNodesColor.push_back((GLfloat) color1.blueF());
                    h1elNodesColor.push_back((GLfloat) color1.alphaF());
                }
               // fprintf(fp3, "face: %4d \t%f\t%f\t%f\n", i, color1.redF(), color1.greenF(), color1.blueF());

            }               // end else -- contourType == 0
        }                   // end i-loop
    }                       // end k-loop for 1st order hexs

    // ----------------------
    //    Add 2nd order hexs.
    for (int k = 0; k < nEls[5]; ++k)
    {
        inst   = h2elements[k].instance;
        matNo  = h2elements[k].material;
        matNo  = matMap[matNo];

        if (contourType == 0)
        {
            // ------------------------------------
            // - QUILTED results color assignment -
            // ------------------------------------
            if (which > - 1)
            {
                x = h2elements[k].results[which];
                //if (k == 7 || k == 6 || k == 8)
                //{
                //    qDebug() << "h2 element result:: k: " << k << "\t" << x;
                //}


                if (intervalType == 1)
                {
                    x = log(x);
                }
                else if (intervalType == 2)
                {
                    x = log10(x);
                }
                index  = searchBins(x, this->resultBins, aSize);
                color1 = resultColors[index];
            }
            // -----------------------------
            // - GEOMTERY color assignment -
            // -----------------------------
            else
            {
                if (colorChoice == 0)
                    color1 = materials[matNo].color;
                else
                    color1 = instances[inst].color;
            }
        }                   // - end contourType == 0

        // Loop over the faces.
        for (int i = 0; i < 6; ++i)
        {
            if (contourType == 1)
            {
                // ------------------------------------
                // - BLENDED results color assignment -
                // ------------------------------------
                for (int j = 0; j < 4; ++j)
                {
                    node  = h2elements[k].connect[ hexFaces[i][j] ];
                    x     = nodes[node].results[which];
                    if (intervalType == 1)
                    {
                        x = log(x);
                    }
                    else if (intervalType == 2)
                    {
                        x = log10(x);
                    }
                    index = searchBins(x, this->resultBins, aSize);
                    color1 = resultColors[index];
                    // Store the node color.
                    h2elNodesColor.push_back((GLfloat) color1.redF());
                    h2elNodesColor.push_back((GLfloat) color1.greenF());
                    h2elNodesColor.push_back((GLfloat) color1.blueF());
                    h2elNodesColor.push_back((GLfloat) color1.alphaF());
                }
            }
            else
            {
                // ---------------------------------------
                // - One color assignment for all nodes. -
                // - Geometry or quilted view.           -
                // ---------------------------------------
                for (int j = 0; j < 4; ++j)
                {
                    // Store the node color.
                    h2elNodesColor.push_back((GLfloat) color1.redF());
                    h2elNodesColor.push_back((GLfloat) color1.greenF());
                    h2elNodesColor.push_back((GLfloat) color1.blueF());
                    h2elNodesColor.push_back((GLfloat) color1.alphaF());
                }
            }               // end else -- contourType == 0
        }                   // end i-loop
    }                       // end k-loop for 2nd order hexs

    // -----------------------
    //    Add 1st order pents.
    for (int k = 0; k < nEls[1]; ++k)
    {
        inst   = p1elements[k].instance;
        matNo  = p1elements[k].material;
        matNo  = matMap[matNo];

        if (contourType == 0)
        {
            // ------------------------------------
            // - QUILTED results color assignment -
            // ------------------------------------
            if (which > - 1)
            {
                x = p1elements[k].results[which];
                if (intervalType == 1)
                {
                    x = log(x);
                }
                else if (intervalType == 2)
                {
                    x = log10(x);
                }
                index  = searchBins(x, this->resultBins, aSize);
                color1 = resultColors[index];
            }
            // -----------------------------
            // - GEOMTERY color assignment -
            // -----------------------------
            else
            {
                if (colorChoice == 0)
                    color1 = materials[matNo].color;
                else
                    color1 = instances[inst].color;
            }
        }                   // - end contourType == 0

        // Loop over the 3-node faces.
        for (int i = 0; i < 2; ++i)
        {
            if (contourType == 1)
            {
                // ------------------------------------
                // - BLENDED results color assignment -
                // ------------------------------------
                for (int j = 0; j < 3; ++j)
                {
                    node  = p1elements[k].connect[ pentFaces[i][j] ];
                    x     = nodes[node].results[which];
                    if (intervalType == 1)
                    {
                        x = log(x);
                    }
                    else if (intervalType == 2)
                    {
                        x = log10(x);
                    }
                    index = searchBins(x, this->resultBins, aSize);
                    color1 = resultColors[index];
                    // Store the node color.
                    p1elNodesColor3.push_back((GLfloat) color1.redF());
                    p1elNodesColor3.push_back((GLfloat) color1.greenF());
                    p1elNodesColor3.push_back((GLfloat) color1.blueF());
                    p1elNodesColor3.push_back((GLfloat) color1.alphaF());
                }
            }
            else
            {
                // ---------------------------------------
                // - One color assignment for all nodes. -
                // - Geometry or quilted view.           -
                // ---------------------------------------
                for (int j = 0; j < 3; ++j)
                {
                    // Store the node color.
                    p1elNodesColor3.push_back((GLfloat) color1.redF());
                    p1elNodesColor3.push_back((GLfloat) color1.greenF());
                    p1elNodesColor3.push_back((GLfloat) color1.blueF());
                    p1elNodesColor3.push_back((GLfloat) color1.alphaF());
                }
            }               // end else -- contourType == 0
        }                   // end i-loop

        // Loop over the 4-node faces.
        for (int i = 2; i < 5; ++i)
        {
            if (contourType == 1)
            {
                // ------------------------------------
                // - BLENDED results color assignment -
                // ------------------------------------
                for (int j = 0; j < 4; ++j)
                {
                    node  = p1elements[k].connect[ pentFaces[i][j] ];
                    x     = nodes[node].results[which];
                    if (intervalType == 1)
                    {
                        x = log(x);
                    }
                    else if (intervalType == 2)
                    {
                        x = log10(x);
                    }
                    index = searchBins(x, this->resultBins, aSize);
                    color1 = resultColors[index];
                    // Store the node color.
                    p1elNodesColor4.push_back((GLfloat) color1.redF());
                    p1elNodesColor4.push_back((GLfloat) color1.greenF());
                    p1elNodesColor4.push_back((GLfloat) color1.blueF());
                    p1elNodesColor4.push_back((GLfloat) color1.alphaF());
                }
            }
            else
            {
                // ---------------------------------------
                // - One color assignment for all nodes. -
                // - Geometry or quilted view.           -
                // ---------------------------------------
                for (int j = 0; j < 4; ++j)
                {
                    // Store the node color.
                    p1elNodesColor4.push_back((GLfloat) color1.redF());
                    p1elNodesColor4.push_back((GLfloat) color1.greenF());
                    p1elNodesColor4.push_back((GLfloat) color1.blueF());
                    p1elNodesColor4.push_back((GLfloat) color1.alphaF());
                }
            }               // end else -- contourType == 0
        }                   // end i-loop
    }                       // end k-loop for 1st order pents

    // -----------------------
    //    Add 2nd order pents.
    for (int k = 0; k < nEls[4]; ++k)
    {
        inst   = p2elements[k].instance;
        matNo  = p2elements[k].material;
        matNo  = matMap[matNo];

        //qDebug() << "2nd order pent: " << k;

        if (contourType == 0)
        {
            // ------------------------------------
            // - QUILTED results color assignment -
            // ------------------------------------
            if (which > - 1)
            {
                x = p2elements[k].results[which];
                if (intervalType == 1)
                {
                    x = log(x);
                }
                else if (intervalType == 2)
                {
                    x = log10(x);
                }
                index  = searchBins(x, this->resultBins, aSize);
                color1 = resultColors[index];
            }
            // -----------------------------
            // - GEOMTERY color assignment -
            // -----------------------------
            else
            {
                if (colorChoice == 0)
                    color1 = materials[matNo].color;
                else
                    color1 = instances[inst].color;
            }
        }                   // - end contourType == 0

        // Loop over the 3-node faces.
        for (int i = 0; i < 2; ++i)
        {
            //qDebug() << "\t face: " << i;
            //qDebug() << "\t\t color: " << color1.red() << color1.green() << color1.blue();

            if (contourType == 1)
            {
                // ------------------------------------
                // - BLENDED results color assignment -
                // ------------------------------------
                for (int j = 0; j < 3; ++j)
                {
                    node  = p2elements[k].connect[ pentFaces[i][j] ];
                    x     = nodes[node].results[which];
                    if (intervalType == 1)
                    {
                        x = log(x);
                    }
                    else if (intervalType == 2)
                    {
                        x = log10(x);
                    }
                    index = searchBins(x, this->resultBins, aSize);
                    color1 = resultColors[index];
                    // Store the node color.
                    p2elNodesColor3.push_back((GLfloat) color1.redF());
                    p2elNodesColor3.push_back((GLfloat) color1.greenF());
                    p2elNodesColor3.push_back((GLfloat) color1.blueF());
                    p2elNodesColor3.push_back((GLfloat) color1.alphaF());
                }
            }
            else
            {
                // ---------------------------------------
                // - One color assignment for all nodes. -
                // - Geometry or quilted view.           -
                // ---------------------------------------
                for (int j = 0; j < 3; ++j)
                {
                    // Store the node color.
                    p2elNodesColor3.push_back((GLfloat) color1.redF());
                    p2elNodesColor3.push_back((GLfloat) color1.greenF());
                    p2elNodesColor3.push_back((GLfloat) color1.blueF());
                    p2elNodesColor3.push_back((GLfloat) color1.alphaF());
                }
            }               // end else -- contourType == 0
        }                   // end i-loop

        // Loop over the 4-node faces.
        for (int i = 2; i < 5; ++i)
        {
            //qDebug() << "\t face: " << i;
            //qDebug() << "\t\t color: " << color1.red() << color1.green() << color1.blue();

            if (contourType == 1)
            {
                // ------------------------------------
                // - BLENDED results color assignment -
                // ------------------------------------
                for (int j = 0; j < 4; ++j)
                {
                    node  = p2elements[k].connect[ pentFaces[i][j] ];
                    x     = nodes[node].results[which];
                    if (intervalType == 1)
                    {
                        x = log(x);
                    }
                    else if (intervalType == 2)
                    {
                        x = log10(x);
                    }
                    index = searchBins(x, this->resultBins, aSize);
                    color1 = resultColors[index];
                    // Store the node color.
                    p2elNodesColor4.push_back((GLfloat) color1.redF());
                    p2elNodesColor4.push_back((GLfloat) color1.greenF());
                    p2elNodesColor4.push_back((GLfloat) color1.blueF());
                    p2elNodesColor4.push_back((GLfloat) color1.alphaF());
                }
            }
            else
            {
                // ---------------------------------------
                // - One color assignment for all nodes. -
                // - Geometry or quilted view.           -
                // ---------------------------------------
                for (int j = 0; j < 4; ++j)
                {
                    // Store the node color.
                    p2elNodesColor4.push_back((GLfloat) color1.redF());
                    p2elNodesColor4.push_back((GLfloat) color1.greenF());
                    p2elNodesColor4.push_back((GLfloat) color1.blueF());
                    p2elNodesColor4.push_back((GLfloat) color1.alphaF());
                }
            }               // end else -- contourType == 0
        }                   // end i-loop
    }                       // end k-loop for 2nd order pents

    //fprintf(fp3, "h1elNodesColor size: %d\n", h1elNodesColor.size());

    //fclose(fp3);
}

// ------------------------------------------------------------------------------------------------
void UMObject::setupVolumeGeometryNodes()
{
    // Setup the volume nodes in vertex arrays.

    GLfloat cox, coy, coz;
    QColor  color1;
    int     inst;
    int     node;

    //qDebug() << "\n--- setupVolumeGeometryNodes ---";

    //if ((fp4 = fopen("/Users/roger/Public/Programs/TestFiles/umobject_vol_nodes_debug.txt", "w")) == NULL)
    //{
    //    qDebug() << "-- error opening umobject_vol_nodes_debug file --\n";
    //}

    if (nEls[0] > 0)
    {
        t1elNodesPosition.clear();
        t1elNodesNormal.clear();
    }
    if (nEls[3] > 0)
    {
        t2elNodesPosition.clear();
        t2elNodesNormal.clear();
    }
    if (nEls[1] > 0)
    {
        p1elNodesPosition3.clear();
        p1elNodesNormal3.clear();
        p1elNodesPosition4.clear();
        p1elNodesNormal4.clear();
    }
    if (nEls[4] > 0)
    {
        p2elNodesPosition3.clear();
        p2elNodesNormal3.clear();
        p2elNodesPosition4.clear();
        p2elNodesNormal4.clear();
    }
    if (nEls[2] > 0)
    {
        h1elNodesPosition.clear();
        h1elNodesNormal.clear();
    }
    if (nEls[5] > 0)
    {
        h2elNodesPosition.clear();
        h2elNodesNormal.clear();
    }

    // ----------------------
    //    Add 1st order tets.
    for (int k = 0; k < nEls[0]; ++k)
    {
        inst   = t1elements[k].instance;

        // Loop over the 4 faces.
        for (int i = 0; i < 4; ++i)
        {
            // Loop over the 3 vertices
            for (int j = 0; j < 3; ++j)
            {
                node = t1elements[k].connect[ tetFaces[i][j] ];
                cox = (GLfloat) (nodes[node].x + instances[inst].expand[0] + instances[inst].extraExpand[0]);
                coy = (GLfloat) (nodes[node].y + instances[inst].expand[1] + instances[inst].extraExpand[1]);
                coz = (GLfloat) (nodes[node].z + instances[inst].expand[2] + instances[inst].extraExpand[2]);
                // Store the node position.
                t1elNodesPosition.push_back(cox);
                t1elNodesPosition.push_back(coy);
                t1elNodesPosition.push_back(coz);
                // Store the node normal.
                t1elNodesNormal.push_back((GLfloat) t1elements[k].normal[i][0]);
                t1elNodesNormal.push_back((GLfloat) t1elements[k].normal[i][1]);
                t1elNodesNormal.push_back((GLfloat) t1elements[k].normal[i][2]);
            }
        }                   // end i-loop
    }                       // end k-loop for 1st order tets

    // ----------------------
    //    Add 2nd order tets.
    for (int k = 0; k < nEls[3]; ++k)
    {
        inst   = t2elements[k].instance;

        // Loop over the 4 faces.
        for (int i = 0; i < 4; ++i)
        {
            // Loop over the 3 vertices
            for (int j = 0; j < 3; ++j)
            {
                node = t2elements[k].connect[ tetFaces[i][j] ];
                cox = (GLfloat) (nodes[node].x + instances[inst].expand[0] + instances[inst].extraExpand[0]);
                coy = (GLfloat) (nodes[node].y + instances[inst].expand[1] + instances[inst].extraExpand[1]);
                coz = (GLfloat) (nodes[node].z + instances[inst].expand[2] + instances[inst].extraExpand[2]);
                // Store the node position.
                t2elNodesPosition.push_back(cox);
                t2elNodesPosition.push_back(coy);
                t2elNodesPosition.push_back(coz);
                // Store the node normal.
                t2elNodesNormal.push_back((GLfloat) t2elements[k].normal[i][0]);
                t2elNodesNormal.push_back((GLfloat) t2elements[k].normal[i][1]);
                t2elNodesNormal.push_back((GLfloat) t2elements[k].normal[i][2]);
            }
        }                   // end i-loop
    }                       // end k-loop for 2nd order tets

    // ----------------------
    //    Add 1st order hexs.
    for (int k = 0; k < nEls[2]; ++k)
    {
        inst   = h1elements[k].instance;

        // Loop over the 6 faces.
        for (int i = 0; i < 6; ++i)
        {
            // Loop over the 4 vertices
            for (int j = 0; j < 4; ++j)
            {
                node = h1elements[k].connect[ hexFaces[i][j] ];
                cox = (GLfloat) (nodes[node].x + instances[inst].expand[0] + instances[inst].extraExpand[0]);
                coy = (GLfloat) (nodes[node].y + instances[inst].expand[1] + instances[inst].extraExpand[1]);
                coz = (GLfloat) (nodes[node].z + instances[inst].expand[2] + instances[inst].extraExpand[2]);
                // Store the node position.
                h1elNodesPosition.push_back(cox);
                h1elNodesPosition.push_back(coy);
                h1elNodesPosition.push_back(coz);
                // Store the node normal.
                h1elNodesNormal.push_back((GLfloat) h1elements[k].normal[i][0]);
                h1elNodesNormal.push_back((GLfloat) h1elements[k].normal[i][1]);
                h1elNodesNormal.push_back((GLfloat) h1elements[k].normal[i][2]);
            }
        }                   // end i-loop
    }                       // end k-loop for 1st order hexs

    // ----------------------
    //    Add 2nd order hexs.
    for (int k = 0; k < nEls[5]; ++k)
    {
        inst   = h2elements[k].instance;

        // Loop over the 6 faces.
        for (int i = 0; i < 6; ++i)
        {
            // Loop over the 4 vertices
            for (int j = 0; j < 4; ++j)
            {
                node = h2elements[k].connect[ hexFaces[i][j] ];
                cox = (GLfloat) (nodes[node].x + instances[inst].expand[0] + instances[inst].extraExpand[0]);
                coy = (GLfloat) (nodes[node].y + instances[inst].expand[1] + instances[inst].extraExpand[1]);
                coz = (GLfloat) (nodes[node].z + instances[inst].expand[2] + instances[inst].extraExpand[2]);
                // Store the node position.
                h2elNodesPosition.push_back(cox);
                h2elNodesPosition.push_back(coy);
                h2elNodesPosition.push_back(coz);
                // Store the node normal.
                h2elNodesNormal.push_back((GLfloat) h2elements[k].normal[i][0]);
                h2elNodesNormal.push_back((GLfloat) h2elements[k].normal[i][1]);
                h2elNodesNormal.push_back((GLfloat) h2elements[k].normal[i][2]);
            }
        }                   // end i-loop
    }                       // end k-loop for 2nd order hexs

    // -----------------------
    //    Add 1st order pents.
    for (int k = 0; k < nEls[1]; ++k)
    {
        inst   = p1elements[k].instance;

        // Loop over the 3-node faces.
        for (int i = 0; i < 2; ++i)
        {
            // Loop over the 3 vertices
            for (int j = 0; j < 3; ++j)
            {
                node = p1elements[k].connect[ pentFaces[i][j] ];
                cox = (GLfloat) (nodes[node].x + instances[inst].expand[0] + instances[inst].extraExpand[0]);
                coy = (GLfloat) (nodes[node].y + instances[inst].expand[1] + instances[inst].extraExpand[1]);
                coz = (GLfloat) (nodes[node].z + instances[inst].expand[2] + instances[inst].extraExpand[2]);
                // Store the node position.
                p1elNodesPosition3.push_back(cox);
                p1elNodesPosition3.push_back(coy);
                p1elNodesPosition3.push_back(coz);
                // Store the node normal.
                p1elNodesNormal3.push_back((GLfloat) p1elements[k].normal[i][0]);
                p1elNodesNormal3.push_back((GLfloat) p1elements[k].normal[i][1]);
                p1elNodesNormal3.push_back((GLfloat) p1elements[k].normal[i][2]);
            }
        }                   // end i-loop
        // Loop over the 4-node faces.
        for (int i = 2; i < 5; ++i)
        {
            // Loop over the 4 vertices
            for (int j = 0; j < 4; ++j)
            {
                node = p1elements[k].connect[ pentFaces[i][j] ];
                cox = (GLfloat) (nodes[node].x + instances[inst].expand[0] + instances[inst].extraExpand[0]);
                coy = (GLfloat) (nodes[node].y + instances[inst].expand[1] + instances[inst].extraExpand[1]);
                coz = (GLfloat) (nodes[node].z + instances[inst].expand[2] + instances[inst].extraExpand[2]);
                // Store the node position.
                p1elNodesPosition4.push_back(cox);
                p1elNodesPosition4.push_back(coy);
                p1elNodesPosition4.push_back(coz);
                // Store the node normal.
                p1elNodesNormal4.push_back((GLfloat) p1elements[k].normal[i][0]);
                p1elNodesNormal4.push_back((GLfloat) p1elements[k].normal[i][1]);
                p1elNodesNormal4.push_back((GLfloat) p1elements[k].normal[i][2]);
            }
        }                   // end i-loop
    }                       // end k-loop for 1st order pents

    // -----------------------
    //    Add 2nd order pents.
    for (int k = 0; k < nEls[4]; ++k)
    {
        inst   = p2elements[k].instance;

        // Loop over the 3-node faces.
        for (int i = 0; i < 2; ++i)
        {
            // Loop over the 3 vertices
            for (int j = 0; j < 3; ++j)
            {
                node = p2elements[k].connect[ pentFaces[i][j] ];
                cox = (GLfloat) (nodes[node].x + instances[inst].expand[0] + instances[inst].extraExpand[0]);
                coy = (GLfloat) (nodes[node].y + instances[inst].expand[1] + instances[inst].extraExpand[1]);
                coz = (GLfloat) (nodes[node].z + instances[inst].expand[2] + instances[inst].extraExpand[2]);
                // Store the node position.
                p2elNodesPosition3.push_back(cox);
                p2elNodesPosition3.push_back(coy);
                p2elNodesPosition3.push_back(coz);
                // Store the node normal.
                p2elNodesNormal3.push_back((GLfloat) p2elements[k].normal[i][0]);
                p2elNodesNormal3.push_back((GLfloat) p2elements[k].normal[i][1]);
                p2elNodesNormal3.push_back((GLfloat) p2elements[k].normal[i][2]);
            }
        }                   // end i-loop
        // Loop over the 4-node faces.
        for (int i = 2; i < 5; ++i)
        {
            // Loop over the 4 vertices
            for (int j = 0; j < 4; ++j)
            {
                node = p2elements[k].connect[ pentFaces[i][j] ];
                cox = (GLfloat) (nodes[node].x + instances[inst].expand[0] + instances[inst].extraExpand[0]);
                coy = (GLfloat) (nodes[node].y + instances[inst].expand[1] + instances[inst].extraExpand[1]);
                coz = (GLfloat) (nodes[node].z + instances[inst].expand[2] + instances[inst].extraExpand[2]);
                // Store the node position.
                p2elNodesPosition4.push_back(cox);
                p2elNodesPosition4.push_back(coy);
                p2elNodesPosition4.push_back(coz);
                // Store the node normal.
                p2elNodesNormal4.push_back((GLfloat) p2elements[k].normal[i][0]);
                p2elNodesNormal4.push_back((GLfloat) p2elements[k].normal[i][1]);
                p2elNodesNormal4.push_back((GLfloat) p2elements[k].normal[i][2]);
            }
        }                   // end i-loop
    }                       // end k-loop for 2nd order pents

    //fprintf(fp4, "h1elNodesPosition size: %d", h1elNodesPosition.size());
    //fclose(fp4);
}

// ------------------------------------------------------------------------------------------------
// - Begin surface routines.
// ------------------------------------------------------------------------------------------------
void UMObject::setupSurfaceNodePositions()
{
    int             element;
    int             inst;
    int             jlim;
    int             klim;
    int             node, node3;
    unsigned char   face;
    unsigned char   etype;
    GLfloat         cox, coy, coz;


 /*   if ((fp2 = fopen("/Users/roger/Public/Programs/TestFiles/umobject_snp3_debug.txt", "w")) == NULL)
    {
        qDebug() << "-- error opening eeoutReadFile_debug2 file --\n";
    }  */

    //qDebug() << "-- setupSurfaceNodePositions --";
    //qDebug() << "h1elNodesIndices.size() "  << this->h1elNodesIndices.size();
    //qDebug() << "t2elNodesIndices.size() "  << this->t2elNodesIndices.size();

    //qDebug() << "nEls[0] : " << nEls[0];
    //qDebug() << "nEls[1] : " << nEls[1];
    //qDebug() << "nEls[2] : " << nEls[2];
    //qDebug() << "nEls[3] : " << nEls[3];
    //qDebug() << "nEls[4] : " << nEls[4];
    //qDebug() << "nEls[5] : " << nEls[5];

    node3 = -1;
    surfaceNodePosition3.clear();
    surfaceNodePosition4.clear();

    for (inst=0; inst < nInstances; ++inst)
    {
        klim = instances[inst].surface.size();
        for(int k = 0; k<klim; ++k)
        {
            element = instances[inst].surface[k].element;
            face    = instances[inst].surface[k].face;
            etype   = instances[inst].surface[k].type;

            if (etype == 0)  // *** 1st Order Tets ***
            {
                for (int j = 0; j < 3; ++j)
                {
                    ++node3;
                    node = t1elements[element].connect[ tetFaces[face][j] ];
                    cox = (GLfloat) (nodes[node].x + instances[inst].expand[0] + instances[inst].extraExpand[0]);
                    coy = (GLfloat) (nodes[node].y + instances[inst].expand[1] + instances[inst].extraExpand[1]);
                    coz = (GLfloat) (nodes[node].z + instances[inst].expand[2] + instances[inst].extraExpand[2]);
                    // Store the node position.
                    surfaceNodePosition3.push_back(cox);
                    surfaceNodePosition3.push_back(coy);
                    surfaceNodePosition3.push_back(coz);

               //     fprintf(fp2, "%d\tnode3: %4d  element: %6d  XYZ: %f\t %f\t %f\n ", etype, node3, element, cox, coy, coz);

         /*           if (node3 < 3)
                    {
                        qDebug() << "setupSurfaceNodePositions:: element: " << element;
                        qDebug() << "x y z: " << cox << "\t" << coy << "\t" << coz;
                    } */

                }
            }                       // -- end etype == 0
            else if (etype == 3)    // -- 2nd Order Tets
            {
                for (int j = 0; j < 3; ++j)
                {
                    //qDebug() << "j: " << j << "\tface: " << face;
                    //qDebug() << "tetFaces: " << tetFaces[face][j];

                    node = t2elements[element].connect[ tetFaces[face][j] ];
                    cox = (GLfloat) (nodes[node].x + instances[inst].expand[0] + instances[inst].extraExpand[0]);
                    coy = (GLfloat) (nodes[node].y + instances[inst].expand[1] + instances[inst].extraExpand[1]);
                    coz = (GLfloat) (nodes[node].z + instances[inst].expand[2] + instances[inst].extraExpand[2]);

                  //  fprintf(fp2, "%d\tnode3: %4d  element: %6d  XYZ: %f\t %f\t %f\n ", etype, node3, element, cox, coy, coz);


                    // Store the node position.
                    surfaceNodePosition3.push_back(cox);
                    surfaceNodePosition3.push_back(coy);
                    surfaceNodePosition3.push_back(coz);
                }
            }                       // -- end etype == 3
            else if (etype == 2)    // -- 1st Order Hexs
            {
                for (int j = 0; j < 4; ++j)
                {
                    //qDebug() << "j: " << j << "\tface: " << face;
                    //qDebug() << "tetFaces: " << tetFaces[face][j];

                    node = h1elements[element].connect[ hexFaces[face][j] ];
                    cox = (GLfloat) (nodes[node].x + instances[inst].expand[0] + instances[inst].extraExpand[0]);
                    coy = (GLfloat) (nodes[node].y + instances[inst].expand[1] + instances[inst].extraExpand[1]);
                    coz = (GLfloat) (nodes[node].z + instances[inst].expand[2] + instances[inst].extraExpand[2]);
                    // Store the node position.
                    surfaceNodePosition4.push_back(cox);
                    surfaceNodePosition4.push_back(coy);
                    surfaceNodePosition4.push_back(coz);
                }
            }                       // -- end etype == 2
            else if (etype == 5)    // -- 2nd Order Hexs
            {
                for (int j = 0; j < 4; ++j)
                {
                    //qDebug() << "j: " << j << "\tface: " << face;
                    //qDebug() << "tetFaces: " << tetFaces[face][j];

                    node = h2elements[element].connect[ hexFaces[face][j] ];
                    cox = (GLfloat) (nodes[node].x + instances[inst].expand[0] + instances[inst].extraExpand[0]);
                    coy = (GLfloat) (nodes[node].y + instances[inst].expand[1] + instances[inst].extraExpand[1]);
                    coz = (GLfloat) (nodes[node].z + instances[inst].expand[2] + instances[inst].extraExpand[2]);
                    // Store the node position.
                    surfaceNodePosition4.push_back(cox);
                    surfaceNodePosition4.push_back(coy);
                    surfaceNodePosition4.push_back(coz);
                }
            }                       // -- end etype == 5
            else if (etype == 1)    // -- 1st Order Pents
            {
                if (face < 2) jlim=3;
                else jlim = 4;

                for (int j = 0; j < jlim; ++j)
                {
                    //qDebug() << "j: " << j << "\tface: " << face;
                    //qDebug() << "tetFaces: " << tetFaces[face][j];

                    node = p1elements[element].connect[ pentFaces[face][j] ];
                    cox = (GLfloat) (nodes[node].x + instances[inst].expand[0] + instances[inst].extraExpand[0]);
                    coy = (GLfloat) (nodes[node].y + instances[inst].expand[1] + instances[inst].extraExpand[1]);
                    coz = (GLfloat) (nodes[node].z + instances[inst].expand[2] + instances[inst].extraExpand[2]);

               /*     if (node3 < 3)
                    {
                        qDebug() << "setupSurfaceNodePositions:: element: " << element;
                        qDebug() << "x y z: " << cox << "\t" << coy << "\t" << coz;
                    } */
                    ++node3;


                    // Store the node position.
                    if (jlim == 3)
                    {
                //        fprintf(fp2, "%d\tnode3: %4d  element: %6d  XYZ: %f\t %f\t %f\n ", etype, node3, element, cox, coy, coz);

                        surfaceNodePosition3.push_back(cox);
                        surfaceNodePosition3.push_back(coy);
                        surfaceNodePosition3.push_back(coz);
                    }
                    else
                    {
                        surfaceNodePosition4.push_back(cox);
                        surfaceNodePosition4.push_back(coy);
                        surfaceNodePosition4.push_back(coz);
                    }
                }
            }                       // -- end etype == 1
            else if (etype == 4)    // -- 2nd Order Pents
            {
                if (face < 2) jlim=3;
                else jlim = 4;

                for (int j = 0; j < jlim; ++j)
                {
                    //qDebug() << "j: " << j << "\tface: " << face;
                    //qDebug() << "tetFaces: " << tetFaces[face][j];

                    node = p2elements[element].connect[ pentFaces[face][j] ];
                    cox = (GLfloat) (nodes[node].x + instances[inst].expand[0] + instances[inst].extraExpand[0]);
                    coy = (GLfloat) (nodes[node].y + instances[inst].expand[1] + instances[inst].extraExpand[1]);
                    coz = (GLfloat) (nodes[node].z + instances[inst].expand[2] + instances[inst].extraExpand[2]);
                    // Store the node position.
                    if (jlim == 3)
                    {
              //          fprintf(fp2, "%d\tnode3: %4d  element: %6d  XYZ: %f\t %f\t %f\n ", etype, node3, element, cox, coy, coz);

                        surfaceNodePosition3.push_back(cox);
                        surfaceNodePosition3.push_back(coy);
                        surfaceNodePosition3.push_back(coz);
                    }
                    else
                    {
                        surfaceNodePosition4.push_back(cox);
                        surfaceNodePosition4.push_back(coy);
                        surfaceNodePosition4.push_back(coz);
                    }
                }
            }                       // -- end etype == 4
        }                           // -- end k-loop for klim entries in current instance inst
    }                               // -- end inst loop

  //  fclose(fp2);

    //qDebug() << "-- size of surfaceNodePosition3: " << surfaceNodePosition3.size();
    //qDebug() << "-- size of surfaceNodePosition4: " << surfaceNodePosition4.size();

    //for (int i=0; i<surfaceNodePosition3.size(); ++i)
    //{
    //    qDebug() << "i: " << i << "\t" << surfaceNodePosition3[i];
    //}
}

// ------------------------------------------------------------------------------------------------
void UMObject::setupSurfaceNodeNormals()
{
    int     element;
    int     inst;
    int     jlim;
    int     klim;
    short   face;
    short   etype;
    GLfloat cox, coy, coz;

    surfaceNodeNormal3.clear();
    surfaceNodeNormal4.clear();

    for (inst=0; inst < nInstances; ++inst)
    {
        klim = instances[inst].surface.size();
        for(int k = 0; k<klim; ++k)
        {
            element = instances[inst].surface[k].element;
            face    = instances[inst].surface[k].face;
            etype   = instances[inst].surface[k].type;

            //qDebug() << "setupSurfaceNodeNormals:: element: " << element << "\t eType: " << etype;

            if (etype == 0)         // -- 1st Order Tets
            {
                for (int j = 0; j < 3; ++j)
                {
                    cox = (GLfloat) (t1elements[element].normal[face][0]);
                    coy = (GLfloat) (t1elements[element].normal[face][1]);
                    coz = (GLfloat) (t1elements[element].normal[face][2]);
                    // Store the node normal.
                    surfaceNodeNormal3.push_back(cox);
                    surfaceNodeNormal3.push_back(coy);
                    surfaceNodeNormal3.push_back(coz);
                }
            }                       // -- end etype == 0
            else if (etype == 3)    // -- 2nd Order Tets
            {
                for (int j = 0; j < 3; ++j)
                {
                    cox = (GLfloat) (t2elements[element].normal[face][0]);
                    coy = (GLfloat) (t2elements[element].normal[face][1]);
                    coz = (GLfloat) (t2elements[element].normal[face][2]);
                    // Store the node normal.
                    surfaceNodeNormal3.push_back(cox);
                    surfaceNodeNormal3.push_back(coy);
                    surfaceNodeNormal3.push_back(coz);
                }
            }                       // -- end etype == 3
            else if (etype == 2)    // -- 1st Order Hexs
            {
                for (int j = 0; j < 4; ++j)
                {
                    cox = (GLfloat) (h1elements[element].normal[face][0]);
                    coy = (GLfloat) (h1elements[element].normal[face][1]);
                    coz = (GLfloat) (h1elements[element].normal[face][2]);
                    // Store the node normal.
                    surfaceNodeNormal4.push_back(cox);
                    surfaceNodeNormal4.push_back(coy);
                    surfaceNodeNormal4.push_back(coz);
                }
            }                       // -- end etype == 2
            else if (etype == 5)    // -- 2nd Order Hexs
            {
                for (int j = 0; j < 4; ++j)
                {
                    cox = (GLfloat) (h2elements[element].normal[face][0]);
                    coy = (GLfloat) (h2elements[element].normal[face][1]);
                    coz = (GLfloat) (h2elements[element].normal[face][2]);
                    // Store the node normal.
                    surfaceNodeNormal4.push_back(cox);
                    surfaceNodeNormal4.push_back(coy);
                    surfaceNodeNormal4.push_back(coz);
                }
            }                       // -- end etype == 5
            else if (etype == 1)    // -- 1st Order Pents
            {
                if (face < 2) jlim=3;
                else jlim = 4;

                for (int j = 0; j < jlim; ++j)
                {
                    cox = (GLfloat) (p1elements[element].normal[face][0]);
                    coy = (GLfloat) (p1elements[element].normal[face][1]);
                    coz = (GLfloat) (p1elements[element].normal[face][2]);
                    // Store the node normal.
                    if (jlim == 3)
                    {
                        surfaceNodeNormal3.push_back(cox);
                        surfaceNodeNormal3.push_back(coy);
                        surfaceNodeNormal3.push_back(coz);
                    }
                    else
                    {
                        surfaceNodeNormal4.push_back(cox);
                        surfaceNodeNormal4.push_back(coy);
                        surfaceNodeNormal4.push_back(coz);
                    }
                }
            }                       // -- end etype == 1
            else if (etype == 4)    // -- 2nd Order Pents
            {
                if (face < 2) jlim=3;
                else jlim = 4;

                for (int j = 0; j < jlim; ++j)
                {
                    cox = (GLfloat) (p2elements[element].normal[face][0]);
                    coy = (GLfloat) (p2elements[element].normal[face][1]);
                    coz = (GLfloat) (p2elements[element].normal[face][2]);
                    // Store the node normal.
                    if (jlim == 3)
                    {
                        surfaceNodeNormal3.push_back(cox);
                        surfaceNodeNormal3.push_back(coy);
                        surfaceNodeNormal3.push_back(coz);
                    }
                    else
                    {
                        surfaceNodeNormal4.push_back(cox);
                        surfaceNodeNormal4.push_back(coy);
                        surfaceNodeNormal4.push_back(coz);
                    }
                }
            }                       // -- end etype == 4
        }                           // -- end k-loop for klim entries in current instance inst
    }                               // -- end inst loop
}

// ------------------------------------------------------------------------------------------------
void  UMObject::setupSurfaceNodeColors(int intervalType, int plotType, int colorChoice, int contourType)
{
    double  x;
    int     element;
    int     index;
    int     inst;
    int     jlim;
    int     klim;
    int     matNo;
    int     node;
    short   face;
    short   etype;
    GLfloat cox, coy, coz;

    QColor  color1;
    int     which = plotType - 1;
    int     aSize = resultBins.size();

    //qDebug() << "***** setupSurfaceNodeColors *****";

    //qDebug() << "setupSurfaceNodeColors:: intervalType:" << intervalType;
    //qDebug() << "setupSurfaceNodeColors::     plotType:" << plotType;
    //qDebug() << "setupSurfaceNodeColors::  colorChoice:" << colorChoice;
    //qDebug() << "setupSurfaceNodeColors::  contourType:" << contourType;

    surfaceNodeColor3.clear();
    surfaceNodeColor4.clear();

    for (inst=0; inst < nInstances; ++inst)
    {
        klim = instances[inst].surface.size();
        for(int k = 0; k<klim; ++k)
        {
            element = instances[inst].surface[k].element;
            face    = instances[inst].surface[k].face;
            etype   = instances[inst].surface[k].type;

            if (etype == 0)         // -- 1st Order Tets
            {
                matNo  = t1elements[element].material;
                matNo  = matMap[matNo];

                if (contourType == 0)
                {
                    // ------------------------------------
                    // - QUILTED results color assignment -
                    // ------------------------------------
                    if (which > - 1)
                    {
                        x = t1elements[element].results[which];
                        if (intervalType == 1)
                        {
                            x = log(x);
                        }
                        else if (intervalType == 2)
                        {
                            x = log10(x);
                        }
                        index = searchBins(x, resultBins, aSize);
                        color1 = resultColors[index];
                    }
                    // -----------------------------
                    // - GEOMTERY color assignment -
                    // -----------------------------
                    else
                    {
                        if (colorChoice == 0)
                            color1 = materials[matNo].color;
                        else
                            color1 = instances[inst].color;
                    }
                }           // -- end if contourType == 0

                if (contourType == 1)
                {
                    // ------------------------------------
                    // - BLENDED results color assignment -
                    // ------------------------------------
                    for (int j = 0; j < 3; ++j)
                    {
                        node  = t1elements[element].connect[ tetFaces[face][j] ];
                        x     = nodes[node].results[which];
                        if (intervalType == 1)
                        {
                            x = log(x);
                        }
                        else if (intervalType == 2)
                        {
                            x = log10(x);
                        }
                        index = searchBins(x, resultBins, aSize);
                        color1 = resultColors[index];
                        // Store the node color.
                        surfaceNodeColor3.push_back((GLfloat) color1.redF());
                        surfaceNodeColor3.push_back((GLfloat) color1.greenF());
                        surfaceNodeColor3.push_back((GLfloat) color1.blueF());
                        surfaceNodeColor3.push_back((GLfloat) color1.alphaF());
                    }
                }
                else
                {
                    for (int j = 0; j < 3; ++j)
                    {
                        // Store the node color.
                        surfaceNodeColor3.push_back((GLfloat) color1.redF());
                        surfaceNodeColor3.push_back((GLfloat) color1.greenF());
                        surfaceNodeColor3.push_back((GLfloat) color1.blueF());
                        surfaceNodeColor3.push_back((GLfloat) color1.alphaF());
                    }
                }
            }                       // -- end if etype == 0
            else if (etype == 3)    // -- 2nd Order Tets
            {
                matNo  = t2elements[element].material;
                matNo  = matMap[matNo];

                if (contourType == 0)
                {
                    // ------------------------------------
                    // - QUILTED results color assignment -
                    // ------------------------------------
                    if (which > - 1)
                    {
                        x = t2elements[element].results[which];
                        if (intervalType == 1)
                        {
                            x = log(x);
                        }
                        else if (intervalType == 2)
                        {
                            x = log10(x);
                        }
                        index = searchBins(x, resultBins, aSize);
                        color1 = resultColors[index];

                        //qDebug() << "k: " << k << "\tindex: " << index << "\t result: " << x;

                    }
                    // -----------------------------
                    // - GEOMTERY color assignment -
                    // -----------------------------
                    else
                    {
                        if (colorChoice == 0)
                            color1 = materials[matNo].color;
                        else
                            color1 = instances[inst].color;
                    }
                }           // -- end if contourType == 0

                if (contourType == 1)
                {
                    // ------------------------------------
                    // - BLENDED results color assignment -
                    // ------------------------------------
                    for (int j = 0; j < 3; ++j)
                    {
                        node  = t2elements[element].connect[ tetFaces[face][j] ];
                        x     = nodes[node].results[which];
                        if (intervalType == 1)
                        {
                            x = log(x);
                        }
                        else if (intervalType == 2)
                        {
                            x = log10(x);
                        }
                        index = searchBins(x, resultBins, aSize);
                        color1 = resultColors[index];
                        // Store the node color.
                        surfaceNodeColor3.push_back((GLfloat) color1.redF());
                        surfaceNodeColor3.push_back((GLfloat) color1.greenF());
                        surfaceNodeColor3.push_back((GLfloat) color1.blueF());
                        surfaceNodeColor3.push_back((GLfloat) color1.alphaF());
                    }
                }
                else
                {
                    for (int j = 0; j < 3; ++j)
                    {
                        // Store the node color.
                        surfaceNodeColor3.push_back((GLfloat) color1.redF());
                        surfaceNodeColor3.push_back((GLfloat) color1.greenF());
                        surfaceNodeColor3.push_back((GLfloat) color1.blueF());
                        surfaceNodeColor3.push_back((GLfloat) color1.alphaF());
                    }
                }
            }                       // -- end if etype == 3
            else if (etype == 2)    // -- 1st Order Hexs
            {
                matNo  = h1elements[element].material;
                matNo  = matMap[matNo];

                if (contourType == 0)
                {
                    // ------------------------------------
                    // - QUILTED results color assignment -
                    // ------------------------------------
                    if (which > - 1)
                    {
                        x = h1elements[element].results[which];
                        if (intervalType == 1)
                        {
                            x = log(x);
                        }
                        else if (intervalType == 2)
                        {
                            x = log10(x);
                        }

                        //qDebug() << "resultBins size:: " << resultBins.size();
                        //qDebug() << "aSize: " << aSize;


                        index = searchBins(x, resultBins, aSize);
                        color1 = resultColors[index];
                        //if (x > 0.0184)
                        //qDebug() << "x: " << x << "\tindex: " << index << "\tcolor: " << color1;
                    }
                    // -----------------------------
                    // - GEOMTERY color assignment -
                    // -----------------------------
                    else
                    {
                        if (colorChoice == 0)
                            color1 = materials[matNo].color;
                        else
                            color1 = instances[inst].color;
                    }
                }           // -- end if contourType == 0

                if (contourType == 1)
                {
                    // ------------------------------------
                    // - BLENDED results color assignment -
                    // ------------------------------------
                    for (int j = 0; j < 4; ++j)
                    {
                        node  = h1elements[element].connect[ hexFaces[face][j] ];
                        x     = nodes[node].results[which];
                        if (intervalType == 1)
                        {
                            x = log(x);
                        }
                        else if (intervalType == 2)
                        {
                            x = log10(x);
                        }
                        index = searchBins(x, resultBins, aSize);
                        color1 = resultColors[index];
                        // Store the node color.
                        surfaceNodeColor4.push_back((GLfloat) color1.redF());
                        surfaceNodeColor4.push_back((GLfloat) color1.greenF());
                        surfaceNodeColor4.push_back((GLfloat) color1.blueF());
                        surfaceNodeColor4.push_back((GLfloat) color1.alphaF());
                    }
                }
                else
                {
                    //qDebug() << inst << colorChoice  << which << " rgb: " << color1.red() << "\t" << color1.green() << "\t" << color1.blue();
                    for (int j = 0; j < 4; ++j)
                    {
                        // Store the node color.
                        surfaceNodeColor4.push_back((GLfloat) color1.redF());
                        surfaceNodeColor4.push_back((GLfloat) color1.greenF());
                        surfaceNodeColor4.push_back((GLfloat) color1.blueF());
                        surfaceNodeColor4.push_back((GLfloat) color1.alphaF());
                    }
                }
            }                       // -- end if etype == 2
            else if (etype == 5)    // -- 2nd Order Hexs
            {
                matNo  = h2elements[element].material;
                matNo  = matMap[matNo];

                if (contourType == 0)
                {
                    // ------------------------------------
                    // - QUILTED results color assignment -
                    // ------------------------------------
                    if (which > - 1)
                    {
                        x = h2elements[element].results[which];
                        if (intervalType == 1)
                        {
                            x = log(x);
                        }
                        else if (intervalType == 2)
                        {
                            x = log10(x);
                        }
                        index = searchBins(x, resultBins, aSize);
                        color1 = resultColors[index];
                    }
                    // -----------------------------
                    // - GEOMTERY color assignment -
                    // -----------------------------
                    else
                    {
                        if (colorChoice == 0)
                            color1 = materials[matNo].color;
                        else
                            color1 = instances[inst].color;
                    }
                }           // -- end if contourType == 0

                if (contourType == 1)
                {
                    // ------------------------------------
                    // - BLENDED results color assignment -
                    // ------------------------------------
                    for (int j = 0; j < 4; ++j)
                    {
                        node  = h2elements[element].connect[ hexFaces[face][j] ];
                        x     = nodes[node].results[which];
                        if (intervalType == 1)
                        {
                            x = log(x);
                        }
                        else if (intervalType == 2)
                        {
                            x = log10(x);
                        }
                        index = searchBins(x, resultBins, aSize);
                        color1 = resultColors[index];
                        // Store the node color.
                        surfaceNodeColor4.push_back((GLfloat) color1.redF());
                        surfaceNodeColor4.push_back((GLfloat) color1.greenF());
                        surfaceNodeColor4.push_back((GLfloat) color1.blueF());
                        surfaceNodeColor4.push_back((GLfloat) color1.alphaF());
                    }
                }
                else
                {
                    for (int j = 0; j < 4; ++j)
                    {
                        // Store the node color.
                        surfaceNodeColor4.push_back((GLfloat) color1.redF());
                        surfaceNodeColor4.push_back((GLfloat) color1.greenF());
                        surfaceNodeColor4.push_back((GLfloat) color1.blueF());
                        surfaceNodeColor4.push_back((GLfloat) color1.alphaF());
                    }
                }
            }                       // -- end if etype == 5
            else if (etype == 1)    // -- 1st Order Pents
            {
                matNo  = p1elements[element].material;
                matNo  = matMap[matNo];

                if (contourType == 0)
                {
                    // ------------------------------------
                    // - QUILTED results color assignment -
                    // ------------------------------------
                    if (which > - 1)
                    {
                        x = p1elements[element].results[which];
                        if (intervalType == 1)
                        {
                            x = log(x);
                        }
                        else if (intervalType == 2)
                        {
                            x = log10(x);
                        }
                        index = searchBins(x, resultBins, aSize);
                        color1 = resultColors[index];
                    }
                    // -----------------------------
                    // - GEOMTERY color assignment -
                    // -----------------------------
                    else
                    {
                        if (colorChoice == 0)
                            color1 = materials[matNo].color;
                        else
                            color1 = instances[inst].color;
                    }
                }           // -- end if contourType == 0

                if (face < 2) jlim = 3;
                else jlim = 4;

                if (contourType == 1)
                {
                    // ------------------------------------
                    // - BLENDED results color assignment -
                    // ------------------------------------
                    for (int j = 0; j < jlim; ++j)
                    {
                        node  = p1elements[element].connect[ pentFaces[face][j] ];
                        x     = nodes[node].results[which];
                        if (intervalType == 1)
                        {
                            x = log(x);
                        }
                        else if (intervalType == 2)
                        {
                            x = log10(x);
                        }
                        index = searchBins(x, resultBins, aSize);
                        color1 = resultColors[index];
                        // Store the node color.
                        if (jlim == 3)
                        {
                            surfaceNodeColor3.push_back((GLfloat) color1.redF());
                            surfaceNodeColor3.push_back((GLfloat) color1.greenF());
                            surfaceNodeColor3.push_back((GLfloat) color1.blueF());
                            surfaceNodeColor3.push_back((GLfloat) color1.alphaF());
                        }
                        else
                        {
                            surfaceNodeColor4.push_back((GLfloat) color1.redF());
                            surfaceNodeColor4.push_back((GLfloat) color1.greenF());
                            surfaceNodeColor4.push_back((GLfloat) color1.blueF());
                            surfaceNodeColor4.push_back((GLfloat) color1.alphaF());
                        }
                    }               // end j-loop
                }
                else
                {
                    if (jlim == 3)
                    {
                        for (int j = 0; j < jlim; ++j)
                        {
                            // Store the node color.
                            surfaceNodeColor3.push_back((GLfloat) color1.redF());
                            surfaceNodeColor3.push_back((GLfloat) color1.greenF());
                            surfaceNodeColor3.push_back((GLfloat) color1.blueF());
                            surfaceNodeColor3.push_back((GLfloat) color1.alphaF());
                        }
                    }
                    else
                    {
                        for (int j = 0; j < jlim; ++j)
                        {
                            // Store the node color.
                            surfaceNodeColor4.push_back((GLfloat) color1.redF());
                            surfaceNodeColor4.push_back((GLfloat) color1.greenF());
                            surfaceNodeColor4.push_back((GLfloat) color1.blueF());
                            surfaceNodeColor4.push_back((GLfloat) color1.alphaF());
                        }
                    }
                }
            }                       // -- end if etype == 1
            else if (etype == 4)    // -- 2nd Order Pents
            {
                matNo  = p2elements[element].material;
                matNo  = matMap[matNo];

                if (contourType == 0)
                {
                    // ------------------------------------
                    // - QUILTED results color assignment -
                    // ------------------------------------
                    if (which > - 1)
                    {
                        x = p2elements[element].results[which];
                        if (intervalType == 1)
                        {
                            x = log(x);
                        }
                        else if (intervalType == 2)
                        {
                            x = log10(x);
                        }
                        index = searchBins(x, resultBins, aSize);
                        color1 = resultColors[index];
                    }
                    // -----------------------------
                    // - GEOMTERY color assignment -
                    // -----------------------------
                    else
                    {
                        if (colorChoice == 0)
                            color1 = materials[matNo].color;
                        else
                            color1 = instances[inst].color;
                    }
                }           // -- end if contourType == 0

                if (face < 2) jlim = 3;
                else jlim = 4;

                if (contourType == 1)
                {
                    // ------------------------------------
                    // - BLENDED results color assignment -
                    // ------------------------------------
                    for (int j = 0; j < jlim; ++j)
                    {
                        node  = p2elements[element].connect[ pentFaces[face][j] ];
                        x     = nodes[node].results[which];
                        if (intervalType == 1)
                        {
                            x = log(x);
                        }
                        else if (intervalType == 2)
                        {
                            x = log10(x);
                        }
                        index = searchBins(x, resultBins, aSize);
                        color1 = resultColors[index];
                        // Store the node color.
                        if (jlim == 3)
                        {
                            surfaceNodeColor3.push_back((GLfloat) color1.redF());
                            surfaceNodeColor3.push_back((GLfloat) color1.greenF());
                            surfaceNodeColor3.push_back((GLfloat) color1.blueF());
                            surfaceNodeColor3.push_back((GLfloat) color1.alphaF());
                        }
                        else
                        {
                            surfaceNodeColor4.push_back((GLfloat) color1.redF());
                            surfaceNodeColor4.push_back((GLfloat) color1.greenF());
                            surfaceNodeColor4.push_back((GLfloat) color1.blueF());
                            surfaceNodeColor4.push_back((GLfloat) color1.alphaF());
                        }
                    }               // end j-loop
                }
                else
                {
                    if (jlim == 3)
                    {
                        for (int j = 0; j < jlim; ++j)
                        {
                            // Store the node color.
                            surfaceNodeColor3.push_back((GLfloat) color1.redF());
                            surfaceNodeColor3.push_back((GLfloat) color1.greenF());
                            surfaceNodeColor3.push_back((GLfloat) color1.blueF());
                            surfaceNodeColor3.push_back((GLfloat) color1.alphaF());
                        }
                    }
                    else
                    {
                        for (int j = 0; j < jlim; ++j)
                        {
                            // Store the node color.
                            surfaceNodeColor4.push_back((GLfloat) color1.redF());
                            surfaceNodeColor4.push_back((GLfloat) color1.greenF());
                            surfaceNodeColor4.push_back((GLfloat) color1.blueF());
                            surfaceNodeColor4.push_back((GLfloat) color1.alphaF());
                        }
                    }
                }
            }                       // -- end if etype == 4
        }                           // -- end k-loop
    }                               // -- end inst loop
}

// ------------------------------------------------------------------------------------------------
void  UMObject::setupSurfaceIndices(int colorChoice)
{
    int     element;
    int     inst;
    int     klim;
    int     matNo;
    GLint   node3, node4;
    short   etype;
    short   face;
    bool    masterDisplay;

//    GLfloat cox, coy, coz;

    node3 = -1;
    node4 = -1;

    surfaceIndices3.clear();
    surfaceIndices4.clear();
    surfaceInstances3.clear();
    surfaceInstances4.clear();

    //qDebug() << " -- nInsances: " << nInstances;

    for (inst=0; inst < nInstances; ++inst)
    {
        klim = instances[inst].surface.size();

        //qDebug() << " -- inst: " << inst << "\tklim: " << klim;

        for(int k = 0; k<klim; ++k)
        {
            element = instances[inst].surface[k].element;
            etype   = instances[inst].surface[k].type;
            face    = instances[inst].surface[k].face;

            //if (k < 2) qDebug() << " -- element: " << element;
            //if (k < 2) qDebug() << " -- etype  : " << etype;

            if (etype == 0)         // -- 1st Order Tets
            {
                matNo  = t1elements[element].material;
                matNo  = matMap[matNo];
                if (colorChoice == 1)
                    masterDisplay = instances[inst].display;
                else
                    masterDisplay = materials[matNo].display;

                //qDebug() << "--- etype == 0 ---:: node3: " << node3;
                //qDebug() << "displayElement: " << t1elements[element].displayElement;
                if (masterDisplay && (t1elements[element].displayElement))
                {
                    for (int j = 0; j < 3; ++j)
                    {
                        ++node3;

                 //   node = t1elements[element].connect[ tetFaces[face][j] ];
                 //   cox = (GLfloat) (nodes[node].x + instances[inst].expand[0]);
                 //   coy = (GLfloat) (nodes[node].y + instances[inst].expand[1]);
                 //   coz = (GLfloat) (nodes[node].z + instances[inst].expand[2]);

                        // Store the node index.
                        surfaceIndices3.push_back(node3);
                  /*      if (node3 < 3)
                        {
                            qDebug() << "setupSurfaceIndices :: element:" << element;
                            qDebug() << "X Y Z: " << cox << "\t" << coy << "\t" << coz;
                        } */

                    }
                }
                else
                {
                    // Increment the node index for the skipped node.
                    node3 += 3;
                }
            }                       // -- end etype == 0
            else if (etype == 3)    // -- 2nd Order Tets
            {
                matNo  = t2elements[element].material;
                matNo  = matMap[matNo];
                if (colorChoice == 1)
                    masterDisplay = instances[inst].display;
                else
                    masterDisplay = materials[matNo].display;

                //qDebug() << "displayElement: " << t2elements[element].displayElement;
                if (masterDisplay && (t2elements[element].displayElement))
                {
                    for (int j = 0; j < 3; ++j)
                    {
                        ++node3;
                        // Store the node index.
                        surfaceIndices3.push_back(node3);
                    }
                }
                else
                {
                    // Increment the node index for the skipped node.
                    node3 += 3;
                }
            }                       // -- end etype == 3
            else if (etype == 2)    // -- 1st Order Hexs
            {
                matNo  = h1elements[element].material;
                matNo  = matMap[matNo];
                if (colorChoice == 1)
                    masterDisplay = instances[inst].display;
                else
                    masterDisplay = materials[matNo].display;

                //qDebug() << "displayElement: " << t2elements[element].displayElement;
                if (masterDisplay && (h1elements[element].displayElement))
                {
                    for (int j = 0; j < 4; ++j)
                    {
                        ++node4;
                        // Store the node index.
                        surfaceIndices4.push_back(node4);
                    }
                }
                else
                {
                    // Increment the node index for the skipped node.
                    node4 += 4;
                }
            }                       // -- end etype == 2
            else if (etype == 5)    // -- 2nd Order Hexs
            {
                matNo  = h2elements[element].material;
                matNo  = matMap[matNo];
                if (colorChoice == 1)
                    masterDisplay = instances[inst].display;
                else
                    masterDisplay = materials[matNo].display;

                //qDebug() << "displayElement: " << t2elements[element].displayElement;
                if (masterDisplay && (h2elements[element].displayElement))
                {
                    for (int j = 0; j < 4; ++j)
                    {
                        ++node4;
                        // Store the node index.
                        surfaceIndices4.push_back(node4);
                    }
                }
                else
                {
                    // Increment the node index for the skipped node.
                    node4 += 4;
                }
            }                       // -- end etype == 5
            else if (etype == 1)    // -- 1st Order Pents
            {
                matNo  = p1elements[element].material;
                matNo  = matMap[matNo];
                if (colorChoice == 1)
                    masterDisplay = instances[inst].display;
                else
                    masterDisplay = materials[matNo].display;

                face    = instances[inst].surface[k].face;
                if (face < 2)
                {
                    //qDebug() << "displayElement 1st pent: " << p1elements[element].displayElement << "\t el: " << element;
                    if (masterDisplay && (p1elements[element].displayElement))
                    {
                        for (int j = 0; j < 3; ++j)
                        {

           //         node = p1elements[element].connect[ pentFaces[face][j] ];
           //         cox = (GLfloat) (nodes[node].x + instances[inst].expand[0]);
           //         coy = (GLfloat) (nodes[node].y + instances[inst].expand[1]);
          //          coz = (GLfloat) (nodes[node].z + instances[inst].expand[2]);


                            ++node3;
                            // Store the node index.
                            surfaceIndices3.push_back(node3);
                  /*          if (node3 < 3)
                            {
                                qDebug() << "setupSurfaceIndices :: element:" << element;
                                qDebug() << "X Y Z: " << cox << "\t" << coy << "\t" << coz;
                            }  */
                        }
                    }
                    else
                    {
                        // Increment the node index for the skipped node.
                        node3 += 3;
                    }
                }                   // -- end else: face > 1
                else
                {
                    if (masterDisplay && (p1elements[element].displayElement))
                    {
                        for (int j = 0; j < 4; ++j)
                        {
                            ++node4;
                            // Store the node index.
                            surfaceIndices4.push_back(node4);
                        }
                    }
                    else
                    {
                        // Increment the node index for the skipped node.
                        node4 += 4;
                    }
                }                   // -- end else: face > 1
            }                       // -- end etype == 1
           else if (etype == 4)     // -- 2nd Order Pents
            {
                matNo  = p2elements[element].material;
                matNo  = matMap[matNo];
                if (colorChoice == 1)
                    masterDisplay = instances[inst].display;
                else
                    masterDisplay = materials[matNo].display;

                face    = instances[inst].surface[k].face;
                if (face < 2)
                {
                    if (masterDisplay && (p2elements[element].displayElement))
                    {
                        for (int j = 0; j < 3; ++j)
                        {
                            ++node3;
                            // Store the node index.
                            surfaceIndices3.push_back(node3);
                        }
                    }
                    else
                    {
                        // Increment the node index for the skipped node.
                        node3 += 3;
                    }
                }                   // -- end else: face > 1
                else
                {
                    if (masterDisplay && (p2elements[element].displayElement))
                    {
                        for (int j = 0; j < 4; ++j)
                        {
                            ++node4;
                            // Store the node index.
                            surfaceIndices4.push_back(node4);
                        }
                    }
                    else
                    {
                        // Increment the node index for the skipped node.
                        node4 += 4;
                    }
                }                   // -- end else: face > 1
            }                       // -- end etype == 2
        }                           // -- end k-loop for klim entries in current instance inst

        //qDebug() << "inst: " << inst << "\t\t size of surfaceIndices3: " << surfaceIndices3.size();
        //qDebug() << "node 3: " << node3 << "\t4: " << node4;

        surfaceInstances3.push_back( surfaceIndices3.size() );
        surfaceInstances4.push_back( surfaceIndices4.size() );

    }                   // -- end inst loop

    //qDebug() << "-- size of Indices3: " << surfaceIndices3.size();
    //for (int i=0; i<surfaceIndices3.size(); ++i)
    //{
    //    qDebug() << "index: " << surfaceIndices3[i];
    //}
}
// ------------------------------------------------------------------------------------------------
void  UMObject::clearVectors()
{
   //qDebug() << "--- clearVectors ---";


    t1elNodesPosition.clear();
    t1elNodesColor.clear();
    t1elNodesNormal.clear();

    p1elNodesPosition3.clear();
    p1elNodesColor3.clear();
    p1elNodesNormal3.clear();
    p1elNodesPosition4.clear();
    p1elNodesColor4.clear();
    p1elNodesNormal4.clear();

    h1elNodesPosition.clear();
    h1elNodesColor.clear();
    h1elNodesNormal.clear();

    t2elNodesPosition.clear();
    t2elNodesColor.clear();
    t2elNodesNormal.clear();

    p2elNodesPosition3.clear();
    p2elNodesColor3.clear();
    p2elNodesNormal3.clear();
    p2elNodesPosition4.clear();
    p2elNodesColor4.clear();
    p2elNodesNormal4.clear();

    h2elNodesPosition.clear();
    h2elNodesColor.clear();
    h2elNodesNormal.clear();

    t1elNodesIndices.clear();
    t2elNodesIndices.clear();
    h1elNodesIndices.clear();
    h2elNodesIndices.clear();
    p1elNodesIndices3.clear();
    p2elNodesIndices3.clear();
    p1elNodesIndices4.clear();
    p2elNodesIndices4.clear();

    surfaceNodePosition3.clear();
    surfaceNodePosition4.clear();
    surfaceNodeNormal3.clear();
    surfaceNodeNormal4.clear();
    surfaceNodeColor3.clear();
    surfaceNodeColor4.clear();

    surfaceIndices3.clear();
    surfaceIndices4.clear();
    surfaceInstances3.clear();
    surfaceInstances4.clear();

    annotatePoints.clear();
    annotateLines.clear();
    annotateStrings.clear();

    //qDebug() << "h1elNodesIndices  size: " << h1elNodesIndices.size() ;
    //qDebug() << "t2elNodesIndices  size: " << h2elNodesIndices.size() ;
    //qDebug() << "--- END clearVectors ---";
}

