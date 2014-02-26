#include "srtm_dem.h"
#include <stdlib.h>

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Constructor. You can provide the database file to use and the 
/// directory where there data can be found. If you leave this as a
/// blank string, we use ${ELEV_ROOT} as the directory and
/// ${ELEV_ROOT}/L2_dem_db.int as the file.
//-----------------------------------------------------------------------

SrtmDem::SrtmDem(const std::string& Db_name, const std::string& Dirbase,
     bool Outside_dem_is_error,
     const boost::shared_ptr<Datum>& D)
{
  dbname = Db_name;
  dirbase = Dirbase;
  if(dirbase =="") {
    char *t = getenv("ELEV_ROOT");
    if(!t)
      throw Exception("Need to either pass in the directory base or set the environment variable ELEV_ROOT");
    dirbase = t;
  }
  if(dbname =="")
    dbname = dirbase + "/L2_dem_db.int";
  f.reset(new VicarMultiFile(dbname, dirbase, "", 10000, 10000, 100000, 4, 4, 
			     false, Outside_dem_is_error, 0, true));
  initialize(D, f->map_info(), Outside_dem_is_error);
}
