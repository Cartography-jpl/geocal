#ifndef VICAR_DEM_H
#define VICAR_DEM_H
#include "dem_tiled_file.h"
#include "vicar_tiled_file.h"
#include "ostream_pad.h"

namespace GeoCal {
/****************************************************************//**
  This uses a VicarTiledFile<T> as a DEM.

  The type of T should be one we can convert to a double, and should
  give the height in meters above sea level. We do a bilinear
  interpolation to get values between points, and we optionally return
  a height of 0 outside of the range of where we have data (or
  alternatively, throw an exception).

  The default datum used is NoDatum, e.g., the heights in the VICAR
  file are relative to WGS-84. If desired, a different Datum can be
  given.
*******************************************************************/

class VicarDem : public DemTiledFile {
public:
//-----------------------------------------------------------------------
/// Open VICAR file as a DEM.
//-----------------------------------------------------------------------

  VicarDem(const std::string& Fname, 
	   bool Outside_dem_is_error = false, 
	   int Number_line_per_tile = 100, int Number_tile = 4,
	   const boost::shared_ptr<Datum>& D = 
	   boost::shared_ptr<Datum>(new NoDatum()))
    : vicar_file_(new VicarFile(Fname))
  {
    initialize(Number_line_per_tile, Number_tile, D, Outside_dem_is_error);
  }

//-----------------------------------------------------------------------
/// Open VICAR file as a DEM.
//-----------------------------------------------------------------------

  VicarDem(int Instance, 
	   bool Outside_dem_is_error = false, 
	   int Number_line_per_tile = 100, int Number_tile = 4,
	   const std::string& Name = "INP",
	   const boost::shared_ptr<Datum>& D = 
	   boost::shared_ptr<Datum>(new NoDatum()))
    : vicar_file_(new VicarFile(Instance, VicarFile::READ, Name))
  {
    initialize(Number_line_per_tile, Number_tile, D, Outside_dem_is_error);
  }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~VicarDem() {}

//-----------------------------------------------------------------------
/// Underlying file.
//-----------------------------------------------------------------------

  const VicarFile& vicar_file() const {return *vicar_file_;}

//-----------------------------------------------------------------------
/// Underlying file.
//-----------------------------------------------------------------------

  VicarFile& vicar_file() {return *vicar_file_;}
  boost::shared_ptr<VicarFile> vicar_file_ptr() const {return vicar_file_;}
//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const 
  { 
        OstreamPad opad(Os, "    ");
	Os << "Vicar Dem:\n"
	   << "  File:    " << vicar_file_->file_name() << "\n"
	   << "  Map info:\n";
	opad << map_info();
	opad.strict_sync();
	Os << "  Datum:\n";
	opad << datum();
	opad.strict_sync();
	Os << "  Outside Dem is error: " << outside_dem_is_error() << "\n";
  }
private:
  boost::shared_ptr<VicarFile> vicar_file_; 
				///< Underlying data.
  void initialize(int Number_line_per_tile, int Number_tile,
		  const boost::shared_ptr<Datum>& D, 
		  bool Outside_dem_is_error);

//-----------------------------------------------------------------------
/// Type dispatched initialization
//-----------------------------------------------------------------------

  template<class T> void initialize_t(int Number_line_per_tile,
				      int Number_tile,
				      const boost::shared_ptr<Datum>& D,
				      bool Outside_dem_is_error)
  {
    boost::shared_ptr<VicarTiledFile<T> > 
      t(new VicarTiledFile<T>(vicar_file_, 1, Number_line_per_tile, Number_tile));
    DemTiledFile::initialize(t, D, vicar_file_->map_info(), Outside_dem_is_error);
  }

#ifdef USE_BOOST_SERIALIZATON
  VicarDem() {}
  friend class boost::serialization::access;
  template<class Archive>
  void save(Archive & ar, const unsigned int version) const
  {
    using boost::serialization::make_nvp;
    ar << BOOST_SERIALIZATION_BASE_OBJECT_NVP(Dem);
    int ntile = number_tile();
    int number_tile_line = number_line_per_tile();
    bool oerror = outside_dem_is_error();
    boost::shared_ptr<Datum> d = datum_ptr();
    ar << GEOCAL_NVP_(vicar_file)
       << GEOCAL_NVP(number_tile_line)
       << make_nvp("number_tile", ntile)
       << make_nvp("datum", d)
       << make_nvp("outside_dem_is_error", oerror);
  }
  template<class Archive>
  void load(Archive & ar, const unsigned int version)
  {
    using boost::serialization::make_nvp;
    ar >> BOOST_SERIALIZATION_BASE_OBJECT_NVP(Dem);
    int ntile, number_tile_line;
    bool oerror;
    boost::shared_ptr<Datum> d;
    ar >> GEOCAL_NVP_(vicar_file) 
       >> GEOCAL_NVP(number_tile_line)
       >> make_nvp("number_tile", ntile)
       >> make_nvp("datum", d)
       >> make_nvp("outside_dem_is_error", oerror);
    initialize(number_tile_line, ntile, d, oerror);
  }
  BOOST_SERIALIZATION_SPLIT_MEMBER();
#endif  

};
}
#endif
