/*===========================================================================
=                                                                           =
=                              Check                                        =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory

             Copyright 2012, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

=============================================================================
!END=======================================================================*/

#include "check.h"	// Function prototypes
#include <fstream>	// Definition of std::ifstream

/////////////////////////////////////////////////////////////
/// Check for differences between files.
/////////////////////////////////////////////////////////////

void 
MSPI::Shared::file_check(
  const std::string& A, 
  const std::string& B, 
  int Line,
  bool& Error,
  std::ostream& Out)
{
  //---------------------------------------------------------
  // Open files.
  //---------------------------------------------------------

  std::ifstream afs(A.c_str(),std::ios::binary);
  std::ifstream bfs(B.c_str(),std::ios::binary); 

  //---------------------------------------------------------
  // Check file lengths.
  //---------------------------------------------------------

  afs.seekg(0,std::ios::end);
  bfs.seekg(0,std::ios::end);
  int alen = afs.tellg();
  int blen = bfs.tellg();
  afs.seekg(0,std::ios::beg);
  bfs.seekg(0,std::ios::beg);

  if (alen < 0) {
    Out << "Trouble with file: " << A << ", alen = " << alen << "\n";
    Out << "Unexpected result at line " << Line << "\n";
    Error = true;
    return;
  }
  if (blen < 0) {
    Out << "Trouble with file: " << B << ", blen = " << blen << "\n";
    Out << "Unexpected result at line " << Line << "\n";
    Error = true;
    return;
  }
  if (alen != blen) {
    Out << "Files are different length: " << A << ", alen = " << alen 
	<< ", " << B << ",  blen = " << blen << "\n";
    Out << "Unexpected result at line " << Line << "\n";
    Error = true;
    return;
  }

  //---------------------------------------------------------
  // Read files.
  //---------------------------------------------------------

  std::vector<char> abuf(alen,0);
  std::vector<char> bbuf(blen,0);
  afs.read(&(abuf[0]), alen);
  bfs.read(&(bbuf[0]), blen);
  if (afs.fail() || afs.eof()) {
    Out << "Trouble reading file: " << A << "\n";
    Out << "Unexpected result at line " << Line << "\n";
    Error = true;
    return;
  }
  if (bfs.fail() || bfs.eof()) {
    Out << "Trouble reading file: " << B << "\n";
    Out << "Unexpected result at line " << Line << "\n";
    Error = true;
    return;
  }

  //---------------------------------------------------------
  // Compare file contents.
  //---------------------------------------------------------

  bool result = std::equal(abuf.begin(), abuf.end(), bbuf.begin());
  if (!result) {
    Out << "Files differ: " << A << " " << B << "\n";
    Out << "Unexpected result at line " << Line << "\n";
    Error = true;
    return;
  }
}

/////////////////////////////////////////////////////////////
/// 2-d float array checker
/////////////////////////////////////////////////////////////

void 
MSPI::Shared::array_check(const boost::multi_array<float,2>& value,
			const boost::multi_array<float,2>& expect,
			const FloatComparator &Cmp,
			int line,
			bool& error,
			std::ostream& Out) 
{
  if (value.shape()[0] != expect.shape()[0] ||
      value.shape()[1] != expect.shape()[1]) {
    Out << "value.shape()[0] = "<<value.shape()[0]
	 << " (expected " << expect.shape()[0]<<")\n";
    Out << "value.shape()[1] = "<<value.shape()[1]
	 << " (expected " << expect.shape()[1]<<")\n";
    Out << "Unexpected result at line "<<line<<"\n";
    error = true;
    return;
  }

  for (size_t i = 0; i < value.shape()[0] ; i++) {
    for (size_t j = 0; j < value.shape()[1] ; j++) {
      if (Cmp.neq(value[i][j],expect[i][j],Out)) {
	Out << "value["<<i<<"]["<<j<<"] = "
	     << value[i][j]
	     << " (expected "<<expect[i][j]<<")\n";
	Out << "Unexpected result at line "<<line<<"\n";
	error = true;
      }
    }
  }
}

/////////////////////////////////////////////////////////////
/// 2-d double array checker
/////////////////////////////////////////////////////////////

