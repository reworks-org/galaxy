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

#ifndef B2_DYNAMIC_TREE_OF_TREES_H
#define B2_DYNAMIC_TREE_OF_TREES_H

#include "Box2D/Collision/b2Collision.h"
#include "Box2D/Common/b2GrowableStack.h"
#include <algorithm>

#define b2_nullNode (-1)

/// A dynamic AABB tree-of-trees broad-phase, based on Erin Cato's b2DynamicTree.
/// The base tree's leaves form a sparse grid, with each containing the root node
/// of a sub-tree. This can improve tree quality in the case of thousands of proxies,
/// and allows parallel sub-tree proxy insertion/removal (in theory; TODO_MT:
/// implement parallel SynchronizeFixtures).
///
/// A dynamic tree arranges data in a binary tree to accelerate
/// queries such as volume queries and ray casts. Leafs are proxies
/// with an AABB. In the tree we expand the proxy AABB by b2_fatAABBFactor
/// so that the proxy AABB is bigger than the client object. This allows the client
/// object to move by small amounts without triggering a tree update.
///
/// Nodes are pooled and relocatable, so we use node indices rather than pointers.
class b2DynamicTreeOfTrees
{
public:
	b2DynamicTreeOfTrees(float32 subTreeWidth = 1000.0f, float32 subTreeHeight = 1000.0f);

	~b2DynamicTreeOfTrees();

	/// Destroy all proxies and set the sub-tree dimensions.
	void Reset(float32 subTreeWidth, float32 subTreeHeight);

	/// Create a proxy. Provide a tight fitting AABB and a userData pointer.
	int32 CreateProxy(const b2AABB& aabb, void* userData);

	/// Destroy a proxy. This asserts if the id is invalid.
	void DestroyProxy(int32 proxyId);

	/// Move a proxy with a swepted AABB. If the proxy has moved outside of its fattened AABB,
	/// then the proxy is removed from the tree and re-inserted. Otherwise
	/// the function returns immediately.
	/// @return true if the proxy was re-inserted.
	bool MoveProxy(int32 proxyId, const b2AABB& aabb1, const b2Vec2& displacement);

	/// Get proxy user data.
	/// @return the proxy user data or 0 if the id is invalid.
	void* GetUserData(int32 proxyId) const;

	/// Get the fat AABB for a proxy.
	const b2AABB& GetFatAABB(int32 proxyId) const;

	/// Query an AABB for overlapping proxies. The callback class
	/// is called for each proxy that overlaps the supplied AABB.
	template <typename T>
	void Query(T* callback, const b2AABB& aabb, uint32 threadId);

	/// Ray-cast against the proxies in the tree. This relies on the callback
	/// to perform a exact ray-cast in the case were the proxy contains a shape.
	/// The callback also performs the any collision filtering. This has performance
	/// roughly equal to k * log(n), where k is the number of collisions and n is the
	/// number of proxies in the tree.
	/// @param input the ray-cast input data. The ray extends from p1 to p1 + maxFraction * (p2 - p1).
	/// @param callback a callback class that is called for each proxy that is hit by the ray.
	template <typename T>
	void RayCast(T* callback, const b2RayCastInput& input, uint32 threadId);

	/// Visit every leaf in the base tree.
	template <typename T>
	void VisitBaseTree(T* callback) const;

	/// Validate this tree. For testing.
	void Validate() const;

	/// Compute the height of the binary tree in O(log(N)) time. Should not be
	/// called often.
	int32 GetHeight() const;

	/// Get the maximum balance of an node in the tree. The balance is the difference
	/// in height of the two children of a node.
	int32 GetMaxBalance() const;

	/// Get the ratio of the sum of the node areas to the root area.
	float32 GetAreaRatio() const;

	/// Shift the world origin. Useful for large worlds.
	/// The shift formula is: position -= newOrigin
	/// @param newOrigin the new origin with respect to the old origin
	void ShiftOrigin(const b2Vec2& newOrigin);

	/// Get the width of sub-trees.
	int32 GetSubTreeWidth() const;

	/// Get the height of sub-trees.
	int32 GetSubTreeHeight() const;

private:
	struct Node
	{
		bool IsLeaf() const;
		int32 GetHeight() const;

		// Enlarged AABB
		b2AABB aabb;

		void* userData;

		union
		{
			int32 parent;
			int32 next;
		};

		int32 child1;

		union
		{
			int32 child2;

			// The proxy's leaf in the next overlapped sub-tree.
			int32 nextProxy;
		};

		union
		{
			// leaf = invalid, free node = -1
			int32 height;

			// The base tree leaf for a sub-tree leaf.
			int32 baseTreeLeaf;

			// The sub-tree root for a base tree leaf.
			int32 subTreeRoot;
		};

		// The user-facing proxy id. This is the head of the nextProxy list.
		int32 proxy;
	};

