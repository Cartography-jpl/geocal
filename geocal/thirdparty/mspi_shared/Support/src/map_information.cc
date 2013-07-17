/*===========================================================================
=                                                                           =
=                               MapInformation                              =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MSPI
                                    Shared

             Copyright 2008, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!CLASS NAME: MapInformation

!ABSTRACT:

   This contains support routines for the class MapInformation 

!PARENT(S):

!REVISION HISTORY:

See ClearCase Log

=============================================================================
!END=======================================================================*/

#include "map_information.h"
				// Definition of class.
#include <cmath> 		// Definition of std::ceil
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.
#include <sstream> 		// Definition of ostringstream.
#include <boost/numeric/ublas/lu.hpp> 
				// Defintion of boost::numeric::ublas::permutation_matrix
#include <boost/numeric/ublas/vector_proxy.hpp> 
				// Defintion of ? (needed by lu_factorize)
#include <boost/numeric/ublas/vector.hpp> 
				// Defintion of ? (needed by lu_factorize)
#include <boost/numeric/ublas/triangular.hpp>
				// Defintion of ? (needed by lu_factorize)
#include <iomanip> 		// Definition of std::setprecision

/////////////////////////////////////////////////////////////////////////////
/// Constructor that takes a GDAL transform vector.
/// GDAL transform vector T is interpreted as follows:
///    X(sample,line) = T[0] + T[1] * sample + T[2] * line
///    Y(sample,line) = T[3] + T[4] * sample + T[5] * line
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::MapInformation::MapInformation(
  const std::vector<double>& T, 
  int Number_sample,
  int Number_line
) 
  : grid_size_x_(Number_sample),
    grid_size_y_(Number_line),
    resolution_x_(fabs(T[1])),
    resolution_y_(fabs(T[5])),
    f_(3,3)
{
  //------------------------------------------------------------------------
  // Initialize coordinate transforms.
  //------------------------------------------------------------------------

  f_(0,0) = T[1];
  f_(0,1) = T[2];
  f_(0,2) = T[0];
  f_(1,0) = T[4];
  f_(1,1) = T[5];
  f_(1,2) = T[3];
  f_(2,0) = 0;
  f_(2,1) = 0;
  f_(2,2) = 1;

  n_ = inverse(f_);

  //------------------------------------------------------------------------
  // Find the map corners.
  //------------------------------------------------------------------------

  std::vector<double> x_corners;
  std::vector<double> y_corners;
  for (int px = 0 ; px <= Number_sample ; px += Number_sample) {
    for (int py = 0 ; py <= Number_line ; py += Number_line) {
      double x,y;
      pixel_coor_to_map_coor(px,py,x,y);
      x_corners.push_back(x);
      y_corners.push_back(y);
    }
  }
  double min_x = *(std::min_element(x_corners.begin(),x_corners.end()));
  double max_x = *(std::max_element(x_corners.begin(),x_corners.end()));
  double min_y = *(std::min_element(y_corners.begin(),y_corners.end()));
  double max_y = *(std::max_element(y_corners.begin(),y_corners.end()));
  map_corner_ = BoxRegionCoor(min_x,min_y,max_x,max_y);

  //------------------------------------------------------------------------
  // Determine origin.
  //------------------------------------------------------------------------

  bool positive_x = f_(0,0) > 0;
  bool positive_y = f_(1,1) > 0;
  
  if (positive_x && !positive_y) {
    origin_code_ = ORIGIN_UL;
  }
  if (!positive_x && !positive_y) {
    origin_code_ = ORIGIN_UR;
  }
  if (positive_x && positive_y) {
    origin_code_ = ORIGIN_LL;
  }
  if (!positive_x && positive_y) {
    origin_code_ = ORIGIN_LR;
  }

  //------------------------------------------------------------------------
  // Assume pixel center registration (always true?)
  //------------------------------------------------------------------------

  pix_reg_code_ = PIX_REG_CENTER;

}

/////////////////////////////////////////////////////////////////////////////
/// Constructor.  Reads from ConfigFile
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::MapInformation::MapInformation(
  const ConfigFile& Config
) 
  : map_corner_(0,0,0,0)
{
  double min_x = Config.get<double>("MapInformation.min_x");
  double max_x = Config.get<double>("MapInformation.max_x");
  double min_y = Config.get<double>("MapInformation.min_y");
  double max_y = Config.get<double>("MapInformation.max_y");
  double res_x = Config.get<double>("MapInformation.res_x");
  double res_y = Config.get<double>("MapInformation.res_y");
  OriginCode origin_code = (OriginCode)Config.get<int>("MapInformation.origin_code");
  PixRegCode pix_reg_code = (PixRegCode)Config.get<int>("MapInformation.pix_reg_code");
  init2(min_x, min_y, max_x, max_y, res_x, res_y, origin_code, pix_reg_code);
}

