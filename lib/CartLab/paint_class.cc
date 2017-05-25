#include "paint_class.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void PaintClass::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CalcRasterMultiBand);
  ar & GEOCAL_NVP(class_id_img) & GEOCAL_NVP(pan_img)
    & GEOCAL_NVP(color) & GEOCAL_NVP(class_id_first_color);
}

GEOCAL_IMPLEMENT(PaintClass);
#endif

PaintClass::PaintClass(const boost::shared_ptr<RasterImage>& Class_id_img,
		       const boost::shared_ptr<RasterImage>& Pan_img,
		       const blitz::Array<int, 2>& Color,
		       int Class_id_first_color)
: CalcRasterMultiBand(*Pan_img, Color.cols()),
  class_id_img(Class_id_img), pan_img(Pan_img), color(Color.copy()),
  class_id_first_color(Class_id_first_color)
{
}

void PaintClass::calc(int Lstart, int Sstart) const
{
  Range ra(Range::all());
  Array<double, 2> pd = pan_img->read_double(Lstart, Sstart, data.cols(), 
					     data.depth());
  Array<int, 2> cid = class_id_img->read(Lstart, Sstart, data.cols(), 
					data.depth());
  for(int i = 0; i < pd.rows(); ++i)
    for(int j = 0; j < pd.cols(); ++j)
      if(cid(i, j) == 0 || cid(i,j) < class_id_first_color ||
	 cid(i, j) - class_id_first_color >= color.rows())
	data(ra, i, j) = pd(i, j);
      else
	data(ra, i, j) = cast<double>(color(cid(i,j) - class_id_first_color, ra));
}
