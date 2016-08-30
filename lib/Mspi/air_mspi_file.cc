#include "air_mspi_file.h"
// This won't be needed once version 3 becomes the default for boost filesystem
#define BOOST_FILESYSTEM_VERSION 3
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <cstdlib>
#include <iostream>
using namespace GeoCal;
using namespace boost::filesystem;

//-----------------------------------------------------------------------
/// We can't use canonical_path to normalize paths, because the file
/// pointed to might not exist, and canonical_path follows symbolic
/// links which we don't actually want.
///
/// Starting in boost 1.61, there is a lexically_normal
/// function. However we don't want to depend on that
/// new a version of boost, at least for now (2016). So we've copied
/// this function from boost 1.61, and massaged it a bit to be a free
/// function. This can go away in the future when boost 1.61 isn't
/// so new.
//-----------------------------------------------------------------------

path GeoCal::lexically_normal(const path& p)
{
  const char dot = '.';
  const char separator = '/';
  if(p.empty())
    return p;
  path temp;
  path::iterator start(p.begin());
  path::iterator last(p.end());
  path::iterator stop(last--);
  for(path::iterator itr(start); itr != stop; ++itr) {
    // ignore "." except at start and last
    if (itr->native().size() == 1
        && (itr->native())[0] == dot
        && itr != start
        && itr != last) continue;

    // ignore a name and following ".."
    if (!temp.empty()
        && itr->native().size() == 2
        && (itr->native())[0] == dot
        && (itr->native())[1] == dot) {
      std::string lf(temp.filename().native());  
      if (lf.size() > 0  
          && (lf.size() != 1
	      || (lf[0] != dot
		  && lf[0] != separator))
          && (lf.size() != 2 
	      || (lf[0] != dot
		  && lf[1] != dot)
	      )
          ) {
	temp.remove_filename();
	path::iterator next(itr);
	if (temp.empty() && ++next != stop
            && next == last && *last == ".")
	  temp /= ".";
	continue;
      }
    }

    temp /= *itr;
  };

  if (temp.empty())
    temp /= ".";
  return temp;
}


//-----------------------------------------------------------------------
/// Returns true if second path contains the first
//-----------------------------------------------------------------------

inline bool has_path(const path& p1,
		     const path& p2)
{
  path::iterator i2(p2.begin());
  for(path::iterator i1(p1.begin()); i1 != p1.end(); ++i1, ++i2) {
    if(i2 == p2.end())
      return false;
    if(i1->compare(*i2) != 0)
      return false;
  }
  return true;
}

//-----------------------------------------------------------------------
/// Removes second path from the first if it is present.
//-----------------------------------------------------------------------

inline path remove_path(const path& p1,
			const path& p2)
{
  if(!has_path(p1, p2))
    return p2;
  path::iterator i2(p2.begin());
  for(path::iterator i1(p1.begin()); i1 != p1.end(); ++i1, ++i2)
    ;
  path res("/");
  for(;i2 != p2.end(); ++i2)
    res /= *i2;
  return res;
}

//-----------------------------------------------------------------------
/// AirMSPI uses a file caching mechanism to speed up the processing
/// by copying files to a local disk before running executables. What
/// this means in practice is that a file can either be found in its
/// original location, or possibly in the directory pointed to by
/// MSPI_TEMP_DIR environment variable.
///
/// This routine handles the logic of determining the permanent file
/// name if given either a permanent file name or a local file name
/// (e.g., it strips off the MSPI_TEMP_DIR part).
//-----------------------------------------------------------------------

std::string GeoCal::air_mspi_permanent_file_name(const std::string& Fname)
{
  path res = lexically_normal(absolute(Fname));
  if(getenv("MSPI_TEMP_DIR")) {
    path t = lexically_normal(absolute(getenv("MSPI_TEMP_DIR")));
    res = remove_path(t, res);
  }
  return res.string();
}

//-----------------------------------------------------------------------
/// AirMSPI uses a file caching mechanism to speed up the processing
/// by copying files to a local disk before running executables. What
/// this means in practice is that a file can either be found in its
/// original location, or possibly in the directory pointed to by
/// MSPI_TEMP_DIR environment variable.
///
/// This routine handles the logic of determining the local file
/// name if given either a permanent file name or a local file name
/// (e.g., it adds the MSPI_TEMP_DIR part).
///
/// Since the file might not have been copied to the MSPI_TEMP_DIR, we
/// check if the file exists. If it doesn't we return the permanent
/// name instead.
//-----------------------------------------------------------------------

std::string GeoCal::air_mspi_local_file_name(const std::string& Fname)
{
  path res(air_mspi_permanent_file_name(Fname));
  if(getenv("MSPI_TEMP_DIR")) {
    path t = lexically_normal(absolute(getenv("MSPI_TEMP_DIR")));
    t /= res;
    if(exists(t))
      res = t;
  }
  return res.string();
}
