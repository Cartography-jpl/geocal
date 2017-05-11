#ifndef STATISTIC_H
#define STATISTIC_H
#include <cmath>
#include "printable.h"

namespace GeoCal {

/****************************************************************//**
   This calculates basic statistics.
*******************************************************************/

class Statistic : public Printable<Statistic> {
public:
//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

  Statistic()
    :sum_(0), sum_squares_(0), min_(0), max_(0), 
     sum_abs_(0), count_(0), min_index_(0), max_index_(0) {}
  void add(double x);

//-----------------------------------------------------------------------
/// Add datapoint (alternative name for add).
//-----------------------------------------------------------------------

  Statistic& operator+=(double x) {add(x); return *this;}

//-----------------------------------------------------------------------
/// Minimum value encountered.
//-----------------------------------------------------------------------

  double min() const {return min_;}

//-----------------------------------------------------------------------
/// Maximum value encountered.
//-----------------------------------------------------------------------

  double max() const {return max_;}

//-----------------------------------------------------------------------
/// Mean value.
//-----------------------------------------------------------------------

  double mean() const {return (count_ > 0 ? sum_ / count_ : 0); }

//-----------------------------------------------------------------------
/// Standard deviation.
//-----------------------------------------------------------------------

  double sigma() const {
    return (count_ > 1 ? 
	    sqrt((sum_squares_ - sum_ * sum_ / count_) / (count_ - 1)) :
	    0);
  }

//-----------------------------------------------------------------------
/// Mean of the absolute value.
//-----------------------------------------------------------------------

  double mean_abs() const {return (count_ > 0 ? sum_abs_ / count_ : 0); }

//-----------------------------------------------------------------------
/// Index number of minimum point, where the first point added is
/// index 0, second is 1 and so on.
//-----------------------------------------------------------------------

  int min_index() const {return min_index_;}

//-----------------------------------------------------------------------
/// Index number of maximum point, where the first point added is
/// index 0, second is 1 and so on.
//-----------------------------------------------------------------------

  int max_index() const {return max_index_;}

//-----------------------------------------------------------------------
/// Number of points added.
//-----------------------------------------------------------------------

  int count() const {return count_;}

  void print(std::ostream& Os) const;
private:
  double sum_, sum_squares_, min_, max_, sum_abs_;
  int count_, min_index_, max_index_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(Statistic);
#endif
