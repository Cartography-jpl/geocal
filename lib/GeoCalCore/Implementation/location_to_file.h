#ifndef LOCATION_TO_FILE_H
#define LOCATION_TO_FILE_H
#include "printable.h"
#include "geocal_exception.h"
#include <sdbx/RTree.h>
#include <sdbx/MemoryTreeBackend.h>
#include <vector>
#include <boost/scoped_ptr.hpp>

namespace GeoCal {
/****************************************************************//**
  A common implementation of MapProjectedMultifile is to have some 
  sort of a look up table that gives the file name for a particular
  location. This class is a thin wrapper around RTree to give a
  mapping between location and file name. We do this both because
  RTree's interface is a bit different than how we want to access
  this, and because we want to abstract out the use of the libsdbx
  library since this may go away at some point.

  Note that boost has added a library "geometry" that looks like it
  also has a R-* tree (as of version 1.47). There is no compelling
  reason right now to change to using this, but if libsdx ever becomes
  an issue we can switch to boost.
*******************************************************************/

class LocationToFile : public Printable<LocationToFile> {
private:
  typedef std::vector<std::string>::size_type value_type;
  typedef RTree<int, value_type, 2, 2, 4, MemoryTreeBackend> tree;
  typedef tree::BoundingBox box;
  typedef tree::OverlappingIterator iterator;
public:

//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

  LocationToFile() {}

//-----------------------------------------------------------------------
/// Add a file with the given range. The range is in x1 <= x < x2, y1
/// <= y < y.
//-----------------------------------------------------------------------
  
  void add(int x1, int y1, int x2, int y2, const std::string& fname)
  {
    range_min_check(x2, x1 + 1);
    range_min_check(y2, y1 + 1);
    filename.push_back(fname);
    if(!rtree.Insert(bounds(x1,y1,x2,y2), filename.size() - 1))
      throw Exception("Bad bound box when adding a file name.");
  }

//-----------------------------------------------------------------------
/// Find the filename that covers a given point. If multiple files
/// cover a point, this just returns one of them. If no filenames
/// cover the point, then we return the empty string "".
//-----------------------------------------------------------------------

  std::string find(int x, int y) const
  {
    boost::scoped_ptr<iterator> 
      i(rtree.FindOverlapping(bounds(x, y, x + 1, y + 1)));
    return ((i && !i->end()) ? filename[i->current().value] : "");
  }


//-----------------------------------------------------------------------
/// Find the filenames that covers a given region. If no filenames
/// cover the region, then we return a empty vector.
//-----------------------------------------------------------------------

  std::vector<std::string> find_region(int x, int y, int num_x_pixel,
				       int num_y_pixel) const
  {
    boost::scoped_ptr<iterator> 
      i(rtree.FindOverlapping(bounds(x, y, x + num_x_pixel, y + num_y_pixel)));
    std::vector<std::string> res;
    if(i && ! i->end())
      do
	res.push_back(filename[i->current().value]);
      while(i->next());
    return res;
  }

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  void print(std::ostream& Os) const 
  { Os << "Location to file with " << rtree.GetItemCount() << " entries\n"; }
private:
  mutable tree rtree;
  std::vector<std::string> filename;
  inline box bounds(int x1, int y1, int x2, int y2) const
  {
    box bb;
    bb.edges[0].first  = x1;
    bb.edges[0].second = x2;
    bb.edges[1].first  = y1;
    bb.edges[1].second = y2;
    return bb;
  }

};

}
#endif
