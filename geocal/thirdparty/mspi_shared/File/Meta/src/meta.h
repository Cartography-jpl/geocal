//===========================================================================
//                                                                          
//                               Meta                                   
//                                                                          
//===========================================================================
//
//             Copyright 2012, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//===========================================================================

#ifndef META_H
#define META_H

#include <string>		// Definition of std::string
#include <map>			// Definition of std::map
#include <boost/variant.hpp> 	// Definition of boost::variant
#include "MSPI-Shared/ErrorHandling/src/exception.h" 
				// Definition of MSPI_EXCEPTIONm.
#include <sstream> 		// Definition of std::ostringstream
#include <iomanip>		// Definition of std::setprecision
#include <limits> 	        // Definition of std::numeric_limits

namespace MSPI {
  namespace Shared {
    class Meta;
  }
}

/////////////////////////////////////////////////////////////////////////////
/// @brief Contains file metadata.  Provides read/write methods.
/////////////////////////////////////////////////////////////////////////////

class MSPI::Shared::Meta {
public:

  Meta();

  size_t number_element() const;
  
  template <class T>
  void add(const std::string& Name, const T& Value);

  void add(const Meta& Meta);

  template <class T>
  T get_value(const std::string& Name) const;

  Meta match_prefix(const std::string& Prefix) const;
  Meta add_prefix(const std::string& Prefix) const;
  void stream_export(std::ostream& Out) const;
  void stream_export2(std::ostream& Out) const;

protected:

  typedef boost::variant<std::string,int,unsigned,bool,float,double> any_type;

  const std::map<std::string,any_type>& meta() const { return meta_; }

  class pointer_visitor : public boost::static_visitor<void*> {
    // Returns pointer to memory associated with a any_type object. 
  public:
    void *operator()(std::string& P) const { return &(P[0]); }
    void *operator()(int& P) const { return &P; }
    void *operator()(unsigned& P) const { return &P; }
    void *operator()(bool& P) const { return &P; }
    void *operator()(float& P) const { return &P; }
    void *operator()(double& P) const { return &P; }
  };

  class const_pointer_visitor : public boost::static_visitor<const void*> {
    // Returns const pointer to memory associated with a any_type object. 
  public:
    const void *operator()(const std::string& P) const { return P.c_str(); }
    const void *operator()(const int& P) const { return &P; }
    const void *operator()(const unsigned& P) const { return &P; }
    const void *operator()(const bool& P) const { return &P; }
    const void *operator()(const float& P) const { return &P; }
    const void *operator()(const double& P) const { return &P; }
  };

  typedef std::map<std::string,any_type>::iterator it_type;
  typedef std::map<std::string,any_type>::const_iterator const_it_type;

  void add_any(const std::string& Name, const any_type& Value);

private:

  class to_string_visitor : public boost::static_visitor<std::string> {
    // Returns string representation of the value.
  public:
    template <typename T>
    std::string operator()(const T& P) const {
      std::ostringstream s;
      s << std::setprecision(std::numeric_limits<T>::digits10) << P;
      return s.str();
    }
  };

  class is_equal_visitor : public boost::static_visitor<bool> {
    // Returns boolean indicating if the given values are equal.
  public:
    template <typename T, typename U>
    bool operator()(const T&, const U&) const { return false; }
    template <typename T>
    bool operator()(const T& A, const T& B) const { return A == B; }
  };

  typedef std::pair<const std::string,any_type> value_type;

  void insert(const value_type& Value);

  std::map<std::string,any_type> meta_;
};

/////////////////////////////////////////////////////////////////////////////
/// Default constructor
/////////////////////////////////////////////////////////////////////////////

inline
MSPI::Shared::Meta::Meta()
{
}

/////////////////////////////////////////////////////////////////////////////
/// Return the number of elements.
/////////////////////////////////////////////////////////////////////////////

inline size_t
MSPI::Shared::Meta::number_element() const
{
  return meta_.size();
}

/////////////////////////////////////////////////////////////////////////////
/// Insert the given element.  Has no effect if there is an existing element/
/// with the same key.
/////////////////////////////////////////////////////////////////////////////

inline void
MSPI::Shared::Meta::insert(const value_type& Value)
{
  meta_.insert(Value);
}

/////////////////////////////////////////////////////////////////////////////
/// Return value associated with the given name.
/// Throws an exception if no value is available.
/////////////////////////////////////////////////////////////////////////////

template <class T> T
MSPI::Shared::Meta::get_value(
  const std::string& Name
) const
{
  const_it_type i = meta_.find(Name);
  if (i == meta_.end()) {
    throw MSPI_EXCEPTIONm("No value for " + Name);
  }

  return boost::get<T>(i->second);
}

/////////////////////////////////////////////////////////////////////////////
/// Add metadata element.  Throw an exception if the element already exists
/// with different value.
/////////////////////////////////////////////////////////////////////////////

template <class T> void
MSPI::Shared::Meta::add(
  const std::string& Name, 
  const T& Value
)
{
  it_type i = meta_.find(Name);

  if (i != meta_.end()) {
    T value = boost::get<T>(i->second);

    if (value != Value) {
      std::ostringstream msg;
      msg << "Update not allowed for metadata: "
	  << Name << "=" << value
	  << " -> " << Name << "=" << Value;
      throw MSPI_EXCEPTIONm(msg.str());
    }
  }
  meta_[Name] = any_type(Value);
}

/////////////////////////////////////////////////////////////////////////////
/// Add metadata element.  Throw an exception if the element already exists
/// with different value.
/////////////////////////////////////////////////////////////////////////////

inline void
MSPI::Shared::Meta::add_any(
  const std::string& Name, 
  const any_type& Value
)
{
  it_type i = meta_.find(Name);

  if (i != meta_.end()) {
    if (not boost::apply_visitor(is_equal_visitor(),i->second, Value)) {
      std::ostringstream msg;
      msg << "Update not allowed for metadata: "
	  << Name << "=" 
	  << boost::apply_visitor(to_string_visitor(),i->second)
	  << " -> " << Name << "=" 
	  << boost::apply_visitor(to_string_visitor(),Value);
      throw MSPI_EXCEPTIONm(msg.str());
    }
  }
  meta_[Name] = Value;
}

#endif
