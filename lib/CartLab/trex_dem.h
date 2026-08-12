#ifndef TREX_DEM_H
#define TREX_DEM_H
#include "dem_map_info.h"
#include "datum_geoid96.h"
#include "cart_lab_multifile.h"
#include "ostream_pad.h"
namespace GeoCal {
/****************************************************************//**
  This is used the read the TREx data. This gets used by TrexDem. 
  Although you can use this class directly, generally uou'll use this
  through TrexDem class.
*******************************************************************/

class TrexDemData: public VicarCartLabMultifile {
public:
  TrexDemData(const std::string& Dir,
	      bool No_coverage_is_error = true,
	      int Number_line_per_tile = -1,
	      int Number_sample_per_tile = -1, 
	      int Number_tile_each_file = 4, int Number_file = 4,
	      bool Favor_memory_mapped = true);
  virtual ~TrexDemData() { }
protected:
  TrexDemData() {}
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  This is used the read the TREx data. This gets used by TrexDem. 
  Although you can use this class directly, generally uou'll use this
  through TrexDem class.
*******************************************************************/

class GdalTrexDemData: public GdalCartLabMultifile {
public:
  GdalTrexDemData(const std::string& Dir,
	      bool No_coverage_is_error = true,
	      int Number_line_per_tile = -1,
	      int Number_sample_per_tile = -1, 
	      int Number_tile_each_file = 4, int Number_file = 4);
  virtual ~GdalTrexDemData() { }
protected:
  GdalTrexDemData() {}
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
  
/****************************************************************//**
  This is used the read the TREx LWM data.

  The values are:

  0=Ocean
  1=Coastline Land/Ocean transition
  2=Land
  3=Permanent In-Land Water
  4=Transitional In-land Water (mostly Permanent except in drought years)
  5=Seasonal In-land Water
  6=Ephemeral In-land Water
*******************************************************************/

class TrexLwmData: public VicarCartLabMultifile {
public:
  enum {OCEAN=0, COSTLINE=1, LAND=2, PERMANENT_INLAND_WATER=3,
	TRANSITIONAL_INLAND_WATER=4, SEASONAL_INLAND_WATER=5,
	EPHEMERAL_INLAND_WATER=6};
  TrexLwmData(const std::string& Dir="",
	      bool No_coverage_is_error = true,
	      int Number_line_per_tile = -1,
	      int Number_sample_per_tile = -1, 
	      int Number_tile_each_file = 4, int Number_file = 4,
	      bool Favor_memory_mapped = true);
  virtual ~TrexLwmData() { }
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
  
/****************************************************************//**
  This class provides access to the TREx.
*******************************************************************/

class TrexDem : public DemMapInfo {
public:
  TrexDem(const std::string& Dirbase ="",
	  bool Outside_dem_is_error = true,
	  const boost::shared_ptr<Datum>& D = 
	  boost::shared_ptr<Datum>(new DatumGeoid96()));
  virtual ~TrexDem() {}

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
/// Directory base that we read TREX data from.
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
    Os << "TREx DEM:\n"
       << "  Datum:\n";
    opad << datum();
    opad.strict_sync();
    Os << "  Data:\n";
    opad << *f;
    opad.strict_sync();
    Os << "  Outside Dem is error: " << outside_dem_is_error() << "\n";
  }
private:
  boost::shared_ptr<TrexDemData> f;
  std::string dbname, dirbase;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  This class provides access to the TREx.
*******************************************************************/

class GdalTrexDem : public DemMapInfo {
public:
  GdalTrexDem(const std::string& Dirbase ="",
	  bool Outside_dem_is_error = true,
	  const boost::shared_ptr<Datum>& D = 
	  boost::shared_ptr<Datum>(new DatumGeoid96()));
  virtual ~GdalTrexDem() {}


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
/// Directory base that we read TREX data from.
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
    Os << "GDAL TREx DEM:\n"
       << "  Datum:\n";
    opad << datum();
    opad.strict_sync();
    Os << "  Data:\n";
    opad << *f;
    opad.strict_sync();
    Os << "  Outside Dem is error: " << outside_dem_is_error() << "\n";
  }
private:
  boost::shared_ptr<GdalTrexDemData> f;
  std::string dbname, dirbase;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}
GEOCAL_EXPORT_KEY(TrexDemData);
GEOCAL_EXPORT_KEY(TrexLwmData);
GEOCAL_EXPORT_KEY(TrexDem);
GEOCAL_EXPORT_KEY(GdalTrexDemData);
GEOCAL_EXPORT_KEY(GdalTrexDem);
#endif
