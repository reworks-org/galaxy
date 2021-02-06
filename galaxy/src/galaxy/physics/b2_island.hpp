// MIT License

// Copyright (c) 2019 Erin Catto

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef B2_ISLAND_H
#define B2_ISLAND_H

#include "galaxy/physics/b2_body.hpp"
#include "galaxy/physics/b2_math.hpp"
#include "galaxy/physics/b2_time_step.hpp"

class b2Contact;
class b2Joint;
class b2StackAllocator;
class b2ContactListener;
struct b2ContactVelocityConstraint;
struct b2Profile;

/// This is an internal class.
class b2Island
{
public:
	b2Island(std::int32_t bodyCapacity, std::int32_t contactCapacity, std::int32_t jointCapacity,
			b2StackAllocator* allocator, b2ContactListener* listener);
	~b2Island();

	void Clear()
	{
		m_bodyCount = 0;
		m_contactCount = 0;
		m_jointCount = 0;
	}

	void Solve(b2Profile* profile, const b2TimeStep& step, const b2Vec2& gravity, bool allowSleep);

	void SolveTOI(const b2TimeStep& subStep, std::int32_t toiIndexA, std::int32_t toiIndexB);

	void Add(b2Body* body)
	{
		b2Assert(m_bodyCount < m_bodyCapacity);
		body->m_islandIndex = m_bodyCount;
		m_bodies[m_bodyCount] = body;
		++m_bodyCount;
	}

	void Add(b2Contact* contact)
	{
		b2Assert(m_contactCount < m_contactCapacity);
		m_contacts[m_contactCount++] = contact;
	}

	void Add(b2Joint* joint)
	{
		b2Assert(m_jointCount < m_jointCapacity);
		m_joints[m_jointCount++] = joint;
	}

	void Report(const b2ContactVelocityConstraint* constraints);

	b2StackAllocator* m_allocator;
	b2ContactListener* m_listener;

	b2Body** m_bodies;
	b2Contact** m_contacts;
	b2Joint** m_joints;

	b2Position* m_positions;
	b2Velocity* m_velocities;

	std::int32_t m_bodyCount;
	std::int32_t m_jointCount;
	std::int32_t m_contactCount;

	std::int32_t m_bodyCapacity;
	std::int32_t m_contactCapacity;
	std::int32_t m_jointCapacity;
};

#endif
