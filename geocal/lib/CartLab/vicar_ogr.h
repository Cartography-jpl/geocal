#ifndef VICAR_OGR_H
#define VICAR_OGR_H
#include "vicar_file.h"
#include "ogr_coordinate.h"
#include "map_info.h"
#include <map>

namespace GeoCal {
/****************************************************************//**
  This class is really part of VicarFile, but because of the
  complication in this software we separate this out into its own
  class. This class handles the reading and writing of the GeoTIFF map
  projection and coordinate transformation information, going to and
  from a MapInfo.

  AFIDS stores map projection information as text labels in a VICAR
  file. The text is GeoTIFF tags stored as text. We can't directly
  work with these to supply map projection information. Instead, we
  want to use the GDAL library to handle this (through the
  OgrCoordinate class). However, there is no easy mapping between GDAL
  which use the Well Known Text (WKT) to express its coordinate
  information and GeoTIFF which uses about 40 different tags for this
  information.

  The two systems contain similar information, so one possible
  approach would be to create a mapping between the two systems -
  e.g., Tag X corresponds to WKT node Y. While possible, this would
  result in a large amount of code.

  As an alternative, we take advantage of the ability of GDAL to
  create and read GeoTIFF files. The GDAL library contains all of the
  code connecting the two, which we don't want to duplicate.

  This class creates a temporary GeoTIFF file, and either writes map
  projection information using GDAL and a MapInfo, or from the VICAR
  GeoTIFF information. We then read the file and go the other way,
  creating a MapInfo or the metadata for a VICAR file. The temporary
  file is then removed.

  This is a bit awkward, but this is the best approach I could come up
  with to map VICAR and GDAL together. 
*******************************************************************/

class VicarOgr {
public:
  VicarOgr();
  MapInfo from_vicar(const VicarFile& F);
  void to_vicar(const MapInfo& Mi, VicarFile& F);
private:
  std::vector<int> geotiff_tag_ascii;
  std::vector<int> geotiff_tag_double;
  std::vector<int> geotiff_tag_short;
  mutable std::map<int, std::string> tag_to_vicar_name;
};
}
#endif
