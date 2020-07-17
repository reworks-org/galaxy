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

#include <cstring>
#include "Box2D/MT/b2DynamicTreeOfTrees.h"
#include "Box2D/MT/b2MtUtil.h"
#include "Box2D/MT/b2ThreadDataSorter.h"

//#define b2_debugEnabled

#ifdef b2_debugEnabled

#define b2_validateTree

#include <bitset>
#include <mutex>

static const int32 kAllocationBitCount = 131072;
static std::bitset<kAllocationBitCount> g_debugAllocated;
static std::bitset<kAllocationBitCount> g_debugInBaseLeafFreeList;
static std::bitset<kAllocationBitCount> g_debugInserted;
static std::mutex g_debugMutex;
static uint32 g_debugCounter;
static uint32 g_debugFinishMoveProxyCounter;

#define b2_debugPrintCondition() (nodeId == b2_nullNode)
#define b2_debugPrintExtra()

#endif

void b2DebugReset()
{
#ifdef b2_debugEnabled
	std::lock_guard<std::mutex> lk(g_debugMutex);
	g_debugAllocated.reset();
	g_debugInBaseLeafFreeList.reset();
	g_debugInserted.reset();
	g_debugCounter = 0;
	g_debugFinishMoveProxyCounter = 0;
#endif
}

inline void b2DynamicTreeOfTrees::DebugNodeAllocate(int32 nodeId) const
{
	B2_NOT_USED(nodeId);
#ifdef b2_debugEnabled
	std::lock_guard<std::mutex> lk(g_debugMutex);
	b2Assert(nodeId < kAllocationBitCount);
	b2Assert(g_debugAllocated[nodeId] == false);
	b2Assert(g_debugInBaseLeafFreeList[nodeId] == false);
	if (b2_debugPrintCondition())
	{
		b2Log("[%d] DebugNodeAllocate(%d)\n", g_debugCounter++, nodeId);
		b2_debugPrintExtra();
	}
	g_debugAllocated[nodeId] = true;
#endif
}

inline void b2DynamicTreeOfTrees::DebugNodeFree(int32 nodeId) const
{
	B2_NOT_USED(nodeId);
#ifdef b2_debugEnabled
	std::lock_guard<std::mutex> lk(g_debugMutex);
	b2Assert(nodeId < kAllocationBitCount);
	b2Assert(g_debugAllocated[nodeId] == true);
	b2Assert(g_debugInBaseLeafFreeList[nodeId] == false);
	if (b2_debugPrintCondition())
	{
		b2Log("[%d] DebugNodeFree(%d)\n", g_debugCounter++, nodeId);
		b2_debugPrintExtra();
	}
	g_debugAllocated[nodeId] = false;
#endif
}

inline void b2DynamicTreeOfTrees::DebugNodeSubTreeAllocate(int32 nodeId) const
{
	B2_NOT_USED(nodeId);
#ifdef b2_debugEnabled
	std::lock_guard<std::mutex> lk(g_debugMutex);
	b2Assert(nodeId < kAllocationBitCount);
	b2Assert(g_debugInBaseLeafFreeList[nodeId] == true);
	b2Assert(g_debugAllocated[nodeId] == true);
	if (b2_debugPrintCondition())
	{
		b2Log("[%d] DebugNodeSubTreeAllocate(%d)\n", g_debugCounter++, nodeId);
		b2_debugPrintExtra();
	}
	g_debugInBaseLeafFreeList[nodeId] = false;
#endif
}

inline void b2DynamicTreeOfTrees::DebugNodeSubTreeFree(int32 nodeId) const
{
	B2_NOT_USED(nodeId);
#ifdef b2_debugEnabled
	std::lock_guard<std::mutex> lk(g_debugMutex);
	b2Assert(nodeId < kAllocationBitCount);
	b2Assert(g_debugInBaseLeafFreeList[nodeId] == false);
	b2Assert(g_debugAllocated[nodeId] == true);
	if (b2_debugPrintCondition())
	{
		b2Log("[%d] DebugNodeSubTreeFree(%d)\n", g_debugCounter++, nodeId);
		b2_debugPrintExtra();
	}
	g_debugInBaseLeafFreeList[nodeId] = true;
#endif
}

