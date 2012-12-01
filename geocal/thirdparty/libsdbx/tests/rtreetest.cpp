/*
 *  Copyright (c) 2008 Dustin Spicuzza <dustin@virtualroadside.com>
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

#include <wx/wx.h>

#include <string>
#include <ctime>
#include <stdio.h>
#include <iostream>


#include <sdbx/RTree.h>

#ifdef USE_MSVC_MEMORY_TRACING
	// this enables advanced memory leak detection
	#include <wx/msw/msvcrt.h>
#endif

#define TEST_MEMORY_BACKEND

#ifdef TEST_MEMORY_BACKEND
	#include <sdbx/MemoryTreeBackend.h>
	#define Backend MemoryTreeBackend
#else
	#include <sdbx/DiskTreeBackend.h>
	#define Backend DiskTreeBackend
#endif


#define RANDOM_DATASET
//#define GUTTMAN_DATASET

#ifdef RANDOM_DATASET
	typedef RTree<int, int, 2, 32, 64, Backend> 	Tree;
#else
	typedef RTree<int, int, 2, 2, 4, Backend>		Tree;
#endif

typedef Tree::BoundingBox			BoundingBox;


BoundingBox bounds(int x, int y, int w, int h)
{
	BoundingBox bb;
	
	bb.edges[0].first  = x;
	bb.edges[0].second = x + w;
	
	bb.edges[1].first  = y;
	bb.edges[1].second = y + h;
	
	return bb;
}


void PrintStats(Tree &tree)
{
	std::cout << "Tree stats:\n\tDim\t: " << tree.GetDimensions()
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
	
#ifndef TEST_MEMORY_BACKEND
	
	DiskTreeBackendArgs args(
		wxFileName(wxT("rtree")),
		wxT("sig_rtree"),
		5, 5
	);

	if (!tree.Initialize(args))
	{
		fprintf(stderr, "Failed to initialize the tree, aborting.");
		return -1;
	}

	PrintStats(tree);

#endif

	//Visitor x;
	
	// insert a bunch of items into the tree
	// Note: this dataset is the one shown on Guttman's original paper
#ifdef GUTTMAN_DATASET

	if (tree.GetItemCount() == 0)
	{
		if (!tree.Insert( bounds( 1,5   , 3,2 ), 8))
			printf("Error inserting into tree: code %d\n", tree.GetErrorCode()); 
		//tree.Print("I1");
		
		if (!tree.Insert( bounds( 6,1   , 2,2 ), 9))
			printf("Error inserting into tree: code %d\n", tree.GetErrorCode()); 
		//tree.Print("I2");
		
		if (!tree.Insert( bounds( 6,4   , 2,2 ), 10))
			printf("Error inserting into tree: code %d\n", tree.GetErrorCode()); 
		//tree.Print("I3");
		
		if (!tree.Insert( bounds( 9,0   , 2,14 ), 11))
			printf("Error inserting into tree: code %d\n", tree.GetErrorCode()); 
		//tree.Print("I4");
		
		if (!tree.Insert( bounds( 13,1  , 1,9 ), 13))
			printf("Error inserting into tree: code %d\n", tree.GetErrorCode()); 
		//tree.Print("I5");
		
		if (!tree.Insert( bounds( 12,5  , 2,2 ), 14))
			printf("Error inserting into tree: code %d\n", tree.GetErrorCode()); 
		//tree.Print("I6");
		
		if (!tree.Insert( bounds( 0,16  , 2,2 ), 15))
			printf("Error inserting into tree: code %d\n", tree.GetErrorCode()); 
		//tree.Print("I7");
		
		if (!tree.Insert( bounds( 3,11  , 6,7 ), 16))
			printf("Error inserting into tree: code %d\n", tree.GetErrorCode()); 
		//tree.Print("I8");
		
		if (!tree.Insert( bounds( 14,10 , 7,4 ), 17))
			printf("Error inserting into tree: code %d\n", tree.GetErrorCode()); 
		//tree.Print("I9");
		
		if (!tree.Insert( bounds( 16,8  , 2,9 ), 18))
			printf("Error inserting into tree: code %d\n", tree.GetErrorCode()); 
		//tree.Print("I10");
		
		if (!tree.Insert( bounds( 17,12 , 3,3 ), 19))
			printf("Error inserting into tree: code %d\n", tree.GetErrorCode()); 
		//tree.Print("I11");
	}
	
	PrintStats(tree);


	BoundingBox bound = bounds( 5,10, 5,5 );
	
	

	//x = tree.Query(Tree::AcceptAny(), Visitor());
	//std::cout << "AcceptAny: " << x.count << " nodes visited (" << tree.GetItemCount() << " nodes in tree)" << std::endl;

	Tree::OverlappingIterator * io = tree.FindOverlapping(bound);
	int count = 0;

	if (!io)
	{
		fprintf(stderr, "Error retrieving iterator: code %d\n", tree.GetErrorCode());
		return 1;
	}
	
	std::cout << "Searching in " << bound.ToString() << std::endl;

	while (!io->end())
	{
		count += 1;
		std::cout << "\tFound " << io->current().key.ToString() << std::endl;
		io->next();
	}

	std::cout << "Visited " << count << " nodes." << std::endl;
	
	delete io;
	
	PrintStats(tree);

	/*
	tree.RemoveBoundedArea(bound);
	
	// stretch the bounds a bit
	
	std::cout << "Searching in " << bound.ToString() << std::endl;
	x = tree.Query(RTree::AcceptOverlapping(bound), Visitor());
	std::cout << "Visited " << x.count << " nodes." << std::endl;
	
	BoundingBox bound2 = bounds(0,10, 10,10);
	std::cout << "Removing enclosed area " << bound2.ToString() << std::endl;
	tree.RemoveBoundedArea(bound2);
	
	std::cout << "Searching in " << bound.ToString() << std::endl;
	x = tree.Query(RTree::AcceptOverlapping(bound), Visitor());
	std::cout << "Visited " << x.count << " nodes." << std::endl;
	
	
	Visitor y = tree.Query(RTree::AcceptAny(), Visitor());
	std::cout << "Visited " << y.count << " nodes." << std::endl;
	*/
	
