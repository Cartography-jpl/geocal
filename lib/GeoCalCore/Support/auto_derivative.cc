#include "auto_derivative.h"
#include "geocal_serialize_support.h"
#include <boost/serialization/array.hpp>
using namespace GeoCal;
#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION

template<class T> template<class Archive> 
void AutoDerivative<T>::serialize(Archive& ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(AutoDerivative<T>);
  ar & GEOCAL_NVP(val) & GEOCAL_NVP(grad);
}

GEOCAL_IMPLEMENT(AutoDerivative<double>);
							 
template<class Archive, class T>
void boost::serialization::save(Archive& ar, const blitz::Array<T, 1>& A, 
			      const unsigned version) 
{
  using boost::serialization::make_array;
  if(A.size() > 0 && !A.isStorageContiguous())
    throw GeoCal::Exception("We can only save contiguous matrix data");
  int size = A.rows();
  ar << GEOCAL_NVP(size);
  if(A.size() > 0)
    ar << GEOCAL_NVP2("data", make_array(A.data(), A.size()));
}

template<typename Archive, class T>
void boost::serialization::load(Archive& ar, blitz::Array<T, 1>& A, 
				const unsigned version) 
{
  using boost::serialization::make_array;
  int size;
  ar >> GEOCAL_NVP(size);
  A.resize(size);
  if(A.size() > 0)
    ar >> GEOCAL_NVP2("data", make_array(A.data(), A.size()));
}

template<class Archive, class T>
void boost::serialization::save(Archive& ar, const blitz::Array<T, 2>& A, 
			      const unsigned version) 
{
  using boost::serialization::make_array;
  if(A.size() > 0 && !A.isStorageContiguous())
    throw GeoCal::Exception("We can only save contiguous matrix data");
  int rows = A.rows();
  int cols = A.cols();
  ar << GEOCAL_NVP(rows) << GEOCAL_NVP(cols);
  if(A.size() > 0)
    ar << GEOCAL_NVP2("data", make_array(A.data(), A.size()));
}

template<typename Archive, class T>
void boost::serialization::load(Archive& ar, blitz::Array<T, 2>& A, 
				const unsigned version) 
{
  using boost::serialization::make_array;
  int rows, cols;
  ar >> GEOCAL_NVP(rows) >> GEOCAL_NVP(cols);
  A.resize(rows, cols);
  if(A.size() > 0)
    ar >> GEOCAL_NVP2("data", make_array(A.data(), A.size()));
}

template void boost::serialization::load(polymorphic_iarchive& ar, 
					 blitz::Array<double, 1>& A, 
					 const unsigned version);
template void boost::serialization::save(polymorphic_oarchive& ar, 
					 const blitz::Array<double, 1>& A, 
					 const unsigned version);

template void boost::serialization::load(polymorphic_iarchive& ar, 
					 blitz::Array<double, 2>& A, 
					 const unsigned version);
template void boost::serialization::save(polymorphic_oarchive& ar, 
					 const blitz::Array<double, 2>& A, 
					 const unsigned version);

template void boost::serialization::load(polymorphic_iarchive& ar, 
					 blitz::Array<bool, 1>& A, 
					 const unsigned version);
template void boost::serialization::save(polymorphic_oarchive& ar, 
					 const blitz::Array<bool, 1>& A, 
					 const unsigned version);

#endif
