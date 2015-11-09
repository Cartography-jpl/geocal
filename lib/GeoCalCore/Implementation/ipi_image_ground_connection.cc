#include "ipi_image_ground_connection.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void IpiImageGroundConnection::serialize(Archive & ar, 
					 const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ImageGroundConnection);
  ar & GEOCAL_NVP_(ipi) 
    & GEOCAL_NVP(res)
    & GEOCAL_NVP(max_h);
  ipi_->camera_ptr()->add_observer(*this);
}

GEOCAL_IMPLEMENT(IpiImageGroundConnection);
#endif

// See base class for description
void IpiImageGroundConnection::footprint_resolution
(int Line, int Sample, 
 double &Line_resolution_meter, 
 double &Sample_resolution_meter) const
{
  Time t;
  FrameCoordinate f;
  ipi_->time_table().time(ImageCoordinate(Line, Sample), t, f);
  boost::shared_ptr<GroundCoordinate> gc = ipi_->orbit().orbit_data(t)->
    reference_surface_intersect_approximate(ipi_->camera(), f, ipi_->band());
  f.line += 1;
  boost::shared_ptr<GroundCoordinate> gc1 = ipi_->orbit().orbit_data(t)->
    reference_surface_intersect_approximate(ipi_->camera(), f, ipi_->band());
  f.line -= 1;
  f.sample += 1;
  boost::shared_ptr<GroundCoordinate> gc2 = ipi_->orbit().orbit_data(t)->
    reference_surface_intersect_approximate(ipi_->camera(), f, ipi_->band());
  Line_resolution_meter = distance(*gc, *gc1);
  Sample_resolution_meter = distance(*gc, *gc2);
}

// See base class for description
blitz::Array<double, 7> 
IpiImageGroundConnection::cf_look_vector_arr
(int ln_start, int smp_start, int nline, int nsamp,
 int nsubpixel_line, int nsubpixel_sample, 
 int nintegration_step) const
{
  CartesianFixedLookVector lv;
  blitz::Array<double, 7>  res(nline, nsamp, nsubpixel_line, 
			       nsubpixel_sample, nintegration_step, 2, 3);
  // Much of the time, cf_look_vector_arr is called repeatedly with
  // the same number of subpixels. We fill this in if we don't match.
  if(nsubpixel_line != (int) sc_look_vector_cache.shape()[1] ||
     nsubpixel_sample != (int) sc_look_vector_cache.shape()[2]) {
    sc_look_vector_cache.resize
      (boost::extents[ipi_->camera().number_sample(ipi_->band())]
       [nsubpixel_line][nsubpixel_sample]);
    for(int j = 0; j < (int) sc_look_vector_cache.shape()[0]; ++j)
      for(int i2 = 0; i2 < (int) sc_look_vector_cache.shape()[1]; ++i2)
	for(int j2 = 0; j2 < (int) sc_look_vector_cache.shape()[2]; ++j2) {
	  FrameCoordinate fc(((double) i2 / nsubpixel_line),
			     j + ((double) j2 / nsubpixel_sample));
	  sc_look_vector_cache[j][i2][j2] = 
	    ipi_->camera().sc_look_vector(fc, ipi_->band());
	}
  }
  for(int i = 0; i < nline; ++i) {
    Time t;
    FrameCoordinate f;
    ipi_->time_table().time(ImageCoordinate(i + ln_start, 0), t, f);
    std::vector<boost::shared_ptr<OrbitData> > od;
    std::vector<boost::shared_ptr<CartesianFixed> > pos;
    for(int k = 0; k < nintegration_step; ++k) {
      double tint = 0;
      if(k != 0)
	tint = ipi_->camera().integration_time(ipi_->band()) / 
	  (nintegration_step - 1) * k;
      od.push_back(ipi_->orbit().orbit_data(t + tint));
      pos.push_back(od[k]->position_cf());
    }
    for(int j = 0; j < nsamp; ++j) 
      for(int i2 = 0; i2 < nsubpixel_line; ++i2)
	for(int j2 = 0; j2 < nsubpixel_sample; ++j2) {
	  const ScLookVector& slv = 
	    sc_look_vector_cache[j + smp_start][i2][j2];
	  for(int k = 0; k < nintegration_step; ++k) {
	    lv = od[k]->cf_look_vector(slv);
	    for(int k2 = 0; k2 < 3; ++k2) {
	      res(i, j, i2, j2, k, 0, k2) = pos[k]->position[k2];
	      res(i, j, i2, j2, k, 1, k2) = lv.look_vector[k2];
	    }
	  }
	}
  }
  return res;
}

