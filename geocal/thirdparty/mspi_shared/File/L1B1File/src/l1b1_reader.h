/*===========================================================================
=                                                                           =
=                             L1B1Reader                                    =
=                                                                           =
=============================================================================

                          Jet Propulsion Laboratory
                                                                        
            Copyright 2012, California Institute of Technology.         
                           ALL RIGHTS RESERVED.                         
                 U.S. Government Sponsorship acknowledged.              

=============================================================================
!END=======================================================================*/

#ifndef L1B1_READER_H
#define L1B1_READER_H

#include <vector> 		// Definition of std::vector
#include "MSPI-Shared/File/hdf4/L1B1File/src/l1b1_file.h"
				// Definition of HDF4::L1B1File
   // KLUDGE: HDF4 includes must precede HDF-EOS5 includes because
   // of duplicate definitions of HDstrcmp in HDF-EOS5 (HE5_HdfEosDef.h) 
   // and HDF4 (hdfi.h).  The HDF4 definition must be first because it has
   // no guard against redefinition.
#include "MSPI-Shared/File/L1B1File/src/l1b1_file.h"
				// Definition of L1B1File
#include "boost/shared_ptr.hpp" // Definition of boost::shared_ptr

namespace MSPI {
  namespace Shared {
    class L1B1Reader;
  }
}

/////////////////////////////////////////////////////////////////////////////
/// @brief Provides read-only access to L1B1 products that may be either 
/// HDF4 or HDF5 format.
/////////////////////////////////////////////////////////////////////////////

class MSPI::Shared::L1B1Reader {
public:
  L1B1Reader(const std::string& Filename);
  std::string epoch() const;

  size_t number_frame(int Row_number) const;
  
  size_t number_pixel() const;

  std::vector<int> row_numbers() const;

  std::vector<std::string> field_names(int Row_number) const;

  std::vector<double> read_time(int Row_number, int Start_frame, 
				int Number_frame) const;

  boost::multi_array<float, 2> read_data(int Row_number, const
					 std::string& Field_name, 
					 int Start_frame, 
					 int Number_frame) const; 

  float wavelength(int Row_number) const;

  float polarization_angle(int Row_number) const;

  std::string swath_name(int Row_number) const;
  const Meta& meta() const;
  std::string granule_id() const;

private:
  bool is_hdf4_;
  boost::shared_ptr<MSPI::Shared::L1B1File> h5_;
  boost::shared_ptr<MSPI::Shared::HDF4::L1B1File> h4_;
  Meta empty_meta_;
};

/////////////////////////////////////////////////////////////////////////////
/// Calls meta method of L1B1File class.
/////////////////////////////////////////////////////////////////////////////

inline const MSPI::Shared::Meta&
MSPI::Shared::L1B1Reader::meta() const
{
  if (is_hdf4_) {
    return empty_meta_;
  }
  return h5_->meta();
}

/////////////////////////////////////////////////////////////////////////////
/// Calls granule_id method of L1B1File class.
/////////////////////////////////////////////////////////////////////////////

inline std::string 
MSPI::Shared::L1B1Reader::granule_id() const
{
  if (is_hdf4_) {
    return h4_->granule_id();
  }
  return h5_->granule_id();
}

/////////////////////////////////////////////////////////////////////////////
/// Calls epoch method of L1B1File class.
/////////////////////////////////////////////////////////////////////////////

inline std::string 
MSPI::Shared::L1B1Reader::epoch() const
{
  if (is_hdf4_) {
    return h4_->epoch();
  }
  return h5_->epoch();
}

/////////////////////////////////////////////////////////////////////////////
/// Calls number_frame method of L1B1File class.
/////////////////////////////////////////////////////////////////////////////

inline size_t
MSPI::Shared::L1B1Reader::number_frame(int Row_number) const
{
  if (is_hdf4_) {
    return h4_->number_frame(Row_number);
  }
  return h5_->number_frame(Row_number);
}

/////////////////////////////////////////////////////////////////////////////
/// Calls number_pixel method of L1B1File class.
/////////////////////////////////////////////////////////////////////////////

inline size_t
MSPI::Shared::L1B1Reader::number_pixel() const
{
  if (is_hdf4_) {
    return h4_->number_pixel();
  }
  return h5_->number_pixel();
}

/////////////////////////////////////////////////////////////////////////////
/// Calls row_numbers method of L1B1File class.
/////////////////////////////////////////////////////////////////////////////

inline std::vector<int> 
MSPI::Shared::L1B1Reader::row_numbers() const
{
  if (is_hdf4_) {
    return h4_->row_numbers();
  }
  return h5_->row_numbers();
}

/////////////////////////////////////////////////////////////////////////////
/// Calls field_names method of L1B1File class.
/////////////////////////////////////////////////////////////////////////////

inline std::vector<std::string> 
MSPI::Shared::L1B1Reader::field_names(int Row_number) const
{
  if (is_hdf4_) {
    return h4_->field_names(Row_number);
  }
  return h5_->field_names(Row_number);
}

/////////////////////////////////////////////////////////////////////////////
/// Calls read_time method of L1B1File class.
/////////////////////////////////////////////////////////////////////////////

inline std::vector<double> 
MSPI::Shared::L1B1Reader::read_time(
  int Row_number, 
  int Start_frame, 
  int Number_frame
) const
{
  if (is_hdf4_) {
    return h4_->read_time(Row_number, Start_frame, Number_frame);
  }
  return h5_->read_time(Row_number, Start_frame, Number_frame);
}

/////////////////////////////////////////////////////////////////////////////
/// Calls read_data method of L1B1File class.
/////////////////////////////////////////////////////////////////////////////

inline boost::multi_array<float, 2> 
MSPI::Shared::L1B1Reader::read_data(
  int Row_number, 
  const std::string& Field_name, 
  int Start_frame, 
  int Number_frame
) const
{
  if (is_hdf4_) {
    return h4_->read_data(Row_number, Field_name, Start_frame, Number_frame);
  }
  return h5_->read_data(Row_number, Field_name, Start_frame, Number_frame);
}

/////////////////////////////////////////////////////////////////////////////
/// Calls wavelength method of L1B1File class.
/////////////////////////////////////////////////////////////////////////////

inline float
MSPI::Shared::L1B1Reader::wavelength(int Row_number) const
{
  if (is_hdf4_) {
    return h4_->wavelength(Row_number);
  }
  return h5_->wavelength(Row_number);
}

/////////////////////////////////////////////////////////////////////////////
/// Calls polarization_angle method of L1B1File class.
/////////////////////////////////////////////////////////////////////////////

inline float
MSPI::Shared::L1B1Reader::polarization_angle(int Row_number) const
{
  if (is_hdf4_) {
    return h4_->polarization_angle(Row_number);
  }
  return h5_->polarization_angle(Row_number);
}

/////////////////////////////////////////////////////////////////////////////
/// Calls swath_name method of L1B1File class.
/////////////////////////////////////////////////////////////////////////////

inline std::string
MSPI::Shared::L1B1Reader::swath_name(int Row_number) const
{
  if (is_hdf4_) {
    return h4_->swath_name(Row_number);
  }
  return h5_->swath_name(Row_number);
}

#endif
