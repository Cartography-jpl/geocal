#ifndef RPC_H
#define RPC_H
#include "printable.h"
#include "image_coordinate.h"
#include "ground_coordinate.h"
#include "coordinate_converter.h"
#include "geocal_exception.h"
#include "dem.h"
#include "geocal_gsl_matrix.h"
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include <blitz/array.h>
#include <limits>
#include <cmath>
#include <set>
#include <vector>

namespace GeoCal {
  class RasterImage;
  class ImageGroundConnection;

/****************************************************************//**
   This is used to handle RPC (Rational Polynomial Coefficients).

   RPC is an alternative to using a rigorous camera/orbit model to
   map project satellite data.

   This is a common technique, and there are numerous references. One
   reference is Fraser, CS, Dial, G, Grodecki, J "Sensor orientation
   via RPCs" ISPRS J PHOTOGRAMM 60 (3): 182-194 MAY 2006.

   Note that there are two versions of the RPCs, type "A" and type
   "B". The difference is the order of the terms. The military tends
   to use "A" and the rest of the world tends to use "B". This class
   supports both types, based on the setting of the rpc_type variable.

   By definition of RPC, the leading term of the denominator
   coefficient should be 1.0 - so there are 20 degrees of freedom for
   the numerator but only 19 for the denominator. After some debate, I
   decided to do nothing in this class to enforce this restriction,
   you can assign a value other than 1 to the first coefficient of the
   denominator. However, this class will complain when you actually
   try to use this.

   The definition of line, sample and height offset and scale allow
   these numbers to be floats. However file formats such as geotiff
   and NITF treat these as integers. If you store a RPC into a file
   with one of these formats, the data will be truncated. If you plan
   on saving to one of these formats, you should be aware of this and
   keep these an whole numbers. This limitation is *not* present
   in the VICAR file format.

   It has never been 100% clear what "line and sample" means according
   to the standard. The best I can determine, the RPC is set up so
   integer values are the center of a pixel, e.g., (0,0) is the center
   of the upper left pixel. This seems to be the convention used by
   WV-2, see https://trac.osgeo.org/gdal/ticket/5993. Note that GDAL
   uses a convention where the upper left corner of the bounding box
   is (0,0) (so the center of the pixel is (0.5,0.5)). This is *not*
   the convention used by ImageCoordinate, so while the referenced
   ticket mentions a 0.5 offset being needed we don't actually need
   to do that in our code.
*******************************************************************/
class Rpc : public Printable<Rpc> {
public:
  Rpc();

//-----------------------------------------------------------------------
/// Type of RPC.
//-----------------------------------------------------------------------

  enum RpcType {RPC_A, RPC_B};

//-----------------------------------------------------------------------
/// Error- Bias. The one sigma (68%) time-varying error estimate 
/// assuming correlated images.
//-----------------------------------------------------------------------

  double error_bias;

//-----------------------------------------------------------------------
/// Error - Random. The one sigma (68%) time-varying error estimate
/// assuming uncorrelated images
//-----------------------------------------------------------------------

  double error_random;

//-----------------------------------------------------------------------
/// Type of RPC.
//-----------------------------------------------------------------------

  RpcType rpc_type;

//-----------------------------------------------------------------------
/// Height offset, in meters.
//-----------------------------------------------------------------------
  
  double height_offset;

//-----------------------------------------------------------------------
/// Height scale, in meters.
//-----------------------------------------------------------------------
  
  double height_scale;

//-----------------------------------------------------------------------
/// Latitude offset, in degrees.
//-----------------------------------------------------------------------
  
  double latitude_offset;

//-----------------------------------------------------------------------
/// Latitude scale, in degrees.
//-----------------------------------------------------------------------
  
  double latitude_scale;

//-----------------------------------------------------------------------
/// Longitude offset, in degrees.
//-----------------------------------------------------------------------
  
  double longitude_offset;

//-----------------------------------------------------------------------
/// Longitude scale, in degrees.
//-----------------------------------------------------------------------
  
