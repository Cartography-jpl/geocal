/*
 *  SDBX
 *	$Id: BTreeNode.h 1841 2009-01-04 18:38:58Z virtuald $
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


#ifndef SDBX_BTREE_NODE_H
#define SDBX_BTREE_NODE_H


#ifdef USE_MSVC_MEMORY_TRACING
	// this enables advanced memory leak detection
	#include <wx/msw/msvcrt.h>
#endif

typedef TreeNodeBase BTreeNodeBase;


/**
	\struct BTreeNode
	\brief Stores and handles serialization for items in a BTree
	
	@tparam _Item		Type of the item being stored. Must support the 
						serialization concept defined in Serialization.h
	@tparam _max_items	Max number of items that this stores
*/
template <typename _Item, std::size_t _max_items>
struct BTreeNode : public BTreeNodeBase {

	typedef _Item							Item;
	typedef std::vector < Item > 			ItemArray;
	typedef typename Item::IdType			IdType;

	
	bool		is_dirty;		//! set to true if this needs to be re-saved
								//! you *must* manage this, it does not get
								//! managed automatically
								
	ItemArray	items;			//! array of items this node holds
	
	IdType		end_id;			//! id on the end of the BTree node

	// default constructor
	BTreeNode() : is_dirty(true) 
	{
		// reduce memory allocations
		items.reserve(_max_items+1);
	}
	
	
	/// Serialization params
	enum {  max_items = _max_items,
			record_size = 
				sizeof(uint16_t) + 					// array count
				record<IdType>::size +				// end_id
				_max_items * record<Item>::size		// array
	};
	
	/// Serialization constructor
	explicit BTreeNode(const unsigned char * const buffer) :
		is_dirty(false)
	{
		unsigned char * cur_buf = (unsigned char *)(buffer + sizeof(uint16_t));
		
		// reduce memory allocations: max+1 because of splits
		items.reserve(_max_items+1);
		
		// grab the count
		std::size_t count = BUFFER_REF(uint16_t, 0);
		assert(count <= _max_items && "corruption in file");
		
		// grab the end_id
		end_id = record<IdType>::deserialize(cur_buf);
		cur_buf += record<IdType>::size;
		
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
		
		// write the end id
		record<IdType>::serialize(end_id, buffer);
		buffer += record<IdType>::size;
		
		// write the array
		for (; i != e; i++)
		{
			i->Serialize(buffer);
			buffer += Item::record_size;
		}
	}

	// utility function
	IdType get_item_id(std::size_t idx)
	{
		if (idx == items.size())
			return end_id;
		else if (idx < items.size())
			return items[idx].id;
		
		return NULL;
	}
};



#endif


