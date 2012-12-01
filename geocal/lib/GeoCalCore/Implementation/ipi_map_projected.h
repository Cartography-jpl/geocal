#ifndef IPI_MAP_PROJECTED_H
#define IPI_MAP_PROJECTED_H
#include "ipi.h"
#include "calc_map_projected.h"
#include <boost/multi_array.hpp>

namespace GeoCal {
/****************************************************************//**
  This is a CalcMapProjected where the calculation of the image
  coordinates is done by an Ipi.

  This class calculates the data on the fly. Sometimes
  this is what you want, but if you are going to be using the
  resulting data a few times, you may want to use a
  MemoryRasterImage to generate a copy once and keep it in
  memory.
*******************************************************************/

class IpiMapProjected : public CalcMapProjected {
public:
  IpiMapProjected(const MapInfo& Mi, const boost::shared_ptr<Ipi>& I,
		  const boost::shared_ptr<RasterImage>& R,
		  const boost::shared_ptr<Dem>& D,
		  int Avg_fact = -1,
		  bool Read_into_memory = true);

  IpiMapProjected(const MapInfo& Mi, const boost::shared_ptr<Ipi>& I,
		  const std::vector<boost::shared_ptr<RasterImage> >& R,
		  const boost::shared_ptr<Dem>& D, 
		  int Avg_fact = -1,
		  bool Read_into_memory = true);

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~IpiMapProjected() {}

  virtual void print(std::ostream& Os) const;
protected:
  virtual ImageCoordinate calc_image_coordinates(const GroundCoordinate& Gc) 
    const;
private:
  boost::shared_ptr<Ipi> ipi;
};

}
#endif
