/*
 *  SDBX
 *  $Id: lru.h 1841 2009-01-04 18:38:58Z virtuald $
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

#ifndef SDBX_LRU_H
#define SDBX_LRU_H

#include <algorithm>
#include <vector>
#include <functional>
#include <cassert>

#include "support/utility.h"

#ifdef USE_MSVC_MEMORY_TRACING
	// this enables advanced memory leak detection
	#include <wx/msw/msvcrt.h>
#endif


#ifdef LRUCACHE_DEBUG
	#define RECORD_HIT m_hits += 1;
	#define RECORD_MISS m_misses += 1;
	#define LRUCACHE_DEBUG_INIT m_hits(0), m_misses(0),
#else
	#define RECORD_HIT
	#define RECORD_MISS
	#define LRUCACHE_DEBUG_INIT
#endif

enum LRUCacheDeleteType {
	LRUDeleteNone,
	LRUDeleteObject,
	LRUDeleteArray,
	LRUFreeMalloc
};


/**	
	\struct LRUCacheNode
	\brief Struct that stores the information for the LRU cache nodes
	
	See notes about the LRUCache
*/
template <
	typename CachedObjectType, 
	typename IdType, 
	LRUCacheDeleteType deleteType
>
struct LRUCacheNode {

	typedef LRUCacheNode<CachedObjectType, IdType, deleteType> _Self;
	
	explicit LRUCacheNode(const IdType &i, CachedObjectType * o, _Self * n)
		: id(i), object(o), next(n), prev(NULL) {}

	IdType id;
	CachedObjectType * object;
			
	// used to create a linked list of items (for LRU functionality)
	_Self * next;
	_Self * prev;
};

/// specialized version: delete
template <typename CachedObjectType, typename IdType>
struct LRUCacheNode<CachedObjectType, IdType, LRUDeleteObject> {

	typedef LRUCacheNode<CachedObjectType, IdType, LRUDeleteObject> _Self;
	
	explicit LRUCacheNode(const IdType &i, CachedObjectType * o, _Self * n)
		: id(i), object(o), next(n), prev(NULL) {}

	~LRUCacheNode() { if (object) delete object; }

	IdType id;
	CachedObjectType * object;
			
	// used to create a linked list of items (for LRU functionality)
	_Self * next;
	_Self * prev;
};

/// specialized version: delete []
template <typename CachedObjectType, typename IdType>
struct LRUCacheNode<CachedObjectType, IdType, LRUDeleteArray> {

	typedef LRUCacheNode<CachedObjectType, IdType, LRUDeleteArray> _Self;
	
	explicit LRUCacheNode(const IdType &i, CachedObjectType * o, _Self * n)
		: id(i), object(o), next(n), prev(NULL) {}

	~LRUCacheNode() { if (object) delete [] object; }

	IdType id;
	CachedObjectType * object;
			
	// used to create a linked list of items (for LRU functionality)
	_Self * next;
	_Self * prev;
};

/// specialized version: free()
template <typename CachedObjectType, typename IdType>
struct LRUCacheNode<CachedObjectType, IdType, LRUFreeMalloc> {

	typedef LRUCacheNode<CachedObjectType, IdType, LRUFreeMalloc> _Self;
	
	explicit LRUCacheNode(const IdType &i, CachedObjectType * o, _Self * n)
		: id(i), object(o), next(n), prev(NULL) {}

	~LRUCacheNode() { if (object) free(object); }

	IdType id;
	CachedObjectType * object;
			
	// used to create a linked list of items (for LRU functionality)
	_Self * next;
	_Self * prev;
};


/// Used for sorting purposes
template <typename _LRUCacheNode>
struct LRUCacheNodeSorter : public std::binary_function<_LRUCacheNode, _LRUCacheNode, bool>
{
	bool operator()(const _LRUCacheNode * const n1, const _LRUCacheNode * const n2)
	{
		return n1->id < n2->id;
	}
};



/**
	\class LRUCache
	\brief Implements a relatively simple and low-overhead LRU-style 
	caching mechanism

	A key thing here is to remember that when you pass this thing an 
	object, it *owns* that object (if you set deleteObject to true). 
	So once it falls over the edge of the LRU list, then it is no 
	longer valid. So basically, anytime	that you need to use an item 
	stored in here, you must query for it. 
	
	Is this a good idea? That sounds like its asking for trouble. However,
	I can't think of a good reason why you would keep references for more
	than one object anyways... 
	
	Ok, I can think of a problem case: recursion. If you call something
	that uses this, which uses this... and one of them falls off the end
	of the list, then you're screwed.
	
	If LRUCACHE_DEBUG == true, then this will keep statistics about hits
	and misses. 
	
	@todo Add locks or something to that effect.. 
	
*/
template <
	typename CachedObjectType, 
	typename IdType, 
	LRUCacheDeleteType deleteObject
