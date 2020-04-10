// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "orbit_des.h"
%}
%base_import(generic_object)
%base_import(orbit)
%geocal_shared_ptr(GeoCal::PosCsephb);
%geocal_shared_ptr(GeoCal::AttCsattb);
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
  enum PositionType { SAME_AS_ORBIT = 0, CARTESIAN_FIXED = 1,
		      CARTESIAN_INERTIAL = 2 };
  enum EphemerisSource { PREDICTED = 0, ACTUAL = 1, REFINED = 2 };
  PosCsephb(const blitz::Array<double, 2>& Pos, const Time& Tmin, double Tstep,
	    bool Is_cf,
	    InterpolationType Itype = LINEAR,
	    LagrangeOrder Lagrange_order = NO_LAGRANGE,
	    EphemerisDataQuality E_quality = EPHEMERIS_QUALITY_GOOD,
	    EphemerisSource E_source = ACTUAL);
  PosCsephb(const Orbit& Orb, double Tstep, InterpolationType Itype = LINEAR,
	    LagrangeOrder Lagrange_order = NO_LAGRANGE,
	    EphemerisDataQuality E_quality = EPHEMERIS_QUALITY_GOOD,
	    EphemerisSource E_source = ACTUAL,
	    PositionType P_type = SAME_AS_ORBIT);
  PosCsephb(const Orbit& Orb, const Time& Min_time, const Time& Max_time,
	    double Tstep,
	    InterpolationType Itype = LINEAR,
	    LagrangeOrder Lagrange_order = NO_LAGRANGE,
	    EphemerisDataQuality E_quality = EPHEMERIS_QUALITY_GOOD,
	    EphemerisSource E_source = ACTUAL,
	    PositionType P_type = SAME_AS_ORBIT);
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
  %python_attribute_with_set(id, std::string);
  %python_attribute(position_data, blitz::Array<double, 2>);
  void min_time_split(std::string& OUTPUT, std::string& OUTPUT) const;
  // Synonyms that map to the pynitf names 
%pythoncode {
@property
def qual_flag_eph(self):
    return self.ephemeris_data_quality

@property
def interp_type_eph(self):
    return self.interpolation_type

@property
def interp_order_eph(self):
    return self.lagrange_order

@property
def ephem_flag(self):  
    return self.ephemeris_source

@property
def eci_ecf_ephem(self):
    return (1 if self.is_cf else 0)

@property
def dt_ephem(self):
    return self.time_step

@property
def date_ephem(self):
    return int(self.min_time_split()[0])

@property
def t0_ephem(self):
    return float(self.min_time_split()[1])

@property
def reserved_len(self):
    return 0

@property
def num_ephem(self):
    return self.position_data.shape[0]

@property
def ephem_x(self):
    return self.position_data[:,0]

@property
def ephem_y(self):
    return self.position_data[:,1]
  
@property
def ephem_z(self):
    return self.position_data[:,2]
}  
  void des_write(std::ostream& Os) const;
  static boost::shared_ptr<PosCsephb> des_read(std::istream& In);
  std::string print_to_string() const;
  %pickle_serialization();
};

