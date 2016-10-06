#include "vicar_dem.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void VicarDem::save(Archive & ar, const unsigned int version) const
{
  unsigned int ntile = number_tile();
  int tile_number_line = number_line_per_tile();
  ar & GEOCAL_NVP2("number_tile", ntile)
    & GEOCAL_NVP(tile_number_line);
}

template<class Archive>
void VicarDem::load(Archive & ar, const unsigned int version)
{
  unsigned int ntile;
  int tile_number_line;
  ar & GEOCAL_NVP2("number_tile", ntile)
    & GEOCAL_NVP(tile_number_line);
  initialize(tile_number_line, ntile, datum_ptr(), outside_dem_is_error());
}

template<class Archive>
void VicarDem::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(DemMapInfo)
    & GEOCAL_NVP_(vicar_file);
  boost::serialization::split_member(ar, *this, version);
}

GEOCAL_IMPLEMENT(VicarDem);
#endif

//-----------------------------------------------------------------------
/// Initialize the class, once vicar_file_ has been filled in.
//-----------------------------------------------------------------------

void VicarDem::initialize(int Number_line_per_tile, int Number_tile,
			  const boost::shared_ptr<Datum>& D, 
			  bool Outside_dem_is_error)
{
  switch(vicar_file_->type()) {
  case VicarFile::VICAR_BYTE:
    initialize_t<VicarByte>(Number_line_per_tile, Number_tile, D,
			    Outside_dem_is_error);
    break;
  case VicarFile::VICAR_HALF:
    initialize_t<VicarHalf>(Number_line_per_tile, Number_tile, D,
			    Outside_dem_is_error);
    break;
  case VicarFile::VICAR_FULL:
    initialize_t<VicarFull>(Number_line_per_tile, Number_tile, D,
			    Outside_dem_is_error);
    break;
  case VicarFile::VICAR_FLOAT:
    initialize_t<VicarFloat>(Number_line_per_tile, Number_tile, D,
			    Outside_dem_is_error);
    break;
  case VicarFile::VICAR_DOUBLE:
    initialize_t<VicarDouble>(Number_line_per_tile, Number_tile, D,
			    Outside_dem_is_error);
    break;
  }
}