>
class LRUCache {
public:
	
	typedef std::size_t 											size_type;
	typedef LRUCacheNode<CachedObjectType, IdType, deleteObject> 	Node;
	typedef std::vector< Node *>									NodeArray;
	typedef typename NodeArray::iterator							NodeIterator;
	typedef LRUCacheNodeSorter<Node>								NodeSorter;
	
	
	explicit LRUCache(const size_type &max_nodes ) 
		: LRUCACHE_DEBUG_INIT
		m_first(NULL), m_last(NULL), m_max_nodes(max_nodes)
		
	{
		assert(max_nodes > 2);
	
		// +1 is needed for the insert, so that we never need to reallocate
		m_nodes.reserve(max_nodes + 1);
	}
	
	~LRUCache()
	{
		NodeIterator i = m_nodes.begin();
		const NodeIterator end = m_nodes.end();
		for (;i != end; i++)
			delete *i;
	}
	
	/**
		\brief Gets the item, or returns NULL
	*/
	CachedObjectType * get(const IdType &id)
	{	
		// quick optimization: see if the most recently used object is the
		// one that we're looking for
		if (m_first && m_first->id == id)
		{
			RECORD_HIT
			return m_first->object;
		}
	
		// create temporary search object
		Node tmp(id, NULL, NULL);
	
		// otherwise, search for the item (sorted search, log n)
		NodeIterator iter = std::lower_bound(m_nodes.begin(), m_nodes.end(), &tmp, NodeSorter());
			
		
		if (iter != m_nodes.end() && (*iter)->id == id)
		{
			Node * node = *iter;
			
			// ensure the node is set up properly
			assert(node == m_first || node->prev);
			assert(node == m_last  || node->next);
		
			// if we found it, move to the beginning of the LRU list
			if (node != m_first)
			{
				// fix up the last ptr if needed
				if (node == m_last)
					m_last = node->prev;
				else
					node->next->prev = node->prev;
				
				node->prev->next = node->next;
				
				// fix up self
				node->prev = NULL;
				node->next = m_first;
				
				// fix up next guy
				if (m_first)
					m_first->prev = node;
				m_first = node;
			}
			
			RECORD_HIT
			return node->object;
		}
		
		RECORD_MISS
		return NULL;
	}
	
	/**
		\brief Adds an item to the list. Use get() first to determine whether
		the object actually lives in the cache or not
		
		@return a node object, you should delete it after this call
		returns. This contains the item and its id.
	*/
	Node * add(const IdType &id, CachedObjectType * object) WARN_IF_UNUSED
	{
		Node * ret = NULL;
	
		m_first = new Node(id, object, m_first);
		
		if (m_nodes.empty())
		{
			m_last = m_first;
			m_nodes.push_back(m_first);
			return NULL;
		}
		
		// double-linked list
		m_first->next->prev = m_first;
		
		// if there are too many nodes, then delete one
		if (m_nodes.size() == m_max_nodes)
		{
			assert(m_last);
			
			// remove the item from the end of the LRU list
			m_nodes.erase(find ( m_nodes.begin(), m_nodes.end(), m_last ));
			
			// switch them out
			ret = m_last;
	
			m_last = ret->prev;
			m_last->next = NULL;
		}
			
		m_nodes.insert(std::upper_bound(m_nodes.begin(), m_nodes.end(), m_first, NodeSorter()), m_first);
		return ret;
	}
	
	/**
		\brief Removes an arbitrary CacheNode
		
		This is useful for clearing the cache: you remove items one by
		one and if the item needs to be written back to disk, you do so.
		
		@return A node object, you should delete it after this call
		returns. This contains the item and its id.
	*/
	Node * removeOne() WARN_IF_UNUSED
	{
		if (m_nodes.empty())
			return NULL;
			
		Node * node = m_nodes.back();
		m_nodes.pop_back();
		
		return node;
	}
	
#ifdef LRUCACHE_DEBUG
	/// number of hits on the cache
	size_type m_hits;
	
	/// number of misses on the cache
	size_type m_misses;
#endif

private:

	// this needs to have two lists:
	//  - a sorted list that allows fast lookup by ID
	//  - a list that is ordered by least recently used
	//
	// so... as a compromise, we have a sorted list, with an embedded
	// linked list created from the nodes

	// this is sorted by ID
	NodeArray m_nodes;
	
	// this is the LRU list
	Node * m_first;
	Node * m_last;
	
	size_type m_max_nodes;
		
};


#undef RECORD_HIT
#undef RECORD_MISS
#undef LRUCACHE_DEBUG_INIT


#endif


