/*
 *  Copyright (c) 2008 Dustin Spicuzza <dustin@virtualroadside.com>
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of version 2 of the GNU General Public License
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

// todo: make this more into a more formal unit test


#include <wx/wx.h>

#include <string>
#include <ctime>

#include <stdio.h>

#include <iostream>

#define BTREE_TRAVERSER_DEBUG


#include <sdbx/BTree.h>

#define TEST_MEMORY_BACKEND

#ifdef TEST_MEMORY_BACKEND
	#include <sdbx/MemoryTreeBackend.h>
	typedef BTree< int, int, 4, MemoryTreeBackend, false> Tree;
#else
	#include <sdbx/DiskTreeBackend.h>
	typedef BTree< int, int, 4, DiskTreeBackend, false> Tree;
#endif


#ifdef USE_WX_MSVC_MEMORY_TRACING
	// this enables advanced memory leak detection
	#include <wx/msw/msvcrt.h>
#endif




void PrintStats(Tree &tree)
{
	std::cout << "Tree stats:"
		<< "\n\titems\t: " << tree.GetItemCount() 
		<< "\n\theight\t: " << tree.GetHeight()
		<< "\n";
}

std::size_t WalkAll(Tree &tree)
{
	Tree::Iterator * iter = tree.Begin();

	if (!iter || iter->end())
	{
		if (iter) delete iter;
		assert(0 && "Error");
		return 0;
	}

	std::size_t count = 0;

	do 
	{
		//Tree::LeafItem &item = iter->current();
		//printf("Item: %d\n", item.key);
		count += 1;

	} while (iter->next());

	assert(count == tree.GetItemCount());
	printf("Count: %d\n", count);
	
	delete iter;
	return count;
}



int main(int argc, char ** argv)
{
	wxInitializer initializer;
    if ( !initializer )
    {
        fprintf(stderr, "Failed to initialize the wxWidgets library, aborting.");
        return -1;
    }

	Tree tree;
	bool ret;
	
#ifndef TEST_MEMORY_BACKEND
	
	DiskTreeBackendArgs args(
		wxFileName(wxT("btree")),
		wxT("sig_y0"),
		5, 5
	);

	if (!tree.Initialize(args))
	{
		fprintf(stderr, "Failed to initialize the tree, aborting.");
		return -1;
	}

	PrintStats(tree);

#endif

	int exists = 0;

	srand(899);

	//#define nodes 10000
	
	if (tree.GetItemCount() == 0)
	{
		for (int i = 0; i < 10000; i++)
		{
			//int val = i + i%2;
			int val = rand() % 10000;
			//int val = 50-i;
			//int val = i;

			printf("Val: %d\n", val);

			ret = tree.Insert(val, i, false);

			if (!ret)
			{
				if (tree.GetErrorCode() != Tree::ERR_ITEM_EXISTS)
					printf("There was an error inserting (%d, %d)\n", val, i);
				else
					exists += 1;
			}
		}
	}

	PrintStats(tree);

	// walk the whole tree
	WalkAll(tree);
	
	size_t found = 0, not_found = 0;
	Tree::Traverser * t = tree.GetTraverser();
	
	// make various queries
	for (int i = 0; i < 50; i++)
	{
		int val = rand() % 10000;
		
		if (t->find(val))
		{
			printf("Found: %d\n", val);
			assert(t->current().key == val);
			++found;
		}
		else
		{
			printf("Not found: %d\n", val);
			++not_found;
		}
	}
	
	printf("10000 queries: found %d, not found %d, resets: %d\n", found, not_found, t->m_resets);
	
	delete t;

	// final query
	WalkAll(tree);

	if (!tree.Verify())
		printf("Invalid tree!\n");
	
	PrintStats(tree);

	return 0;
}


