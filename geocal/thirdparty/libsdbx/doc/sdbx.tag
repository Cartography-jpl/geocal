<?xml version='1.0' encoding='ISO-8859-1' standalone='yes' ?>
<tagfile>
  <compound kind="class">
    <name>BTree</name>
    <filename>a00001.html</filename>
    <member kind="typedef">
      <type>_Backend&lt; NodeBase, BTreeNode, TreeNodeItem, LeafNode, KeyType, max_child_items &gt;</type>
      <name>Backend</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>45a144d9388ab70b0530054b40c0526e</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>Backend::Args</type>
      <name>BackendArgs</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>a3231d6c9e4a330ef179b7cd4750e9e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>Backend::IdType</type>
      <name>IdType</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>be734097976f0426326957a07cfdbbed</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>Traverser</type>
      <name>Iterator</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>35363ef010d53e88d0e358bf193fef09</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>_KeyType</type>
      <name>KeyType</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>f801a917b495912cbbb239a3859be6d4</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>TreeLeafItem&lt; KeyType, LeafType &gt;</type>
      <name>LeafItem</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>1a7c274ae1c562a385a392fcb45ce531</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>LeafNode::ItemArray</type>
      <name>LeafItemArray</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>65ac78bb4f7221c4d45e6b57acb41e37</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>LeafItemArray::iterator</type>
      <name>LeafItemIterator</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>d67a7c4c0610439bf31806669e4857b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>TreeNode&lt; LeafItem, max_child_items &gt;</type>
      <name>LeafNode</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>d43bcd1d90ea300e977ad1b66bdc3a48</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>_leaf_type</type>
      <name>LeafType</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>9ab7074c4eeac904c4960537af9a13fe</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>Backend::Node</type>
      <name>Node</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>c622f1cedbc6f94ed4f6f97cf4bbd4fa</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>BTreeNodeBase</type>
      <name>NodeBase</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>3fe1470e08c78343f51144a4261927f2</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>Backend::NodeItem</type>
      <name>NodeItem</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>0331e938f93584a419282437a28d9c82</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>Node::ItemArray</type>
      <name>NodeItemArray</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>2ed9fcc51681f507ab263958584bf6fa</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>NodeItemArray::iterator</type>
      <name>NodeItemIterator</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>4688282b76f348be7a607ea10eea14a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>Backend::size_type</type>
      <name>size_type</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>aad0fd7bde6a8a6ac1473c961270f1eb</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>BTree&lt; __BTREE_TEMPLATE_FN &gt;</type>
      <name>this_type</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>6a351f9e18e51d3adf239afb363d77c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>BTreeTraverser&lt; this_type &gt;</type>
      <name>Traverser</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>356381169e6ec6c29c9d945e42afd8ea</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>BTree</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>e3adf097939b1bce7fba5a5d55ff8907</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>GetHeight</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>c0061021d9258709326bace218172bfc</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>GetItemCount</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>bdf9810775b2f8cf9599161332b0dd92</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Insert</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>ab50f07342832b39c3bd29aacd283dfb</anchor>
      <arglist>(const KeyType &amp;key, const LeafType &amp;leaf, bool update=true) WARN_IF_UNUSED</arglist>
    </member>
    <member kind="function">
      <type>uint64_t</type>
      <name>Print</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>47c195fa985ccd957c54fe6970d64589</anchor>
      <arglist>(const char *filename)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Verify</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>80aa7d0aa00e7d46655894a167436986</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Iterator *</type>
      <name>Begin</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>0941e161bad609defc47d3b09939e9f8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Traverser *</type>
      <name>GetTraverser</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>ce71aba8c57958978a31d993cf822d6c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Destroy</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>84028cac8fb514413ee73361ba678a5e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>ErrorCode</type>
      <name>GetErrorCode</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>ae74f51fe733b3c48ebff53510dea2cb</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Initialize</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>6d1943d72e893359a142ded378de6cbe</anchor>
      <arglist>(BackendArgs &amp;args) WARN_IF_UNUSED</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsOk</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>661de2670698ce4317cad22e7fd19d1d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>FinishNodeSplit</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>bdb68e8faf1cf9ad9a1f4bbb11a58df2</anchor>
      <arglist>(NodeBase *_node, NodeBase *_new_node)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>NodeItem *</type>
      <name>InsertInternal</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>7e892d97c6b579f322ad1e5399d76f41</anchor>
      <arglist>(const LeafItem &amp;leafItem, const IdType node_id, const std::size_t level, bool update)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>Node *</type>
      <name>PropagateSplit</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>34428d82513b57b9ea2b03f2539c0961</anchor>
      <arglist>(const IdType node_id, const size_type old_idx, NodeItem *splitItem)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>NodeItem *</type>
      <name>Split</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>b2ff4f5cf2c3cefbe3492dedd6afc46b</anchor>
      <arglist>(NodeType *node, const std::size_t &amp;level)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>NodeItem *</type>
      <name>SplitRoot</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>b9de0b0b136f069ae866f34c0503551b</anchor>
      <arglist>(const KeyType &amp;key, const IdType &amp;split_id)</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>Backend</type>
      <name>backend</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>4cbaace943cc00adb7a387aa580cf3d1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>size_type</type>
      <name>m_inserts</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>d39a666b52f6a21ecd23693eba39eaa9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>ErrorCode</type>
      <name>m_lastError</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>c2c2d2ca81569327b47410ae17e35c21</anchor>
      <arglist></arglist>
    </member>
    <member kind="friend">
      <type>friend class</type>
      <name>BTreeTraverser&lt; this_type &gt;</name>
      <anchorfile>a00001.html</anchorfile>
      <anchor>95365baf630836d7653215c102d0b6b7</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>BTreeNode</name>
    <filename>a00002.html</filename>
    <templarg></templarg>
    <templarg>_max_items</templarg>
    <member kind="typedef">
      <type>Item::IdType</type>
      <name>IdType</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>9bb5fda8edc5a1df582ccfe44eb71a90</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>_Item</type>
      <name>Item</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>9868805bffe57a55fa1ad1b2c5788d09</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>std::vector&lt; Item &gt;</type>
      <name>ItemArray</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>7059295562ffdcbd33005c1ba340e2bf</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>BTreeNode</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>f1397918891996b699a610b384db6b75</anchor>
      <arglist>(const unsigned char *const buffer)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>BTreeNode</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>f1983f111a095e28f9a60674ec23e882</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>IdType</type>
      <name>get_item_id</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>4e3bf1358c177df511fce40c073b9a8c</anchor>
      <arglist>(std::size_t idx)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Serialize</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>ab68c76d6595f8cdf42bba4bdcc2b435</anchor>
      <arglist>(unsigned char *buffer) const </arglist>
    </member>
    <member kind="variable">
      <type>IdType</type>
      <name>end_id</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>56c5e836c76cb18d512f9e144e6158e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>is_dirty</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>095ea381c0927f23cf72b908d3968b3f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ItemArray</type>
      <name>items</name>
      <anchorfile>a00002.html</anchorfile>
      <anchor>2462d8614df04f0efc7fdbdb0d2d4037</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>BTreeTraverser</name>
    <filename>a00003.html</filename>
    <templarg></templarg>
    <member kind="typedef">
      <type>_Tree::Backend::ErrorCode</type>
      <name>ErrorCode</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>711047aba2a0d969ccc8e7a3e8988b0e</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>_Tree::IdType</type>
      <name>IdType</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>40f4ce987c76a0c90b8c5deae15aab95</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>_Tree::KeyType</type>
      <name>KeyType</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>145d86d4daee6431b21b662d35e3fdb2</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>_Tree::LeafItem</type>
      <name>LeafItem</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>cf66b578e0e0e6792739c36a87430116</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>_Tree::LeafItemIterator</type>
      <name>LeafItemIterator</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>4dea341213e70bcab7f9c2791cc7893b</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>_Tree::LeafNode</type>
      <name>LeafNode</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>760d2644f55b53fd82c5c08719f2283d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>_Tree::Node</type>
      <name>Node</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>cb2effd50c1711dfeac463415d11033f</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>_Tree::NodeItem</type>
      <name>NodeItem</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>f8cbfa738d5c055b54b245e41b1b6e7a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>_Tree::NodeItemIterator</type>
      <name>NodeItemIterator</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>51ddde1e9866930c398839b29ed1d684</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>begin</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>c9734d7d618e849578dad013caf6a994</anchor>
      <arglist>() WARN_IF_UNUSED</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>BTreeTraverser</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>2a20a6c28bc7de4bf818fa153a389d9d</anchor>
      <arglist>(_Tree *tree)</arglist>
    </member>
    <member kind="function">
      <type>const LeafItem &amp;</type>
      <name>current</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>40e592d8700acf785548411ef090d9fb</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>LeafItem &amp;</type>
      <name>current</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>e3f37ca45b3e37de7969cd134f72bb59</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>end</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>7eef96c0e3e262e35fd59dada2a8d8c9</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>find</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>c4e1935dff52e2ac17ffda583c2dea63</anchor>
      <arglist>(const KeyType &amp;key) WARN_IF_UNUSED</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>next</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>02219cd40c2dbed41d5e9063a8ff30ca</anchor>
      <arglist>() WARN_IF_UNUSED</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>reset</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>618551b1df0c5af62ae8b2531305b518</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_dirty</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>aa113c2152f681e594d73884c36394ad</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>_Tree *</type>
      <name>m_tree</name>
      <anchorfile>a00003.html</anchorfile>
      <anchor>b4face5f3dffad92870c2d57bed994e0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>DiskTree</name>
    <filename>a00004.html</filename>
  </compound>
  <compound kind="class">
    <name>LRUCache</name>
    <filename>a00005.html</filename>
    <templarg></templarg>
    <templarg></templarg>
    <templarg>deleteObject</templarg>
    <member kind="typedef">
      <type>LRUCacheNode&lt; CachedObjectType, IdType, deleteObject &gt;</type>
      <name>Node</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>6411bcd6a31b6fdec535693994a6328c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>std::vector&lt; Node * &gt;</type>
      <name>NodeArray</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>79f9a43f0345076e86050c6cebdfa0e7</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>NodeArray::iterator</type>
      <name>NodeIterator</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>3cc85ab629d0ea0414adaf8f148dba05</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>LRUCacheNodeSorter&lt; Node &gt;</type>
      <name>NodeSorter</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>04b7e1f3f7dd1c1beacc51c0af541f8e</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>std::size_t</type>
      <name>size_type</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>e6a856c278487a3d36f84d528a1b2908</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>Node *</type>
      <name>add</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>d9c503c0b8f77c161f80205495bb4b2e</anchor>
      <arglist>(const IdType &amp;id, CachedObjectType *object) WARN_IF_UNUSED</arglist>
    </member>
    <member kind="function">
      <type>CachedObjectType *</type>
      <name>get</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>86654d056acc1762c34f0a8ef47f1e35</anchor>
      <arglist>(const IdType &amp;id)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>LRUCache</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>6477b5cfa837db678c128743fa29581c</anchor>
      <arglist>(const size_type &amp;max_nodes)</arglist>
    </member>
    <member kind="function">
      <type>Node *</type>
      <name>removeOne</name>
      <anchorfile>a00005.html</anchorfile>
      <anchor>b34c7737ff4ca63d5bdcef9f35edf08b</anchor>
      <arglist>() WARN_IF_UNUSED</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>LRUCacheNode</name>
    <filename>a00006.html</filename>
    <templarg></templarg>
    <templarg></templarg>
    <templarg>deleteType</templarg>
    <member kind="typedef">
      <type>LRUCacheNode&lt; CachedObjectType, IdType, deleteType &gt;</type>
      <name>_Self</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>451144449da5649fd920fadf240ec192</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>LRUCacheNode</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>75815ba6ff9f20ee2e25e442d8f101a1</anchor>
      <arglist>(const IdType &amp;i, CachedObjectType *o, _Self *n)</arglist>
    </member>
    <member kind="variable">
      <type>IdType</type>
      <name>id</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>a9f8fca3a3cd842ce4ef139ecb79e668</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>_Self *</type>
      <name>next</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>db212348500ed1c6267d3450d2c380a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>CachedObjectType *</type>
      <name>object</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>b81202afef8325e106875742c07d11ab</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>_Self *</type>
      <name>prev</name>
      <anchorfile>a00006.html</anchorfile>
      <anchor>d626cf2ba65d1b8bef4b58eabe014231</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>LRUCacheNode&lt; CachedObjectType, IdType, LRUDeleteArray &gt;</name>
    <filename>a00007.html</filename>
    <templarg></templarg>
    <templarg></templarg>
    <member kind="typedef">
      <type>LRUCacheNode&lt; CachedObjectType, IdType, LRUDeleteArray &gt;</type>
      <name>_Self</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>b5b0c208df72d92cd1d395cd2cbe709c</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>LRUCacheNode</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>fdbad572bd463fcc887be06cee1aa64b</anchor>
      <arglist>(const IdType &amp;i, CachedObjectType *o, _Self *n)</arglist>
    </member>
    <member kind="variable">
      <type>IdType</type>
      <name>id</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>e5dadee3bad55b56c5f632b4255ff85e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>_Self *</type>
      <name>next</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>03ff3de8f90cf6925f3b2ee8adda7576</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>CachedObjectType *</type>
      <name>object</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>889e1c8e97b95b69733d1641cbe5c0c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>_Self *</type>
      <name>prev</name>
      <anchorfile>a00007.html</anchorfile>
      <anchor>0ea2ee091bb4b76ec1c784397162df8f</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>LRUCacheNode&lt; CachedObjectType, IdType, LRUDeleteObject &gt;</name>
    <filename>a00008.html</filename>
    <templarg></templarg>
    <templarg></templarg>
    <member kind="typedef">
      <type>LRUCacheNode&lt; CachedObjectType, IdType, LRUDeleteObject &gt;</type>
      <name>_Self</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>2752fe4d2d3d413a9eb5cd6c3da8af8e</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>LRUCacheNode</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>301c2000fb1b688685fe36147c5284cf</anchor>
      <arglist>(const IdType &amp;i, CachedObjectType *o, _Self *n)</arglist>
    </member>
    <member kind="variable">
      <type>IdType</type>
      <name>id</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>449493463a4f3a3157d1ea74458f967d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>_Self *</type>
      <name>next</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>63cc0f961a86fa3ec6398e3b8f95f570</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>CachedObjectType *</type>
      <name>object</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>83be6e75eeef490266b5935de2dadfb0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>_Self *</type>
      <name>prev</name>
      <anchorfile>a00008.html</anchorfile>
      <anchor>e82e801a3078a57c10b629bd01ac5d08</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>LRUCacheNode&lt; CachedObjectType, IdType, LRUFreeMalloc &gt;</name>
    <filename>a00009.html</filename>
    <templarg></templarg>
    <templarg></templarg>
    <member kind="typedef">
      <type>LRUCacheNode&lt; CachedObjectType, IdType, LRUFreeMalloc &gt;</type>
      <name>_Self</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>28426dc9621e421c667801996d1aa942</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>LRUCacheNode</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>7f40952e57ccfc6b8c0716249fabc6a9</anchor>
      <arglist>(const IdType &amp;i, CachedObjectType *o, _Self *n)</arglist>
    </member>
    <member kind="variable">
      <type>IdType</type>
      <name>id</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>548a85d162059e9759ab051a60a7610b</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>_Self *</type>
      <name>next</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>d0fd50de5727847bfcbb9a1166fa2243</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>CachedObjectType *</type>
      <name>object</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>36cf82bd125e57c73af59d5298498555</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>_Self *</type>
      <name>prev</name>
      <anchorfile>a00009.html</anchorfile>
      <anchor>90a81154e1a70a1044831d186706fa00</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>LRUCacheNodeSorter</name>
    <filename>a00010.html</filename>
    <templarg></templarg>
    <member kind="function">
      <type>bool</type>
      <name>operator()</name>
      <anchorfile>a00010.html</anchorfile>
      <anchor>825e5793f07827d678fcb1702fdf81c5</anchor>
      <arglist>(const _LRUCacheNode *const n1, const _LRUCacheNode *const n2)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>MemoryTreeBackend</name>
    <filename>a00011.html</filename>
    <templarg></templarg>
    <templarg>_NodeType</templarg>
    <templarg>_NodeItemType</templarg>
    <templarg></templarg>
    <templarg></templarg>
    <templarg>_max_node_items</templarg>
    <member kind="typedef">
      <type>MemoryTreeBackendArgs</type>
      <name>Args</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>e86acd69b9a5b32a338f83079a1d8e75</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>NodeBase *</type>
      <name>IdType</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>c8d71476d00b925a142d8d570be61b75</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>_LeafNode</type>
      <name>LeafNode</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>c933aed307dd339b01a33e346d5b6fd4</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>_NodeType&lt; NodeItem, _max_node_items &gt;</type>
      <name>Node</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>298b8b62f4dc8cea52cb570faa889060</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>_NodeBase</type>
      <name>NodeBase</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>57a864bb7a27c3a7d3dbe459757fb805</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>_NodeItemType&lt; _KeyType, IdType &gt;</type>
      <name>NodeItem</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>e6c7a25ede7ee2bfdb7a79c5ee4a62c9</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>std::size_t</type>
      <name>size_type</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>cebf0b45fbd4a533126e8d835af2469d</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Clear</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>bd913d854bbd785ce5074a740cb9a6e2</anchor>
      <arglist>(NodeBase *nd, size_type level)</arglist>
    </member>
    <member kind="function">
      <type>LeafNode *</type>
      <name>create_leaf_node</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>c8d88b24b0d52ba9ecdc9e0c8f803efb</anchor>
      <arglist>(IdType id) const </arglist>
    </member>
    <member kind="function">
      <type>Node *</type>
      <name>create_node</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>a16c85a8add912496afd7f6c813df466</anchor>
      <arglist>(IdType id) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>create_root_node</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>28ddb39baa83f959e897435ae3daf597</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>LeafNode *</type>
      <name>get_leaf_node</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>a72b4603b41bce74defdd11fea3ffe98</anchor>
      <arglist>(IdType id) const </arglist>
    </member>
    <member kind="function">
      <type>Node *</type>
      <name>get_node</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>97ee43d319257faba054b84447ea4847</anchor>
      <arglist>(IdType id) const </arglist>
    </member>
    <member kind="function">
      <type>ErrorCode</type>
      <name>GetErrorCode</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>9d93b63493b183e97efe840294867cf6</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Initialize</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>160c214beb0bf2faabc788506e71acc9</anchor>
      <arglist>(Args &amp;args) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsOk</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>5afad5f3a18eb1defae5217922d8bb83</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>IdType</type>
      <name>new_leaf_node_id</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>10aef9af413d5b7f4d8764d60b73405f</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>IdType</type>
      <name>new_node_id</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>2326d6f9760c1d46786efda36d3309f7</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="variable">
      <type>IdType</type>
      <name>m_rootId</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>b70c8c74987540f01627ca5a7a1addfa</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_type</type>
      <name>m_size</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>5e30597fc6ba97dc0d90fd80b581e6b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>size_type</type>
      <name>m_treeHeight</name>
      <anchorfile>a00011.html</anchorfile>
      <anchor>54df75824db95670d7b6c1ca68e7f56c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>record</name>
    <filename>a00012.html</filename>
    <templarg></templarg>
    <member kind="function" static="yes">
      <type>static const unsigned char *const</type>
      <name>deserialize</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>130a824f53e91001ae8656f26b5691f8</anchor>
      <arglist>(const unsigned char *const buffer)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>serialize</name>
      <anchorfile>a00012.html</anchorfile>
      <anchor>32e74e6b2780dd28403cdc3a3aa1e466</anchor>
      <arglist>(const T &amp;var, unsigned char *buffer)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>RTree</name>
    <filename>a00013.html</filename>
    <class kind="struct">RTree::OverflowItem</class>
    <member kind="typedef">
      <type>RTreeAcceptAny&lt; NodeItem, LeafItem &gt;</type>
      <name>AcceptAny</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>6a0b56a93da62f760939f62d5e841711</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>RTreeAcceptEnclosing&lt; NodeItem, LeafItem &gt;</type>
      <name>AcceptEnclosing</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>050521fcb0828dae353cd4aa8225a6b4</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>RTreeAcceptOverlapping&lt; NodeItem, LeafItem &gt;</type>
      <name>AcceptOverlapping</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>b1cd11ac78af030c3a1edb30692a9c3e</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>_Backend&lt; NodeBase, RTreeNode, TreeNodeItem, LeafNode, KeyType, max_child_items &gt;</type>
      <name>Backend</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>d94abdeb422dd45c69a5c9511ca4d6e5</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>Backend::Args</type>
      <name>BackendArgs</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>c69d197ed2d75739f82501da83cf5a7b</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>RTreeBoundingBox&lt; dimensions, _bound_type &gt;</type>
      <name>BoundingBox</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>e8a1dfc0f3ac64857e49f91db915c89b</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>RTreeIterator&lt; this_type, AcceptEnclosing &gt;</type>
      <name>EnclosedIterator</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>31f396f9be9609fa55782ad4829c0b5b</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>Backend::IdType</type>
      <name>IdType</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>148a74616ebab1138a50364e41430735</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>RTreeIterator&lt; this_type, AcceptAny &gt;</type>
      <name>Iterator</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>2acb045036b6b5778c5183cb573ac269</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>BoundingBox</type>
      <name>KeyType</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>6af6bb2ee0401a1f141f55ce431971ac</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>TreeLeafItem&lt; KeyType, LeafType &gt;</type>
      <name>LeafItem</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>4e26a3610d2271dcbb760bf4d7263f77</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>LeafNode::ItemArray</type>
      <name>LeafItemArray</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>5a46af0df672bb6a70f25af0f2c290a9</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>LeafItemArray::iterator</type>
      <name>LeafItemIterator</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>e2a75f62592b13676432d7cd4fea785d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>TreeNode&lt; LeafItem, max_child_items &gt;</type>
      <name>LeafNode</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>89ce586f5e8fe01930fcd95d9638e051</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>_leaf_type</type>
      <name>LeafType</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>d5de834085bbce66e4c76e1517ac9c89</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>Backend::Node</type>
      <name>Node</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>1a293f5d37b9ce0a311083d47e32477e</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>TreeNodeBase</type>
      <name>NodeBase</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>4bd8f158d2348e3b4cd15dbf6d143b63</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>Backend::NodeItem</type>
      <name>NodeItem</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>9a9e19d08177b06cc5455f7f81b5f983</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>Node::ItemArray</type>
      <name>NodeItemArray</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>c457769a7ec256851f478a29e6f5ac9e</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>NodeItemArray::iterator</type>
      <name>NodeItemIterator</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>eb51ffcf9a33c246c73a53848ce27a66</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>RTreeIterator&lt; this_type, AcceptOverlapping &gt;</type>
      <name>OverlappingIterator</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>15589782110bd0afdf547ae131568be9</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>Backend::size_type</type>
      <name>size_type</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>5346e1bbbcff05fdc69899736667a34c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>RTree&lt; __RTREE_TEMPLATE_FN &gt;</type>
      <name>this_type</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>dff160509b37615cd3a9f13146c9fc28</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>ExtendBounds</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>598d6cb66c6ce6120bb24975462e1f1d</anchor>
      <arglist>(const BoundingBox &amp;key, const LeafType &amp;leaf) WARN_IF_UNUSED</arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>GetDimensions</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>8d50cf62b427f2d4f3f4fa848d945c68</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>GetHeight</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>61d8107796bdc5417a2a2925643373a5</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>GetItemCount</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>760f5098c2a20f8d50ca6a2dfb378836</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Insert</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>e146de0605ea9cad42e6861922a5b3f2</anchor>
      <arglist>(const BoundingBox &amp;key, const LeafType &amp;leaf) WARN_IF_UNUSED</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>RTree</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>311d1bfc3a3f75f8ac2366602cbf009a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Verify</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>a97acc30439f24081de19765d8598484</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Iterator *</type>
      <name>Begin</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>49838e1afbc587ea4b3331831ca7983c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>EnclosedIterator *</type>
      <name>FindEnclosed</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>16310cb8c4ec8d9c7f8b371caced5141</anchor>
      <arglist>(const BoundingBox &amp;bound)</arglist>
    </member>
    <member kind="function">
      <type>OverlappingIterator *</type>
      <name>FindOverlapping</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>0a5b51cb5d2e776244c07af0a24d318f</anchor>
      <arglist>(const BoundingBox &amp;bound)</arglist>
    </member>
    <member kind="function">
      <type>ErrorCode</type>
      <name>GetErrorCode</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>73cc91cc6815efa9acaea74b294bb960</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Initialize</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>5fa9e0f480079e5e72127763c6eac990</anchor>
      <arglist>(BackendArgs &amp;args) WARN_IF_UNUSED</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsOk</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>d7399987b58b4c8e77fbe91085641037</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="friend">
      <type>friend class</type>
      <name>RTreeIterator</name>
      <anchorfile>a00013.html</anchorfile>
      <anchor>775575f3806172df875689499df6eba3</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>RTreeAcceptAny</name>
    <filename>a00014.html</filename>
    <templarg></templarg>
    <templarg></templarg>
    <member kind="function">
      <type>bool</type>
      <name>operator()</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>d154b83e17d2746d9aac8b44bf1793b1</anchor>
      <arglist>(const LeafItem &amp;item) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator()</name>
      <anchorfile>a00014.html</anchorfile>
      <anchor>4ecfeda6c988d2ce22634c7cd8a9d6a0</anchor>
      <arglist>(const NodeItem &amp;item) const </arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>RTreeAcceptEnclosing</name>
    <filename>a00015.html</filename>
    <templarg></templarg>
    <templarg></templarg>
    <member kind="function">
      <type>bool</type>
      <name>operator()</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>34140e007948fe62d9cac65b1431b31c</anchor>
      <arglist>(const LeafItem &amp;item) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator()</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>b5cc97e7f521c49f5c63c79ccfb85e0f</anchor>
      <arglist>(const NodeItem &amp;item) const </arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>RTreeAcceptEnclosing</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>c2e191c6661ed580c4c6fdeef9560b5f</anchor>
      <arglist>(const typename NodeItem::KeyType &amp;bound)</arglist>
    </member>
    <member kind="variable">
      <type>const NodeItem::KeyType &amp;</type>
      <name>m_bound</name>
      <anchorfile>a00015.html</anchorfile>
      <anchor>49a6e7955392d02ebf44fd16eb2374d1</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>RTreeAcceptOverlapping</name>
    <filename>a00016.html</filename>
    <templarg></templarg>
    <templarg></templarg>
    <member kind="function">
      <type>bool</type>
      <name>operator()</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>fc3eeade4b6f6f9cda1775c3c9f7f1f3</anchor>
      <arglist>(const LeafItem &amp;item) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator()</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>1cc305d0ed885f1b046f2bcd91c42c82</anchor>
      <arglist>(const NodeItem &amp;item) const </arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>RTreeAcceptOverlapping</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>a279217e450d144e76d48b1dd3dd9ebf</anchor>
      <arglist>(const typename NodeItem::KeyType &amp;bound)</arglist>
    </member>
    <member kind="variable">
      <type>const NodeItem::KeyType &amp;</type>
      <name>m_bound</name>
      <anchorfile>a00016.html</anchorfile>
      <anchor>4b7e88b7179b6c5c46f6ef8b4d5a89ff</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>RTreeIterator</name>
    <filename>a00017.html</filename>
    <templarg></templarg>
    <templarg></templarg>
    <member kind="typedef">
      <type>_Tree::IdType</type>
      <name>IdType</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>d60c61c77bfa9f8bcf3706830b022d9c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>_Tree::KeyType</type>
      <name>KeyType</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>d9a2dbe848fe0fd1785d62b86bc7382e</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>_Tree::LeafItem</type>
      <name>LeafItem</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>f062b136505e757f50b25004cdc29ae4</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>_Tree::LeafItemIterator</type>
      <name>LeafItemIterator</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>bf3c78181801f0a3d5b24b5d4f41f26c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>_Tree::LeafNode</type>
      <name>LeafNode</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>dbac4aa8cd8a67270c5bd18e33f1da69</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>_Tree::Node</type>
      <name>Node</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>56f0f7d524f1a085637882baeb92ffc5</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>_Tree::NodeItem</type>
      <name>NodeItem</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>80ed9a5a95db560cee6f37db4fc01780</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>_Tree::NodeItemIterator</type>
      <name>NodeItemIterator</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>c01c1479fdd0cc3bf03c8f5b847fa4a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>const LeafItem &amp;</type>
      <name>current</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>71692035c2f1c18fa4b4807d6bd5c79f</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>LeafItem &amp;</type>
      <name>current</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>563e235674f5a604135791c1483d462e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>end</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>2d84d7c5711e9d58bc4c4f2e0a75c52e</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>next</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>aab00f06be1eadf148cde48558d6b8e5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>RTreeIterator</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>b77d9b39c319242f74e36bd9e44eda9c</anchor>
      <arglist>(_Tree *tree, const _Acceptor &amp;a)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_dirty</name>
      <anchorfile>a00017.html</anchorfile>
      <anchor>404769bab56dcf888ef17d12a1581fd5</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>RTreeNode</name>
    <filename>a00018.html</filename>
    <templarg></templarg>
    <templarg>_max_items</templarg>
    <member kind="typedef">
      <type>_Item::IdType</type>
      <name>IdType</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>b328e2a881be47b08d0d8c33c28083df</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>_Item</type>
      <name>Item</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>c777621cdac752144fda63ab7c238053</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>std::vector&lt; Item &gt;</type>
      <name>ItemArray</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>070fbd9fa01acafc36bd273ad5ab9c77</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>IdType</type>
      <name>get_item_id</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>27d5f95667bfdb033f62bf3c400d8615</anchor>
      <arglist>(std::size_t idx)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>RTreeNode</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>2560856d00425b83961c766ab0c4486c</anchor>
      <arglist>(const unsigned char *const buffer)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>RTreeNode</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>3533996b97aebe92fb35ea2094ab0fb2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Serialize</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>44c99d2762ddd1c2b77a9f55974cccdc</anchor>
      <arglist>(unsigned char *buffer) const </arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>is_dirty</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>ae347610887727452633625b4c495b4d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ItemArray</type>
      <name>items</name>
      <anchorfile>a00018.html</anchorfile>
      <anchor>80350751cbebf83dd8eff34b6695e490</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>SerializedFile</name>
    <filename>a00019.html</filename>
    <templarg></templarg>
    <templarg>start_offset</templarg>
    <member kind="typedef">
      <type>size_type</type>
      <name>IdType</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>b65d15a8fbb4002a579b920ff79407ad</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <name>SerializedFileError</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>99054aed7a18dee9afd6ba6e8a3e347f</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ERR_BAD_SEEK</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>99054aed7a18dee9afd6ba6e8a3e347fece3cfbcbe16262f50266fa2b597b634</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ERR_BAD_WRITE</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>99054aed7a18dee9afd6ba6e8a3e347f128d9a14eed7bd9e3ef32d4888189550</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ERR_BAD_READ</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>99054aed7a18dee9afd6ba6e8a3e347f691a032468e8f0a09da4e989921deb58</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>uint64_t</type>
      <name>size_type</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>6772c0b609d718f936e60f3b71e31931</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Delete</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>0102df05ef6e2fd4ad61aceff9a20248</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>SerializedFileError</type>
      <name>GetErrorCode</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>7f72d52abd9202cf7b01933c9b9078aa</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>wxString</type>
      <name>GetErrorString</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>6df78547557b3c0460a49492f1ef5249</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>wxFFile *</type>
      <name>GetFFile</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>9a0884008210efce9a31601496b40ddd</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>IdType</type>
      <name>GetLastId</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>da9cb5dbe8308b74c31a4798c49d255f</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsOpened</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>677d49ee2f8616db15f86822f2c04df8</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>LoadObjects</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>3c6b781c2dfa9d9759786602b6e133fe</anchor>
      <arglist>(const IdType &amp;start_id, const size_t count, T *dst)</arglist>
    </member>
    <member kind="function">
      <type>size_t</type>
      <name>SaveObjects</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>dc926bd60ea435b9a1a1adede3a79e7e</anchor>
      <arglist>(const IdType &amp;start_id, const size_t count, const T *const src)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SerializedFile</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>7a12f5b8c1959ec98516461816416d3d</anchor>
      <arglist>(const wxFileName &amp;filename, size_t buffer_size=4096)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetBufferSize</name>
      <anchorfile>a00019.html</anchorfile>
      <anchor>405112a9647df4e890a6df51f5cca836</anchor>
      <arglist>(const size_t buffer_size)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>StringKey</name>
    <filename>a00020.html</filename>
    <templarg>key_length</templarg>
    <member kind="typedef">
      <type>StringKey&lt; key_length &gt;</type>
      <name>this_type</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>a326a0602e412a046cd5174b12b8485e</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>bc5c07ef84905d6d5b20beb4a3b4515f</anchor>
      <arglist>(const this_type &amp;bsk) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>7aae34383375682d5d5f33d64d784eb1</anchor>
      <arglist>(const this_type &amp;bsk) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>29892b6e201e52317e5c29ba9d613c6b</anchor>
      <arglist>(const this_type &amp;bsk) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Serialize</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>aa63f297ca4009be94bb9bbcdaa965f5</anchor>
      <arglist>(unsigned char *buffer) const </arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>StringKey</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>286cd216361c81bc50078a57b70a387d</anchor>
      <arglist>(const unsigned char *const buffer)</arglist>
    </member>
    <member kind="variable">
      <type>char</type>
      <name>keyval</name>
      <anchorfile>a00020.html</anchorfile>
      <anchor>3d63844acea734a6edf92d1f21f647cd</anchor>
      <arglist>[key_length]</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>StringTable</name>
    <filename>a00021.html</filename>
    <templarg>_key_length</templarg>
    <templarg>max_child_items</templarg>
    <member kind="typedef">
      <type>LRUCache&lt; char, UniqueId, LRUFreeMalloc &gt;</type>
      <name>Cache</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>f070479b99817e45696d0da2d73af428</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>UniqueId</type>
      <name>IdType</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>5f1961ddd6533e9ce2693d0e529c7d6d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>Tree::Iterator</type>
      <name>Iterator</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>f0c4e8ba791fb1b6a41601698d65d0a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>StringKey&lt; key_length &gt;</type>
      <name>KeyType</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>d8c5e92f95bf1cc59fd0d05741d4d20d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>Tree::size_type</type>
      <name>size_type</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>d84f7f3f83c32765ede8d1b097157521</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>BTree&lt; KeyType, IdType, max_child_items, DiskTreeBackend, true &gt;</type>
      <name>Tree</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>8e79899f8c7c96dc380315e9c4928a0b</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>Iterator *</type>
      <name>Begin</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>7126971c16c23f957c8c0ab19a03c866</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>FindStringId</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>c92b05d069b14cb0a4da7733e6784d52</anchor>
      <arglist>(const char *str, IdType &amp;id) WARN_IF_UNUSED</arglist>
    </member>
    <member kind="function">
      <type>ErrorCode</type>
      <name>GetErrorCode</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>cc07e37f66c38cd23a1e7bc5623c6fb7</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>size_type</type>
      <name>GetItemCount</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>17000a94505633e0579ec899cb7445da</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>GetStringById</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>a8395a3054781013e45d909a057d3d54</anchor>
      <arglist>(const IdType &amp;id)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>Initialize</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>ad29815a144f9ac1c738314891d21359</anchor>
      <arglist>(const wxFileName &amp;filename, std::size_t node_cache_sz=500, std::size_t string_cache_sz=10000) WARN_IF_UNUSED</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>InsertString</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>cbac23c55d6a42a98882ec97b8006aac</anchor>
      <arglist>(const char *str, IdType &amp;id) WARN_IF_UNUSED</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>VerifyIndex</name>
      <anchorfile>a00021.html</anchorfile>
      <anchor>12d0b73740a07a590f6d7331b6c9c268</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>TreeNode</name>
    <filename>a00022.html</filename>
    <templarg></templarg>
    <templarg>_max_items</templarg>
    <member kind="typedef">
      <type>_Item</type>
      <name>Item</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>18da787aa09c3874f1a23d34b3fa5ee5</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>std::vector&lt; Item &gt;</type>
      <name>ItemArray</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>3518fd1ed35931eb71bb44e22f6cc72b</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Serialize</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>801a4f558aba2af3e77f9e86a6691327</anchor>
      <arglist>(unsigned char *buffer) const </arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>TreeNode</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>44bb25b0fa1e86caee551c8c93595672</anchor>
      <arglist>(const unsigned char *const buffer)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>TreeNode</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>208839dc50026f3b56273ec1e1ff621d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="variable">
      <type>bool</type>
      <name>is_dirty</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>ef78e40d163c358c90dbd1d15d5ec0db</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>ItemArray</type>
      <name>items</name>
      <anchorfile>a00022.html</anchorfile>
      <anchor>9ae94d6e7435ced4ab79536df760ab75</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>UniqueIdT</name>
    <filename>a00023.html</filename>
    <templarg></templarg>
    <member kind="typedef">
      <type>UniqueIdT&lt; high_type &gt;</type>
      <name>this_type</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>e9f00e134fefa7a44c35361682fd71f2</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsNull</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>a8d80122f4247f0c4ab319a872788ae6</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>operator uint64_t</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>8a04cb171b9ea720dfa86c48c5e60a4c</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>60f863d484477cb1a0cbdf8ada61496b</anchor>
      <arglist>(const this_type &amp;x) const </arglist>
    </member>
    <member kind="function">
      <type>this_type &amp;</type>
      <name>operator++</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>3783ca64e152dda2e3321d8b660273d4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>972784ac16954d306c48c5df375b7eb6</anchor>
      <arglist>(const this_type &amp;x) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>12b1db09602f4ed85261195d945ec16f</anchor>
      <arglist>(const this_type &amp;x) const </arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&gt;</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>31c63849ccd34289ae7df835a19ab902</anchor>
      <arglist>(const this_type &amp;x) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>Serialize</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>d2b9ee96f688c19ec4e3e4eeca9a93eb</anchor>
      <arglist>(unsigned char *buffer) const </arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>UniqueIdT</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>5134c0966907c7d6d5280fdb4855395e</anchor>
      <arglist>(const unsigned char *const buffer)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>UniqueIdT</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>a9d670942ddb04b3231e5464683ffc2b</anchor>
      <arglist>(uint32_t l, high_type h)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>UniqueIdT</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>9862d0b85248f174e64c0ce34582ca5e</anchor>
      <arglist>(uint64_t val)</arglist>
    </member>
    <member kind="variable">
      <type>high_type</type>
      <name>hi</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>f4b7eace3074587a8103eb774a1abf85</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>lo</name>
      <anchorfile>a00023.html</anchorfile>
      <anchor>b72d7a69f6d0dd53d959f203a4efbd49</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>std</name>
    <filename>a00050.html</filename>
    <class kind="class">std::allocator</class>
    <class kind="class">std::auto_ptr</class>
    <class kind="class">std::bad_alloc</class>
    <class kind="class">std::bad_cast</class>
    <class kind="class">std::bad_exception</class>
    <class kind="class">std::bad_typeid</class>
    <class kind="class">std::basic_fstream</class>
    <class kind="class">std::basic_ifstream</class>
    <class kind="class">std::basic_ios</class>
    <class kind="class">std::basic_iostream</class>
    <class kind="class">std::basic_istream</class>
    <class kind="class">std::basic_istringstream</class>
    <class kind="class">std::basic_ofstream</class>
    <class kind="class">std::basic_ostream</class>
    <class kind="class">std::basic_ostringstream</class>
    <class kind="class">std::basic_string</class>
    <class kind="class">std::basic_stringstream</class>
    <class kind="class">std::bitset</class>
    <class kind="class">std::complex</class>
    <class kind="class">std::deque</class>
    <class kind="class">std::domain_error</class>
    <class kind="class">std::exception</class>
    <class kind="class">std::fstream</class>
    <class kind="class">std::ifstream</class>
    <class kind="class">std::invalid_argument</class>
    <class kind="class">std::ios</class>
    <class kind="class">std::ios_base</class>
    <class kind="class">std::istream</class>
    <class kind="class">std::istringstream</class>
    <class kind="class">std::length_error</class>
    <class kind="class">std::list</class>
    <class kind="class">std::logic_error</class>
    <class kind="class">std::map</class>
    <class kind="class">std::multimap</class>
    <class kind="class">std::multiset</class>
    <class kind="class">std::ofstream</class>
    <class kind="class">std::ostream</class>
    <class kind="class">std::ostringstream</class>
    <class kind="class">std::out_of_range</class>
    <class kind="class">std::overflow_error</class>
    <class kind="class">std::priority_queue</class>
    <class kind="class">std::queue</class>
    <class kind="class">std::range_error</class>
    <class kind="class">std::runtime_error</class>
    <class kind="class">std::set</class>
    <class kind="class">std::stack</class>
    <class kind="class">std::string</class>
    <class kind="class">std::stringstream</class>
    <class kind="class">std::underflow_error</class>
    <class kind="class">std::valarray</class>
    <class kind="class">std::vector</class>
    <class kind="class">std::wfstream</class>
    <class kind="class">std::wifstream</class>
    <class kind="class">std::wios</class>
    <class kind="class">std::wistream</class>
    <class kind="class">std::wistringstream</class>
    <class kind="class">std::wofstream</class>
    <class kind="class">std::wostream</class>
    <class kind="class">std::wostringstream</class>
    <class kind="class">std::wstring</class>
    <class kind="class">std::wstringstream</class>
    <member kind="function">
      <type>bool</type>
      <name>is_sorted</name>
      <anchorfile>a00050.html</anchorfile>
      <anchor>042280b675ab84027160f36b36f05c29</anchor>
      <arglist>(ForwardIterator first, ForwardIterator last)</arglist>
    </member>
  </compound>
  <compound kind="dir">
    <name>BTree/</name>
    <path>/Users/smyth/CartLab/thirdparty/libsdbx/sdbx/BTree/</path>
    <filename>dir_fc737d041ef8f2c4185ce3a0bc957e6a.html</filename>
    <file>BTree.h</file>
    <file>BTreeNode.h</file>
    <file>BTreePrint.h</file>
    <file>BTreeTraverser.h</file>
    <file>BTreeVerify.h</file>
  </compound>
  <compound kind="dir">
    <name>RTree/</name>
    <path>/Users/smyth/CartLab/thirdparty/libsdbx/sdbx/RTree/</path>
    <filename>dir_77e788db3be33bcba33e4b74c0683ce9.html</filename>
    <file>RTree.h</file>
    <file>RTreeBoundingBox.h</file>
    <file>RTreeIterator.h</file>
    <file>RTreeNode.h</file>
    <file>RTreeVerify.h</file>
    <file>RTreeVisitor.h</file>
  </compound>
  <compound kind="dir">
    <name>support/</name>
    <path>/Users/smyth/CartLab/thirdparty/libsdbx/sdbx/support/</path>
    <filename>dir_bf4f60c2190ddc9913e18eab3dd204dd.html</filename>
    <file>is_sorted.h</file>
    <file>pstdint.h</file>
    <file>StringKey.h</file>
    <file>TreeItem.h</file>
    <file>TreeNode.h</file>
    <file>UniqueId.h</file>
    <file>utility.h</file>
  </compound>
</tagfile>
