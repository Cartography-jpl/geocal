/*
 *  SDBX
 *	$Id: MemoryTreeBackend.h 1841 2009-01-04 18:38:58Z virtuald $
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


	Implements the things needed to make a memory-based tree work
*/

#ifndef SDBX_MEMORY_TREE_BACKEND_H
#define SDBX_MEMORY_TREE_BACKEND_H

#ifdef USE_MSVC_MEMORY_TRACING
	// this enables advanced memory leak detection
	#include <wx/msw/msvcrt.h>
#endif


struct MemoryTreeBackendArgs {};


/**
	\class MemoryTreeBackend
	\brief Implements a memory-only backend for a tree
	
	See StorageConcepts for more information

	The basic premise behind this is that anything that is an
	'id' is really just a pointer to a node, since they are *never* 
	compared or used otherwise by the trees other than to call 
	these routines. Makes it really easy to implement this. :)
*/
template <
	typename _NodeBase,
	template <typename _i, std::size_t _mi> class _NodeType,
	template <typename _k, typename _v> class _NodeItemType,
	typename _LeafNode,
	typename _KeyType,
	std::size_t _max_node_items>
class MemoryTreeBackend {
public:

	// types provided to us
	typedef _NodeBase									NodeBase;
	typedef _LeafNode									LeafNode;


	// types that the backend needs to define
	typedef NodeBase*									IdType;
	typedef std::size_t									size_type;
	
	typedef _NodeItemType<_KeyType, IdType >			NodeItem;
	typedef _NodeType<NodeItem, _max_node_items>		Node;
	
	// args for the initialization function
	typedef MemoryTreeBackendArgs						Args;

	enum ErrorCode {
		ERR_NONE
	};


	MemoryTreeBackend() :
		m_rootId(NULL),
		m_treeHeight(0),
		m_size(0)
	{}

	~MemoryTreeBackend()
	{
		// walk the tree, delete stuffs
		Clear(m_rootId, 1);
	}
	
	// there are no meaningful errors that can be returned
	ErrorCode GetErrorCode() const { return ERR_NONE; }

	void Clear(NodeBase * nd, size_type level)
	{
		if (!nd)
			return;

		if (level == m_treeHeight)
		{
			delete static_cast<LeafNode*>(nd);
			return;
		}

		Node * node = static_cast<Node*>(nd);

		int i = 0;
		NodeBase * base;
		while ((base = node->get_item_id(i++)))
			Clear(base, level + 1);
			
		delete node;
	}


	// initialization function: doesn't do anything
	bool Initialize(Args &args) const { return true; }

	// always ok
	bool IsOk() const { return true; }

	// this creates a new root node and places its ID in m_rootId, you
	// MUST hold onto the old ID and place it in the new root node (which
	// you will need to retrieve using get_node or get_leaf_node). Each time
	// you call this the tree height is incremented
	bool create_root_node() 
	{
		if (!m_treeHeight)
			m_rootId = new LeafNode();
		else
			m_rootId = new Node();
		
		if (m_rootId)
		{
			m_treeHeight += 1;
			return true;
		}
		
		return false;
	}

	IdType new_node_id() const {
		return new Node();
	}
	
	Node * create_node(IdType id) const {
		return static_cast<Node*>(id);
	}

	Node * get_node(IdType id) const {
		return static_cast<Node*>(id);
	}

	IdType new_leaf_node_id() const {
		return new LeafNode();
	}

	LeafNode * create_leaf_node(IdType id) const {
		return static_cast<LeafNode*>(id);
	}

	LeafNode * get_leaf_node(IdType id) const {
		return static_cast<LeafNode*>(id);
	}

	// the backend defines these variables
	IdType 		m_rootId;
	size_type	m_treeHeight;
	size_type	m_size;

};

#endif

