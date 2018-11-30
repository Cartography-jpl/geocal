#include "smooth_image.h"
using namespace GeoCal;
using namespace blitz;

//-----------------------------------------------------------------------
/// Constructor. We give the size of the area to average over by
/// supplying the number half size (Nhs), and then average over a
/// region (2 * Nhs + 1) x (2 * Nhs + 1)
//-----------------------------------------------------------------------

SmoothImage::SmoothImage(const boost::shared_ptr<RasterImage>& Data,
			 int Nhs, int Number_tile)
: CalcRaster(Data->number_line() - 2 * Nhs,
	     Data->number_sample() - 2 * Nhs,
	     // We don't let the tile size be less than 100 because we
	     // lose the border when we read. This means if the tile
	     // size was 1 line for example, we would read 2 * Nhs + 1
	     // lines to process a single line, or that we go through
	     // the data roughly 2 * Nhs times
	     std::max(Data->number_tile_line(), 100),
	     std::max(Data->number_tile_sample(), 100), 
	     Number_tile),
  raw_data(Data), nhs(Nhs)
{
  range_min_check(nhs, 0);
  if(nhs > 3)
    throw Exception("The current implementation of smooth_image only handles 0 <= nhs <= 3. We can extend this if needed, but for now this is all that is supported");
  if(Data->has_map_info())
    map_info_.reset(new MapInfo(Data->map_info().subset(nhs, nhs, 
  		    number_sample(), number_line())));
  if(Data->has_rpc()) {
    rpc_.reset(new Rpc(Data->rpc()));
    rpc_->line_offset -= Nhs;
    rpc_->sample_offset -= Nhs;
  }
  if(Data->has_rsm())
    throw Exception("Don't support RSM yet");
}

BZ_DECLARE_STENCIL2(smooth3,A,B)
A = (B(-3,-3) + B(-3,-2) + B(-3,-1) + B(-3,0) + B(-3,1) + B(-3,2) + B(-3,3) +
     B(-2,-3) + B(-2,-2) + B(-2,-1) + B(-2,0) + B(-2,1) + B(-2,2) + B(-2,3) +
     B(-1,-3) + B(-1,-2) + B(-1,-1) + B(-1,0) + B(-1,1) + B(-1,2) + B(-1,3) +
     B(0,-3) + B(0,-2) + B(0,-1) + B(0,0) + B(0,1) + B(0,2) + B(0,3) +
     B(1,-3) + B(1,-2) + B(1,-1) + B(1,0) + B(1,1) + B(1,2) + B(1,3) +
     B(2,-3) + B(2,-2) + B(2,-1) + B(2,0) + B(2,1) + B(2,2) + B(2,3) +
     B(3,-3) + B(3,-2) + B(3,-1) + B(3,0) + B(3,1) + B(3,2) + B(3,3)) / 49.0;
BZ_END_STENCIL

BZ_DECLARE_STENCIL2(smooth2,A,B)
A = (B(-2, -2) + B(-2, -1) + B(-2, 0) + B(-2, 1) + B(-2, 2) + 
     B(-1, -2) + B(-1, -1) + B(-1, 0) + B(-1, 1) + B(-1, 2) + 
     B(0, -2) + B(0, -1) + B(0, 0) + B(0, 1) + B(0, 2) + 
     B(1, -2) + B(1, -1) + B(1, 0) + B(1, 1) + B(1, 2) + 
     B(2, -2) + B(2, -1) + B(2, 0) + B(2, 1) + B(2, 2)) / 25.0;
BZ_END_STENCIL

BZ_DECLARE_STENCIL2(smooth1,A,B)
A = (B(-1, -1) + B(-1, 0) + B(-1, 1) +
     B(0, -1) + B(0, 0) + B(0, 1) +
     B(1, -1) + B(1, 0) + B(1, 1)) / 9.0;
BZ_END_STENCIL


// See base class for description
void
SmoothImage::calc(int Lstart, int Sstart) const
{
  int nline = data.rows();
  int nsamp = data.cols();
  Array<double, 2> rawd = raw_data->read_double(Lstart + nhs - nhs,
						Sstart + nhs - nhs,
						nline + 2 * nhs,
						nsamp + 2 * nhs);
  Array<double, 2> res(rawd.shape());
  switch(nhs) {
  case 0:
    res = rawd;			// No smoothing.
  case 1:
    applyStencil(smooth1(), res, rawd);
    break;
  case 2:
    applyStencil(smooth2(), res, rawd);
    break;
  case 3:
    applyStencil(smooth3(), res, rawd);
    break;
  default:
    throw Exception("nhs is an unallowed value");
  }
  data = res(Range(nhs, nhs + nline - 1), Range(nhs, nhs + nsamp - 1));
}