  double longitude_scale;

//-----------------------------------------------------------------------
/// Line offset.
//-----------------------------------------------------------------------
  
  double line_offset;

//-----------------------------------------------------------------------
/// Line scale.
//-----------------------------------------------------------------------
  
  double line_scale;

//-----------------------------------------------------------------------
/// Sample offset.
//-----------------------------------------------------------------------
  
  double sample_offset;

//-----------------------------------------------------------------------
/// Sample scale.
//-----------------------------------------------------------------------
  
  double sample_scale;

//-----------------------------------------------------------------------
/// Line denominator coefficient
//-----------------------------------------------------------------------
  
  boost::array<double, 20> line_denominator;

//-----------------------------------------------------------------------
/// Line numerator coefficient
//-----------------------------------------------------------------------
  
  boost::array<double, 20> line_numerator;

//-----------------------------------------------------------------------
/// Sample denominator coefficient
//-----------------------------------------------------------------------
  
  boost::array<double, 20> sample_denominator;

//-----------------------------------------------------------------------
/// Sample numerator coefficient
//-----------------------------------------------------------------------
  
  boost::array<double, 20> sample_numerator;

//-----------------------------------------------------------------------
/// Line numerator parameters to fit for in fit(). If true, the
/// argument is fitted for, otherwise it is held constant. On
/// construction of the RPC, this is initialized to all false.
//-----------------------------------------------------------------------
  
  boost::array<bool, 20> fit_line_numerator;

//-----------------------------------------------------------------------
/// Sample numerator parameters to fit for in fit(). If true, the
/// argument is fitted for, otherwise it is held constant. On
/// construction of the RPC, this is initialized to all false.
//-----------------------------------------------------------------------
  
  boost::array<bool, 20> fit_sample_numerator;

//-----------------------------------------------------------------------
/// Type of coordinate we are generating (e.g., support
/// MarsPlanetocentric). If this is null, then we assume earth and use
/// Geodetic coordinates.
//-----------------------------------------------------------------------

  boost::shared_ptr<CoordinateConverter> coordinate_converter;
  static boost::shared_ptr<CoordinateConverter> default_coordinate_converter;

//-----------------------------------------------------------------------
/// Return NAIF code for planet RPC is for.
//-----------------------------------------------------------------------

  int naif_code() const { return coor_conv_or_default()->naif_code(); }

  void naif_code(int Naif_code);
  
//-----------------------------------------------------------------------
///
//-----------------------------------------------------------------------
  
  void print(std::ostream& Os) const;

  void fit_coarse(const std::vector<boost::shared_ptr<GroundCoordinate> >& Gc,
		  const std::vector<ImageCoordinate>& Ic);
  void fit(const std::vector<boost::shared_ptr<GroundCoordinate> >& Gc,
	   const std::vector<ImageCoordinate>& Ic, 
	   const std::vector<double>& Line_sigma,
	   const std::vector<double>& Sample_sigma,
	   std::set<int>& Blunder,
	   double Blunder_threshold = 3,
	   double Chisq_threshold = 0.8,
	   bool Blunder_detect = true);
  void fit_all(const std::vector<double>& Line,
	       const std::vector<double>& Sample,
	       const std::vector<double>& Latitude,
	       const std::vector<double>& Longitude,
	       const std::vector<double>& Height);
  boost::shared_ptr<GroundCoordinate> ground_coordinate(const ImageCoordinate& Ic, const Dem& D) const;
  boost::shared_ptr<GroundCoordinate> ground_coordinate(const ImageCoordinate& Ic, double Height) const;
  static Rpc generate_rpc(const ImageGroundConnection& Igc,
			  double Min_height, double Max_height,
			  int Nlat = 20, int Nlon = 20, int Nheight = 20,
			  bool Skip_masked_point = false,
			  bool Ignore_error = false);
	   
//-----------------------------------------------------------------------
/// Use the RPC to convert from a GroundCoordinate to image space.
//-----------------------------------------------------------------------
  
