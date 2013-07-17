//===========================================================================
//                                                                          
//                              ScaledDouble                                
//                                                                          
//===========================================================================
//
//             Copyright 2013, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//=============================================================================

#ifndef SCALED_DOUBLE_H
#define SCALED_DOUBLE_H

#include <vector> 		// Definition of std::vector
#include <cmath>		// Definition of std::floor
#include <sstream> 		// Definition of std::ostringstream
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm

namespace MSPI {
  namespace Shared {
    class ScaledDouble;
  }
}

/////////////////////////////////////////////////////////////////////////////
/// @brief Provides operations for translating between double and scaled 
/// integer values.
/////////////////////////////////////////////////////////////////////////////

class MSPI::Shared::ScaledDouble {
public:
  ScaledDouble(const std::vector<double>& Data);
  double min() const;
  double max() const;
  int scale(double Value) const;
  double unscale(int Value) const;
  std::vector<int> scale(const std::vector<double>& Data) const;
  std::vector<double> unscale(const std::vector<int>& Data) const;
private:  
  double min_;
  double max_;
  double offset_;
  double scale_factor_;
};


/////////////////////////////////////////////////////////////////////////////
/// Return minimum floating-point value allowed.
/////////////////////////////////////////////////////////////////////////////

inline double
MSPI::Shared::ScaledDouble::min() const
{
  return min_;
}

/////////////////////////////////////////////////////////////////////////////
/// Return maximum floating-point value allowed.
/////////////////////////////////////////////////////////////////////////////

inline double
MSPI::Shared::ScaledDouble::max() const
{
  return max_;
}

/////////////////////////////////////////////////////////////////////////////
/// Return scaled integer representation for the given floating-point value.
/////////////////////////////////////////////////////////////////////////////

inline int
MSPI::Shared::ScaledDouble::scale(double Value) const
{
  if (Value < min_) {
    std::ostringstream msg;
    msg << "ScaledDouble::scale: exceeded min = " << min_ 
	<< ", value = " << Value << ", delta = " << Value - min_;
    throw MSPI_EXCEPTIONm(msg.str());
  }
  if (Value > max_) {
    std::ostringstream msg;
    msg << "ScaledDouble::scale: exceeded max = " << max_ 
	<< ", value = " << Value << ", delta = " << Value - max_;
    throw MSPI_EXCEPTIONm(msg.str());
  }
  return (int)std::floor((Value - offset_) / scale_factor_ + 0.5);
}

/////////////////////////////////////////////////////////////////////////////
/// Return floating-point representation for the given scaled integer value.
/////////////////////////////////////////////////////////////////////////////

inline double
MSPI::Shared::ScaledDouble::unscale(int Value) const
{
  return Value * scale_factor_ + offset_;
}

#endif
