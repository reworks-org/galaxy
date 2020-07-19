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

#include "Box2D/Dynamics/Contacts/b2Contact.h"
#include "Box2D/Dynamics/Contacts/b2CircleContact.h"
#include "Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.h"
#include "Box2D/Dynamics/Contacts/b2PolygonContact.h"
#include "Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.h"
#include "Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.h"
#include "Box2D/Dynamics/Contacts/b2ChainAndCircleContact.h"
#include "Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.h"
#include "Box2D/Dynamics/Contacts/b2ContactSolver.h"

#include "Box2D/Collision/b2Collision.h"
#include "Box2D/Collision/b2TimeOfImpact.h"
#include "Box2D/Collision/Shapes/b2Shape.h"
#include "Box2D/Common/b2BlockAllocator.h"
#include "Box2D/Dynamics/b2Body.h"
#include "Box2D/Dynamics/b2ContactManager.h"
#include "Box2D/Dynamics/b2Fixture.h"
#include "Box2D/Dynamics/b2World.h"

b2ContactRegister b2Contact::s_registers[b2Shape::e_typeCount][b2Shape::e_typeCount];
bool b2Contact::s_initialized = false;

void b2Contact::InitializeRegisters()
{
	AddType(b2CircleContact::Create, b2CircleContact::Destroy, b2Shape::e_circle, b2Shape::e_circle);
	AddType(b2PolygonAndCircleContact::Create, b2PolygonAndCircleContact::Destroy, b2Shape::e_polygon, b2Shape::e_circle);
	AddType(b2PolygonContact::Create, b2PolygonContact::Destroy, b2Shape::e_polygon, b2Shape::e_polygon);
	AddType(b2EdgeAndCircleContact::Create, b2EdgeAndCircleContact::Destroy, b2Shape::e_edge, b2Shape::e_circle);
	AddType(b2EdgeAndPolygonContact::Create, b2EdgeAndPolygonContact::Destroy, b2Shape::e_edge, b2Shape::e_polygon);
	AddType(b2ChainAndCircleContact::Create, b2ChainAndCircleContact::Destroy, b2Shape::e_chain, b2Shape::e_circle);
	AddType(b2ChainAndPolygonContact::Create, b2ChainAndPolygonContact::Destroy, b2Shape::e_chain, b2Shape::e_polygon);
	s_initialized = true;
}

void b2Contact::AddType(b2ContactCreateFcn* createFcn, b2ContactDestroyFcn* destoryFcn,
						b2Shape::Type type1, b2Shape::Type type2)
{
	b2Assert(0 <= type1 && type1 < b2Shape::e_typeCount);
	b2Assert(0 <= type2 && type2 < b2Shape::e_typeCount);

	s_registers[type1][type2].createFcn = createFcn;
	s_registers[type1][type2].destroyFcn = destoryFcn;
	s_registers[type1][type2].primary = true;

	if (type1 != type2)
	{
		s_registers[type2][type1].createFcn = createFcn;
		s_registers[type2][type1].destroyFcn = destoryFcn;
		s_registers[type2][type1].primary = false;
	}
}

b2Contact* b2Contact::Create(b2Fixture* fixtureA, int32 indexA, b2Fixture* fixtureB, int32 indexB, b2BlockAllocator* allocator)
{
	b2Assert(s_initialized == true);

	b2Shape::Type type1 = fixtureA->GetType();
	b2Shape::Type type2 = fixtureB->GetType();

	b2Assert(0 <= type1 && type1 < b2Shape::e_typeCount);
	b2Assert(0 <= type2 && type2 < b2Shape::e_typeCount);

	b2ContactCreateFcn* createFcn = s_registers[type1][type2].createFcn;
	if (createFcn)
	{
		if (s_registers[type1][type2].primary)
		{
			return createFcn(fixtureA, indexA, fixtureB, indexB, allocator);
		}
		else
		{
			return createFcn(fixtureB, indexB, fixtureA, indexA, allocator);
		}
	}
	else
	{
		return nullptr;
	}
}

void b2Contact::Destroy(b2Contact* contact, b2BlockAllocator* allocator)
{
	b2Assert(s_initialized == true);

	b2Fixture* fixtureA = contact->m_fixtureA;
	b2Fixture* fixtureB = contact->m_fixtureB;

	if (contact->m_manifold.pointCount > 0 &&
		fixtureA->IsSensor() == false &&
		fixtureB->IsSensor() == false)
	{
		fixtureA->GetBody()->SetAwake(true);
		fixtureB->GetBody()->SetAwake(true);
	}

	b2Shape::Type typeA = fixtureA->GetType();
	b2Shape::Type typeB = fixtureB->GetType();

	b2Assert(0 <= typeA && typeB < b2Shape::e_typeCount);
	b2Assert(0 <= typeA && typeB < b2Shape::e_typeCount);

	b2ContactDestroyFcn* destroyFcn = s_registers[typeA][typeB].destroyFcn;
	destroyFcn(contact, allocator);
}

