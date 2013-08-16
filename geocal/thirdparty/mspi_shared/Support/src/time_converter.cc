/*===========================================================================
=                                                                           =
=                       TimeConverter                                       =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory

             Copyright 2012, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

=============================================================================
!END=======================================================================*/

#include "time_converter.h"	// Definition of class
#include "MSPI-Shared/ErrorHandling/src/exception.h"
				// Definition of Exception.

/////////////////////////////////////////////////////////////////////////////
/// Constructor.
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::TimeConverter::TimeConverter(
  const std::string& Epoch,
  ///< String format time (e.g. 2000-01-01T00:00:00.000000Z)
  ///< Time zone extension is ignored. Time zone is always interpreted
  ///< as GMT. 
  signed long long Rate
  ///< Clock rate in cycles per second.
) 
{
  //------------------------------------------------------------------------
  // The boost library does not accept the "T" or the trailing "Z".
  //------------------------------------------------------------------------

  std::string epoch_copy(Epoch);

  size_t pos = epoch_copy.find_first_of('T',0);
  if (pos != std::string::npos) {
    epoch_copy[pos] = ' ';
  }

  if (epoch_copy[epoch_copy.size()-1] == 'Z') {
    epoch_copy.erase(epoch_copy.size()-1);
  }
  
  //------------------------------------------------------------------------
  // Parse time string.
  //------------------------------------------------------------------------

  boost::posix_time::ptime pt = boost::posix_time::time_from_string(epoch_copy);

  //------------------------------------------------------------------------
  // Initialize class.
  //------------------------------------------------------------------------

  init(pt, Rate);

}

/////////////////////////////////////////////////////////////////////////////
/// Constructor.
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::TimeConverter::TimeConverter(
  const boost::posix_time::ptime& Epoch,
  signed long long Rate
  ///< Clock rate in cycles per second.
) 
{
  init(Epoch, Rate);
}

/////////////////////////////////////////////////////////////////////////////
/// Class initializer
/////////////////////////////////////////////////////////////////////////////

void
MSPI::Shared::TimeConverter::init(
  const boost::posix_time::ptime& Epoch,
  signed long long Rate
  ///< Clock rate in cycles per second.
) 
{

  //------------------------------------------------------------------------
  // Argument check: Rate < 1
  //------------------------------------------------------------------------

  if (Rate < 1) {
    throw MSPI_EXCEPTIONm("Rate < 1");
  }

  //------------------------------------------------------------------------
  // Initialize private attributes.
  //------------------------------------------------------------------------

  epoch_ = Epoch;
  usec_ = Rate/1000000LL;
  half_usec_ = Rate/2000000LL;
  hour_ = Rate * 3600LL;

  //------------------------------------------------------------------------
  // Argument check: Insufficient clock resolution.
  //------------------------------------------------------------------------

  if (usec_ == 0) {
    throw MSPI_EXCEPTIONm("Clock must have at least 1 microsecond resolution.");
  }
}

/////////////////////////////////////////////////////////////////////////////
/// Convert integer timestamp to UTC time string.
/////////////////////////////////////////////////////////////////////////////

void
MSPI::Shared::TimeConverter::file_time_to_utc(
  signed long long Timestamp,
  char Utc[28]
) const
{
  using boost::posix_time::ptime;
  using boost::posix_time::hours;
  using boost::posix_time::microseconds;

  signed long long hour_rem = Timestamp % hour_;
  signed long long hour = Timestamp / hour_;
  signed long long usec_rem = hour_rem % usec_;
  signed long long usec = hour_rem / usec_;

  if (usec_rem == half_usec_) {
    usec += (usec % 2);  // Round up 0.5 remainder on odd numbers only
  } else if (usec_rem > half_usec_) {
    usec += 1;
  }

  ptime pt = (epoch_ + hours(hour) + microseconds(usec));

  sprintf(Utc,"%.28sZ",to_iso_extended_string(pt).c_str());
}
