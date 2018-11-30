#include "magnify_bilinear.h"
using namespace GeoCal;
using namespace blitz;

//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

MagnifyBilinear::MagnifyBilinear(const boost::shared_ptr<RasterImage>& Data,
				 int Magfactor, int Number_tile)
  : CalcRaster((Data->number_line() - 1) * Magfactor,
	       (Data->number_sample() - 1) * Magfactor,
	       Data->number_tile_line() * Magfactor,
	       Data->number_tile_sample() * Magfactor, Number_tile),
    raw_data(Data), magfactor(Magfactor)
{
  range_min_check(Magfactor, 1);
  if(Data->has_map_info()) {
    Array<double, 1> parm = Data->map_info().transform();
    parm(1) /= Magfactor;
    parm(2) /= Magfactor;
    parm(4) /= Magfactor;
    parm(5) /= Magfactor;
    Data->map_info().index_to_coordinate(-0.5 / magfactor, -0.5 / magfactor,
					 parm(0), parm(3));
    map_info_.reset(new MapInfo(Data->map_info().coordinate_converter_ptr(),
				parm,
				number_sample(), number_line()));
  }
  if(Data->has_rpc()) {
    rpc_.reset(new Rpc(Data->rpc()));
    rpc_->line_scale *= Magfactor;
    rpc_->sample_scale *= Magfactor;
    rpc_->line_offset = (rpc_->line_offset + 0.5) * Magfactor - 0.5;
    rpc_->sample_offset = (rpc_->sample_offset + 0.5) * Magfactor - 0.5;
  }
  if(Data->has_rsm())
    throw Exception("Don't support RSM yet");
}

// Stencil used to do bilinear interpolation at one value
struct BilinearIntepolate {
  static double ln, smp;
  template<typename T1,typename T2,typename T3,typename T4,typename T5,
	   typename T6,
	   typename T7,typename T8,typename T9,typename T10,typename T11>
  static inline void apply(T1& A, T2& B, T3, T4, T5, T6, T7, T8, T9, T10, T11)
  {
    A = B(0,0) * (1 - ln) * (1 - smp) + B(0, 1) * (1 - ln) * smp
      + B(1, 0) * ln * (1 - smp) + B(1, 1) * ln * smp;
  }
  static const bool hasExtent = false; 
};

double BilinearIntepolate::ln, BilinearIntepolate::smp;

void MagnifyBilinear::calc(int Lstart, int Sstart) const
{
  int data_lstart = (int) floor(Lstart / magfactor);
  int data_sstart = (int) floor(Sstart / magfactor);
  int nl = (int) ceil((Lstart + data.rows()) / magfactor)  - data_lstart + 1;
  int ns = (int) ceil((Sstart + data.cols()) / magfactor) - data_sstart + 1;
  if((data_lstart + nl) < raw_data->number_line())
    nl += 1;
  if((data_sstart + ns) < raw_data->number_sample())
    ns += 1;
  Array<double, 2> d = raw_data->read_double(data_lstart, data_sstart, nl, ns);
  Array<double, 2> res(d.rows() * magfactor, d.cols() * magfactor);
  for(int i = 0; i < magfactor; ++i) {
    BilinearIntepolate::ln = 1.0 / magfactor * i;
    Range r1(i, i + (d.rows() - 1) * magfactor, magfactor);
    for(int j = 0; j < magfactor; ++j) {
      BilinearIntepolate::smp = 1.0 / magfactor * j;
      Range r2(j, j + (d.cols() - 1) * magfactor, magfactor);
      Array<double, 2> ressub(res(r1, r2));
      applyStencil(BilinearIntepolate(), ressub, d);
    }
  }
  int res_lstart = Lstart - data_lstart * magfactor;
  int res_sstart = Sstart - data_sstart * magfactor;
  Range r1(res_lstart, res_lstart + data.rows() - 1);
  Range r2(res_sstart, res_sstart + data.cols() - 1);
  data = res(r1, r2);
}



