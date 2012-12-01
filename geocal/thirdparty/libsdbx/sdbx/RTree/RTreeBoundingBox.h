/*
 *  SDBX
 *	$Id: RTreeBoundingBox.h 1844 2009-03-31 14:19:13Z virtuald $
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


#ifndef SDBX_RTREE_BOUNDING_BOX_H
#define SDBX_RTREE_BOUNDING_BOX_H

#include <string>
#include <sstream>


#ifdef USE_MSVC_MEMORY_TRACING
	// this enables advanced memory leak detection
	#include <wx/msw/msvcrt.h>
#endif

template <std::size_t _dimensions, typename bound_type>
struct RTreeBoundingBox {

	typedef RTreeBoundingBox<_dimensions, bound_type>	this_type;

	// accessed externally
	enum { 
		dimensions = _dimensions,
		record_size = sizeof(bound_type) * 2 * _dimensions			// serialization
	};

	// edges[x].first is low value, edges[x].second is high value
	std::pair<bound_type, bound_type> edges[dimensions];
	
	RTreeBoundingBox() { reset(); }

	// serialization constructor
	explicit RTreeBoundingBox(const unsigned char * const buffer)
	{
		for (std::size_t axis = 0; axis < dimensions; axis++)
		{
			edges[axis].first =  BUFFER_REF(bound_type, axis*sizeof(bound_type)*2);
			edges[axis].second = BUFFER_REF(bound_type, axis*sizeof(bound_type)*2+sizeof(bound_type));
		}
	}

	/// Serializer
	void Serialize(unsigned char * buffer) const
	{
		for (std::size_t axis = 0; axis < dimensions; axis++)
		{
			SERIALIZE(bound_type, axis*sizeof(bound_type)*2, edges[axis].first);
			SERIALIZE(bound_type, axis*sizeof(bound_type)*2+sizeof(bound_type), edges[axis].second);
		}
	}
	
	
	// forces all edges to their extremes so we can stretch() it
	void reset()
	{
		for (std::size_t axis = 0; axis < dimensions; axis++)
		{
			edges[axis].first = std::numeric_limits<bound_type>::max();
			edges[axis].second = std::numeric_limits<bound_type>::min();
		}
	}
	
	// verifies that the bounding box is valid
	bool verify() const
	{
		for (std::size_t axis = 0; axis < dimensions; axis++)
		{
			if (edges[axis].second < edges[axis].first)
			{
				assert(0 && "invalid bounding box!");
				return false;
			}
		}
		
		return true;
	}
	
	
	// returns a new bounding box that has the maximum boundaries
	static RTreeBoundingBox MaximumBounds()
	{
		this_type bound;
		bound.reset();
		return bound;
	}
	

	// fits another box inside of this box, returns true if a stretch occured
	bool stretch(const this_type &bb)
	{
		bool ret = false;
		
		for (std::size_t axis = 0; axis < dimensions; axis++)
		{
			
			if (edges[axis].first > bb.edges[axis].first)
			{
				edges[axis].first = bb.edges[axis].first;
				ret = true;
			}
		
			if (edges[axis].second < bb.edges[axis].second)
			{
				edges[axis].second = bb.edges[axis].second;
				ret = true;
			}
		}
			
		return ret;
	}
	
	// the sum of all deltas between edges
	inline int edgeDeltas() const
	{
		int distance = 0;
		for (std::size_t axis = 0; axis < dimensions; axis++)
			distance += edges[axis].second - edges[axis].first;
			
		return distance;
	}
	
	// calculates the area of a bounding box
	inline double area() const
	{
		double area = 1;
		for (std::size_t axis = 0; axis < dimensions; axis++)
			area *= (double)(edges[axis].second - edges[axis].first);
		
		return area;
	}
	
	// this determines if a bounding box is fully contained within this bounding box
	inline bool encloses(const this_type& bb) const
	{
		// if (y1 < x1 || x2 < y2)
		for (std::size_t axis = 0; axis < dimensions; axis++)
			if (bb.edges[axis].first < edges[axis].first || edges[axis].second < bb.edges[axis].second)
				return false;
		
		return true;
	}
	
	// a quicker way to determine if two bounding boxes overlap
	inline bool overlaps(const this_type& bb) const
	{
		// do it this way so theres no equal signs (in case of doubles)
		// if (!(x1 < y2) && !(x2 > y1))
		for (std::size_t axis = 0; axis < dimensions; axis++)
		{		
			if (!(edges[axis].first < bb.edges[axis].second) || !(bb.edges[axis].first < edges[axis].second))
				return false;
		}

		return true;
	}
	
	// calculates the total overlapping area of two boxes
	double overlap(const this_type& bb) const
	{
		double area = 1.0;
		for (std::size_t axis = 0; area && axis < dimensions; axis++)
		{
			// this makes it easier to understand
			const int x1 = edges[axis].first;
			const int x2 = edges[axis].second;
			const int y1 = bb.edges[axis].first;
			const int y2 = bb.edges[axis].second;
		
			// left edge outside left edge
			if (x1 < y1)
			{
				// and right edge inside left edge
				if (y1 < x2)
				{
					// right edge outside right edge
					if (y2 < x2)
						area *= (double)( y2 - y1 );
					else
						area *= (double)( x2 - y1 );
						
					continue;
				}
			}
			// right edge inside left edge
			else if (x1 < y2)
			{
				// right edge outside right edge
				if (x2 < y2)
					area *= (double)( x2 - x1 );
				else
					area *= (double)( y2 - x1 );
					
				continue;
			}
			
			// if we get here, there is no overlap
			return 0.0;
		}

		return area;
	}
	
	// sums the total distances from the center of another bounding box
	double distanceFromCenter(const this_type& bb) const
	{
		double distance = 0, t;
		for (std::size_t axis = 0; axis < dimensions; axis++)
		{
			t = ((double)edges[axis].first + (double)edges[axis].second + 
			     (double)bb.edges[axis].first + (double)bb.edges[axis].second)
				 /2.0;
			distance += t*t;
		}
			
		return distance;
	}
	
	// determines if two bounding boxes are identical
	bool operator==(const this_type& bb)
	{
		for (std::size_t axis = 0; axis < dimensions; axis++)
			if (edges[axis].first != bb.edges[axis].first || edges[axis].second != bb.edges[axis].second)
				return false;
			
		return true;
	}
	
	
	// very slow, use for debugging only
	std::string ToString() const
	{
		std::stringstream name("");
		name << "[";
		for (std::size_t axis = 0; axis < dimensions; axis++)
		{
			name << "(" << edges[axis].first << "," << edges[axis].second << ")";
			if (axis != dimensions -1)
				name << ",";
		}
		name << "]";
		
		return name.str();
	}
};


/********************************************************************
 * Functor used to iterate over a set and stretch a
 * bounding box
 ********************************************************************/

