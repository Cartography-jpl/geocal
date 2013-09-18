#include "doughnut_average.h"

using namespace GeoCal;
using namespace blitz;

//-----------------------------------------------------------------------
/// Very closely related in the doughnut average is the cvdnorm for
/// one of the bands. So we provide the calculation for that.
//-----------------------------------------------------------------------

Array<double, 2> DoughnutAverage::cvdnorm(int band, int Lstart, int Sstart,
		  int Number_line, int Number_sample) const
{
  Range ra(Range::all());
  range_check(band, 0, number_band());
  Array<double, 3> davg = read_double(Lstart, Sstart, Number_line, 
				      Number_sample);
  if(number_band() != 2)
    throw Exception("Only have logic in place now for 2 cvdnorm items. We can extend this is needed");
  Array<double, 2> res(Number_line, Number_sample);
  res = input_data(band, Range(0,Number_line - 1), Range(0,Number_sample - 1));
  if(band == 1)
    res += davg(0, ra, ra) - davg(1, ra, ra);
  return res;
}

//-----------------------------------------------------------------------
/// This is the difference of the first two cvdnorm bands.
//-----------------------------------------------------------------------

blitz::Array<double, 2> DoughnutAverage::pandif(int Lstart, int Sstart,
			int Number_line, int Number_sample) const
{
  Range ra(Range::all());
  Array<double, 3> davg = read_double(Lstart, Sstart, Number_line, 
				      Number_sample);
  Array<double, 3> inpd(davg.shape());
  inpd = input_data(ra, Range(0,Number_line - 1), 
		    Range(0,Number_sample - 1));
  return Array<double, 2>
    (abs(inpd(0,ra,ra) - (inpd(1, ra, ra) + davg(0,ra,ra) - davg(1,ra,ra))));
}

//-----------------------------------------------------------------------
/// Constructor.
/// \param Rimg The underlying image we are producing the average for.
/// \param Window_size The window size we are averaging. Should be odd number.
/// \param Doughnut_size The doughnut we are excluding in the average.
///    Should be odd, and < Window_size.
/// \param Allow_gore If "Allow_gore" is true, then any point
///    other than the center can be gore and we just exclude it from the
///    average. If "Allow_gore" is false, we set a value to 0 if any value
///    in the window is 0. In all cases, we set a value to 0 if the center
//     is 0.
/// \param Number_tile_line Number of lines in each tile
/// \param Number_tile_sample Number of samples in each tile
/// \param Number_tile Number of tiles to use
//-----------------------------------------------------------------------

DoughnutAverage::DoughnutAverage
(const boost::shared_ptr<RasterImageMultiBand>& Rimg, 
 int Window_size, int Doughnut_size, bool Allow_gore, 
 int Number_tile_line,
 int Number_tile_sample, int Number_tile)
  : CalcRasterMultiBand(Rimg->raster_image(0), Rimg->number_band(), 
			Number_tile_line, Number_tile_sample, Number_tile),
    rimg(Rimg),
    whsize((Window_size - 1) / 2),
    dhsize((Doughnut_size - 1) / 2),
    allow_gore_(Allow_gore)
{
  range_min_check(Window_size, 0);
  range_min_check(Doughnut_size, 0);
  if(Window_size % 2 != 1)
    throw Exception("Window_size must be odd");
  if(Doughnut_size % 2 != 1)
    throw Exception("Doughnut_size must be odd");
  if(Doughnut_size >= Window_size)
    throw Exception("Doughnut_size must be < Window_size");
}

// See base class for description.
void
DoughnutAverage::calc(int Lstart, int Sstart) const
{
  firstIndex i1; secondIndex i2; thirdIndex i3;
  int number_line = data.cols();
  int number_sample = data.depth();
  // Read in all the data we will be working with. Because it is
  // convenient, shuffle the array bounds so they go from -whsize,
  // -whsize to Number_line + whsize, Number_sample + whsize. Be
  // careful not to go past the edge of the underlying data, we add
  // zero padding if needed.
  input_data.resize(Range(0, number_band() - 1),
		    Range(-whsize, number_line + 2 * whsize - 1),
		    Range(-whsize, number_sample + 2 * whsize - 1));
  input_data = 0;
  int rlstart = std::max(Lstart - whsize, 0);
  int rlend = std::min(Lstart + number_line + whsize, 
		       raster_image(0).number_line());
  int rsstart = std::max(Sstart - whsize, 0);
  int rsend = std::min(Sstart + number_sample + whsize, 
		       raster_image(0).number_sample());
  Range ra(Range::all());
  Range rl(rlstart - Lstart, rlend - Lstart - 1);
  Range rs(rsstart - Sstart, rsend - Sstart - 1);
  input_data(ra, rl, rs) = rimg->read_double(rlstart, rsstart, rlend - rlstart,
					rsend - rsstart);
  // Translate any 0's to all the data.
  for(int i = rl.first(); i <= rl.last(); ++i)
    for(int j = rs.first(); j <= rs.last(); ++j)
      if(any(input_data(ra, i, j) == 0))
	input_data(ra, i, j) = 0;
  for(int i = 0; i < number_line; ++i)
    for(int j = 0; j < number_sample; ++j) 
      if(input_data(0, i,j) == 0) {
	data(ra, i,j) = 0;
	// If we aren't allowing gore, then set the value to 0 if any
	// point is 0.
      } else if(!allow_gore_ && any(input_data == 0)) {
	data(ra, i, j) = 0;
      } else {
      // Get the data around the doughnut. We get this is four pieces,
      // the value above, below, left, and right of the doughnut.
	Array<double, 3> dabove(input_data(ra,Range(i - whsize, i - dhsize - 1),
				      Range(j - whsize, j + whsize)));
	Array<double, 3> dbelow(input_data(ra,Range(i + dhsize + 1, i + whsize),
				  Range(j - whsize, j + whsize)));
	Array<double, 3> dleft(input_data(ra,Range(i - dhsize, i + dhsize),
				 Range(j - whsize, j - dhsize - 1)));
	Array<double, 3> dright(input_data(ra,Range(i - dhsize, i + dhsize),
				  Range(j + dhsize + 1, j + whsize)));

	// Average all the data, excluding 0's
	
	int cnt = count(dabove(0,ra,ra) != 0) + count(dbelow(0,ra,ra) != 0) +
	  count(dleft(0,ra,ra) != 0) + count(dright(0,ra,ra) != 0);
	if(cnt > 0) {
	  data(ra, i, j) = sum(sum(dabove, i3), i2);
	  data(ra, i, j) += sum(sum(dbelow, i3), i2);
	  data(ra, i, j) += sum(sum(dleft, i3), i2);
	  data(ra, i, j) += sum(sum(dright, i3), i2);
	  data(ra, i, j) /= cnt;
	} else
	  data(ra, i, j) = 0;
      }
}

