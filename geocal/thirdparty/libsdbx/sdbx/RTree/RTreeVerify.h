/*
 *  SDBX
 *	$Id$
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


#ifndef SDBX_RTREE_VERIFY_H
#define SDBX_RTREE_VERIFY_H

#include <vector>

//! Use this to verify that a tree has a valid structure (walks all nodes)
__RTREE_TEMPLATE_DEFN
bool RTree<__RTREE_TEMPLATE_FN>::Verify()
{
	uint64_t leaf_count = 0;

	if (backend.m_treeHeight)
	{
		std::vector<IdType>			id;
		std::vector<std::size_t>	idx;
		std::vector<BoundingBox>	bounds;
		
		// reduce allocations
		id.reserve(backend.m_treeHeight);
		idx.reserve(backend.m_treeHeight);
		bounds.reserve(backend.m_treeHeight);
		
		// initialize the stack
		id.push_back(backend.m_rootId);
		idx.push_back(0);
	
		// this could be recursive, but that would be so much easier! :)
		while (!id.empty())
		{
			if (id.size() < backend.m_treeHeight)
			{
				Node * node = backend.get_node(id.back());
				std::size_t cur_idx = idx.back()++;
				
				if (node->items.size() == 0)
				{
					assert(0 && "verify failure type 0");
					return false;
				}
				
				if (cur_idx < node->items.size())
				{
					// perform a check on each level when you initially enter it
					if (cur_idx == 0 && bounds.size() > 0) 
					{
						// verify that all nodes are enclosed
						for (std::size_t i = 0; i < node->items.size(); ++i)
						{
							if (!bounds.back().encloses(node->items[i].key))
							{
								assert(0 && "verify failure type 1");
								return false;
							}
						}
					}

					// iterate over all nodes, traverse down the current level
					id.push_back(node->items[idx.back()-1].id);
					bounds.push_back(node->items[idx.back()-1].key);
					idx.push_back(0);
					
					continue;
				}
			}
			else
			{
				// at a leaf
				LeafNode * leaf = backend.get_leaf_node(id.back());
				
				// perform a check on each leaf level
				if (bounds.size() > 0) 
				{
					// verify that all nodes are enclosed
					for (std::size_t x = 0; x < leaf->items.size(); x++)
					{
						if (!bounds.back().encloses(leaf->items[x].key))
						{
							assert(0 && "verify failure type 2");
							return false;
						}
					}
				}
				
				leaf_count += leaf->items.size();
			}
			

			// go up a level			
			id.pop_back();
			idx.pop_back();

			if (bounds.size())
			{
				bounds.pop_back();
				assert(bounds.size() == idx.size()-1);
			}

			assert(id.size() == idx.size());
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


