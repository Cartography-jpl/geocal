// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "pos_export_orbit.h"
%}

%geocal_shared_ptr(PosExportOrbit);
namespace GeoCal {
class PosExportOrbit : public OrbitQuaternionList {
public:
  PosExportOrbit(const std::string& Fname, const Time& Epoch);
  const AircraftOrbitData& aircraft_orbit_data(const Time& T) const;
  %python_attribute(file_name, std::string)
  %python_attribute(file_epoch, Time)
  %pickle_init(self.file_name, self.file_epoch)
};
}
