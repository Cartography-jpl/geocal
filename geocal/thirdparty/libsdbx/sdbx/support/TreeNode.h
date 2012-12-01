/*
 *  SDBX
 *	$Id: TreeNode.h 1841 2009-01-04 18:38:58Z virtuald $
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
 *	Do not include this file directly
 */


#ifndef SDBX_TREE_NODE_H
#define SDBX_TREE_NODE_H

// this doesn't actually do anything, it just allows us to store
// a base pointer for m_root. Thats it. 
struct TreeNodeBase {};


/**
	\class TreeNode
	\brief Stores and handles serialization for items in a Tree
	
	@tparam _Item		Type of the item being stored. Must support the 
						serialization concept defined in Serialization.h
	@tparam _max_items	Max number of items that this stores
*/
template <typename _Item, std::size_t _max_items>
struct TreeNode : public TreeNodeBase {

	typedef _Item							Item;
	typedef std::vector < Item > 			ItemArray;

	
	bool		is_dirty;		//! set to true if this needs to be re-saved
								//! you *must* manage this, it does not get
								//! managed automatically
								
	ItemArray	items;			//! array of items this node holds

	// default constructor
	TreeNode() : is_dirty(true) 
	{
		// reduce memory allocations
		items.reserve(_max_items+1);
	}
	
	
	/// Serialization params
	enum {  
			max_items = _max_items,
			record_size = 
				sizeof(uint16_t) + 					// array count
				_max_items * record<Item>::size		// array
	};

	
	/// Serialization constructor
	explicit TreeNode(const unsigned char * const buffer) :
		is_dirty(false)
	{
		unsigned char * cur_buf = (unsigned char *)(buffer + sizeof(uint16_t));
		
		// reduce memory allocations: max+1 because of splits
		items.reserve(_max_items+1);
		
		// grab the count
		std::size_t count = BUFFER_REF(uint16_t, 0);
		assert(count <= _max_items && "corruption in file");
		
		for (std::size_t i = 0; i < count; i++)
		{
			// add the item, move to the next spot in the buffer
			items.push_back(Item(record<Item>::deserialize(cur_buf)));
			cur_buf += record<Item>::size;
		}
	}
	
	
	/// Serializer
	void Serialize(unsigned char * buffer) const
	{
		// serialize each node
		assert(items.size() <= _max_items);
		
		typename ItemArray::const_iterator i = items.begin();
		const typename ItemArray::const_iterator e = items.end();
		
		// write the size
		SERIALIZE(uint16_t, 0, (uint16_t)items.size());
		buffer += sizeof(uint16_t);
		
		// write the array
		for (; i != e; i++)
		{
			i->Serialize(buffer);
			buffer += Item::record_size;
		}
	}	
};


#endif


