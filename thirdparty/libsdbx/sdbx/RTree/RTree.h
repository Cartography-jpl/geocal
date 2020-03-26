/*
 *  SDBX
 *	$Id: RTree.h 1844 2009-03-31 14:19:13Z virtuald $
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

#ifndef SDBX_RTREE_RTREE_H
#define SDBX_RTREE_RTREE_H


#ifdef USE_MSVC_MEMORY_TRACING
	// this enables advanced memory leak detection
	#include <wx/msw/msvcrt.h>
#endif

// R* tree parameters
#define RTREE_REINSERT_P 0.30
#define RTREE_CHOOSE_SUBTREE_P 32


#define SET_BAD_ERROR( code )	\
	m_lastError = code;			\
	assert(0 && #code)


// this is the template definition for the RTree.. no, I'm not
// kidding unfortunately

#define __RTREE_TEMPLATE_DEFN												\
template <																	\
	typename _bound_type,													\
	typename _leaf_type,													\
	std::size_t _dimensions,												\
	std::size_t _min_child_items,											\
	std::size_t _max_child_items,											\
	template <																\
		typename _b_nb,														\
		template <typename _bn_i, std::size_t _bn_mi> class _NodeType,		\
		template <typename _bni_k, typename _bni_i> class _NodeItemType,	\
		typename _b_l,														\
		typename _b_k,														\
		std::size_t _b_mi>													\
	class _Backend															\
>

#define __RTREE_TEMPLATE_FN		\
	_bound_type,				\
	_leaf_type,					\
	_dimensions,				\
	_min_child_items,			\
	_max_child_items,			\
	_Backend												



/**
	\class RTree
	\brief Implementation of an RTree with an R* index

	This is intended to be a templated implementation of an R* Tree, designed
	to create an efficient and (relatively) small spatial indexing container 
	in N dimensions. 

	Based on "The R*-Tree: An Efficient and Robust Access Method for Points 
	and Rectangles" by N. Beckmann, H.P. Kriegel, R. Schneider, and B. Seeger


	@tparam _bound_type		Type used to represent the bounds for the bounding box
	@tparam dimensions  	number of dimensions the bounding boxes are described in
	@tparam	min_child_items m, in the range 1 <= m <= M/2
	@tparam max_child_items M, which must be M >= m*2

*/
__RTREE_TEMPLATE_DEFN
class RTree {
public:

	typedef RTree<__RTREE_TEMPLATE_FN>					this_type;

	// expose template types
	enum {
		dimensions = _dimensions,
		min_child_items = _min_child_items,
		max_child_items = _max_child_items,
	};

	typedef RTreeBoundingBox<dimensions, _bound_type>	BoundingBox;
	
	// indexed by bounding box
	typedef BoundingBox									KeyType;
	typedef _leaf_type									LeafType;

	// shortcut definitions
	typedef TreeNodeBase								NodeBase;
	

	typedef TreeLeafItem<KeyType, LeafType>				LeafItem;
	typedef TreeNode<LeafItem, max_child_items> 		LeafNode;
	typedef typename LeafNode::ItemArray				LeafItemArray;
	typedef typename LeafItemArray::iterator			LeafItemIterator;

	// import types from the backend
	typedef _Backend<
					NodeBase, 
					RTreeNode, 
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
	
	// acceptor functors

	//! overlapping the specified bound
	typedef RTreeAcceptOverlapping<NodeItem, LeafItem>	AcceptOverlapping;

	//! enclosed in the specified bound
	typedef RTreeAcceptEnclosing<NodeItem, LeafItem>	AcceptEnclosing;

	//! accepts every single node
	typedef RTreeAcceptAny<NodeItem, LeafItem>			AcceptAny;
	
