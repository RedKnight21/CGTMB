#ifndef DATA_STRUCTS_H
#define DATA_STRUCTS_H

typedef struct
{
    int a, b, c;
} SSS_ABC;

typedef struct
{
    double x, y, z;
} XYZ2;

typedef struct
{
    double u, v, w;
} UVW;

typedef struct
{
    XYZ2 pvec;
    UVW  dvec;
    double rin;
    double rout;
    double dist[3];
    bool   inside;
    bool   twoHits;
    bool   face[6];
    signed char lri;
    signed char lro;

} MagiTrack;



#endif // DATA_STRUCTS_H