class AttCsattb : public GenericObject {
public:
  enum AttitudeDataQuality {ATTITUDE_QUALITY_SUSPECT = 0,
			     ATTITUDE_QUALITY_GOOD = 1};
  enum InterpolationType { NEAREST_NEIGHBOR = 0,
			   LINEAR = 1,
			   LAGRANGE = 2 };
  enum LagrangeOrder { NO_LAGRANGE = 0, LAGRANGE_1 = 1, LAGRANGE_3 = 3,
		       LAGRANGE_5 = 5,
		       LAGRANGE_7 = 7};
  enum AttitudeType { SAME_AS_ORBIT = 0, CARTESIAN_FIXED = 1,
		      CARTESIAN_INERTIAL = 2 };
  enum AttitudeSource { PREDICTED = 0, ACTUAL = 1, REFINED = 2 };
  AttCsattb(const blitz::Array<double, 2>& Att, const Time& Tmin, double Tstep,
	    bool Is_cf,
	    InterpolationType Itype = LINEAR,
	    LagrangeOrder Lagrange_order = NO_LAGRANGE,
	    AttitudeDataQuality A_quality = ATTITUDE_QUALITY_GOOD,
	    AttitudeSource A_source = ACTUAL);
  AttCsattb(const Orbit& Orb, double Tstep, InterpolationType Itype = LINEAR,
	    LagrangeOrder Lagrange_order = NO_LAGRANGE,
	    AttitudeDataQuality E_quality = ATTITUDE_QUALITY_GOOD,
	    AttitudeSource E_source = ACTUAL,
	    AttitudeType A_type = SAME_AS_ORBIT);
  AttCsattb(const Orbit& Orb, const Time& Min_time, const Time& Max_time,
	    double Tstep,
	    InterpolationType Itype = LINEAR,
	    LagrangeOrder Lagrange_order = NO_LAGRANGE,
	    AttitudeDataQuality E_quality = ATTITUDE_QUALITY_GOOD,
	    AttitudeSource E_source = ACTUAL,
	    AttitudeType A_type = SAME_AS_ORBIT);
  %python_attribute_with_set(id, std::string);
  %python_attribute(is_cf, bool);
  %python_attribute(min_time, Time);
  %python_attribute(max_time, Time);
  %python_attribute(time_step, double);
  %python_attribute_with_set(interpolation_type, InterpolationType);
  %python_attribute_with_set(attitude_data_quality, AttitudeDataQuality);
  %python_attribute_with_set(attitude_source, AttitudeSource);
  %python_attribute_with_set(lagrange_order, LagrangeOrder);
  %python_attribute(attitude_data, blitz::Array<double, 2>);
  boost::math::quaternion<double> att_q(const Time& T) const;
  boost::math::quaternion<AutoDerivative<double> > 
  att_q(const TimeWithDerivative& T) const;
  void min_time_split(std::string& OUTPUT, std::string& OUTPUT) const;
  // Synonyms that map to the pynitf names 
%pythoncode {
@property
def qual_flag_att(self):
    return self.attitude_data_quality

@property
def interp_type_att(self):
    return self.interpolation_type

@property
def interp_order_att(self):
    return self.lagrange_order

@property
def att_type(self):  
    return self.attitude_source

@property
def eci_ecf_att(self):
    return (1 if self.is_cf else 0)

@property
def dt_att(self):
    return self.time_step

@property
def date_att(self):
    return int(self.min_time_split()[0])

@property
def t0_att(self):
    return float(self.min_time_split()[1])

@property
def reserved_len(self):
    return 0

@property
def num_att(self):
    return self.attitude_data.shape[0]

@property
def q1(self):
    return self.attitude_data[:,0]

@property
def q2(self):
    return self.attitude_data[:,1]
  
@property
def q3(self):
    return self.attitude_data[:,2]
  
@property
def q4(self):
    return self.attitude_data[:,3]
  
}  
  void des_write(std::ostream& Os) const;
  static boost::shared_ptr<AttCsattb> des_read(std::istream& In);
  std::string print_to_string() const;
  %pickle_serialization();
};

class OrbitDes: public Orbit {
public:
  OrbitDes(const boost::shared_ptr<PosCsephb>& Pos,
	   const boost::shared_ptr<AttCsattb>& Att,
	   int Naif_code = Ecr::EARTH_NAIF_CODE);
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> orbit_data(const TimeWithDerivative& T) 
    const;
  %python_attribute(pos_csephb, boost::shared_ptr<PosCsephb>);
  %python_attribute(att_csattb, boost::shared_ptr<AttCsattb>);
  %python_attribute_with_set(naif_code, int);
  %pickle_serialization();
};
 
}
// List of things "import *" will include
%python_export("PosCsephb", "AttCsattb", "OrbitDes")
