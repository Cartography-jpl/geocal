/*
 *  SDBX
 *	$Id: RTree.h 1842 2009-01-05 01:17:04Z virtuald $
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
 */
 
 
#ifndef SDBX_RTREE_H
#define SDBX_RTREE_H

// needed STL includes
#include <algorithm>
#include <cassert>
#include <functional>
#include <limits>
#include <vector>

#include "Serialization.h"

// #include "support/pstdint.h"
#include <stdint.h>
#include "support/TreeNode.h"
#include "support/TreeItem.h"
#include "support/utility.h"

// R* tree internals
#include "RTree/RTreeBoundingBox.h"
#include "RTree/RTreeNode.h"
#include "RTree/RTreeVisitor.h"
#include "RTree/RTreeIterator.h"

#include "RTree/RTree.h"

#include "RTree/RTreeVerify.h"

#endif

