#include "raster_image_multi_band.h"
#include "raster_image_multi_band_variable.h"
#include "sub_raster_image.h"
#include "geocal_serialize_support.h"
using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void RasterImageMultiBand::serialize(Archive & UNUSED(ar), const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(RasterImageMultiBand);
}

GEOCAL_IMPLEMENT(RasterImageMultiBand);
#endif

template<class T, int D> blitz::Array<T, D> 
to_c_order_const(const blitz::Array<T, D> &In)
{
  bool is_ok = In.isStorageContiguous();
  for(int i = 0; i < D; ++i)
    if(In.ordering(i) != D - i - 1||
       !In.isRankStoredAscending(i))
      is_ok = false;
  if(is_ok)
    return In;
  else {
    blitz::Array<T, D> res(In.shape());
    res = In;
    return res;
  }
}

//-----------------------------------------------------------------------
/// Read the data from the same location in each image, and put into
/// an array. This will be number_band x number_line x number_sample.
//-----------------------------------------------------------------------

Array<int, 3> 
RasterImageMultiBand::read(int Lstart, int Sstart, int Nline, 
				 int Nsamp) const
{
  Array<int, 3> res(number_band(), Nline, Nsamp);
  int* res_ptr = res.data();
  for(int i = 0; i < number_band(); ++i, res_ptr += Nline * Nsamp)
    raster_image(i).read_ptr(Lstart, Sstart, Nline, Nsamp, res_ptr);
  return res;
}

//-----------------------------------------------------------------------
/// Read the data from the same location in each image, and put into
/// an array. This will be number_band x number_line x number_sample.
//-----------------------------------------------------------------------

Array<double, 3> 
RasterImageMultiBand::read_double(int Lstart, int Sstart, int Nline, 
				  int Nsamp) const
{
  Array<double, 3> res(number_band(), Nline, Nsamp);
  for(int i = 0; i < number_band(); ++i)
    res(i, Range::all(), Range::all()) = 
      raster_image(i).read_double(Lstart, Sstart, Nline, Nsamp);
  return res;
}

//-----------------------------------------------------------------------
/// Write the data to the same location in each image. Data should be
/// number_band() x nline x nsamp.
//-----------------------------------------------------------------------


void RasterImageMultiBand::write(int Lstart, int Sstart, 
				 const blitz::Array<int, 3>& Data)
{
  if(Data.rows() != number_band())
    throw Exception("Data is not number_band() in size");
  Array<int, 3> d(to_c_order_const(Data));
  const int* d_ptr = d.data();
  int nline = d.cols();
  int nsamp = d.depth();
  for(int i = 0; i < number_band(); ++i, d_ptr += nline * nsamp)
    raster_image(i).write_ptr(Lstart, Sstart, nline, nsamp, d_ptr);
}

void RasterImageMultiBand::write(int Lstart, int Sstart, 
				 const blitz::Array<double, 3>& Data)
{
  if(Data.rows() != number_band())
    throw Exception("Data is not number_band() in size");
  Array<double, 3> d(to_c_order_const(Data));
  const double* d_ptr = d.data();
  int nline = d.cols();
  int nsamp = d.depth();
  for(int i = 0; i < number_band(); ++i, d_ptr += nline * nsamp)
    raster_image(i).write_ptr(Lstart, Sstart, nline, nsamp, d_ptr);
}

//-----------------------------------------------------------------------
/// Create a RasterImageMultiBandVariable by finding the overview (if
/// any) for each of the raster bands. The bands will all be the same
/// size, so if the overviews don't match in size we don't use them.
/// If we can't find overviews, then the RasterImageMultiBandVariable
/// will just point to the same data as this object does.
//-----------------------------------------------------------------------

RasterImageMultiBandVariable
RasterImageMultiBand::overview(int Min_number_sample) const
{
  RasterImageMultiBandVariable res;
  bool give_up = false;
  for(int i = 0; i < number_band() && !give_up; ++i) {
    boost::shared_ptr<RasterImage> ri = 
      raster_image(i).overview(Min_number_sample);
    if(!ri.get())
      give_up = true;
    else {
      res.add_raster_image(ri);
      if(ri->number_line() != res.raster_image(0).number_line() ||
	 ri->number_sample() != res.raster_image(0).number_sample())
	give_up = true;
    }
  }
  if(!give_up)
    return res;
  RasterImageMultiBandVariable res2;
  for(int i = 0; i < number_band(); ++i)
    res2.add_raster_image(raster_image_ptr(i));
  return res2;
}

//-----------------------------------------------------------------------
/// \ingroup Miscellaneous
/// This copies one multiband image to another. The images should be the same
/// size. We also assume that all the bands are the same size.
///
/// Setting Diagnostic to true causes messages to be printed as
/// we do the copying.
//-----------------------------------------------------------------------

void GeoCal::copy(const RasterImageMultiBand& Img_in, 
		  RasterImageMultiBand& Img_out, bool Log_progress)
{
  if(Img_in.number_band() != Img_out.number_band())
    throw Exception("Need to have the same number of bands");
  for(int i = 0; i < Img_in.number_band(); ++i)
    if(Img_out.raster_image(i).number_line() != 
       Img_in.raster_image(i).number_line() ||
       Img_out.raster_image(i).number_sample() != 
       Img_in.raster_image(i).number_sample() ||
       Img_out.raster_image(i).number_line() != 
       Img_in.raster_image(0).number_line() ||
       Img_out.raster_image(i).number_sample() != 
       Img_in.raster_image(0).number_sample())
      throw Exception("Images need to be the same size");
  for(RasterImageTileIterator i(Img_in.raster_image(0));
      !i.end(); ++i) {
    if(Img_in.copy_needs_double())
      Img_out.write(i.istart(), i.jstart(), 
		    Img_in.read_double(i.istart(), i.jstart(),
				i.number_line(), i.number_sample()));
    else
      Img_out.write(i.istart(), i.jstart(), 
		    Img_in.read(i.istart(), i.jstart(),
				i.number_line(), i.number_sample()));
  }
}

