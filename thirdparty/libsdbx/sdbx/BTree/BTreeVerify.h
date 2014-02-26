/*
 *  SDBX
 *	$Id: BTreeVerify.h 1842 2009-01-05 01:17:04Z virtuald $
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


#ifndef SDBX_BTREE_VERIFY_H
#define SDBX_BTREE_VERIFY_H

#include <vector>

//! Use this to verify that a tree has a valid structure (walks all nodes)
__BTREE_TEMPLATE_DEFN
bool BTree<__BTREE_TEMPLATE_FN>::Verify()
{
	uint64_t leaf_count = 0;

	if (backend.m_treeHeight)
	{
		std::vector<IdType>			id;
		std::vector<std::size_t>	idx;
		std::vector<bool>			idx_is_end;
		std::vector<KeyType>		keys;
		
		// reduce allocations
		id.reserve(backend.m_treeHeight);
		idx.reserve(backend.m_treeHeight);
		idx_is_end.reserve(backend.m_treeHeight);
		keys.reserve(backend.m_treeHeight);
		
		// initialize the stack
		id.push_back(backend.m_rootId);
		idx.push_back(0);
		idx_is_end.push_back(false);
	
		// this could be recursive, but that would be so much easier! :)
		while (!id.empty())
		{
			if (id.size() < backend.m_treeHeight)
			{
				Node * node = backend.get_node(id.back());
				std::size_t cur_idx = idx.back();
				
				if (node->items.size() == 0)
				{
					assert(0 && "verify failure type 0");
					return false;
				}
				
				if (cur_idx <= node->items.size())
				{
					// perform a check on each node when you initially enter it
					if (cur_idx == 0) 
					{
						if (!std::is_sorted(node->items.begin(), node->items.end()))
						{
							assert(0 && "verify failure type 1a");
							return false;
						}

						if (keys.size() > 0 && 
							!*(idx_is_end.end()-2) && 
							keys.back() < node->items.back().key)
						{
							assert(0 && "verify failure type 1b");
							return false;
						}
					}

					if (idx.back()++ == node->items.size())
					{
						// then traverse down the last id in our set
						id.push_back(node->end_id);
						idx_is_end.back() = true;
						keys.push_back(KeyType());
					}
					else
					{
						// otherwise iterate over all nodes
						id.push_back(node->items[idx.back()-1].id);
						keys.push_back(node->items[idx.back()-1].key);
					}
					
					idx.push_back(0);
					if (id.size() < backend.m_treeHeight)
						idx_is_end.push_back(false);
					continue;
				}
			}
			else
			{
				// at a leaf
				LeafNode * leaf = backend.get_leaf_node(id.back());
				
				if (!idx_is_end.back() && keys.back() < leaf->items.back().key )
				{
					assert(0 && "verify failure type 2a");
					return false;
				}
				
				if (!std::is_sorted(leaf->items.begin(), leaf->items.end()))
				{
					assert(0 && "verify failure type 2b");
					return false;
				}
				
				leaf_count += leaf->items.size();
			}
			

			// go up a level
			
			if (id.size() < backend.m_treeHeight) idx_is_end.pop_back();
						
			id.pop_back();
			idx.pop_back();

			if (keys.size())
			{
				keys.pop_back();
				assert(keys.size() == idx.size()-1);
			}

			assert(id.size() == idx.size());
			assert(idx.size() == idx_is_end.size());
			
		}
	}

	if (leaf_count != GetItemCount())
	{
		assert(0 && "verify failure type 3");
		return false;
	}

	return true;
}


#endif