/////////////////////////////////////////////////////////////////////////////
/// Write a ConfigFile format string containing the state of the class.
/////////////////////////////////////////////////////////////////////////////

std::string 
MSPI::Shared::MapInformation::config() const
{
  std::ostringstream config;
  config << std::setprecision(std::numeric_limits<double>::digits10)
	 << min_x() << " : MapInformation.min_x\n"
	 << max_x() << " : MapInformation.max_x\n"
	 << min_y() << " : MapInformation.min_y\n"
	 << max_y() << " : MapInformation.max_y\n"
	 << resolution_x() << " : MapInformation.res_x\n"
	 << resolution_y() << " : MapInformation.res_y\n"
	 << origin_code() << " : MapInformation.origin_code\n"
	 << pix_reg_code() << " : MapInformation.pix_reg_code\n";
  return config.str();
}

/////////////////////////////////////////////////////////////////////////////
/// Constructor.  Calculate the grid size dynamically based on the
/// given resolution.  If necessary, the maximum X,Y corner of the map
/// will be increased to match the pixel boundary.
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::MapInformation::MapInformation(
  double X1, 
  double Y1, 
  double X2, 
  double Y2,
  double Resolution_x, 
  double Resolution_y,
  OriginCode Origin_code, 
  PixRegCode Pix_reg_code
) 
  : map_corner_(0,0,0,0)
{
  init2(X1, Y1, X2, Y2, Resolution_x, Resolution_y, Origin_code, Pix_reg_code);
}

/////////////////////////////////////////////////////////////////////////////
/// Constructor.   The coordinates X1,Y1 and X2,Y2 define diagonally
/// opposing corners of the map in map units.  Grid_size_x, and
/// Grid_size_y defines the number of pixels in the grid.  Origin_code
/// defines the corner of the map at which pixel 0,0 is located. 
/// Possible values are:
///     ORIGIN_CODE_UL - Upper Left (min X, max Y)
///     ORIGIN_CODE_UR - Upper Right (max X, max Y)
///     ORIGIN_CODE_LL - Lower Left (min X, min Y)
///     ORIGIN_CODE_LR - Lower Right (max X, min Y)
///
/// Pix_reg_code defines whether a pixel value is related to the corner or
/// center of the corresponding area of that pixel on the map.  If the
/// corner is used, then it is always the corner corresponding to the
/// corner of the origin.  Possible values are:
///     PIX_REG_CODE_CENTER - Center
///     PIX_REG_CODE_CORNER - Corner
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::MapInformation::MapInformation(
  double X1,
  double Y1,
  double X2,
  double Y2,
  int Grid_size_x, 
  int Grid_size_y, 
  MSPI::Shared::MapInformation::OriginCode Origin_code,   
  MSPI::Shared::MapInformation::PixRegCode Pix_reg_code
) 
  : map_corner_(0,0,0,0)
{
  init(X1, Y1, X2, Y2, Grid_size_x, Grid_size_y, Origin_code, Pix_reg_code);
}

/////////////////////////////////////////////////////////////////////////////
/// Private helper function to initialize class.
/////////////////////////////////////////////////////////////////////////////

void
MSPI::Shared::MapInformation::init2(
  double X1, 
  double Y1, 
  double X2, 
  double Y2,
  double Resolution_x, 
  double Resolution_y,
  OriginCode Origin_code, 
  PixRegCode Pix_reg_code
) 
{
  BoxRegionCoor box(X1, Y1, X2, Y2);
  int grid_size_x = (int)std::ceil((box.max_x() - box.min_x()) / Resolution_x);
  int grid_size_y = (int)std::ceil((box.max_y() - box.min_y()) / Resolution_y);

  double max_x = box.min_x() + grid_size_x * Resolution_x;
  double max_y = box.min_y() + grid_size_y * Resolution_y;
  
  init(box.min_x(), box.min_y(), max_x, max_y, grid_size_x, grid_size_y, 
       Origin_code, Pix_reg_code);
}

/////////////////////////////////////////////////////////////////////////////
/// Private helper function to initialize class.
/////////////////////////////////////////////////////////////////////////////

