// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "srtm_90m_dem.h"
%}

%base_import(dem_map_info)
%base_import(cart_lab_multifile)

%geocal_shared_ptr(GeoCal::Srtm90mData);
%geocal_shared_ptr(GeoCal::Srtm90mDem);
namespace GeoCal {
class Srtm90mData: public GdalCartLabMultifile {
public:
  enum {FILL_VALUE=0};
  Srtm90mData(const std::string& Dir,
	      bool No_coverage_is_error = true,
	      int Number_line_per_tile = -1,
	      int Number_sample_per_tile = -1, 
	      int Number_tile_each_file = 4, int Number_file = 4);
  %pickle_serialization();
};

class Srtm90mDem : public DemMapInfo {
public:
  Srtm90mDem(const std::string& Dir,
	  bool Outside_dem_is_error = true);
  Srtm90mDem(const std::string& Dir,
	  bool Outside_dem_is_error,
	  const boost::shared_ptr<Datum>& D);

  %python_attribute(directory_base, std::string);
  %python_attribute(number_file_read, int);
  void reset_number_file_read();
  %pickle_serialization();
protected:
  virtual double elevation(int Y_index, int X_index) const;
};
}