inline void b2DynamicTreeOfTrees::DebugInsert(int32 nodeId) const
{
	B2_NOT_USED(nodeId);
#ifdef b2_debugEnabled
	std::lock_guard<std::mutex> lk(g_debugMutex);
	b2Assert(nodeId < kAllocationBitCount);
	b2Assert(g_debugInserted[nodeId] == false);
	if (b2_debugPrintCondition())
	{
		b2Log("[%d] DebugInsert(%d)\n", g_debugCounter++, nodeId);
		b2_debugPrintExtra();
	}
	g_debugInserted[nodeId] = true;
#endif
}

inline void b2DynamicTreeOfTrees::DebugRemove(int32 nodeId) const
{
	B2_NOT_USED(nodeId);
#ifdef b2_debugEnabled
	std::lock_guard<std::mutex> lk(g_debugMutex);
	b2Assert(nodeId < kAllocationBitCount);
	b2Assert(g_debugInserted[nodeId] == true);
	if (b2_debugPrintCondition())
	{
		b2Log("[%d] DebugRemove(%d)\n", g_debugCounter++, nodeId);
		b2_debugPrintExtra();
	}
	g_debugInserted[nodeId] = false;
#endif
}

inline void b2DynamicTreeOfTrees::DebugReplace(int32 nodeId) const
{
	B2_NOT_USED(nodeId);
#ifdef b2_debugEnabled
	std::lock_guard<std::mutex> lk(g_debugMutex);
	if (b2_debugPrintCondition())
	{
		b2Log("[%d] DebugReplace(%d)\n", g_debugCounter++, nodeId);
		b2_debugPrintExtra();
	}
#endif
}

inline void b2DynamicTreeOfTrees::DebugFinishMoveProxies() const
{
#ifdef b2_debugEnabled
	std::lock_guard<std::mutex> lk(g_debugMutex);
	++g_debugFinishMoveProxyCounter;
#endif
}

bool b2DeferredInsertNewSubTreeLessThan(const b2DynamicTreeOfTrees::DeferredInsertNewSubTree& a,
										const b2DynamicTreeOfTrees::DeferredInsertNewSubTree& b)
{
	if (a.subTreePosition.y != b.subTreePosition.y)
	{
		return a.subTreePosition.y < b.subTreePosition.y;
	}

	if (a.subTreePosition.x != b.subTreePosition.x)
	{
		return a.subTreePosition.x < b.subTreePosition.x;
	}

	return a.proxy < b.proxy;
}

bool b2DeferredInsertLessThan(	const b2DynamicTreeOfTrees::DeferredInsert& a,
								const b2DynamicTreeOfTrees::DeferredInsert& b)
{
	if (a.baseLeaf != b.baseLeaf)
	{
		return a.baseLeaf < b.baseLeaf;
	}
	return a.proxy < b.proxy;
}

bool b2DeferredMoveLessThan(	const b2DynamicTreeOfTrees::DeferredMove& a,
								const b2DynamicTreeOfTrees::DeferredMove& b)
{
	if (a.baseLeaf != b.baseLeaf)
	{
		return a.baseLeaf < b.baseLeaf;
	}
	return a.subProxy < b.subProxy;
}

bool b2DeferredRemoveLessThan(	const b2DynamicTreeOfTrees::DeferredRemove& a,
								const b2DynamicTreeOfTrees::DeferredRemove& b)
{
	if (a.baseLeaf != b.baseLeaf)
	{
		return a.baseLeaf < b.baseLeaf;
	}
	return a.subProxy < b.subProxy;
}

bool b2DeferredReplaceLessThan(	const b2DynamicTreeOfTrees::DeferredReplace& a,
								const b2DynamicTreeOfTrees::DeferredReplace& b)
{
	// A source proxy can't have multiple replaces in the same event queue.
	b2Assert(a.proxy != b.proxy);

	return a.proxy < b.proxy;
}

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

class b2UpdateSubTreeTask : public b2Task
{
public:
	b2UpdateSubTreeTask() {}
	b2UpdateSubTreeTask(b2DynamicTreeOfTrees* tree,
		b2DynamicTreeOfTrees::DeferredInsert* inserts, uint32 insertCount,
		b2DynamicTreeOfTrees::DeferredMove* moves, uint32 moveCount,
		b2DynamicTreeOfTrees::DeferredRemove* removes, uint32 removeCount)
	: m_tree(tree)
	, m_inserts(inserts)
	, m_moves(moves)
	, m_removes(removes)
	, m_insertCount(insertCount)
	, m_moveCount(moveCount)
	, m_removeCount(removeCount)
	{}

