#include "ipi_image_ground_connection.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void IpiImageGroundConnection::serialize(Archive & ar, 
					 const unsigned int version)
{
  // May change this to ipi base class, we'll see
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ImageGroundConnection);
  ar & GEOCAL_NVP_(ipi) 
    & GEOCAL_NVP(res)
    & GEOCAL_NVP(max_h);
  // Temp until we get ipi working
  std::cerr << "Skipping Ipi observer setup for now";
  //ipi_->camera_ptr()->add_observer(*this);
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
