#ifndef BOOST_ARCHIVE_BASIC_OARCHIVE_HPP
#define BOOST_ARCHIVE_BASIC_OARCHIVE_HPP

// MS compatible compilers support #pragma once
#if defined(_MSC_VER)
# pragma once
#endif

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8
// basic_oarchive.hpp:

// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com . 
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org for updates, documentation, and revision history.

#include <cstddef> // NULL
#include <boost/config.hpp>
#include <boost/noncopyable.hpp>

// can't use this - much as I'd like to as borland doesn't support it
// #include <boost/scoped_ptr.hpp>

#include <boost/archive/basic_archive.hpp>
#include <boost/serialization/tracking_enum.hpp>
#include <boost/archive/detail/helper_collection.hpp>
#include <boost/archive/detail/abi_prefix.hpp> // must be the last header

namespace boost {
namespace serialization {
    class extended_type_info;
} // namespace serialization

namespace archive {
namespace detail {

class basic_oarchive_impl;
class BOOST_ARCHIVE_DECL(BOOST_PP_EMPTY()) basic_oserializer;
class BOOST_ARCHIVE_DECL(BOOST_PP_EMPTY()) basic_pointer_oserializer;

//////////////////////////////////////////////////////////////////////
// class basic_oarchive - write serialized objects to an output stream
class BOOST_ARCHIVE_OR_WARCHIVE_DECL(BOOST_PP_EMPTY()) basic_oarchive :
    private boost::noncopyable,
    public boost::archive::detail::helper_collection
{
    friend class basic_oarchive_impl;
    // hide implementation of this class to minimize header conclusion
    // in client code. note: borland can't use scoped_ptr
    //boost::scoped_ptr<basic_oarchive_impl> pimpl;
    basic_oarchive_impl * pimpl;

    // overload these to bracket object attributes. Used to implement
    // xml archives
    virtual void vsave(const version_type t) =  0;
    virtual void vsave(const object_id_type t) =  0;
    virtual void vsave(const object_reference_type t) =  0;
    virtual void vsave(const class_id_type t) =  0;
    virtual void vsave(const class_id_optional_type t) = 0;
    virtual void vsave(const class_id_reference_type t) =  0;
    virtual void vsave(const class_name_type & t) = 0;
    virtual void vsave(const tracking_type t) = 0;
protected:
    basic_oarchive(unsigned int flags = 0);
    boost::archive::detail::helper_collection &
    get_helper_collection(){
        return *this;
    }
    // account for bogus gcc warning
    #if defined(__GNUC__)
    virtual
    #endif
    ~basic_oarchive();
public:
    // note: NOT part of the public interface
    void register_basic_serializer(
        const basic_oserializer & bos
    );
    void save_object(
        const void *x, 
        const basic_oserializer & bos
    );
    void save_pointer(
        const void * t, 
        const basic_pointer_oserializer * bpos_ptr
    );
    void save_null_pointer(){
        vsave(NULL_POINTER_TAG);
    }
    // real public interface starts here
    void end_preamble(); // default implementation does nothing
    library_version_type get_library_version() const;
    unsigned int get_flags() const;
};

} // namespace detail
} // namespace archive
} // namespace boost

#include <boost/archive/detail/abi_suffix.hpp> // pops abi_suffix.hpp pragmas

#endif //BOOST_ARCHIVE_BASIC_OARCHIVE_HPP
