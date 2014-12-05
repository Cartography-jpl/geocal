#include "image_ground_connection.h"
#include "simple_dem.h"
#include "ostream_pad.h"
using namespace GeoCal;
using namespace blitz;

//-----------------------------------------------------------------------
/// Initializer. As a convenience, if Img_mask or Ground_mask are null
/// pointer, we replace them with an empty CombinedImageMask or
/// CombinedGroundMask.
//-----------------------------------------------------------------------

void ImageGroundConnection::initialize
(const boost::shared_ptr<Dem>& d, 
 const boost::shared_ptr<RasterImage>& Img, 
 const boost::shared_ptr<RasterImageMultiBand>& Img_mb, 
 const std::string& Title,
 const boost::shared_ptr<ImageMask>& Img_mask,
 const boost::shared_ptr<GroundMask>& Ground_mask)
{
  dem_ = d;
  image_ = Img;
  image_mb_ = Img_mb;
  title_ = Title;
  image_mask_ = Img_mask;
  ground_mask_ = Ground_mask;
  if(!image_mask_)
    image_mask_.reset(new CombinedImageMask);
  if(!ground_mask_)
    ground_mask_.reset(new CombinedGroundMask);
}

//-----------------------------------------------------------------------
/// Return an array of look vector information. This is really
/// intended for use with python. This is nline x nsamp x 2 x 3 in
/// size, where we give the position first followed by the look vector.
//-----------------------------------------------------------------------

blitz::Array<double, 7> ImageGroundConnection::cf_look_vector_arr
(int ln_start, int smp_start, 
 int nline, int nsamp, int nsubpixel_line, int nsubpixel_sample, 
 int nintegration_step) const
{
  CartesianFixedLookVector lv;
  boost::shared_ptr<CartesianFixed> pos;
  blitz::Array<double, 7>  res(nline, nsamp, nsubpixel_line, 
			       nsubpixel_sample, nintegration_step, 2, 3);
  for(int i = 0; i < nline; ++i)
    for(int j = 0; j < nsamp; ++j) 
      for(int i2 = 0; i2 < nsubpixel_line; ++i2)
	for(int j2 = 0; j2 < nsubpixel_sample; ++j2) {
	  ImageCoordinate ic(i + ln_start + ((double) i2 / nsubpixel_line),
			     j + smp_start + ((double) j2 / nsubpixel_sample));
	  cf_look_vector(ic, lv, pos);
	  for(int k = 0; k < nintegration_step; ++k)
	    for(int k2 = 0; k2 < 3; ++k2) {
	      res(i, j, i2, j2, k, 0, k2) = pos->position[k2];
	      res(i, j, i2, j2, k, 1, k2) = lv.look_vector[k2];
	    }
	}
  return res;
}

//-----------------------------------------------------------------------
/// Find a MapInfo that covers the ground coordinate of this 
/// ImageGroundConnection. We calculate the ground coordinate of the
/// four corners, then find the MapInfo that covers those corners,
/// optionally adding a boundary in map pixels.  Depending on the
/// actual warping involved, this may or may not fully cover the edges
/// of the image.
//-----------------------------------------------------------------------

MapInfo ImageGroundConnection::cover(const MapInfo& Mi,
				     int Boundary) const {
  std::vector<boost::shared_ptr<GroundCoordinate> > pt;
  pt.push_back(ground_coordinate(ImageCoordinate(0, 0)));
  pt.push_back(ground_coordinate(ImageCoordinate(0, number_sample() - 1)));
  pt.push_back(ground_coordinate(ImageCoordinate(number_line() - 1, 
						 number_sample() - 1)));
  pt.push_back(ground_coordinate(ImageCoordinate(number_line() - 1, 0)));
  return Mi.cover(pt, Boundary);
}

//-----------------------------------------------------------------------
/// Return the Jacobian of the image coordinates with respect to the
/// X, Y, and Z components of the CartesianFixed ground location. 
//-----------------------------------------------------------------------

