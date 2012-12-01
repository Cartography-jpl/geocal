/*
 *  SDBX
 *	$Id: BTreePrint.h 1841 2009-01-04 18:38:58Z virtuald $
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
 */

/*
	This file is just for debugging the B+ tree... thus why it is in its
	own file. You only need it if you call the print function. 
	
	You need to include this directly. Sorry.
*/
 

#ifdef SDBX_BTREE_H 
 

#ifndef SDBX_BTREE_PRINT_H
#define SDBX_BTREE_PRINT_H

#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

// testing only: traverse the tree and print it out in a graphviz format
// returns the number of leaves it printed out
__BTREE_TEMPLATE_DEFN
uint64_t BTree<__BTREE_TEMPLATE_FN>::Print(const char * filename = "tree")
{
	uint64_t leaf_count = 0;
	std::ofstream output;
	std::string digraph(filename);
	output.open((digraph + ".dot").c_str(), std::ifstream::trunc);
	
	output << "digraph " << filename << " {\nnode [shape = record];\n";
	
	if (backend.m_treeHeight)
	{
		std::vector<IdType>			id;
		std::vector<std::size_t>	idx;

		// reduce allocations
		id.reserve(backend.m_treeHeight);
		idx.reserve(backend.m_treeHeight);
		
		id.push_back(backend.m_rootId);
		idx.push_back(0);
	
		// this could be recursive, but that would be so much easier! :)
		while (!id.empty())
		{
			if (id.size() < backend.m_treeHeight)
			{
				Node * node = backend.get_node(id.back());
				std::size_t cur_idx = idx.back();
				
				if (cur_idx <= node->items.size())
				{
					// define the node label and its parent
					if (cur_idx == 0)
					{
						// linkage
						if (id.size() == 1)
							output << "\"root\" -> \"";
						else
							output << "\"node" << *(id.end()-2) << "\" -> \"";

						output << "node" << id.back() << "\"\nnode" << id.back() << " [ label =\"";

						// label
						int i = 1;
						for (NodeItemIterator it = node->items.begin(); it != node->items.end(); ++it, ++i)
							output << "<f" << i << "> " <<  it->key << " | ";

						while (i <= max_child_items)
							output << "<f" << (i++) << "> x | ";

						output << "<f" << i << ">\"];\n";
					}
				

					if (idx.back()++ == node->items.size())
						// then traverse down the last id in our set
						id.push_back(node->end_id);
					else
						// otherwise iterate over all nodes
						id.push_back(node->items[idx.back()-1].id);
					
					idx.push_back(0);
					continue;
				}
			}
			else
			{
				// at a leaf: print it out
				LeafNode * leaf = backend.get_leaf_node(id.back());
				
				if (id.size() == 1)
					output << "\"root\" -> \"";
				else
					output << "\"node" << *(id.end()-2) << "\":f" << *(idx.end()-2) << " -> \"";

				// print out all of the nodes
				int i = 1;
				for (LeafItemIterator it = leaf->items.begin(); it != leaf->items.end(); ++it, ++i)
					output << it->key << " | ";

				while (i++ <= max_child_items)
					output << "x | ";

				leaf_count += leaf->items.size();

				output << "\"\n";
			}
			
			// go up a level
			id.pop_back();
			idx.pop_back();		
		}
	}

	output << "}\n";
	output << "# " << leaf_count << " leaves visited\n"; 
	output.close();

	return leaf_count;
}


#endif
#endif


