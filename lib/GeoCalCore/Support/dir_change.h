#ifndef DIR_CHANGE_H
#define DIR_CHANGE_H
#include <string>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <iostream>
#include "geocal_exception.h"

namespace GeoCal {

//-----------------------------------------------------------------------
/// Utility class. This changes to a new directory, and on destruction
/// changes back.
//-----------------------------------------------------------------------

class DirChange {
public:
  DirChange(const std::string& newdir)
  {
    dirhandle = open(".", O_RDONLY);
    if(dirhandle == -1) {
      Exception e;
      e << "Open failed, error: " << strerror(errno) << "\n";
      throw e;
    }
    int status = chdir(newdir.c_str());
    if(status != 0) {
      close(dirhandle);
      Exception e;
      e << "Could not change to directory: " << newdir;
      throw e;
    }
  }
  // We can't throw an exception from a destructor (this calls
  // std::terminate), so just report a failure to change back.
  ~DirChange() 
  {
    int status = fchdir(dirhandle);
    if(status != 0)
      std::cerr << "DirChange: failed to change back to the original directory: "
		<< strerror(errno) << "\n";
    close(dirhandle);
  }
  DirChange(const DirChange&) = delete;
  DirChange& operator=(const DirChange&) = delete;
private:
  int dirhandle;
};
}
#endif
