// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "pos_export_orbit.h"
%}
%base_import(orbit_quaternion_list)
%import "aircraft_orbit_data.i"
%geocal_shared_ptr(GeoCal::PosExportOrbit);
namespace GeoCal {
class PosExportOrbit : public OrbitQuaternionList {
public:
  PosExportOrbit(const std::string& Fname, const Time& Epoch);
  const AircraftOrbitData& aircraft_orbit_data(const Time& T) const;
  %python_attribute(file_name, std::string)
  %python_attribute(file_epoch, Time)
  %pickle_init(1, self.file_name, self.file_epoch)
};
}