	virtual b2Task::Type GetType() const override { return b2Task::e_broadPhaseUpdateSubTree; }

	virtual void Execute(const b2ThreadContext&) override
	{
		// Remove proxies.
		for (uint32 i = 0; i < m_removeCount; ++i)
		{
			int32 baseLeaf = m_removes[i].baseLeaf;
			int32 subProxy = m_removes[i].subProxy;
			bool freeSubProxy = m_removes[i].freeSubProxy != 0;

			m_tree->SubTreeRemoveLeaf<true>(&m_tree->m_nodes[baseLeaf].subTreeRoot, subProxy);

			if (freeSubProxy)
			{
				m_tree->SubTreeFreeNode(subProxy, baseLeaf);
			}
		}

		// Move proxies.
		for (uint32 i = 0; i < m_moveCount; ++i)
		{
			int32 baseLeaf = m_moves[i].baseLeaf;
			int32 subProxy = m_moves[i].subProxy;

			m_tree->SubTreeRemoveLeaf<true>(&m_tree->m_nodes[baseLeaf].subTreeRoot, subProxy);

			m_tree->m_nodes[subProxy].aabb = m_moves[i].aabb;

			m_tree->SubTreeInsertLeaf<true>(&m_tree->m_nodes[baseLeaf].subTreeRoot, subProxy);
		}

		// Insert proxies.
		for (uint32 i = 0; i < m_insertCount; ++i)
		{
			int32 baseLeaf = m_inserts[i].baseLeaf;
			int32 subProxy = m_inserts[i].proxy;

			m_tree->m_nodes[subProxy].aabb = m_inserts[i].aabb;
			m_tree->m_nodes[subProxy].baseTreeLeaf = baseLeaf;

			m_tree->SubTreeInsertLeaf<true>(&m_tree->m_nodes[baseLeaf].subTreeRoot, subProxy);
		}
	}

private:
	b2DynamicTreeOfTrees* m_tree;
	b2DynamicTreeOfTrees::DeferredInsert* m_inserts;
	b2DynamicTreeOfTrees::DeferredMove* m_moves;
	b2DynamicTreeOfTrees::DeferredRemove* m_removes;
	uint32 m_insertCount;
	uint32 m_moveCount;
	uint32 m_removeCount;
};