b2Contact::b2Contact(b2Fixture* fA, int32 indexA, b2Fixture* fB, int32 indexB)
{
	m_flags = e_enabledFlag;

	m_managerIndex = -1;

	m_fixtureA = fA;
	m_fixtureB = fB;

	m_indexA = indexA;
	m_indexB = indexB;

	m_manifold.pointCount = 0;

	m_prev = nullptr;
	m_next = nullptr;

	m_nodeA.contact = nullptr;
	m_nodeA.prev = nullptr;
	m_nodeA.next = nullptr;
	m_nodeA.other = nullptr;

	m_nodeB.contact = nullptr;
	m_nodeB.prev = nullptr;
	m_nodeB.next = nullptr;
	m_nodeB.other = nullptr;

	m_toiCount = 0;
	m_toi = 1.0f;

	m_friction = b2MixFriction(m_fixtureA->m_friction, m_fixtureB->m_friction);
	m_restitution = b2MixRestitution(m_fixtureA->m_restitution, m_fixtureB->m_restitution);

	m_tangentSpeed = 0.0f;
}

void b2Contact::Update(b2ContactListener* listener)
{
	UpdateImpl<true>(nullptr, listener, 0);
}

void b2Contact::Update(b2ContactManagerPerThreadData& td, b2ContactListener* listener, uint32 threadId)
{
	UpdateImpl<false>(&td, listener, threadId);
}

// Update the contact manifold and touching status.
// Note: do not assume the fixture AABBs are overlapping or are valid.
template<bool isSingleThread>
void b2Contact::UpdateImpl(b2ContactManagerPerThreadData* td, b2ContactListener* listener, uint32 threadId)
{
	b2Manifold oldManifold = m_manifold;

	// Re-enable this contact.
	m_flags |= e_enabledFlag;

	bool touching = false;
	bool wasTouching = (m_flags & e_touchingFlag) == e_touchingFlag;

	bool sensorA = m_fixtureA->IsSensor();
	bool sensorB = m_fixtureB->IsSensor();
	bool sensor = sensorA || sensorB;

	b2Body* bodyA = m_fixtureA->GetBody();
	b2Body* bodyB = m_fixtureB->GetBody();
	const b2Transform& xfA = bodyA->GetTransform();
	const b2Transform& xfB = bodyB->GetTransform();

	// Is this contact a sensor?
	if (sensor)
	{
		const b2Shape* shapeA = m_fixtureA->GetShape();
		const b2Shape* shapeB = m_fixtureB->GetShape();
		touching = b2TestOverlap(shapeA, m_indexA, shapeB, m_indexB, xfA, xfB);

		// Sensors don't generate manifolds.
		m_manifold.pointCount = 0;
	}
	else
	{
		Evaluate(&m_manifold, xfA, xfB);
		touching = m_manifold.pointCount > 0;

		// Match old contact ids to new contact ids and copy the
		// stored impulses to warm start the solver.
		for (int32 i = 0; i < m_manifold.pointCount; ++i)
		{
			b2ManifoldPoint* mp2 = m_manifold.points + i;
			mp2->normalImpulse = 0.0f;
			mp2->tangentImpulse = 0.0f;
			b2ContactID id2 = mp2->id;

			for (int32 j = 0; j < oldManifold.pointCount; ++j)
			{
				b2ManifoldPoint* mp1 = oldManifold.points + j;

				if (mp1->id.key == id2.key)
				{
					mp2->normalImpulse = mp1->normalImpulse;
					mp2->tangentImpulse = mp1->tangentImpulse;
					break;
				}
			}
		}

		if (touching != wasTouching)
		{
			if (isSingleThread)
			{
				bodyA->SetAwake(true);
				bodyB->SetAwake(true);
			}
			else
			{
				td->m_awakes.push_back(this);
			}
		}
	}

	if (touching)
	{
		m_flags |= e_touchingFlag;
	}
	else
	{
		m_flags &= ~e_touchingFlag;
	}

	if (wasTouching == false && touching == true && listener)
	{
		if (listener->BeginContactImmediate(this, threadId))
		{
			if (isSingleThread)
			{
				listener->BeginContact(this);
			}
			else
			{
				td->m_beginContacts.push_back(this);
			}
		}
	}

	if (wasTouching == true && touching == false && listener)
	{
		if (listener->EndContactImmediate(this, threadId))
		{
			if (isSingleThread)
			{
				listener->EndContact(this);
			}
			else
			{
				td->m_endContacts.push_back(this);
			}
		}
	}

	if (sensor == false && touching && listener)
	{
		if (listener->PreSolveImmediate(this, &oldManifold, threadId))
		{
			if (isSingleThread)
			{
				listener->PreSolve(this, &oldManifold);
			}
			else
			{
				b2DeferredPreSolve presolve = { this, oldManifold };
				td->m_preSolves.push_back(presolve);
			}
		}
	}
}

bool b2Contact::IsToiCandidate(b2Fixture* fA, b2Fixture* fB)
{
	if (fA->IsSensor() == false && fB->IsSensor() == false)
	{
		b2Body* bA = fA->GetBody();
		b2Body* bB = fB->GetBody();

		if (bA->IsBullet() || bB->IsBullet())
		{
			return true;
		}
		else
		{
			bool includesNonDynamic = bA->GetType() != b2_dynamicBody || bB->GetType() != b2_dynamicBody;
			bool neitherIsThickShape = fA->IsThickShape() == false && fB->IsThickShape() == false;

			if (includesNonDynamic && neitherIsThickShape)
			{
				return true;
			}
		}
	}

	return false;
}

bool b2Contact::ToiLessThan(float32 alpha0, const b2Contact* contact0, float32 alpha1, const b2Contact* contact1)
{
	if (alpha0 == alpha1)
	{
		return b2ContactPointerLessThan(contact0, contact1);
	}

	return alpha0 < alpha1;
}
