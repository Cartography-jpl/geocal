#include "sub_raster_image_multi_band.h"
#include "sub_raster_image.h"
#include "geocal_serialize_support.h"
#include "ostream_pad.h"
using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void SubRasterImageMultiBand::serialize(Archive & ar, 
					const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(RasterImageMultiBandVariable)
    & GEOCAL_NVP(rmb)
    & GEOCAL_NVP(sl)
    & GEOCAL_NVP(ss)
    & GEOCAL_NVP(nl)
    & GEOCAL_NVP(ns);
}

GEOCAL_IMPLEMENT(SubRasterImageMultiBand);
#endif

SubRasterImageMultiBand::SubRasterImageMultiBand
(const boost::shared_ptr<RasterImageMultiBand>& Rmb,
 int Start_line, int Start_sample,
 int Number_line, int Number_sample)
: rmb(Rmb),
    sl(Start_line), ss(Start_sample),
    nl(Number_line), ns(Number_sample) 
{ 
  range_check(Start_line, 0, rmb->raster_image(0).number_line() - 
	      Number_line + 1);
  range_check(Start_sample, 0, rmb->raster_image(0).number_sample() - 
	      Number_sample + 1);
  for(int i = 0; i < rmb->number_band(); ++i)
    add_raster_image(boost::shared_ptr<RasterImage>
		     (new SubRasterImage(rmb->raster_image_ptr(i), sl, ss,
					 nl, ns)));
}

blitz::Array<int, 3> SubRasterImageMultiBand::read
(int Lstart, int Sstart, int Nline, 
 int Nsamp) const
{
  range_check(Lstart, 0, nl - Nline + 1);
  range_check(Sstart, 0, ns - Nsamp + 1);
  return rmb->read(Lstart + sl, Sstart + ss, Nline, Nsamp);
}

blitz::Array<double, 3> SubRasterImageMultiBand::read_double
(int Lstart, int Sstart, 
 int Nline, int Nsamp) const
{
  range_check(Lstart, 0, nl - Nline + 1);
  range_check(Sstart, 0, ns - Nsamp + 1);
  return rmb->read_double(Lstart + sl, Sstart + ss, Nline, Nsamp);
}

void SubRasterImageMultiBand::write
(int Lstart, int Sstart, const blitz::Array<int, 3>& Data)
{
  range_check(Lstart, 0, nl - Data.cols() + 1);
  range_check(Sstart, 0, ns - Data.depth() + 1);
  rmb->write(Lstart + sl, Sstart + ss, Data);
}

void SubRasterImageMultiBand::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "SubRasterImageMultiBand from (" << sl << ", " << ss
     << ") to (" << sl + nl - 1 << ", "
     << ss + ns - 1 << ")\n"
     << "  Underlying RasterImageMultiBand: \n";
  opad << *full_raster_image_multi_band();
  opad.strict_sync();
}
