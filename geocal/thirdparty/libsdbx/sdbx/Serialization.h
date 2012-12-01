/*
 *  SDBX
 *	$Id: Serialization.h 1841 2009-01-04 18:38:58Z virtuald $
 *
 *  Copyright (c) 2008 - 2009 Dustin Spicuzza <dustin@virtualroadside.com>
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

/*
	Macros to make serialization easier
*/

#ifndef SDBX_SERIALIZATION_H
#define SDBX_SERIALIZATION_H

/// \name Macros for intrinsic types
/// @{


/// Generally, this macro is not preferred: but use it for converting types
/// from a buffer in a non-constructor
#define BUFFER_REF(type, offset)			*((type*)(buffer + (offset)))

/// Use this macro to deserialize a type from a buffer in a serialization constructor
#define DESERIALIZE(type, offset, var ) 	var(BUFFER_REF(type, offset))

/// Use this macro to serialize a type into a buffer in a Serialize() call
#define SERIALIZE(type, offset, var ) 		BUFFER_REF(type, offset) = var

/// @}


/// \name Macros for types supporting the generic serialization mechanism types
/// @{


/// Use this macro to deserialize a type from a buffer in a serialization constructor
#define DESERIALIZE_G(type, offset, var) \
	var(record<type>::deserialize(buffer + (offset)))
	
/// Use this macro to serialize a type into a buffer in a Serialize() call
#define SERIALIZE_G(type, offset, var) \
	record<type>::serialize(var, buffer + (offset));

/// @}
	

/**
	\struct record
	\brief Generic serialization class

	Read the notes in SerializedFile.h first

	These allow us to serialize/deserialize anything that we want without 
	knowing whether	its an intrinsic or a user-defined class. There is a 
	template defined, record<T>, that provides three pieces of
	functionality:
	
		record<T>::size						The record_size of the type
		
		record<T>::deserialize(buffer)		Used in an initializer of a 
											constructor
											
		record<T>::serialize(var, buffer)	Used to serialize a type to a
											buffer.
	
	As always, an example is the best way to explain how to use it:
	
		template <typename T1, typename T2>
		struct test_record {
			enum { record_size =	record<T1>::size +
									record<T2>::size};

			test_record(const unsigned char * const buffer) :
				value1(record<T1>::deserialize(buffer)),
				value2(record<T2>::deserialize(buffer + record<T1>::size))
			{}
			
			void Serialize(unsigned char * buffer) const
			{
				record<T1>::serialize(value1, buffer);
				record<T2>::serialize(value2, buffer + record<T1>::size);
			}

			T1 value1;
			T2 value2;
		};
	
	As you can see, its pretty straightforward. The compiler should inline
	the extra function calls when you're using optimizations, so there 
	shouldn't be any extra overhead. However I would anticipate that 
	compiling without optimizations would result in some overhead.

*/


// this struct works with any user defined type that conforms to the
// serialization concept as defined in SerializedFile.h 
//
// This doesn't work with built in types, but there are specializations
// defined below which work for those.
template <class T>
struct record {
	enum { size = T::record_size };
	
	static inline 
	const unsigned char * const deserialize(const unsigned char * const buffer) 
	{ 
		return buffer; 
	}
	
	static inline 
	void serialize(const T &var, unsigned char * buffer) 
	{ 
		var.Serialize(buffer);
	}
};

// specialize for pointers in a braindead way.. but at least it
// will compile
template <class T>
struct record <T *> {
	enum { size = 0 };
	
	static inline 
	T * deserialize(const unsigned char * const buffer) 
	{ 
		assert(0 && "you should not serialize pointers");
		return NULL;
	}
};




// used to define specializations for intrinsic types
#define RECORD_DEFS(T) 										\
template <>													\
struct record<T> {											\
	enum { size = sizeof(T) };								\
															\
	static inline 											\
	T deserialize(const unsigned char * const buffer) 		\
	{ 														\
		return *((T*)buffer);								\
	}														\
															\
	static inline 											\
	void serialize(const T &var, unsigned char *  buffer) 	\
	{														\
		*((T*)buffer) = var;								\
	}														\
};

// this probably takes care of all the intrinsics.. 
RECORD_DEFS(char);
RECORD_DEFS(unsigned char);
RECORD_DEFS(short int);
RECORD_DEFS(unsigned short int);
RECORD_DEFS(int);
RECORD_DEFS(unsigned int);
RECORD_DEFS(long);
RECORD_DEFS(unsigned long);
RECORD_DEFS(long long);
RECORD_DEFS(unsigned long long);


#undef RECORD_DEFS



#endif


