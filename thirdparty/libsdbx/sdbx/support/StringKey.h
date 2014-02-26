/*
 *  SDBX
 *	$Id: StringKey.h 1841 2009-01-04 18:38:58Z virtuald $
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

#ifndef SDBX_STRING_KEY_H
#define SDBX_STRING_KEY_H

#include <iostream>

/**
	\struct StringKey
	\brief A key type to use for a tree that stores a string
	
	This class creates prefix strings of key_length size, that can
	be stored in the  structure as a key. You must specify
	in the template instantiation that you want to allow duplicate
	keys. 
	
	This is intended to be used where the key is this class, and the 
	string itself is value used in the tree (or something referring 
	to the string, such as a file offset)
	
	When querying a tree with these in them, you must check each prefix
	that has the same prefix as the item you're searching for to determine
	if the correct value is present.
*/
template <std::size_t key_length>
struct StringKey 
{
	// not guaranteed to be null terminated: use strncpy
	char keyval[key_length];
	
	/// Serialization parameters
	enum { record_size = key_length	};

	typedef StringKey<key_length>		this_type;
	
	StringKey(){}

	/// constructor
	StringKey(const unsigned char * const buffer)
	{
		strncpy(keyval, (const char *)buffer, key_length);
	}
	
	/// serialization routine
	void Serialize(unsigned char * buffer) const
	{
		strncpy((char *)buffer, keyval, key_length);
	}
	
	bool operator==(const this_type &bsk) const 
	{ 
		return strncmp(keyval, bsk.keyval, key_length) == 0;
	}

	bool operator!=(const this_type &bsk) const 
	{ 
		return strncmp(keyval, bsk.keyval, key_length) != 0;
	}
	
	// this probably isn't correct for sorting, but good enough
	bool operator< (const this_type &bsk) const
	{
		return strncmp(keyval, bsk.keyval, key_length ) < 0;
	}
	
};

// implemented for debugging
template <std::size_t key_length>
inline std::ostream& operator <<(std::ostream &os, const StringKey<key_length> &bsk)
{
	char str[key_length+1];
	strncpy(str, bsk.keyval, key_length);
	str[key_length] = 0;

	os << str;
	return os;
}

#endif

