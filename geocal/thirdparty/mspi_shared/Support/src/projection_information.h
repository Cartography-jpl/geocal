/*===========================================================================
=                                                                           =
=                               ProjectionInformation                       =
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

#ifndef PROJECTION_INFORMATION_H
#define PROJECTION_INFORMATION_H

#include <vector>		   // Definition of std::vector
#include "MSPI-Shared/Config/src/config_file.h"
				// Definition of ConfigFile
#include <ogr_spatialref.h>
				// Definition of OGRSpatialReferencea
#include <boost/shared_ptr.hpp> // Definition of boost::shared_ptr

namespace MSPI {
  namespace Shared {
    class ProjectionInformation;
  }
}

/////////////////////////////////////////////////////////////////////////////
/// @brief Defines a mapping between latitude/longitude coordinates and
/// X,Y map coordinates.
/////////////////////////////////////////////////////////////////////////////

class MSPI::Shared::ProjectionInformation {
public:

  ProjectionInformation();
  static const size_t number_proj_param = 15;

  virtual ~ProjectionInformation();
  ProjectionInformation(const std::string& Gdal_wkt);
  ProjectionInformation(const ConfigFile& Config);
  ProjectionInformation(int Proj_code, int Sphere_code, int Zone_code, 
			const std::vector<double>& Proj_param);

  inline int proj_code() const;

  virtual inline const std::vector<double>& proj_param() const;

  inline int sphere_code() const;
  
  inline int zone_code() const;

  virtual void latlon_to_xy(double Latitude, double Longitude, double& X,
			    double& Y) const;

  void latlon_to_xy(const std::vector<double>& Latitude, 
		    const std::vector<double>& Longitude, 
		    std::vector<double>& X, 
		    std::vector<double>& Y) const;

  virtual void xy_to_latlon(double X, double Y, double& Latitude, 
			    double& Longitude) const;

  void xy_to_latlon(const std::vector<double>& X, 
		    const std::vector<double>& Y, 
		    std::vector<double>& Latitude, 
		    std::vector<double>& Longitude) const;
  std::string wkt() const;
  std::string pretty_wkt() const;
  std::string config() const;

private:
  std::string wkt(const OGRSpatialReference& Spatial_ref) const;
  std::string pretty_wkt(const OGRSpatialReference& Spatial_ref) const;
  void init(const OGRSpatialReference& Map);
  OGRSpatialReference geo_;
  OGRSpatialReference map_;
  boost::shared_ptr<OGRCoordinateTransformation> geo_to_map_;
  boost::shared_ptr<OGRCoordinateTransformation> map_to_geo_;
  long proj_code_;
  long sphere_code_;
  long zone_code_;
  std::vector<double> proj_param_;
};

/////////////////////////////////////////////////////////////////////////////
/// Return well-known-text string describing the map projection.
/////////////////////////////////////////////////////////////////////////////

inline
std::string
MSPI::Shared::ProjectionInformation::wkt() const
{
  return wkt(map_);
}

/////////////////////////////////////////////////////////////////////////////
/// Return human readable well-known-text string describing the map projection.
/////////////////////////////////////////////////////////////////////////////

inline
std::string
MSPI::Shared::ProjectionInformation::pretty_wkt() const
{
  return pretty_wkt(map_);
}

/////////////////////////////////////////////////////////////////////////////
/// Default constructor
/////////////////////////////////////////////////////////////////////////////

inline 
MSPI::Shared::ProjectionInformation::ProjectionInformation()
  : proj_code_(0),
    sphere_code_(0),
    zone_code_(0)
{
}

/////////////////////////////////////////////////////////////////////////////
/// Return the GCTP projection code for this projection type.
/// GCTP projection codes are defined in HdfEosDef.h.
/////////////////////////////////////////////////////////////////////////////

inline int 
MSPI::Shared::ProjectionInformation::proj_code() const
{
  return proj_code_;
}

/////////////////////////////////////////////////////////////////////////////
/// Return the list of GCTP projection parameters.
/////////////////////////////////////////////////////////////////////////////

inline const std::vector<double>& 
MSPI::Shared::ProjectionInformation::proj_param() const
{
  return proj_param_;
}

/////////////////////////////////////////////////////////////////////////////
/// Return the Sphere code.
/////////////////////////////////////////////////////////////////////////////

inline int 
MSPI::Shared::ProjectionInformation::sphere_code() const
{

  return sphere_code_;
}

/////////////////////////////////////////////////////////////////////////////
/// Return the UTM zone code if applicable.
/////////////////////////////////////////////////////////////////////////////

inline int 
MSPI::Shared::ProjectionInformation::zone_code() const
{
  return zone_code_;
}

/////////////////////////////////////////////////////////////////////////////
/// Destructor
/////////////////////////////////////////////////////////////////////////////

inline 
MSPI::Shared::ProjectionInformation::~ProjectionInformation()
{
}

#endif
