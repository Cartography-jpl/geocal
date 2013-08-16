/*===========================================================================
=                                                                           =
=                              ParaxialTransform                            =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                    MSPI
                             GeoCal Processing

             Copyright 2009, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

!CLASS NAME: ParaxialTransform

!ABSTRACT:

   This contains support routines for the class ParaxialTransform 

!PARENT(S):

!REVISION HISTORY:

See Subversion Log

=============================================================================
!END=======================================================================*/

#include "paraxial_transform.h"	// Definition of class.
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.
#include <utility> 		// Definition of std::pair
#include <sstream>              // Definition of std::ostringstream

using namespace GeoCal;

/////////////////////////////////////////////////////////////////////////////
/// Constructor.   Initialize class with the given parameters.
/////////////////////////////////////////////////////////////////////////////

ParaxialTransform::ParaxialTransform(
  std::map<int,std::vector<double> > A, ///< Real to paraxial transform coefficients for each row. (xf)
  std::map<int,std::vector<double> > B, ///< Real to paraxial transform coefficients for each row. (yf)
  std::map<int,std::vector<double> > C, ///< Paraxial to real transform coefficients for each row. (xf)
  std::map<int,std::vector<double> > D  ///< Paraxial to real transform coefficients for each row. (yf)
) 
  : a_(A),
    b_(B),
    c_(C),
    d_(D)
{
}

/////////////////////////////////////////////////////////////////////////////
/// Constructor.
/////////////////////////////////////////////////////////////////////////////

ParaxialTransform::ParaxialTransform(
  const MSPI::Shared::ConfigFile& Config
)
{

  //--------------------------------------------------------------------------
  // Initialize transform coefficients from config file.
  //--------------------------------------------------------------------------

  int number_row = Config.get<int>("paraxial_number_row");
  std::vector<int> row_numbers = Config.get<int>("paraxial_row_numbers",number_row);

  for (int irow = 0 ; irow < number_row ; irow++) {
    int row_number = row_numbers[irow];
    std::ostringstream row_number_string;
    row_number_string << row_number;

    a_[row_number] = Config.get<double>("a"+row_number_string.str(),ParaxialTransform::size_a);
    b_[row_number] = Config.get<double>("b"+row_number_string.str(),ParaxialTransform::size_b);
    c_[row_number] = Config.get<double>("c"+row_number_string.str(),ParaxialTransform::size_c);
    d_[row_number] = Config.get<double>("d"+row_number_string.str(),ParaxialTransform::size_d);
  }
}

/////////////////////////////////////////////////////////////////////////////
/// Translate paraxial focal plane coordinates to real focal plane coordinates,
/// for the given row number.  Units are millimeters.
/////////////////////////////////////////////////////////////////////////////

