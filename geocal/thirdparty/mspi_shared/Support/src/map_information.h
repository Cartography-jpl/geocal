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

=============================================================================
!END=======================================================================*/

#ifndef MAP_INFORMATION_H
#define MAP_INFORMATION_H

#include "box_region_coor.h"	// Definition of BoxRegionCoor
#include "MSPI-Shared/Config/src/config_file.h"
				// Definition of ConfigFile
#include <boost/numeric/ublas/matrix.hpp>
				// Definition of matrix
namespace MSPI {
  namespace Shared {
    class MapInformation;
  }
}

/////////////////////////////////////////////////////////////////////////////
/// @brief Defines the relationship between a rectangular map in map units
/// (e.g. Space Oblique Mercator X,Y) and a rectangular grid of pixels in
/// pixel units.
/////////////////////////////////////////////////////////////////////////////

class MSPI::Shared::MapInformation {
public:
  enum OriginCode { 
    ORIGIN_UL = 0, 
    ORIGIN_UR, 
    ORIGIN_LL, 
    ORIGIN_LR
  };

  enum PixRegCode {
    PIX_REG_CENTER = 0,
    PIX_REG_CORNER
  };

  MapInformation();

  MapInformation(const ConfigFile& Config);

  MapInformation(double X1, double Y1, double X2, double Y2,
		 int Grid_size_x, int Grid_size_y, 
		 OriginCode Origin_code,
		 PixRegCode Pix_reg_code);

  MapInformation(double X1, double Y1, double X2, double Y2,
		 double Resolution_x, double Resolution_y,
		 OriginCode Origin_code, PixRegCode Pix_reg_code);

  MapInformation(const std::vector<double>& T, 
		 int Number_sample, int Number_line);

  inline double min_x() const;
  inline double min_y() const;
  inline double max_x() const;
  inline double max_y() const;

  inline int grid_size_x() const;
  inline int grid_size_y() const;

  inline double resolution_x() const;
  inline double resolution_y() const;

  inline OriginCode origin_code() const;
  inline PixRegCode pix_reg_code() const;

  inline void map_coor_to_pixel_coor(double Map_x, double Map_y, 
				     double& Pixel_x, double& Pixel_y) const;

  BoxRegionCoor map_coor_to_pixel_coor(const BoxRegionCoor &Map_region) const; 

  void map_coor_to_pixel_coor(const BoxRegionCoor &Map_region, int& Min_x,
                              int& Min_y, int& Size_x, int& Size_y) const;

  inline void pixel_coor_to_map_coor(double Pixel_x, double Pixel_y, 
				     double& Map_x, double& Map_y) const;

  BoxRegionCoor pixel_coor_to_map_coor(const BoxRegionCoor &Pixel_region) const;
  std::string config() const;
  
protected:
  void init2(double X1, double Y1, double X2, double Y2,
	     double Resolution_x, double Resolution_y,
	     OriginCode Origin_code,
	     PixRegCode Pix_reg_code);
  void init(double X1, double Y1, double X2, double Y2,
	    int Grid_size_x, int Grid_size_y, 
	    OriginCode Origin_code,
	    PixRegCode Pix_reg_code);
private:
  boost::numeric::ublas::matrix<double> 
    inverse(const boost::numeric::ublas::matrix<double>& Matrix);
  BoxRegionCoor map_corner_;
  int grid_size_x_;
  int grid_size_y_;
  double resolution_x_;
  double resolution_y_;
  OriginCode origin_code_;
  PixRegCode pix_reg_code_;
  boost::numeric::ublas::matrix<double> f_;
  boost::numeric::ublas::matrix<double> n_;
};

/////////////////////////////////////////////////////////////////////////////
// Default constructor
/////////////////////////////////////////////////////////////////////////////

inline 
MSPI::Shared::MapInformation::MapInformation()
{
}

/////////////////////////////////////////////////////////////////////////////
/// Minimum X coordinate of region (or 0 if empty).
/////////////////////////////////////////////////////////////////////////////

inline double MSPI::Shared::MapInformation::min_x() const
{
  return map_corner_.min_x();
}

/////////////////////////////////////////////////////////////////////////////
/// Minimum Y coordinate of region (or 0 if empty).
/////////////////////////////////////////////////////////////////////////////

inline double MSPI::Shared::MapInformation::min_y() const
{
  return map_corner_.min_y();
}

/////////////////////////////////////////////////////////////////////////////
/// Maximum X coordinate of region (or 0 if empty).
/////////////////////////////////////////////////////////////////////////////

inline double MSPI::Shared::MapInformation::max_x() const
{
  return map_corner_.max_x();
}

/////////////////////////////////////////////////////////////////////////////
/// Maximum Y coordinate of region (or 0 if empty).
/////////////////////////////////////////////////////////////////////////////

inline double MSPI::Shared::MapInformation::max_y() const
{
  return map_corner_.max_y();
}

/////////////////////////////////////////////////////////////////////////////
/// Number of pixels along X-axis.
/////////////////////////////////////////////////////////////////////////////

inline int MSPI::Shared::MapInformation::grid_size_x() const
{
  return grid_size_x_;
}

/////////////////////////////////////////////////////////////////////////////
/// Number of pixels along Y-axis.
/////////////////////////////////////////////////////////////////////////////

inline int MSPI::Shared::MapInformation::grid_size_y() const
{
  return grid_size_y_;
}

/////////////////////////////////////////////////////////////////////////////
/// Pixel resolution along X-axis, in map units.
/////////////////////////////////////////////////////////////////////////////

inline double MSPI::Shared::MapInformation::resolution_x() const
{
  return resolution_x_;
}

/////////////////////////////////////////////////////////////////////////////
/// Pixel resolution along Y-axis, in map units.
/////////////////////////////////////////////////////////////////////////////

inline double MSPI::Shared::MapInformation::resolution_y() const
{
  return resolution_y_;
}

/////////////////////////////////////////////////////////////////////////////
/// 
/////////////////////////////////////////////////////////////////////////////

inline MSPI::Shared::MapInformation::OriginCode MSPI::Shared::MapInformation::origin_code() const
{
  return origin_code_;
}

/////////////////////////////////////////////////////////////////////////////
/// 
/////////////////////////////////////////////////////////////////////////////

inline MSPI::Shared::MapInformation::PixRegCode MSPI::Shared::MapInformation::pix_reg_code() const
{
  return pix_reg_code_;
}

/////////////////////////////////////////////////////////////////////////////
/// Calculate pixel coordinate corresponding to a given map coordinate.
/////////////////////////////////////////////////////////////////////////////

inline void MSPI::Shared::MapInformation::map_coor_to_pixel_coor(
  double Map_x, 
  double Map_y, 
  double& Pixel_x, 
  double& Pixel_y
) const
{
  Pixel_x = n_(0,2) + Map_x * n_(0,0) + Map_y * n_(0,1);
  Pixel_y = n_(1,2) + Map_x * n_(1,0) + Map_y * n_(1,1);
}

/////////////////////////////////////////////////////////////////////////////
/// Calculate map coordinate corresponding to a given pixel coordinate.
/////////////////////////////////////////////////////////////////////////////

inline void MSPI::Shared::MapInformation::pixel_coor_to_map_coor(
  double Pixel_x,
  double Pixel_y, 
  double& Map_x, 
  double& Map_y
) const
{
  Map_x = f_(0,2) + Pixel_x * f_(0,0) + Pixel_y * f_(0,1);
  Map_y = f_(1,2) + Pixel_x * f_(1,0) + Pixel_y * f_(1,1);
}

#endif
