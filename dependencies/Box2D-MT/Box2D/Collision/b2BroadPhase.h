/*
* Copyright (c) 2006-2009 Erin Catto http://www.box2d.org
* Copyright (c) 2015 Justin Hoffman https://github.com/jhoffman0x/Box2D-MT
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

#ifndef B2_BROAD_PHASE_H
#define B2_BROAD_PHASE_H

#include "Box2D/Collision/b2Collision.h"
#ifdef b2_dynamicTreeOfTrees
#include "Box2D/MT/b2DynamicTreeOfTrees.h"
#else
#include "Box2D/Collision/b2DynamicTree.h"
#endif
#include "Box2D/Common/b2GrowableArray.h"
#include "Box2D/MT/b2ThreadDataSorter.h"
#include "Box2D/MT/b2MtUtil.h"
#include <algorithm>

struct b2ExecutorContext;

struct b2Pair
{
	int32 proxyIdA;
	int32 proxyIdB;
};

struct b2BroadPhasePerThreadData
{
	bool QueryCallback(int32 proxyId);

	b2GrowableArray<int32> m_moveBuffer;
	b2GrowableArray<b2Pair> m_pairBuffer;
	int32 m_queryProxyId;

	uint8 m_padding[b2_cacheLineSize];
};

/// The broad-phase is used for computing pairs and performing volume queries and ray casts.
/// This broad-phase does not persist pairs. Instead, this reports potentially new pairs.
/// It is up to the client to consume the new pairs and to track subsequent overlap.
class b2BroadPhase
{
public:

	enum
	{
		e_nullProxy = -1
	};

	b2BroadPhase();
	~b2BroadPhase();

#ifdef b2_dynamicTreeOfTrees
	/// Destroy all proxies and set the sub-tree dimensions.
	void Reset(float32 subTreeWidth, float32 subTreeHeight);

	/// Visit every leaf in the base tree.
	template <typename T>
	void VisitBaseTree(T* callback) const;

	// TODO_MT
	void QueueMoveProxy(int32 proxyId, const b2AABB& aabb, const b2Vec2& displacement, uint32 threadId);

	// TODO_MT
	void FinishMoveProxies(b2TaskExecutor& executor, b2TaskGroup* taskGroup, b2StackAllocator& allocator);
#endif

	/// Create a proxy with an initial AABB. Pairs are not reported until
	/// UpdatePairs is called.
	int32 CreateProxy(const b2AABB& aabb, void* userData);

	/// Destroy a proxy. It is up to the client to remove any pairs.
	void DestroyProxy(int32 proxyId);

	/// Call MoveProxy as many times as you like, then when you are done
	/// call UpdatePairs to finalized the proxy pairs (for your time step).
	void MoveProxy(int32 proxyId, const b2AABB& aabb, const b2Vec2& displacement);

	/// Call to trigger a re-processing of it's pairs on the next call to UpdatePairs.
	void TouchProxy(int32 proxyId);

	/// Get the fat AABB for a proxy.
	const b2AABB& GetFatAABB(int32 proxyId) const;

	/// Get user data from a proxy. Returns nullptr if the id is invalid.
	void* GetUserData(int32 proxyId) const;

	/// Test overlap of fat AABBs.
	bool TestOverlap(int32 proxyIdA, int32 proxyIdB) const;

	/// Get the number of proxies.
	int32 GetProxyCount() const;

	/// Update the pairs. This results in pair callbacks. This can only add pairs.
	template <typename T>
	void UpdatePairsSingleThread(T* callback);

	/// Update the pairs. This results in pair callbacks. This can only add pairs.
	/// @warning callback can be called from multiple threads
	template <typename T>
	void UpdatePairs(b2TaskExecutor& executor, b2TaskGroup* taskGroup, b2StackAllocator& allocator, T* callback);

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

	/// Get the height of the embedded tree.
	int32 GetTreeHeight() const;

	/// Get the balance of the embedded tree.
	int32 GetTreeBalance() const;

	/// Get the quality metric of the embedded tree.
	float32 GetTreeQuality() const;

	/// Shift the world origin. Useful for large worlds.
	/// The shift formula is: position -= newOrigin
	/// @param newOrigin the new origin with respect to the old origin
	void ShiftOrigin(const b2Vec2& newOrigin);

	/// Get the number of proxies in the move buffer.
	int32 GetMoveCount() const;

private:

	friend class b2DynamicTree;
	template<typename T>
	friend class b2BroadphaseFindContactsTask;

	template <typename T>
	void UpdatePairs(int32* moveBegin, int32* moveEnd, T* callback, uint32 threadId);

	void BufferMove(int32 proxyId, uint32 threadId);
	void UnBufferMove(int32 proxyId, uint32 threadId);

	b2BroadPhasePerThreadData m_perThreadData[b2_maxThreads];

#ifdef b2_dynamicTreeOfTrees
	b2DynamicTreeOfTrees m_tree;
#else
	b2DynamicTree m_tree;
#endif

	int32 m_proxyCount;
};

/// This is used to sort pairs.
inline bool b2PairLessThan(const b2Pair& pair1, const b2Pair& pair2)
{
	if (pair1.proxyIdA < pair2.proxyIdA)
	{
		return true;
	}

	if (pair1.proxyIdA == pair2.proxyIdA)
	{
		return pair1.proxyIdB < pair2.proxyIdB;
	}

	return false;
}

inline void* b2BroadPhase::GetUserData(int32 proxyId) const
{
	return m_tree.GetUserData(proxyId);
}

inline bool b2BroadPhase::TestOverlap(int32 proxyIdA, int32 proxyIdB) const
{
	const b2AABB& aabbA = m_tree.GetFatAABB(proxyIdA);
	const b2AABB& aabbB = m_tree.GetFatAABB(proxyIdB);
	return b2TestOverlap(aabbA, aabbB);
}

inline const b2AABB& b2BroadPhase::GetFatAABB(int32 proxyId) const
{
	return m_tree.GetFatAABB(proxyId);
}

inline int32 b2BroadPhase::GetProxyCount() const
{
	return m_proxyCount;
}

inline int32 b2BroadPhase::GetTreeHeight() const
{
	return m_tree.GetHeight();
}

inline int32 b2BroadPhase::GetTreeBalance() const
{
	return m_tree.GetMaxBalance();
}

inline float32 b2BroadPhase::GetTreeQuality() const
{
	return m_tree.GetAreaRatio();
}

template<typename T>
class b2BroadphaseFindContactsTask : public b2RangeTask
{
public:
	b2BroadphaseFindContactsTask() {}
	b2BroadphaseFindContactsTask(const b2RangeTaskRange& range,
		b2BroadPhase* broadPhase, int32* moveBuffer, T* callback)
	: b2RangeTask(range)
	, m_broadPhase(broadPhase)
	, m_moveBuffer(moveBuffer)
	, m_callback(callback)
	{}

	virtual b2Task::Type GetType() const override { return b2Task::e_broadPhaseFindContacts; }

	virtual void Execute(const b2ThreadContext& threadCtx, const b2RangeTaskRange& range) override
	{
		m_broadPhase->UpdatePairs(m_moveBuffer + range.begin, m_moveBuffer + range.end, m_callback, threadCtx.threadId);
	}

private:
	b2BroadPhase* m_broadPhase;
	int32* m_moveBuffer;
	T* m_callback;
};

template <typename T>
void b2BroadPhase::UpdatePairsSingleThread(T* callback)
{
#ifdef b2DEBUG
	for (uint32 i = 1; i < b2_maxThreads; ++i)
	{
		const b2BroadPhasePerThreadData& td = m_perThreadData[i];
		b2Assert(td.m_moveBuffer.size() == 0);
	}
#endif

	b2BroadPhasePerThreadData& td = m_perThreadData[0];

	UpdatePairs(td.m_moveBuffer.begin(), td.m_moveBuffer.end(), callback, 0);
	td.m_moveBuffer.clear();
}

template <typename T>
void b2BroadPhase::UpdatePairs(b2TaskExecutor& executor, b2TaskGroup* taskGroup, b2StackAllocator& allocator, T* callback)
{
#ifdef b2_dynamicTreeOfTrees
	m_tree.FinishMoveProxies(executor, taskGroup, allocator);
#endif

	// Sort per-thread moves.
	b2_threadDataSorter(moves, int32, 1, executor, allocator, m_perThreadData,
		&b2BroadPhasePerThreadData::m_moveBuffer, std::less<int32>());

	moves.wait();

	// Update pairs for the moved proxies.
	b2BroadphaseFindContactsTask<T> tasks[b2_maxRangeSubTasks];
	b2PartitionedRange ranges;
	executor.PartitionRange(b2Task::e_broadPhaseFindContacts, 0, moves.size(), ranges);
	for (uint32 i = 0; i < ranges.count; ++i)
	{
		tasks[i] = b2BroadphaseFindContactsTask<T>(ranges[i], this, moves.begin(), callback);
	}
	b2SubmitTasks(executor, taskGroup, tasks, ranges.count);
	executor.Wait(taskGroup, b2MainThreadCtx(&allocator));
}

template <typename T>
void b2BroadPhase::UpdatePairs(int32* moveBegin, int32* moveEnd, T* callback, uint32 threadId)
{
	b2BroadPhasePerThreadData& td = m_perThreadData[threadId];

	// Perform tree queries for all moving proxies.
	for (int32* moveIt = moveBegin; moveIt < moveEnd; ++moveIt)
	{
		td.m_queryProxyId = *moveIt;
		if (td.m_queryProxyId == e_nullProxy)
		{
			continue;
		}

		// We have to query the tree with the fat AABB so that
		// we don't fail to create a pair that may touch later.
		b2AABB fatAABB = m_tree.GetFatAABB(td.m_queryProxyId);

		// Query the tree, create pairs and add them pair buffer.
#ifdef b2_dynamicTreeOfTrees
		m_tree.Query(&td, fatAABB, threadId);
#else
		m_tree.Query(&td, fatAABB);
#endif
	}

	// Sort the pair buffer to expose duplicates.
	std::sort(td.m_pairBuffer.data(), td.m_pairBuffer.data() + td.m_pairBuffer.size(), b2PairLessThan);

	// Send the pairs back to the client.
	uint32 i = 0;
	while (i < td.m_pairBuffer.size())
	{
		b2Pair& primaryPair = td.m_pairBuffer[i];

		void* userDataA = m_tree.GetUserData(primaryPair.proxyIdA);
		void* userDataB = m_tree.GetUserData(primaryPair.proxyIdB);

		callback->AddPair(userDataA, userDataB, threadId);

		++i;

		// Skip any duplicate pairs.
		while (i < td.m_pairBuffer.size())
		{
			b2Pair& pair = td.m_pairBuffer[i];
			if (pair.proxyIdA != primaryPair.proxyIdA || pair.proxyIdB != primaryPair.proxyIdB)
			{
				break;
			}
			++i;
		}
	}

	td.m_pairBuffer.clear();

	// Try to keep the tree balanced.
	//m_tree.Rebalance(4);
}

template <typename T>
inline void b2BroadPhase::Query(T* callback, const b2AABB& aabb, uint32 threadId)
{
#ifdef b2_dynamicTreeOfTrees
	m_tree.Query(callback, aabb, threadId);
#else
	B2_NOT_USED(threadId);
	m_tree.Query(callback, aabb);
#endif
}

template <typename T>
inline void b2BroadPhase::RayCast(T* callback, const b2RayCastInput& input, uint32 threadId)
{
#ifdef b2_dynamicTreeOfTrees
	m_tree.RayCast(callback, input, threadId);
#else
	B2_NOT_USED(threadId);
	m_tree.RayCast(callback, input);
#endif
}

#ifdef b2_dynamicTreeOfTrees
template <typename T>
inline void b2BroadPhase::VisitBaseTree(T* callback) const
{
	m_tree.VisitBaseTree(callback);
}
#endif

inline void b2BroadPhase::ShiftOrigin(const b2Vec2& newOrigin)
{
	m_tree.ShiftOrigin(newOrigin);
}

inline int32 b2BroadPhase::GetMoveCount() const
{
	uint32 moveCount = 0;
	for (uint32 i = 0; i < b2_maxThreads; ++i)
	{
		const b2BroadPhasePerThreadData& td = m_perThreadData[i];
		moveCount += td.m_moveBuffer.size();
	}
	return moveCount;
}

#endif
