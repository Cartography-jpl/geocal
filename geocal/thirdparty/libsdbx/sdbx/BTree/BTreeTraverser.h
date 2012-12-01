/*
 *  SDBX
 *	$Id: BTreeTraverser.h 1841 2009-01-04 18:38:58Z virtuald $
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
 *	Do not include this file directly, include <sdbx/BTree.h> instead.
 */



#ifndef SDBX_BTREE_ITERATOR_H
#define SDBX_BTREE_ITERATOR_H

#ifdef USE_MSVC_MEMORY_TRACING
	// this enables advanced memory leak detection
	#include <wx/msw/msvcrt.h>
#endif

#ifdef BTREE_TRAVERSER_DEBUG
	#define BDEBUG(x) x
	#define BDEBUG_INIT ,m_resets(0)
#else
	#define BDEBUG(x)
	#define BDEBUG_INIT
#endif



/**
	\class BTreeTraverser

	This defines a traverser/iterator for the B+ Tree. While it is similar, 
	it is *not* an STL iterator, since the design constraints for that 
	require double-linked lists (or lots of overhead), and we aren't doing 
	it that way.

	If any of these functions cause an error, then you can check the main
	tree to see what the error was.

	@warning At the moment, you can only use one iterator on a tree at a time.
	Usage of another iterator may invalidate the previous iterator. 
	
	@todo Fix that

	Finding a specific item:
	
		it = tree->GetTraverser();
		
		if (it && it->find(key))
		{
			LeafNode & item = it->current();
			
			// do stuff here
		}
	
	Iterating all items:
 
		it = tree->Begin();
		
		if (it && !it->end())
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
template <typename _Tree>
class BTreeTraverser {
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

	typedef typename _Tree::Backend::ErrorCode	ErrorCode;
	
private:

	std::vector<IdType>			id;
	std::vector<std::size_t>	idx;
	
	// cached leaf
	LeafNode *					leaf;
	
	// pointer to the tree
	
	
	BTreeTraverser();
	BTreeTraverser(const BTreeTraverser&);
	void operator=(const BTreeTraverser&);

public:

	_Tree * 					m_tree;

	// performance testing variable
	BDEBUG( std::size_t	m_resets; )

	BTreeTraverser(_Tree * tree) :
		m_tree(tree)
		BDEBUG_INIT
	{
		reset();
	}


	/**
		\brief grabs the current leaf that this is pointing to
		@warning You should *never* modify the returned key, but
		if you modify the value then you must call set_dirty()
	*/
	LeafItem & current()
	{
		return leaf->items[idx.back()];
	}
	
	//! const accessor
	const LeafItem & current() const
	{
		return leaf->items[idx.back()];
	}
	
	void set_dirty()
	{
		leaf->is_dirty = true;
	}
	
	//! this function traverses to the very first node (lowest value)
	bool begin() WARN_IF_UNUSED
	{
		if (m_tree->backend.m_treeHeight == 0)
			return false;
	
		reset();
	
		while (id.size() < m_tree->backend.m_treeHeight)
		{
			// grab the node
			Node * node = m_tree->backend.get_node(id.back());
			if (!node) return false;

			// grab the first item
			id.push_back(node->items[0].id);
			idx.push_back(0);
		}
		
		leaf = m_tree->backend.get_leaf_node(id.back());
		if (!leaf) return false;

		idx.push_back(0);

		return true;
	}
	
	//! returns true if the traverser is finished traversing
	bool end() const
	{
		return id.empty();
	}
	
	
	/**
		\brief Goes to the next entry
		
		This call will fail unless the Traverser is currently
		at a leaf node. This means that you have either  
		
			- called begin()
			- Created the traverser with go_to_start set to true
			- called find() and found an object
			
		Additionally, you need to ensure that no inserts occur while
		you are traversing using begin/next/end, otherwise you will
		get incorrect results. find() does not suffer from this, but
		using next in conjunction with find will cause errors if an
		insert occurs.
	*/
	bool next() WARN_IF_UNUSED
	{
		m_tree->m_lastError = _Tree::ERR_NONE;

		if (id.empty() || !leaf)
			return false;
	
		// the current leaf should be cached
		if (++idx.back() < leaf->items.size())
			return true;
		
		// otherwise, pop up a level
		id.pop_back();
		idx.pop_back();
		if (!idx.empty()) ++idx.back();
	
		// and try to find the next node
		while (!id.empty())
		{
			// found the next leaf
			if (id.size() >= m_tree->backend.m_treeHeight)
			{
				leaf = m_tree->backend.get_leaf_node(id.back());
				if (!leaf) return false;

				idx.back() = 0;
				return true;
			}
			
			const Node * node = m_tree->backend.get_node(id.back());
			if (!node) return false;

			const std::size_t cur_idx = idx.back();

			if (cur_idx <= node->items.size())
			{
				if (cur_idx == node->items.size())
					// traverse down the last id in our set
					id.push_back(node->end_id);
				else
					// otherwise iterate over all nodes
					id.push_back(node->items[cur_idx].id);
				
				idx.push_back(0);
				continue;
			}
		
			// pop up a level
			id.pop_back();
			idx.pop_back();
			if (!idx.empty()) ++idx.back();
		}
		
		return false;
	}
	
	/**
		\brief finds an item 
		
		This caches its current position, so it doesn't need to traverse the
		height of the tree each time that it searches for an item -- IF the
		item happens to be nearby (for some definition of nearby)
		
		If an insert occurs, this will automatically detect it, discard the
		cached data, and start the search from the root of the tree. 

		If allow_duplicates is specified, find() will only find the
		left-most instance of a key. You can use next() to iterate over
		other instances of the key.
		
		@return true if an item with the key is found
	*/
	bool find(const KeyType &key) WARN_IF_UNUSED
	{
		// this introduces a few special cases
		bool one_level_tree = (m_tree->backend.m_treeHeight == 1);

		NodeItem tmp(key);
		m_tree->m_lastError = _Tree::ERR_NONE;
				
		// detect if we're out of sync
		if (m_tree->m_inserts != m_inserts)
		{
			reset();
			if (one_level_tree)
			{
				idx.push_back(0);
				leaf = m_tree->backend.get_leaf_node(id.back());
			}
		}
		
		if (id.empty())
			return false;

		// if we're at a leaf, then work our way from that leaf
		// to the item we're searching for
		if (id.size() == m_tree->backend.m_treeHeight)
		{		
			// see if the item resides here
			const LeafItemIterator bi = leaf->items.begin();
			const LeafItemIterator ei = leaf->items.end();
			LeafItemIterator i = std::lower_bound(bi, ei, tmp);
			
			if (i != ei && i->key == key)
			{
				// found it, set the current index
				idx.back() = i - bi;

				// if allow_duplicates is true, then return true if the index 
				// is not left-most unless this is a one level tree. Otherwise, 
				// return true
				if (!_Tree::allow_duplicates || one_level_tree || idx.back())
					return true;
			}
			
			// otherwise, pop back up a level
			id.pop_back();
			idx.pop_back();

			// interesting question: would going up two levels 
			// increase performance? It reduces the number of resets
			// by probably half or so, but increases the number of
			// total traversals
		}
		
		// apparently we didn't find it
		if (one_level_tree)
			return false;

		// the gotcha: we don't actually know the extent of the edge 
		// of any node, so we can't tell whether or not the item falls within
		// its bounds. So we can only jump up one level, attempt to traverse
		// to it, and if it is not found then we give up and purge everything,
		// and try from the top. If that fails, the item must not exist.
		
		
		if (id.size() > 1)
		{
			if (traverse_to(tmp))
				return true;

			// blow it all and try again
			reset();
		}
		
		// last try -- we're at the top
		return traverse_to(tmp);
	}
	
	void reset()
	{
		id.clear();
		idx.clear();
		
		// reduce allocations -- some STL apparently remove the
		// reservation after you clear
		id.reserve(m_tree->backend.m_treeHeight);
		idx.reserve(m_tree->backend.m_treeHeight);
		
		// start at the root
		if (m_tree->backend.m_treeHeight > 0)
			id.push_back(m_tree->backend.m_rootId);

		// transaction id -- use this to determine when an insert
		// occurred and we need to force a reset
		m_inserts = m_tree->m_inserts;

		// no longer valid
		leaf = NULL;
		
		BDEBUG( m_resets += 1; )
	}

