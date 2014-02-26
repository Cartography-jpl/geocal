/*
 *  SDBX
 *	$Id: BTree.h 1841 2009-01-04 18:38:58Z virtuald $
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

#ifndef SDBX_BTREE_BTREE_H
#define SDBX_BTREE_BTREE_H


#ifdef USE_MSVC_MEMORY_TRACING
	// this enables advanced memory leak detection
	#include <wx/msw/msvcrt.h>
#endif


#define SET_BAD_ERROR( code )	\
	m_lastError = code;			\
	assert(0 && #code)


// this is the template definition for the BTree.. no, I'm not
// kidding unfortunately

#define __BTREE_TEMPLATE_DEFN												\
template <																	\
	typename _KeyType,														\
	typename _leaf_type,													\
	std::size_t max_child_items,											\
	template <																\
		typename _b_nb,														\
		template <typename _bn_i, std::size_t _bn_mi> class _NodeType,		\
		template <typename _bni_k, typename _bni_i> class _NodeItemType,	\
		typename _b_l,														\
		typename _b_k,														\
		std::size_t _b_mi>													\
	class _Backend,															\
	bool _allow_duplicates													\
>

#define __BTREE_TEMPLATE_FN													\
	_KeyType,																\
	_leaf_type,																\
	max_child_items,														\
	_Backend,																\
	_allow_duplicates


/**
	\class BTree
	\brief Implementation of a modified B/B+ Tree
	
	This is intended to be a templated implementation of an B+ Tree. Yes its
	been done before, but... this is better. Maybe.

	Amusingly enough, this is adapted from the R* Tree implementation, so 
	many of the steps are derived from the way the R* tree does it.

	Structure:
	
		- Each node has up to max_items in it. Each item has a key, and an 
		id that points to the next node or a leaf node. 
			
		- In any given node, the key for that node is always equivalent to
		the key	of the last child item of that node, so it is always
		>= the keys of all child items. 
			
		- There is an end_id of each non-leaf node, which points to nodes
		that are always > the last key in the node. 
				
		- There are LeafNodes and Nodes. LeafNodes contain leaves, whereas Nodes
		always contain pointers to either LeafNodes or Nodes depending on the
		level of the tree. 
	
	@tparam _KeyType			Type of key you want to index on
	@tparam _leaf_type			Type of leaf you want to store
	@tparam max_child_items		Maximum number of items in a node/leaf
	@tparam _Backend			Type of the backend to use to store nodes,	
								either DiskTreeBackend or MemoryTreeBackend
	@tparam _allow_duplicates	Allow the tree to store items with the same keys
	
	@todo Turn this into a buffer tree. Doing it for the B tree should be
	easy enough, then it should be trivial to convert the R* tree to do it
	
*/

__BTREE_TEMPLATE_DEFN
class BTree 
{
public:

	enum { allow_duplicates = _allow_duplicates };

	typedef BTree<__BTREE_TEMPLATE_FN>					this_type;

	typedef _KeyType									KeyType;
	typedef _leaf_type									LeafType;

	// shortcut definitions
	typedef BTreeNodeBase								NodeBase;
	

	typedef TreeLeafItem<KeyType, LeafType>				LeafItem;
	typedef TreeNode<LeafItem, max_child_items> 		LeafNode;
	typedef typename LeafNode::ItemArray				LeafItemArray;
	typedef typename LeafItemArray::iterator			LeafItemIterator;

	// import types from the backend
	typedef _Backend<
					NodeBase, 
					BTreeNode, 
					TreeNodeItem, 
					LeafNode, 
					KeyType, 
					max_child_items>					Backend;

	typedef typename Backend::Args						BackendArgs;

	typedef typename Backend::size_type					size_type;
	typedef typename Backend::IdType					IdType;

	typedef typename Backend::NodeItem					NodeItem;
	typedef typename Backend::Node 						Node;
	typedef typename Node::ItemArray					NodeItemArray;
	typedef typename NodeItemArray::iterator			NodeItemIterator;
	
	// define an traversal/iterator type (not STL compatible)
	typedef BTreeTraverser<this_type>					Traverser;
	typedef Traverser									Iterator;
	
	friend class BTreeTraverser<this_type>;

	// error codes
	enum ErrorCode {
		ERR_NONE = 0,
		ERR_CREATE_NODE_IN_SPLIT = 100,
		ERR_CREATE_ROOT,
		ERR_CREATE_SPLIT_ROOT,
		ERR_GET_LEAFNODE,
		ERR_GET_NODE,
		ERR_GET_ROOT,
		ERR_GET_SPLIT_ROOT,
		ERR_PROPAGATE_SPLIT,

		// these are non-fatal errors
		ERR_ITEM_EXISTS = 200

	};
	
	
	//! Default constructor
	BTree() :
		m_inserts(0)
	{}
	
	// destructor
	~BTree() {
	}

	/// \name Backend/control interface
	/// @{