	struct PerThreadData
	{
		uint32* m_proxyQueryCounters;
		uint32 m_queryCounter;

		uint8 _padding[b2_cacheLineSize];
	};

	friend struct b2InsertLeafQueryCallback;

	int32 AllocateNode();
	void FreeNode(int32 node);

	void InsertLeaf(int32 node);
	void InsertLeaf(int32& root, int32 node);

	void RemoveLeaf(int32 node);
	void RemoveLeaf(int32& root, int32 node);

	int32 Balance(int32& root, int32 index);

	int32 GetHeight(int32 nodeId) const;

	int32 ComputeHeight() const;
	template<bool isSubTree>
	int32 ComputeHeight(int32 nodeId) const;

	template<bool isSubTree>
	void ValidateStructure(int32 index) const;
	template<bool isSubTree>
	void ValidateMetrics(int32 index) const;

	template <typename T>
	void Query(T* callback, const b2Vec2& p) const;

	template <bool querySubTrees, bool useCounters, typename T>
	bool Query(int32 root, T* callback, const b2AABB& aabb, uint32 threadId);

	template <bool querySubTrees, typename T>
	bool RayCast(int32 root, T* callback, const b2RayCastInput& input, uint32 threadId);

	PerThreadData m_perThreadData[b2_maxThreads];

	// The root tree. Its leaves hold sub-trees.
	int32 m_root;

	float32 m_subTreeWidth;
	float32 m_subTreeHeight;
	float32 m_subTreeInvWidth;
	float32 m_subTreeInvHeight;

	int32 m_nodeCount;
	Node* m_nodes;
	int32 m_nodeCapacity;

	int32 m_freeList;

	/// This is used to incrementally traverse the tree for re-balancing.
	uint32 m_path;

	int32 m_insertionCount;
};

inline bool b2DynamicTreeOfTrees::Node::IsLeaf() const
{
	return child1 == b2_nullNode;
}

inline int32 b2DynamicTreeOfTrees::Node::GetHeight() const
{
	if (IsLeaf())
	{
		return 0;
	}
	return height;
}

inline void* b2DynamicTreeOfTrees::GetUserData(int32 proxyId) const
{
	b2Assert(0 <= proxyId && proxyId < m_nodeCapacity);
	return m_nodes[proxyId].userData;
}

inline const b2AABB& b2DynamicTreeOfTrees::GetFatAABB(int32 proxyId) const
{
	b2Assert(0 <= proxyId && proxyId < m_nodeCapacity);
	return m_nodes[proxyId].aabb;
}

template <typename T>
b2_forceInline void b2DynamicTreeOfTrees::Query(T* callback, const b2AABB& aabb, uint32 threadId)
{
	PerThreadData& td = m_perThreadData[threadId];
	if (++td.m_queryCounter == 0)
	{
		memset(td.m_proxyQueryCounters, 0, sizeof(uint32));
		td.m_queryCounter = 1;
	}
	Query<true, true>(m_root, callback, aabb, threadId);
}

template <typename T>
b2_forceInline void b2DynamicTreeOfTrees::RayCast(T* callback, const b2RayCastInput& input, uint32 threadId)
{
	PerThreadData& td = m_perThreadData[threadId];
	if (++td.m_queryCounter == 0)
	{
		memset(td.m_proxyQueryCounters, 0, sizeof(uint32));
		td.m_queryCounter = 1;
	}
	RayCast<true>(m_root, callback, input, threadId);
}

template <typename T>
b2_forceInline void b2DynamicTreeOfTrees::VisitBaseTree(T* callback) const
{
	b2GrowableStack<int32, 256> stack;
	stack.Push(m_root);

	while (stack.GetCount() > 0)
	{
		int32 nodeId = stack.Pop();
		if (nodeId == b2_nullNode)
		{
			continue;
		}

		const Node* node = m_nodes + nodeId;

		if (node->IsLeaf())
		{
			bool proceed = callback->QueryCallback(node->proxy);
			if (proceed == false)
			{
				return;
			}
		}
		else
		{
			stack.Push(node->child1);
			stack.Push(node->child2);
		}
	}
}

template <typename T>
b2_forceInline void b2DynamicTreeOfTrees::Query(T* callback, const b2Vec2& p) const
{
	b2GrowableStack<int32, 256> stack;
	stack.Push(m_root);

	while (stack.GetCount() > 0)
	{
		int32 nodeId = stack.Pop();
		if (nodeId == b2_nullNode)
		{
			continue;
		}

		const Node* node = m_nodes + nodeId;

		if (node->aabb.Contains(p))
		{
			if (node->IsLeaf())
			{
				bool proceed = callback->QueryCallback(node->proxy);
				if (proceed == false)
				{
					return;
				}
			}
			else
			{
				stack.Push(node->child1);
				stack.Push(node->child2);
			}
		}
	}
}