  ImageCoordinate image_coordinate(const GroundCoordinate& Gc) const
  { return image_coordinate(Gc.latitude(), Gc.longitude(), 
			    Gc.height_reference_surface()); }

//-----------------------------------------------------------------------
/// Convert from object space to image space (i.e., calculate RPC). The
/// longitude and latitude are given in degrees (this is the geodetic
/// latitude). The height is in meters, relative to the reference
/// ellipsoid (e.g., WGS-84).
//-----------------------------------------------------------------------

  ImageCoordinate image_coordinate(double Latitude, double Longitude, 
				   double Height_ellipsoid) const
  {
    check_valid();
    double x = (Latitude - latitude_offset) / latitude_scale;
    double y = (Longitude - longitude_offset) / longitude_scale;
    double z = (Height_ellipsoid - height_offset) / height_scale;
    double f1 = evaluate_polynomial(line_numerator, x, y, z);
    double f2 = evaluate_polynomial(line_denominator, x, y, z);
    double f3 = evaluate_polynomial(sample_numerator, x, y, z);
    double f4 = evaluate_polynomial(sample_denominator, x, y, z);
    return ImageCoordinate(f1 / f2 * line_scale + line_offset,
			   f3 / f4 * sample_scale + sample_offset);
  }

  blitz::Array<double, 2> 
  image_coordinate_jac_parm(double Latitude, double Longitude, 
			    double Height_ellipsoid) const;


//-----------------------------------------------------------------------
/// Jacobian of image_coordinate with respect to the parameters that
/// are marked as ones we are fitting. This has two rows, the first is
/// for line and the second sample. The columns are first the true
/// values in fit_line_numerator, and then fit_sample_numerator.
//-----------------------------------------------------------------------

  blitz::Array<double, 2> image_coordinate_jac_parm(const GroundCoordinate& Gc)
    const
  { return image_coordinate_jac_parm(Gc.latitude(), Gc.longitude(),
				     Gc.height_reference_surface()); }

//-----------------------------------------------------------------------
/// Jacobian of image_coordinate with respect to the
/// Latitude, Longitude, and Height_ellipsoid. This is a 2 x 3 matrix,
/// with the first row being line and the second sample. The columns
/// are in Latitude, Longitude and Height_ellipsoid order.
//-----------------------------------------------------------------------

  blitz::Array<double, 2> image_coordinate_jac(double Latitude, 
					      double Longitude, 
					      double Height_ellipsoid) const
  {
    check_valid();
    double x = (Latitude - latitude_offset) / latitude_scale;
    double y = (Longitude - longitude_offset) / longitude_scale;
    double z = (Height_ellipsoid - height_offset) / height_scale;
    double f1 = evaluate_polynomial(line_numerator, x, y, z);
    double f2 = evaluate_polynomial(line_denominator, x, y, z);
    double f3 = evaluate_polynomial(sample_numerator, x, y, z);
    double f4 = evaluate_polynomial(sample_denominator, x, y, z);
    double f1dx = evaluate_polynomial_dx(line_numerator, x, y, z);
    double f2dx = evaluate_polynomial_dx(line_denominator, x, y, z);
    double f3dx = evaluate_polynomial_dx(sample_numerator, x, y, z);
    double f4dx = evaluate_polynomial_dx(sample_denominator, x, y, z);
    double f1dy = evaluate_polynomial_dy(line_numerator, x, y, z);
    double f2dy = evaluate_polynomial_dy(line_denominator, x, y, z);
    double f3dy = evaluate_polynomial_dy(sample_numerator, x, y, z);
    double f4dy = evaluate_polynomial_dy(sample_denominator, x, y, z);
    double f1dz = evaluate_polynomial_dz(line_numerator, x, y, z);
    double f2dz = evaluate_polynomial_dz(line_denominator, x, y, z);
    double f3dz = evaluate_polynomial_dz(sample_numerator, x, y, z);
    double f4dz = evaluate_polynomial_dz(sample_denominator, x, y, z);
    blitz::Array<double, 2> res(2, 3);
    res(0, 0) = 
      (f1dx * f2 - f1 * f2dx) / (f2 * f2) * line_scale / latitude_scale;
    res(1, 0) = 
      (f3dx * f4 - f3 * f4dx) / (f4 * f4) * sample_scale / latitude_scale;
    res(0, 1) = 
      (f1dy * f2 - f1 * f2dy) / (f2 * f2) * line_scale / longitude_scale;
    res(1, 1) = 
      (f3dy * f4 - f3 * f4dy) / (f4 * f4) * sample_scale / longitude_scale;
    res(0, 2) = 
      (f1dz * f2 - f1 * f2dz) / (f2 * f2) * line_scale / height_scale;
    res(1, 2) = 
      (f3dz * f4 - f3 * f4dz) / (f4 * f4) * sample_scale / height_scale;
    return res;
  }

//-----------------------------------------------------------------------
/// Convert from object space to image space (i.e., calculate RPC). The
/// longitude and latitude are given in degrees (this is the geodetic
/// latitude). The height is in meters, relative to the reference
/// ellipsoid (e.g., WGS-84).
///
/// This converts a whole block of latitude, longitude and height
/// points at once. The results returned have a first dimension of 2,
/// the first value is line and the second sample.
//-----------------------------------------------------------------------