	// initializes the backend using the specified arguments
	bool Initialize(BackendArgs &args) WARN_IF_UNUSED
	{
		return backend.Initialize(args);
	}
	
	bool IsOk()
	{
		return backend.IsOk();
	}
	
	/// returns the last error code: it may either be the
	/// trees error code, or the backend's error code
	ErrorCode GetErrorCode() const
	{
		if (m_lastError != ERR_NONE)
			return m_lastError;

		return (ErrorCode)backend.GetErrorCode();
	}
	
	/// erases all nodes from the tree
	void Destroy()
	{
		backend.Destroy();
	}

	/// @}
	
	/// \name Query functions
	/// @{
	
	Traverser * GetTraverser()
	{	
		return new Traverser(this);
	}
	
	/// when you call this, ensure that end() is not true
	Iterator * Begin()
	{	
		Iterator * it = new Iterator(this);
		if (!it->begin()) {}
		return it;
	}
	
	
	/// @}
	

	/**
		\brief Insert function, adds a new item to the tree

		@param key		Key to index by (must be unique, see the update param)
		@param leaf		Item to insert
		@param update	(ignored if allow_duplicates is true) If this is true, 
						then if the item is found in the tree it will update it. 
						Otherwise it will just insert a new value. If this is 
						false, then if the item is in the tree it will return 
						an error. 
						
	*/
	bool Insert(
		const KeyType &key, 
		const LeafType &leaf, 
		bool update = true)
		WARN_IF_UNUSED
	{
		m_lastError = ERR_NONE;
	
		// create a new root node if necessary
		if (backend.m_treeHeight == 0)
		{
			// create a root node
			if (!backend.create_root_node())
			{
				SET_BAD_ERROR(ERR_CREATE_ROOT);
				return false;
			}
			
			LeafNode * root = backend.get_leaf_node(backend.m_rootId);
	
			if (!root)
			{
				SET_BAD_ERROR(ERR_GET_ROOT);
				return false;
			}
			
			root->items.push_back(LeafItem(key, leaf));
			root->is_dirty = true;
		}
		else
		{
			// begin a recursive insert
			#ifndef NDEBUG
				NodeItem * oi = 
			#endif
			InsertInternal(LeafItem(key, leaf), backend.m_rootId, 1, update);

			assert(!oi && "this should always be NULL");	
		}
		
		if (m_lastError != ERR_NONE)
			return false;

		++backend.m_size;
		++m_inserts;
		return true;
	}

	
	size_type GetItemCount() const { return backend.m_size; }
	size_type GetHeight() const { return backend.m_treeHeight; }
	
	// for debugging only.. if you want this, include BTreePrint.h
	uint64_t Print(const char * filename);
	
	// tests the tree and ensures that it is valid. include BTreeVerify.h
	bool Verify();
	
protected:

	/*
		inserts nodes recursively. If this returns something, then that item should
		be added to the caller's level of the tree and then the object should
		be deleted
	
		Lists of items inside Nodes should always be sorted.
	*/
	NodeItem * InsertInternal(
		const LeafItem &leafItem, 
		const IdType node_id,
		const std::size_t level,
		bool update)
	{	
	
		// If we're at a leaf, then insert at that level
		if (level == backend.m_treeHeight)
		{
			LeafNode * node = backend.get_leaf_node(node_id);
			
			// make sure there are no errors
			if (!node) { SET_BAD_ERROR(ERR_GET_LEAFNODE); return NULL; }
			
			assert(!node->items.empty());
			assert(std::is_sorted(node->items.begin(), node->items.end()));
		
			// If N has less than M items, accommodate E in N
			LeafItemIterator item(std::lower_bound(node->items.begin(), node->items.end(), leafItem));

			// if the item exists, then update or return an error
			if (allow_duplicates || item == node->items.end() || item->key != leafItem.key)
				node->items.insert(item, leafItem);
			else if (update)
				*item = leafItem;
			else
			{
				m_lastError = ERR_ITEM_EXISTS;
				return NULL;
			}

			node->is_dirty = true;

			// split, and propagate the split upwards if necessary
			return Split(node, level);
		}
		
		// if we're not at a leaf.. 

		// grab the node from the cache
		Node * node = backend.get_node(node_id);
		
		if (!node) { SET_BAD_ERROR( ERR_GET_NODE ); return NULL; }
		
		assert(!node->items.empty());
		assert(std::is_sorted(node->items.begin(), node->items.end()));
	
		const NodeItemIterator begin(node->items.begin());
		const NodeItemIterator end(node->items.end());

		// Find an appropriate node N, m which to place the
		// new leaf E: the one with the key <= us
		const NodeItemIterator selected(std::lower_bound( begin, end, leafItem));

		// may need this later
		const std::size_t idx = selected - begin;
	
		// insert recursively, this also tells us if we need
		// to progagate a split or not
		NodeItem * si = InsertInternal(	leafItem, 
										// select the right node id if its at the end
										(selected == end ? node->end_id : selected->id), 
										level+1,
										update);
		if (!si)
			return NULL;
	
		// propagate the split
		node = PropagateSplit(node_id, idx, si);
			
		// check for errors
		if (m_lastError != ERR_NONE)
			return NULL;
		
		//char strbuf[10];
		//sprintf(strbuf, "split%d", split_num++);
		//Print(strbuf);

		// do a split on ths if necessary
		return Split(node, level);		
	}
	
