#ifndef CGTMB_CONSTANTS_H
#define CGTMB_CONSTANTS_H

enum BodyContextActionType {NONE, ANDPLUS, ANDMINUS, ORPLUS, ORMINUS, SSS_DEL, EDIT, QUERY};

enum ProjectType {NOPROJECT, MCNP, ITS, XGEN};

struct BodiesForCell
{
    int bodyNo;
    int op;
    unsigned char sign;
    bool          display;
};

#endif // CGTMB_CONSTANTS_H