  template<int N> 
  blitz::Array<double, N + 1> image_coordinate(
    const blitz::Array<double, N>& Latitude, 
    const blitz::Array<double, N>& Longitude, 
    const blitz::Array<double, N>& Height_ellipsoid) const
  {
    check_valid();
    typedef blitz::Array<double, N> T;
    T x((Latitude - latitude_offset) / latitude_scale);
    T y((Longitude - longitude_offset) / longitude_scale);
    T z((Height_ellipsoid - height_offset) / height_scale);
    T f1(evaluate_polynomial(line_numerator, x, y, z));
    T f2(evaluate_polynomial(line_denominator, x, y, z));
    T f3(evaluate_polynomial(sample_numerator, x, y, z));
    T f4(evaluate_polynomial(sample_denominator, x, y, z));
    blitz::TinyVector<int, N + 1> sz;
    sz[0] = 2;
    for(int i = 0; i < N; ++i)
      sz[i + 1] = Latitude.extent(i);
    blitz::Array<double, N + 1> res(sz);
    subarray(0, res) = f1 / f2 * line_scale + line_offset;
    subarray(1, res) = f3 / f4 * sample_scale + sample_offset;
    return res;
  }

  double resolution_meter(const Dem& D) const;

  void rpc_project(RasterImage& Res, const RasterImage& Img, 
		   const Dem& D, double Line_scale = 1.0, 
		   double Sample_scale = 1.0) const;

  Rpc rpc_type_a() const;
  Rpc rpc_type_b() const;
private:
//-----------------------------------------------------------------------
/// Return coordinate_converter or default
//-----------------------------------------------------------------------
  boost::shared_ptr<CoordinateConverter> coor_conv_or_default() const
  { return (coordinate_converter ? coordinate_converter : default_coordinate_converter);
  }

//-----------------------------------------------------------------------
/// Check that RPC is valid
//-----------------------------------------------------------------------

  void check_valid() const 
  {
    if(fabs(line_denominator[0] - 1) > 
       10 * std::numeric_limits<double>::epsilon()  ||
       fabs(sample_denominator[0] - 1) >
       10 * std::numeric_limits<double>::epsilon())
      throw Exception("RPC must have first coefficient of denominator = 1");
    if(rpc_type != RPC_A && rpc_type != RPC_B)
      throw Exception("RPC must be type A or type B");
  }

//-----------------------------------------------------------------------
/// Evaluate polynomial.
//-----------------------------------------------------------------------

