#include "context_sqrt_decode_image.h"
#include "geocal_serialize_support.h"
#include <cmath>
using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void ContextSqrtDecodeImage::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CalcRaster)
    & GEOCAL_NVP_(raw_data);
}

GEOCAL_IMPLEMENT(ContextSqrtDecodeImage);
#endif

// These values comes from the SIS referenced at the start of this
// class.


int ContextSqrtDecodeImage::data_value[256] =
  {1, 3, 5, 7, 9, 11, 13, 15, 17, 20, 22, 24, 27, 29, 32, 35, 38,
   41, 44, 47, 50, 54, 58, 61, 65, 69, 73, 77, 82, 86, 91, 95, 100,
   105, 110, 115, 121, 126, 131, 137, 143, 149, 155, 161, 167, 173,
   179, 186, 193, 199, 206, 213, 220, 228, 235, 243, 250, 258, 266,
   274, 282, 290, 298, 306, 315, 324, 332, 341, 350, 359, 369, 378,
   387, 397, 407, 416, 426, 436, 446, 457, 467, 478, 488, 499, 510,
   521, 532, 543, 554, 566, 577, 589, 601, 613, 625, 637, 649, 662,
   674, 687, 699, 712, 725, 738, 751, 765, 778, 792, 805, 819, 833,
   847, 861, 875, 890, 904, 919, 933, 948, 963, 978, 993, 1009,
   1024, 1039, 1055, 1071, 1087, 1103, 1119, 1135, 1151, 1168, 1184,
   1201, 1218, 1235, 1252, 1269, 1286, 1304, 1321, 1339, 1356, 1374,
   1392, 1410, 1429, 1447, 1465, 1484, 1502, 1521, 1540, 1559, 1578,
   1598, 1617, 1636, 1656, 1676, 1696, 1715, 1736, 1756, 1776, 1796,
   1817, 1838, 1858, 1879, 1900, 1921, 1943, 1964, 1985, 2007, 2029,
   2050, 2072, 2094, 2117, 2139, 2161, 2184, 2206, 2229, 2252, 2275,
   2298, 2321, 2345, 2368, 2392, 2415, 2439, 2463, 2487, 2511, 2535,
   2560, 2584, 2609, 2634, 2658, 2683, 2709, 2734, 2759, 2784, 2810,
   2836, 2861, 2887, 2913, 2939, 2966, 2992, 3019, 3045, 3072, 3099,
   3126, 3153, 3180, 3207, 3235, 3262, 3290, 3317, 3345, 3373, 3401,
   3430, 3458, 3486, 3515, 3544, 3573, 3601, 3630, 3660, 3689, 3718,
   3748, 3777, 3807, 3837, 3867, 3897, 3927, 3958, 3988, 4019, 4049,
   4080};


void ContextSqrtDecodeImage::calc(int Lstart, int Sstart) const
{
  blitz::Array<int, 2> rd = raw_data_->read(Lstart, Sstart, data.rows(),
					   data.cols());
  for(int i = 0; i < data.rows(); ++i)
    for(int j = 0; j < data.cols(); ++j) {
      if(rd(i,j) < 0 || rd(i, j) > 255) {
	Exception e;
	e << "Out of range data found at (" << i + Lstart
	  << ", " << j + Sstart << "), value " << rd(i, j);
	throw e;
      }
      data(i,j) = data_value[rd(i,j)];
    }
}
