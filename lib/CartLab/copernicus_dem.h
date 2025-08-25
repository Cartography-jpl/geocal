#ifndef COPERNICUS_DEM_H
#define COPERNICUS_DEM_H
#include "dem_map_info.h"
#include "datum_geoid96.h"
#include "cart_lab_multifile.h"
#include "ostream_pad.h"
namespace GeoCal {
/****************************************************************//**
  This is used the read the Copernicus data. This gets used by CopernicusDem. 
  Although you can use this class directly, generally uou'll use this
  through CopernicusDem class.
*******************************************************************/

class CopernicusDemData: public GdalCartLabMultifile {
public:
  CopernicusDemData(const std::string& Dir,
		    bool No_coverage_is_error = true,
		    int Number_line_per_tile = -1,
		    int Number_sample_per_tile = -1, 
		    int Number_tile_each_file = 4, int Number_file = 4);
  virtual ~CopernicusDemData() { }
protected:
  CopernicusDemData() {}
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  This is used the read the Copernicus LWM data.

   0=land; 1=Ocean;
   2=Minimum Lake Drought (from "GSW Transitions" or original COPLWM);
   3=Additional to achieve Full COPLWM Lake extension;
   4=Minimum River Drought (from "GSW Transitions" or original COPLWM);
   5=Additional to achieve Full COPLWM River extension;
   6=Additional "GSW Transitions" Permanent lake/water extension
   7=Additional "GSW Transitions" Mostly Permanent lake/Water extension
   8=Additional "GSW Transitions" Seasonal Flood extensions
   9=Additional "GSW Transitions" Ephemeral/Rare Flood extensions

   Histogram Stretch to exactly match the original COPLWM (0-3; land/ocean/lake/river):

   stretch  LWM-to-Original
   table=(0,0.,1,1.,2,2.,3,2.,4,3.,5,3.,6,0.,7,0.,8,0.,9,0.)
   
  Histogram Stretch for maximum lake/wet years (0-3; land/ocean/lake/river):

    stretch  LWM-to-MaxWet
    table=(0,0.,1,1.,2,2.,3,2.,4,3.,5,3.,6,2.,7,2.,8,2.,9,2.)
    
  Histogram Stretch for maximum drought (0-3; land/ocean/lake/river):
  
    stretch  LWM-to-MaxDry table=(0,0.,1,1.,2,2.,3,0.,4,3.,5,0.,6,0.,7,0.,8,0.,9,0.)
*******************************************************************/

class CopernicusLwmData: public GdalCartLabMultifile {
public:
  enum {LAND=0, OCEAN=1, MIN_LAKE_DROUGHT=2, ADD_LAKE_DROUGHT=3,
	MIN_RIVER_DROUGHT=4, ADD_RIVER_DROUGHT=5, PERM_LAKE=6,
	MOSTLY_PERM_LAKE=7, SEASONAL_FLOOD=8, EPHEMERAL_FLOOD=9};
  CopernicusLwmData(const std::string& Dir,
		    bool No_coverage_is_error = true,
		    int Number_line_per_tile = -1,
		    int Number_sample_per_tile = -1, 
		    int Number_tile_each_file = 4, int Number_file = 4);
  virtual ~CopernicusLwmData() { }
protected:
  CopernicusLwmData() {}
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
  
/****************************************************************//**
  This class provides access to the Copernicus.
*******************************************************************/

class CopernicusDem : public DemMapInfo {
public:
  CopernicusDem(const std::string& Dirbase ="",
	  bool Outside_dem_is_error = true,
	  const boost::shared_ptr<Datum>& D = 
	  boost::shared_ptr<Datum>(new DatumGeoid96()));
  virtual ~CopernicusDem() {}

  virtual void change_to_geodetic360()
  {
    f->change_to_geodetic360();
    map_info_ = f->map_info();
  }
  
  virtual void change_to_geodetic()
  {
    f->change_to_geodetic();
    map_info_ = f->map_info();
  }
  
//-----------------------------------------------------------------------
/// Directory base that we read Copernicus data from.
//-----------------------------------------------------------------------

  const std::string& directory_base() const { return f->directory_base(); }

//-----------------------------------------------------------------------
/// Return height in meters relative to datum().
//-----------------------------------------------------------------------

  virtual double elevation(int Y_index, int X_index) const
  { 
    return (*f)(Y_index, X_index);
  }

//-----------------------------------------------------------------------
/// Write to a stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const
  {     
    OstreamPad opad(Os, "    ");
    Os << "Copernicus DEM:\n"
       << "  Datum:\n";
    opad << datum();
    opad.strict_sync();
    Os << "  Data:\n";
    opad << *f;
    opad.strict_sync();
    Os << "  Outside Dem is error: " << outside_dem_is_error() << "\n";
  }
private:
  boost::shared_ptr<CopernicusDemData> f;
  std::string dbname, dirbase;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}
GEOCAL_EXPORT_KEY(CopernicusDemData);
GEOCAL_EXPORT_KEY(CopernicusLwmData);
GEOCAL_EXPORT_KEY(CopernicusDem);
#endif