void MSPI::Shared::MapInformation::init(
  double X1,
  double Y1,
  double X2,
  double Y2,
  int Grid_size_x, 
  int Grid_size_y, 
  MSPI::Shared::MapInformation::OriginCode Origin_code,   
  MSPI::Shared::MapInformation::PixRegCode Pix_reg_code
) 
{

//-----------------------------------------------------------------------
// Initialize private attributes.
//-----------------------------------------------------------------------

  map_corner_ = BoxRegionCoor(X1, Y1, X2, Y2);
  grid_size_x_ = Grid_size_x;
  grid_size_y_ = Grid_size_y;
  origin_code_ = Origin_code;
  pix_reg_code_ = Pix_reg_code;

//-----------------------------------------------------------------------
// Calculate pixel resolution. 
//-----------------------------------------------------------------------

  resolution_x_ = ( map_corner_.max_x() - map_corner_.min_x() ) / grid_size_x_;
  resolution_y_ = ( map_corner_.max_y() - map_corner_.min_y() ) / grid_size_y_;

//-----------------------------------------------------------------------
// Setup transform coefficients for converting pixel coordinates to 
// map coordinates.
//-----------------------------------------------------------------------

  f_.resize(3,3);
  f_(2,0) = 0;
  f_(2,1) = 0;
  f_(2,2) = 1;
  
  switch(Origin_code) {
  case ORIGIN_UL: 
    f_(0,2) = map_corner_.min_x();
    f_(1,2) = map_corner_.max_y();
    f_(0,0) = resolution_x_;
    f_(1,0) = 0;
    f_(0,1) = 0;
    f_(1,1) = -resolution_y_;
    break;
  case ORIGIN_UR:
    f_(0,2) = map_corner_.max_x();
    f_(1,2) = map_corner_.max_y();
    f_(0,0) = -resolution_x_;
    f_(1,0) = 0;
    f_(0,1) = 0;
    f_(1,1) = -resolution_y_;
    break;
  case ORIGIN_LL:
    f_(0,2) = map_corner_.min_x();
    f_(1,2) = map_corner_.min_y();
    f_(0,0) = resolution_x_;
    f_(1,0) = 0;
    f_(0,1) = 0;
    f_(1,1) = resolution_y_;
    break;
  case ORIGIN_LR:
    f_(0,2) = map_corner_.max_x();
    f_(1,2) = map_corner_.min_y();
    f_(0,0) = -resolution_x_;
    f_(1,0) = 0;
    f_(0,1) = 0;
    f_(1,1) = resolution_y_;
    break;
  default:
    // Unsupported Origin_code
    std::ostringstream message;
    message << "Unsupported origin code: " << Origin_code;
    throw MSPI_EXCEPTIONm(message.str());
  }

  switch(Pix_reg_code) {
  case PIX_REG_CORNER: break;
  case PIX_REG_CENTER:
    f_(0,2) += 0.5 * f_(0,0) + 0.5 * f_(0,1);
    f_(1,2) += 0.5 * f_(1,0) + 0.5 * f_(1,1);
    break;
  default:
    std::ostringstream message;
    message << "Unsupported pix_reg_code: " << Pix_reg_code;
    throw MSPI_EXCEPTIONm(message.str());
  }

//-----------------------------------------------------------------------
// Construct inverse transform from map coordinates to pixel coordinates.
//-----------------------------------------------------------------------

  n_ = inverse(f_);
}

/////////////////////////////////////////////////////////////////////////////
/// Return the inverse of the given matrix.
/////////////////////////////////////////////////////////////////////////////

boost::numeric::ublas::matrix<double> 
MSPI::Shared::MapInformation::inverse(
  const boost::numeric::ublas::matrix<double>& Matrix
)
{
  boost::numeric::ublas::matrix<double> A(Matrix);
  boost::numeric::ublas::permutation_matrix<std::size_t> pm(A.size1());
  int status = boost::numeric::ublas::lu_factorize(A,pm);
  if (status != 0) {
    throw MSPI_EXCEPTIONm("map_information : Trouble with lu_factorize");
  }
  n_.resize(3,3);
  boost::numeric::ublas::matrix<double> 
    result(boost::numeric::ublas::identity_matrix<double>(A.size1()));
  boost::numeric::ublas::lu_substitute(A, pm, result);
  return result;
}

