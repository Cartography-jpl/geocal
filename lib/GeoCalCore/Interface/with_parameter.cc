#include "with_parameter.h"
#include "geocal_serialize_support.h"
#include <boost/foreach.hpp>
using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void WithParameter::serialize
(Archive & UNUSED(ar), const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(WithParameter);
}

template<class Archive>
void WithParameterNested::serialize
(Archive & ar, const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(WithParameter);
  GEOCAL_GENERIC_BASE(WithParameterNested);
  GEOCAL_BASE(WithParameterNested, WithParameter);
  ar & GEOCAL_NVP(obj_list);
}

template<class Archive>
void WithParameterShare::serialize
(Archive & ar, const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(WithParameter);
  GEOCAL_GENERIC_BASE(WithParameterShare);
  GEOCAL_BASE(WithParameterShare, WithParameter);
  ar & GEOCAL_NVP(obj_list)
    & GEOCAL_NVP_(parameter_mask);
}

GEOCAL_IMPLEMENT(WithParameter);
GEOCAL_IMPLEMENT(WithParameterNested);
GEOCAL_IMPLEMENT(WithParameterShare);
#endif

//-----------------------------------------------------------------------
/// Return the part of the parameter set that passes the mask.
//-----------------------------------------------------------------------

blitz::Array<double, 1> WithParameter::parameter_subset() const
{ 
  blitz::Array<bool, 1> pm(parameter_mask());
  blitz::Array<double, 1> p(blitz::count(pm));
  blitz::Array<double, 1> pfull = parameter();
  if(pm.rows() != pfull.rows())
    throw Exception("parameter_mask must be the same size as parameter");
  int j = 0;
  for(int i = 0; i < pfull.rows(); ++i)
    if(pm(i))
      p(j++) = pfull(i);
  return p;
}

//-----------------------------------------------------------------------
/// Set the part of the parameter set that passes the mask.
//-----------------------------------------------------------------------

void WithParameter::parameter_subset(const blitz::Array<double, 1>& P)
{ 
  blitz::Array<bool, 1> pm(parameter_mask());
  if(P.rows() != blitz::count(pm))
    throw Exception("P is not the size expected from the parameter_mask");
  blitz::Array<double, 1> pfull = parameter().copy();
  if(pm.rows() != pfull.rows())
    throw Exception("parameter_mask must be the same size as parameter");
  int j = 0;
  for(int i = 0; i < pfull.rows(); ++i)
    if(pm(i))
      pfull(i) = P(j++);
  parameter(pfull);
}

//-----------------------------------------------------------------------
/// Return the part of the parameter set that passes the mask.
//-----------------------------------------------------------------------

ArrayAd<double, 1> WithParameter::parameter_with_derivative_subset() const
{ 
  blitz::Array<bool, 1> pm(parameter_mask());
  ArrayAd<double, 1> pfull = parameter_with_derivative();
  ArrayAd<double, 1> p(blitz::count(pm), pfull.number_variable());
  int j = 0;
  if(pm.rows() != pfull.rows())
    throw Exception("parameter_mask must be the same size as parameter");
  for(int i = 0; i < pfull.rows(); ++i)
    if(pm(i))
      p(j++) = pfull(i);
  return p;
}

//-----------------------------------------------------------------------
/// Set the part of the parameter set that passes the mask.
//-----------------------------------------------------------------------

void WithParameter::parameter_with_derivative_subset
(const ArrayAd<double, 1>& P)
{
  blitz::Array<bool, 1> pm(parameter_mask());
  if(P.rows() != blitz::count(pm))
    throw Exception("P is not the size expected from the parameter_mask");
  ArrayAd<double, 1> pfull = parameter().copy();
  pfull.resize_number_variable(P.number_variable());
  int j = 0;
  if(pm.rows() != pfull.rows())
    throw Exception("parameter_mask must be the same size as parameter");
  for(int i = 0; i < pfull.rows(); ++i)
    if(pm(i))
      pfull(i) = P(j++);
  parameter_with_derivative(pfull);
}

//-----------------------------------------------------------------------
/// Return the part of the parameter name that passes the mask.
//-----------------------------------------------------------------------

std::vector<std::string> WithParameter::parameter_name_subset() const
{
  blitz::Array<bool, 1> pm(parameter_mask());
  std::vector<std::string> res;
  std::vector<std::string> name_full = parameter_name();
  if(pm.rows() != (int) name_full.size())
    throw Exception("parameter_mask must be the same size as parameter_name");
  for(int i = 0; i < (int) name_full.size(); ++i)
    if(pm(i))
      res.push_back(name_full[i]);
  return res;
}

//-----------------------------------------------------------------------
/// A very common thing is to want the gradient each of the active
/// parameters (those returned by parameter_subset) to be the
/// parameters the gradient is relative to, i.e. the first parameter
/// has gradient of [1,0,0...], the second [0,1,0...] etc. This
/// utility routine sets that up.
//-----------------------------------------------------------------------

void WithParameter::add_identity_gradient()
{
  ArrayAd<double, 1> p(parameter_with_derivative_subset());
  p.resize_number_variable(p.rows());
  p.jacobian() = 0;
  for(int i = 0; i < p.rows(); ++i)
    p.jacobian()(i,i) = 1.0;
  parameter_with_derivative_subset(p);
}

