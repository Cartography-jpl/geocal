#ifndef SRTM_DEM_H
#define SRTM_DEM_H
#include "dem_map_info.h"
#include "datum_geoid96.h"
#include "cart_lab_multifile.h"
#include "ostream_pad.h"
namespace GeoCal {
/****************************************************************//**
  This is used the read the SRTM data. This gets used by SrtmDem. 
  Although you can use this class directly, generally uou'll use this
  through SrtmDem class.
*******************************************************************/

class SrtmDemData: public VicarCartLabMultifile {
public:
  SrtmDemData(const std::string& Dir,
	      bool No_coverage_is_error = true,
	      int Number_line_per_tile = -1,
	      int Number_sample_per_tile = -1, 
	      int Number_tile_each_file = 4, int Number_file = 4,
	      bool Favor_memory_mapped = true, bool Force_area_pixel = true);
  virtual ~SrtmDemData() { }
private:
  SrtmDemData() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  This class provides access to the SRTM.

  Note comment from Tom about this data:
  
  The srtm_filled data were prepared back in 2003. The goal was to
  create a quick and dirty global data set, which means the data came
  from a variety of sources, scales, datums, and accuracy. At that
  time, a half pixel error was well below the threshold of concern
  (15-30 was high resolution back then). So, point and area data were
  often mixed in the void filling process, and some regional areas
  (not covered by srtm) were carved into quadrangles and retained
  their individual pixel types. I know its a mess, but the bottom line
  is that the data are not accurate to half a pixel, which makes the
  'point vs. area' discuss not really relevant.

  The SRTM VICAR files incorrectly label the data as "pixel is point",
  when really this is "pixel is area". This matters, because it is a
  1/2 pixel shift.  The code overrides what the VICAR files says, and
  treats it as "area" rather than "pixel".
*******************************************************************/

class SrtmDem : public DemMapInfo {
public:
  SrtmDem(const std::string& Dirbase ="",
	  bool Outside_dem_is_error = true,
	  const boost::shared_ptr<Datum>& D = 
	  boost::shared_ptr<Datum>(new DatumGeoid96()));
  virtual ~SrtmDem() {}

//-----------------------------------------------------------------------
/// Directory base that we read SRTM data from.
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
    Os << "SRTM DEM:\n"
       << "  Datum:\n";
    opad << datum();
    opad.strict_sync();
    Os << "  Data:\n";
    opad << *f;
    opad.strict_sync();
    Os << "  Outside Dem is error: " << outside_dem_is_error() << "\n";
  }
private:
  boost::shared_ptr<SrtmDemData> f;
  std::string dbname, dirbase;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}
GEOCAL_EXPORT_KEY(SrtmDemData);
GEOCAL_EXPORT_KEY(SrtmDem);
#endif