	// define iterator types (not STL compatible)
	typedef RTreeIterator<this_type, AcceptOverlapping>	OverlappingIterator;
	typedef RTreeIterator<this_type, AcceptEnclosing>	EnclosedIterator;
	typedef RTreeIterator<this_type, AcceptAny>			Iterator;
	
	
	template <typename this_type, typename Acceptor> 
	friend class RTreeIterator;
	
	
	// error codes
	enum ErrorCode {
		ERR_NONE = 0,
		ERR_CREATE_ROOT,
		ERR_GET_LEAFNODE,
		ERR_GET_NODE,
		ERR_GET_ROOT,
		ERR_CREATE_NODE_IN_SPLIT,
		ERR_CREATE_SPLIT_ROOT,
		ERR_GET_SPLIT_ROOT,
		ERR_EXTEND_BOUNDS_FAILED
	};



	//! Default constructor
	RTree()
	{ 
		assert(1 <= min_child_items && min_child_items <= max_child_items/2);
	}
	
	// destructor
	~RTree() {
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
	
	/// returns the last error code
	ErrorCode GetErrorCode() const
	{
		return m_lastError;
	}
	
	/// @}
	
	/// \name Query functions
	/// @{
	
	// find items that overlap a specified bounding box
	OverlappingIterator * FindOverlapping(const BoundingBox &bound)
	{
		return new OverlappingIterator(this, AcceptOverlapping(bound));
	}
	
	// find items that are enclosed in a specified bounding box
	EnclosedIterator * FindEnclosed(const BoundingBox &bound)
	{	
		return new EnclosedIterator(this, AcceptEnclosing(bound));
	}
	
	// iterate the entire tree
	Iterator * Begin()
	{
		return new Iterator(this, AcceptAny());
	}
	
	/// @}
	
	
	// Single insert function, adds a new item to the tree
	bool Insert(const BoundingBox &key, const LeafType &leaf) WARN_IF_UNUSED
	{
		m_lastError = ERR_NONE;
		
		// do not insert invalid bounding boxes
		if (!key.verify())
			return false;
	
		// ID1: Invoke Insert starting with the leaf level as a
		// parameter, to Insert a new data rectangle

		// create a new root node if necessary
		if (backend.m_treeHeight == 0)
		{
			// create a root node
			if (!backend.create_root_node())
			{
				SET_BAD_ERROR(ERR_CREATE_ROOT);
				return false;
			}
			
			// reserve memory
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
				OverflowItem * oi = 
			#endif
			
			InsertInternal(LeafItem(key, leaf), backend.m_rootId, 1);
			assert(!oi && "this should always be NULL");	
		}
			
		if (m_lastError != ERR_NONE)
			return false;

		// stretch the root bound -- this isn't used, but maybe it should
		// be at some point.. 
		//backend.m_rootItem.key.stretch(key);
			
		backend.m_size += 1;
		return true;
	}
	
	bool ExtendBounds(const BoundingBox &key, const LeafType &leaf) WARN_IF_UNUSED
	{
		m_lastError = ERR_NONE;
	
		// fast search first
		OverlappingIterator * it1 = new OverlappingIterator(this, AcceptOverlapping(key));
		
		if (!it1->end())
		{
			do
			{
				// find the object and extend the bounds 
				LeafType &l = it1->current().value;
				
				if (l == leaf)
				{
					//bool ret = it1->ExtendStackBounds(key);
					delete it1;
					return true;
				}
			} while (it1->next());
		}
		
		delete it1;

		// slow search next.. 
		Iterator * it2 = new Iterator(this, AcceptAny());
		
		if (!it2->end())
		{
			do
			{
				// find the object and extend the bounds 
				LeafType &l = it2->current().value;
				
				if (l == leaf)
				{
					//bool ret = it2->ExtendStackBounds(key);
					delete it2;
					return true;
				}

			} while (it2->next());
		}
		
		delete it2;

		// this should never happen
		SET_BAD_ERROR( ERR_EXTEND_BOUNDS_FAILED );
		return false;
	}

	
	size_type GetItemCount() const { return backend.m_size; }
	size_type GetHeight() const { return backend.m_treeHeight; }
	size_type GetDimensions() const { return dimensions; }
	
	bool Verify();
	
private:
	
	/**
		@note only pass this items that do not have leaves
		
		I took out the loop portion of this algorithm, so it only
		picks a subtree at that particular level. The rest is done
		recursively by insert

		After the subnode is picked, then its bounding box is stretched
		to include the bound selected. 
		
		@return id of next node
	*/
	IdType ChooseSubtree(Node * node, const size_type &level, const BoundingBox &bound)
	{
		assert(node);

		NodeItemIterator next_item;
		
		// If the child pointers in N point to leaves 
		if (level + 1 == backend.m_treeHeight)
		{
			// determine the minimum overlap cost
			if (max_child_items > (RTREE_CHOOSE_SUBTREE_P*2)/3  && node->items.size() > RTREE_CHOOSE_SUBTREE_P)
			{
				// ** alternative algorithm:
				// Sort the rectangles in N in increasing order of
				// the area enlargement needed to include the new
				// data rectangle
				
				// Let A be the group of the first p entrles
				std::partial_sort( node->items.begin(), node->items.begin() + RTREE_CHOOSE_SUBTREE_P, node->items.end(), 
					SortBoundedItemsByAreaEnlargement<NodeItem>(bound));
				
				// From the items in A, considering all items in
				// N, choose the leaf whose rectangle needs least
				// overlap enlargement
				
				next_item = std::min_element(node->items.begin(), node->items.begin() + RTREE_CHOOSE_SUBTREE_P,
					SortBoundedItemsByOverlapEnlargement<NodeItem>(bound));
			}
			else
			{
				// choose the leaf in N whose rectangle needs least
				// overlap enlargement to include the new data
				// rectangle Resolve ties by choosmg the leaf
				// whose rectangle needs least area enlargement, then
				// the leaf with the rectangle of smallest area
				
				next_item = std::min_element(node->items.begin(), node->items.end(),
					SortBoundedItemsByOverlapEnlargement<NodeItem>(bound));	
			}
		}
		else // if the chlld pointers in N do not point to leaves
		{
			// [determine the minimum area cost],
			// choose the leaf in N whose rectangle needs least
			// area enlargement to include the new data
			// rectangle. Resolve ties by choosing the leaf
			// with the rectangle of smallest area
				
			next_item =	std::min_element( node->items.begin(), node->items.end(),
					SortBoundedItemsByAreaEnlargement<NodeItem>(bound));
		}
		
		// I4: Adjust all covering rectangles in the insertion path
		// such that they are minimum bounding boxes
		// enclosing the children rectangles
		next_item->key.stretch(bound);
		node->is_dirty = true;
		
		return next_item->id;
	}
	
	struct OverflowItem {
		BoundingBox oldBound;			// bound for the old item, to be updated
		NodeItem * splitItem;			// the new item to add to that level
		LeafItemArray removed_items;

		OverflowItem(const BoundingBox &b, NodeItem * i) :
			oldBound(b), splitItem(i) {}

		~OverflowItem() { if (splitItem) delete splitItem; }
	};
	
	// inserts nodes recursively. As an optimization, the algorithm steps are
	// way out of order. :) If this returns something, then that item should
	// be added to the caller's level of the tree and then the object should
	// be deleted
	OverflowItem * InsertInternal(
		const LeafItem &leafItem, 
		const IdType node_id,
		const std::size_t level, 
		const bool firstInsert = true)
	{
		// normally we would stretch the rectangle of the retrieved
		// node here. However, we shall assume that it has already
		// been done for us (when the decent to the next level occurs),
		// since that value is stored in the level above (it is done in
		// ChooseSubTree)
		
		// CS2: If we're at a leaf, then use that level
		if (level == backend.m_treeHeight)
		{
			LeafNode * node = backend.get_leaf_node(node_id);
			
			// make sure there are no errors
			if (!node) { SET_BAD_ERROR(ERR_GET_LEAFNODE); return NULL; }
			
			assert(!node->items.empty());
		
			// I2: If N has less than M items, accommodate E in N
			node->items.push_back(leafItem);
			node->is_dirty = true;
			
			// see if we need to deal with overflow on this node: this is step I3
			return OverflowTreatment(node, level, firstInsert);
		}

		// I1: Invoke ChooseSubtree. with the level as a parameter,
		// to find an appropriate node N, m which to place the
		// new leaf E
	
		// grab the node from the cache
		Node * node = backend.get_node(node_id);
		
		if (!node) { SET_BAD_ERROR( ERR_GET_NODE ); return NULL; }
		
		assert(!node->items.empty());
	
		// ChooseSubtree also does I4
		IdType selected_id = ChooseSubtree(node, level, leafItem.key);
	
		// of course, this already does all of that recursively. we just need to
		// determine whether we need to split the overflow or not
		OverflowItem * overflowItem = InsertInternal( 
			leafItem, 
			selected_id, 
			level+1, 
			firstInsert );

		
		if (!overflowItem)
			return NULL;

		// finish any overflow stuff that is pending
		node = FinishOverflowOperation(overflowItem, node_id, selected_id);

		// see if we need to deal with overflow on this node: this is step I3
		return OverflowTreatment(node, level, firstInsert);
	}
	

	// this may invalidate the node passed to it
	template <typename NodeType>
	OverflowItem * OverflowTreatment(
		NodeType * node, 
		const std::size_t &level, 
		bool firstInsert)
	{
		// If N has M+1 items. invoke OverflowTreatment with the
		// level of N as a parameter [for reinsertion or split]
		if (node->items.size() <= max_child_items )
			return NULL;
	
		// I3: If OverflowTreatment was called and a split was
		// performed, propagate OverflowTreatment upwards
		// if necessary (this is done with the return values)
	
		// OT1: If the level is not the root level AND this is the first
		// call of OverflowTreatment in the given level during the 
		// insertion of one data rectangle, then invoke Reinsert
		if (level != 1 && firstInsert)
		{
			assert(level == backend.m_treeHeight);

			// this is a really ugly cast, but its perfectly valid
			// because the NodeType is a template and it will always be a
			// LeafNode if the assert() doesn't fail
			return QueueReinsert( (LeafNode*) (NodeBase*) node);
		}
		
		OverflowItem * overflowItem = Split(node, level);
		
		// check to see if we are splitting the root
		if (level == 1)
			return SplitRoot(overflowItem);
		
		return overflowItem;
	}
	
	/**
		This combines Split, ChooseSplitAxis, and ChooseSplitIndex into 
		one function as an optimization (they all share data structures,
		so it would be pointless to do all of that copying)
	
		This returns a SplitItem, which should be added to the items of the
		passed node's parent, and have the nodes bound expanded
	*/
	template <typename NodeType>
	OverflowItem * Split(NodeType * node, const std::size_t &level)
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
		

		const size_type n_items = node->items.size();
		const size_type distribution_count = n_items - 2*min_child_items + 1;
		
		size_type split_axis = dimensions+1, split_edge = 0, split_index = 0;
		int split_margin = 0;
		
		BoundingBox R1, R2;

		// these should always hold true
		assert(distribution_count > 0);
		assert(min_child_items + distribution_count-1 <= n_items);
		
		// S1: Invoke ChooseSplitAxis to determine the axis,
		// perpendicular to which the split 1s performed
		// S2: Invoke ChooseSplitIndex to determine the best
		// distribution into two groups along that axis
		
		// NOTE: We don't compare against node->bound, so it gets overwritten
		// at the end of the loop
		
		// CSA1: For each axis
		for (size_type axis = 0; axis < dimensions; axis++)
		{
			// initialize per-loop items
			int margin = 0;
			double overlap = 0, dist_area, dist_overlap;
			size_type dist_edge = 0, dist_index = 0;
		
			dist_area = dist_overlap = std::numeric_limits<double>::max();
			
			
			// Sort the items by the lower then by the upper
			// edge of their bounding box on this particular axis and 
			// determine all distributions as described . Compute S. the
			// sum of all margin-values of the different
			// distributions
		
			// lower edge == 0, upper edge = 1
			for (size_type edge = 0; edge < 2; edge++)
			{
				// sort the items by the correct key (upper edge, lower edge)
				if (edge == 0)
					std::sort(node->items.begin(), node->items.end(), SortBoundedItemsByFirstEdge<typename NodeType::Item>(axis));
				else
					std::sort(node->items.begin(), node->items.end(), SortBoundedItemsBySecondEdge<typename NodeType::Item>(axis));
		
				// Distributions: pick a point m in the middle of the thing, call the left
				// R1 and the right R2. Calculate the bounding box of R1 and R2, then 
				// calculate the margins. Then do it again for some more points	
				for (size_type k = 0; k < distribution_count; k++)
		        {
					double area = 0;
				
					// calculate bounding box of R1
					R1.reset();
					std::for_each(node->items.begin(), node->items.begin()+(min_child_items+k), StretchBoundingBox<typename NodeType::Item>(&R1));
							
					// then do the same for R2
					R2.reset();
					std::for_each(node->items.begin()+(min_child_items+k+1), node->items.end(), StretchBoundingBox<typename NodeType::Item>(&R2));
					
					
					// calculate the three values
					margin 	+= R1.edgeDeltas() + R2.edgeDeltas();
					area 	+= R1.area() + R2.area();		// TODO: need to subtract.. overlap?
					overlap =  R1.overlap(R2);
					
					
					// CSI1: Along the split axis, choose the distribution with the 
					// minimum overlap-value. Resolve ties by choosing the distribution
					// with minimum area-value. 
					if (overlap < dist_overlap || (overlap == dist_overlap && area < dist_area))
					{
						// if so, store the parameters that allow us to recreate it at the end
						dist_edge = 	edge;
						dist_index = 	min_child_items+k;
						dist_overlap = 	overlap;
						dist_area = 	area;
					}		
				}
			}
			
			// CSA2: Choose the axis with the minimum S as split axis
			if (split_axis == dimensions+1 || split_margin > margin )
			{
				split_axis 		= axis;
				split_margin 	= margin;
				split_edge 		= dist_edge;
				split_index 	= dist_index;
			}
		}
	
		// S3: Distribute the items into two groups
	
		// ok, we're done, and the best distribution on the selected split
		// axis has been recorded, so we just have to recreate it and
		// return the correct index
		
		if (split_edge == 0)
			std::sort(node->items.begin(), node->items.end(), SortBoundedItemsByFirstEdge<typename NodeType::Item>(split_axis));

		// only reinsert the sort key if we have to
		else if (split_axis != dimensions-1)
			std::sort(node->items.begin(), node->items.end(), SortBoundedItemsBySecondEdge<typename NodeType::Item>(split_axis));	
		
		// distribute the end of the array to the new node, then erase them from the original node
		new_node->items.assign(node->items.begin() + split_index, node->items.end());
		node->items.erase(node->items.begin() + split_index, node->items.end());
		
		// adjust the bounding box for each node resulting from the split
		BoundingBox new_node_bound;
		new_node_bound.reset();
		std::for_each(new_node->items.begin(), new_node->items.end(), StretchBoundingBox<typename NodeType::Item>(&new_node_bound));

		BoundingBox old_node_bound;
		old_node_bound.reset();
		std::for_each(node->items.begin(), node->items.end(), StretchBoundingBox<typename NodeType::Item>(&old_node_bound));

		// return the item that needs to be added to the level above 
		return new OverflowItem(old_node_bound, new NodeItem(new_node_bound, new_id));
	}
	
	
	// If OverflowTreatment caused a split of the root, create a new root
	OverflowItem * SplitRoot(OverflowItem * overflowItem )
	{
		// retain the id of the old root
		IdType old_root_id = backend.m_rootId;
		
		// create a new root node
		if (!backend.create_root_node())
		{
			SET_BAD_ERROR( ERR_CREATE_SPLIT_ROOT );
			return NULL;
		}
		
		// if we split the root -- it will always be a node
		Node * root = backend.get_node(backend.m_rootId);

		if (!root) { SET_BAD_ERROR( ERR_GET_SPLIT_ROOT ); return NULL; }

		// fill in the new root using the information in overflowItem,
		// the old root gets the 'old' bound, and the other split item
		// is inserted after that
		root->items.push_back(NodeItem(overflowItem->oldBound, old_root_id));
		root->items.push_back(*(overflowItem->splitItem));
		root->is_dirty = true;

		// and done
		delete overflowItem;
		return NULL;
	}
	
	
	// This routine is used to queue the opportunistic reinsertion that the
	// R* algorithm calls for
	OverflowItem * QueueReinsert(LeafNode * node)
	{
		OverflowItem * overflowItem = new OverflowItem(BoundingBox(), NULL);

		const size_type n_items = node->items.size();
		const size_type p = (size_type)((double)n_items * RTREE_REINSERT_P) > 0 ? (size_type)((double)n_items * RTREE_REINSERT_P) : 1;
		
		// RI1 For all M+l items of a node N, compute the distance
		// between the centers of their rectangles and the center
		// of the bounding rectangle of N
		assert(n_items == max_child_items + 1);

		// recompute this, since we don't have the bounding box for N
		// (though it exists somewhere, we can't guarantee it will
		// still exist by the time we get here)
		BoundingBox bound;
		bound.reset();
		std::for_each(node->items.begin(), node->items.end(), StretchBoundingBox<LeafItem>(&bound));
		
		// RI2: Sort the items in increasing order of their distances
		// computed in RI1
		std::partial_sort(node->items.begin(), node->items.end() - p, node->items.end(), 
			SortBoundedItemsByDistanceFromCenter<LeafItem>(bound));
			
		// RI3.A: Remove the last p items from N
		overflowItem->removed_items.assign(node->items.end() - p, node->items.end());
		node->items.erase(node->items.end() - p, node->items.end());
		
		node->is_dirty = true;

		// RI3.B: adjust the bounding rectangle of N
		overflowItem->oldBound.reset();
		std::for_each(node->items.begin(), node->items.end(), StretchBoundingBox<LeafItem>(&overflowItem->oldBound));

		// return the information to be done later
		return overflowItem;
	}




