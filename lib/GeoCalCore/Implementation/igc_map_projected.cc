#include "igc_map_projected.h"
#include "raster_averaged.h"
#include "memory_raster_image.h"
#include "memory_multi_band.h"
#include "ostream_pad.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void IgcMapProjectedBase::save(Archive & ar, const unsigned int version) const
{
  // Nothing more to do
}

template<class Archive>
void IgcMapProjectedBase::load(Archive & ar, const unsigned int version)
{
  ic_line.resize(grid_spacing_, grid_spacing_);
  ic_sample.resize(grid_spacing_, grid_spacing_);
}

template<class Archive>
void IgcMapProjectedBase::serialize(Archive & ar, 
				    const unsigned int version)
{
  GEOCAL_GENERIC_BASE(IgcMapProjectedBase);
  ar & GEOCAL_NVP_(igc_original) 
    & GEOCAL_NVP_(igc)
    & GEOCAL_NVP_(avg_factor)
    & GEOCAL_NVP_(grid_spacing)
    & GEOCAL_NVP_(fill_value)
    & GEOCAL_NVP(mi)
    & GEOCAL_NVP_(read_into_memory);
  boost::serialization::split_member(ar, *this, version);
}

template<class Archive>
void IgcMapProjected::serialize(Archive & ar, 
				    const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CalcRaster)
    & BOOST_SERIALIZATION_BASE_OBJECT_NVP(IgcMapProjectedBase);
}

template<class Archive>
void IgcMapProjectedMultiBand::serialize(Archive & ar, 
				    const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CalcRasterMultiBand)
    & BOOST_SERIALIZATION_BASE_OBJECT_NVP(IgcMapProjectedBase);
}

GEOCAL_IMPLEMENT(IgcMapProjectedBase);
GEOCAL_IMPLEMENT(IgcMapProjected);
GEOCAL_IMPLEMENT(IgcMapProjectedMultiBand);
#endif

//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

IgcMapProjectedBase::IgcMapProjectedBase
(const MapInfo& Mi, 
 const boost::shared_ptr<ImageGroundConnection>& Igc,
 int Grid_spacing,
 int Avg_fact,
 bool Read_into_memory,
 double Fill_value)
: igc_original_(Igc), 
  avg_factor_(Avg_fact),
  grid_spacing_(Grid_spacing),
  fill_value_(Fill_value),
  mi(Mi),
  read_into_memory_(Read_into_memory),
  ic_line(grid_spacing_, grid_spacing_),
  ic_sample(grid_spacing_, grid_spacing_)
{
  range_min_check(grid_spacing_, 1);
  if(avg_factor_ < 0)
    avg_factor_ = (int) floor(Mi.resolution_meter() / 
			   Igc->resolution_meter(ImageCoordinate(Igc->number_line() / 2.0, Igc->number_sample() / 2.0)));
  if(avg_factor_ > 1)
    igc_.reset(new AveragedImageGroundConnection(Igc, avg_factor_, avg_factor_,
						 Read_into_memory));
  else {
    if(Read_into_memory) {
      igc_.reset(new ImageGroundConnectionCopy(Igc));
      if(Igc->image())
	igc_->image(boost::shared_ptr<RasterImage>
		    (new MemoryRasterImage(*Igc->image())));
      if(Igc->image_multi_band())
	igc_->image_multi_band(boost::shared_ptr<RasterImageMultiBand>
		    (new MemoryMultiBand(*Igc->image_multi_band())));
    } else
      igc_ = Igc;
  }
}

//-----------------------------------------------------------------------
/// This sets up ic_line and ic_sample to interpolate. This will cover
/// The given start line and start sample.
///
/// This might fail, if the underlying ImageGroundConnection can't
/// convert the needed points to image_coordinate. If so, we return
/// false. Otherwise we return true.
//-----------------------------------------------------------------------

