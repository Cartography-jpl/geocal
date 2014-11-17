#include "ipi_image_ground_connection.h"

using namespace GeoCal;

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
