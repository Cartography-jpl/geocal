/*===========================================================================
=                                                                           =
=                             ParaxialTransform                             =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                     MSPI
                                  ParaxialTransform

             Copyright 2009, California Institute of Technology.
                               ALL RIGHTS RESERVED.
                    U.S. Government Sponsorship acknowledged.

!C++=========================================================================

=============================================================================
!END=======================================================================*/

#ifndef PARAXIAL_TRANSFORM_H
#define PARAXIAL_TRANSFORM_H

#include "boost/multi_array.hpp"  // Definition of boost::multi_array
#include <map>			  // Definition of std::map
#include "MSPI-Shared/Config/src/config_file.h" 
				// Definition of MSPI::Shared::ConfigFile

namespace GeoCal {
   namespace Mspi {
      class ParaxialTransform;
   }
}

/////////////////////////////////////////////////////////////////////////////
/// @brief Implements paraxial-to-real and real-to-paraxial transform for 
/// MSPI camera model.
///
/////////////////////////////////////////////////////////////////////////////

class GeoCal::Mspi::ParaxialTransform {
public:
  ParaxialTransform(std::map<int,std::vector<double> > A, 
		    std::map<int,std::vector<double> > B,
		    std::map<int,std::vector<double> > C,
		    std::map<int,std::vector<double> > D);
  ParaxialTransform(const MSPI::Shared::ConfigFile& Config);
  void paraxial_to_real(int Row_number,
			double Paraxial_x,
			double Paraxial_y,
			double& Real_x,
			double& Real_y) const;

  void real_to_paraxial(int Row_number,
			double Real_x,
			double Real_y,
			double& Paraxial_x,
			double& Paraxial_y) const;

  static const size_t size_a = 4;
  static const size_t size_b = 4;
  static const size_t size_c = 12;
  static const size_t size_d = 12;

protected:

private:
  
  std::map<int,std::vector<double> > a_; ///< Real to paraxial transform coefficients for each row. (xf)
  std::map<int,std::vector<double> > b_; ///< Real to paraxial transform coefficients for each row. (yf)
  std::map<int,std::vector<double> > c_; ///< Paraxial to real transform coefficients for each row. (xf)
  std::map<int,std::vector<double> > d_; ///< Paraxial to real transform coefficients for each row. (yf)
};

//***************************************************************************
// Include source of inline functions.
//***************************************************************************

//#include "mspi_camera.icc"
#endif
