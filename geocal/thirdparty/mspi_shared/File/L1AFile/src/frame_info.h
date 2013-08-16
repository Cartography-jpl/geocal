/*===========================================================================
=                                                                           =
=                                  FrameInfo                                =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MSPI
                                    Shared

             Copyright 2008, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!CLASS NAME: FrameInfo

!ABSTRACT:

!DEFINITIONS:

!DERIVED FROM:

!DESCRIPTION:

!PUBLIC MEMBERS:

!PROTECTED MEMBERS:

!PRIVATE MEMBERS:

!ASSOCIATED FUNCTIONS:

!KNOWN BUGS AND LIMITATIONS:

!REVISION HISTORY:

See ClearCase Log

=============================================================================
!END=======================================================================*/

#ifndef FRAME_INFO_H
#define FRAME_INFO_H

namespace MSPI {
  namespace Shared {
    class FrameInfo;
  }
}

/////////////////////////////////////////////////////////////////////////////
/// @brief Derives frame info (tframe, omega_bar and omega_beat) from
///        frequency of first and second PEM.
///
///   "f1" is the frequency of the first PEM.
///   "f2" is the frequency of the second PEM.
///
///   "T" is the high-frequency cycle time.
///   "tframe" is the duration of one frame.
///   "omega_bar" is the average frequency.
///   "omega_beat" is the beat frequency.
/////////////////////////////////////////////////////////////////////////////

class MSPI::Shared::FrameInfo {
public:

  FrameInfo();

  FrameInfo(double f1, double f2, double Tframe_measured, double Timetag);

  FrameInfo(double T, double Tframe_nominal, double Omega_bar, double Omega_beat,
	    double Tframe_measured, double Timetag);

  double T() const;

  double tframe_nominal() const;

  double omega_bar() const;

  double omega_beat() const;

  double tframe_measured() const;

  double timetag() const;
 
protected:

private:
  double T_;			// High-frequency cycle time
  double tframe_nominal_;       // Nominal frame duration (in seconds), based on PEM frequencies.
  double omega_bar_;		// Average frequency
  double omega_beat_;		// Beat frequency
  double tframe_measured_;      // Measured frame duration (in seconds), based on hardware timer.
  double timetag_;		// Measured time offset relative to frame 0 in seconds.
};

/////////////////////////////////////////////////////////////////////////////
/// Returns T (high-frequency cycle time).
/////////////////////////////////////////////////////////////////////////////

inline double
MSPI::Shared::FrameInfo::T() const
{
  return T_;
}




/////////////////////////////////////////////////////////////////////////////
/// Returns nominal frame duration (in seconds), based on PEM frequency.
/////////////////////////////////////////////////////////////////////////////

inline double
MSPI::Shared::FrameInfo::tframe_nominal() const
{
  return tframe_nominal_;
}




/////////////////////////////////////////////////////////////////////////////
/// Returns omega_bar (average frequency).
/////////////////////////////////////////////////////////////////////////////

inline double
MSPI::Shared::FrameInfo::omega_bar() const
{
  return omega_bar_;
}




/////////////////////////////////////////////////////////////////////////////
/// Returns omega_beat (beat frequency).
/////////////////////////////////////////////////////////////////////////////

inline double
MSPI::Shared::FrameInfo::omega_beat() const
{
  return omega_beat_;
}

/////////////////////////////////////////////////////////////////////////////
/// Returns measured frame duration (in seconds), based on hardware timer.
/////////////////////////////////////////////////////////////////////////////

inline double
MSPI::Shared::FrameInfo::tframe_measured() const
{
  return tframe_measured_;
}

/////////////////////////////////////////////////////////////////////////////
/// Returns time offset in seconds relative to frame 0.
/////////////////////////////////////////////////////////////////////////////

inline double
MSPI::Shared::FrameInfo::timetag() const
{
  return timetag_;
}


#endif