#endif
	

#ifdef RANDOM_DATASET
	srand(time(0));

	#define nodes 10000
	
	if (tree.GetItemCount() == 0)
	{
		for (int i = 0; i < nodes/2; i++)
		{
			tree.Insert(bounds( rand() % 1000, rand() % 1000, rand() % 10, rand() % 10), i);
			//WalkAll(tree);
		}
		
		PrintStats(tree);

		for (int i = 0; i < nodes/2; i++)
		{
			tree.Insert( bounds( rand() % 1000, rand() % 1000, rand() % 20, rand() % 20), i);
			//WalkAll(tree);
		}

		
	}

	PrintStats(tree);

	/*
	BoundingBox bound = bounds( 100,100, 300,400 );
	
	x = tree.Query(Tree::AcceptAny(), Visitor());
	std::cout << "AcceptAny: " << x.count << " nodes visited (" << tree.GetItemCount() << " nodes in tree)" << std::endl;
	
	
	std::cout << "Searching in " << bound.ToString() << std::endl;
	x = tree.Query(Tree::AcceptEnclosing(bound), Visitor());
	std::cout << "Visited " << x.count << " nodes (" << tree.GetItemCount() << " nodes in tree)" << std::endl;
	
	
	std::cout << "Removing enclosed area " << bound.ToString() << std::endl;
	tree.RemoveBoundedArea(bound);
	
	std::cout << "Searching in " << bound.ToString() << std::endl;
	x = tree.Query(Tree::AcceptEnclosing(bound), Visitor());
	std::cout << "Visited " << x.count << " nodes. (" << tree.GetSize() << " nodes in tree)" << std::endl;
	*/

	//tree.Print();
		
#endif

	WalkAll(tree);
	
	PrintStats(tree);
	
	if (tree.Verify())
		printf("Tree structure verified\n");
	else
		printf("ERROR: Tree structure is corrupt!");

	return 0;
}


