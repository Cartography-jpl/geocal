#include "display_stretch_std_dev.h"
#include "statistic.h"
#include <boost/foreach.hpp>
using namespace GeoCal;
using namespace blitz;

inline unsigned char in_range(double x)
{
  return (unsigned char) (x < 0 ? 0 : (x > 255 ? 255 : x));
}

BZ_DECLARE_FUNCTION_RET(in_range, unsigned char);

// See base class for description.
void DisplayStretchStdDev::stretch(const blitz::Array<int, 2>& Raw_data, 
		     blitz::Array<unsigned char, 2>& Stretched_data) const
{
  std::cerr << "Calculate stretch stat\n";
  Statistic stat;
  BOOST_FOREACH(int v, Raw_data)
    stat += v;
  double mn = stat.mean() + min_sig * stat.sigma();
  double mx = stat.mean() + max_sig * stat.sigma();
  std::cerr << "Stretch data\n";
  Stretched_data = in_range(rint((Raw_data - mn) * (255.0 / (mx - mn))));
  std::cerr << "Done with stretch data\n";
}
