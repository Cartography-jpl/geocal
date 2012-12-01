#include "vicar_raster_image.h"
#include "vicar_dem.h"
#include "vicar_map_projected_image.h"
#include "rpc_image.h"
#include "gdal_map_projected_image.h"
using namespace GeoCal;

int main(int Argc, char** Argv)
{
  boost::shared_ptr<VicarRasterImage> img(new VicarRasterImage(Argv[1]));
  boost::shared_ptr<Dem> dem(new VicarDem(Argv[2]));
  VicarMapProjectedImage mp(Argv[3]);
  RpcImage rpc_img(img, img->vicar_file().rpc(), dem, mp.map_info());
  GdalMapProjectedImage out(Argv[4], "hfa", rpc_img.map_info(), 1, GdalMapProjectedImage::Int16);
  copy(rpc_img, out);
}
