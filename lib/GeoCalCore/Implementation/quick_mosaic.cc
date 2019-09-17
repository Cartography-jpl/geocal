#include "quick_mosaic.h"
#include "ostream_pad.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void QuickMosaic::serialize(Archive & ar, 
				    const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CalcRaster)
    & GEOCAL_NVP_(fill_value) & GEOCAL_NVP_(img_list);
}

GEOCAL_IMPLEMENT(QuickMosaic);
#endif

//-----------------------------------------------------------------------
/// Constructor. 
//-----------------------------------------------------------------------

QuickMosaic::QuickMosaic
(const MapInfo& Mi, 
 const std::vector<boost::shared_ptr<RasterImage> >& Img_list,
 int Number_tile_line, int Number_tile_sample, 
 double Fill_value)
  : CalcRaster(Mi),
    fill_value_(Fill_value),
    img_list_(Img_list)
{
  if(Number_tile_line < 0)
    number_tile_line_ = number_line();
  else
    number_tile_line_ = Number_tile_line;
  if(Number_tile_sample < 0)
    number_tile_sample_ = number_sample();
  else
    number_tile_sample_ = Number_tile_sample;
}

// See base class for description
void QuickMosaic::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "QuickMosaic:\n"
     << "  Map info:\n";
  opad << map_info() << "\n";
  opad.strict_sync();
  Os << "  Image list:\n";
  BOOST_FOREACH(const boost::shared_ptr<RasterImage>& img, img_list_) {
    opad << *img << "\n";
    opad.strict_sync();
  }
}

// See base class for description
void QuickMosaic::calc(int Lstart, int Sstart) const
{
  MapInfo calc_mi = map_info().subset(Sstart, Lstart, data.cols(), data.rows());
  data = fill_value_;
  std::vector<blitz::Array<double, 2> > img_data;
  BOOST_FOREACH(const boost::shared_ptr<RasterImage>& img, img_list_) {
    MapInfo mintersect = img->map_info().intersection(calc_mi);
    if(mintersect.number_x_pixel() > 0 && mintersect.number_y_pixel() > 0) {
      ImageCoordinate ic = img->coordinate(*ground_coordinate(Lstart, Sstart));
      int lstart = floor(ic.line + 0.5);
      int sstart = floor(ic.sample + 0.5);
      img_data.push_back(img->read_double_with_pad(lstart, sstart,
		   data.rows(), data.cols(), fill_value_));
    }
  }
  for(int i = 0; i < data.rows(); ++i)
    for(int j = 0; j < data.cols(); ++j)
      for(int k = 0; k < (int) img_data.size() && data(i, j) == fill_value_;
	  ++k)
	data(i,j)  = img_data[k](i,j);
}