// for_each(items.begin(), items.end(), StretchBoundedItem::KeyType(bound));
template <typename BoundedItem>
struct StretchBoundingBox : 
	public std::unary_function< const BoundedItem&, void >
{
	typename BoundedItem::KeyType * m_bound;
	explicit StretchBoundingBox(typename BoundedItem::KeyType * bound) : m_bound(bound) {}

	void operator() (const BoundedItem &item)
	{
		m_bound->stretch(item.key);
	}
};


/********************************************************************
 * R* Tree related functors used for sorting BoundedItems
 *
 * The key in each BoundedItem is assumed to be a BoundingBox
 *
 * @todo Take advantage of type traits
 ********************************************************************/

template <typename BoundedItem>
struct SortBoundedItemsByFirstEdge : 
	public std::binary_function< const BoundedItem&, const BoundedItem&, bool >
{
	const std::size_t m_axis;
	explicit SortBoundedItemsByFirstEdge (const std::size_t axis) : m_axis(axis) {}
	
	bool operator() (const BoundedItem& bi1, const BoundedItem& bi2) const 
	{
		return bi1.key.edges[m_axis].first < bi2.key.edges[m_axis].first;
	}
};

template <typename BoundedItem>
struct SortBoundedItemsBySecondEdge : 
	public std::binary_function< const BoundedItem&, const BoundedItem&, bool >
{
	const std::size_t m_axis;
	explicit SortBoundedItemsBySecondEdge (const std::size_t axis) : m_axis(axis) {}

	bool operator() (const BoundedItem& bi1, const BoundedItem& bi2) const 
	{
		return bi1.key.edges[m_axis].second < bi2.key.edges[m_axis].second;
	}
};


template <typename BoundedItem>
struct SortBoundedItemsByDistanceFromCenter : 
	public std::binary_function< const BoundedItem&, const BoundedItem&, bool >
{
	const typename BoundedItem::KeyType m_center;
	explicit SortBoundedItemsByDistanceFromCenter(const typename BoundedItem::KeyType&center) : m_center(center) {}

	bool operator() (const BoundedItem& bi1, const BoundedItem& bi2) const 
	{
		return bi1.key.distanceFromCenter(m_center) < bi2.key.distanceFromCenter(m_center);
	}
};

template <typename BoundedItem>
struct SortBoundedItemsByAreaEnlargement : 
	public std::binary_function< const BoundedItem&, const BoundedItem&, bool >
{
	const double area;
	explicit SortBoundedItemsByAreaEnlargement(const typename BoundedItem::KeyType &center) : area(center.area()) {}

	bool operator() (const BoundedItem &bi1, const BoundedItem &bi2) const 
	{
		return area - bi1.key.area() < area - bi2.key.area();
	}
};

template <typename BoundedItem>
struct SortBoundedItemsByOverlapEnlargement : 
	public std::binary_function< const BoundedItem&, const BoundedItem&, bool >
{
	const typename BoundedItem::KeyType m_center;
	explicit SortBoundedItemsByOverlapEnlargement(const typename BoundedItem::KeyType &center) : m_center(center) {}

	bool operator() (const BoundedItem& bi1, const BoundedItem& bi2) const 
	{
		return bi1.key.overlap(m_center) < bi2.key.overlap(m_center);
	}
};


#endif
