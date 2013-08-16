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

!CLASS NAME: ProjectionInformation

!ABSTRACT:

   This contains support routines for the class ProjectionInformation 

!PARENT(S):

!REVISION HISTORY:

See ClearCase Log

=============================================================================
!END=======================================================================*/

#include "projection_information.h"
				// Definition of class.
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.
#include "cproj.h" 		// Definition of for_init()
#include <sstream>  		// Definition of std::ostringstream
#include <iomanip> 		// Defintion of std::setprecision

/////////////////////////////////////////////////////////////////////////////
/// Return well-known-text string describing the given spatial reference
/////////////////////////////////////////////////////////////////////////////

std::string
MSPI::Shared::ProjectionInformation::wkt(
  const OGRSpatialReference& Spatial_ref
) const
{
  char *p;
  Spatial_ref.exportToWkt(&p);
  std::string wkt(p);
  OGRFree(p);
  return wkt;
}

/////////////////////////////////////////////////////////////////////////////
/// Return human readable well-known-text string describing the given 
/// spatial reference.
/////////////////////////////////////////////////////////////////////////////

std::string
MSPI::Shared::ProjectionInformation::pretty_wkt(
  const OGRSpatialReference& Spatial_ref
) const
{
  char *p;
  Spatial_ref.exportToPrettyWkt(&p);
  std::string wkt(p);
  OGRFree(p);
  return wkt;
}

/////////////////////////////////////////////////////////////////////////////
/// Initialize class for the given map projection.
/////////////////////////////////////////////////////////////////////////////

void
MSPI::Shared::ProjectionInformation::init(
  const OGRSpatialReference& Map
)
{
  //------------------------------------------------------------------------
  // Set private attributes.
  //------------------------------------------------------------------------

  map_ = Map;

  //------------------------------------------------------------------------
  // Copy the geographic component of map projection.
  //------------------------------------------------------------------------

  geo_.CopyGeogCSFrom(&map_);

  //------------------------------------------------------------------------
  // Set radians units on the geographic component.  
  //------------------------------------------------------------------------

  geo_.SetAngularUnits(SRS_UA_RADIAN,1.0);

  //------------------------------------------------------------------------
  // Generate forward and inverse coordinate transforms.
  //------------------------------------------------------------------------

  geo_to_map_ = 
    boost::shared_ptr<OGRCoordinateTransformation>(OGRCreateCoordinateTransformation(&geo_, &map_));

  map_to_geo_ = 
    boost::shared_ptr<OGRCoordinateTransformation>(OGRCreateCoordinateTransformation(&map_, &geo_));

  //------------------------------------------------------------------------
  // Generate GCTP projection information.
  //------------------------------------------------------------------------

  {
    double *proj_param = NULL;
    OGRErr status = map_.exportToUSGS(&proj_code_,&zone_code_,&proj_param,&sphere_code_);
    if (status) {
      std::ostringstream msg;
      msg << "Trouble with OGRSpatialReference::exportToUSGS: status = " << status;
      throw MSPI_EXCEPTIONm(msg.str());
    }
    proj_param_.assign(proj_param,proj_param+ProjectionInformation::number_proj_param);
    free(proj_param);
  }
}

/////////////////////////////////////////////////////////////////////////////
/// Constructor
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::ProjectionInformation::ProjectionInformation(
  const std::string& Gdal_wkt
) 
{
  OGRSpatialReference map;
  std::vector<char> tmp(Gdal_wkt.begin(),Gdal_wkt.end());
  char *p = &tmp[0];
  OGRErr status = map.importFromWkt(&p);
  if (status) {
    std::ostringstream msg;
    msg << "Trouble with OGRSpatialReference::importFromWkt: status = " << status
	<< ", wkt = " << std::string(p);
    throw MSPI_EXCEPTIONm(msg.str());
  }
  init(map);
}

/////////////////////////////////////////////////////////////////////////////
/// Constructor
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::ProjectionInformation::ProjectionInformation(
  const ConfigFile& Config
) 
{
  OGRSpatialReference map;
  std::vector<double> 
    proj_param(Config.get<double>("ProjectionInformation.proj_param",
				  ProjectionInformation::number_proj_param));
  OGRErr status = map.importFromUSGS(Config.get<int>("ProjectionInformation.proj_code"),
				     Config.get<int>("ProjectionInformation.zone_code"),
				     &proj_param[0],
				     Config.get<int>("ProjectionInformation.sphere_code"));
  if (status) {
    std::ostringstream msg;
    msg << "Trouble with OGRSpatialReference::importFromUSGS(1): status = " << status;
    throw MSPI_EXCEPTIONm(msg.str());
  }
  init(map);
}

/////////////////////////////////////////////////////////////////////////////
/// Generate ConfigFile format string representing the state of the class.
/////////////////////////////////////////////////////////////////////////////

