#include "map_reprojected_image.h"
#include "raster_averaged.h"

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Constructor. We resample Img_in to the projection and resolution
/// given by MapInfo.
//-----------------------------------------------------------------------

MapReprojectedImage::MapReprojectedImage(const 
		    boost::shared_ptr<RasterImage> Img_in,
		    const MapInfo& Mi)
: RasterImageVariable(Mi)
{
  boost::shared_ptr<GroundCoordinate> gc1 = 
    Img_in->ground_coordinate(ImageCoordinate(Img_in->number_line() / 2, 
					      Img_in->number_sample() / 2));
  boost::shared_ptr<GroundCoordinate> gc2 = 
    Img_in->ground_coordinate(ImageCoordinate(Img_in->number_line() / 2 + 1, 
					      Img_in->number_sample() / 2 + 1));
  ImageCoordinate ic1 = coordinate(*gc1);
  ImageCoordinate ic2 = coordinate(*gc2);
  int line_avg = (int) round(1.0 / fabs(ic1.line - ic2.line));
  int samp_avg = (int) round(1.0 / fabs(ic1.sample - ic2.sample));
  if(line_avg < 1)
    line_avg = 1;
  if(samp_avg < 1)
    samp_avg = 1;
  if(line_avg > 1 || samp_avg > 1)
    img.reset(new RasterAveraged(Img_in, line_avg, samp_avg));
  else
    img = Img_in;
}

//-----------------------------------------------------------------------
/// Read pixel value at given line and sample.
//-----------------------------------------------------------------------

int MapReprojectedImage::unchecked_read(int Line, int Sample) const
{
  boost::shared_ptr<GroundCoordinate> gc = 
    ground_coordinate(ImageCoordinate(Line, Sample));
  ImageCoordinate ic = img->coordinate(*gc);
  if(ic.line < 0 || ic.line >= img->number_line() - 1 ||
     ic.sample < 0 || ic.sample >= img->number_sample() - 1)
    return 0;			// Data outside of image, so return 0.
  return (int) round(img->interpolate(ic));
}

//-----------------------------------------------------------------------
/// Return a subset of the image.
//-----------------------------------------------------------------------

void MapReprojectedImage::read_ptr(int Lstart, int Sstart, int Number_line, 
				   int Number_sample, int* Res) const
{
  range_min_check(Lstart, 0);
  range_min_check(Sstart, 0);
  range_max_check(Lstart + Number_line - 1, number_line());
  range_max_check(Sstart + Number_sample - 1, number_sample());
  for(int i = Lstart; i < Lstart + Number_line; ++i)
    for(int j = Sstart; j < Sstart + Number_sample; ++j, ++Res)
      *Res = (*this)(i, j);
}

//-----------------------------------------------------------------------
/// Write a value. This isn't actually implemented for this type of 
/// RasterImage.
//-----------------------------------------------------------------------

void MapReprojectedImage::unchecked_write(int Line, int Sample, int Val)
{
  throw Exception("Write not implemented for MapReprojectedImage");
}

//-----------------------------------------------------------------------
/// Print out a description of the object.
//-----------------------------------------------------------------------

void MapReprojectedImage::print(std::ostream& Os) const
{
  Os << "MapReprojectedImage:\n"
     << "  Map info: " << map_info() << "\n"
     << "  Underlying data: " << *img << "\n";
}