  template<class T> T evaluate_polynomial(const boost::array<double, 20>& c,
  const T& x, const T& y, const T& z) const
  {
    if(rpc_type == RPC_B) {
// Rewrite this to be more efficient to calculate
//       T res(c[0] + c[1] * y + c[2] * x + c[3] * z +
// 	    c[4] * x * y + c[5] * y * z + c[6] * x * z  + 
// 	    c[7] * y * y  + c[8] * x * x + c[9] * z * z  + 
// 	    c[10] * x * y * z  + c[11] * y * y * y + 
// 	    c[12] * x * x * y  + 
// 	    c[13] * y * z * z + c[14] * x * y * y +
// 	    c[15] * x * x * x + 
// 	    c[16] * x * z * z + c[17] * y * y * z  + 
// 	    c[18] * x * x * z + 
// 	    c[19] * z * z * z);
      T res(c[0] + 
	    x * (c[2] + 
		 x * (c[8] + 
		      c[15] * x +
		      c[12] * y + 
		      c[18] * z  
		      ) + 
		 y * (c[4] + 
		      c[14] * y +
		      c[10] * z 
		      ) + 
		 z * (c[6] + 
		      c[16] * z
		      )
		 ) + 
	    y * (c[1] + 
		 y * ( c[7] + 
		       c[11] * y + 
		       c[17] * z 
		       ) + 
		 z * (c[5] + 
		      c[13] * z) 
		 )+ 
 	    z * (c[3] +
		 z * (c[9]  + 
		      c[19] * z)
		 )
	    );
      return res;
    } else {
// Rewrite this to be more efficient to calculate
//       T res(c[0] + c[1] * y + c[2] * x + c[3] * z +
// 	    c[4] * x * y + c[5] * y * z + c[6] * x * z  + 
// 	    c[10] * y * y  + c[7] * x * x + c[8] * z * z  + 
// 	    c[9] * x * y * z  + c[11] * y * y * y + 
// 	    c[14] * x * x * y  + 
// 	    c[17] * y * z * z + c[12] * x * y * y +
// 	    c[15] * x * x * x + 
// 	    c[18] * x * z * z + c[13] * y * y * z  + 
// 	    c[16] * x * x * z + 
// 	    c[19] * z * z * z);
      T res(c[0] + 
	    x * (c[2] + 
		 x * (c[7] + 
		      c[15] * x +
		      c[14] * y + 
		      c[16] * z  
		      ) + 
		 y * (c[4] + 
		      c[12] * y +
		      c[9] * z 
		      ) + 
		 z * (c[6] + 
		      c[18] * z
		      )
		 ) + 
	    y * (c[1] + 
		 y * ( c[10] + 
		       c[11] * y + 
		       c[13] * z 
		       ) + 
		 z * (c[5] + 
		      c[17] * z) 
		 )+ 
 	    z * (c[3] +
		 z * (c[8]  + 
		      c[19] * z)
		 )
	    );
      return res;
    }
  }

//-----------------------------------------------------------------------
/// Derivative of polynomial with respect to z.
//-----------------------------------------------------------------------
  template<class T> T evaluate_polynomial_dz(const boost::array<double, 20>& c,
  const T& x, const T& y, const T& z) const
  {
    if(rpc_type == RPC_B) {
      T res(c[3] + c[5] * y + c[6] * x + 
	    2 * c[9] * z  + 
	    c[10] * x * y +
	    2 * c[13] * z * y + 
	    2 * c[16] * z * x + 
	    c[17] * y * y + 
	    c[18] * x * x + 
	    3 * c[19] * z * z);
      return res;
    } else {
      T res(c[3] + c[5] * y + c[6] * x + 
	    2 * c[8] * z  + 
	    c[9] * x * y +
	    2 * c[17] * z * y + 
	    2 * c[18] * z * x + 
	    c[13] * y * y + 
	    c[16] * x * x + 
	    3 * c[19] * z * z);
      return res;
    }
  }

//-----------------------------------------------------------------------
/// Derivative of polynomial with respect to x.
//-----------------------------------------------------------------------

