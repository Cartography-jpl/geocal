// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "air_mspi_l1b1.h"
%}
%base_import(raster_image_tiled_file)
%geocal_shared_ptr(GeoCal::AirMspiL1b1File);
%geocal_shared_ptr(GeoCal::AirMspiL1b1);

namespace GeoCal {
class AirMspiL1b1File: public GenericObject {
public:
  AirMspiL1b1File(const std::string& Fname, 
		  const std::string& Swath_to_use = "660-I",
		  int Min_l1b1_line = 0,
		  int Max_l1b1_line = -1,
		  int Tile_number_line = -1,
		  int Tile_number_sample = -1, 
		  unsigned int Number_tile = 4);
  %python_attribute(number_row_index, int);
  %python_attribute(file_name, std::string);
  %python_attribute(swath_to_use, std::string);
  %python_attribute(row_index_to_use, int);
  %python_attribute(row_number_to_use, int);
  %python_attribute(granule_id, std::string);
  %python_attribute(time, std::vector<Time>);
  std::vector<std::string> field_names(int Row_index) const;
  float wavelength(int Row_index) const;
  float polarization_angle(int Row_index) const;
  std::string swath_name(int Row_index) const;
  int row_index_to_row_number(int Row_index) const;
  int row_number_to_row_index(int Row_number) const;
  %pickle_serialization();
};

class AirMspiL1b1: public RasterImageTiledFile {
public:
  AirMspiL1b1(const std::string& Fname, 
	      const std::string& Swath_to_use = "660-I",
	      int Tile_number_line = -1,
	      int Tile_number_sample = -1, 
	      unsigned int Number_tile = 4);
  AirMspiL1b1(const boost::shared_ptr<AirMspiL1b1File>& L1b1_file,
	      int Tile_number_line = -1,
	      int Tile_number_sample = -1, 
	      unsigned int Number_tile = 4);
  %pickle_serialization();
};
}
