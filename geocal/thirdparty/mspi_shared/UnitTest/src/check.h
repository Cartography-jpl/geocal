/*===========================================================================
=                                                                           =
=                                 Check                                     =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory

             Copyright 2012, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

=============================================================================
!END=======================================================================*/

#ifndef CHECK_H
#define CHECK_H

#include "MSPI-Shared/UnitTest/src/float_comparator.h"
                                // Definition of FloatComparator
#include "MSPI-Shared/UnitTest/src/double_comparator.h"
                                // Definition of DoubleComparator
#include "boost/multi_array.hpp"  // Definition of boost::multi_array

namespace MSPI {
  namespace Shared {
    void file_check(const std::string& A, 
		    const std::string& B, 
		    int Line, 
		    bool& Error,
		    std::ostream& Out = std::cerr);

    void array_check(const boost::multi_array<float,2>& value,
		     const boost::multi_array<float,2>& expect,
		     const FloatComparator &Cmp,
		     int line,
		     bool& error,
		     std::ostream& Out = std::cerr);

    int array_check(const boost::multi_array<double,2>& value,
		    const boost::multi_array<double,2>& expect,
		    const DoubleComparator &Cmp,
		    int line,
		    bool& error,
		    std::ostream& Out = std::cerr);

    void array_check(const boost::multi_array<float,3>& value,
		     const boost::multi_array<float,3>& expect,
		     const FloatComparator &Cmp,
		     int line,
		     bool& error,
		     std::ostream& Out = std::cerr);

    void array_check(const boost::multi_array<double,3>& value,
		     const boost::multi_array<double,3>& expect,
		     const DoubleComparator &Cmp,
		     int line,
		     bool& error,
		     std::ostream& Out = std::cerr);

    void array_check(const boost::multi_array<float,4>& value,
		     const boost::multi_array<float,4>& expect,
		     const FloatComparator &Cmp,
		     int line,
		     bool& error,
		     std::ostream& Out = std::cerr);

    void array_check(const boost::multi_array<double,4>& value,
		     const boost::multi_array<double,4>& expect,
		     const DoubleComparator &Cmp,
		     int line,
		     bool& error,
		     std::ostream& Out = std::cerr);

    template <class T> 
      int array_check(const boost::multi_array<T,2>& value,
		       const boost::multi_array<T,2>& expect,
		       int line,
		       bool& error,
		       std::ostream& Out = std::cerr);

    template <class T> 
      int array_check(const boost::multi_array<T,3>& value,
		       const boost::multi_array<T,3>& expect,
		       int line,
		       bool& error,
		       std::ostream& Out = std::cerr);

    void scalar_check_int(int value,
			  int expect,
			  int line,
			  bool& error,
			  std::ostream& Out = std::cerr);

    void scalar_check_double(double value,
			     double expect,
			     const DoubleComparator &Cmp,
			     int line,
			     bool& error,
			     std::ostream& Out = std::cerr);

    void scalar_check_float(float value,
			    float expect,
			    const FloatComparator &Cmp,
			    int line,
			    bool& error,
			    std::ostream& Out = std::cerr);
    
    void vector_check(const std::vector<float>& value,
		      const std::vector<float>& expect,
		      const FloatComparator &Cmp,
		      int line,
		      bool& error,
		      std::ostream& Out = std::cerr);

    void vector_check(const std::vector<double>& value,
		      const std::vector<double>& expect,
		      const DoubleComparator &Cmp,
		      int line,
		      bool& error,
		      std::ostream& Out = std::cerr);

    template <class T>
    void vector_check(const std::vector<T>& value,
		      const std::vector<T>& expect,
		      int line,
		      bool& error,
		      std::ostream& Out = std::cerr);
  
  }
}

/////////////////////////////////////////////////////////////
/// vector checker
/////////////////////////////////////////////////////////////

template <class T>
inline void 
MSPI::Shared::vector_check(const std::vector<T>& value,
			   const std::vector<T>& expect,
			   int line,
			   bool& error,
			   std::ostream& Out)
{
  if (value.size() != expect.size()) {
    Out << "value.size() = "<<value.size()
	 << " (expected " << expect.size()<<")\n";
    Out << "Unexpected result at line "<<line<<"\n";
    error = true;
    return;
  }
  
  for (size_t i = 0; i < value.size() ; i++) {
    if (value[i] != expect[i]) {
      Out << "value["<<i<<"] = "
	   << value[i]
	   << " (expected "<<expect[i]<<")\n";
      Out << "Unexpected result at line "<<line<<"\n";
      error = true;
    }
  }
}

/////////////////////////////////////////////////////////////
/// 2-d array checker
/////////////////////////////////////////////////////////////

template <class T> 
inline int
MSPI::Shared::array_check(const boost::multi_array<T,2>& value,
			const boost::multi_array<T,2>& expect,
			int line,
			bool& error,
			std::ostream& Out) 
{
  int error_count = 0;
  if (value.shape()[0] != expect.shape()[0] ||
      value.shape()[1] != expect.shape()[1]) {
    Out << "value.shape()[0] = "<<value.shape()[0]
	 << " (expected " << expect.shape()[0]<<")\n";
    Out << "value.shape()[1] = "<<value.shape()[1]
	 << " (expected " << expect.shape()[1]<<")\n";
    Out << "Unexpected result at line "<<line<<"\n";
    error = true;
    error_count++;
    return error_count;
  }

  for (size_t i = 0; i < value.shape()[0] ; i++) {
    for (size_t j = 0; j < value.shape()[1] ; j++) {
      if (value[i][j] != expect[i][j]) {
	Out << "value["<<i<<"]["<<j<<"] = "
	     << value[i][j]
	     << " (expected "<<expect[i][j]<<")\n";
	Out << "Unexpected result at line "<<line<<"\n";
	error = true;
	error_count++;
      }
    }
  }
  return error_count;
}

/////////////////////////////////////////////////////////////
/// 3-d array checker
/////////////////////////////////////////////////////////////

template <class T> 
inline int
MSPI::Shared::array_check(const boost::multi_array<T,3>& value,
			const boost::multi_array<T,3>& expect,
			int line,
			bool& error,
			std::ostream& Out) 
{
  int error_count = 0;
  if (value.shape()[0] != expect.shape()[0] ||
      value.shape()[1] != expect.shape()[1] ||
      value.shape()[2] != expect.shape()[2]) {
    Out << "value.shape()[0] = "<<value.shape()[0]
	 << " (expected " << expect.shape()[0]<<")\n";
    Out << "value.shape()[1] = "<<value.shape()[1]
	 << " (expected " << expect.shape()[1]<<")\n";
    Out << "value.shape()[2] = "<<value.shape()[2]
	 << " (expected " << expect.shape()[2]<<")\n";
    Out << "Unexpected result at line "<<line<<"\n";
    error = true;
    error_count++;
    return error_count;
  }

  for (size_t i = 0; i < value.shape()[0] ; i++) {
    for (size_t j = 0; j < value.shape()[1] ; j++) {
      for (size_t k = 0; k < value.shape()[2] ; k++) {
	if (value[i][j][k] != expect[i][j][k]) {
	  Out << "value["<<i<<"]["<<j<<"]["<<k<<"] = "
	      << value[i][j][k]
	      << " (expected "<<expect[i][j][k]<<")\n";
	  Out << "Unexpected result at line "<<line<<"\n";
	  error = true;
	  error_count++;
	}
      }
    }
  }
  return error_count;
}

#endif
