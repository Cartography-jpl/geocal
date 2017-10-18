#include "geometric_model_image.h"
#include "ostream_pad.h"
#include "geocal_serialize_support.h"
using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void GeometricModelImage::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CalcRaster)
    & GEOCAL_NVP_(fill_value) & GEOCAL_NVP_(itype) & GEOCAL_NVP_(raw_data)
    & GEOCAL_NVP(model);
}


GEOCAL_IMPLEMENT(GeometricModelImage);
#endif

// Print to stream.
void GeometricModelImage::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "GeometricModelImage\n"
     << "  number line: " << number_line() << "\n"
     << "  number sample: " << number_sample() << "\n"
     << "  geometric model:\n";
  opad << *model << "\n";
  opad.strict_sync();
  Os << "  underlying raster: \n";
  opad << *raw_data_ << "\n";
  opad.strict_sync();
}

blitz::Array<double, 2> 
GeometricModelImage::interpolate
(double Line, double Sample, int Number_line, int Number_sample,
 double Fill_value) const
{
  blitz::Array<double, 2> res(Number_line, Number_sample);
  for(int i = 0; i < res.rows(); ++i)
    for(int j = 0; j < res.cols(); ++j) {
      // This is 1 based. We need to sort this out, but temporarily
      // just convert.
      ImageCoordinate ic =
	model->original_image_coordinate(ImageCoordinate(i + Line + 1, 
							 j + Sample + 1));
      ic.line -= 1;
      ic.sample -= 1;
      if(itype_ == BILINEAR) {
	if(ic.line < 0 || ic.line >= raw_data_->number_line() - 1 ||
	   ic.sample < 0 || ic.sample >= raw_data_->number_sample() - 1)
	  res(i,j) = Fill_value;
	else
	  res(i, j) = raw_data_->unchecked_interpolate(ic.line, ic.sample);
      } else if(itype_ == NEAREST_NEIGHBOR) {
	int ln = (int) floor(ic.line + 0.5);
	int smp = (int) floor(ic.sample + 0.5);
	if(ln < 0 || ln >= raw_data_->number_line() ||
	   smp < 0 || smp >= raw_data_->number_sample())
	  res(i,j) = Fill_value;
	else
	  res(i, j) = raw_data_->unchecked_read_double(ln, smp);
      } else
	throw Exception("Unknown itype_");
    }
  return res;
}

// See base class for description
void GeometricModelImage::calc(int Lstart, int Sstart) const
{
  for(int i = 0; i < data.rows(); ++i)
    for(int j = 0; j < data.cols(); ++j) {
      ImageCoordinate ic =
	model->original_image_coordinate(ImageCoordinate(i + Lstart, 
							 j + Sstart));
      if(itype_ == BILINEAR) {
	if(ic.line < 0 || ic.line >= raw_data_->number_line() - 1 ||
	   ic.sample < 0 || ic.sample >= raw_data_->number_sample() - 1)
	  data(i,j) = fill_value_;
	else
	  data(i, j) = raw_data_->unchecked_interpolate(ic.line, ic.sample);
      } else if(itype_ == NEAREST_NEIGHBOR) {
	int ln = (int) floor(ic.line + 0.5);
	int smp = (int) floor(ic.sample + 0.5);
	if(ln < 0 || ln >= raw_data_->number_line() ||
	   smp < 0 || smp >= raw_data_->number_sample())
	  data(i,j) = fill_value_;
	else
	  data(i, j) = raw_data_->unchecked_read_double(ln, smp);
      } else
	throw Exception("Unknown itype_");
    }
}