/////////////////////////////////////////////////////////////////////////////
/// Calculate pixel coordinates corresponding to a given rectangular
/// region in map coordinates.  The returned BoxRegionCoor is in units of
/// pixels. 
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::BoxRegionCoor MSPI::Shared::MapInformation::map_coor_to_pixel_coor(
  const BoxRegionCoor& Map_region
) const
{

//-----------------------------------------------------------------------
// Convert map region corner coordinates to pixel coordinates.
//-----------------------------------------------------------------------

  double pixel_min_x;
  double pixel_min_y;
  double pixel_max_x;
  double pixel_max_y;

  map_coor_to_pixel_coor(Map_region.min_x(), Map_region.min_y(), 
			 pixel_min_x, pixel_min_y);

  map_coor_to_pixel_coor(Map_region.max_x(), Map_region.max_y(), 
			 pixel_max_x, pixel_max_y);

//-----------------------------------------------------------------------
// Return
//-----------------------------------------------------------------------

  return BoxRegionCoor(pixel_min_x, pixel_min_y, pixel_max_x, pixel_max_y);  
}

/////////////////////////////////////////////////////////////////////////////
/// Aligns the given region boundary to the nearest pixel boundary.
/// The returned Min_x, Min_y, Size_x, and Size_y defines the minimum size
/// region containing all the pixel centers within the area defined by the
/// map coordinates.  If the area defined by the map coordinates does 
/// not contain any pixel centers, then the returned size will be zero.
/// If a map region edge lies directly on a pixel center, that pixel
/// center will be excluded at the minimum edges of the region and 
/// included at the maximum edges.
/////////////////////////////////////////////////////////////////////////////

void MSPI::Shared::MapInformation::map_coor_to_pixel_coor(
const BoxRegionCoor &Map_region, 
int& Min_x,
int& Min_y, 
int& Size_x, 
int& Size_y
) const
{

//-----------------------------------------------------------------------
// Convert map region corner coordinates to pixel coordinates.
//-----------------------------------------------------------------------

  BoxRegionCoor pixel_coor = map_coor_to_pixel_coor(Map_region);

//-----------------------------------------------------------------------
// Round pixel coordinates to integer pixel offsets.               
// This aligns the region boundary to the nearest pixel boundary,  
// and effectively selects the minimum size region containing all  
// the pixel centers within the area defined by the pixel          
// coordinates.  
// 
// If a map region edge lies directly on a pixel center, that pixel
// center will be excluded on the minimum edges of the region and 
// included on the maximum edges.
//-----------------------------------------------------------------------

  // ceil(min + 100 * DBL_EPSILON), excludes pixel center on a minimum edge //
  Min_x = (int)std::ceil(pixel_coor.min_x() + 100 * std::numeric_limits<double>::epsilon());
  Min_y = (int)std::ceil(pixel_coor.min_y() + 100 * std::numeric_limits<double>::epsilon());

  // floor(max + 100 * DBL_EPSILON), includes pixel center on a maximum edge //
  int max_x = (int)std::floor(pixel_coor.max_x() + 100 * std::numeric_limits<double>::epsilon());
  int max_y = (int)std::floor(pixel_coor.max_y() + 100 * std::numeric_limits<double>::epsilon());

//-----------------------------------------------------------------------
// Calculate the region size.
// If the area defined by the pixel coordinates does 
// not contain any pixel centers, then set the size to zero.  
//-----------------------------------------------------------------------

  if (Min_x > max_x) {
    Size_x = 0;
  } else {
    Size_x = max_x - Min_x + 1;
  }

  if (Min_y > max_y) {
    Size_y = 0;
  } else {
    Size_y = max_y - Min_y + 1;
  }
}

/////////////////////////////////////////////////////////////////////////////
/// Calculate map coordinates corresponding to a given rectangular
/// region in pixel coordinates.  The returned BoxRegionCoor is in map
/// units.
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::BoxRegionCoor MSPI::Shared::MapInformation::pixel_coor_to_map_coor(
  const BoxRegionCoor& Pixel_region
) const
{

//-----------------------------------------------------------------------
// Convert pixel region corner coordinates to map coordinates.
//-----------------------------------------------------------------------

  double map_min_x;
  double map_min_y;
  double map_max_x;
  double map_max_y;

  pixel_coor_to_map_coor(Pixel_region.min_x(), Pixel_region.min_y(), 
			 map_min_x, map_min_y);

  pixel_coor_to_map_coor(Pixel_region.max_x(), Pixel_region.max_y(), 
			 map_max_x, map_max_y);

//-----------------------------------------------------------------------
// Return
//-----------------------------------------------------------------------

  return BoxRegionCoor(map_min_x, map_min_y, map_max_x, map_max_y);  
}