void b2DynamicTreeOfTrees::QueueMoveProxy(int32 proxyId, const b2AABB& aabb1, const b2Vec2& displacement, uint32 threadId)
{
	PerThreadData& td = m_perThreadData[threadId];

	b2AABB aabb = ComputeAABB(aabb1, displacement);

    int32 beginX = int32(std::round(aabb.lowerBound.x * m_subTreeInvWidth));
    int32 beginY = int32(std::round(aabb.lowerBound.y * m_subTreeInvHeight));
    int32 endX = int32(std::round(aabb.upperBound.x * m_subTreeInvWidth)) + 1;
    int32 endY = int32(std::round(aabb.upperBound.y * m_subTreeInvHeight)) + 1;

	int32 subProxyCount = 0;
	int32 firstProxy = proxyId;
	int32 prevSubProxy = b2_nullNode;
	for (int32 subProxy = proxyId; subProxy != b2_nullNode; subProxy = m_nodes[subProxy].nextProxy)
	{
		b2Assert(m_nodes[subProxy].IsLeaf());

		int32 baseTreeLeaf = m_nodes[subProxy].baseTreeLeaf;
		int32 x = m_nodes[baseTreeLeaf].subTreePosition.x;
		int32 y = m_nodes[baseTreeLeaf].subTreePosition.y;

		// Is the sub-proxy's tree still overlapped by the new aabb?
		if (beginX <= x && x < endX &&
			beginY <= y && y < endY)
		{
			// Move the sub-proxy.
			DeferredMove move;
			move.aabb = aabb;
			move.baseLeaf = baseTreeLeaf;
			move.subProxy = subProxy;
			td.m_moves.push_back(move);
			++subProxyCount;
			prevSubProxy = subProxy;
		}
		else
		{
			b2Assert(m_nodes[baseTreeLeaf].subTreeRoot == subProxy || m_nodes[subProxy].parent != b2_nullNode); // TEMP_MT

			// Remove the sub-proxy.
			DeferredRemove remove;
			remove.subProxy = subProxy;
			remove.baseLeaf = baseTreeLeaf;
			remove.freeSubProxy = subProxy != proxyId ? 1 : 0;
			td.m_removes.push_back(remove);

			if (subProxy != firstProxy)
			{
				// Remove from the proxy list.
				if (prevSubProxy != b2_nullNode)
				{
					m_nodes[prevSubProxy].nextProxy = m_nodes[subProxy].nextProxy;
				}
			}
			else
			{
				// The user-facing proxy must remain as the head of the proxy list.
				// The first valid proxy can be copied into the user proxy's slot (renamed).
				firstProxy = m_nodes[subProxy].nextProxy;
			}
		}
	}

	int32 subTreesX = endX - beginX;
	int32 subTreesY = endY - beginY;
	int32 subTreeCount = subTreesX * subTreesY;

	// Do we need to add sub-proxies to newly overlapped sub-trees?
	if (subProxyCount != subTreeCount)
	{
		b2Assert(subProxyCount < subTreeCount);

		// Find newly overlapped sub-trees. Create if they don't exist, otherwise insert.
		for (int32 y = beginY; y < endY; ++y)
		{
			for (int32 x = beginX; x < endX; ++x)
			{
				// Look for the sub-tree in the proxy list.
				int32 subProxy = firstProxy;
				while (subProxy != b2_nullNode)
				{
					int32 baseTreeLeaf = m_nodes[subProxy].baseTreeLeaf;
					int32 subProxyX = m_nodes[baseTreeLeaf].subTreePosition.x;
					int32 subProxyY = m_nodes[baseTreeLeaf].subTreePosition.y;

					if (x == subProxyX && y == subProxyY)
					{
						break;
					}

					subProxy = m_nodes[subProxy].nextProxy;
				}

				if (subProxy != b2_nullNode)
				{
					// We already have a sub-proxy in this sub-tree.
					continue;
				}

				// Look for the sub-tree by its position.
				b2Vec2 subTreeCenter(x * m_subTreeWidth, y * m_subTreeHeight);
				b2FindFirstLeafQueryCallback findQuery(this);
				Query(&findQuery, subTreeCenter);

				// Does the sub-tree need to be created?
				if (findQuery.m_leaf == b2_nullNode)
				{
					// Create the sub-tree and the sub-proxy.
					DeferredInsertNewSubTree insertNewSubTree;
					insertNewSubTree.subTreePosition.x = x;
					insertNewSubTree.subTreePosition.y = y;
					insertNewSubTree.proxy = proxyId;
					td.m_insertNewSubTrees.push_back(insertNewSubTree);
					continue;
				}

				// Create a sub-proxy and insert it into the sub-tree.
				DeferredInsert insert;
				insert.aabb = aabb;
				insert.baseLeaf = findQuery.m_leaf;
				insert.proxy = proxyId;
				td.m_inserts.push_back(insert);
			}
		}
	}

	// This would mean that the proxy isn't in any sub-trees.
	b2Assert(firstProxy != b2_nullNode);

	// The user proxy was removed from its sub-tree, so the next sub-proxy
	// needs to become the new user proxy.
	if (firstProxy != proxyId)
	{
		m_nodes[proxyId].nextProxy = firstProxy;

		DeferredReplace replace;
		replace.proxy = proxyId;
		td.m_replaces.push_back(replace);
	}
}

