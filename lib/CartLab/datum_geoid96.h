#ifndef DATUM_GEOID96_H
#define DATUM_GEOID96_H
#include "geocal_datum.h"
#include "vicar_raster_image.h"
namespace GeoCal {
/****************************************************************//**
  This class read the EGM 96 geoid data to provide a Datum.
*******************************************************************/

class DatumGeoid96 : public Datum {
public:
  DatumGeoid96(const std::string& Fname = "");
  virtual ~DatumGeoid96() {}
  const std::string& file_name() const { return data->vicar_file().file_name();}
  virtual double undulation(const GroundCoordinate& Gc) const;
  virtual double undulation(const Geodetic& Gc) const;
  virtual void print(std::ostream& Os) const;
private:
  boost::shared_ptr<VicarRasterImage> data;
#ifdef USE_BOOST_SERIALIZATON
  friend class boost::serialization::access;
  template<class Archive>
   void serialize(Archive & ar, const unsigned int version)
  {
    // Nothing to do here, since save_construct_data and
    // load_construct_data handles everything
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
inline void save_construct_data(Archive & ar, const GeoCal::DatumGeoid96* d, 
			 const unsigned int version)
{
  std::string file_name = d->file_name();
  detail::base_register<GeoCal::Datum, GeoCal::DatumGeoid96>::invoke();
  ar << BOOST_SERIALIZATION_NVP(file_name);
}
template<class Archive>
inline void load_construct_data(Archive & ar, GeoCal::DatumGeoid96* d,
				const unsigned int version)
{
  detail::base_register<GeoCal::Datum, GeoCal::DatumGeoid96>::invoke();
  std::string file_name;
  ar >> BOOST_SERIALIZATION_NVP(file_name);
  ::new(d)GeoCal::DatumGeoid96(file_name);
}
  }
}
#endif

#endif
