#ifndef MSPI_PARAXIAL_TRANSFORM_H
#define MSPI_PARAXIAL_TRANSFORM_H
#include "printable.h"
#include "auto_derivative.h"
#include <blitz/array.h>
#include <map>

namespace GeoCal {
/****************************************************************//**
  This handles the non-linearity of MSPI camera optics. This goes to
  and from real frame camera coordinate (i.e. x and y of the CCD) and the
  location we'd get for a pinhole camera with no non-linearity  (the
  paraxial approximation).

  This transformation is a polynomial expansion going to or from the
  paraxial coordinate system. It is described in detail in [1]. 

  References:
  [1] Veljko Jovanovic, "Algorithm Theoretical Basis and
  processing software design consideration for GroundMSPI L1B2
  process", Rev A.1, April 4, 2012.
*******************************************************************/
class MspiParaxialTransform: public Printable<MspiParaxialTransform> {
public:
  MspiParaxialTransform(const std::string& File_name);
  virtual ~MspiParaxialTransform() {}
  void paraxial_to_real(int Row_number, double Paraxial_x,
			double Paraxial_y, double& Real_x, 
			double& Real_y) const;
  void paraxial_to_real(int Row_number, const AutoDerivative<double>& 
			Paraxial_x,
			const AutoDerivative<double>& Paraxial_y, 
			AutoDerivative<double>& Real_x, 
			AutoDerivative<double>& Real_y) const;
  void real_to_paraxial(int Row_number,
			double Real_x,
			double Real_y,
			double& Paraxial_x,
			double& Paraxial_y) const;
  void real_to_paraxial(int Row_number,
			const AutoDerivative<double>& Real_x,
			const AutoDerivative<double>& Real_y,
			AutoDerivative<double>& Paraxial_x,
			AutoDerivative<double>& Paraxial_y) const;
  
//-----------------------------------------------------------------------
/// File name for MspiConfigFile.
//-----------------------------------------------------------------------

  const std::string& file_name() const {return fname;}
  void file_name(const std::string& Fname) const {fname = Fname;}

//-----------------------------------------------------------------------
/// True if we have the given row.
//-----------------------------------------------------------------------
  bool has_row(int Row_number) const
  { return row_to_index.count(Row_number) > 0; }
  virtual void print(std::ostream& Os) const;
private:
  std::string fname;
  std::map<int, int> row_to_index;
  blitz::Array<double, 2> a, b, c, d;
  MspiParaxialTransform() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(MspiParaxialTransform);
#endif
