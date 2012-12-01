/*
 *  SDBX
 *	$Id: StringTable.h 1841 2009-01-04 18:38:58Z virtuald $
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

#ifndef SDBX_STRINGTABLE_H
#define SDBX_STRINGTABLE_H

#include <wx/ffile.h>

#include "lru.h"
#include "DiskTreeBackend.h"
#include "BTree.h"

#include "support/StringKey.h"

#ifndef _MSC_VER
	// stupid microsoft... 
	#define _strdup strdup
#endif

#ifdef USE_WX_MSVC_MEMORY_TRACING
	// this enables advanced memory leak detection
	#include <wx/msw/msvcrt.h>
#endif

#define SET_BAD_ERROR( code )	\
	m_lastError = code;				\
	assert(0 && #code)

#define STRINGTABLE_MAGIC "STRINGTABLE_01"
	

/**
	\brief This utilizes a BTree structure to store a table of strings
	in (mostly) sorted order. The strings are cached for quick access, but
	only some of them. 
	
	There are two index files created, *.ldx and *.ndx, and then a 
	user-specified filename to actually store the files in. This file stores
	the actual strings, in the following format:
	
		[byte] 				Length (0-127)
		[optional byte]		If the high bit is set on the first byte, then the
							next byte is the low byte of a 2-byte word, and the
							first byte is the high byte & 0x7F. For strings 
							longer than 127 characters
		[string of bytes]	The actual string, not null-terminated

	@tparam _key_length			Length of the key to be stored
	@tparam max_child_items		Number of items the tree should store in a node

	@note Maximum string size is fixed at 2^15-1

	@todo Fix it so that its completely ordered. The problem is that the
	tree cannot sort on the value, only on the keys (which have duplicates
	due to only storing a portion of the string)
*/
template <std::size_t _key_length, std::size_t max_child_items>
class StringTable {
public:

	enum { key_length = _key_length };

	typedef StringKey<key_length>						KeyType;
	typedef UniqueId									IdType;

	typedef BTree< 
		KeyType, 
		IdType, 
		max_child_items, 
		DiskTreeBackend, 
		true>											Tree;

	typedef typename Tree::size_type					size_type;
	typedef typename Tree::Iterator						Iterator;

	typedef LRUCache<char, UniqueId, LRUFreeMalloc>		Cache;

	enum ErrorCode {
		ERR_NONE = 0,
		ERR_INIT_BACKEND = 500,
		ERR_INIT_CACHE,
		ERR_WRITE_HEADER,
		ERR_READ_HEADER,
		ERR_OPEN_FILE,
		ERR_BAD_SEEK,
		ERR_BAD_WRITE,
		ERR_BAD_READ
	};

	StringTable() :
		m_cache(NULL),
		m_traverser(NULL),
		m_initialized(false),
		m_dirty(false)
	{}

	~StringTable()
	{
		DELETE_IF_NOT_NULL(m_cache);
		DELETE_IF_NOT_NULL(m_traverser);
	}

	// returns an iterator to the tree.. generally, you shouldn't
	// need to use this. The tree stores keys as truncated strings
	// and values as the ID of the string
	Iterator * Begin()
	{
		return m_idx.Begin();
	}

	/**
		@return Either the error code of the string table operation, 
				or the error code of the backend operation
	*/
	ErrorCode GetErrorCode() const 
	{
		if (m_lastError != ERR_NONE)
			return m_lastError;
		return (ErrorCode)m_idx.GetErrorCode();
	}

	//! Gets the number of strings contained in this table
	size_type GetItemCount() const
	{
		return m_idx.GetItemCount();
	}

	//! ensures that the index isn't corrupted in a basic way
	bool VerifyIndex() 
	{
		return m_idx.Verify();
	}
	
	
	/**
		\brief Initialization
		
		@param filename				File to store strings in
		@param node_cache_sz		Node cache size
		@param string_cache_sz		Leaf/String Cache size
	*/
	bool Initialize(
		const wxFileName &filename, 
		std::size_t node_cache_sz = 500,
		std::size_t string_cache_sz = 10000)
		WARN_IF_UNUSED
	{
		assert(!m_initialized);
		m_lastError = ERR_NONE;

		DiskTreeBackendArgs args(filename, wxT(STRINGTABLE_MAGIC), node_cache_sz, string_cache_sz);

		if (!m_idx.Initialize(args))
		{
			SET_BAD_ERROR(ERR_INIT_BACKEND);
			return false;
		}

		// create a traverser instance
		m_traverser = m_idx.GetTraverser();
		
		// create the LRU
		m_cache = new Cache( string_cache_sz );
		if (!m_cache)
		{
			SET_BAD_ERROR(ERR_INIT_CACHE);
			return false;
		}
		
		// create a new file if it doesn't exist, otherwise open existing file
		if (!wxFileExists(filename.GetFullPath()))
			m_file.Open(filename.GetFullPath(), wxT("wb+"));
		else
			m_file.Open(filename.GetFullPath(), wxT("rb+"));
		
		if (!m_file.IsOpened())
		{
			SET_BAD_ERROR(ERR_OPEN_FILE);
			return false;
		}
		
		m_next_new_id = m_file.Length();

		if (m_next_new_id == 0)
		{
			// new file, write the headers
			if (m_file.Write(STRINGTABLE_MAGIC, sizeof(STRINGTABLE_MAGIC)-1) != sizeof(STRINGTABLE_MAGIC)-1)
			{
				SET_BAD_ERROR( ERR_WRITE_HEADER );
				return false;
			}

			m_file.Flush();
			m_next_new_id = sizeof(STRINGTABLE_MAGIC)-1;
		}
		else
		{
			// ok, ensure that the thing we're loading is what we expect, or
			// if theres a mismatch
			char buffer[sizeof(STRINGTABLE_MAGIC)-1];
			if (m_file.Read(buffer, sizeof(STRINGTABLE_MAGIC)-1) != sizeof(STRINGTABLE_MAGIC)-1 ||
				strncmp(buffer, STRINGTABLE_MAGIC, sizeof(STRINGTABLE_MAGIC)-1))
			{
				SET_BAD_ERROR( ERR_READ_HEADER );
				return false;
			}
		}
		
		
		m_initialized = true;
		return true;
	}
	