int
MSPI::Shared::array_check(const boost::multi_array<double,2>& value,
			const boost::multi_array<double,2>& expect,
			const DoubleComparator &Cmp,
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
      if (Cmp.neq(value[i][j],expect[i][j],Out)) {
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
/// 3-d float array checker
/////////////////////////////////////////////////////////////

void 
MSPI::Shared::array_check(const boost::multi_array<float,3>& value,
			const boost::multi_array<float,3>& expect,
			const FloatComparator &Cmp,
			int line,
			bool& error,
			std::ostream& Out) 
{
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
    return;
  }

  for (size_t i = 0; i < value.shape()[0] ; i++) {
    for (size_t j = 0; j < value.shape()[1] ; j++) {
      for (size_t k = 0; k < value.shape()[2] ; k++) {
	if (Cmp.neq(value[i][j][k],expect[i][j][k],Out)) {
	  Out << "value["<<i<<"]["<<j<<"]["<<k<<"] = "
	      << value[i][j][k]
	      << " (expected "<<expect[i][j][k]<<")\n";
	  Out << "Unexpected result at line "<<line<<"\n";
	  error = true;
	}
      }
    }
  }
}

/////////////////////////////////////////////////////////////
/// 3-d double array checker
/////////////////////////////////////////////////////////////

void 
MSPI::Shared::array_check(const boost::multi_array<double,3>& value,
			const boost::multi_array<double,3>& expect,
			const DoubleComparator &Cmp,
			int line,
			bool& error,
			std::ostream& Out) 
{
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
    return;
  }

  for (size_t i = 0; i < value.shape()[0] ; i++) {
    for (size_t j = 0; j < value.shape()[1] ; j++) {
      for (size_t k = 0; k < value.shape()[2] ; k++) {
	if (Cmp.neq(value[i][j][k],expect[i][j][k],Out)) {
	  Out << "value["<<i<<"]["<<j<<"]["<<k<<"] = "
	      << value[i][j][k]
	      << " (expected "<<expect[i][j][k]<<")\n";
	  Out << "Unexpected result at line "<<line<<"\n";
	  error = true;
	}
      }
    }
  }
}

/////////////////////////////////////////////////////////////
/// 4-d float array checker
/////////////////////////////////////////////////////////////

void 
MSPI::Shared::array_check(const boost::multi_array<float,4>& value,
			const boost::multi_array<float,4>& expect,
			const FloatComparator &Cmp,
			int line,
			bool& error,
			std::ostream& Out) 
{
  if (value.shape()[0] != expect.shape()[0] ||
      value.shape()[1] != expect.shape()[1] ||
      value.shape()[2] != expect.shape()[2] ||
      value.shape()[3] != expect.shape()[3]) {
    Out << "value.shape()[0] = "<<value.shape()[0]
	 << " (expected " << expect.shape()[0]<<")\n";
    Out << "value.shape()[1] = "<<value.shape()[1]
	 << " (expected " << expect.shape()[1]<<")\n";
    Out << "value.shape()[2] = "<<value.shape()[2]
	 << " (expected " << expect.shape()[2]<<")\n";
    Out << "value.shape()[3] = "<<value.shape()[3]
	 << " (expected " << expect.shape()[3]<<")\n";
    Out << "Unexpected result at line "<<line<<"\n";
    error = true;
    return;
  }

  for (size_t i = 0; i < value.shape()[0] ; i++) {
    for (size_t j = 0; j < value.shape()[1] ; j++) {
      for (size_t k = 0; k < value.shape()[2] ; k++) {
	for (size_t l = 0; l < value.shape()[3] ; l++) {
	  if (Cmp.neq(value[i][j][k][l],expect[i][j][k][l],Out)) {
	    Out << "value["<<i<<"]["<<j<<"]["<<k<<"]["<<l<<"] = "
		<< value[i][j][k][l]
		<< " (expected "<<expect[i][j][k][l]<<")\n";
	    Out << "Unexpected result at line "<<line<<"\n";
	    error = true;
	  }
	}
      }
    }
  }
}

/////////////////////////////////////////////////////////////
/// 4-d double array checker
/////////////////////////////////////////////////////////////

