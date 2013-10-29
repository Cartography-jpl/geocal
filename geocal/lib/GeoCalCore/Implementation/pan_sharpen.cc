#include "pan_sharpen.h"
#include "simple_dem.h"
#include "magnify_bilinear.h"
#include "smooth_image.h"
#include "sub_raster_image.h"
#include "rpc_image_ground_connection.h"
#include "map_info_image_ground_connection.h"
#include "sub_raster_image_multi_band.h"
#include "statistic.h"
#include <boost/progress.hpp>
using namespace GeoCal;
using namespace blitz;

//-----------------------------------------------------------------------
/// This actually calculates the Pansharpened data. We do all bands at once.
//-----------------------------------------------------------------------

void PanSharpen::calc(int Start_line, int Start_sample) const
{
  Range ra(Range::all());
  int number_line = data.cols();
  int number_sample = data.depth();
  Array<double, 3> ncs = mag->read_double(Start_line, Start_sample,
					 number_line, number_sample);
  Array<double, 2> pan_data = 
    pansub->read_double(Start_line, Start_sample,
			number_line, number_sample);
  Array<double, 2> pansmooth_data = 
    psmooth->read_double(Start_line, Start_sample,
			 number_line, number_sample);
  Array<double, 3> hcs = to_hcs(ncs);
  iadj(hcs(0,ra,ra), pan_data, pansmooth_data);
  to_ncs(hcs);
// Any Nan gets set to the fill value of 0. These Nan may naturally
// arise if the image pixels are 0 (which happens, and it ok), or just
// because of some odd condition. We allow this, and then set this to
// 0 if it occurs.
  data = where(blitz_isnan(data), 0.0, data);
}

//-----------------------------------------------------------------------
/// Go to HCS space, as described in the paper equations 3 - 6.
/// Note that this calculation can return Nan, we explicitly allow
/// this and will check for this later.
//-----------------------------------------------------------------------

Array<double, 3> PanSharpen::to_hcs(const Array<double, 3>& ncs) const 
{
  Range ra(Range::all());
  firstIndex i1; secondIndex i2; thirdIndex i3;
  Array<double, 3> res(ncs.shape());
  res(0, ra, ra) = sqrt(sum(sqr(ncs(i3, i1, i2)), i3));
  for(int i = 1; i < ncs.rows(); ++i) {
    Range r(i, toEnd);
    Array<double, 3> ncs_sub(ncs(r, ra, ra));
    res(i, ra, ra) = 
      atan(sqrt(sum(sqr(ncs_sub(i3, i1, i2)), i3)) / ncs(i - 1, ra, ra));
  }
  return res;
}

//-----------------------------------------------------------------------
/// Go to NCS space, as described in the paper equations 7 - 10.
/// Note that this calculation can return Nan, we explicitly allow
/// this and will check for this later.
//-----------------------------------------------------------------------

void PanSharpen::to_ncs(const Array<double, 3>& hcs) const
{
  Range ra(Range::all());
  Array<double, 2> t(data.cols(), data.depth());
  t = hcs(0, ra, ra);
  for(int i = 0; i < data.rows() - 1; ++i) {
    data(i, ra, ra) = t * cos(hcs(i + 1, ra, ra));
    t *= sin(hcs(i + 1, ra, ra));
  }
  data(data.rows() - 1, ra, ra) = t;
}

//-----------------------------------------------------------------------
/// Adjust to use for pan sharpening. This is equation 25 of the
/// paper
/// Note that this calculation can return Nan, we explicitly allow
/// this and will check for this later.
//-----------------------------------------------------------------------

void PanSharpen::iadj(Array<double, 2> i, const Array<double, 2>& pan_data,
		      const Array<double, 2>& pansmooth_data) const
{
  i *= sqrt(
      (sig0() / sig1() * (sqr(pan_data) - mu1() + sig1()) + mu0() - sig0()) /
      (sig0() / sig1() * (sqr(pansmooth_data) - mu1() + sig1()) + 
       mu0() - sig0()));
}

//-----------------------------------------------------------------------
/// Constructor. This takes in a pan image and a multi spectral
/// image. We subset to the area we can calculate PanSharpen over.
///
/// \param Pan The panchromatic image to use
/// \param Mul The multispectral images to use
/// \param Force_rpc Sometimes an image will have both map information
/// and an RPC. In this case, we use the map information by
/// preference, unless directed by Force_rpc being true.
/// \param Pan_overview Optional overview to use instead of Pan for
/// calculating the initial statistics.
/// \param Mul_overview Optional overview to use instead of Mul for
/// calculating the initial statistics.
/// \param Log_progress If true, write progress message to std::cout
/// as we work through the data.
//-----------------------------------------------------------------------