bool IgcMapProjectedBase::interpolate_ic(int Start_line, int Start_sample) const
{
  int sl = Start_line - (Start_line % grid_spacing_);
  int ss = Start_sample - (Start_sample % grid_spacing_);
  int el = std::min(sl + grid_spacing_ - 1, mi.number_y_pixel() - 1);
  int es = std::min(ss + grid_spacing_ - 1, mi.number_x_pixel() - 1);
  ic_line.reindexSelf(TinyVector<int, 2>(sl, ss));
  ic_sample.reindexSelf(TinyVector<int, 2>(sl, ss));
  double line[2][2];
  double sample[2][2];
  boost::shared_ptr<GroundCoordinate> gc;
  gc = mi.ground_coordinate(ss, sl, igc_->dem());
  ImageCoordinate ic;
  bool status;
  igc_->image_coordinate_with_status(*gc, ic, status);
  if(!status)
    return false;
  line[0][0] = ic.line;
  sample[0][0] = ic.sample;
  gc = mi.ground_coordinate(es, sl, igc_->dem());
  igc_->image_coordinate_with_status(*gc, ic, status);
  if(!status)
    return false;
  line[0][1] = ic.line;
  sample[0][1] = ic.sample;
  gc = mi.ground_coordinate(ss, el, igc_->dem());
  igc_->image_coordinate_with_status(*gc, ic, status);
  if(!status)
    return false;
  line[1][0] = ic.line;
  sample[1][0] = ic.sample;
  gc = mi.ground_coordinate(es, el, igc_->dem());
  igc_->image_coordinate_with_status(*gc, ic, status);
  if(!status)
    return false;
  line[1][1] = ic.line;
  sample[1][1] = ic.sample;
  int nline = el - sl + 1;
  int nsamp = es - ss + 1;
  for(int i = 0; i < nline; ++i)
    for(int j = 0; j < nsamp; ++j) {
      double lfrac = (nline > 1 ? i / (nline - 1.0) : 0.0);
      double sfrac = (nsamp > 1 ? j / (nsamp - 1.0) : 0.0);
      ic_line(sl + i, ss + j) = 
	(1.0 - lfrac - sfrac + lfrac * sfrac) * line[0][0] +
	(sfrac - lfrac * sfrac) * line[0][1] +
	(lfrac - lfrac * sfrac) * line[1][0] +
	(lfrac * sfrac) * line[1][1];
      ic_sample(sl + i, ss + j) = 
	(1.0 - lfrac - sfrac + lfrac * sfrac) * sample[0][0] +
	(sfrac - lfrac * sfrac) * sample[0][1] +
	(lfrac - lfrac * sfrac) * sample[1][0] +
	(lfrac * sfrac) * sample[1][1];
    }
  return true;
}

//-----------------------------------------------------------------------
/// Constructor. We average the data either by the factor given as
/// Avg_fact, or by ratio of the Mapinfo resolution and the Igc
/// resolution. 
///
/// You can optionally pass a grid spacing to use. We calculate
/// image coordinates in the input exactly at the grid spacing, and
/// interpolate in betweeen. This is much faster than calculating
/// every point, and if the grid spacing is small compared to the Dem
/// and any nonlinearities then it gives results very close to the
/// full calculation.
//-----------------------------------------------------------------------

IgcMapProjected::IgcMapProjected
(const MapInfo& Mi, 
 const boost::shared_ptr<ImageGroundConnection>& Igc,
 int Grid_spacing,
 int Avg_fact,
 bool Read_into_memory,
 int Number_tile_line, int Number_tile_sample, 
 double Fill_value)
  : CalcRaster(Mi), 
    IgcMapProjectedBase(Mi, Igc, Grid_spacing, Avg_fact, Read_into_memory, 
			Fill_value)
{
  if(!Igc->image())
    throw Exception("No Image found in IgcMapProjected. Did you mean to use IgcMapProjectedMultiBand?");
  if(Number_tile_line < 0)
    number_tile_line_ = number_line();
  else
    number_tile_line_ = Number_tile_line;
  if(Number_tile_sample < 0)
    number_tile_sample_ = number_sample();
  else
    number_tile_sample_ = Number_tile_sample;
}

//-----------------------------------------------------------------------
/// Constructor. We average the data either by the factor given as
/// Avg_fact, or by ratio of the Mapinfo resolution and the Igc
/// resolution. 
///
/// You can optionally pass a grid spacing to use. We calculate
/// image coordinates in the input exactly at the grid spacing, and
/// interpolate in betweeen. This is much faster than calculating
/// every point, and if the grid spacing is small compared to the Dem
/// and any nonlinearities then it gives results very close to the
/// full calculation.
//-----------------------------------------------------------------------

IgcMapProjectedMultiBand::IgcMapProjectedMultiBand
(const MapInfo& Mi, 
 const boost::shared_ptr<ImageGroundConnection>& Igc,
 int Grid_spacing,
 int Avg_fact,
 bool Read_into_memory,
 int Number_tile_line, int Number_tile_sample, double Fill_value)
: IgcMapProjectedBase(Mi, Igc, Grid_spacing, Avg_fact, Read_into_memory, 
		      Fill_value)
{
  if(!Igc->image_multi_band())
    throw Exception("No mult-band image found in IgcMapProjectedMultiBand. Did you mean to use IgcMapProjected?");
  int ntl = Number_tile_line;
  int nts = Number_tile_sample;
  if(Number_tile_line < 0)
    ntl = Mi.number_y_pixel();
  if(Number_tile_sample < 0)
    nts = Mi.number_x_pixel();
  initialize(Mi, Igc->number_band(), ntl, nts);
}

