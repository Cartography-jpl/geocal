#include "tie_point.h"
#include "geocal_quaternion.h"
#include "geocal_serialize_support.h"
#include <cmath>

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void TiePoint::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(TiePoint);
  ar & GEOCAL_NVP_(id) & GEOCAL_NVP_(is_gcp) & GEOCAL_NVP_(ground_location)
    & GEOCAL_NVP_(ic) & GEOCAL_NVP_(line_sigma) & GEOCAL_NVP_(sample_sigma);
}

GEOCAL_IMPLEMENT(TiePoint);
#endif

//-----------------------------------------------------------------------
/// Print to a stream.
//-----------------------------------------------------------------------

void TiePoint::print(std::ostream& Os) const
{
  Os << "TiePoint\n"
     << "  Id:              " << id() << "\n"
     << "  Is GCP:          " << (is_gcp() ? "True" : "False") << "\n"
     << "  Ground location: ";
  if(ground_location())
    Os << *ground_location() << "\n";
  else
    Os << "None\n";
  Os  << "  Image coordinate:\n";
  for(int i = 0; i < number_image(); ++i) {
    Os << "     ";
    if(image_coordinate(i))
      Os << *image_coordinate(i) << ", " << line_sigma(i) << ", "
	 << sample_sigma(i) << "\n";
    else
      Os << "None\n";
  }
}

//-----------------------------------------------------------------------
/// This repackages the image_coordinate, in a better way for doing
/// such things as plotting. We return a 2D array with the first row
/// being the image line and the second the image sample. The columns
/// are the image indexes. For image locations that are missing, we
/// return a NaN.
///
/// This is really intended for use by python, I'm not sure this would
/// be of much use in C++.
//-----------------------------------------------------------------------

blitz::Array<double, 2> TiePoint::ic() const
{
  blitz::Array<double, 2> res(2, number_image());
  double nanv = nan("");
  for(int i = 0; i < number_image(); ++i)
    if(ic_[i]) {
      res(0, i) = ic_[i]->line;
      res(1, i) = ic_[i]->sample;
    } else {
      res(0,i) = nanv;
      res(1,i) = nanv;
    }
  return res;
}

//-----------------------------------------------------------------------
/// Like ic, but return line and sample sigma.
//-----------------------------------------------------------------------

blitz::Array<double, 2> TiePoint::ic_sigma() const
{
  blitz::Array<double, 2> res(2, number_image());
  double nanv = nan("");
  for(int i = 0; i < number_image(); ++i)
    if(ic_[i]) {
      res(0, i) = line_sigma_(i);
      res(1, i) = sample_sigma_(i);
    } else {
      res(0,i) = nanv;
      res(1,i) = nanv;
    }
  return res;
}

//-----------------------------------------------------------------------
/// Like ic, but uses the supplied igccol to predict the image 
/// location given our current ground position.
//-----------------------------------------------------------------------

blitz::Array<double, 2> TiePoint::ic_pred
(const IgcCollection& Igccol) const
{
  blitz::Array<double, 2> res(2, number_image());
  double nanv = nan("");
  for(int i = 0; i < number_image(); ++i)
    if(ic_[i]) {
      ImageCoordinate icp = Igccol.image_coordinate(i, *ground_location());
      res(0, i) = icp.line;
      res(1, i) = icp.sample;
    } else {
      res(0,i) = nanv;
      res(1,i) = nanv;
    }
  return res;
}

//-----------------------------------------------------------------------
/// Difference between observed and predicted image coordinates
//-----------------------------------------------------------------------

blitz::Array<double, 2> TiePoint::ic_diff
(const IgcCollection& Igccol) const
{
  blitz::Array<double, 2> res(2, number_image());
  res = ic() - ic_pred(Igccol);
  return res;
}


