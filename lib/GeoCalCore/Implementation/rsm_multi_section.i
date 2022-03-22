// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "rsm_multi_section.h"
#include "image_ground_connection.h"
#include "coordinate_converter.h"  
%}
%base_import(rsm_base)
%import "image_coordinate.i"
%import "image_ground_connection.i"
%import "coordinate_converter.i"

%geocal_shared_ptr(GeoCal::RsmMultiSection);

namespace GeoCal {
class RsmMultiSection : public RsmBase {
public:
  RsmMultiSection(int Nline, int Nsamp, int Nrow_section, int Ncol_section,
		  const RsmBase& Rsm_prototype, int Border=5);
  virtual void fit(const ImageGroundConnection& Igc,
		   const CoordinateConverter& Cconv,
		   double Min_height, double Max_height,
		   int Min_line, int Max_line, int Min_sample,
		   int Max_sample);
  %python_attribute(number_row_section, int);
  %python_attribute(number_col_section, int);
  %python_attribute(number_line_per_section, double);
  %python_attribute(number_sample_per_section, double);
  void fit_start(const ImageGroundConnection& Igc,
		 const CoordinateConverter& Cconv,
		 double Min_height, double Max_height);
  void fit_start(const blitz::Array<double, 2>& Data);
  const boost::shared_ptr<RsmBase>&
  fit_section(int i, int j, const ImageGroundConnection& Igc,
		   const CoordinateConverter& Cconv,
		   double Min_height, double Max_height);
  const boost::shared_ptr<RsmBase>&
  fit_section(int i, int j, const blitz::Array<double, 2>& Data);
  boost::shared_ptr<RsmBase> section(int i, int j) const;
  void section(int i, int j, const boost::shared_ptr<RsmBase>& V);
  const boost::shared_ptr<RsmBase>&
  section_ls(double Line, double Sample) const;
  const boost::shared_ptr<RsmBase>&
  section_xyz(double X, double Y, double Z) const;
  std::string tre_string() const;
  static boost::shared_ptr<RsmMultiSection>
  read_tre_string(const std::string& Tre_in);
  %pickle_serialization()
};
}
// List of things "import *" will include
%python_export("RsmMultiSection")
