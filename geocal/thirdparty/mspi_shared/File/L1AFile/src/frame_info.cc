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

   This contains support routines for the class FrameInfo 

!PARENT(S):

!REVISION HISTORY:

See ClearCase Log

=============================================================================
!END=======================================================================*/

#include "frame_info.h" 	// Definition of class.
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.
#include <cmath>                // Definition of fabs() and M_PI.

/////////////////////////////////////////////////////////////////////////////
/// Default constructor with initialization to fill value.
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::FrameInfo::FrameInfo(
)
  : T_(-9999.0),
    tframe_nominal_(-9999.0),
    omega_bar_(-9999.0),
    omega_beat_(-9999.0),
    tframe_measured_(-9999.0),
    timetag_(-9999.0)
{
}

/////////////////////////////////////////////////////////////////////////////
/// Creates nominal frame info given PEM frequency and time measurements.
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::FrameInfo::FrameInfo(
  double F1,
  double F2,
  double Tframe_measured,
  double Timetag
)
{
  double omega1 = 2.0 * M_PI * F1;
  double omega2 = 2.0 * M_PI * F2;

  omega_bar_ = 0.5 * (omega1 + omega2);
  omega_beat_ = 0.5 * std::fabs(omega1 - omega2);
  tframe_nominal_ = M_PI / omega_beat_;
  T_ = 2.0 * M_PI / omega_bar_;
  tframe_measured_ = Tframe_measured;
  timetag_ = Timetag;
}



/////////////////////////////////////////////////////////////////////////////
/// Creates frame info given each element explicitly.
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::FrameInfo::FrameInfo(
  double T,
  double Tframe_nominal,
  double Omega_bar,
  double Omega_beat,
  double Tframe_measured,
  double Timetag
)
{
  T_ = T;
  tframe_nominal_ = Tframe_nominal;
  omega_bar_ = Omega_bar;
  omega_beat_ = Omega_beat;
  tframe_measured_ = Tframe_measured;
  timetag_ = Timetag;
}