  template<class T> T evaluate_polynomial_dx(const boost::array<double, 20>& c,
  const T& x, const T& y, const T& z) const
  {
    if(rpc_type == RPC_B) {
       T res(c[2] + c[4] * y + c[6] * z  + 2 * c[8] * x + c[10] * y * z  + 
	     2 * c[12] * x * y  + c[14] * y * y + 3 * c[15] * x * x + 
	     c[16] * z * z + 2 * c[18] * x * z);
      return res;
    } else {
       T res(c[2] + c[4] * y + c[6] * z  + 2 * c[7] * x + c[9] * y * z  + 
	     2 * c[14] * x * y  + c[12] * y * y + 3 * c[15] * x * x + 
	     c[18] * z * z + 2 * c[16] * x * z);
      return res;
    }
  }

//-----------------------------------------------------------------------
/// Derivative of polynomial with respect to y.
//-----------------------------------------------------------------------

  template<class T> T evaluate_polynomial_dy(const boost::array<double, 20>& c,
  const T& x, const T& y, const T& z) const
  {
    if(rpc_type == RPC_B) {
       T res(c[1] + c[4] * x + c[5] * z + 2 * c[7] * y  +
 	    c[10] * x * z  + 3 * c[11] * y * y + c[12] * x * x + 
 	    c[13] * z * z + 2 * c[14] * x * y + 2 * c[17] * y * z);
      return res;
    } else {
       T res(c[1] + c[4] * x + c[5] * z + 2 * c[10] * y  +
 	    c[9] * x * z  + 3 * c[11] * y * y + c[14] * x * x + 
 	    c[17] * z * z + 2 * c[12] * x * y + 2 * c[13] * y * z);
      return res;
    }
  }

  void fit_no_blunder(const blitz::Array<double, 1>& y,
		      const blitz::Array<double, 1>& w,
		      const blitz::Array<double, 2>& jac,
		      const std::vector<bool>& Blunder,
		      blitz::Array<double, 1>& Standard_residual,
		      std::vector<int>& Res_index_map,
		      double& Chisq,
		      GslVector& C);
		      
  void polynomial_jac(double x, double y, double z,
		      boost::array<double, 20>& jac) const;
  void jac_line(double x, double y, double z,
		blitz::Array<double, 1> jac) const;
  void jac_sample(double x, double y, double z,
		blitz::Array<double, 1> jac) const;

//-----------------------------------------------------------------------
/// Slice out an array based on the dimension.
//-----------------------------------------------------------------------
      
  blitz::Array<double, 1> subarray(int i, blitz::Array<double, 2>& M) const
  { return M(i, blitz::Range::all()); }
  blitz::Array<double, 2> subarray(int i, blitz::Array<double, 3>& M) const
  { return M(i, blitz::Range::all(), blitz::Range::all()); }
  blitz::Array<double, 3> subarray(int i, blitz::Array<double, 4>& M) const
  { return M(i, blitz::Range::all(), blitz::Range::all(), 
	     blitz::Range::all()); }
  blitz::Array<double, 4> subarray(int i, blitz::Array<double, 5>& M) const
  { return M(i, blitz::Range::all(), blitz::Range::all(), 
	     blitz::Range::all(),blitz::Range::all()); }
  blitz::Array<double, 5> subarray(int i, blitz::Array<double, 6>& M) const
  { return M(i, blitz::Range::all(), blitz::Range::all(), 
	     blitz::Range::all(),blitz::Range::all(), blitz::Range::all()); }
  blitz::Array<double, 6> subarray(int i, blitz::Array<double, 7>& M) const
  { return M(i, blitz::Range::all(), blitz::Range::all(), 
	     blitz::Range::all(),blitz::Range::all(), blitz::Range::all(),
	     blitz::Range::all()); }
  blitz::Array<double, 7> subarray(int i, blitz::Array<double, 8>& M) const
  { return M(i, blitz::Range::all(), blitz::Range::all(), 
	     blitz::Range::all(),blitz::Range::all(), blitz::Range::all(),
	     blitz::Range::all(),blitz::Range::all()); }

  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

}

GEOCAL_EXPORT_KEY(Rpc);
GEOCAL_CLASS_VERSION(Rpc, 1);
#endif