void 
MSPI::Shared::array_check(const boost::multi_array<double,4>& value,
			const boost::multi_array<double,4>& expect,
			const DoubleComparator &Cmp,
			int line,
			bool& error,
			std::ostream& Out) 
{
  if (value.shape()[0] != expect.shape()[0] ||
      value.shape()[1] != expect.shape()[1] ||
      value.shape()[2] != expect.shape()[2] ||
      value.shape()[3] != expect.shape()[3]) {
    Out << "value.shape()[0] = "<<value.shape()[0]
	 << " (expected " << expect.shape()[0]<<")\n";
    Out << "value.shape()[1] = "<<value.shape()[1]
	 << " (expected " << expect.shape()[1]<<")\n";
    Out << "value.shape()[2] = "<<value.shape()[2]
	 << " (expected " << expect.shape()[2]<<")\n";
    Out << "value.shape()[3] = "<<value.shape()[3]
	 << " (expected " << expect.shape()[3]<<")\n";
    Out << "Unexpected result at line "<<line<<"\n";
    error = true;
    return;
  }

  for (size_t i = 0; i < value.shape()[0] ; i++) {
    for (size_t j = 0; j < value.shape()[1] ; j++) {
      for (size_t k = 0; k < value.shape()[2] ; k++) {
	for (size_t l = 0; l < value.shape()[3] ; l++) {
	  if (Cmp.neq(value[i][j][k][l],expect[i][j][k][l],Out)) {
	    Out << "value["<<i<<"]["<<j<<"]["<<k<<"]["<<l<<"] = "
		<< value[i][j][k][l]
		<< " (expected "<<expect[i][j][k][l]<<")\n";
	    Out << "Unexpected result at line "<<line<<"\n";
	    error = true;
	  }
	}
      }
    }
  }
}

/////////////////////////////////////////////////////////////
/// scalar int checker
/////////////////////////////////////////////////////////////

void 
MSPI::Shared::scalar_check_int(int value,
			     int expect,
			     int line,
			     bool& error,
			     std::ostream& Out) 
{
  if (value != expect) {
    Out << "scalar_check_int: value = " << value << " (expected " << expect << ")\n";
    Out << "Unexpected result at line "<<line<<"\n";
    error = true;
  }
}

/////////////////////////////////////////////////////////////
/// scalar double checker
/////////////////////////////////////////////////////////////

void 
MSPI::Shared::scalar_check_double(double value,
				double expect,
				const DoubleComparator &Cmp,
				int line,
				bool& error,
				std::ostream& Out)
{
  if (Cmp.neq(value,expect,Out)) {
    Out << "scalar_check_double: value = " << value << " (expected " << expect << ")\n";
    Out << "Unexpected result at line "<<line<<"\n";
    error = true;
  }
}

/////////////////////////////////////////////////////////////
/// scalar float checker
/////////////////////////////////////////////////////////////

void 
MSPI::Shared::scalar_check_float(float value,
			       float expect,
			       const FloatComparator &Cmp,
			       int line,
			       bool& error,
			       std::ostream& Out)
{
  if (Cmp.neq(value,expect,Out)) {
    Out << "scalar_check_float: value = " << value << " (expected " << expect << ")\n";
    Out << "Unexpected result at line "<<line<<"\n";
    error = true;
  }
}

/////////////////////////////////////////////////////////////
/// vector float checker
/////////////////////////////////////////////////////////////

void 
MSPI::Shared::vector_check(const std::vector<float>& value,
			 const std::vector<float>& expect,
			 const FloatComparator &Cmp,
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
    if (Cmp.neq(value[i],expect[i],Out)) {
      Out << "value["<<i<<"] = "
	   << value[i]
	   << " (expected "<<expect[i]<<")\n";
      Out << "Unexpected result at line "<<line<<"\n";
      error = true;
    }
  }
}

/////////////////////////////////////////////////////////////
/// vector double checker
/////////////////////////////////////////////////////////////

void 
MSPI::Shared::vector_check(const std::vector<double>& value,
			 const std::vector<double>& expect,
			 const DoubleComparator &Cmp,
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
    if (Cmp.neq(value[i],expect[i],Out)) {
      Out << "value["<<i<<"] = "
	   << value[i]
	   << " (expected "<<expect[i]<<")\n";
      Out << "Unexpected result at line "<<line<<"\n";
      error = true;
    }
  }
}

