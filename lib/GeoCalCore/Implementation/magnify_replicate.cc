#include "magnify_replicate.h"
using namespace GeoCal;
using namespace blitz;

//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

MagnifyReplicate::MagnifyReplicate(const boost::shared_ptr<RasterImage>& Data,
				   int Magfactor, int Number_tile)
  : CalcRaster((Data->number_line() - 1) * Magfactor,
	       (Data->number_sample() - 1) * Magfactor,
	       Data->number_tile_line() * Magfactor,
	       Data->number_tile_sample() * Magfactor, Number_tile),
    raw_data(Data), magfactor_(Magfactor)
{
  range_min_check(Magfactor, 1);
  if(Data->has_map_info()) {
    Array<double, 1> parm = Data->map_info().transform();
    parm(1) /= Magfactor;
    parm(2) /= Magfactor;
    parm(4) /= Magfactor;
    parm(5) /= Magfactor;
    Data->map_info().index_to_coordinate(-0.5 / magfactor_, -0.5 / magfactor_,
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
}

void MagnifyReplicate::calc(int Lstart, int Sstart) const
{
  int data_lstart = (int) floor(Lstart / magfactor_);
  int data_sstart = (int) floor(Sstart / magfactor_);
  int nl = (int) ceil(data.rows() / magfactor_) + 1;
  int ns = (int) ceil(data.cols() / magfactor_) + 1;
  if((data_lstart + nl) < raw_data->number_line())
    nl += 1;
  if((data_sstart + ns) < raw_data->number_sample())
    ns += 1;
  Array<double, 2> d = raw_data->read_double(data_lstart, data_sstart, nl, ns);
  Array<double, 2> res(d.rows() * magfactor_, d.cols() * magfactor_);
  for(int i = 0; i < magfactor_; ++i) {
    Range r1(i, i + (d.rows() - 1) * magfactor_, magfactor_);
    for(int j = 0; j < magfactor_; ++j) {
      Range r2(j, j + (d.cols() - 1) * magfactor_, magfactor_);
      res(r1, r2) = d;
    }
  }
  int res_lstart = Lstart - data_lstart * magfactor_;
  int res_sstart = Sstart - data_sstart * magfactor_;
  Range r1(res_lstart, res_lstart + data.rows() - 1);
  Range r2(res_sstart, res_sstart + data.cols() - 1);
  data = res(r1, r2);
}



