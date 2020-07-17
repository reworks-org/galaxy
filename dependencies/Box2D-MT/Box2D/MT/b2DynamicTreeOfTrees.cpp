/*
* Copyright (c) 2019 Justin Hoffman https://github.com/jhoffman0x/Box2D-MT
*
* Copyright (c) 2009 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#include "Box2D/MT/b2DynamicTreeOfTrees.h"
#include <string.h>
#include <cmath>

//#define b2_validateTree

b2DynamicTreeOfTrees::b2DynamicTreeOfTrees(float32 subTreeWidth, float32 subTreeHeight)
{
	m_root = b2_nullNode;

    m_subTreeWidth = subTreeWidth;
    m_subTreeHeight = subTreeHeight;
    m_subTreeInvWidth = 1.0f / subTreeWidth;
    m_subTreeInvHeight = 1.0f / subTreeHeight;

	m_nodeCapacity = 16;
	m_nodeCount = 0;
	m_nodes = (Node*)b2Alloc(m_nodeCapacity * sizeof(Node));
#if defined(b2DEBUG)
	memset((char*)m_nodes, 0xcd, m_nodeCapacity * sizeof(Node));
#endif

    for (uint32 i = 0; i < b2_maxThreads; ++i)
    {
        m_perThreadData[i].m_proxyQueryCounters = (uint32*)b2Alloc(m_nodeCapacity * sizeof(uint32));
        memset(m_perThreadData[i].m_proxyQueryCounters, 0, m_nodeCapacity * sizeof(uint32));
        m_perThreadData[i].m_queryCounter = 0;
    }

	// Build a linked list for the free list.
	for (int32 i = 0; i < m_nodeCapacity - 1; ++i)
	{
		m_nodes[i].next = i + 1;
		m_nodes[i].child1 = 0;
		m_nodes[i].height = -1;
	}
	m_nodes[m_nodeCapacity-1].next = b2_nullNode;
    m_nodes[m_nodeCapacity-1].child1 = 0;
	m_nodes[m_nodeCapacity-1].height = -1;
	m_freeList = 0;

	m_path = 0;

	m_insertionCount = 0;
}

b2DynamicTreeOfTrees::~b2DynamicTreeOfTrees()
{
	// This frees the entire tree in one shot.
	b2Free(m_nodes);
}

// Allocate a node from the pool. Grow the pool if necessary.
int32 b2DynamicTreeOfTrees::AllocateNode()
{
	// Expand the node pool as needed.
	if (m_freeList == b2_nullNode)
	{
		b2Assert(m_nodeCount == m_nodeCapacity);

		// The free list is empty. Rebuild a bigger pool.
		Node* oldNodes = m_nodes;
		m_nodeCapacity *= 2;
		m_nodes = (Node*)b2Alloc(m_nodeCapacity * sizeof(Node));
		memcpy(m_nodes, oldNodes, m_nodeCount * sizeof(Node));
		b2Free(oldNodes);

        for (uint32 i = 0; i < b2_maxThreads; ++i)
        {
            uint32* oldCounters = m_perThreadData[i].m_proxyQueryCounters;
            m_perThreadData[i].m_proxyQueryCounters = (uint32*)b2Alloc(m_nodeCapacity * sizeof(uint32));
            memset(m_perThreadData[i].m_proxyQueryCounters, 0, m_nodeCapacity * sizeof(uint32));
            b2Free(oldCounters);
            m_perThreadData[i].m_queryCounter = 0;
        }

		// Build a linked list for the free list. The parent
		// pointer becomes the "next" pointer.
		for (int32 i = m_nodeCount; i < m_nodeCapacity - 1; ++i)
		{
			m_nodes[i].next = i + 1;
            m_nodes[i].child1 = 0;
			m_nodes[i].height = -1;
		}
		m_nodes[m_nodeCapacity-1].next = b2_nullNode;
        m_nodes[m_nodeCapacity-1].child1 = 0;
		m_nodes[m_nodeCapacity-1].height = -1;
		m_freeList = m_nodeCount;
	}

	// Peel a node off the free list.
	int32 nodeId = m_freeList;
	m_freeList = m_nodes[nodeId].next;
	m_nodes[nodeId].parent = b2_nullNode;
	m_nodes[nodeId].child1 = b2_nullNode;
	m_nodes[nodeId].child2 = b2_nullNode;
	m_nodes[nodeId].height = 0;
	m_nodes[nodeId].userData = nullptr;
	m_nodes[nodeId].proxy = nodeId;
	++m_nodeCount;
	return nodeId;
}

// Return a node to the pool.
void b2DynamicTreeOfTrees::FreeNode(int32 nodeId)
{
	b2Assert(0 <= nodeId && nodeId < m_nodeCapacity);
	b2Assert(0 < m_nodeCount);
	m_nodes[nodeId].next = m_freeList;
    m_nodes[nodeId].child1 = 0;
	m_nodes[nodeId].height = -1;
	m_freeList = nodeId;
	--m_nodeCount;
}

void b2DynamicTreeOfTrees::Reset(float32 subTreeWidth, float32 subTreeHeight)
{
    m_root = b2_nullNode;

    m_subTreeWidth = subTreeWidth;
	m_subTreeHeight = subTreeHeight;
    m_subTreeInvWidth = 1.0f / subTreeWidth;
    m_subTreeInvHeight = 1.0f / subTreeHeight;

	m_nodeCount = 0;

	for (int32 i = 0; i < m_nodeCapacity - 1; ++i)
	{
		m_nodes[i].next = i + 1;
        m_nodes[i].child1 = 0;
		m_nodes[i].height = -1;
	}
	m_nodes[m_nodeCapacity-1].next = b2_nullNode;
    m_nodes[m_nodeCapacity-1].child1 = 0;
	m_nodes[m_nodeCapacity-1].height = -1;
	m_freeList = 0;
}

// Create a proxy in the tree as a leaf node. We return the index
// of the node instead of a pointer so that we can grow
// the node pool.
int32 b2DynamicTreeOfTrees::CreateProxy(const b2AABB& aabb, void* userData)
{
	int32 proxyId = AllocateNode();

	// Fatten the aabb.
	b2Vec2 r(b2_aabbExtension, b2_aabbExtension);
	m_nodes[proxyId].aabb.lowerBound = aabb.lowerBound - r;
	m_nodes[proxyId].aabb.upperBound = aabb.upperBound + r;
	m_nodes[proxyId].userData = userData;
	m_nodes[proxyId].height = 0;

	InsertLeaf(proxyId);

	return proxyId;
}

void b2DynamicTreeOfTrees::DestroyProxy(int32 proxyId)
{
	b2Assert(0 <= proxyId && proxyId < m_nodeCapacity);
	b2Assert(m_nodes[proxyId].IsLeaf());

	RemoveLeaf(proxyId);

    while (proxyId != b2_nullNode)
    {
        int32 nextProxy = m_nodes[proxyId].nextProxy;
	    FreeNode(proxyId);
        proxyId = nextProxy;
    }
}

bool b2DynamicTreeOfTrees::MoveProxy(int32 proxyId, const b2AABB& aabb, const b2Vec2& displacement)
{
	b2Assert(0 <= proxyId && proxyId < m_nodeCapacity);

	b2Assert(m_nodes[proxyId].IsLeaf());

	if (m_nodes[proxyId].aabb.Contains(aabb))
	{
		return false;
	}

	RemoveLeaf(proxyId);

	// Extend AABB.
	b2AABB b = aabb;
	b2Vec2 r(b2_aabbExtension, b2_aabbExtension);
	b.lowerBound = b.lowerBound - r;
	b.upperBound = b.upperBound + r;

	// Predict AABB displacement.
	b2Vec2 d = b2_aabbMultiplier * displacement;

	if (d.x < 0.0f)
	{
		b.lowerBound.x += d.x;
	}
	else
	{
		b.upperBound.x += d.x;
	}

	if (d.y < 0.0f)
	{
		b.lowerBound.y += d.y;
	}
	else
	{
		b.upperBound.y += d.y;
	}

	m_nodes[proxyId].aabb = b;

	InsertLeaf(proxyId);
	return true;
}

struct b2InsertLeafQueryCallback
{
    b2InsertLeafQueryCallback(b2DynamicTreeOfTrees* tree, int32 proxy)
        : m_tree(tree)
        , m_aabb()
        , m_proxy(proxy)
        , m_currProxy(proxy)
        , m_insertCount(0)
    { }

    bool QueryCallback(int32 baseTreeLeaf)
    {
        if (m_insertCount == 0)
        {
            m_aabb = m_tree->m_nodes[baseTreeLeaf].aabb;
        }
        else
        {
            // Track the insertion bounds.
            m_aabb.Combine(m_tree->m_nodes[baseTreeLeaf].aabb);

            // Each sub-tree insertion requires a separate proxy.
            int32 nextProxy = m_tree->AllocateNode();
            b2Assert (m_tree->m_nodes[m_currProxy].nextProxy == b2_nullNode);
            m_tree->m_nodes[m_currProxy].nextProxy = nextProxy;

            m_tree->m_nodes[nextProxy].aabb = m_tree->m_nodes[m_proxy].aabb;
            m_tree->m_nodes[nextProxy].userData = m_tree->m_nodes[m_proxy].userData;
            m_tree->m_nodes[nextProxy].proxy = m_proxy;

            m_currProxy = nextProxy;
        }

        m_tree->m_nodes[m_currProxy].baseTreeLeaf = baseTreeLeaf;

        ++m_insertCount;

        int32 subTreeRoot = m_tree->m_nodes[baseTreeLeaf].subTreeRoot;
        m_tree->InsertLeaf(subTreeRoot, m_currProxy);
        m_tree->m_nodes[baseTreeLeaf].subTreeRoot = subTreeRoot;

#ifdef b2_validateTree
        m_tree->Validate();
#endif

        return true;
    }

    b2DynamicTreeOfTrees* m_tree;
    b2AABB m_aabb;
    int32 m_proxy;
    int32 m_currProxy;
    uint32 m_insertCount;
};

struct b2FindFirstLeafQueryCallback
{
    b2FindFirstLeafQueryCallback(b2DynamicTreeOfTrees* tree)
        : m_tree(tree)
        , m_leaf(-1)
    { }

    bool QueryCallback(int32 proxyId)
    {
        m_leaf = proxyId;
        return false;
    }

    b2DynamicTreeOfTrees* m_tree;
    int32 m_leaf;
};

b2_forceInline void b2DynamicTreeOfTrees::InsertLeaf(int32 proxy)
{
	++m_insertionCount;

    b2AABB proxyAABB = m_nodes[proxy].aabb;
    void* proxyUserData = m_nodes[proxy].userData;

    // Insert the leaf into all existing overlapped sub-trees.
    b2InsertLeafQueryCallback insertQuery(this, proxy);

    uint32 threadId = 0; // TODO_MT parallel SynchronizeFixtures
    Query<false, false>(m_root, &insertQuery, proxyAABB, threadId);

    // Is the leaf fully contained by the sub-trees it was inserted into?
    // Note: if the leaf AABB spans more than 2 sub-trees per dimension, then it's
    // possible for it to be contained by the query even when some of the required
    // sub-trees don't currently exist.
    if (insertQuery.m_insertCount > 0 && insertQuery.m_aabb.Contains(proxyAABB))
    {
        // Is the leaf fully contained by one sub-tree?
        if (insertQuery.m_insertCount == 1)
        {
            return;
        }

        float32 leafWx = proxyAABB.upperBound.x - proxyAABB.lowerBound.x;
        float32 leafWy = proxyAABB.upperBound.y - proxyAABB.lowerBound.y;

        // Is the leaf AABB small enough to be contained by a single sub-tree?
        // This means that it can't span more than 2 sub-trees per dimension,
        // and must have been inserted into all overlapped sub-trees.
        if (leafWx < m_subTreeWidth && leafWy < m_subTreeHeight)
        {
            b2Assert(insertQuery.m_insertCount <= 4);
            return;
        }

        // The leaf AABB is large enough to span more than 2 sub-trees per dimension,
        // so we need to rule out gaps in the query AABB by checking if it has the
        // expected number of insertions.

        float32 queryWx = insertQuery.m_aabb.upperBound.x - insertQuery.m_aabb.lowerBound.x;
        float32 queryWy = insertQuery.m_aabb.upperBound.y - insertQuery.m_aabb.lowerBound.y;

        uint32 subTreeCountX = uint32(queryWx * m_subTreeInvWidth + 0.5f);
        uint32 subTreeCountY = uint32(queryWy * m_subTreeInvHeight + 0.5f);
        uint32 subTreeCount = subTreeCountX * subTreeCountY;

        // Has the leaf been inserted into the expected number of sub-trees?
        if (insertQuery.m_insertCount == subTreeCount)
        {
            return;
        }
    }

    // Create any overlapped sub-trees that don't currently exist.

    int32 beginX = int32(std::round(proxyAABB.lowerBound.x * m_subTreeInvWidth));
    int32 beginY = int32(std::round(proxyAABB.lowerBound.y * m_subTreeInvHeight));
    int32 endX = int32(std::round(proxyAABB.upperBound.x * m_subTreeInvWidth)) + 1;
    int32 endY = int32(std::round(proxyAABB.upperBound.y * m_subTreeInvHeight)) + 1;

    int32 insertProxy = b2_nullNode;

    if (insertQuery.m_insertCount > 0)
    {
        insertProxy = insertQuery.m_currProxy;
    }

    for (int32 y = beginY; y < endY; ++y)
    {
        for (int32 x = beginX; x < endX; ++x)
        {
            // Look for the sub-tree.
            b2Vec2 subTreePos(x * m_subTreeWidth, y * m_subTreeHeight);
            b2FindFirstLeafQueryCallback findQuery(this);
            Query(&findQuery, subTreePos);

            // Does the sub-tree need to be created?
            if (findQuery.m_leaf == -1)
            {
                // Create a new proxy node if we already inserted the first one.
                if (insertProxy != b2_nullNode)
                {
                    int32 newLeaf = AllocateNode();

                    b2Assert(m_nodes[insertProxy].nextProxy == b2_nullNode);
                    m_nodes[insertProxy].nextProxy = newLeaf;

                    m_nodes[newLeaf].aabb = proxyAABB;
                    m_nodes[newLeaf].userData = proxyUserData;
                    m_nodes[newLeaf].proxy = proxy;

                    insertProxy = newLeaf;
                }
                else
                {
                    insertProxy = proxy;
                }

                // Create a new sub-tree for the proxy.
                int32 baseLeaf = AllocateNode();
                m_nodes[baseLeaf].aabb.lowerBound = subTreePos - b2Vec2(m_subTreeWidth * 0.5f, m_subTreeHeight * 0.5f);
                m_nodes[baseLeaf].aabb.upperBound = subTreePos + b2Vec2(m_subTreeWidth * 0.5f, m_subTreeHeight * 0.5f);
                InsertLeaf(m_root, baseLeaf);
                m_nodes[baseLeaf].subTreeRoot = insertProxy;
                m_nodes[insertProxy].baseTreeLeaf = baseLeaf;
                m_nodes[insertProxy].parent = b2_nullNode;

#ifdef b2_validateTree
                Validate();
#endif
            }
        }
    }
}

b2_forceInline void b2DynamicTreeOfTrees::RemoveLeaf(int32 proxy)
{
    // Remove the proxy from each sub-tree it's in.
    int32 currentProxy = proxy;

    while (currentProxy != b2_nullNode)
    {
        int32 baseTreeLeaf = m_nodes[currentProxy].baseTreeLeaf;
        RemoveLeaf(m_nodes[baseTreeLeaf].subTreeRoot, currentProxy);
        if (m_nodes[baseTreeLeaf].subTreeRoot == b2_nullNode)
        {
            RemoveLeaf(m_root, baseTreeLeaf);
        }

        int32 nextProxy = m_nodes[currentProxy].nextProxy;
        m_nodes[currentProxy].nextProxy = b2_nullNode;
        if (currentProxy != proxy)
        {
            FreeNode(currentProxy);
        }
        currentProxy = nextProxy;
    }
}

b2_forceInline void b2DynamicTreeOfTrees::InsertLeaf(int32& root, int32 leaf)
{
	m_nodes[leaf].parent = b2_nullNode;

	if (root == b2_nullNode)
	{
		root = leaf;
		return;
	}

	// Find the best sibling for this node
	b2AABB leafAABB = m_nodes[leaf].aabb;
	int32 index = root;
	while (m_nodes[index].IsLeaf() == false)
	{
		int32 child1 = m_nodes[index].child1;
		int32 child2 = m_nodes[index].child2;

		float32 area = m_nodes[index].aabb.GetPerimeter();

		b2AABB combinedAABB;
		combinedAABB.Combine(m_nodes[index].aabb, leafAABB);
		float32 combinedArea = combinedAABB.GetPerimeter();

		// Cost of creating a new parent for this node and the new leaf
		float32 cost = 2.0f * combinedArea;

		// Minimum cost of pushing the leaf further down the tree
		float32 inheritanceCost = 2.0f * (combinedArea - area);

		// Cost of descending into child1
		float32 cost1;
		if (m_nodes[child1].IsLeaf())
		{
			b2AABB aabb;
			aabb.Combine(leafAABB, m_nodes[child1].aabb);
			cost1 = aabb.GetPerimeter() + inheritanceCost;
		}
		else
		{
			b2AABB aabb;
			aabb.Combine(leafAABB, m_nodes[child1].aabb);
			float32 oldArea = m_nodes[child1].aabb.GetPerimeter();
			float32 newArea = aabb.GetPerimeter();
			cost1 = (newArea - oldArea) + inheritanceCost;
		}

		// Cost of descending into child2
		float32 cost2;
		if (m_nodes[child2].IsLeaf())
		{
			b2AABB aabb;
			aabb.Combine(leafAABB, m_nodes[child2].aabb);
			cost2 = aabb.GetPerimeter() + inheritanceCost;
		}
		else
		{
			b2AABB aabb;
			aabb.Combine(leafAABB, m_nodes[child2].aabb);
			float32 oldArea = m_nodes[child2].aabb.GetPerimeter();
			float32 newArea = aabb.GetPerimeter();
			cost2 = newArea - oldArea + inheritanceCost;
		}

		// Descend according to the minimum cost.
		if (cost < cost1 && cost < cost2)
		{
			break;
		}

		// Descend
		if (cost1 < cost2)
		{
			index = child1;
		}
		else
		{
			index = child2;
		}
	}

	int32 sibling = index;

	// Create a new parent.
	int32 oldParent = m_nodes[sibling].parent;
	int32 newParent = AllocateNode();
	m_nodes[newParent].parent = oldParent;
	m_nodes[newParent].userData = nullptr;
	m_nodes[newParent].aabb.Combine(leafAABB, m_nodes[sibling].aabb);
	m_nodes[newParent].height = m_nodes[sibling].GetHeight() + 1;

	if (oldParent != b2_nullNode)
	{
		// The sibling was not the root.
		if (m_nodes[oldParent].child1 == sibling)
		{
			m_nodes[oldParent].child1 = newParent;
		}
		else
		{
			m_nodes[oldParent].child2 = newParent;
		}

		m_nodes[newParent].child1 = sibling;
		m_nodes[newParent].child2 = leaf;
		m_nodes[sibling].parent = newParent;
		m_nodes[leaf].parent = newParent;
	}
	else
	{
		// The sibling was the root.
		m_nodes[newParent].child1 = sibling;
		m_nodes[newParent].child2 = leaf;
		m_nodes[sibling].parent = newParent;
		m_nodes[leaf].parent = newParent;
		root = newParent;
	}

	// Walk back up the tree fixing heights and AABBs
	index = m_nodes[leaf].parent;
	while (index != b2_nullNode)
	{
		index = Balance(root, index);

		int32 child1 = m_nodes[index].child1;
		int32 child2 = m_nodes[index].child2;

		b2Assert(child1 != b2_nullNode);
		b2Assert(child2 != b2_nullNode);

		m_nodes[index].height = 1 + b2Max(m_nodes[child1].GetHeight(), m_nodes[child2].GetHeight());
		m_nodes[index].aabb.Combine(m_nodes[child1].aabb, m_nodes[child2].aabb);

		index = m_nodes[index].parent;
	}
}

void b2DynamicTreeOfTrees::RemoveLeaf(int32& root, int32 leaf)
{
	if (leaf == root)
	{
		root = b2_nullNode;
		return;
	}

	int32 parent = m_nodes[leaf].parent;
    b2Assert(parent != b2_nullNode);
    b2Assert(m_nodes[parent].IsLeaf() == false);
	int32 grandParent = m_nodes[parent].parent;
	int32 sibling;
	if (m_nodes[parent].child1 == leaf)
	{
		sibling = m_nodes[parent].child2;
	}
	else
	{
		sibling = m_nodes[parent].child1;
	}

	if (grandParent != b2_nullNode)
	{
        b2Assert(m_nodes[grandParent].IsLeaf() == false);

		// Destroy parent and connect sibling to grandParent.
		if (m_nodes[grandParent].child1 == parent)
		{
			m_nodes[grandParent].child1 = sibling;
		}
		else
		{
			m_nodes[grandParent].child2 = sibling;
		}
		m_nodes[sibling].parent = grandParent;
		FreeNode(parent);

		// Adjust ancestor bounds.
		int32 index = grandParent;
		while (index != b2_nullNode)
		{
            b2Assert(m_nodes[index].IsLeaf() == false);
			index = Balance(root, index);
            b2Assert(m_nodes[index].IsLeaf() == false);

			int32 child1 = m_nodes[index].child1;
			int32 child2 = m_nodes[index].child2;

			m_nodes[index].aabb.Combine(m_nodes[child1].aabb, m_nodes[child2].aabb);
			m_nodes[index].height = 1 + b2Max(m_nodes[child1].GetHeight(), m_nodes[child2].GetHeight());

			index = m_nodes[index].parent;
		}
	}
	else
	{
		root = sibling;
		m_nodes[sibling].parent = b2_nullNode;
		FreeNode(parent);
	}

#ifdef b2_validateTree
	Validate();
#endif
}

// Perform a left or right rotation if node A is imbalanced.
// Returns the new root index.
int32 b2DynamicTreeOfTrees::Balance(int32& root, int32 iA)
{
	b2Assert(iA != b2_nullNode);

	Node* A = m_nodes + iA;
	if (A->IsLeaf() || A->GetHeight() < 2)
	{
		return iA;
	}

	int32 iB = A->child1;
	int32 iC = A->child2;
	b2Assert(0 <= iB && iB < m_nodeCapacity);
	b2Assert(0 <= iC && iC < m_nodeCapacity);

	Node* B = m_nodes + iB;
	Node* C = m_nodes + iC;

	int32 balance = C->GetHeight() - B->GetHeight();

	// Rotate C up
	if (balance > 1)
	{
		int32 iF = C->child1;
		int32 iG = C->child2;
		Node* F = m_nodes + iF;
		Node* G = m_nodes + iG;
		b2Assert(0 <= iF && iF < m_nodeCapacity);
		b2Assert(0 <= iG && iG < m_nodeCapacity);

		// Swap A and C
		C->child1 = iA;
		C->parent = A->parent;
		A->parent = iC;

		// A's old parent should point to C
		if (C->parent != b2_nullNode)
		{
			if (m_nodes[C->parent].child1 == iA)
			{
				m_nodes[C->parent].child1 = iC;
			}
			else
			{
				b2Assert(m_nodes[C->parent].child2 == iA);
				m_nodes[C->parent].child2 = iC;
			}
		}
		else
		{
			root = iC;
		}

		// Rotate
		if (F->GetHeight() > G->GetHeight())
		{
			C->child2 = iF;
			A->child2 = iG;
			G->parent = iA;
			A->aabb.Combine(B->aabb, G->aabb);
			C->aabb.Combine(A->aabb, F->aabb);

			A->height = 1 + b2Max(B->GetHeight(), G->GetHeight());
			C->height = 1 + b2Max(A->GetHeight(), F->GetHeight());
		}
		else
		{
			C->child2 = iG;
			A->child2 = iF;
			F->parent = iA;
			A->aabb.Combine(B->aabb, F->aabb);
			C->aabb.Combine(A->aabb, G->aabb);

			A->height = 1 + b2Max(B->GetHeight(), F->GetHeight());
			C->height = 1 + b2Max(A->GetHeight(), G->GetHeight());
		}

		return iC;
	}

	// Rotate B up
	if (balance < -1)
	{
		int32 iD = B->child1;
		int32 iE = B->child2;
		Node* D = m_nodes + iD;
		Node* E = m_nodes + iE;
		b2Assert(0 <= iD && iD < m_nodeCapacity);
		b2Assert(0 <= iE && iE < m_nodeCapacity);

		// Swap A and B
		B->child1 = iA;
		B->parent = A->parent;
		A->parent = iB;

		// A's old parent should point to B
		if (B->parent != b2_nullNode)
		{
			if (m_nodes[B->parent].child1 == iA)
			{
				m_nodes[B->parent].child1 = iB;
			}
			else
			{
				b2Assert(m_nodes[B->parent].child2 == iA);
				m_nodes[B->parent].child2 = iB;
			}
		}
		else
		{
			root = iB;
		}

		// Rotate
		if (D->GetHeight() > E->GetHeight())
		{
			B->child2 = iD;
			A->child1 = iE;
			E->parent = iA;
			A->aabb.Combine(C->aabb, E->aabb);
			B->aabb.Combine(A->aabb, D->aabb);

			A->height = 1 + b2Max(C->GetHeight(), E->GetHeight());
			B->height = 1 + b2Max(A->GetHeight(), D->GetHeight());
		}
		else
		{
			B->child2 = iE;
			A->child1 = iD;
			D->parent = iA;
			A->aabb.Combine(C->aabb, D->aabb);
			B->aabb.Combine(A->aabb, E->aabb);

			A->height = 1 + b2Max(C->GetHeight(), D->GetHeight());
			B->height = 1 + b2Max(A->GetHeight(), E->GetHeight());
		}

		return iB;
	}

	return iA;
}

int32 b2DynamicTreeOfTrees::GetHeight(int32 nodeId) const
{
	b2Assert(0 <= nodeId && nodeId < m_nodeCapacity);
	Node* node = m_nodes + nodeId;

	if (node->IsLeaf())
	{
        if (node->subTreeRoot != b2_nullNode)
        {
            return 1 + m_nodes[node->subTreeRoot].GetHeight();
        }
        else
        {
            return 0;
        }
	}

	int32 height1 = GetHeight(node->child1);
	int32 height2 = GetHeight(node->child2);
	return 1 + b2Max(height1, height2);
}

int32 b2DynamicTreeOfTrees::GetHeight() const
{
    if (m_root == b2_nullNode)
    {
        return 0;
    }

    return GetHeight(m_root);
}

//
float32 b2DynamicTreeOfTrees::GetAreaRatio() const
{
	if (m_root == b2_nullNode)
	{
		return 0.0f;
	}

	const Node* root = m_nodes + m_root;
	float32 rootArea = root->aabb.GetPerimeter();

	float32 totalArea = 0.0f;
	for (int32 i = 0; i < m_nodeCapacity; ++i)
	{
		const Node* node = m_nodes + i;
		if (node->GetHeight() < 0)
		{
			// Free node in pool
			continue;
		}

		totalArea += node->aabb.GetPerimeter();
        b2Assert(b2IsValid(totalArea));
	}

	return totalArea / rootArea;
}

// Compute the height of a sub-tree.
template<bool isSubTree>
int32 b2DynamicTreeOfTrees::ComputeHeight(int32 nodeId) const
{
	b2Assert(0 <= nodeId && nodeId < m_nodeCapacity);
	Node* node = m_nodes + nodeId;

	if (node->IsLeaf())
	{
        if (isSubTree || node->subTreeRoot == b2_nullNode)
        {
    		return 0;
        }
        else
        {
            return 1 + ComputeHeight<true>(node->subTreeRoot);
        }
	}

	int32 height1 = ComputeHeight<isSubTree>(node->child1);
	int32 height2 = ComputeHeight<isSubTree>(node->child2);
	return 1 + b2Max(height1, height2);
}

int32 b2DynamicTreeOfTrees::ComputeHeight() const
{
	int32 height = ComputeHeight<false>(m_root);
	return height;
}

template<bool isSubTree>
void b2DynamicTreeOfTrees::ValidateStructure(int32 index) const
{
	if (index == b2_nullNode)
	{
		return;
	}

	if (index == m_root)
	{
		b2Assert(m_nodes[index].parent == b2_nullNode);
	}

	const Node* node = m_nodes + index;

	int32 child1 = node->child1;
	int32 child2 = node->child2;

	if (node->IsLeaf())
	{
        if (isSubTree)
        {
            b2Assert(child1 == b2_nullNode);
            b2Assert(node->GetHeight() == 0);
        }
        else if (node->subTreeRoot != b2_nullNode)
        {
            b2Assert(m_nodes[node->subTreeRoot].parent == b2_nullNode);
            ValidateStructure<true>(node->subTreeRoot);
        }
		return;
	}

	b2Assert(0 <= child1 && child1 < m_nodeCapacity);
	b2Assert(0 <= child2 && child2 < m_nodeCapacity);

	b2Assert(m_nodes[child1].parent == index);
	b2Assert(m_nodes[child2].parent == index);

	ValidateStructure<isSubTree>(child1);
	ValidateStructure<isSubTree>(child2);
}

template<bool isSubTree>
void b2DynamicTreeOfTrees::ValidateMetrics(int32 index) const
{
	if (index == b2_nullNode)
	{
		return;
	}

	const Node* node = m_nodes + index;

	int32 child1 = node->child1;
	int32 child2 = node->child2;

	if (node->IsLeaf())
	{
        if (isSubTree)
        {
            b2Assert(child1 == b2_nullNode);
            b2Assert(node->GetHeight() == 0);
        }
        else
        {
            ValidateMetrics<true>(node->subTreeRoot);
        }
        return;
	}

	b2Assert(0 <= child1 && child1 < m_nodeCapacity);
	b2Assert(0 <= child2 && child2 < m_nodeCapacity);

	int32 height1 = m_nodes[child1].GetHeight();
	int32 height2 = m_nodes[child2].GetHeight();
	int32 height;
	height = 1 + b2Max(height1, height2);
	b2Assert(node->GetHeight() == height);

	b2AABB aabb;
	aabb.Combine(m_nodes[child1].aabb, m_nodes[child2].aabb);

	b2Assert(aabb.lowerBound == node->aabb.lowerBound);
	b2Assert(aabb.upperBound == node->aabb.upperBound);

	ValidateMetrics<isSubTree>(child1);
	ValidateMetrics<isSubTree>(child2);
}

void b2DynamicTreeOfTrees::Validate() const
{
#if defined(b2DEBUG)
	ValidateStructure<false>(m_root);
	ValidateMetrics<false>(m_root);

	int32 freeCount = 0;
	int32 freeIndex = m_freeList;
	while (freeIndex != b2_nullNode)
	{
		b2Assert(0 <= freeIndex && freeIndex < m_nodeCapacity);
		freeIndex = m_nodes[freeIndex].next;
		++freeCount;
	}

    int32 height = GetHeight();
    int32 computedHeight = ComputeHeight();
	b2Assert(height == computedHeight);

	b2Assert(m_nodeCount + freeCount == m_nodeCapacity);
#endif
}

int32 b2DynamicTreeOfTrees::GetMaxBalance() const
{
	int32 maxBalance = 0;
	for (int32 i = 0; i < m_nodeCapacity; ++i)
	{
		const Node* node = m_nodes + i;
		if (node->GetHeight() <= 1)
		{
			continue;
		}

		b2Assert(node->IsLeaf() == false);

		int32 child1 = node->child1;
		int32 child2 = node->child2;
		int32 balance = b2Abs(m_nodes[child2].GetHeight() - m_nodes[child1].GetHeight());
		maxBalance = b2Max(maxBalance, balance);
	}

	return maxBalance;
}

void b2DynamicTreeOfTrees::ShiftOrigin(const b2Vec2& newOrigin)
{
	// Build array of leaves. Free the rest.
	for (int32 i = 0; i < m_nodeCapacity; ++i)
	{
		m_nodes[i].aabb.lowerBound -= newOrigin;
		m_nodes[i].aabb.upperBound -= newOrigin;
	}
}
