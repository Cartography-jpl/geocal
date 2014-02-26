#ifndef MAGNIFY_REPLICATE_H
#define MAGNIFY_REPLICATE_H
#include "calc_raster.h"
namespace GeoCal {
/****************************************************************//**
  This creates a magnified image of a RasterImage. We do replicatione
  to get the values.
*******************************************************************/
class MagnifyReplicate : public CalcRaster {
public:
  MagnifyReplicate(const boost::shared_ptr<RasterImage>& Data,
		  int Magfactor, int Number_tile = 4);
  virtual ~MagnifyReplicate() {}
  virtual void print(std::ostream& Os) const
  {
    Os << "MagnifyReplicate\n"
       << "  magfactor: " << magfactor_ << "\n"
       << "  underlying raster: \n" << *raw_data << "\n";
  }
  const boost::shared_ptr<RasterImage>& raw_data_ptr() const {return raw_data;}
  int magfactor() const {return magfactor_;}
protected:
  virtual void calc(int Lstart, int Sstart) const;
private:
  boost::shared_ptr<RasterImage> raw_data;
  int magfactor_;
};
}
#endif