void b2DynamicTreeOfTrees::FinishMoveProxies(b2TaskExecutor& executor, b2TaskGroup* taskGroup, b2StackAllocator& allocator)
{
	uint32 sortCost = 5;

	b2_threadDataSorter(inserts, DeferredInsert, sortCost--, executor, allocator, m_perThreadData,
		&b2DynamicTreeOfTrees::PerThreadData::m_inserts, &b2DeferredInsertLessThan);

	b2_threadDataSorter(moves, DeferredMove, sortCost--, executor, allocator, m_perThreadData,
		&b2DynamicTreeOfTrees::PerThreadData::m_moves, &b2DeferredMoveLessThan);

	b2_threadDataSorter(removes, DeferredRemove, sortCost--, executor, allocator, m_perThreadData,
		&b2DynamicTreeOfTrees::PerThreadData::m_removes, &b2DeferredRemoveLessThan);

	b2_threadDataSorter(insertNewSubTrees, DeferredInsertNewSubTree, sortCost--, executor, allocator, m_perThreadData,
		&b2DynamicTreeOfTrees::PerThreadData::m_insertNewSubTrees, &b2DeferredInsertNewSubTreeLessThan);

	b2_threadDataSorter(replaces, DeferredReplace, sortCost--, executor, allocator, m_perThreadData,
		&b2DynamicTreeOfTrees::PerThreadData::m_replaces, &b2DeferredReplaceLessThan);

	b2Assert(sortCost == 0);

	// Allocate nodes for insertions.
	inserts.wait();
	for (DeferredInsert* it = inserts.begin(); it != inserts.end(); ++it)
	{
		int32 baseLeaf = it->baseLeaf;
		int32 proxy = it->proxy;

		int32 parent = AllocateNode();
		SubTreeFreeNode(parent, baseLeaf);

		int32 subProxy = AllocateNode();
		m_nodes[subProxy].userData = m_nodes[proxy].userData;
		m_nodes[subProxy].proxy = proxy;

		m_nodes[subProxy].nextProxy = m_nodes[proxy].nextProxy;
		m_nodes[proxy].nextProxy = subProxy;

		it->proxy = subProxy;
	}

	b2UpdateSubTreeTask* tasks = (b2UpdateSubTreeTask*)allocator.Allocate(m_subTreeCount * sizeof(b2UpdateSubTreeTask));
	b2UpdateSubTreeTask* currTask = tasks;

	int32* removalBaseLeaves = (int32*)allocator.Allocate(m_subTreeCount * sizeof(int32));
	uint32 removalBaseLeavesCount = 0;

	// Run tasks to process sub-tree events.
	moves.wait();
	removes.wait();
	DeferredInsert* insertBegin = inserts.begin();
	DeferredMove* moveBegin = moves.begin();
	DeferredRemove* removeBegin = removes.begin();
	while (true)
	{
		int32 minBaseLeaf = b2_maxInt;

		if (insertBegin != inserts.end())
		{
			minBaseLeaf = b2Min(insertBegin->baseLeaf, minBaseLeaf);
		}

		if (moveBegin != moves.end())
		{
			minBaseLeaf = b2Min(moveBegin->baseLeaf, minBaseLeaf);
		}

		if (removeBegin != removes.end())
		{
			minBaseLeaf = b2Min(removeBegin->baseLeaf, minBaseLeaf);
		}

		if (minBaseLeaf == b2_maxInt)
		{
			break;
		}

		uint32 insertCount = 0;
		if (insertBegin != inserts.end() && insertBegin->baseLeaf == minBaseLeaf)
		{
			DeferredInsert val;
			val.baseLeaf = minBaseLeaf;
			val.proxy = b2_maxInt;
			DeferredInsert* insertEnd = inserts.end();
			insertEnd = std::upper_bound(insertBegin, insertEnd, val, b2DeferredInsertLessThan);
			insertCount = insertEnd - insertBegin;
		}

		uint32 moveCount = 0;
		if (moveBegin != moves.end() && moveBegin->baseLeaf == minBaseLeaf)
		{
			DeferredMove val;
			val.baseLeaf = minBaseLeaf;
			val.subProxy = b2_maxInt;
			DeferredMove* moveEnd = moves.end();
			moveEnd = std::upper_bound(moveBegin, moveEnd, val, b2DeferredMoveLessThan);
			moveCount = moveEnd - moveBegin;
		}

		uint32 removeCount = 0;
		if (removeBegin != removes.end() && removeBegin->baseLeaf == minBaseLeaf)
		{
			DeferredRemove val;
			val.baseLeaf = minBaseLeaf;
			val.subProxy = b2_maxInt;
			DeferredRemove* removeEnd = removes.end();
			removeEnd = std::upper_bound(removeBegin, removeEnd, val, b2DeferredRemoveLessThan);
			removeCount = removeEnd - removeBegin;
			removalBaseLeaves[removalBaseLeavesCount++] = minBaseLeaf;
			b2Assert(removalBaseLeavesCount <= m_subTreeCount);
		}

		// TODO_MT: Cost threshold.
		new(currTask) b2UpdateSubTreeTask(this, insertBegin, insertCount,
			moveBegin, moveCount, removeBegin, removeCount);

		// TODO_MT: Why does this seem to make Solve slower?
		//uint32 cost = insertCount + 2 * moveCount + removeCount;
		//currTask->SetCost(cost);

		b2SubmitTask(executor, taskGroup, currTask);

		++currTask;
		insertBegin += insertCount;
		moveBegin += moveCount;
		removeBegin += removeCount;
	}

	executor.Wait(taskGroup, b2MainThreadCtx(&allocator));

	// Free nodes no longer in use.
	for (uint32 i = 0; i < removalBaseLeavesCount; ++i)
	{
		int32 baseLeaf = removalBaseLeaves[i];

		int32 subTreeFree = m_nodes[baseLeaf].subTreeNextFree;
		m_nodes[baseLeaf].subTreeNextFree = b2_nullNode;
		while (subTreeFree != b2_nullNode)
		{
			int32 nextFree = m_nodes[subTreeFree].subTreeNextFree;
			m_nodes[subTreeFree].subTreeNextFree = b2_nullNode;
			DebugNodeSubTreeAllocate(subTreeFree); // TEMP_MT remove from free list tracker
			FreeNode(subTreeFree);
			subTreeFree = nextFree;
		}

		if (m_nodes[baseLeaf].subTreeRoot == b2_nullNode)
		{
            SubTreeRemoveLeaf<false>(&m_root, baseLeaf);
		}
	}

	// Create sub-trees and insert proxies.
	insertNewSubTrees.wait();
	for (DeferredInsertNewSubTree* it = insertNewSubTrees.begin(); it != insertNewSubTrees.end(); ++it)
	{
		int32 subProxy = AllocateNode();
		m_nodes[subProxy].aabb = m_nodes[it->proxy].aabb;
		m_nodes[subProxy].userData = m_nodes[it->proxy].userData;
		m_nodes[subProxy].proxy = it->proxy;

		m_nodes[subProxy].nextProxy = m_nodes[it->proxy].nextProxy;
		m_nodes[it->proxy].nextProxy = subProxy;

		InsertNewSubTree(it->subTreePosition, subProxy);
	}

	// Replace removed user proxies with valid sub-proxies.
	replaces.wait();
	for (DeferredReplace* it = replaces.begin(); it != replaces.end(); ++it)
	{
		int32 dstProxy = it->proxy;
		int32 srcProxy = m_nodes[dstProxy].nextProxy;
		b2Assert(srcProxy != b2_nullNode);

		DebugReplace(srcProxy);
		DebugReplace(dstProxy);

		int32 parent = m_nodes[srcProxy].parent;
		if (parent != b2_nullNode)
		{
			// Update the parent's reference.
			if (m_nodes[parent].child1 == srcProxy)
			{
				m_nodes[parent].child1 = dstProxy;
			}
			else
			{
				b2Assert(m_nodes[parent].child2 == srcProxy);
				m_nodes[parent].child2 = dstProxy;
			}
		}
		else
		{
			int32 baseLeaf = m_nodes[srcProxy].baseTreeLeaf;

			// The source must be the root since it has no parent.
			b2Assert(m_nodes[baseLeaf].subTreeRoot == srcProxy);

			m_nodes[baseLeaf].subTreeRoot = dstProxy;
		}

		m_nodes[dstProxy] = m_nodes[srcProxy];
		DebugRemove(srcProxy);
		DebugInsert(dstProxy);
		FreeNode(srcProxy);
	}

	allocator.Free(removalBaseLeaves);
	allocator.Free(tasks);

#ifdef b2_validateTree
	Validate();
#endif
	DebugFinishMoveProxies();
}

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

	m_subTreeCount = 0;

	m_path = 0;
}

