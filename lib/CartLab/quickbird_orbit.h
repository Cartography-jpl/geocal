#ifndef QUICKBIRD_ORBIT_H
#define QUICKBIRD_ORBIT_H
#include "printable.h"
#include "geocal_time.h"
#include "orbit.h"
#include <vector>
#include <boost/array.hpp>
#include <fstream>
#include <boost/regex.hpp>
#include <boost/foreach.hpp>
#include <boost/math/quaternion.hpp>

namespace GeoCal {
/****************************************************************//**
  Reading QuickBird ephemeris and attitude is almost identical. We
  put most of the functionality here, and then derive classes with
  the few differences between attitude and ephemeris reading.
*******************************************************************/

template<int D> class QuickBirdFile {
public:
//-----------------------------------------------------------------------
/// Read the given file.
//-----------------------------------------------------------------------

  QuickBirdFile(const std::string& Fname)
  : fname(Fname)
  {
    std::ifstream in(Fname.c_str());
    in.exceptions(std::ifstream::eofbit | std::ifstream::failbit | 
		  std::ifstream::badbit);
    std::string buf = "";
    bool p1 = false, p2 = false, p3 = false;
    while(buf.find("List") == std::string::npos) {
      boost::smatch m;
      if(regex_match(buf, m, boost::regex("startTime\\s*=\\s*([^ ]+)\\s*;"))) {
	p1 = true;
	stime = Time::parse_time(m[1]);
      }
      if(regex_match(buf, m, boost::regex("numPoints\\s*=\\s*(\\d+)\\s*;"))) {
	p2 = true;
	d.resize(atoi(m[1].str().c_str()));
      }
      if(regex_match(buf, m, 
		     boost::regex("timeInterval\\s*=\\s*([\\d\\.]+)\\s*;"))) {
	p3 = true;
	tspace = atof(m[1].str().c_str());
      }
      getline(in, buf);
    }
    if(!(p1 && p2 && p3))
      throw Exception("File \"" + file_name() + "\" doesn't have all the expected values in it");
    typedef boost::array<double, D> d_ref;
    int lnum_expect = 1;
    BOOST_FOREACH(d_ref& e, d) {
      getline(in, buf);
      
      std::stringstream ins(buf);
      char ctrash;
      int lnum;
      ins >> ctrash >> lnum >> ctrash;
      if(lnum != lnum_expect) {
	Exception e;
	e << "File\"" << file_name() 
	  << "\" seems to be corrupt. Trouble reading line" << lnum_expect;
	throw e;
      }
      lnum_expect += 1;
      BOOST_FOREACH(double& er, e) {
	ins >> er >> ctrash;
      }
    }
  }

//-----------------------------------------------------------------------
/// Start time of data.
//-----------------------------------------------------------------------

  Time min_time() const {return stime; }

//-----------------------------------------------------------------------
/// End time of data.
//-----------------------------------------------------------------------

  Time max_time() const {return stime + time_spacing() * data().size(); }

//-----------------------------------------------------------------------
/// Spacing in seconds between points.
//-----------------------------------------------------------------------

  double time_spacing() const {return tspace; }

//-----------------------------------------------------------------------
/// Data in the file.
//-----------------------------------------------------------------------

  const std::vector<boost::array<double, D> >& data() const
  { return d; }

//-----------------------------------------------------------------------
/// File name we are reading.
//-----------------------------------------------------------------------

  const std::string& file_name() const {return fname;}
private:
  std::string fname;
  std::vector<boost::array<double, D> > d;
  double tspace;
  Time stime;
};

/****************************************************************//**
  This is a low level class that reads a Quickbird ephemeris file.
  You probably don't want to use this directly, but rather use the
  QuickBirdOrbit
*******************************************************************/

class QuickBirdEphemeris : public Printable<QuickBirdEphemeris>, 
			   public QuickBirdFile<12> {
public:
//-----------------------------------------------------------------------
/// Read the quickbird ephemeris file
//-----------------------------------------------------------------------

  QuickBirdEphemeris(const std::string& Fname) : QuickBirdFile<12>(Fname)
  { }

//-----------------------------------------------------------------------
/// Ephemeris data. This as 12 numbers in each entry. The first 3 are
/// the X, Y, and Z position in meters, in ECF (or ECR?) coordinate
/// system. The next 3 are the velocity in m/s. The final 6 are the
/// upper right coordinates of the position covariance matrix (so
/// order is (1, 1), (1, 2), (1, 3), (2, 2), (2,3), (3, 3)).
//-----------------------------------------------------------------------

  const std::vector<boost::array<double, 12> >& ephemeris() const
  { return data(); }

//-----------------------------------------------------------------------
/// Print to a stream.
//-----------------------------------------------------------------------

  void print(std::ostream& Os) const;
};

/****************************************************************//**
  This is a low level class that reads a Quickbird attitude file.
  You probably don't want to use this directly, but rather use the
  QuickBirdOrbit.

  Note a possible source of confusion. There are a few different
  conventions about the ordering of the quaternion coefficients. The
  boost library places the real part at the front, so we have a + b i
  + c j + d k and the quaternion is 4-tuple (a, b, c, d). The
  convention used by quickbird data is q1 i + q2 j + q3 k + q4 with
  the 4-tuple is (q1, q2, q3, q4). That means when we bring this over
  to the boost library, we need to reorder this to the 4-tuple (q4,
  q1, q2, q3).

  The code in QuickBirdOrbit accounts for these different conventions,
  but if you are using this class directly you need to be aware of
  this difference.
*******************************************************************/

class QuickBirdAttitude : public Printable<QuickBirdAttitude>, 
		          public QuickBirdFile<14> {
public:
//-----------------------------------------------------------------------
/// Read the quickbird attitude file
//-----------------------------------------------------------------------

  QuickBirdAttitude(const std::string& Fname) : QuickBirdFile<14>(Fname)
  { }

//-----------------------------------------------------------------------
/// Attitude data. The first 4 parameters are the quaternion
/// parameters (q1, q2, q3, q4).  The next 10 are the upper right
/// elements of the attitude quaternion covariance matrix.
///
/// Make sure to see the class notes for QuickBirdAttitude to see a
/// discussion about the difference between the boost and Quickbird
/// quaternion conventions.
//-----------------------------------------------------------------------

  const std::vector<boost::array<double, 14> >& attitude() const
  { return data(); }

//-----------------------------------------------------------------------
/// Print to a stream.
//-----------------------------------------------------------------------

  void print(std::ostream& Os) const;
};


/****************************************************************//**
  This is a Quickbird Orbit. This can be used for a rigorous model of
  Quickbird. 

  Note an important limitation of the rigorous model vs. RPC. We don't
  currently account for atmospheric refraction, while the RPC does.
  Depending on the zenith angle, this can be somewhat important. From
  the approximate atmospheric model described in "Theoretical Basis of
  the SDP Toolkit Geolocation package for the ECS", Table 6-5 the
  linear displacement for a zenith angle of 10 is 0.549 meters, 20
  degress is 1.223 meters, and 30 degrees is 2.221. The typical
  Quickbird scene has something like 10 to 20 degree zenith angles, so
  this is a correction of 1 or 2 pixels.

  We will need to add atmospheric refraction in the future, but this
  hasn't been done yet.
*******************************************************************/

class QuickBirdOrbit : public Orbit {
public:
  QuickBirdOrbit(const std::string& Ephemeris_file,
		 const std::string& Attitude_file = "");

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~QuickBirdOrbit() {}
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual void print(std::ostream& Os) const;
  std::string ephemeris_file_name() const {return eph->file_name();}
  std::string attitude_file_name() const {return att->file_name();}
private:
  boost::shared_ptr<QuickBirdEphemeris> eph;
  boost::shared_ptr<QuickBirdAttitude> att;
};
}
#endif
