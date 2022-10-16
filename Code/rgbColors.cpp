#include "rgbColors.h"

void setRGBcolors(int maxColors, int numColors, vector<int> &r, vector<int> &g, vector<int> &b)
{
  //
  //  Function to create a default color assignments for the numColors requested.
  //  Colors are chosen along the edges of the color cube going from red to
  //  yellow to green to cyan to blue to magenta.  This routine tries to
  //  evenly distribute the RGB choices along the edge-path.
  //

  int nSides;                   // # of sides of the color cube to use
  int ncte2;                    // working counter for # of color table entries
  int mr;                       // # remaining after mod arithmatic
  int m5;
  int i;
  int j;
  int k;
  int m;

  int *nc     = new int[5];
  int *ccoder = new int[5];
  int *ccodeg = new int[5];
  int *ccodeb = new int[5];
  int *xr     = new int[5];
  int *xg     = new int[5];
  int *xb     = new int[5];

  int *specr = new int[10];
  int *specg = new int[10];
  int *specb = new int[10];


  nSides    =  5;
  nc[0]     =  0; nc[1]     =  0; nc[2]     =  0; nc[3]     =  0; nc[4]     =  0;
  ccoder[0] =  0; ccoder[1] = -1; ccoder[2] =  0; ccoder[3] =  0; ccoder[4] =  1;
  ccodeg[0] =  1; ccodeg[1] =  0; ccodeg[2] =  0; ccodeg[3] = -1; ccodeg[4] =  0;
  ccodeb[0] =  0; ccodeb[1] =  0; ccodeb[2] =  1; ccodeb[3] =  0; ccodeb[4] =  0;

  xr[0] = maxColors; xr[1] =  maxColors; xr[2] =         0; xr[3] =         0; xr[4] =         0;
  xg[0] =         0; xg[1] =  maxColors; xg[2] = maxColors; xg[3] = maxColors; xg[4] =         0;
  xb[0] =         0; xb[1] =          0; xb[2] =         0; xb[3] = maxColors; xb[4] = maxColors;

    // Colors from colorbrewer2.org
    specr[0] = 215;  specg[0] =  25;  specb[0] =  28;  // red
    specr[1] = 255;  specg[1] = 255;  specb[1] = 191;  // cream
    specr[2] =  44;  specg[2] = 123;  specb[2] = 182;  // medium blue
    specr[3] =   0;  specg[3] = 136;  specb[3] =  55;  // medium green
    specr[4] = 192;  specg[4] = 192;  specb[4] = 192;  // silver
    specr[5] = 194;  specg[5] = 165;  specb[5] = 207;  // violet
    specr[6] = 166;  specg[6] =  97;  specb[6] =  26;  // brown
    specr[7] = 171;  specg[7] = 217;  specb[7] = 233;  // sky blue
    specr[8] = 166;  specg[8] = 219;  specb[8] = 160;  // pale green
    specr[9] = 235;  specg[9] = 150;  specb[9] =   5;  // honey


    if (numColors < 1)
    {
        printf(" >>>>> ERROR: Trying to select Colors with less than 1 choice!\n\n");
    }
    else if (numColors < 10)
    {
        for(i=0; i<numColors; ++i)
        {
            r[i] = specr[i];  g[i] = specg[i];  b[i] = specb[i];
        }
    }
  else
  {
    //  Set last color.

    ncte2 = numColors - 1;
    r[ncte2] = maxColors;
    g[ncte2] =         0;
    b[ncte2] = maxColors;

    // qDebug() << "Last Color: " << ncte2 << r[ncte2] << "\t" << g[ncte2] << "\t" << b[ncte2];

    if (ncte2 > 0)
    {
      if (ncte2 < nSides) nSides = ncte2;

      m = 0;
      m5 = ncte2 / nSides;
      mr = ncte2 % nSides ;

      //  Set the # of color points per segment.
      for(i=0; i<nSides; i++)
      {
        nc[i] = m5;
        if (mr > 0)
        {
          nc[i] = nc[i] + 1;
          mr--;
        }
      }             // end i-loop

      //  Assign the RGB values.
      for(i=0; i<nSides; i++)
      {
        if (nc[i] > 0) k = maxColors / nc[i];
        else k = maxColors;

        for(j=0; j<nc[i]; j++)
        {
          r[m] = (xr[i] + ccoder[i] * k * j);
          g[m] = (xg[i] + ccodeg[i] * k * j);
          b[m] = (xb[i] + ccodeb[i] * k * j);

 //         qDebug() << "Color: " << m << r[m] << "\t" << g[m] << "\t" << b[m];
//          qDebug() << "i: " << i << " j: " << j << " k: " << k;
          m++;
        }
      }                 // end i-loop

      if (m != ncte2)
      {
        r[m] = maxColors;
        g[m] =         0;
        b[m] = maxColors;
      }

      //for(m=0; m<numColors; m++)
      //  qDebug() << "Color: " << m << r[m] << "\t" << g[m] << "\t" << b[m];


    }                 // end if ncte2 > 0
  }                   // end else for numColors > 0

  delete [] nc;
  delete [] ccoder;
  delete [] ccodeg;
  delete [] ccodeb;
  delete [] xr;
  delete [] xg;
  delete [] xb;
}                     // end setRGBcolors
// ------------------------------------------------------------------------------------------------
void setGrayScale(int maxColors, int numColors, vector<int> &r, vector<int> &g, vector<int> &b)
{
    if (numColors < 1)
    {
        printf(" >>>>> ERROR: Trying to select Colors with less than 1 choice!\n\n");
    }
    else if (numColors > maxColors)
    {
        printf(" >>>>> ERROR: Trying to gray scale with more thatn 255 values!\n\n");
    }
    else
    {
        int delta;
        delta = maxColors / numColors;
        //qDebug() << "gray scale delta is " << delta;

        r[0] = 0;   g[0] = 0;   b[0] = 0;
        for (int i=1; i<numColors; ++i)
        {
            r[i] = g[i] = b[i] = (i * delta);
        }
    }
}                       // end setGrayScale
// ------------------------------------------------------------------------------------------------
void setRampColors(int maxColors, int numColors, QColor lowColor, QColor hiColor, vector<double> &r, vector<double> &g, vector<double> &b)
{
    double  red_step, green_step, blue_step;
    double  red_low,  green_low,  blue_low;

    if (numColors < 1)
    {
        printf(" >>>>> ERROR: Trying to select Colors with less than 1 choice!\n\n");
    }
    else if (numColors > maxColors)
    {
        printf(" >>>>> ERROR: Trying to gray scale with more thatn 255 values!\n\n");
    }
    else
    {
        double xi;

        red_low   = (double) lowColor.redF();
        green_low = (double) lowColor.greenF();
        blue_low  = (double) lowColor.blueF();
        // Divide the difference in colors by the number of color steps.
        red_step   = (((double) hiColor.redF())   - red_low)   / ((double) numColors);
        green_step = (((double) hiColor.greenF()) - green_low) / ((double) numColors);
        blue_step  = (((double) hiColor.blueF())  - blue_low)  / ((double) numColors);

        //qDebug() << "low color:" << "\t red: " << red_low << "\t gre: " << green_low << "\t blu: " << blue_low;

        for (int i=0; i<numColors; ++i)
        {
            xi = (double) i;
            r[i] = red_low   + red_step * xi;
            g[i] = green_low + green_step * xi;
            b[i] = blue_low  + blue_step * xi;

            //qDebug() << i << "\t red: " << r[i] << "\t gre: " << g[i] << "\t blu: " << b[i];
        }

        //qDebug() << "hi color:" << "\t red: " << hiColor.redF() << "\t gre: " << hiColor.greenF() << "\t blu: " << hiColor.blueF();
    }
}