blitz::Array<double, 2> ImageGroundConnection::image_coordinate_jac_cf
(const CartesianFixed& Gc) const
{
  // Default is just to do a numerical derivative.
  double eps = 0.1;
  blitz::Array<double, 2> res(2, 3);
  ImageCoordinate ic0 = image_coordinate(Gc);
  boost::shared_ptr<CartesianFixed> gcx = Gc.convert_to_cf();
  for(int i = 0; i < 3; ++i) {
    gcx->position[i] += eps;
    ImageCoordinate ic = image_coordinate(*gcx);
    res(0, i) = (ic.line - ic0.line) / eps;
    res(1, i) = (ic.sample - ic0.sample) / eps;
    gcx->position[i] = Gc.position[i];
  }
  return res;
}

//-----------------------------------------------------------------------
/// Return ground coordinate that is nearly the given height above
/// the reference surface. This is exact in the sense that the
/// returned point matches the image coordinate (to 
/// igc.image_coordinate(res) = Ic up to roundoff errors), but it is
/// approximate in the sense that the height might not be exactly
/// the supplied height. This is similar to
/// CartesianFixed::reference_surface_intersect_approximate. A particular
/// implementation can be much faster than ground_coordinate_dem,
/// since it doesn't need to do ray tracing.
//-----------------------------------------------------------------------

boost::shared_ptr<GroundCoordinate> 
ImageGroundConnection::ground_coordinate_approx_height
(const ImageCoordinate& Ic, 
 double H) const
{
  // Default implementation just uses ground_coordinate_dem
  return ground_coordinate_dem(Ic, SimpleDem(H));
}

//-----------------------------------------------------------------------
/// Calculate the approximate resolution on the ground of a given
/// ImageCoordinate. This finds the intersection with the
/// reference surface for the given pixel, + 1 in the
/// line and sample direction. We find the difference in meters
/// between these points, and select the maximum value.
//-----------------------------------------------------------------------

double ImageGroundConnection::resolution_meter
(const ImageCoordinate& Ic) const
{
  boost::shared_ptr<GroundCoordinate> gc1 = ground_coordinate(Ic);
  boost::shared_ptr<GroundCoordinate> gc2 = 
    ground_coordinate(ImageCoordinate(Ic.line + 1, Ic.sample));
  boost::shared_ptr<GroundCoordinate> gc3 = 
    ground_coordinate(ImageCoordinate(Ic.line, Ic.sample + 1));
  double d1 = distance(*gc1, *gc2);
  double d2 = distance(*gc1, *gc3);
  return std::max(d1, d2);
}

//-----------------------------------------------------------------------
/// Footprint resolution in the line and sample direction. Note that
/// in general the footprint on the ground of a particular line is
/// *not* the same as the spacing between pixels in the
/// image. resolution_meter returns the spacing in the acquired image,
/// while this footprint is the size of a particular pixel on the
/// ground, including any overlap or underlap with surrounding
/// pixels.
///
/// Default implementation just returns the spacing between image
/// pixels, but derived classes should give the correct implementation
/// for what they are modeling.
//-----------------------------------------------------------------------

void ImageGroundConnection::footprint_resolution
(int Line, int Sample, 
 double &Line_resolution_meter, 
 double &Sample_resolution_meter) const
{
  ImageCoordinate ic(Line, Sample);
  boost::shared_ptr<GroundCoordinate> gc1 = ground_coordinate(ic);
  boost::shared_ptr<GroundCoordinate> gc2 = 
    ground_coordinate(ImageCoordinate(ic.line + 1, ic.sample));
  boost::shared_ptr<GroundCoordinate> gc3 = 
    ground_coordinate(ImageCoordinate(ic.line, ic.sample + 1));
  Line_resolution_meter = distance(*gc1, *gc2);
  Sample_resolution_meter = distance(*gc1, *gc3);
}

//-----------------------------------------------------------------------
/// Variation of resolution_meter that find the resolution of the
/// center pixel.
//-----------------------------------------------------------------------

double ImageGroundConnection::resolution_meter() const
{
  return resolution_meter(ImageCoordinate(number_line() / 2.0, 
					  number_sample() / 2.0));
}

