#ifndef SRTM_DEM_H
#define SRTM_DEM_H
#include "dem_map_info.h"
#include "datum_geoid96.h"
#include "vicar_multi_file.h"
#include "ostream_pad.h"
namespace GeoCal {
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
  SrtmDem(const std::string& Db_name = "", const std::string& Dirbase ="",
	  bool Outside_dem_is_error = true,
	  const boost::shared_ptr<Datum>& D = 
	  boost::shared_ptr<Datum>(new DatumGeoid96()));
  virtual ~SrtmDem() {}


//-----------------------------------------------------------------------
/// Database name
//-----------------------------------------------------------------------

  const std::string& database_name() const {return dbname;}

//-----------------------------------------------------------------------
/// Database base directory
//-----------------------------------------------------------------------

  const std::string& directory_base() const {return dirbase;}

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
  boost::shared_ptr<VicarMultiFile> f;
  std::string dbname, dirbase;
#ifdef USE_BOOST_SERIALIZATON
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
    // Nothing to do
  }
#endif
};
}
#ifdef USE_BOOST_SERIALIZATON
// This is a little more complicated, because we can't really
// construct a object using a default constructor. So we need to
// directly handle the object construction.
namespace boost { namespace serialization {
template<class Archive> 
inline void save_construct_data(Archive & ar, const GeoCal::SrtmDem* d, 
			 const unsigned int version)
{
  void_cast_register(static_cast<GeoCal::SrtmDem*>(0),
		     static_cast<GeoCal::DemMapInfo*>(0));
  std::string database_name = d->database_name();
  std::string directory_base = d->directory_base();
  bool outside_dem_is_error = d->outside_dem_is_error();
  boost::shared_ptr<GeoCal::Datum> datum = d->datum_ptr();
  ar << GEOCAL_NVP(database_name)
     << GEOCAL_NVP(directory_base)
     << GEOCAL_NVP(outside_dem_is_error)
     << GEOCAL_NVP(datum);
}
template<class Archive>
inline void load_construct_data(Archive & ar, GeoCal::SrtmDem* d,
				const unsigned int version)
{
  void_cast_register(static_cast<GeoCal::SrtmDem*>(0),
		     static_cast<GeoCal::DemMapInfo*>(0));
  std::string database_name, directory_base;
  bool outside_dem_is_error;
  boost::shared_ptr<GeoCal::Datum> datum;
  ar >> GEOCAL_NVP(database_name)
     >> GEOCAL_NVP(directory_base)
     >> GEOCAL_NVP(outside_dem_is_error)
     >> GEOCAL_NVP(datum);
  ::new(d)GeoCal::SrtmDem(database_name, directory_base, outside_dem_is_error,
			  datum);
}
  }
}
#endif

#endif
