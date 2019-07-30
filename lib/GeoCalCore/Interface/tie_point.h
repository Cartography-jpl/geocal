#ifndef TIE_POINT_H
#define TIE_POINT_H
#include "image_coordinate.h"
#include "igc_collection.h"
#include "ground_coordinate.h"
#include "geocal_exception.h"
#include <blitz/array.h>

namespace GeoCal {
/****************************************************************//**
    This is a tiepoint. This is little more than a structure.

    Note that we previously had TiePoint a python only object, and
    stored it as a python pickled object. The primary motivation for
    moving this to C++ is to be able to use the boost XML
    serialization, which is more stable and can support old
    versions. If you have an older tiepoint data using the old pickle
    format, you can either rerun the process that generated the tie
    points, or you can run the old version of the code, write the
    tiepoints out as the old MSPI format (write_old_mspi_format
    defined in python), and the read the old MSPI format using the new
    version of the code with the tiepoints in C++. This is just a
    temporary fix, going forward the intention is that the tiepoints
    will use the boost format and will automatically support older
    versions. 
*******************************************************************/

class TiePoint : public Printable<TiePoint> {
public:
//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

  TiePoint(int Number_image)
    : id_(1),
      is_gcp_(false),
      ic_(Number_image),
      line_sigma_(Number_image),
      sample_sigma_(Number_image)
  {
    line_sigma_ = 0.1;
    sample_sigma_ = 0.1;
  }

//-----------------------------------------------------------------------
/// Default constructor
//-----------------------------------------------------------------------

  TiePoint() {}
  TiePoint(const TiePoint& Tp);
  TiePoint& operator=(const TiePoint& Tp);
  
//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~TiePoint() {}

//-----------------------------------------------------------------------
/// ID of tiepoint
//-----------------------------------------------------------------------

  int id() const { return id_;}
  void id(int V) { id_ = V; }

//-----------------------------------------------------------------------
/// True if TiePoint is a ground control point.
//-----------------------------------------------------------------------

  bool is_gcp() const { return is_gcp_;}
  void is_gcp(bool V) { is_gcp_ = V; }
  
//-----------------------------------------------------------------------
// Ground location.
//-----------------------------------------------------------------------

  boost::shared_ptr<GroundCoordinate> ground_location() const
  { return ground_location_; }
  void ground_location(const boost::shared_ptr<GroundCoordinate>& Gc)
  { ground_location_ = Gc; }

//-----------------------------------------------------------------------
/// Number of images covered by the tie point.
//-----------------------------------------------------------------------

  int number_image() const { return (int) ic_.size(); }

//-----------------------------------------------------------------------
// Number of image coordinate that we have values for.
//-----------------------------------------------------------------------

  int number_image_location() const
  {
    int res = 0;
    BOOST_FOREACH(const boost::shared_ptr<ImageCoordinate>& i, ic_)
      if(i)
	++res;
    return res;
  }

//-----------------------------------------------------------------------  
/// Image coordinate
//-----------------------------------------------------------------------

  boost::shared_ptr<ImageCoordinate> image_coordinate(int Image_index) const
  { range_check(Image_index, 0, number_image());
    return ic_[Image_index];
  }
  void image_coordinate(int Image_index,
			const boost::shared_ptr<ImageCoordinate>& Ic,
			double Line_sigma = 0.1, double Sample_sigma = 0.1)
  {
    range_check(Image_index, 0, number_image());
    ic_[Image_index] = Ic;
    line_sigma_(Image_index) = Line_sigma;
    sample_sigma_(Image_index) = Sample_sigma;
  }

//-----------------------------------------------------------------------
/// Line sigma.
//-----------------------------------------------------------------------  

  double line_sigma(int Image_index) const
  {
    range_check(Image_index, 0, number_image());
    return line_sigma_(Image_index);
  }

//-----------------------------------------------------------------------
/// Sample sigma.
//-----------------------------------------------------------------------  

  double sample_sigma(int Image_index) const
  {
    range_check(Image_index, 0, number_image());
    return sample_sigma_(Image_index);
  }

  blitz::Array<double, 2> ic() const;
  blitz::Array<double, 2> ic_sigma() const;
  blitz::Array<double, 2> ic_pred(const IgcCollection& Igccol) const;
  blitz::Array<double, 2> ic_diff(const IgcCollection& Igccol) const;
  
  virtual void print(std::ostream& Os) const;
private:
  int id_;
  bool is_gcp_;
  boost::shared_ptr<GroundCoordinate> ground_location_;
  std::vector<boost::shared_ptr<ImageCoordinate> > ic_;
  blitz::Array<double, 1> line_sigma_;
  blitz::Array<double, 1> sample_sigma_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

typedef std::vector<boost::shared_ptr<TiePoint> > TiePointVector;

/****************************************************************//**
  This is a list of TiePoints with a few useful functions added.
*******************************************************************/

class TiePointCollection: public TiePointVector,
			  public Printable<TiePointCollection> {
public:
//-----------------------------------------------------------------------
/// Default constructor.
//-----------------------------------------------------------------------

  TiePointCollection() {}

//-----------------------------------------------------------------------
/// Constructor with an initial list of TiePoints.
//-----------------------------------------------------------------------
  TiePointCollection(const TiePointVector& Tlist)
    : TiePointVector(Tlist)
  {}

//-----------------------------------------------------------------------
/// Destructor
//-----------------------------------------------------------------------
  virtual ~TiePointCollection() {}

  void add_ibis_file(const std::string& Ibis_fname,
		     const boost::shared_ptr<RasterImage>& New_image,
		     const boost::shared_ptr<Dem>& D);
  blitz::Array<double, 2> data_array(const boost::shared_ptr<RasterImage>& Ref_image) const;
  
//-----------------------------------------------------------------------
/// Number of gcps.
//-----------------------------------------------------------------------
  int number_gcp() const;
  
  virtual void print(std::ostream& Os) const;
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(TiePoint);
GEOCAL_EXPORT_KEY(TiePointVector);
GEOCAL_EXPORT_KEY(TiePointCollection);
#endif