	/*
		Used by InsertInternal to finish off any overflow operation

		The reason this exists is because split() and reinsert() both
		require the 'current' node bounding box to be modified, and 
		the bounding box does not reside on the node itself, but in 
		its parent. 

		So, this does a few things:

			a) always sets the bound for the item with id = id
			
			b) If there is a split item, it propagates the split

			c) If there is a list of nodes to be reinserted, then
			it goes ahead and does the actual reinsertion

	*/
	Node * FinishOverflowOperation(OverflowItem * overflowItem, IdType node_id, IdType bound_id)
	{
		Node * node = backend.get_node(node_id);

		// set the bound for the selected item
		SearchForItemAndSetBound(node, bound_id, overflowItem);
			
		// propagate the split: this gets joined to the list of items 
		// at this level
		if (overflowItem->splitItem)
			node->items.push_back(*(overflowItem->splitItem));

		node->is_dirty = true;

		// if there are items to reinsert, do so now
		if (!overflowItem->removed_items.empty())
		{
			// RI4: In the sort, defined in RI2, starting with the 
			// minimum distance (= close reinsert), invoke Insert 
			// to reinsert the items
			LeafItemIterator end = overflowItem->removed_items.end();
			
			for (LeafItemIterator it = overflowItem->removed_items.begin(); it != end; it++)
				InsertInternal( *it, backend.m_rootId, 1, false);
				
			// we have to assume that the current node id may have been invalidated
			node = backend.get_node(node_id);
		}

		delete overflowItem;
		return node;
	}


	// used for recovering from a split
	void SearchForItemAndSetBound(Node * node, IdType node_id, OverflowItem * overflowItem )
	{
		const NodeItemIterator end = node->items.end();
		for (NodeItemIterator it = node->items.begin(); it != end; it++)
			if (it->id == node_id)
			{	
				it->key = overflowItem->oldBound;
				return;
			}

		assert(false && "should not get here");
	}


	Backend		backend;
	ErrorCode 	m_lastError;	
	
	
};

#undef RTREE_REINSERT_P
#undef RTREE_CHOOSE_SUBTREE_P

#undef SET_BAD_ERROR


#endif

