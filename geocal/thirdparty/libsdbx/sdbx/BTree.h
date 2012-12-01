/*
 *  SDBX
 *	$Id: BTree.h 1841 2009-01-04 18:38:58Z virtuald $
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

 
#ifndef SDBX_BTREE_H
#define SDBX_BTREE_H

// needed STL includes
#include <algorithm>
#include <functional>
#include <cassert> 
#include <vector>

#include "Serialization.h"

// support stuff
#include "support/is_sorted.h"	// MS STL doesn't have this for some reason
// #include "support/pstdint.h"
#include <stdint.h>
#include "support/TreeNode.h"
#include "support/TreeItem.h"
#include "support/utility.h"

// tree internals
#include "BTree/BTreeTraverser.h"
#include "BTree/BTreeNode.h"

#include "BTree/BTree.h"

// files dependent on the tree
#include "BTree/BTreeVerify.h"

#endif