	/*
		Splits a given node
		
		@return a NodeItem if a split occurred. If an error occurs, err is
		set to true.
	*/
	
	template <typename NodeType>
	NodeItem * Split(
		NodeType * node, 
		const std::size_t &level)
	{
		// If N has M+1 items, split the node
		if (node->items.size() <= max_child_items )
			return NULL;
	
		// this should always hold true
		assert(node->items.size() == max_child_items + 1);
	
		// this will be true soon
		node->is_dirty = true;
	
		// create a new node id
		IdType new_id;
		NodeBase * tmp;
		
		// do it this way to avoid compile errors.. 
		if (level == backend.m_treeHeight)
		{
			new_id =	backend.new_leaf_node_id();
			tmp =		backend.create_leaf_node(new_id);
		}
		else
		{
			new_id =	backend.new_node_id();
			tmp =		backend.create_node(new_id);
		}
		
		if (!tmp) { SET_BAD_ERROR( ERR_CREATE_NODE_IN_SPLIT ); return NULL;}
		
		// cast it to the proper type
		NodeType * new_node = static_cast<NodeType*>(tmp);
				
		
		// decide where the split should occur
		const typename NodeType::ItemArray::iterator middle( 
			node->items.begin() + ((node->items.size()+1)/2));
			
		const typename NodeType::ItemArray::iterator end(node->items.end());	
	
		// distribute the end of the array to the new node, then erase them from the original node
		new_node->items.assign(middle, end);
		node->items.erase(middle, end);
		
		// this is the new key for the existing node
		const KeyType middle_key(node->items.back().key);

		// splitting nodes requires some extra twiddling
		if (level != backend.m_treeHeight)
			FinishNodeSplit(node, new_node);

		
		
		// If we caused a split of the root, split it
		if (level == 1)
			return SplitRoot( middle_key, new_id);
		
		// otherwise, propagate it upwards
		return new NodeItem(middle_key, new_id);	
	}
	
	// extra twiddling required for non-leafnode types..
	void FinishNodeSplit(NodeBase * _node, NodeBase * _new_node)
	{
		Node * node = static_cast<Node*>(_node);
		Node * new_node = static_cast<Node*>(_new_node);

		// switch these
		new_node->end_id = node->end_id;
		// and these
		node->end_id = node->items.back().id;
		// and remove that node
		node->items.erase(node->items.end()-1);
	}
	
	
	
	// we should be at the root
	NodeItem * SplitRoot(
		const KeyType &key, 
		const IdType &split_id )
	{
		// retain the id of the old root
		IdType old_root_id(backend.m_rootId);
		
		// create a new root node
		if (!backend.create_root_node())
		{
			SET_BAD_ERROR( ERR_CREATE_SPLIT_ROOT );
			return NULL;
		}
		
		// if we split the root -- it will always be a node
		Node * root = backend.get_node(backend.m_rootId);

		if (!root) { SET_BAD_ERROR( ERR_GET_SPLIT_ROOT ); return NULL; }
		
		// fill in the new root -- the old root id goes on the left with the
		// new key, and the split node on the end
		root->items.push_back(NodeItem(key, old_root_id));
		root->end_id = split_id;
		root->is_dirty = true;
		
		return NULL;
	}
	
	
	Node * PropagateSplit(
		const IdType node_id,
		const size_type old_idx,
		NodeItem * splitItem)
	{
		// finish the split: get the node that we're working with
		Node * node = backend.get_node(node_id);	
		
		if (!node)
		{
			SET_BAD_ERROR( ERR_PROPAGATE_SPLIT );
			delete splitItem;
			return NULL;
		}
		
		// find the node that was decended from: the split item should fall
		// directly at the node we're looking for
		NodeItemIterator it(node->items.begin() + old_idx);
	
		if (it == node->items.end())
			std::swap(node->end_id, splitItem->id);
		else
		{
			// swap keys (since we can assert that the old node's key was <=
			// the combined set, so it must be the key for the split item
			// (though if it == end then it will be discarded)
			//std::swap(it->key, splitItem->key);

			std::swap(it->id, splitItem->id);
		}

		node->items.insert(it, *splitItem);
		node->is_dirty = true;
		
		// if all is well, then the range should be sorted.. 
		assert(std::is_sorted(node->items.begin(), node->items.end()));
		
		// done, delete this
		delete splitItem;
		
		// return this so it can be used
		return node;
	}
	
	
	Backend		backend;
	ErrorCode 	m_lastError;
	
	// number of inserts (used as a transaction counter)
	size_type 	m_inserts;
};

#undef SET_BAD_ERROR

#endif

