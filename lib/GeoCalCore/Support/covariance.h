#ifndef COVARIANCE_H
#define COVARIANCE_H
#include "printable.h"

namespace GeoCal {

/****************************************************************//**
  This calculates simple statistics on a pair of numbers. It does 
  this by keeping running sums, so we don't need all the numbers to
  be available at once.
*******************************************************************/

class Covariance : public Printable<Covariance> {
public:
//-----------------------------------------------------------------------
/// Constructor  
//-----------------------------------------------------------------------
  Covariance() : sum1(0), sum2(0), sum_product(0), count_(0) {}

//-----------------------------------------------------------------------
/// Add another pair to the running total.
//-----------------------------------------------------------------------

  void add(double x1, double x2) {
    sum1 += x1; 
    sum2 += x2; 
    sum_product += x1 * x2; 
    count_ += 1; 
  }

//-----------------------------------------------------------------------
/// Return number of pairs that have been added.
//-----------------------------------------------------------------------
  
  int count() const {return count_;}

//-----------------------------------------------------------------------
/// Return mean of first number. Returns 0 if count < 1.
//-----------------------------------------------------------------------

  double mean1() const { 
    if(count_ < 1)
      return 0.0;
    return sum1 / count_; 
  }

//-----------------------------------------------------------------------
/// Return mean of second number. Returns 0 if count < 1.
//-----------------------------------------------------------------------

  double mean2() const { 
    if(count_ < 1)
      return 0.0;
    return sum2 / count_; 
  }

//-----------------------------------------------------------------------
/// Return covariance between numbers. Returns 0 if count < 2.
//-----------------------------------------------------------------------

  double covariance() const {
    if(count_ < 2)
      return 0;
    return (sum_product - sum1 * sum2 / count_) / (count_ - 1);
  }
  void print(std::ostream& os) const;
private:
  double sum1;			///< Sum of the first value.
  double sum2; 			///< Sum of the second value.
  double sum_product;		///< Sum of the product of the two
				///values.
  int count_;			///< Number of items that have been added.
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(Covariance);
#endif

