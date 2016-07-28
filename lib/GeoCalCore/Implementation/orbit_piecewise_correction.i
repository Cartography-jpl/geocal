// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "orbit_piecewise_correction.h"
%}
%base_import(orbit_correction)
%base_import(with_parameter)
%import "piecewise_linear.i"
%geocal_shared_ptr(GeoCal::OrbitPiecewiseCorrection);
namespace GeoCal {
class OrbitPiecewiseCorrection: public OrbitCorrection {
public:
  OrbitPiecewiseCorrection(const boost::shared_ptr<Orbit> Orb_uncorr,
			   const boost::shared_ptr<PiecewiseLinear>& E_corr,
			   const boost::shared_ptr<PiecewiseLinear>& N_corr,
			   const boost::shared_ptr<PiecewiseLinear>& U_corr);
  %python_attribute(e_corr, boost::shared_ptr<PiecewiseLinear>);
  %python_attribute(n_corr, boost::shared_ptr<PiecewiseLinear>);
  %python_attribute(u_corr, boost::shared_ptr<PiecewiseLinear>);
  %pickle_serialization();
};
}

