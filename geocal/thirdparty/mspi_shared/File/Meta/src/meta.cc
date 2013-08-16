//===========================================================================
//                                                                          
//                              Meta                                    
//                                                                          
//===========================================================================
//
//             Copyright 2012, California Institute of Technology.
//                               ALL RIGHTS RESERVED.
//                    U.S. Government Sponsorship acknowledged.
//
//=============================================================================

#include "meta.h" 		// Definition of class
#include <vector> 		// Definition of std::vector

/////////////////////////////////////////////////////////////////////////////
/// Return copy of Meta with the given prefix added to each element name.
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::Meta
MSPI::Shared::Meta::add_prefix(
  const std::string& Prefix
) const
{
  Meta result;
  for (const_it_type i = meta_.begin() ; i != meta_.end() ; ++i) {
    std::string name = Prefix + i->first;
    result.add_any(name, i->second);
  }
  return result;
}

/////////////////////////////////////////////////////////////////////////////
/// Return subset of Meta containing only those elements whose name matches 
/// the given prefix.
/////////////////////////////////////////////////////////////////////////////

MSPI::Shared::Meta
MSPI::Shared::Meta::match_prefix(
  const std::string& Prefix
) const
{
  Meta result;
  for (const_it_type i = meta_.begin() ; i != meta_.end() ; ++i) {
    if (0 == i->first.compare(0,Prefix.size(),Prefix)) {
      result.insert(*i);
    }
  }
  return result;
}

/////////////////////////////////////////////////////////////////////////////
/// Add the given meta data to the current set.  Throw an exception if an
/// element already exists with a different value.
/////////////////////////////////////////////////////////////////////////////

void
MSPI::Shared::Meta::add(
  const Meta& Meta
)
{
  for (const_it_type i = Meta.meta_.begin() ; i != Meta.meta_.end() ; ++i) {
    add_any(i->first, i->second);
  }
}

/////////////////////////////////////////////////////////////////////////////
/// Write current metadata to output stream.
/////////////////////////////////////////////////////////////////////////////

void
MSPI::Shared::Meta::stream_export(
  std::ostream& Out
) const
{
  for (const_it_type i = meta_.begin() ; i != meta_.end() ; ++i) {
    Out << boost::apply_visitor(to_string_visitor(),i->second) 
	<< " : " << i->first << "\n";;
  }
}

/////////////////////////////////////////////////////////////////////////////
/// Write current metadata to output stream.
/////////////////////////////////////////////////////////////////////////////

void
MSPI::Shared::Meta::stream_export2(
  std::ostream& Out
) const
{
  for (const_it_type i = meta_.begin() ; i != meta_.end() ; ++i) {
    Out << i->first << " = " << boost::apply_visitor(to_string_visitor(),i->second) << "\n";;
  }
}
