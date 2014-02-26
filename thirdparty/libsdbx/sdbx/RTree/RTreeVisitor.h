/*
 *  SDBX
 *	$Id: RTreeVisitor.h 1844 2009-03-31 14:19:13Z virtuald $
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
 *
 *
 *	Do not include this file directly, include <sdbx/RTree.h> instead.
 */

#ifndef SDBX_RTREE_VISITOR_H
#define SDBX_RTREE_VISITOR_H


#ifdef USE_MSVC_MEMORY_TRACING
	// this enables advanced memory leak detection
	#include <wx/msw/msvcrt.h>
#endif


/********************************************************************
 * These are all 'acceptor' functors used for queries, and they have 
 * the following characteristics:
 *
 * template<typename Item>
 *
 *	bool operator()(const Item &item, bool isLeaf)
 *		-- returns true if this branch (or if isLeaf is true, the leaf)
 * 		   should be visited
 *
 * This class of functions should be easy to copy, and are expected 
 * to be const. They are only used to determine whether something i
 * 
 ********************************************************************/

//! returns true if the Item overlaps the specified bound
template <typename NodeItem, typename LeafItem>
struct RTreeAcceptOverlapping
{
	const typename NodeItem::KeyType &m_bound;
	explicit RTreeAcceptOverlapping(const typename NodeItem::KeyType &bound) : m_bound(bound) {
		assert(bound.verify());
	}
	
	bool operator()(const NodeItem &item) const 
	{ 
		return m_bound.overlaps(item.key);
	}
	
	bool operator()(const LeafItem &item) const 
	{ 
		return m_bound.overlaps(item.key);
	}
	
	private: RTreeAcceptOverlapping(){}
};


//! returns true if the compared boundary is within the specified bound
template <typename NodeItem, typename LeafItem>
struct RTreeAcceptEnclosing
{
	const typename NodeItem::KeyType &m_bound;
	explicit RTreeAcceptEnclosing(const typename NodeItem::KeyType &bound) : m_bound(bound) 
	{
		assert(bound.verify());
	}
	
	bool operator()(const NodeItem &item) const 
	{ 
		return m_bound.overlaps(item.key);
	}
	
	bool operator()(const LeafItem &item) const 
	{ 
		return m_bound.encloses(item.key);
	}
	
	private: RTreeAcceptEnclosing(){}
};


//! will always return true, no matter what -- used to iterate over 
//! the entire set of data
template <typename NodeItem, typename LeafItem>
struct RTreeAcceptAny
{
	bool operator()(const NodeItem &item) const  { return true; }
	bool operator()(const LeafItem &item) const  { return true;	}
};
 
 
#endif
