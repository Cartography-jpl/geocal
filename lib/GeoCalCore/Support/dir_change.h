#ifndef DIR_CHANGE_H
#define DIR_CHANGE_H
#include <string>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
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
      Exception e;
      e << "Could not change to directory: " << newdir;
      throw e;
    }
  }
  ~DirChange() 
  {
    int status = fchdir(dirhandle);
    if(status != 0) {
      close(dirhandle);
      throw Exception("Call to fchdir failed");
    }
    close(dirhandle);
  }
private:
  int dirhandle;
};
}
#endif
