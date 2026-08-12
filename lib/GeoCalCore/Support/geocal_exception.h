#ifndef GEOCAL_EXCEPTION_H
#define GEOCAL_EXCEPTION_H

#include "printable.h"
#include "backtrace.hpp"
#include <sstream>		// Definition of ostringstream.
#include <gsl/gsl_errno.h>
#include <cmath>

namespace GeoCal {

/****************************************************************//**
  This is the base of the exception hierarchy for GeoCal code. This
  can be written to like a stream to contain information about the
  exception. This is derived from the standard library std::exception
*******************************************************************/

class Exception: public std::exception, public Printable<Exception>, 
		 public boost::backtrace {
public:
//-----------------------------------------------------------------------
/// Default constructor. Can give an optional string describing
/// the error.
//-----------------------------------------------------------------------

  Exception(const std::string& W = "") 
  { 
    // This reserve shouldn't really be necessary, but on a Mac
    // 10.4.11 using gcc 4.0.1, there is some kind of bug where we get
    // a "Double free" error when printing in Ruby. I never tracked
    // exactly where this occurred, but it was somewhere in the
    // iostream library when the buffer of os was resized. We just
    // reserve enough space up front so this isn't an issue. Since
    // this only gets called when an exception occurs, there shouldn't
    // be much of a performance issue with this.
    std::string buf("blah");
    buf.reserve(1000);
    s_.str(buf);
    s_ << W;  
  }

//-----------------------------------------------------------------------
/// Copy constructor.
//-----------------------------------------------------------------------

  Exception(const Exception& E)
    : boost::backtrace(E)
  {
    try {
      std::string cp(E.s_.str());
      s_.str(cp);
    } catch(...) {		// Ignore all errors.
    }
  }
  
//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------
    
  virtual ~Exception() throw() {}

//-----------------------------------------------------------------------
/// Write to exception what() string.
//-----------------------------------------------------------------------
    
  template<class T> inline Exception& operator<<(const T& V)
  {
    s_ << V;
    return *this;
  }

//-----------------------------------------------------------------------
/// Print out description of object.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const 
  {
    Os << "GeoCal Exception:\n"
       << "=========================\n" 
       << what() << "\n"
       << "=========================\n" 
       << "Backtrace:\n"
       << "=========================\n" 
       << boost::trace(*this) << "\n"
       << "=========================\n";
  }

//-----------------------------------------------------------------------
/// Description of what the error is.
//-----------------------------------------------------------------------

  virtual const char* what() const throw()
  {
    scratch = s_.str();
    return scratch.c_str();
  }
private:
  mutable std::string scratch;
  std::ostringstream s_;
};

/****************************************************************//**
   Exception thrown if Math routine fails to converge.
*******************************************************************/

class ConvergenceFailure : public Exception {
public:

//-----------------------------------------------------------------------
/// Default constructor. Can give an optional string describing
/// the error.
//-----------------------------------------------------------------------

  ConvergenceFailure(const std::string& W = "") : Exception(W) {}

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~ConvergenceFailure() throw() {}
};

/****************************************************************//**
   Exception thrown if no coverage.
*******************************************************************/

class NoCoverage : public Exception {
public:

//-----------------------------------------------------------------------
/// Default constructor. Can give an optional string describing
/// the error.
//-----------------------------------------------------------------------

  NoCoverage(const std::string& W = "") : Exception(W) {}

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~NoCoverage() throw() {}
};

/****************************************************************//**
   Exception thrown if Metadata is missing.
*******************************************************************/

class MetadataMissing : public Exception {
public:

//-----------------------------------------------------------------------
/// Default constructor. Can give an optional string describing
/// the error.
//-----------------------------------------------------------------------

  MetadataMissing(const std::string& W = "") : Exception(W) {}

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~MetadataMissing() throw() {}
};


/****************************************************************//**
   Exception thrown if Vicar call fails.
*******************************************************************/

class VicarException : public Exception {
public:

//-----------------------------------------------------------------------
/// Default constructor. Can give an optional string describing
/// the error.
//-----------------------------------------------------------------------

  VicarException(int status, const std::string& W = "") : Exception(W) 
  { *this << "\nVicar Status Code: " << status; }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~VicarException() throw() {}
};

/****************************************************************//**
   Exception thrown if Vicar isn't available.
*******************************************************************/

class VicarNotAvailableException : public Exception {
public:
//-----------------------------------------------------------------------
/// Constructor
//-----------------------------------------------------------------------

  VicarNotAvailableException() : Exception("Geocal library wasn't configured to use VICAR during installation, so can't call Vicar functions.")
  { }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~VicarNotAvailableException() throw() {}
};

/****************************************************************//**
   Exception thrown if Spice isn't available.
*******************************************************************/

class SpiceNotAvailableException : public Exception {
public:
//-----------------------------------------------------------------------
/// Constructor
//-----------------------------------------------------------------------

  SpiceNotAvailableException() : Exception("Geocal library wasn't configured to use SPICE during installation, so can't call Spice functions.")
  { }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~SpiceNotAvailableException() throw() {}
};

/****************************************************************//**
   Exception thrown if SDP Toolkit isn't available.
*******************************************************************/

class SdpNotAvailableException : public Exception {
public:
//-----------------------------------------------------------------------
/// Constructor
//-----------------------------------------------------------------------

