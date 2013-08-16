/*===========================================================================
=                                                                           =
=                               AutoRemoveTempDir                           =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MSPI
                                     Shared

             Copyright 2008, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!CLASS NAME: AutoRemoveTempDir

!ABSTRACT:

   Creates temporary directory resource which is automatically removed
   on destruction of the class.

=============================================================================
!END=======================================================================*/

#ifndef AUTO_REMOVE_TEMP_DIR_H
#define AUTO_REMOVE_TEMP_DIR_H

#include <string>		// Definition of std::string
#include <vector> 		// Definition of std::vector
#include "boost/filesystem.hpp" // Definition of boost::filesystem

namespace MSPI {
  namespace Shared {
    class AutoRemoveTempDir;
  }
}

class MSPI::Shared::AutoRemoveTempDir {
public:
  AutoRemoveTempDir(const std::string& Tmp_dir, int Persistent = 0);
  // Use int for Persistent to avoid confusion with implicit conversion from char* to bool
  AutoRemoveTempDir(const std::string& Tmp_path, const std::string& Prefix, int Persistent = 0);
  void set_persistent();
  ~AutoRemoveTempDir();
  const std::string& dirname() const;
private:
  bool persistent_;
  std::string dirname_;
};

inline const std::string&
MSPI::Shared::AutoRemoveTempDir::dirname() const
{
  return dirname_;
}

inline void 
MSPI::Shared::AutoRemoveTempDir::set_persistent()
{
  persistent_ = true;
}

inline 
MSPI::Shared::AutoRemoveTempDir::~AutoRemoveTempDir()
{
  if (!persistent_) {
    boost::filesystem::remove_all(dirname_.c_str());
  }
}

#endif
