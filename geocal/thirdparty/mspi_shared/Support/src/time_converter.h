/*===========================================================================
=                                                                           =
=                           TimeConverter                                   =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory

             Copyright 2012, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

=============================================================================
!END=======================================================================*/

#ifndef TIME_CONVERTER_H
#define TIME_CONVERTER_H

#include <boost/date_time.hpp>  // Definition of boost::posix_time

namespace MSPI {
  namespace Shared {
    class TimeConverter;
  }
}

////////////////////////////////////////////////////////////////////////////
/// @brief Provides conversion integer time stamp to UTC.
////////////////////////////////////////////////////////////////////////////

class MSPI::Shared::TimeConverter {
public:
  TimeConverter(const std::string& Epoch, 
		signed long long Rate);
  TimeConverter(const boost::posix_time::ptime& Epoch, 
		signed long long Rate);
  void file_time_to_utc(signed long long Ftime, char Utc[28]) const;
private:
  void init(const boost::posix_time::ptime& Epoch, 
	    signed long long Rate);
  boost::posix_time::ptime epoch_;
  signed long long usec_;
  signed long long half_usec_;
  signed long long hour_;
};

#endif
