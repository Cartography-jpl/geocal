#include "geometric_model_image.h"
#include "ostream_pad.h"
using namespace GeoCal;
using namespace blitz;

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

// See base class for description
void GeometricModelImage::calc(int Lstart, int Sstart) const
{
  for(int i = 0; i < data.rows(); ++i)
    for(int j = 0; j < data.cols(); ++j)
      data(i, j) = raw_data_->interpolate
	(model->original_image_coordinate(ImageCoordinate(i + Lstart, 
							  j + Sstart)));
}
