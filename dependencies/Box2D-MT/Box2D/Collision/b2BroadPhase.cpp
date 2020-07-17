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

#include "Box2D/Collision/b2BroadPhase.h"

b2BroadPhase::b2BroadPhase()
{
	m_proxyCount = 0;

	for (int32 i = 0; i < b2_maxThreads; ++i)
	{
		m_perThreadData[i].m_queryProxyId = -1;
	}
}

b2BroadPhase::~b2BroadPhase()
{

}

#ifdef b2_dynamicTreeOfTrees
void b2BroadPhase::Reset(float32 subTreeWidth, float32 subTreeHeight)
{
	m_tree.Reset(subTreeWidth, subTreeHeight);
	m_proxyCount = 0;
	b2Assert(m_moveBuffer.size() == 0);
}
#endif

int32 b2BroadPhase::CreateProxy(const b2AABB& aabb, void* userData)
{
	int32 proxyId = m_tree.CreateProxy(aabb, userData);
	++m_proxyCount;
	BufferMove(proxyId);
	return proxyId;
}

void b2BroadPhase::DestroyProxy(int32 proxyId)
{
	UnBufferMove(proxyId);
	--m_proxyCount;
	m_tree.DestroyProxy(proxyId);
}

void b2BroadPhase::MoveProxy(int32 proxyId, const b2AABB& aabb, const b2Vec2& displacement)
{
	bool buffer = m_tree.MoveProxy(proxyId, aabb, displacement);
	if (buffer)
	{
		BufferMove(proxyId);
	}
}

void b2BroadPhase::TouchProxy(int32 proxyId)
{
	BufferMove(proxyId);
}

void b2BroadPhase::BufferMove(int32 proxyId)
{
	m_moveBuffer.push_back(proxyId);
}

void b2BroadPhase::UnBufferMove(int32 proxyId)
{
	for (uint32 i = 0; i < m_moveBuffer.size(); ++i)
	{
		if (m_moveBuffer[i] == proxyId)
		{
			m_moveBuffer[i] = e_nullProxy;
		}
	}
}

// This is called from b2DynamicTree::Query when we are gathering pairs.
bool b2BroadPhasePerThreadData::QueryCallback(int32 proxyId)
{
	// A proxy cannot form a pair with itself.
	if (proxyId == m_queryProxyId)
	{
		return true;
	}

	b2Pair pair;
	pair.proxyIdA = b2Min(proxyId, m_queryProxyId);
	pair.proxyIdB = b2Max(proxyId, m_queryProxyId);

	m_pairBuffer.push_back(pair);

	return true;
}