std::string
MSPI::Shared::ProjectionInformation::config() const
{
  std::ostringstream config;
  std::vector<double> param = proj_param();
  
  config << std::setprecision(std::numeric_limits<double>::digits10);
  for (size_t i = 0 ; i < param.size() ; i++) {
    config << param[i] << " ";
  }
  config << " : ProjectionInformation.proj_param\n"
	 << proj_code() << " : ProjectionInformation.proj_code\n"
	 << zone_code() << " : ProjectionInformation.zone_code\n"
	 << sphere_code() << " : ProjectionInformation.sphere_code\n";

  return config.str();
}

/////////////////////////////////////////////////////////////////////////////
/// Constructor
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::ProjectionInformation::ProjectionInformation(
  int Proj_code, 
  int Sphere_code, 
  int Zone_code, 
  const std::vector<double>& Proj_param
) 
{
  std::vector<double> proj_param_copy(Proj_param);
  OGRSpatialReference map;
  OGRErr status = map.importFromUSGS(Proj_code,Zone_code,&proj_param_copy[0],Sphere_code);
  if (status) {
    std::ostringstream msg;
    msg << "Trouble with OGRSpatialReference::importFromUSGS(2): status = " << status;
    throw MSPI_EXCEPTIONm(msg.str());
  }
  init(map);
}

/////////////////////////////////////////////////////////////////////////////
/// Convert latitude, longitude (in radians)  to map X,Y (in meters)
/////////////////////////////////////////////////////////////////////////////

void 
MSPI::Shared::ProjectionInformation::latlon_to_xy(
double Latitude, double Longitude, double& X, double& Y) const
{
  X = Longitude;
  Y = Latitude;
  int status = geo_to_map_->Transform(1,&X,&Y);
  if (!status) {
    std::ostringstream msg;
    msg << "Trouble with latlon_to_xy(double), OGRCoordinateTransform: status = " << status;
    throw MSPI_EXCEPTIONm(msg.str());
  }
}

/////////////////////////////////////////////////////////////////////////////
/// Convert latitude, longitude (in radians)  to map X,Y (in meters)
/////////////////////////////////////////////////////////////////////////////

void 
MSPI::Shared::ProjectionInformation::latlon_to_xy(
  const std::vector<double>& Latitude, 
  const std::vector<double>& Longitude, 
  std::vector<double>& X, 
  std::vector<double>& Y
) const
{

  //------------------------------------------------------------------------
  // Argument check: Latitude.size() != Longitude.size()
  //------------------------------------------------------------------------

  if (Latitude.size() != Longitude.size()) {
    throw MSPI_EXCEPTIONm("Latitude.size() != Longitude.size()\n");
  }

  //------------------------------------------------------------------------
  // Do conversion.
  //------------------------------------------------------------------------

  X = Longitude;
  Y = Latitude;
  int status = geo_to_map_->Transform(X.size(),&X[0],&Y[0]);
  if (!status) {
    std::ostringstream msg;
    msg << "Trouble with latlon_to_xy(vector), OGRCoordinateTransform: status = " << status;
    throw MSPI_EXCEPTIONm(msg.str());
  }
}

/////////////////////////////////////////////////////////////////////////////
/// Convert map X,Y (in meters) to latitude, longitude (in radians). 
/////////////////////////////////////////////////////////////////////////////

void 
MSPI::Shared::ProjectionInformation::xy_to_latlon(
double X, double Y, double& Latitude, double& Longitude) const
{
  Longitude = X;
  Latitude = Y;
  int status = map_to_geo_->Transform(1,&Longitude,&Latitude);
  if (!status) {
    std::ostringstream msg;
    msg << "Trouble with xy_to_latlon(double), OGRCoordinateTransform: status = " << status;
    throw MSPI_EXCEPTIONm(msg.str());
  }
}

/////////////////////////////////////////////////////////////////////////////
/// Convert map X,Y (in meters) to latitude, longitude (in radians). 
/////////////////////////////////////////////////////////////////////////////

void 
MSPI::Shared::ProjectionInformation::xy_to_latlon(
  const std::vector<double>& X, 
  const std::vector<double>& Y, 
  std::vector<double>& Latitude, 
  std::vector<double>& Longitude
) const
{

  //------------------------------------------------------------------------
  // Argument check: X.size() != Y.size()
  //------------------------------------------------------------------------

  if (X.size() != Y.size()) {
    throw MSPI_EXCEPTIONm("X.size() != Y.size()\n");
  }

  //------------------------------------------------------------------------
  // Do conversion.
  //------------------------------------------------------------------------

  Longitude = X;
  Latitude = Y;
  int status = map_to_geo_->Transform(Longitude.size(),&Longitude[0],&Latitude[0]);
  if (!status) {
    std::ostringstream msg;
    msg << "Trouble with xy_to_latlon(vector), OGRCoordinateTransform: status = " << status;
    throw MSPI_EXCEPTIONm(msg.str());
  }
}
