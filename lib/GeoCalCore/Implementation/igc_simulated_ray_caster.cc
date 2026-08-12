#include "igc_simulated_ray_caster.h"
#include "igc_ray_caster.h"
#include "geocal_serialize_support.h"
#include "ostream_pad.h"
using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void IgcSimulatedRayCaster::serialize(Archive & ar, 
				      const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CalcRaster);
  ar & GEOCAL_NVP_(igc) 
    & GEOCAL_NVP_(number_integration_step)
    & GEOCAL_NVP_(fill_value)
    & GEOCAL_NVP_(raycast_resolution)
    & GEOCAL_NVP_(max_height)
    & GEOCAL_NVP_(img);
}

GEOCAL_IMPLEMENT(IgcSimulatedRayCaster);
#endif

//-----------------------------------------------------------------------
/// Constructor.
/// If the Raycast_resolution is passed as a negative number, then we
/// use the resolution of the Map_projected_image.
//-----------------------------------------------------------------------

IgcSimulatedRayCaster::IgcSimulatedRayCaster(
const boost::shared_ptr<ImageGroundConnection>& Igc,
const boost::shared_ptr<RasterImage>& Map_projected_image, 
int Number_integration_step,
double Raycast_resolution,
double Max_height,
int Number_tile_line,
int Number_tile_sample,
double Fill_value
)
: CalcRaster(Igc->number_line(), Igc->number_sample()),
  igc_(Igc),
  number_integration_step_(Number_integration_step),
  fill_value_(Fill_value),
  raycast_resolution_(Raycast_resolution),
  max_height_(Max_height),
  img_(Map_projected_image)
{
  if(raycast_resolution_ < 0)
    raycast_resolution_ = img_->map_info().resolution_meter();
  if(Number_tile_line < 0)
    number_tile_line_ = number_line();
  else
    number_tile_line_ = Number_tile_line;
  if(Number_tile_sample < 0)
    number_tile_sample_ = number_sample();
  else
    number_tile_sample_ = Number_tile_sample;
}

// See base class for description
void IgcSimulatedRayCaster::calc(int Lstart, int Sstart) const
{
  IgcRayCaster rcast(igc_, 1, Lstart, data.rows(), number_integration_step_,
		     raycast_resolution_, max_height_, Sstart, data.cols());
  while(!rcast.last_position()) {
    blitz::Array<double, 2> r = rcast.next_radiance(*img_, fill_value_);
    blitz::Array<double, 1> 
      t(data(rcast.current_position() - rcast.start_position(), Range::all()));
    t = r(0,Range::all());
  }
}

// See base class for description
void IgcSimulatedRayCaster::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "IgcSimulatedRayCaster:\n"
     << "  Number integration step: " << number_integration_step() << "\n"
     << "  Raycast resolution:      " << raycast_resolution() << "\n"
     << "  Max height:              " << max_height() << "\n"
     << "  Fill value:              " << fill_value() << "\n";
  Os << "  Image ground connection:\n" ;
  opad << *igc_ << "\n";
  opad.strict_sync();
  Os << "  Map projected image:\n" ;
  opad << *img_ << "\n";
  opad.strict_sync();
}
