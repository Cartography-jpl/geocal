#include "sdp_helper.h"
#include "ground_coordinate.h"
#include "geocal_exception.h"
#ifdef HAVE_SDP
#define LINUX			// Prevent error message when
				// including next file.
#include <PGS_TD.h>
#include <PGS_CSC.h>
#endif

using namespace GeoCal;

//-----------------------------------------------------------------------
/// This converts CCSDS ASCII time format (e.g.,
/// "1996-07-03T04:13:57.987654Z") to Time.
//-----------------------------------------------------------------------

Time SdpHelper::parse_time(const std::string& Time_string)
{
#ifdef HAVE_SDP
  double pgs;
  PGSt_SMF_code pgs_status_code = 
    PGS_TD_UTCtoTAI(const_cast<char*>(Time_string.c_str()), &pgs);
  if(pgs_status_code != PGS_S_SUCCESS) {
    Exception e("Call to PGS_TD_UTCtoTAI failed with message");
    char m[PGS_SMF_MAX_MSG_SIZE];
    PGS_SMF_GetMsgByCode(pgs_status_code, m);
    e << m;
    throw e;
  }
  return Time::time_pgs(pgs);
#else
  throw SdpNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// This converts Time to CCSDS ASCII time format (e.g.,
/// "1996-07-03T04:13:57.987654Z").
//-----------------------------------------------------------------------

std::string SdpHelper::to_string(const Time& T)
{
#ifdef HAVE_SDP
  char temp[28];
  PGSt_SMF_code pgs_status_code = PGS_TD_TAItoUTC(T.pgs(), temp);
  if(pgs_status_code != PGS_S_SUCCESS) {
    Exception e("Call to PGS_TD_UTCtoTAI failed with message");
    char m[PGS_SMF_MAX_MSG_SIZE];
    PGS_SMF_GetMsgByCode(pgs_status_code, m);
    e << m;
    throw e;
  }
  return temp;
#else
  throw SdpNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// This converts from Ecr to Eci.
//-----------------------------------------------------------------------

void SdpHelper::convert_to_eci(const Time& T, const CartesianFixed& From,
			       CartesianInertial& To)
{
#ifdef HAVE_SDP
  PGSt_double ecr[6];
  PGSt_double eci[6];
  PGSt_double offset = 0.0;
  ecr[0] = From.position[0];
  ecr[1] = From.position[1];
  ecr[2] = From.position[2];
  ecr[3] = ecr[4] = ecr[5] = 0;
  PGSt_SMF_status status = 
    PGS_CSC_ECRtoECI(1, const_cast<char*>(T.to_string().c_str()),
		     &offset, &ecr, &eci);
  if(status != PGSCSC_W_PREDICTED_UT1 &&
     status != PGS_S_SUCCESS) {
    Exception e("Call to PGS_CSC_ECRtoECI failed with message");
    char m[PGS_SMF_MAX_MSG_SIZE];
    PGS_SMF_GetMsgByCode(status, m);
    e << m;
    throw e;
  }
  To.position[0] = eci[0];
  To.position[1] = eci[1];
  To.position[2] = eci[2];
#else
  throw SdpNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// This converts from Eci to Ecr.
//-----------------------------------------------------------------------

void SdpHelper::convert_to_ecr(const Time& T, double Ci_to_cf[3][3])
{
#ifdef HAVE_SDP
  PGSt_double eci[3][6] = {{1, 0, 0, 0, 0, 0}, 
			   {0, 1, 0, 0, 0, 0}, 
			   {0, 0, 1, 0, 0, 0}};
  PGSt_double ecr[3][6];
  PGSt_double offset = 0.0;
  PGSt_SMF_status status = 
    PGS_CSC_ECItoECR(3, const_cast<char*>(T.to_string().c_str()),
		     &offset, eci, ecr);
  if(status != PGSCSC_W_PREDICTED_UT1 &&
     status != PGS_S_SUCCESS) {
    Exception e("Call to PGS_CSC_ECRtoECI failed with message");
    char m[PGS_SMF_MAX_MSG_SIZE];
    PGS_SMF_GetMsgByCode(status, m);
    e << m;
    throw e;
  }
  Ci_to_cf[0][0] = ecr[0][0];
  Ci_to_cf[1][0] = ecr[0][1];
  Ci_to_cf[2][0] = ecr[0][2];
  Ci_to_cf[0][1] = ecr[1][0];
  Ci_to_cf[1][1] = ecr[1][1];
  Ci_to_cf[2][1] = ecr[1][2];
  Ci_to_cf[0][2] = ecr[2][0];
  Ci_to_cf[1][2] = ecr[2][1];
  Ci_to_cf[2][2] = ecr[2][2];
#else
  throw SdpNotAvailableException();
#endif
}

//-----------------------------------------------------------------------
/// This converts from Eci to Ecr.
//-----------------------------------------------------------------------

void SdpHelper::convert_to_ecr(const Time& T, const CartesianInertial& From,
			       CartesianFixed& To)
{
#ifdef HAVE_SDP
  PGSt_double ecr[6];
  PGSt_double eci[6];
  PGSt_double offset = 0.0;
  eci[0] = From.position[0];
  eci[1] = From.position[1];
  eci[2] = From.position[2];
  eci[3] = eci[4] = eci[5] = 0;
  PGSt_SMF_status status = 
    PGS_CSC_ECItoECR(1, const_cast<char*>(T.to_string().c_str()),
		     &offset, &eci, &ecr);
  if(status != PGSCSC_W_PREDICTED_UT1 &&
     status != PGS_S_SUCCESS) {
    Exception e("Call to PGS_CSC_ECItoECR failed with message");
    char m[PGS_SMF_MAX_MSG_SIZE];
    PGS_SMF_GetMsgByCode(status, m);
    e << m;
    throw e;
  }
  To.position[0] = ecr[0];
  To.position[1] = ecr[1];
  To.position[2] = ecr[2];
#else
  throw SdpNotAvailableException();
#endif
}

Time SdpToolkitTimeInterface::parse_time(const std::string Time_string) const
{ 
  return SdpHelper::parse_time(Time_string);
}

std::string SdpToolkitTimeInterface::to_string(const Time& T) const
{
  return SdpHelper::to_string(T);
}

//-----------------------------------------------------------------------
/// This converts from CartesianFixed to CartesianInertial for the
/// given body. We use the NAIF coding for the bodies (see the SPICE
/// documentation for details). Note that the SDP toolkit only can do
/// this for earth, any other body will trigger an error.
//-----------------------------------------------------------------------

void SdpToolkitCoordinateInterface::to_inertial(int Body_id, 
  const Time& T, const CartesianFixed& From, CartesianInertial& To)
{
  if(Body_id != 399)		// Are we Earth?
    throw Exception("The SDP toolkit only supports the Earth for converting to and from inertial coordinates");
  SdpHelper::convert_to_eci(T, From, To);
}

//-----------------------------------------------------------------------
/// Calculate matrix to  converts from CartesianInertial to
/// CartesianFixed for the given body. We use the NAIF coding for
/// the bodies (see the SPICE  documentation for details). Note that
/// the SDP toolkit only can do  this for earth, any other body will
/// trigger an error. 
//-----------------------------------------------------------------------

void SdpToolkitCoordinateInterface::to_fixed(int Body_id, 
const Time& T, double Ci_to_cf[3][3])
{
  if(Body_id != 399)		// Are we Earth?
    throw Exception("The SDP toolkit only supports the Earth for converting to and from inertial coordinates");
  SdpHelper::convert_to_ecr(T, Ci_to_cf);
}

//-----------------------------------------------------------------------
/// This converts from CartesianInertial to CartesianFixed for the
/// given body. We use the NAIF coding for the bodies (see the SPICE
/// documentation for details). Note that the SDP toolkit only can do
/// this for earth, any other body will trigger an error.
//-----------------------------------------------------------------------

void SdpToolkitCoordinateInterface::to_fixed(int Body_id, 
  const Time& T, const CartesianInertial& From, CartesianFixed& To)
{
  if(Body_id != 399)		// Are we Earth?
    throw Exception("The SDP toolkit only supports the Earth for converting to and from inertial coordinates");
  SdpHelper::convert_to_ecr(T, From, To);
}

void
SdpToolkitCoordinateInterface::sub_solar_point(int Body_id, const Time& T,
					       CartesianFixed& P)
{
  throw Exception("Not implemented for SDP toolkit");
}

double SdpToolkitCoordinateInterface::solar_distance(int Body_id, const Time& T)
{
  throw Exception("Not implemented for SDP toolkit");
}

//-----------------------------------------------------------------------
/// Return true if we have SDP functionality available, false otherwise.
//-----------------------------------------------------------------------

bool SdpHelper::sdp_available()
{
#ifdef HAVE_SDP
  return true;
#else
  return false;
#endif
}
