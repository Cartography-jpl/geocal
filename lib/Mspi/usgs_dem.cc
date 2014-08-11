#include "usgs_dem.h"
#include <cstdlib>

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Constructor. You can provide the directory to look for USGS DEM
/// data, or if you leave this blank we use the value of environment
/// variable USGSDATA.
///
/// We don't have USGS files that completely cover the area. If you
/// ask for a point outside of the area this can either be treated as
/// an error, or alternatively you can return a value of FILL_VALUE
/// instead. This is controlled by No_coverage_is_error.
///
/// There are two kinds of tiling going on. At the top level, we have
/// a number of files open at one time, given by Number_file. For each
/// file, we read that it tiles with the given Number_line_per_tile x
/// Number_sample_per_tile
/// Number_tile_each_file tiles.
//-----------------------------------------------------------------------

UsgsDemData::UsgsDemData
(const std::string& Dir,
 bool No_coverage_is_error,
 int Number_line_per_tile,
 int Number_sample_per_tile, 
 int Number_tile_each_file, 
 int Number_file
)
: RasterMultifile(Number_file, No_coverage_is_error, FILL_VALUE),
  number_line_per_tile(Number_line_per_tile),
  number_sample_per_tile(Number_sample_per_tile),
  number_tile_each_file(Number_tile_each_file)
{
  std::string dirbase = Dir;
  if(dirbase == "") {
    char *t = getenv("USGSDATA");
    if(!t)
      throw Exception("Need to either pass in the directory of the USGS DEM data, or set the environment variable USGSDATA");
    dirbase = t;
  }
}
    
RasterMultifileTile UsgsDemData::get_file(int Line, int Sample) const
{
}



