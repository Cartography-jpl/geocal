// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "geocal_datum.h"
#include "datum_geoid96.h"
#include "did_datum.h"
#include "gdal_datum.h"
%}

%geocal_shared_ptr(Datum);
%geocal_shared_ptr(SimpleDatum);

%shared_ptr_dynamic_list(GeoCal::Datum,
			 GeoCal::SimpleDatum,
			 GeoCal::DatumGeoid96,
			 GeoCal::DidDatum,
			 GeoCal::GdalDatum);
namespace GeoCal {
class Datum {
public:
  virtual double undulation(const GroundCoordinate& Gc) const = 0;
  std::string print_to_string() const;
};

class SimpleDatum : public Datum {
public:
  SimpleDatum(double U = 0.0);
  %python_attribute(u, double)
  virtual double undulation(const GroundCoordinate& Gc) const;
  %pickle_init(self.u)
};
}