	/**
		\brief Find the id of a string

		@param str	The string to find
		@param id	(out) the id of the string, if found

		@return true if string found, false if string not found or error
	*/
	bool FindStringId(const char * str, IdType &id) WARN_IF_UNUSED
	{
		KeyType key((const unsigned char *)str);
		m_lastError = ERR_NONE;
		
		if (m_traverser->find(key))
		{
			do
			{
				const char * found = GetStringById(m_traverser->current().value);
				if (!found) return false;

				// did we find it?
				if (!strcmp(found, str))
				{
					id = m_traverser->current().value;
					return true;
				}
			
				// keep on going until we can't
				if(!m_traverser->next())
					break;

			} while (key == m_traverser->current().key);
		}
		
		return false;
	}
	
	/**
		\brief Retrieves a string, given its ID

		@warning This does not detect invalid ID's particularly well

		@param id	Id of the string to find

		@return		NULL if string not found, or the string if it is 
					found. You should not store this string, it will 
					be deleted automatically.
	*/
	const char * GetStringById(const IdType &id)
	{
		// see if it exists in the LRU cache
		const char * str = m_cache->get(id);

		m_lastError = ERR_NONE;

		if (str)
			// ok, return that string
			return str;
	
		// otherwise read it from the file
		if (m_dirty)
		{
			m_file.Flush();
			m_dirty = false;
		}

		if (!m_file.Seek(id))
		{
			SET_BAD_ERROR(ERR_BAD_SEEK);
			return NULL;
		}

		// read by offset into buffer
		unsigned char buffer[1024];
		
		unsigned bytes_read = m_file.Read(buffer, 1024);
		
		unsigned len = buffer[0];
		unsigned offset = (buffer[0] & 0x80) ? 2 : 1;
		
		if (offset == 2)
			len = ((len << 8) | buffer[1]) & 0x7FFF;
		
		// verify the length
		if (bytes_read < 2 || bytes_read < offset + len)
		{
			SET_BAD_ERROR(ERR_BAD_READ);
			return NULL;
		}

		// allocate something big enough
		char * s = (char*)malloc(len+1);

		// copy the string to its new home in the cache if its small enough
		if (len < 1024-3)
		{
			strncpy(s, (char*)buffer+offset, len);
		}
		else
		{
			if (!m_file.Seek(id+offset))
			{
				SET_BAD_ERROR(ERR_BAD_SEEK);
				return NULL;
			}

			if (m_file.Read(s, len) != len)
			{
				SET_BAD_ERROR(ERR_BAD_READ);
				return NULL;
			}
		}

		// null-terminate it
		s[len] = '\0';

		Cache::Node * node = m_cache->add( id, s );
		if (node)
			delete node;
		
		return s;
	}
	
	/**
		\brief Inserts a new string into the table

		If there is a duplicate string, this will return the id for that
		string instead.

		@param str	String to insert
		@param Id	(out) returns the id of the inserter string

		@return		true if string inserted/found
	*/
	bool InsertString(const char * str, IdType &id) WARN_IF_UNUSED
	{
		/// @todo this is very inefficient, it can be done better
		if (FindStringId(str, id))
			return true;

		if (m_lastError != ERR_NONE)
			return false;

		const std::size_t _len = strlen(str);
		const uint16_t len = _len > 32767 ? 32767 : _len;

		// get the offset, call that the id
		if (!m_file.Seek(m_next_new_id))
		{
			SET_BAD_ERROR(ERR_BAD_SEEK);
			return false;
		}

		id = m_next_new_id;

		// insert it into the index
		if (!m_idx.Insert( KeyType((const unsigned char*)str), id))
			return false;
			
		unsigned char encoded_len[2];
		
		if (len < 128)
		{
			// one byte
			encoded_len[0] = (unsigned char)len;
			
			if (m_file.Write(encoded_len, 1) != 1)
			{
				SET_BAD_ERROR(ERR_BAD_WRITE);
				return false;
			}

			m_next_new_id += (len + 1);
		}
		else
		{
			// two bytes
			encoded_len[1] = (unsigned char)len;
			encoded_len[0] = (unsigned char)(len >> 8) | 0x80;
			
			if (m_file.Write(encoded_len, 2) != 2)
			{
				SET_BAD_ERROR(ERR_BAD_WRITE);
				return false;
			}

			m_next_new_id += (len + 2);
		}
		
		// write the string
		if (m_file.Write( str, len ) != len)
		{
			/// @todo need a way to roll this stuff back
			SET_BAD_ERROR(ERR_BAD_WRITE);
			return false;
		}
		
		// insert into the LRU
		Cache::Node * node = m_cache->add( id, _strdup(str) );
		if (node) delete node;
		
		m_dirty = true;
		

		return true;
	}
	
private:

	wxFFile m_file;
	Cache * m_cache;
	
	Tree m_idx;
	typename Tree::Traverser * m_traverser;

	uint64_t m_next_new_id;

	bool m_initialized;
	bool m_dirty;
	ErrorCode m_lastError;
};

#undef SET_BAD_ERROR

#endif

