/*
 *  SDBX
 *	$Id: TreeItem.h 1841 2009-01-04 18:38:58Z virtuald $
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


#ifndef SDBX_TREE_ITEM_H
#define SDBX_TREE_ITEM_H


// definition of an node item
template <typename _KeyType, typename _IdType>
struct TreeNodeItem  {
	
	typedef _KeyType			KeyType;
	typedef _IdType				IdType;

	KeyType 		key;
	IdType 			id;
	
	/// initialization constructor
	explicit TreeNodeItem(const KeyType &k)
		: key(k)
	{}

	/// initialization constructor
	explicit TreeNodeItem(const KeyType &k, const IdType &i)
		: key(k), id(i)
	{}

	TreeNodeItem(){}
	
	/// Serialization params
	enum { 
		record_size = record<KeyType>::size + record<IdType>::size,
		id_offset = record<KeyType>::size
	};
	
	/// Serialization constructor
	explicit TreeNodeItem(const unsigned char * const buffer) :
		key(record<KeyType>::deserialize(buffer)),
		id(record<IdType>::deserialize(buffer + id_offset))
	{}
	
	/// Serializer
	void Serialize(unsigned char * buffer) const
	{
		record<KeyType>::serialize(key, buffer);
		record<IdType>::serialize(id, buffer + id_offset);
	}
		
	// gratuitious operators. I love templates.. :p
	template <typename ItemWithKey>
	bool operator < (const ItemWithKey &item) const { return key < item.key; }
	template <typename ItemWithKey>
	bool operator <=(const ItemWithKey &item) const { return key <=item.key; }
	template <typename ItemWithKey>
	bool operator > (const ItemWithKey &item) const { return key > item.key; }
	template <typename ItemWithKey>
	bool operator >=(const ItemWithKey &item) const { return key >=item.key; }
	template <typename ItemWithKey>
	bool operator ==(const ItemWithKey &item) const { return key ==item.key; }
	template <typename ItemWithKey>
	bool operator !=(const ItemWithKey &item) const { return key !=item.key; }
};

// definition of a leaf item -- pretty much the same as a node item,
// except the names are changed
template <typename _KeyType, typename _ValueType>
struct TreeLeafItem  {
	
	typedef _KeyType			KeyType;
	typedef _ValueType			ValueType;

	KeyType 		key;
	ValueType 		value;
	
	/// initialization constructor
	explicit TreeLeafItem(const KeyType &k, const ValueType &v)
		: key(k), value(v)
	{}
	
	/// Serialization params
	enum { 
		record_size = record<KeyType>::size + record<ValueType>::size,
		value_offset = record<KeyType>::size
	};
	
	/// Serialization constructor
	explicit TreeLeafItem(const unsigned char * const buffer) :
		key(record<KeyType>::deserialize(buffer)),
		value(record<ValueType>::deserialize(buffer + value_offset))
	{}
	
	/// Serializer
	void Serialize(unsigned char * buffer) const
	{
		record<KeyType>::serialize(key, buffer);
		record<ValueType>::serialize(value, buffer + value_offset);
	}
	
	// gratuitious operators. I love templates.. :p
	template <typename ItemWithKey>
	bool operator < (const ItemWithKey &item) const { return key < item.key; }
	template <typename ItemWithKey>
	bool operator <=(const ItemWithKey &item) const { return key <=item.key; }
	template <typename ItemWithKey>
	bool operator > (const ItemWithKey &item) const { return key > item.key; }
	template <typename ItemWithKey>
	bool operator >=(const ItemWithKey &item) const { return key >=item.key; }
	template <typename ItemWithKey>
	bool operator ==(const ItemWithKey &item) const { return key ==item.key; }
	template <typename ItemWithKey>
	bool operator !=(const ItemWithKey &item) const { return key !=item.key; }
};




#endif
