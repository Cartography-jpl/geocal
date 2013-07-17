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

#include "scaled_double.h" 	// Definition of class
#include <limits> 		// Definition of std::numeric_limits

///////////////////////////////////////////////////////////////////////////////
/// Initializes scale factor and offset using the given sample data values.
///////////////////////////////////////////////////////////////////////////////

MSPI::Shared::ScaledDouble::ScaledDouble(
  const std::vector<double>& Data
)
 : min_(0),
   max_(0)
{
  bool first = true;
  for (size_t i = 0 ; i < Data.size() ; i++) {
    if (first || Data[i] < min_) {
      min_ = Data[i];
    }
    if (first || Data[i] > max_) {
      max_ = Data[i];
    }
    first = false;
  }
  
  scale_factor_ = (max_ - min_) / ((double)std::numeric_limits<int>::max() - std::numeric_limits<int>::min());
  offset_ = -scale_factor_ * std::numeric_limits<int>::min() + min_;
}

///////////////////////////////////////////////////////////////////////////////
/// Convert vector of scaled integer values to double.
///////////////////////////////////////////////////////////////////////////////

std::vector<double>
MSPI::Shared::ScaledDouble::unscale(
  const std::vector<int>& Data
) const
{
  std::vector<double> result(Data.size());
  for (size_t i = 0 ; i < Data.size() ; i++) {
    result[i] = unscale(Data[i]);
  }
  return result;
}

/////////////////////////////////////////////////////////////////////////////
/// Convert double to scaled integer.
/////////////////////////////////////////////////////////////////////////////

std::vector<int> 
MSPI::Shared::ScaledDouble::scale(
  const std::vector<double>& Data
) const
{ 
  std::vector<int> result(Data.size());
  for (size_t i = 0 ; i < Data.size() ; i++) {
    result[i] = scale(Data[i]);
  }
  return result;
}