void
ParaxialTransform::paraxial_to_real(
  int Row_number,
  double Paraxial_x,
  double Paraxial_y,
  double& Real_x,
  double& Real_y
) const
{
  //-------------------------------------------------------------------------
  // Get coefficients.
  // Argument check: Row_number not supported.
  //-------------------------------------------------------------------------

  std::map<int,std::vector<double> >::const_iterator i;

  if ((i = c_.find(Row_number)) == c_.end()) {
    std::ostringstream message;
    message << "Row_number " << Row_number << " not supported by paraxial transform (C)";
    throw MSPI_EXCEPTIONm(message.str());
  }

  const std::vector<double>&c = i->second;

  if ((i = d_.find(Row_number)) == d_.end()) {
    std::ostringstream message;
    message << "Row_number " << Row_number << " not supported by paraxial transform (D)";
    throw MSPI_EXCEPTIONm(message.str());
  }

  const std::vector<double>&d = i->second;

  //-------------------------------------------------------------------------
  // Argument check: c.size()( != size_c
  //-------------------------------------------------------------------------
  
  if (c.size() != size_c) {
    throw MSPI_EXCEPTIONm("c.size() != size_c");
  }

  //-------------------------------------------------------------------------
  // Argument check: d.size() != size_d
  //-------------------------------------------------------------------------
  
  if (d.size() != size_d) {
    throw MSPI_EXCEPTIONm("d.size() != size_d");
  }

  //-------------------------------------------------------------------------
  // Transform paraxial focal plane coordinate to real focal plane coordinate.
  //-------------------------------------------------------------------------

  double& xf = Paraxial_x;
  double& yf = Paraxial_y;

  double xf2 = xf * xf;
  double xf3 = xf2 * xf;
  double xf4 = xf3 * xf;
  double xf5 = xf3 * xf2;
  double xf6 = xf5 * xf;
  double xf7 = xf5 * xf2;
  double yf2 = yf * yf;
  
  Real_x = (c[0] * xf + 
	    c[1] * xf * yf + 
	    c[2] * xf * yf2 + 
	    c[3] * xf3 + 
	    c[4] * xf3 * yf + 
	    c[5] * xf3 * yf2 + 
	    c[6] * xf5 + 
	    c[7] * xf5 * yf +
	    c[8] * xf5 * yf2 + 
	    c[9] * xf7 + 
	    c[10] * xf7 * yf + 
	    c[11] * xf7 * yf2);

  Real_y = (d[0] * 1 +
	    d[1] * yf +
	    d[2] * yf2 +
	    d[3] * xf2 +
	    d[4] * xf2 * yf +
	    d[5] * xf2 * yf2 +
	    d[6] * xf4 +
	    d[7] * xf4 * yf +
	    d[8] * xf4 * yf2 +
	    d[9] * xf6 +
	    d[10] * xf6 * yf +
	    d[11] * xf6 * yf2);
}

/////////////////////////////////////////////////////////////////////////////
/// Translate real focal plane coordinates to paraxial focal plane coordinates,
/// for the given row number.  Units are millimeters.
/////////////////////////////////////////////////////////////////////////////

void
ParaxialTransform::real_to_paraxial(
  int Row_number,
  double Real_x,
  double Real_y,
  double& Paraxial_x,
  double& Paraxial_y
) const
{
  //-------------------------------------------------------------------------
  // Get coefficients.
  // Argument check: Row_number not supported.
  //-------------------------------------------------------------------------

  std::map<int,std::vector<double> >::const_iterator i;

  if ((i = a_.find(Row_number)) == a_.end()) {
    std::ostringstream message;
    message << "Row_number " << Row_number << " not supported by paraxial transform (A)";
    throw MSPI_EXCEPTIONm(message.str());
  }

  const std::vector<double>&a = i->second;

  if ((i = b_.find(Row_number)) == b_.end()) {
    std::ostringstream message;
    message << "Row_number " << Row_number << " not supported by paraxial transform (B)";
    throw MSPI_EXCEPTIONm(message.str());
  }

  const std::vector<double>&b = i->second;

  //-------------------------------------------------------------------------
  // Argument check: a.size()( != size_a
  //-------------------------------------------------------------------------
  
  if (a.size() != size_a) {
    throw MSPI_EXCEPTIONm("a.size() != size_a");
  }

  //-------------------------------------------------------------------------
  // Argument check: b.size() != size_b
  //-------------------------------------------------------------------------
  
  if (b.size() != size_b) {
    throw MSPI_EXCEPTIONm("b.size() != size_b");
  }

  //-------------------------------------------------------------------------
  // Transform real focal plane coordinate to paraxial focal plane coordinate.
  //-------------------------------------------------------------------------

  double& xfp = Real_x;
  double& yfp = Real_y;

  double xfp2 = xfp * xfp;
  double xfp3 = xfp2 * xfp;
  double xfp4 = xfp3 * xfp;
  double xfp5 = xfp4 * xfp;
  double xfp6 = xfp5 * xfp;
  double xfp7 = xfp6 * xfp;

  Paraxial_x = (a[0] * xfp + 
		a[1] * xfp3 +
		a[2] * xfp5 +
		a[3] * xfp7);

  Paraxial_y = (b[0] +
		yfp +
		b[1] * xfp2 +
		b[2] * xfp4 +
		b[3] * xfp6);
}