PanSharpen::PanSharpen(const boost::shared_ptr<RasterImage>& Pan,
		       const boost::shared_ptr<RasterImageMultiBand>& Mul,
		       bool Force_rpc,
		       bool Log_progress,
		       const boost::shared_ptr<RasterImage>& Pan_overview,
		       const boost::shared_ptr<RasterImageMultiBand>& Mul_overview)
{
  firstIndex i1; secondIndex i2; thirdIndex i3;
  boost::shared_ptr<ImageGroundConnection> pan_ig, ms_ig;
  boost::shared_ptr<Dem> d;
  bool remove_rpc;
  if(!Force_rpc &&
     Pan->has_map_info() &&
     Mul->raster_image(0).has_map_info()) {
    d.reset(new SimpleDem);
    pan_ig.reset(new MapInfoImageGroundConnection(Pan, d));
    ms_ig.reset(new MapInfoImageGroundConnection(Mul->raster_image_ptr(0), d));
    remove_rpc = true;
  } else {
    if(!Pan->has_rpc() ||
       !Mul->raster_image(0).has_rpc())
      throw Exception("We require Pan and Mul to have RPCs or map info");
    d.reset(new SimpleDem(Pan->rpc().height_offset));
    pan_ig.reset(new RpcImageGroundConnection(Pan->rpc(), d, Pan));
    ms_ig.reset
      (new RpcImageGroundConnection(Mul->raster_image(0).rpc(), d,
				    Mul->raster_image_ptr(0)));
    remove_rpc = false;
  }

//-----------------------------------------------------------------------
// Determine the magnification of Mul we need to do to get to the
// same resolution as Pan.
//-----------------------------------------------------------------------

  ImageCoordinate ic1 =
    pan_ig->image_coordinate(*ms_ig->ground_coordinate(ImageCoordinate(0, 0)));
  ImageCoordinate ic2 =
    pan_ig->image_coordinate(*ms_ig->ground_coordinate(ImageCoordinate(1, 0)));
  int magfactor = (int) floor(ic2.line - ic1.line + 0.5);

//-----------------------------------------------------------------------
// Set up the smoothed version of the pan band and the magnified
// version of the multispectral bands. This is at the same
// resolution, but the images aren't yet lined up at the corners (do
// that in the next step).
//-----------------------------------------------------------------------

  int nhs = 3;
  boost::shared_ptr<SmoothImage> psmooth_orig(new SmoothImage(Pan, nhs));

  // Sometimes we have an old RPC even after the data has been map
  // projected. Remove this if we are using map projected data.
  if(remove_rpc)
    psmooth_orig->remove_rpc();
  psmooth = psmooth_orig;
  boost::shared_ptr<ImageGroundConnection> 
    psmooth_ig(new OffsetImageGroundConnection(pan_ig, -nhs, -nhs));
  boost::shared_ptr<ImageGroundConnection>
    mag_ig(new MagnifyBilinearImageGroundConnection(ms_ig, magfactor));
  boost::shared_ptr<RasterImageMultiBandVariable> 
    mags(new RasterImageMultiBandVariable());
  for(int i = 0; i < Mul->number_band(); ++i)
    mags->add_raster_image(boost::shared_ptr<RasterImage>
			   (new MagnifyBilinear(Mul->raster_image_ptr(i), 
						magfactor)));

//-----------------------------------------------------------------------
// Determine overlap area. We do this in psmooth image, just to pick
// a common one.
//-----------------------------------------------------------------------

  ImageCoordinate ulc_psmooth(0,0);
  ImageCoordinate lrc_psmooth(psmooth->number_line(), 
			      psmooth->number_sample());
  ImageCoordinate ulc_mag =
    psmooth_ig->image_coordinate
    (*mag_ig->ground_coordinate(ImageCoordinate(0,0)));
  int nline = mags->raster_image(0).number_line();
  int nsamp = mags->raster_image(0).number_sample();
  ImageCoordinate lrc_mag =
    psmooth_ig->image_coordinate
    (*mag_ig->ground_coordinate(ImageCoordinate(nline,nsamp)));
  ImageCoordinate ulc(std::max(ulc_psmooth.line, ulc_mag.line),
		      std::max(ulc_psmooth.sample, ulc_mag.sample));
  ImageCoordinate lrc(std::min(lrc_psmooth.line, lrc_mag.line),
		      std::min(lrc_psmooth.sample, lrc_mag.sample));
  nline = lrc.line - ulc.line;
  nsamp = lrc.sample - ulc.sample;
  boost::shared_ptr<GroundCoordinate> ulc_g = 
    psmooth_ig->ground_coordinate(ulc);
  boost::shared_ptr<GroundCoordinate> lrc_g = 
    psmooth_ig->ground_coordinate(lrc);
  ImageCoordinate ulc_mul = ms_ig->image_coordinate(*ulc_g);
  ImageCoordinate lrc_mul = ms_ig->image_coordinate(*lrc_g);

//-----------------------------------------------------------------------
// Now subset all the data to the common area.
//-----------------------------------------------------------------------

  ulc_mag = mag_ig->image_coordinate(*ulc_g);
  int ulc_mag_lstart = (int) floor(ulc_mag.line + 0.5);
  int ulc_mag_sstart = (int) floor(ulc_mag.sample + 0.5);
  nline = std::min(nline, mags->raster_image(0).number_line() - ulc_mag_lstart);
  nsamp = std::min(nsamp, mags->raster_image(0).number_sample() - ulc_mag_sstart);
  psmooth.reset(new SubRasterImage(psmooth, (int) floor(ulc.line + 0.5),
	   (int) floor(ulc.sample + 0.5), nline, nsamp));
  mag.reset(new SubRasterImageMultiBand(mags, ulc_mag_lstart, ulc_mag_sstart, 
					nline, nsamp));
  ImageCoordinate ulc_pan = pan_ig->image_coordinate(*ulc_g);
  pansub.reset(new SubRasterImage(Pan, (int) floor(ulc_pan.line + 0.5),
	  (int) floor(ulc_pan.sample + 0.5), nline, nsamp));
  
  int mulstart_ln = std::max((int) floor(ulc_mul.line), 0);
  int mulstart_smp = std::max((int) floor(ulc_mul.sample), 0);
  int mulstart_nline = 
    std::min(Mul->raster_image(0).number_line() - mulstart_ln,
	     ((int) ceil(lrc_mul.line)) - ((int) floor(ulc_mul.line)));
  int mulstart_nsamp = 
    std::min(Mul->raster_image(0).number_sample() - mulstart_smp,
	     ((int) ceil(lrc_mul.sample)) - ((int) floor(ulc_mul.sample)));
  mulsub.reset(new SubRasterImageMultiBand(Mul, mulstart_ln, mulstart_smp, 
					   mulstart_nline, mulstart_nsamp));

//-----------------------------------------------------------------------
// Calculate statistics on PS^2 and I^2 as described in the paper.
// 
// If we can, we try to use overview for the Pan and Multispectral
// data. If we can't then we use all of the data.
//-----------------------------------------------------------------------

  RasterImageMultiBand* mul_stat = 0;
  if(Mul_overview.get()) {
    if(Log_progress)
      std::cout 
	<< "Calculating statistics on multispectral image using overview:\n";
    mul_stat = Mul_overview.get();
  } else {
    if(Log_progress)
      std::cout << "Calculating statistics on multispectral image:\n";
    mul_stat = mulsub.get();
  }
  boost::shared_ptr<boost::progress_display> disp;
  if(Log_progress)
    disp.reset(new boost::progress_display
	       (mul_stat->raster_image(0).number_line() * 
		mul_stat->raster_image(0).number_sample()));
  for(RasterImageTileIterator i(mul_stat->raster_image(0)); !i.end(); ++i) {
    Array<double, 3> tl = mul_stat->read_double(i.istart(), i.jstart(),
			     i.number_line(), i.number_sample());
    Array<double, 2> isq(i.number_line(), i.number_sample());
    isq = sum(sqr(tl(i3, i1, i2)), i3);
    for(int j = 0; j < i.number_line(); ++j)
      for(int k = 0; k < i.number_sample(); ++k) {
	isq_stat += isq(j, k);
	if(disp)
	  *disp += 1;
      }
  }
  RasterImage* pan_stat = 0;
  if(Pan_overview.get()) {
    if(Log_progress)
      std::cout << "Calculating statistics on pan image using overview:\n";
    pan_stat = Pan_overview.get();
  } else {
    if(Log_progress)
      std::cout << "Calculating statistics on pan image:\n";
    pan_stat = pansub.get();
  }
  if(Log_progress)
    disp.reset(new boost::progress_display
	       (pan_stat->number_line() * pan_stat->number_sample()));
  for(RasterImageTileIterator i(*pan_stat); !i.end(); ++i) {
    Array<double, 2> tl = pan_stat->read_double(i.istart(), i.jstart(),
			     i.number_line(), i.number_sample());
    for(int j = 0; j < i.number_line(); ++j)
      for(int k = 0; k < i.number_sample(); ++k) {
	psq_stat += tl(j, k) * tl(j, k);
	if(disp)
	  *disp += 1;
      }
  }

  initialize(*psmooth, mag->number_band());
}
