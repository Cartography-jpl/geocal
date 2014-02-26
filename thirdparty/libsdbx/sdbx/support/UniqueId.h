/*
 *  SDBX
 *	$Id: UniqueId.h 1841 2009-01-04 18:38:58Z virtuald $
 *
 *  Copyright (c) 2008 - 2009 Dustin Spicuzza <dustin@virtualroadside.com>
 *  Copyright (c) 2004 - 2008 Richard L. Lynch <rllynch@users.sourceforge.net>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef UNIQUEID_H
#define UNIQUEID_H

// #include "pstdint.h"
#include <stdint.h>

#define UNIQUEID_USAGE_DEBUG

// useful for trying to detect uninitialized usage of the class.. 
#ifdef UNIQUEID_USAGE_DEBUG
	#define UNIQUEID_DBG(x) x
	#define UNIQUEID_IS_INIT assert(is_init);
	#define UNIQUEID_DBG_CONSTRUCTOR ,is_init(true)
	#define UNIQUEID_DEF_CONSTRUCTOR : is_init(false)
	
#else
	#define UNIQUEID_DBG(x)
	#define UNIQUEID_IS_INIT
	#define UNIQUEID_DBG_CONSTRUCTOR
	#define UNIQUEID_DEF_CONSTRUCTOR
#endif


/**
	\struct UniqueIdT
	\brief Implementation of a generic Unique ID type
	
	mini-rant about unique ids:
	
	First, lets look at the number of objects we can have depending on
	the size of the unique ID:

	32 bits: 4294967296 			(4 billion)
	40 bits: 1099511627776 			(1 trillion)
	48 bits: 281474976710656 		(281 trillion)
	64 bits: 18446744073709551615 	(ridiculously big number)


	32 bits will be good enough for really tiny systems, but I'm pretty sure
	quite a few people will run out of unique ID's with this many nodes. 

	Now we could add another 8 bits, and go to 1 trillion. And for most
	users, that will be far more than enough. Loading a planet.osm file 
	might go over this number. But we could be reasonably sure that it 
	would work for most people.

	If you add another 8 bits, then you get 281 trillion unique ID's. This
	will almost certainly be enough for almost anyone. And if you use this
	many.. then you're going to run into other problems, I suspect. 

	I'm pretty sure that Roadnav is never going	to need	64 bits for ID's. 
	Really. Ever. Thats a really big number.
	
	So we will use a custom type for this unless you want to enable very
	small ID's (not particularly recommended)
	
	You *must* use the generic serialization routines defined in
	Serialization.h if you are serializing these, since it may be an 
	intrinsic type or a custom type. 
*/

// default to 6
#ifndef UNIQUEID_SZ
	#define UNIQUEID_SZ 6
#endif


#if UNIQUEID_SZ == 4

	typedef uint32_t UniqueId;

#else

	template <typename high_type>
	struct UniqueIdT {
		typedef UniqueIdT<high_type> this_type;
		
		uint32_t 	lo;
		high_type	hi;
		UNIQUEID_DBG( bool is_init; )
	
		bool IsNull() const { 
			UNIQUEID_IS_INIT
			return !lo && !hi; 
		}

		// useful operators
		bool operator > (const this_type & x) const { UNIQUEID_IS_INIT return hi == x.hi ? lo > x.lo : hi > x.hi; }
		bool operator < (const this_type & x) const { UNIQUEID_IS_INIT return hi == x.hi ? lo < x.lo : hi < x.hi; }
		bool operator ==(const this_type & x) const { UNIQUEID_IS_INIT return lo == x.lo && hi == x.hi; }
		bool operator !=(const this_type & x) const { UNIQUEID_IS_INIT return lo != x.lo || hi != x.hi; }
		
		// increment
		this_type& operator++ ()
		{	
			UNIQUEID_IS_INIT
			if (!++lo)
				hi++;
			return *this;
		}

		// conversion operator
		operator uint64_t () const
		{
			UNIQUEID_IS_INIT
			uint64_t ret = ((uint64_t) hi) << 32;
			ret |= (uint64_t) lo;
			return ret;
		}

		UniqueIdT() 
			UNIQUEID_DEF_CONSTRUCTOR 
		{}

		UniqueIdT(uint64_t val) :
			lo((uint32_t)val),
			hi((high_type)(val >> 32))
			UNIQUEID_DBG_CONSTRUCTOR
		{}

		UniqueIdT(uint32_t l, high_type h) : 
			lo(l), hi(h) 
			UNIQUEID_DBG_CONSTRUCTOR
		{}
		
		/// serialization constructor
		UniqueIdT(const unsigned char * const buffer) :
			DESERIALIZE(uint32_t, 0, lo),
			DESERIALIZE(high_type, sizeof(uint32_t), hi)
			UNIQUEID_DBG_CONSTRUCTOR
		{}
		
		/// serializer function
		void Serialize(unsigned char * buffer) const
		{
			UNIQUEID_IS_INIT
			SERIALIZE(uint32_t, 0, lo);
			SERIALIZE(high_type, sizeof(uint32_t), hi);
		}
		
		/// serialization typedef
		enum { record_size = sizeof(uint32_t) + sizeof(high_type) };
	};
	
	
	#if UNIQUEID_SZ == 5
		typedef UniqueIdT<uint8_t> UniqueId;
	#elif UNIQUEID_SZ == 6
		typedef UniqueIdT<uint16_t> UniqueId;
	#elif UNIQUEID_SZ == 8
		//typedef UniqueIdT<uint32_t> UniqueId;
		typedef uint64_t			UniqueId;
	#else
		#error "Undefined magic unique id size"
	#endif

#endif

#undef UNIQUEID_DBG
#undef UNIQUEID_IS_INIT
#undef UNIQUEID_DBG_CONSTRUCTOR
#undef UNIQUEID_DEF_CONSTRUCTOR

#endif