b2DynamicTreeOfTrees::~b2DynamicTreeOfTrees()
{
	// This frees the entire tree in one shot.
	b2Free(m_nodes);

	b2DebugReset();
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
	m_nodes[nodeId].subTreeNextFree = b2_nullNode;
	DebugNodeAllocate(nodeId);
	++m_nodeCount;
	return nodeId;
}

// Return a node to the pool.
void b2DynamicTreeOfTrees::FreeNode(int32 nodeId)
{
	b2Assert(0 <= nodeId && nodeId < m_nodeCapacity);
	b2Assert(0 < m_nodeCount);
	DebugNodeFree(nodeId);
	m_nodes[nodeId].next = m_freeList;
    m_nodes[nodeId].child1 = 0;
	m_nodes[nodeId].height = -1;
	m_freeList = nodeId;
	--m_nodeCount;
}

int32 b2DynamicTreeOfTrees::SubTreeAllocateNode(int32 proxy, int32 baseLeaf)
{
	int32 nodeId = m_nodes[baseLeaf].subTreeNextFree;
	DebugNodeSubTreeAllocate(nodeId);
	b2Assert(nodeId != b2_nullNode);
	m_nodes[baseLeaf].subTreeNextFree = m_nodes[nodeId].subTreeNextFree;
	m_nodes[nodeId].parent = b2_nullNode;
	m_nodes[nodeId].child1 = b2_nullNode;
	m_nodes[nodeId].child2 = b2_nullNode;
	m_nodes[nodeId].height = 0;
	m_nodes[nodeId].userData = nullptr;
	m_nodes[nodeId].proxy = proxy;
	m_nodes[nodeId].subTreeNextFree = b2_nullNode;
	return nodeId;
}