// See base class for description
blitz::Array<double, 2> 
OffsetImageGroundConnection::image_coordinate_jac_parm
(const GroundCoordinate& Gc) const
{ 
  Array<double, 2> rest = ig_->image_coordinate_jac_parm(Gc);
  Array<double, 2> res(2, 2 + rest.cols());
  res(0,0) = 1;
  res(1,1) = 1;
  res(1,0) = 0;
  res(0,1) = 0;
  if(rest.cols() > 0)
    res(Range::all(), Range(2, toEnd)) = rest;
  return res;
}

// See base class for description
blitz::Array<double, 1> OffsetImageGroundConnection::parameter() const 
{ 
  Array<double, 1> rest = ig_->parameter(); 
  Array<double, 1> res(2 + rest.rows());
  res(0) = line_offset_.value();
  res(1) = sample_offset_.value();
  if(res.rows() > 2)
    res(Range(2, toEnd)) = rest;
  return res;
}

// See base class for description
ArrayAd<double, 1> 
OffsetImageGroundConnection::parameter_with_derivative() const 
{ 
  ArrayAd<double, 1> rest = ig_->parameter_with_derivative(); 
  ArrayAd<double, 1> res(2 + rest.rows(), res.number_variable());
  res(0) = line_offset_;
  res(1) = sample_offset_;
  if(res.rows() > 2)
    res(Range(2, toEnd)) = rest;
  return res;
}

// See base class for description
void OffsetImageGroundConnection::parameter
(const blitz::Array<double, 1>& Parm)
{ 
  if(Parm.rows() != parameter().rows()) {
    Exception e;
    e << "Expected parameter to have " << parameter().rows() 
      << " rows, but got " << Parm.rows();
    throw e;
  }
  line_offset_ = Parm(0);
  sample_offset_ = Parm(1);
  if(Parm.rows() > 2)
    ig_->parameter(Parm(Range(2,toEnd)));
}

void OffsetImageGroundConnection::parameter_with_derivative
(const ArrayAd<double, 1>& Parm)
{ 
  if(Parm.rows() != parameter().rows()) {
    Exception e;
    e << "Expected parameter to have " << parameter().rows() 
      << " rows, but got " << Parm.rows();
    throw e;
  }
  line_offset_ = Parm(0);
  sample_offset_ = Parm(1);
  if(Parm.rows() > 2)
    ig_->parameter_with_derivative(Parm(Range(2,toEnd)));
}

blitz::Array<bool, 1> OffsetImageGroundConnection::parameter_mask() const
{
  Array<bool, 1> rest = ig_->parameter_mask(); 
  Array<bool, 1> res(2 + rest.rows());
  res(0) = true;
  res(1) = true;
  if(res.rows() > 2)
    res(Range(2, toEnd)) = rest;
  return res;
}

// See base class for description
std::vector<std::string> OffsetImageGroundConnection::parameter_name() const
{ 
  std::vector<std::string> res;
  res.push_back("Line offset");
  res.push_back("Sample offset");
  std::vector<std::string> rest(ig_->parameter_name());
  res.insert(res.end(), rest.begin(), rest.end());
  return res;
}


void ImageGroundConnectionCopy::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "ImageGroundConnectionCopy:\n"
     << "  Underlying Igc:\n";
  opad << *igc_original() << "\n";
  opad.strict_sync();
  Os << "  Dem:\n";
  opad << dem() << "\n";
  opad.strict_sync();
  if(image()) {
    Os << "  Image:\n";
    opad << *image() << "\n";
    opad.strict_sync();
  }
  if(image_multi_band()) {
    Os << "  Image multi-band:\n";
    opad << *image_multi_band() << "\n";
    opad.strict_sync();
  }
  Os << "  Title: " << title() << "\n"
     << "  Image mask:\n";
  opad << *image_mask() << "\n";
  opad.strict_sync();
  Os << "  Ground mask:\n";
  opad << *ground_mask() << "\n";
  opad.strict_sync();
}
