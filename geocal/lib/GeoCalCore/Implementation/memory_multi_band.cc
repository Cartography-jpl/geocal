#include "memory_multi_band.h"
#include "memory_raster_image.h"

using namespace GeoCal;
using namespace blitz;

//-----------------------------------------------------------------------
/// Read all of the data from the given RasterImageBand into memory,
/// and provide access to it.
///
/// Note that we require that all the bands have the same number of
/// lines and samples.
//-----------------------------------------------------------------------

MemoryMultiBand::MemoryMultiBand(const RasterImageMultiBand& R)
{
  if(R.number_band() == 0)	// Handle degenerate case.
    return;
  int nl = R.raster_image(0).number_line();
  int ns = R.raster_image(0).number_sample();
  for(int i = 1; i < R.number_band(); ++i)
    if(R.raster_image(i).number_line() != nl ||
       R.raster_image(i).number_sample() != ns)
      throw Exception("MemoryMultiBand requires all raster images passed to it to have the same number of lines and samples");
  data_.resize(R.number_band(), nl, ns);
  for(int i = 0; i < R.number_band(); ++i) {
    R.raster_image(i).read_ptr(0, 0, nl, ns, &data_(i, 0, 0));
    raster_image_.push_back(boost::shared_ptr<RasterImage>
			    (new MemoryRasterImageRef<int>(&data_(i, 0, 0), nl,
							   ns)));
  }
}