void b2DynamicTreeOfTrees::SubTreeFreeNode(int32 nodeId, int32 baseLeaf)
{
	b2Assert(0 <= nodeId && nodeId < m_nodeCapacity);
	b2Assert(0 < m_nodeCount);
	b2Assert(m_nodes[baseLeaf].subTreeNextFree != nodeId);
	DebugNodeSubTreeFree(nodeId);
	m_nodes[nodeId].subTreeNextFree = m_nodes[baseLeaf].subTreeNextFree;
	m_nodes[baseLeaf].subTreeNextFree = nodeId;
}

void b2DynamicTreeOfTrees::InsertNewSubTree(const SubTreePosition& subTreePosition, int32 subProxy)
{
	b2Assert(m_nodes[subProxy].IsLeaf());

	b2Vec2 subTreeCenter;
	subTreeCenter.x = (float32)subTreePosition.x * m_subTreeWidth;
	subTreeCenter.y = (float32)subTreePosition.y * m_subTreeHeight;

	b2FindFirstLeafQueryCallback findQuery(this);
	Query(&findQuery, subTreeCenter);

	int32 baseTreeLeaf = findQuery.m_leaf;

	if (baseTreeLeaf == b2_nullNode)
	{
		// Allocate and insert the sub-tree's base tree leaf.
		baseTreeLeaf = AllocateNode();
		m_nodes[baseTreeLeaf].aabb.lowerBound = subTreeCenter - b2Vec2(m_subTreeWidth * 0.5f, m_subTreeHeight * 0.5f);
		m_nodes[baseTreeLeaf].aabb.upperBound = subTreeCenter + b2Vec2(m_subTreeWidth * 0.5f, m_subTreeHeight * 0.5f);
		m_nodes[baseTreeLeaf].subTreePosition = subTreePosition;
		SubTreeInsertLeaf<false>(&m_root, baseTreeLeaf);

		// Insert the sub-proxy into the new sub-tree.
		m_nodes[baseTreeLeaf].subTreeRoot = subProxy;
		m_nodes[subProxy].baseTreeLeaf = baseTreeLeaf;
		m_nodes[subProxy].parent = b2_nullNode;
		DebugInsert(subProxy);

		++m_subTreeCount;
	}
	else
	{
		// Insert the sub-proxy into the existing sub-tree.
		m_nodes[subProxy].baseTreeLeaf = baseTreeLeaf;
		SubTreeInsertLeaf<false>(&m_nodes[baseTreeLeaf].subTreeRoot, subProxy);
	}
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

	m_nodes[proxyId].aabb = ComputeAABB(aabb, displacement);

	InsertLeaf(proxyId);

	return true;
}

inline b2AABB b2DynamicTreeOfTrees::ComputeAABB(const b2AABB& aabb, const b2Vec2& displacement)
{
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

	return b;
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
        m_tree->SubTreeInsertLeaf<false>(&subTreeRoot, m_currProxy);
        m_tree->m_nodes[baseTreeLeaf].subTreeRoot = subTreeRoot;

        return true;
    }

    b2DynamicTreeOfTrees* m_tree;
    b2AABB m_aabb;
    int32 m_proxy;
    int32 m_currProxy;
    uint32 m_insertCount;
};

