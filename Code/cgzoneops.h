#ifndef CGZONEOPS_H
#define CGZONEOPS_H

// ------------------------------------------------------------------------------------------------

struct CGZoneOps
{
public:
    // Operations to add a body to the cell / zone.
    int boolOp;
    int senseOp;
    int bodyNum;
    int meshReduction;
    bool display;

    bool operator < (const CGZoneOps & cgz) const
    {
        // Return true or false.
//        return (bodyNum < cgz.bodyNum);
        return (senseOp*bodyNum < cgz.senseOp*cgz.bodyNum);
    }
};

#endif // CGZONEOPS_H