  SdpNotAvailableException() : Exception("Geocal library wasn't configured to use SDP Toolkit during installation, so can't call SDP functions.")
  { }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~SdpNotAvailableException() throw() {}
};

/****************************************************************//**
   Exception thrown if GDAL isn't available.
*******************************************************************/

class GdalNotAvailableException : public Exception {
public:
//-----------------------------------------------------------------------
/// Constructor
//-----------------------------------------------------------------------

  GdalNotAvailableException() : Exception("Geocal library wasn't configured to use GDAL during installation, so can't call GDAL functions.")
  { }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~GdalNotAvailableException() throw() {}
};



/** \defgroup Error Error checking routines */
/*@{*/

//-----------------------------------------------------------------------
/// We can have range check fail because the value is something like a
/// nan. We still want to consider this an invalid number, so add a test.
/// This only applies to floating point number, so for simplicity we
/// have a dummy function for other types.
//-----------------------------------------------------------------------

template<class T> inline bool nancheck(const T& UNUSED(Val))
{
  return false;
}

template<> inline bool nancheck(const double& Val)
{
  return std::isnan(Val);
}

template<> inline bool nancheck(const float& Val)
{
  return std::isnan(Val);
}

template<> inline bool nancheck(const long double& Val)
{
  return std::isnan(Val);
}

//-----------------------------------------------------------------------
/// Range check
//-----------------------------------------------------------------------

template <class T> inline void range_check_template(
const T&	   Val,		// Value to be checked.
const T&	   Min,		// Minimum allowed value.
const T&	   Max,		// Maximum allowed value.
const char*        File,
int                Line
)
{
  if(Val < Min ||
     !(Val < Max) ||
     nancheck(Val)) {
    Exception e;
    e << "Out of range error in file " << File << " at line " << Line << "\n"
      << "Value:           " << Val << "\n"
      << "Minimum allowed: " << Min << "\n"
      << "Maximum allowed: " << Max;
    throw e;
  }
}

template <class T> inline void range_check_inclusive_template(
const T&	   Val,		// Value to be checked.
const T&	   Min,		// Minimum allowed value.
const T&	   Max,		// Maximum allowed value.
const char*        File,
int                Line
)
{
  if(Val < Min ||
     Val > Max ||
     nancheck(Val)) {
    Exception e;
    e << "Out of range error in file " << File << " at line " << Line << "\n"
      << "Value:           " << Val << "\n"
      << "Minimum allowed: " << Min << "\n"
      << "Maximum allowed: " << Max;
    throw e;
  }
}

//-----------------------------------------------------------------------
/// Range check
//-----------------------------------------------------------------------

#define range_check(V, Min, Max) \
      GeoCal::range_check_template(V, Min, Max, __FILE__, __LINE__)
#define range_check_inclusive(V, Min, Max) \
      GeoCal::range_check_inclusive_template(V, Min, Max, __FILE__, __LINE__)

//-----------------------------------------------------------------------
/// Range check
//-----------------------------------------------------------------------

template <class T> inline void range_min_check_template(
const T&	   Val,		// Value to be checked.
const T&	   Min,		// Minimum allowed value.
const char*        File,
int                Line
)
{
  if(Val < Min || nancheck(Val)) {
    Exception e;
    e << "Out of range error in file " << File << " at line " << Line << "\n"
      << "Value:           " << Val << "\n"
      << "Minimum allowed: " << Min << "\n";
    throw e;
  }
}

//-----------------------------------------------------------------------
/// Range check
//-----------------------------------------------------------------------

#define range_min_check(V, Min) \
      GeoCal::range_min_check_template(V, Min, __FILE__, __LINE__)

//-----------------------------------------------------------------------
/// Range check
//-----------------------------------------------------------------------

template <class T> inline void range_max_check_template(
const T&	   Val,		// Value to be checked.
const T&	   Max,		// Maximum allowed value.
const char*        File,
int                Line
)
{
  if(!(Val < Max) || nancheck(Val)) {
    Exception e;
    e << "Out of range error in file " << File << " at line " << Line << "\n"
      << "Value:           " << Val << "\n"
      << "Maximum allowed: " << Max << "\n";
    throw e;
  }
}

//-----------------------------------------------------------------------
/// Range check
//-----------------------------------------------------------------------

#define range_max_check(V, Max) \
      GeoCal::range_max_check_template(V, Max, __FILE__, __LINE__)

//-----------------------------------------------------------------------
/// Turn off gsl errors abort
//-----------------------------------------------------------------------

inline void no_gsl_abort()
{
  gsl_set_error_handler_off();
}

//-----------------------------------------------------------------------
/// Check for gsl errors
//-----------------------------------------------------------------------

inline void gsl_check_func
(int status,
const char* File,
int Line
)
{
  if(status != 0) {
    Exception e;
    e << "GSL error in file " << File << " at line " << Line << "\n"
      << "GSL error: " << gsl_strerror(status) << "\n";
    throw e;
  }
}

//-----------------------------------------------------------------------
/// GSL check
//-----------------------------------------------------------------------

#define gsl_check(status) \
      GeoCal::gsl_check_func(status, __FILE__, __LINE__)

}
/*@}*/
#endif
