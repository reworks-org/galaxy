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

#ifndef B2_ISLAND_H
#define B2_ISLAND_H

#include "Box2D/Common/b2GrowableArray.h"
#include "Box2D/Common/b2Math.h"
#include "Box2D/Dynamics/b2Body.h"
#include "Box2D/Dynamics/b2TimeStep.h"

class b2Contact;
class b2Joint;
class b2StackAllocator;
class b2ContactListener;
struct b2ContactVelocityConstraint;
struct b2Profile;
struct b2DeferredPostSolve;

/// This is an internal class.
class b2Island
{
public:
    b2Island();
	b2Island(b2Body** bodies, b2Contact** contacts,
		b2Velocity* velocities, b2Position* positions);
	b2Island(int32 bodyCount, int32 contactCount, int32 jointCount,
		b2Body** bodies, b2Contact** contacts, b2Joint** joints,
		b2Velocity* velocities, b2Position* positions);

	void Clear()
	{
		m_bodyCount = 0;
		m_contactCount = 0;
		m_jointCount = 0;
	}

	void Solve(b2Profile* profile, const b2TimeStep& step, const b2Vec2& gravity, b2StackAllocator* allocator,
		b2ContactListener* listener, uint32 threadId, bool allowSleep, b2GrowableArray<b2DeferredPostSolve>& postSolves);

	void SolveTOI(const b2TimeStep& subStep, int32 toiIndexA, int32 toiIndexB, b2StackAllocator* allocator,
		b2ContactListener* listener);

	void Add(b2Body* body)
	{
		body->SetIslandIndex(m_bodyCount, 0);
		m_bodies[m_bodyCount] = body;
		++m_bodyCount;
	}

	void Add(b2Contact* contact)
	{
		m_contacts[m_contactCount++] = contact;
	}

	void Add(b2Joint* joint)
	{
		m_joints[m_jointCount++] = joint;
	}

	template<bool isSingleThread>
	void Report(const b2ContactVelocityConstraint* constraints, b2ContactListener* listener, uint32 threadId,
		b2GrowableArray<b2DeferredPostSolve>* postSolves);

	b2Body** m_bodies;
	b2Contact** m_contacts;
	b2Joint** m_joints;

	b2Position* m_positions;
	b2Velocity* m_velocities;

	int32 m_bodyCount;
	int32 m_jointCount;
	int32 m_contactCount;
};

#endif