// See base class for description
void IgcMapProjected::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "IgcMapProjected:\n"
     << "  Avg_factor:   " << avg_factor() << "\n"
     << "  Grid spacing: " << grid_spacing() << "\n"
     << "  Fill value:   " << fill_value() << "\n"
     << "  Map info:\n";
  opad << map_info() << "\n";
  opad.strict_sync();
  Os << "  Image ground connection:\n" ;
  opad << *igc_ << "\n";
  opad.strict_sync();
}

// See base class for description
void IgcMapProjectedMultiBand::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "IgcMapProjectedMultiBand:\n"
     << "  Avg_factor:   " << avg_factor() << "\n"
     << "  Grid spacing: " << grid_spacing() << "\n"
     << "  Fill value:   " << fill_value() << "\n"
     << "  Map info:\n";
  opad << raster_image(0).map_info() << "\n";
  opad.strict_sync();
  Os << "  Image ground connection:\n" ;
  opad << *igc_ << "\n";
  opad.strict_sync();
}

// See base class for description
void IgcMapProjected::calc(int Lstart, int Sstart) const
{
  if(grid_spacing_ == 1)
    calc_no_grid(Lstart, Sstart);
  else
    calc_grid(Lstart, Sstart);
}

// See base class for description
void IgcMapProjectedMultiBand::calc(int Lstart, int Sstart) const
{
  if(grid_spacing_ == 1)
    calc_no_grid(Lstart, Sstart);
  else
    calc_grid(Lstart, Sstart);
}


//-----------------------------------------------------------------------
/// Calculation of map projected data when we don't do any grid
/// interpolation.
//-----------------------------------------------------------------------

void IgcMapProjected::calc_no_grid(int Lstart, int Sstart) const
{
  ImageCoordinate ic;
  bool status;
  for(int i = 0; i < data.rows(); ++i)
    for(int j = 0; j < data.cols(); ++j) {
      boost::shared_ptr<GroundCoordinate> gc = 
	ground_coordinate(ImageCoordinate(Lstart + i, Sstart + j), 
			  igc_->dem());
      if(igc_->ground_mask()->mask(*gc))
	data(i, j) = fill_value_;
      else {
	igc_->image_coordinate_with_status(*gc, ic, status);
	if(!status || ic.line < 0 || ic.line >= igc_->number_line() - 1 ||
	   ic.sample < 0 || ic.sample >= igc_->number_sample() - 1 ||
	   igc_->image_mask()->mask_ic(ic))
	  data(i, j) =  fill_value_;	// Data outside of image, so fill_value.
	else
	  data(i, j) = igc_->image()->unchecked_interpolate(ic.line, ic.sample);
      }
    }
}

//-----------------------------------------------------------------------
/// Calculation of map projected data when we don't do any grid
/// interpolation.
//-----------------------------------------------------------------------

void IgcMapProjectedMultiBand::calc_no_grid(int Lstart, int Sstart) const
{
  ImageCoordinate ic;
  bool status;
  for(int i = 0; i < data.cols(); ++i)
    for(int j = 0; j < data.depth(); ++j) {
      boost::shared_ptr<GroundCoordinate> gc = 
	mi.ground_coordinate(Sstart + j, Lstart + i, igc_->dem());
      igc_->image_coordinate_with_status(*gc, ic, status);
      if(igc_->ground_mask()->mask(*gc))
	data(Range::all(), i, j) = fill_value_;
      else {
	if(!status || ic.line < 0 || ic.line >= igc_->number_line() - 1 ||
	   ic.sample < 0 || ic.sample >= igc_->number_sample() - 1 ||
	   igc_->image_mask()->mask_ic(ic))
	  data(Range::all(), i, j) =  fill_value_;	
	else
	  for(int k = 0; k < data.rows(); ++k)
	    data(k, i, j) = 
	      igc_->image_multi_band()->raster_image(k).
	      unchecked_interpolate(ic.line, ic.sample);
      }
    }
}

//-----------------------------------------------------------------------
/// Calculation when we do have grid interpolation.
//-----------------------------------------------------------------------

