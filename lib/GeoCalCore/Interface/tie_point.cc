#include "tie_point.h"
#include "geocal_quaternion.h"
#include "geocal_serialize_support.h"
#include "ibis_file.h"
#include <boost/make_shared.hpp>
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

template<class Archive>
void TiePointCollection::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(TiePointCollection);
  GEOCAL_BASE(TiePointCollection, TiePointVector);
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(TiePointVector);
}

GEOCAL_IMPLEMENT(TiePoint);
GEOCAL_IMPLEMENT(TiePointCollection);
#endif

//-----------------------------------------------------------------------
/// Copy constructor. We make a deep copy of everything.
//-----------------------------------------------------------------------

TiePoint::TiePoint(const TiePoint& Tp)
  : id_(Tp.id_),
    is_gcp_(Tp.is_gcp_),
    ground_location_(Tp.ground_location_->convert_to_cf()),
    ic_(Tp.ic_.size()),
    line_sigma_(Tp.line_sigma_.copy()),
    sample_sigma_(Tp.sample_sigma_.copy())
{
  for(int i = 0; i < (int) ic_.size(); ++i)
    if(Tp.ic_[i])
      ic_[i] = boost::make_shared<ImageCoordinate>(*Tp.ic_[i]);
}

//-----------------------------------------------------------------------
/// Assignment. We make a deep copy of everything.
//-----------------------------------------------------------------------

TiePoint& TiePoint::operator=(const TiePoint& Tp)
{
  id_ = Tp.id_;
  is_gcp_ = Tp.is_gcp_;
  ground_location_ = Tp.ground_location_->convert_to_cf();
  ic_.resize(Tp.ic_.size());
  line_sigma_.reference(Tp.line_sigma_.copy());
  sample_sigma_.reference(Tp.sample_sigma_.copy());
  for(int i = 0; i < (int) ic_.size(); ++i)
    if(Tp.ic_[i])
      ic_[i] = boost::make_shared<ImageCoordinate>(*Tp.ic_[i]);
  return *this;
}

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


//-----------------------------------------------------------------------
/// Number of gcps.
//-----------------------------------------------------------------------
int TiePointCollection::number_gcp() const
{
  int res = 0;
  BOOST_FOREACH(const boost::shared_ptr<TiePoint>& tp, *this)
    if(tp->is_gcp())
      ++res;
  return res;
}

//-----------------------------------------------------------------------
/// Print to a stream.
//-----------------------------------------------------------------------

void TiePointCollection::print(std::ostream& Os) const
{
  Os << "TiePointCollection\n"
     << "  Total number tp: " << size() << "\n"
     << "  Number GCPs:     " << number_gcp() << "\n";
}

//-----------------------------------------------------------------------
/// Add tiepoints from a IBIS file like one passed to geomv. This is a
/// bit of a specific function, but it can be useful to treat the grid
/// geomv uses (usually created with tieconv) as a set of tiepoints.
///
/// We assume that the 4 columns in (newline,newsamp,oldline,oldsamp)
/// order. We take in the New_image to translate newline,newsamp to
/// a ground location, and oldline,oldsamp are the image
/// coordinates. So this gives a tiepoint in the old image that can we
/// used to map it to the new image.
//-----------------------------------------------------------------------

void TiePointCollection::add_ibis_file
(const std::string& Ibis_fname,
 const boost::shared_ptr<RasterImage>& New_image,
 const boost::shared_ptr<Dem>& D)
{
  IbisFile f(Ibis_fname);
  for(int i = 0; i < f.number_row(); ++i) {
    boost::shared_ptr<GroundCoordinate> gc;
    if(D)
      gc = New_image->ground_coordinate(VicarImageCoordinate(f.data<double>(i,0), f.data<double>(i,1)), *D);
    else
      gc = New_image->ground_coordinate(VicarImageCoordinate(f.data<double>(i,0), f.data<double>(i,1)));
    boost::shared_ptr<TiePoint> tp = boost::make_shared<TiePoint>(1);
    tp->id(i+1);
    tp->is_gcp(true);
    tp->ground_location(gc->convert_to_cf());
    VicarImageCoordinate ic(f.data<double>(i,2), f.data<double>(i,3));
    tp->image_coordinate(0, boost::make_shared<ImageCoordinate>(ic));
    push_back(tp);
  }
}

//-----------------------------------------------------------------------
/// Write out data as a blitz::Array that can be ingested by pandas
/// dataframe. Can do this all in python, but the C++ is much faster.
///
/// This returns "ID", "Is_GCP", "Longitude (deg)", "Latitude (deg)",
/// "Height (m)". If Ref_image is not null, then returns
/// "Reference Line", "Reference Sample". Then we return "Line Image %d"
/// "Sample Image %d" "Line Sigma Image %d" "Sample Sigma Image %d"
/// for each image in the tiepoints.
//-----------------------------------------------------------------------

blitz::Array<double, 2> TiePointCollection::data_array
(const boost::shared_ptr<RasterImage>& Ref_image) const
{
  double nanv = nan("");
  int numimg = (*this)[0]->number_image();
  int numcol = 5 +  numimg * 4;
  if(Ref_image)
    numcol += 2;
  blitz::Array<double, 2> res(size(), numcol);
  for(int i = 0; i < res.rows(); ++i) {
    boost::shared_ptr<TiePoint> tp = (*this)[i];
    res(i,0) = tp->id();
    res(i, 1) = (tp->is_gcp() ? 1 : 0);
    res(i, 2) = tp->ground_location()->longitude();
    res(i, 3) = tp->ground_location()->latitude();
    res(i, 4) = tp->ground_location()->height_reference_surface();
    int j = 5;
    if(Ref_image) {
      ImageCoordinate ic = Ref_image->coordinate(*tp->ground_location());
      res(i,5) = ic.line;
      res(i,6) = ic.sample;
      j += 2;
    }
    for(int ii = 0; ii < numimg; ++ii) {
      boost::shared_ptr<ImageCoordinate> ic = tp->image_coordinate(ii);
      if(ic) {
	res(i, j) = ic->line;
	res(i, j+1) = ic->sample;
	res(i, j+2) = tp->line_sigma(ii);
	res(i, j+3) = tp->sample_sigma(ii);
      } else {
	res(i, j) = nanv;
	res(i, j+1) = nanv;
	res(i, j+2) = nanv;
	res(i, j+3) = nanv;
      }
      j += 4;
    }
  }
  return res;
}
