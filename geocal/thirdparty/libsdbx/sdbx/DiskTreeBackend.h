/*
 *  SDBX
 *	$Id: DiskTreeBackend.h 1841 2009-01-04 18:38:58Z virtuald $
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

 
	Implements the things needed to make a disk-based tree work.

*/

#ifndef SDBX_DISK_TREE_BACKEND_H
#define SDBX_DISK_TREE_BACKEND_H

#ifdef USE_MSVC_MEMORY_TRACING
	// this enables advanced memory leak detection
	#include <wx/msw/msvcrt.h>
#endif

#include "lru.h"
#include "SerializedFile.h"



#define DISKTREE_NODE_MAGIC "DISKTREE_ND_01"	// disk tree standard signature
#define DISKTREE_LEAF_MAGIC "DISKTREE_LF_01"	// disk tree standard signature
#define DISKTREE_CUSTOM_SIG_SZ 16 	// size of custom signature


#define SET_BAD_ERROR( code )	\
	m_lastError = code;			\
	assert(0 && #code)


struct DiskTreeBackendArgs {
	wxFileName 	filename;			//! filename to open (no extension)
	
	wxString	signature;			//! signature to embed in file
									//! fixed size: must be less than 16 chars
	
	std::size_t	nodeCacheSz;		//! size of cache for nodes
	std::size_t	leafNodeCacheSz;	//! size of cache for leafNodes
	
	// constructor
	DiskTreeBackendArgs(
			const wxFileName &_filename,
			const wxString &_signature,
			std::size_t	_nodeCacheSz,
			std::size_t	_leafNodeCacheSz
		) :
		filename(_filename),
		signature(_signature),
		nodeCacheSz(_nodeCacheSz),
		leafNodeCacheSz(_leafNodeCacheSz)
	{}
};


/**
	\class DiskTree
	
	So the disk version of this has two files: the node index file, and the
	leaf file... the tree nodes are stored in the node index file. The leaf
	nodes are stored in the lead index file.
	
	This uses an LRU cache to optimize memory usage, as opposed to keeping
	every single node in memory. Items are automatically written to disk.
	SerializedFile is used to actually read/write the items, so all items
	used with this class must be serializable using the methods in
	SerializationGeneric.h

	Two files are created, filename.ldx (leaf node file) and filename.ndx
	(node file), and they contain a special signature block at the start
	to ensure that they were created with appropriate versions/etc of
	this class. filename is specified using the DiskTreeBackendArgs

	You do not need to directly use this class, this is designed to be
	used internally by the tree structures. However, you do need to provide 
	the	type to the tree.
	
	@todo Need to determine optimal block sizes/etc
*/
template <
	typename _NodeBase,
	template <typename _i, std::size_t _mi> class _NodeType,
	template <typename _k, typename _v> class _NodeItemType,
	typename _LeafNode,
	typename _KeyType,
	std::size_t _max_node_items>
class DiskTreeBackend {
public:
	
	// types provided to us
	typedef _NodeBase									NodeBase;
	typedef _LeafNode									LeafNode;


	// types that the backend needs to define
	typedef UniqueId									IdType;
	typedef uint64_t									size_type;
	
	typedef _NodeItemType<_KeyType, IdType >			NodeItem;
	typedef _NodeType<NodeItem, _max_node_items>		Node;
	
	// args for the initialization function
	typedef DiskTreeBackendArgs							Args;
	
	
	enum {
		// defines the descriptor block at the beginning of the file
		node_header_sz = 
			sizeof(uint16_t) +					// size of header
			(sizeof(DISKTREE_NODE_MAGIC)-1) + 	// signature
			DISKTREE_CUSTOM_SIG_SZ +			// custom signature
				
			sizeof(uint32_t) +					// height of tree
			record<IdType>::size +				// node count (max id)
			sizeof(uint32_t) +					// max items per node
			
			record<IdType>::size				// root node id
		,
		
		leaf_header_sz = 
			sizeof(uint16_t) +					// size of leaf header
			(sizeof(DISKTREE_LEAF_MAGIC)-1) +	// signature
			DISKTREE_CUSTOM_SIG_SZ +			// custom signature
			
			record<IdType>::size +				// leaf node count (max id)
			sizeof(size_type) +					// total leaf items
			sizeof(uint32_t)					// max node items
	};
	

	// cache type
	typedef LRUCache<Node, IdType, LRUDeleteObject>		NodeCache;
	typedef typename NodeCache::Node					NodeCacheNode;
	typedef LRUCache<LeafNode, IdType, LRUDeleteObject>	LeafNodeCache;
	typedef typename LeafNodeCache::Node				LeafCacheNode;
	
	// serializer type
	typedef SerializedFile < Node, node_header_sz > 	NodeFile;
	typedef SerializedFile < LeafNode, leaf_header_sz > LeafNodeFile; 

	enum ErrorCode {
		ERR_NONE = 0,
		ERR_OPEN_NODE_FILE = 1000,
		ERR_OPEN_LEAFNODE_FILE,
		ERR_WRITE_NODE_HEADER,
		ERR_WRITE_LEAF_HEADER,
		ERR_READ_NODE_HEADER,
		ERR_READ_LEAF_HEADER,
		ERR_CREATE_ROOT_WRITE_LEAF,
		ERR_CREATE_ROOT_WRITE_NODE,
		ERR_GET_NODE_LOAD,
		ERR_GET_LEAFNODE_LOAD,
		ERR_CREATE_NODE_WRITE,
		ERR_CREATE_LEAFNODE_WRITE
	};


	DiskTreeBackend() :
		m_treeHeight(0),
		m_size(0),
		m_initialized(false),
		
		m_node_count((wxULongLong_t)0),
		m_leaf_node_count((wxULongLong_t)0),
		
		m_nodeCache(NULL),
		m_nodeFile(NULL),
		m_root(NULL),

		m_leafNodeCache(NULL),
		m_leafNodeFile(NULL),

		m_lastError(ERR_NONE)
	{}

	~DiskTreeBackend() 
	{
		// cycle through the node cache, and ensure that anything
		// dirty is written back to disk
		
		if (m_initialized)
		{
			if (m_root)
			{
				if (m_treeHeight == 1)
				{
					LeafNode * node = static_cast<LeafNode*>(m_root);
					if (node->is_dirty)
						m_leafNodeFile->SaveObjects(m_rootId, 1, node);
					delete node;
				}
				else
				{
					Node * node = static_cast<Node*>(m_root);
					if (node->is_dirty)
						m_nodeFile->SaveObjects(m_rootId, 1, node);
					delete node;
				}
			}
		
			// clear the cache, and write all nodes
			ClearCache(m_nodeCache, m_nodeFile);
			ClearCache(m_leafNodeCache, m_leafNodeFile);
		
			// write the headers to disk -- have to ignore errors here
			if (!WriteNodeHeader()){}
			if (!WriteLeafHeader()){}
		}

		if (m_leafNodeFile)
			delete m_leafNodeFile;
		if (m_leafNodeCache)
			delete m_leafNodeCache;
			
		if (m_nodeFile)
			delete m_nodeFile;
		if (m_nodeCache)
			delete m_nodeCache;
	}
	
	/**
		This initializes a file-based R-tree with R* indexing.

		@param filename		Filename of tree
	*/
	bool Initialize(const Args &args) WARN_IF_UNUSED
	{
		wxFileName filename = args.filename;
		custom_signature = args.signature;

		// load the file
		filename.SetExt(wxT("ndx"));
		m_nodeFile = new NodeFile(filename);
		
		// see if it opened ok
		if (!m_nodeFile->IsOpened())
		{
			SET_BAD_ERROR( ERR_OPEN_NODE_FILE );
			return false;
		}
			
		// try the other file next
		filename.SetExt(wxT("ldx"));
		m_leafNodeFile = new LeafNodeFile(filename);

		if (!m_leafNodeFile->IsOpened())
		{
			SET_BAD_ERROR( ERR_OPEN_LEAFNODE_FILE );
			return false;
		}

		if (m_nodeFile->GetFFile()->Length() == 0)
		{
			// new file, write the headers
			if (!WriteNodeHeader())
			{
				SET_BAD_ERROR( ERR_WRITE_NODE_HEADER );
				return false;
			}
			
			if (!WriteLeafHeader())
			{
				SET_BAD_ERROR( ERR_WRITE_LEAF_HEADER );
				return false;
			}
		}
		else
		{
			// ok, ensure that the thing we're loading is what we expect, or
			// if theres a mismatch
			// new file, write the headers
			if (!ReadNodeHeader())
			{
				SET_BAD_ERROR( ERR_READ_NODE_HEADER );
				return false;
			}
			
			if (!ReadLeafHeader())
			{
				SET_BAD_ERROR( ERR_READ_LEAF_HEADER );
				return false;
			}
		}
		
		m_nodeCache = new NodeCache(args.nodeCacheSz);
		m_leafNodeCache = new LeafNodeCache(args.leafNodeCacheSz);

		m_initialized = true;
		return true;
	}


	IdType GetNodeCount() const { return m_node_count; }
	IdType GetLeafNodeCount() const { return m_leaf_node_count; }

	ErrorCode GetErrorCode() const 
	{ 
		return m_lastError; 
	}
	
	/// This deletes the database. After calling this, you must call 
	/// initialize to use the object again. 
	void Destroy()
	{
		if (m_initialized)
		{
			if (m_root)
			{
				if (m_treeHeight == 1)
					delete static_cast<LeafNode*>(m_root);
				else
					delete static_cast<Node*>(m_root);
			}

			delete m_nodeCache; m_nodeCache = NULL;
			delete m_leafNodeCache; m_leafNodeCache = NULL;
	
			// delete the files
			m_nodeFile->Delete();
			delete m_nodeFile; m_nodeFile = NULL;
		
			m_leafNodeFile->Delete();
			delete m_leafNodeFile; m_leafNodeFile = NULL;
		
			m_initialized = false;
		}
	}
	
	
	
	// this creates a new root node and places its ID in m_rootId, you
	// MUST hold onto the old ID and place it in the new root node (which
	// you will need to retrieve using get_node or get_leaf_node). Each time
	// you call this the tree height is incremented
	bool create_root_node() WARN_IF_UNUSED
	{
		// first, put the old node into the cache
		// then create a new node and place it in the correct places

		if (m_treeHeight == 0)
		{
			assert(!m_root);
			m_rootId = new_leaf_node_id();
			m_root = new LeafNode();
		}
		else 
		{
			assert(m_root);

			if (m_treeHeight == 1)
			{
				LeafCacheNode * removed = m_leafNodeCache->add(m_rootId, static_cast<LeafNode*>(m_root));
				if (removed && !write_leaf(removed))
				{
					SET_BAD_ERROR( ERR_CREATE_ROOT_WRITE_LEAF );
					return false;
				}
			}
			else
			{
				NodeCacheNode * removed = m_nodeCache->add(m_rootId, static_cast<Node*>(m_root));
				if (removed && !write_node(removed))
				{
					SET_BAD_ERROR( ERR_CREATE_ROOT_WRITE_NODE);
					return false;
				}
			}

			m_rootId = new_node_id();
			m_root = new Node();
		}
		
		// dont increment the height if its not null.. 
		return (m_root != NULL && ++m_treeHeight);
	}

	

	IdType new_node_id()
	{	
		// TODO: support returning deleted nodes when we support
		// removal
		IdType ret = ++m_node_count;
		return ret;
	}

	//! Creates a new node with the specified id, and adds it to the cache
	Node * create_node(const IdType &id)
	{
		Node * node = new Node();
	
		// add it to the cache
		NodeCacheNode * removed = m_nodeCache->add(id, node);
		
		// if an item is returned, then an item was removed 
		// from the cache, so deal with it
		if (removed && !write_node(removed))
		{
			SET_BAD_ERROR( ERR_CREATE_NODE_WRITE );
			return NULL;
		}
		
		return node;
	}
	

	/*
		gets a node from the cache, or loads it from file
	*/
	Node * get_node(const IdType &id)
	{
		// check for m_treeHeight > 1 is implicit:
		assert(m_treeHeight > 1 && "should not happen");
		
		if (id == m_rootId)
			return static_cast<Node*>(m_root);
	
		Node * node = m_nodeCache->get(id);
		if (!node)
		{
			node = create_node(id);
			if (!node)
				return NULL;
			
			// load one object from file
			if (m_nodeFile->LoadObjects(id, 1, node) != 1)
			{
				SET_BAD_ERROR( ERR_GET_NODE_LOAD );
				return NULL;
			}
		}
		
		return node;
	}
	
	
	IdType new_leaf_node_id() {
		// TODO: support returning deleted nodes when we support
		// removal
		IdType ret = ++m_leaf_node_count;
		return ret;
	}

	LeafNode * create_leaf_node(const IdType &id) 
	{
		LeafNode * node = new LeafNode();
	
		// add it to the cache
		LeafCacheNode * removed = m_leafNodeCache->add(id, node);
		
		// if an item is returned, then an item was removed 
		// from the cache, so deal with it
		if (removed && !write_leaf(removed))
		{
			SET_BAD_ERROR( ERR_CREATE_LEAFNODE_WRITE );
			return NULL;
		}
		
		return node;
	}

	LeafNode * get_leaf_node(const IdType &id) 
	{
		if (m_treeHeight == 1 && id == m_rootId)
			return static_cast<LeafNode*>(m_root);
	
		LeafNode * node = m_leafNodeCache->get(id);
		if (!node)
		{
			node = create_leaf_node(id);
			if (!node)
				return NULL;
			
			// load one object from file
			if (m_leafNodeFile->LoadObjects(id, 1, node) != 1)
			{
				SET_BAD_ERROR( ERR_GET_LEAFNODE_LOAD );
				return NULL;
			}
		}
		
		return node;
	}
	
	// the backend defines these variables for the tree
	IdType 		m_rootId;
	uint32_t	m_treeHeight;	// this shouldn't exceed 2^32
	size_type	m_size;			// number of nodes, total
	
private:

	bool write_node(NodeCacheNode * node) WARN_IF_UNUSED
	{
		assert(node && "this should never be NULL");
		bool ret = true;
	
		// see if it needs to be written back
		if (node->object->is_dirty)
			ret = (m_nodeFile->SaveObjects(node->id, 1, node->object) == 1);
		
		delete node;
		return ret;
	}
	
	bool write_leaf(LeafCacheNode * node) WARN_IF_UNUSED
	{
		assert(node && "this should never be NULL");
		bool ret = true;
	
		// see if it needs to be written back
		if (node->object->is_dirty)
			ret = (m_leafNodeFile->SaveObjects(node->id, 1, node->object) == 1);
		
		delete node;
		return ret;
	}
	

	bool WriteNodeHeader() WARN_IF_UNUSED
	{
		unsigned char buffer[node_header_sz];
		int offset = 0;

		// write header size
		BUFFER_REF(uint16_t, offset) = node_header_sz;
		offset += sizeof(uint16_t);
		
		// our signature (no NULL byte)
		strncpy((char*)(buffer + offset), DISKTREE_NODE_MAGIC, sizeof(DISKTREE_NODE_MAGIC)-1);
		offset += (sizeof(DISKTREE_NODE_MAGIC)-1);
		
		// custom signature
		memset(buffer + offset, 0, DISKTREE_CUSTOM_SIG_SZ);
		strncpy((char*)(buffer + offset), custom_signature.mb_str(), DISKTREE_CUSTOM_SIG_SZ);
		offset += DISKTREE_CUSTOM_SIG_SZ;
		
		// tree height
		BUFFER_REF(uint32_t, offset) = (uint32_t)m_treeHeight;
		offset += sizeof(uint32_t);
		
		// node count
		record<IdType>::serialize(m_node_count, buffer + offset);
		offset += record<IdType>::size;
		
		// max node items
		BUFFER_REF(uint32_t, offset) = (uint32_t)_max_node_items;		
		offset += sizeof(uint32_t);
		
		// root node item (if its valid -- valgrind complains)
		if (m_root)
			record<IdType>::serialize(m_rootId, buffer + offset);
		else
			memset(&buffer[offset], 0, record<IdType>::size);
		offset += record<IdType>::size;


		assert(offset == node_header_sz);
		
		// and write it to file
		wxFFile * file = m_nodeFile->GetFFile();
		file->Seek(0);
		if (file->Write(buffer, node_header_sz) != node_header_sz)
			return false;
		file->Flush();
		
		return true;
	}
	
	bool WriteLeafHeader() WARN_IF_UNUSED
	{
		unsigned char buffer[leaf_header_sz];
		int offset = 0;

		// write header size
		BUFFER_REF(uint16_t, offset) = leaf_header_sz;
		offset += sizeof(uint16_t);
		
		// our signature (no NULL byte)
		strncpy((char*)(buffer + offset), DISKTREE_LEAF_MAGIC, sizeof(DISKTREE_LEAF_MAGIC)-1);
		offset += (sizeof(DISKTREE_LEAF_MAGIC)-1);
		
		// custom signature
		memset(buffer + offset, 0, DISKTREE_CUSTOM_SIG_SZ);
		strncpy((char*)(buffer + offset), custom_signature.mb_str(), DISKTREE_CUSTOM_SIG_SZ);
		offset += DISKTREE_CUSTOM_SIG_SZ;
		
		// leaf count
		record<IdType>::serialize(m_leaf_node_count, buffer + offset);
		offset += record<IdType>::size;

		// number of leaf items
		BUFFER_REF(size_type, offset) = m_size;
		offset += sizeof(size_type);
		
		// max node items
		BUFFER_REF(uint32_t, offset) = (uint32_t)_max_node_items;		
		offset += sizeof(uint32_t);

		
		assert(offset == leaf_header_sz);
		
		// and write it to file
		wxFFile * file = m_leafNodeFile->GetFFile();
		file->Seek(0);
		if (file->Write(buffer, leaf_header_sz) != leaf_header_sz)
			return false;
		
		file->Flush();
		return true;
	}
	
	
	bool ReadNodeHeader() WARN_IF_UNUSED
	{
		unsigned char buffer[node_header_sz];
		wxFFile * file = m_nodeFile->GetFFile();
		
		file->Seek(0);
		if (file->Read(buffer, node_header_sz) != node_header_sz)
			return false;
	
		int offset = 0;
		
		// header sz
		if (BUFFER_REF(uint16_t, offset) != node_header_sz)
			return false;
		offset += sizeof(uint16_t);
		
		// signature
		if (strncmp((char*)(buffer + offset), DISKTREE_NODE_MAGIC, sizeof(DISKTREE_NODE_MAGIC)-1))
			return false;
		offset += (sizeof(DISKTREE_NODE_MAGIC)-1);
		
		// custom signature
		if (strncmp((char*)(buffer + offset), custom_signature.mb_str(), DISKTREE_CUSTOM_SIG_SZ))
			return false;
		offset += DISKTREE_CUSTOM_SIG_SZ;
		
		// height
		m_treeHeight = BUFFER_REF(uint32_t, offset);
		offset += sizeof(uint32_t);
			
		// node count
		m_node_count = record<IdType>::deserialize(buffer + offset);;
		offset += record<IdType>::size;
		
		// max node items
		if (BUFFER_REF(uint32_t, offset) != (uint32_t)_max_node_items)
			return false;
		offset += sizeof(uint32_t);
			
		// root node id
		m_rootId = record<IdType>::deserialize(buffer + offset);;
		offset += record<IdType>::size;
	
	
		assert(offset == node_header_sz);
		
		// try to load the root node of the tree from file
		if (m_treeHeight > 1)
		{
			m_root = new Node();
			if (!m_nodeFile->LoadObjects(m_rootId, 1, static_cast<Node*>(m_root)))
				return false;
		}
		
		return true;
	}
	
	bool ReadLeafHeader() WARN_IF_UNUSED
	{		
		unsigned char buffer[node_header_sz];
		wxFFile * file = m_leafNodeFile->GetFFile();
		
		file->Seek(0);
		if (file->Read(buffer, leaf_header_sz) != leaf_header_sz)
			return false;
	
		int offset = 0;
		
		// header sz
		if (BUFFER_REF(uint16_t, offset) != leaf_header_sz)
			return false;
		offset += sizeof(uint16_t);
		
		// signature
		if (strncmp((char*)(buffer + offset), DISKTREE_LEAF_MAGIC, sizeof(DISKTREE_LEAF_MAGIC)-1))
			return false;
		offset += (sizeof(DISKTREE_LEAF_MAGIC)-1);
		
		// custom signature
		if (strncmp((char*)(buffer + offset), custom_signature.mb_str(), DISKTREE_CUSTOM_SIG_SZ))
			return false;
		offset += DISKTREE_CUSTOM_SIG_SZ;
			
		// leaf count
		m_leaf_node_count = record<IdType>::deserialize(buffer + offset);
		offset += record<IdType>::size;

		m_size = BUFFER_REF(size_type, offset);
		offset += sizeof(size_type);
		
		// max node items
		if (BUFFER_REF(uint32_t, offset) != (uint32_t)_max_node_items)
			return false;
		offset += sizeof(uint32_t);
			
	
		assert(offset == leaf_header_sz);
		
		// try to load the root node of the tree from file
		if (m_treeHeight == 1)
		{
			m_root = new LeafNode();
			if (!m_leafNodeFile->LoadObjects(m_rootId, 1, static_cast<LeafNode*>(m_root)))
				return false;
		}
		
		return true;
	}

	template <typename CacheType, typename NodeFileType>
	void ClearCache(CacheType * cache, NodeFileType * file)
	{
		typename CacheType::Node * cacheItem;
			
		while ((cacheItem = cache->removeOne()))
		{
			// see if it needs to be written back
			if (cacheItem->object->is_dirty)
				file->SaveObjects(cacheItem->id, 1, cacheItem->object);
			
			delete cacheItem;
		}
	}
	

	bool m_initialized;
	IdType m_node_count;
	IdType m_leaf_node_count;
	
	wxString custom_signature;

	// as a sidenote, the size of these caches needs to be at least as big
	// as the height of the tree, otherwise performance is going to be
	// *quite* miserable for insertions
	
	// for nodes
	NodeCache * m_nodeCache;
	NodeFile * m_nodeFile;

	// root node
	NodeBase * m_root;
	
	// for leaves
	LeafNodeCache * m_leafNodeCache;
	LeafNodeFile * m_leafNodeFile;

	ErrorCode m_lastError;
};


#undef SET_BAD_ERROR
	
#undef DISKTREE_NODE_MAGIC
#undef DISKTREE_LEAF_MAGIC

#undef DISKTREE_CUSTOM_SIG_SZ

#endif
