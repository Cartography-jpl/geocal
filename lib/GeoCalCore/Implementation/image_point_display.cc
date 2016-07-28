#include "image_point_display.h"

using namespace GeoCal;

void ImagePointDisplay::initialize()
{
  RASTER_FOREACH(*this, i, j)
    write(i, j, 0);

//-----------------------------------------------------------------------
// Create color table for image point.
//-----------------------------------------------------------------------

  GDALColorTable ct;
  GDALColorEntry transparent = {0, 0, 0, 0};
  ct.SetColorEntry(0, &transparent);
  int nstep = 10;
  GDALColorEntry startc = {0, 0, 255, 255};
  GDALColorEntry endc = {255, 0, 0, 255};
  for(int i = 0; i < nstep; ++i) {
    GDALColorEntry ce = 
      {(short int)((endc.c1 - startc.c1) * i / (nstep - 1) + startc.c1),
       (short int)((endc.c2 - startc.c2) * i / (nstep - 1) + startc.c2),
       (short int)((endc.c3 - startc.c3) * i / (nstep - 1) + startc.c3),
       (short int)((endc.c4 - startc.c4) * i / (nstep - 1) + startc.c4)};
    ct.SetColorEntry(i + 1, &ce);
  }
  {
    GDALColorEntry ce = {0xFF, 0xCC, 0x00, 255};
    ct.SetColorEntry(11, &ce);
  }
  {
    GDALColorEntry ce = {0x99, 0xCC, 0x00, 255};
    ct.SetColorEntry(12, &ce);
  }
  {
    GDALColorEntry ce = {0x33, 0x99, 0x00, 255};
    ct.SetColorEntry(13, &ce);
  }
  {
    GDALColorEntry ce = {0x33, 0xff, 0xff, 255};
    ct.SetColorEntry(14, &ce);
  }
  {
    GDALColorEntry ce = {0xff, 0x66, 0x00, 255};
    ct.SetColorEntry(15, &ce);
  }
  {
    GDALColorEntry ce = {0x99, 0x33, 0x99, 255};
    ct.SetColorEntry(16, &ce);
  }
  {
    GDALColorEntry ce = {0xff, 0x99, 0x33, 255};
    ct.SetColorEntry(17, &ce);
  }
  raster_band().SetColorTable(&ct);
}

//-----------------------------------------------------------------------
/// Display Point with the given color index.
//-----------------------------------------------------------------------

void ImagePointDisplay::image_point(const ImageCoordinate& Ic, int Cindex)
{
  const int cross_size = 5;	// Should be odd.
  int l = (int) rint(Ic.line);
  int s = (int) rint(Ic.sample);
  for(int i = l - cross_size / 2; i <= l + cross_size / 2; ++i)
    if(i >= 0 && i < number_line() && s > 0 && s < number_sample())
      write(i, s, Cindex);
  for(int j = s - cross_size / 2; j <= s + cross_size / 2; ++j)
    if(l >= 0 && l < number_line() && j > 0 && j < number_sample())
      write(l, j, Cindex);
}
