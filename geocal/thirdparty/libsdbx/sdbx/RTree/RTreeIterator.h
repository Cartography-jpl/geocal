/*
 *  SDBX
 *	$Id: RTreeIterator.h 1845 2009-04-01 06:04:06Z virtuald $
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



#ifndef SDBX_RTREE_ITERATOR_H
#define SDBX_RTREE_ITERATOR_H

#ifdef USE_MSVC_MEMORY_TRACING
	// this enables advanced memory leak detection
	#include <wx/msw/msvcrt.h>
#endif

/**
	\class RTreeIterator

	This defines an iterator for the R Tree. It is *not* an STL iterator,
	since the design constraints for that require double-linked lists (or lots
	of overhead), and we aren't doing it that way.

	@warning At the moment, you can only use one iterator on a tree at a time.
	Usage of another iterator may invalidate the previous iterator. 
	
	@todo Make this more like the B-Tree traverser?
	
	Finding a specific item:
	
		it = tree->Find(key);
		if (it && !it->end())
		{
			LeafNode & item = it->current();
			if (item->key == key)
				// do stuff here
		}
	
	Iterating all items:
 
		it = tree->Begin();
		
		if (it)
		{
			do
			{
				LeafItem & item = it->current();
				
			} while (it->next());
		}
		
	Iterating a range:
	
		it = tree->Find(start_key);
		
		if (it && !it->end())
		{
			do
			{
				LeafItem & item = it->current();
				
				if (item.key > end_key)
					break;
				
			} while (it->next());
		}
*/
template <typename _Tree, typename _Acceptor>
class RTreeIterator {
public:

	// import various definitions from the tree

	typedef typename _Tree::KeyType				KeyType;
	typedef typename _Tree::IdType				IdType;
	
	typedef typename _Tree::Node				Node;
	typedef typename _Tree::NodeItem			NodeItem;
	typedef typename _Tree::NodeItemIterator	NodeItemIterator;
	typedef typename _Tree::LeafNode			LeafNode;
	typedef typename _Tree::LeafItem			LeafItem;
	typedef typename _Tree::LeafItemIterator 	LeafItemIterator;
	
private:

	const _Acceptor accept;

	std::vector<IdType>			id;
	std::vector<std::size_t>	idx;
	
	// cached leaf
	LeafNode *					leaf;
	
	// pointer to the tree
	_Tree * 					m_tree;
	
	RTreeIterator();
	RTreeIterator(const RTreeIterator&);
	void operator=(const RTreeIterator&);
	
public:

	RTreeIterator(_Tree * tree, const _Acceptor &a) :
		accept(a),
		m_tree(tree)
	{
		if (m_tree->backend.m_treeHeight > 0)
		{
			// reduce allocations
			id.reserve(m_tree->backend.m_treeHeight);
			idx.reserve(m_tree->backend.m_treeHeight);
			
			id.push_back(m_tree->backend.m_rootId);
			idx.push_back(0);
			
			// go to the first leaf that matches the acceptor
			traverse();
		}
	}
	
	bool end() const
	{
		return id.empty();
	}
	
	// grabs the current leaf the iterator is pointing to
	// @warning You should *never* modify the returned key, but
	// if you modify the value then you must call set_dirty()
	LeafItem & current()
	{
		return leaf->items[idx.back()];
	}
	
	const LeafItem & current() const
	{
		return leaf->items[idx.back()];
	}
	
	void set_dirty()
	{
		leaf->is_dirty = true;
	}
	
	// traverses to the next element
	bool next()
	{
		if (id.empty() || !leaf)
			return false;
	
		// the current leaf should be cached
		while (++idx.back() < leaf->items.size())
			if (accept(leaf->items[idx.back()]))
				return true;
		
		// otherwise, pop up a level
		id.pop_back();
		idx.pop_back();
	
		// and try to find the next node
		return traverse();
	}
	
private:
	
	// traverses to the bottom of the to get the next requested element
	bool traverse()
	{
		while (!id.empty())
		{
			if (id.size() >= m_tree->backend.m_treeHeight)
			{
				leaf = m_tree->backend.get_leaf_node(id.back());
				if (!leaf) return false;

				// find an acceptable leaf
				while (idx.back() < leaf->items.size())
					if (accept(leaf->items[idx.back()]))
						return true;
					else
						++idx.back();
			}
			else
			{
				const Node * node = m_tree->backend.get_node(id.back());
				if (!node) return false;
				
				// find an acceptable leaf
				while (idx.back() < node->items.size())
					if (accept(node->items[idx.back()++]))
					{
						id.push_back(node->items[idx.back()-1].id);
						idx.push_back(0);
						break;
					}

				// go to next level if we haven't reached the end
				if (idx.back() < node->items.size())
					continue;
			}
		
			// pop up a level
			id.pop_back();
			idx.pop_back();
		}
		
		return false;
	}
	
	
	typedef typename _Tree::BoundingBox 	BoundingBox;
	
	// this doesn't belong here really, but its the best place for it
	bool ExtendStackBounds(const BoundingBox &bound)
	{
		// expand the actual item
		if (!leaf->items[idx.back()].key.stretch(bound))
		{
			leaf->is_dirty = true;
			return true;
		}
	
	
		// start from the bottom, and expand the bounds of
		// every row that we hold
		std::size_t i = id.size();
		
		while (--i)
		{
			Node * node = m_tree->backend.get_node(id[i]);
			if (!node)
				return false;
			
			if (!node->items[idx[i]].key.stretch(bound))
			{
				node->is_dirty = true;
				return true;
			}
		}
	
		return true;
	}
	
};

#endif
