#include "scale_image.h"
#include "ostream_pad.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void ScaleImage::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CalcRaster)
    & GEOCAL_NVP_(raw_data)
    & GEOCAL_NVP_(scale);
}

template<class Archive>
void ScaleImageMultiBand::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CalcRasterMultiBand)
    & GEOCAL_NVP_(raw_data)
    & GEOCAL_NVP_(scale);
}

template<class Archive>
void ScaleImageGroundConnection::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ImageGroundConnectionCopy);
}

GEOCAL_IMPLEMENT(ScaleImage);
GEOCAL_IMPLEMENT(ScaleImageMultiBand);
GEOCAL_IMPLEMENT(ScaleImageGroundConnection);
#endif

//-----------------------------------------------------------------------
/// Scale the image of an ImageGroundConnection.
//-----------------------------------------------------------------------

ScaleImageGroundConnection::ScaleImageGroundConnection
(const boost::shared_ptr<ImageGroundConnection>& Igc, double Scale_factor)
  : ImageGroundConnectionCopy(Igc)
{
  if(Igc->image())
    image(boost::shared_ptr<RasterImage>
	  (new ScaleImage(Igc->image(), Scale_factor)));
  if(Igc->image_multi_band())
    image_multi_band
      (boost::shared_ptr<RasterImageMultiBand>
       (new ScaleImageMultiBand(Igc->image_multi_band(), Scale_factor)));
}

void ScaleImageGroundConnection::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "ScaleImageGroundConnection:\n"
     << "  Underlying Igc:\n";
  ImageGroundConnectionCopy::print(opad);
  opad << "\n";
  opad.strict_sync();
}