b2_forceInline void b2DynamicTreeOfTrees::InsertLeaf(int32 proxy)
{
    b2AABB proxyAABB = m_nodes[proxy].aabb;

    // Insert the leaf into all existing overlapped sub-trees.
    b2InsertLeafQueryCallback insertQuery(this, proxy);

    Query<false, false>(m_root, &insertQuery, proxyAABB, 0);

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

        // Rule out gaps in the query AABB by checking if it has the expected number of insertions.

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

	uint32 insertCount = insertQuery.m_insertCount;

    for (int32 y = beginY; y < endY; ++y)
    {
        for (int32 x = beginX; x < endX; ++x)
        {
            // Look for the sub-tree.
            b2Vec2 subTreeCenter(x * m_subTreeWidth, y * m_subTreeHeight);
            b2FindFirstLeafQueryCallback findQuery(this);
            Query(&findQuery, subTreeCenter);

			int32 subProxy = proxy;

            // Does the sub-tree need to be created?
            if (findQuery.m_leaf == -1)
            {
				if (insertCount++ > 0)
				{
					subProxy = AllocateNode();
					m_nodes[subProxy].aabb = m_nodes[proxy].aabb;
					m_nodes[subProxy].userData = m_nodes[proxy].userData;
					m_nodes[subProxy].proxy = proxy;

					// Insert into the proxy list.
					m_nodes[subProxy].nextProxy = m_nodes[proxy].nextProxy;
					m_nodes[proxy].nextProxy = subProxy;
				}

				SubTreePosition subTreePosition;
				subTreePosition.x = x;
				subTreePosition.y = y;

				InsertNewSubTree(subTreePosition, subProxy);
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
        SubTreeRemoveLeaf<false>(&m_nodes[baseTreeLeaf].subTreeRoot, currentProxy);
        if (m_nodes[baseTreeLeaf].subTreeRoot == b2_nullNode)
        {
            SubTreeRemoveLeaf<false>(&m_root, baseTreeLeaf);
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

template<bool useSubTreeFreeList>
b2_forceInline void b2DynamicTreeOfTrees::SubTreeInsertLeaf(int32* rootInOut, int32 leaf)
{
	DebugInsert(leaf);
	b2Assert(m_nodes[leaf].IsLeaf());

	m_nodes[leaf].parent = b2_nullNode;

	int32 root = *rootInOut;

	if (root == b2_nullNode)
	{
		*rootInOut = leaf;
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
	int32 newParent;
	if (useSubTreeFreeList)
	{
		newParent = SubTreeAllocateNode(m_nodes[leaf].proxy, m_nodes[leaf].baseTreeLeaf);
	}
	else
	{
		newParent = AllocateNode();
	}
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
		index = Balance(&root, index);

		int32 child1 = m_nodes[index].child1;
		int32 child2 = m_nodes[index].child2;

		b2Assert(child1 != b2_nullNode);
		b2Assert(child2 != b2_nullNode);

		m_nodes[index].height = 1 + b2Max(m_nodes[child1].GetHeight(), m_nodes[child2].GetHeight());
		m_nodes[index].aabb.Combine(m_nodes[child1].aabb, m_nodes[child2].aabb);

		index = m_nodes[index].parent;
	}

	*rootInOut = root;

#ifdef b2_validateTree
	ValidateStructure<true>(root);
	ValidateMetrics<true>(root);
#endif
}

template<bool useSubTreeFreeList>
void b2DynamicTreeOfTrees::SubTreeRemoveLeaf(int32* rootInOut, int32 leaf)
{
	DebugRemove(leaf);

	int32 root = *rootInOut;

	if (leaf == root)
	{
		*rootInOut = b2_nullNode;
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
		if (useSubTreeFreeList)
		{
			SubTreeFreeNode(parent, m_nodes[leaf].baseTreeLeaf);
		}
		else
		{
			FreeNode(parent);
		}

		// Adjust ancestor bounds.
		int32 index = grandParent;
		while (index != b2_nullNode)
		{
            b2Assert(m_nodes[index].IsLeaf() == false);
			index = Balance(&root, index);
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
		if (useSubTreeFreeList)
		{
			SubTreeFreeNode(parent, m_nodes[leaf].baseTreeLeaf);
		}
		else
		{
			FreeNode(parent);
		}
	}

	*rootInOut = root;

#ifdef b2_validateTree
	ValidateStructure<true>(root);
	ValidateMetrics<true>(root);
#endif
}

// Perform a left or right rotation if node A is imbalanced.
// Returns the new root index.
int32 b2DynamicTreeOfTrees::Balance(int32* rootInOut, int32 iA)
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
			*rootInOut = iC;
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
			*rootInOut = iB;
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