int WithParameterNested::total_size() const
{
  int res = 0;
  BOOST_FOREACH(boost::shared_ptr<WithParameter> obj, obj_list)
    res += obj->parameter().rows();
  return res;
}

int WithParameterNested::max_num_var() const
{
  int res = 0;
  BOOST_FOREACH(boost::shared_ptr<WithParameter> obj, obj_list)
    res = std::max(res, obj->parameter_with_derivative().number_variable());
  return res;
}

blitz::Array<double, 1> WithParameterNested::parameter() const
{
  Array<double, 1> res(total_size());
  int i = 0;
  BOOST_FOREACH(boost::shared_ptr<WithParameter> obj, obj_list) {
    Array<double, 1> p = obj->parameter();
    if(p.rows() > 0) {
      Range r(i, i + p.rows()-1);
      res(r) = p;
      i += p.rows();
    }
  }
  return res;
}

void WithParameterNested::parameter(const blitz::Array<double, 1>& Parm)
{
  if(Parm.rows() != total_size())
    throw Exception("Parm is not the right size");
  int i = 0;
  BOOST_FOREACH(boost::shared_ptr<WithParameter> obj, obj_list) {
    Array<double, 1> p = obj->parameter();
    if(p.rows() > 0) {
      Range r(i, i + p.rows()-1);
      obj->parameter(Parm(r));
      i += p.rows();
    }
  }
}

ArrayAd<double, 1> WithParameterNested::parameter_with_derivative() const
{
  ArrayAd<double, 1> res(total_size(), max_num_var());
  int i = 0;
  BOOST_FOREACH(boost::shared_ptr<WithParameter> obj, obj_list) {
    ArrayAd<double, 1> p = obj->parameter_with_derivative();
    if(p.rows() > 0) {
      Range r(i, i + p.rows()-1);
      res(r) = p;
      i += p.rows();
    }
  }
  return res;
}

void WithParameterNested::parameter_with_derivative
(const ArrayAd<double, 1>& Parm)
{
  if(Parm.rows() != total_size())
    throw Exception("Parm is not the right size");
  int i = 0;
  BOOST_FOREACH(boost::shared_ptr<WithParameter> obj, obj_list) {
    ArrayAd<double, 1> p = obj->parameter_with_derivative();
    if(p.rows() > 0) {
      Range r(i, i + p.rows()-1);
      obj->parameter_with_derivative(Parm(r));
      i += p.rows();
    }
  }
}

std::vector<std::string> WithParameterNested::parameter_name() const
{
  std::vector<std::string> res;
  BOOST_FOREACH(boost::shared_ptr<WithParameter> obj, obj_list) {
    std::vector<std::string> t = obj->parameter_name();
    res.insert(res.end(), t.begin(), t.end());
  }
  return res;
}

blitz::Array<bool, 1> WithParameterNested::parameter_mask() const
{
  Array<bool, 1> res(total_size());
  int i = 0;
  BOOST_FOREACH(boost::shared_ptr<WithParameter> obj, obj_list) {
    Array<bool, 1> p = obj->parameter_mask();
    if(p.rows() > 0) {
      Range r(i, i + p.rows()-1);
      res(r) = p;
      i += p.rows();
    }
  }
  return res;
}

void WithParameterShare::add_object(const boost::shared_ptr<WithParameter>& Obj)
{
  obj_list.push_back(Obj);
  if(obj_list.size() == 1)
    parameter_mask_.reference(Obj->parameter_mask().copy());
}

blitz::Array<double, 1> WithParameterShare::parameter() const
{
  if(obj_list.size() == 0)
    throw Exception("Need to have at least one object in WithParameterShare");
  return obj_list[0]->parameter();
}

void WithParameterShare::parameter(const blitz::Array<double, 1>& Parm)
{
  if(obj_list.size() == 0)
    throw Exception("Need to have at least one object in WithParameterShare");
  for(auto obj : obj_list)
    obj->parameter(Parm);
}

ArrayAd<double, 1> WithParameterShare::parameter_with_derivative() const
{
  if(obj_list.size() == 0)
    throw Exception("Need to have at least one object in WithParameterShare");
  return obj_list[0]->parameter_with_derivative();
}

void WithParameterShare::parameter_with_derivative
(const ArrayAd<double, 1>& Parm)
{
  if(obj_list.size() == 0)
    throw Exception("Need to have at least one object in WithParameterShare");
  for(auto obj : obj_list)
    obj->parameter_with_derivative(Parm);
}

std::vector<std::string> WithParameterShare::parameter_name() const
{
  if(obj_list.size() == 0)
    throw Exception("Need to have at least one object in WithParameterShare");
  return obj_list[0]->parameter_name();
}

blitz::Array<bool, 1> WithParameterShare::parameter_mask() const
{
  return parameter_mask_;
}

void WithParameterShare::parameter_mask(const blitz::Array<bool, 1>& M)
{
  if(M.rows() != parameter().rows())
    throw Exception("M is wrong size");
  parameter_mask_.reference(M.copy());
}
