// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "orbit_des.h"
%}
%base_import(generic_object)
%base_import(orbit)
%geocal_shared_ptr(GeoCal::PosCsephb);
%geocal_shared_ptr(GeoCal::OrbitDes);
namespace GeoCal {
class PosCsephb : public GenericObject {
public:
  enum EphemerisDataQuality {EPHEMERIS_QUALITY_SUSPECT = 0,
			     EPHEMERIS_QUALITY_GOOD = 1};
  enum InterpolationType { NEAREST_NEIGHBOR = 0,
			   LINEAR = 1,
			   LAGRANGE = 2 };
  enum LagrangeOrder { NO_LAGRANGE = 0, LAGRANGE_1 = 1, LAGRANGE_3 = 3,
		       LAGRANGE_5 = 5,
		       LAGRANGE_7 = 7};
  enum EphemerisSource { PREDICTED = 0, ACTUAL = 1, REFINED = 2 };
  PosCsephb(const Orbit& Orb, double Tstep, InterpolationType Itype = LINEAR,
	    LagrangeOrder Lagrange_order = NO_LAGRANGE,
	    EphemerisDataQuality E_quality = EPHEMERIS_QUALITY_GOOD,
	    EphemerisSource E_source = ACTUAL);
  PosCsephb(const Orbit& Orb, const Time& Min_time, const Time& Max_time,
	    double Tstep,
	    InterpolationType Itype = LINEAR,
	    LagrangeOrder Lagrange_order = NO_LAGRANGE,
	    EphemerisDataQuality E_quality = EPHEMERIS_QUALITY_GOOD,
	    EphemerisSource E_source = ACTUAL);
  blitz::Array<double, 1> pos_vel(const Time& T) const;
  blitz::Array<AutoDerivative<double>, 1>
    pos_vel(const TimeWithDerivative& T) const;
  %python_attribute(is_cf, bool);
  %python_attribute(min_time, Time);
  %python_attribute(max_time, Time);
  %python_attribute(time_step, double);
  %python_attribute_with_set(interpolation_type, InterpolationType);
  %python_attribute_with_set(ephemeris_data_quality, EphemerisDataQuality);
  %python_attribute_with_set(ephemeris_source, EphemerisSource);
  %python_attribute_with_set(lagrange_order, LagrangeOrder);
  void des_write(std::ostream& Os) const;
  static boost::shared_ptr<PosCsephb> des_read(std::istream& In);
  std::string print_to_string() const;
  %pickle_serialization();
};

class OrbitDes: public Orbit {
public:
  OrbitDes(const boost::shared_ptr<PosCsephb>& Pos);
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> orbit_data(const TimeWithDerivative& T) 
    const;
  %python_attribute(pos_csephb, boost::shared_ptr<PosCsephb>);
  %pickle_serialization();
};
 
}
// List of things "import *" will include
%python_export("PosCsephb", "OrbitDes")