void IgcMapProjected::calc_grid(int Lstart, int Sstart) const
{
  for(int i = -(Lstart % grid_spacing_); i < data.rows(); i += grid_spacing_)
    for(int j = -(Sstart % grid_spacing_); j < data.cols(); 
	j += grid_spacing_) {
      if(interpolate_ic(Lstart + i, Sstart + j)) {
	for(int ii = std::max(i, 0); 
	    ii < std::min(i + grid_spacing_, data.rows()); ++ii)
	  for(int jj = std::max(j, 0); 
	      jj < std::min(j + grid_spacing_, data.cols()); ++jj) {
	    boost::shared_ptr<GroundCoordinate> gc = 
	      ground_coordinate(ImageCoordinate(Lstart + ii, Sstart + jj), 
				igc_->dem());
	    if(igc_->ground_mask()->mask(*gc)) {
	      data(ii, jj) =  fill_value_;
	    } else {
	      double ln = ic_line(ii + Lstart, jj + Sstart);
	      double smp = ic_sample(ii + Lstart, jj + Sstart);
	      if(ln < 0 || ln >= igc_->number_line() - 1 ||
		 smp < 0 || smp >= igc_->number_sample() - 1 ||
		 igc_->image_mask()->mask(ln, smp))
		data(ii, jj) =  fill_value_;
	      else
		data(ii, jj) = igc_->image()->unchecked_interpolate(ln, smp);
	    }
	  }
      } else {
	// Might not be able to interpolate, because one of the
	// corners fails (e.g., we are near the edge of the IPI). Fall
	// back to trying to do every pixel.
	ImageCoordinate ic;
	bool status;
	for(int ii = std::max(i, 0); 
	    ii < std::min(i + grid_spacing_, data.rows()); ++ii)
	  for(int jj = std::max(j, 0); 
	      jj < std::min(j + grid_spacing_, data.cols()); ++jj) {
	    boost::shared_ptr<GroundCoordinate> gc = 
	      mi.ground_coordinate(Sstart + jj, Lstart + ii, igc_->dem());
	    igc_->image_coordinate_with_status(*gc, ic, status);
	    if(igc_->ground_mask()->mask(*gc)) {
	      data(ii, jj) =  fill_value_;
	    } else {
	      if(!status || ic.line < 0 || ic.line >= igc_->number_line() - 1 ||
		 ic.sample < 0 || ic.sample >= igc_->number_sample() - 1 ||
		 igc_->image_mask()->mask_ic(ic))
		data(ii, jj) =  fill_value_;
	      else
		data(ii, jj) = igc_->image()->
		  unchecked_interpolate(ic.line, ic.sample);
	    }
	  }
      }
    }
}
//-----------------------------------------------------------------------
/// Calculation when we do have grid interpolation.
//-----------------------------------------------------------------------

void IgcMapProjectedMultiBand::calc_grid(int Lstart, int Sstart) const
{
  for(int i = -(Lstart % grid_spacing_); i < data.cols(); i += grid_spacing_)
    for(int j = -(Sstart % grid_spacing_); j < data.depth(); 
	j += grid_spacing_) {
      if(interpolate_ic(Lstart + i, Sstart + j)) {
	for(int ii = std::max(i, 0); 
	    ii < std::min(i + grid_spacing_, data.cols()); ++ii)
	  for(int jj = std::max(j, 0); 
	      jj < std::min(j + grid_spacing_, data.depth()); ++jj) {
	    boost::shared_ptr<GroundCoordinate> gc = 
	      mi.ground_coordinate(Sstart + jj, Lstart + ii, igc_->dem());
	    if(igc_->ground_mask()->mask(*gc)) {
	      data(Range::all(), ii, jj) =  fill_value_;
	    } else {
	      double ln = ic_line(ii + Lstart, jj + Sstart);
	      double smp = ic_sample(ii + Lstart, jj + Sstart);
	      if(ln < 0 || ln >= igc_->number_line() - 1 ||
		 smp < 0 || smp >= igc_->number_sample() - 1 ||
		 igc_->image_mask()->mask(ln, smp))
		data(Range::all(), ii, jj) =  fill_value_;
	      else
		for(int k = 0; k < data.rows(); ++k)
		  data(k, ii, jj) = 
		    igc_->image_multi_band()->raster_image(k).
		    unchecked_interpolate(ln, smp);
	    }
	  }
      } else {
	// Might not be able to interpolate, because one of the
	// corners fails (e.g., we are
	// near the edge of the IPI). Fall back to trying to do every
	// pixel. 
	ImageCoordinate ic;
	bool status;
	for(int ii = std::max(i, 0); 
	    ii < std::min(i + grid_spacing_, data.rows()); ++ii)
	  for(int jj = std::max(j, 0); 
	      jj < std::min(j + grid_spacing_, data.cols()); ++jj) {
	    boost::shared_ptr<GroundCoordinate> gc = 
	      mi.ground_coordinate(Sstart + jj, Lstart + ii, igc_->dem());
	    igc_->image_coordinate_with_status(*gc, ic, status);
	    if(igc_->ground_mask()->mask(*gc)) {
	      data(Range::all(), ii, jj) =  fill_value_;
	    } else {
	      if(!status || ic.line < 0 || ic.line >= igc_->number_line() - 1 ||
		 ic.sample < 0 || ic.sample >= igc_->number_sample() - 1 ||
		 igc_->image_mask()->mask_ic(ic))
		data(Range::all(), ii, jj) =  fill_value_;
	      else
		for(int k = 0; k < data.rows(); ++k)
		  data(k, ii, jj) = 
		    igc_->image_multi_band()->raster_image(k).
		    unchecked_interpolate(ic.line, ic.sample);
	    }
	  }
      }
    }
}