template <bool querySubTrees, bool useCounters, typename T>
b2_forceInline bool b2DynamicTreeOfTrees::Query(int32 root, T* callback, const b2AABB& aabb, uint32 threadId)
{
	PerThreadData& td = m_perThreadData[threadId];

	b2GrowableStack<int32, 256> stack;
	stack.Push(root);

	while (stack.GetCount() > 0)
	{
		int32 nodeId = stack.Pop();
		if (nodeId == b2_nullNode)
		{
			continue;
		}

		const Node* node = m_nodes + nodeId;

		if (b2TestOverlap(node->aabb, aabb))
		{
			if (node->IsLeaf())
			{
				if (querySubTrees)
				{
					b2Assert(node->subTreeRoot != b2_nullNode);
					bool proceed = Query<false, useCounters>(node->subTreeRoot, callback, aabb, threadId);
					if (proceed == false)
					{
						return false;
					}
				}
				else if (useCounters == false || td.m_proxyQueryCounters[node->proxy] != td.m_queryCounter)
				{
					if (useCounters)
					{
						td.m_proxyQueryCounters[node->proxy] = td.m_queryCounter;
					}
					bool proceed = callback->QueryCallback(node->proxy);
					if (proceed == false)
					{
						return false;
					}
				}
			}
			else
			{
				stack.Push(node->child1);
				stack.Push(node->child2);
			}
		}
	}

	return true;
}

template <bool querySubTrees, typename T>
b2_forceInline bool b2DynamicTreeOfTrees::RayCast(int32 root, T* callback, const b2RayCastInput& input, uint32 threadId)
{
	PerThreadData& td = m_perThreadData[threadId];

	b2Vec2 p1 = input.p1;
	b2Vec2 p2 = input.p2;
	b2Vec2 r = p2 - p1;
	b2Assert(r.LengthSquared() > 0.0f);
	r.Normalize();

	// v is perpendicular to the segment.
	b2Vec2 v = b2Cross(1.0f, r);
	b2Vec2 abs_v = b2Abs(v);

	// Separating axis for segment (Gino, p80).
	// |dot(v, p1 - c)| > dot(|v|, h)

	float32 maxFraction = input.maxFraction;

	// Build a bounding box for the segment.
	b2AABB segmentAABB;
	{
		b2Vec2 t = p1 + maxFraction * (p2 - p1);
		segmentAABB.lowerBound = b2Min(p1, t);
		segmentAABB.upperBound = b2Max(p1, t);
	}

	b2GrowableStack<int32, 256> stack;
	stack.Push(root);

	while (stack.GetCount() > 0)
	{
		int32 nodeId = stack.Pop();
		if (nodeId == b2_nullNode)
		{
			continue;
		}

		const Node* node = m_nodes + nodeId;

		if (b2TestOverlap(node->aabb, segmentAABB) == false)
		{
			continue;
		}

		// Separating axis for segment (Gino, p80).
		// |dot(v, p1 - c)| > dot(|v|, h)
		b2Vec2 c = node->aabb.GetCenter();
		b2Vec2 h = node->aabb.GetExtents();
		float32 separation = b2Abs(b2Dot(v, p1 - c)) - b2Dot(abs_v, h);
		if (separation > 0.0f)
		{
			continue;
		}

		if (node->IsLeaf())
		{
			if (querySubTrees)
			{
				b2Assert(node->subTreeRoot != b2_nullNode);
				bool proceed = RayCast<false>(node->subTreeRoot, callback, input, threadId);
				if (proceed == false)
				{
					return false;
				}
			}
			else if (td.m_proxyQueryCounters[node->proxy] != td.m_queryCounter)
			{
				td.m_proxyQueryCounters[node->proxy] = td.m_queryCounter;

				b2RayCastInput subInput;
				subInput.p1 = input.p1;
				subInput.p2 = input.p2;
				subInput.maxFraction = maxFraction;

				float32 value = callback->RayCastCallback(subInput, node->proxy);

				if (value == 0.0f)
				{
					// The client has terminated the ray cast.
					return false;
				}

				if (value > 0.0f)
				{
					// Update segment bounding box.
					maxFraction = value;
					b2Vec2 t = p1 + maxFraction * (p2 - p1);
					segmentAABB.lowerBound = b2Min(p1, t);
					segmentAABB.upperBound = b2Max(p1, t);
				}
			}
		}
		else
		{
			stack.Push(node->child1);
			stack.Push(node->child2);
		}
	}

	return true;
}

inline int32 b2DynamicTreeOfTrees::GetSubTreeWidth() const
{
	return m_subTreeWidth;
}

inline int32 b2DynamicTreeOfTrees::GetSubTreeHeight() const
{
	return m_subTreeHeight;
}

#endif
