#include "with_parameter.h"
using namespace GeoCal;
using namespace blitz;

//-----------------------------------------------------------------------
/// Return the part of the parameter set that passes the mask.
//-----------------------------------------------------------------------

blitz::Array<double, 1> WithParameter::parameter_subset() const
{ 
  blitz::Array<double, 1> p(blitz::count(parameter_mask_));
  blitz::Array<double, 1> pfull = parameter();
  if(parameter_mask_.rows() != pfull.rows())
    throw Exception("parameter_mask must be the same size as parameter");
  int j = 0;
  for(int i = 0; i < pfull.rows(); ++i)
    if(parameter_mask_(i))
      p(j++) = pfull(i);
  return p;
}

//-----------------------------------------------------------------------
/// Set the part of the parameter set that passes the mask.
//-----------------------------------------------------------------------

void WithParameter::parameter_subset(const blitz::Array<double, 1>& P)
{ 
  if(P.rows() != blitz::count(parameter_mask_))
    throw Exception("P is not the size expected from the parameter_mask");
  blitz::Array<double, 1> pfull = parameter().copy();
  if(parameter_mask_.rows() != pfull.rows())
    throw Exception("parameter_mask must be the same size as parameter");
  int j = 0;
  for(int i = 0; i < pfull.rows(); ++i)
    if(parameter_mask_(i))
      pfull(i) = P(j++);
  parameter(pfull);
}

//-----------------------------------------------------------------------
/// Return the part of the parameter set that passes the mask.
//-----------------------------------------------------------------------

ArrayAd<double, 1> WithParameter::parameter_with_derivative_subset() const
{ 
  ArrayAd<double, 1> pfull = parameter_with_derivative();
  ArrayAd<double, 1> p(blitz::count(parameter_mask_), pfull.number_variable());
  int j = 0;
  if(parameter_mask_.rows() != pfull.rows())
    throw Exception("parameter_mask must be the same size as parameter");
  for(int i = 0; i < pfull.rows(); ++i)
    if(parameter_mask_(i))
      p(j++) = pfull(i);
  return p;
}

//-----------------------------------------------------------------------
/// Set the part of the parameter set that passes the mask.
//-----------------------------------------------------------------------

void WithParameter::parameter_with_derivative_subset
(const ArrayAd<double, 1>& P)
{
  if(P.rows() != blitz::count(parameter_mask_))
    throw Exception("P is not the size expected from the parameter_mask");
  ArrayAd<double, 1> pfull = parameter().copy();
  pfull.resize_number_variable(P.number_variable());
  int j = 0;
  if(parameter_mask_.rows() != pfull.rows())
    throw Exception("parameter_mask must be the same size as parameter");
  for(int i = 0; i < pfull.rows(); ++i)
    if(parameter_mask_(i))
      pfull(i) = P(j++);
  parameter_with_derivative_subset(pfull);
}

//-----------------------------------------------------------------------
/// Return the part of the parameter name that passes the mask.
//-----------------------------------------------------------------------

std::vector<std::string> WithParameter::parameter_name_subset() const
{
  std::vector<std::string> res;
  std::vector<std::string> name_full = parameter_name();
  if(parameter_mask_.rows() != (int) name_full.size())
    throw Exception("parameter_mask must be the same size as parameter_name");
  for(int i = 0; i < (int) name_full.size(); ++i)
    if(parameter_mask_(i))
      res.push_back(name_full[i]);
  return res;
}