private:

	// attempt to traverse to a item with a specified key
	bool traverse_to(const NodeItem &item)
	{	
		assert(id.size() != m_tree->backend.m_treeHeight);
		
		bool started_at_top = (id.size() == 1);
		bool cant_be_start = false;

		do
		{
			// grab the node
			Node * node = m_tree->backend.get_node(id.back());
			if (!node) return false;

			// Find the one with the key greater than us
			const NodeItemIterator b = node->items.begin();
			const NodeItemIterator e = node->items.end();
			//const NodeItemIterator selected = std::upper_bound( b, e, item);
			const NodeItemIterator selected = std::lower_bound(b, e, item);
			
			if (selected == e)
			{
				// try going down the right side of the mountain
				id.push_back(node->end_id);
				idx.push_back(node->items.size()+1);
			}		
			else
			{
				// otherwise go down the middle of the tree somewhere
				id.push_back(selected->id);
				idx.push_back(selected - b);

				// this is for handling duplicates: if this node is at the
				// edge, then the final selected item cannot be at the edge
				// since there *may* be another one to the left
				if (_Tree::allow_duplicates && !started_at_top && idx.back() == 0)
					cant_be_start = true;
			}
		}
		while (id.size() < m_tree->backend.m_treeHeight);
		
		
		// at a leaf, get the id
		leaf = m_tree->backend.get_leaf_node(id.back());
		if (!leaf) return false;
		
		const LeafItemIterator b = leaf->items.begin();
		const LeafItemIterator e = leaf->items.end();
		const LeafItemIterator selected = std::lower_bound(b, e, item);
		
		idx.push_back(selected - b);

		// special handling for duplicate items
		if (_Tree::allow_duplicates && cant_be_start && !idx.back())
			return false;
		
		// see if we found the item
		if (selected != e && item.key == selected->key)
			return true;
		
		// didn't find the item in this traversal
		return false;
	}
	
	std::size_t m_inserts;
};

#endif