blitz::Array<double, 2> 
IpiImageGroundConnection::image_coordinate_jac_parm(const GroundCoordinate& Gc) const
{
  ImageCoordinateWithDerivative ic;
  bool success;
  ipi_->image_coordinate_with_derivative_extended(Gc, ic, success);
  if(!success)
    throw ImageGroundConnectionFailed();
  Array<double, 2> res(2, std::max(ic.line.number_variable(), 
				   ic.sample.number_variable()));
  if(!ic.line.is_constant())
    res(0, Range::all()) = ic.line.gradient();
  else
    res(0, Range::all()) = 0;
  if(!ic.sample.is_constant())
    res(1, Range::all()) = ic.sample.gradient();
  else
    res(1, Range::all()) = 0;
  return res; 
}

blitz::Array<double, 1> 
IpiImageGroundConnection::collinearity_residual
(const GroundCoordinate& Gc,
 const ImageCoordinate& Ic_actual) const
{
  // Instead of the difference in ImageCoordinates returned by the
  // Ipi, use the difference in the FrameCoordinate for the time
  // associated with Ic_actual. This is faster to calculate, and the
  // Jacobian is much better behaved.
  Time t;
  FrameCoordinate fc_actual;
  ipi_->time_table().time(Ic_actual, t, fc_actual);
  FrameCoordinate fc_predict = 
    ipi_->orbit().frame_coordinate(t, Gc, ipi_->camera(), ipi_->band());
  Array<double, 1> res(2);
  res(0) = fc_predict.line - fc_actual.line;
  res(1) = fc_predict.sample - fc_actual.sample;
  return res;
}

blitz::Array<double, 2> 
IpiImageGroundConnection::collinearity_residual_jacobian
(const GroundCoordinate& Gc,
 const ImageCoordinate& Ic_actual) const
{
  TimeWithDerivative t;
  FrameCoordinateWithDerivative fc_actual;
  ImageCoordinateWithDerivative ica(Ic_actual.line, Ic_actual.sample);
  ipi_->time_table().time_with_derivative(ica, t, fc_actual);
  FrameCoordinateWithDerivative fc_predict = 
    ipi_->orbit().frame_coordinate_with_derivative(t, Gc, ipi_->camera(),
						    ipi_->band());
  Array<double, 2> res(2, fc_predict.line.number_variable() + 3);
  res(0, Range(0, res.cols() - 4)) = 
    (fc_predict.line - fc_actual.line).gradient();
  res(1, Range(0, res.cols() - 4)) = 
    (fc_predict.sample - fc_actual.sample).gradient();

  // Part of jacobian for cf coordinates.
  boost::shared_ptr<OrbitData> od = ipi_->orbit().orbit_data(t.value());
  boost::shared_ptr<CartesianFixed> p1 = od->position_cf();
  boost::shared_ptr<CartesianFixed> p2 = Gc.convert_to_cf();
  CartesianFixedLookVectorWithDerivative lv;
  for(int i = 0; i < 3; ++i) {
    AutoDerivative<double> p(p2->position[i], i, 3);
    lv.look_vector[i] = p - p1->position[i];
  }
  ScLookVectorWithDerivative sl = od->sc_look_vector(lv);
  boost::shared_ptr<Camera> c = ipi_->camera_ptr();
  ArrayAd<double, 1> poriginal = c->parameter_with_derivative();
  c->parameter_with_derivative(c->parameter());
  FrameCoordinateWithDerivative fc_gc =
    ipi_->camera().frame_coordinate_with_derivative(sl, ipi_->band());
  c->parameter_with_derivative(poriginal);
  res(0, Range(res.cols() - 3, toEnd)) = fc_gc.line.gradient();
  res(1, Range(res.cols() - 3, toEnd)) = fc_gc.sample.gradient();
  return res;
}
