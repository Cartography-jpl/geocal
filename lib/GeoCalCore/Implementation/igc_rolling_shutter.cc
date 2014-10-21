#include "igc_rolling_shutter.h"
using namespace GeoCal;

//-----------------------------------------------------------------------
/// Initialize object.
//-----------------------------------------------------------------------

void IgcRollingShutter::init
(const boost::shared_ptr<Orbit>& Orb,
 const boost::shared_ptr<TimeTable>& Time_table,
 const boost::shared_ptr<Camera>& Cam, 
 const boost::shared_ptr<Dem>& D,
 const boost::shared_ptr<RasterImage>& Img,
 RollDirection Roll_direction,
 const std::string Title,
 const boost::shared_ptr<Refraction>& Ref,
 double Resolution, 
 int Band, 
 double Max_height
)
{
}

// See base class for description

void IgcRollingShutter::cf_look_vector
(const ImageCoordinate& Ic, CartesianFixedLookVector& Lv,
 boost::shared_ptr<CartesianFixed>& P) const
{
}

// See base class for description

boost::shared_ptr<GroundCoordinate> IgcRollingShutter::ground_coordinate_dem
(const ImageCoordinate& Ic, const Dem& D) const
{
}

// See base class for description

boost::shared_ptr<GroundCoordinate> 
IgcRollingShutter::ground_coordinate_approx_height
(const ImageCoordinate& Ic, double H) const
{
}

// See base class for description

ImageCoordinate IgcRollingShutter::image_coordinate
(const GroundCoordinate& Gc) const
{
}

// See base class for description

blitz::Array<double, 2> IgcRollingShutter::image_coordinate_jac_parm
(const GroundCoordinate& Gc) const
{
}

// See base class for description

blitz::Array<double, 7> IgcRollingShutter::cf_look_vector_arr
(int ln_start, int smp_start, int nline, int nsamp, int nsubpixel_line, 
 int nsubpixel_sample, int nintegration_step) const
{
}

// See base class for description

void IgcRollingShutter::footprint_resolution
(int Line, int Sample, double &Line_resolution_meter, 
 double &Sample_resolution_meter)
{
}

// See base class for description
void IgcRollingShutter::print(std::ostream& Os) const
{
}

