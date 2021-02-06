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

#include "galaxy/physics/b2_fixture.hpp"
#include "galaxy/physics/b2_block_allocator.hpp"
#include "galaxy/physics/b2_broad_phase.hpp"
#include "galaxy/physics/b2_chain_shape.hpp"
#include "galaxy/physics/b2_circle_shape.hpp"
#include "galaxy/physics/b2_collision.hpp"
#include "galaxy/physics/b2_contact.hpp"
#include "galaxy/physics/b2_edge_shape.hpp"
#include "galaxy/physics/b2_polygon_shape.hpp"
#include "galaxy/physics/b2_world.hpp"

b2Fixture::b2Fixture()
{
	m_body       = nullptr;
	m_next       = nullptr;
	m_proxies    = nullptr;
	m_proxyCount = 0;
	m_shape      = nullptr;
	m_density    = 0.0f;
}

void b2Fixture::Create(b2BlockAllocator* allocator, b2Body* body, const b2FixtureDef* def)
{
	m_userData             = def->userData;
	m_friction             = def->friction;
	m_restitution          = def->restitution;
	m_restitutionThreshold = def->restitutionThreshold;

	m_body = body;
	m_next = nullptr;

	m_filter = def->filter;

	m_isSensor = def->isSensor;

	m_shape = def->shape->Clone(allocator);

	// Reserve proxy space
	std::int32_t childCount = m_shape->GetChildCount();
	m_proxies               = (b2FixtureProxy*)allocator->Allocate(childCount * sizeof(b2FixtureProxy));
	for (std::int32_t i = 0; i < childCount; ++i)
	{
		m_proxies[i].fixture = nullptr;
		m_proxies[i].proxyId = b2BroadPhase::e_nullProxy;
	}
	m_proxyCount = 0;

	m_density = def->density;
}

void b2Fixture::Destroy(b2BlockAllocator* allocator)
{
	// The proxies must be destroyed before calling this.
	b2Assert(m_proxyCount == 0);

	// Free the proxy array.
	std::int32_t childCount = m_shape->GetChildCount();
	allocator->Free(m_proxies, childCount * sizeof(b2FixtureProxy));
	m_proxies = nullptr;

	// Free the child shape.
	switch (m_shape->m_type)
	{
		case b2Shape::e_circle:
		{
			b2CircleShape* s = (b2CircleShape*)m_shape;
			s->~b2CircleShape();
			allocator->Free(s, sizeof(b2CircleShape));
		}
		break;

		case b2Shape::e_edge:
		{
			b2EdgeShape* s = (b2EdgeShape*)m_shape;
			s->~b2EdgeShape();
			allocator->Free(s, sizeof(b2EdgeShape));
		}
		break;

		case b2Shape::e_polygon:
		{
			b2PolygonShape* s = (b2PolygonShape*)m_shape;
			s->~b2PolygonShape();
			allocator->Free(s, sizeof(b2PolygonShape));
		}
		break;

		case b2Shape::e_chain:
		{
			b2ChainShape* s = (b2ChainShape*)m_shape;
			s->~b2ChainShape();
			allocator->Free(s, sizeof(b2ChainShape));
		}
		break;

		default:
			b2Assert(false);
			break;
	}

	m_shape = nullptr;
}

void b2Fixture::CreateProxies(b2BroadPhase* broadPhase, const b2Transform& xf)
{
	b2Assert(m_proxyCount == 0);

	// Create proxies in the broad-phase.
	m_proxyCount = m_shape->GetChildCount();

	for (std::int32_t i = 0; i < m_proxyCount; ++i)
	{
		b2FixtureProxy* proxy = m_proxies + i;
		m_shape->ComputeAABB(&proxy->aabb, xf, i);
		proxy->proxyId    = broadPhase->CreateProxy(proxy->aabb, proxy);
		proxy->fixture    = this;
		proxy->childIndex = i;
	}
}

void b2Fixture::DestroyProxies(b2BroadPhase* broadPhase)
{
	// Destroy proxies in the broad-phase.
	for (std::int32_t i = 0; i < m_proxyCount; ++i)
	{
		b2FixtureProxy* proxy = m_proxies + i;
		broadPhase->DestroyProxy(proxy->proxyId);
		proxy->proxyId = b2BroadPhase::e_nullProxy;
	}

	m_proxyCount = 0;
}

void b2Fixture::Synchronize(b2BroadPhase* broadPhase, const b2Transform& transform1, const b2Transform& transform2)
{
	if (m_proxyCount == 0)
	{
		return;
	}

	for (std::int32_t i = 0; i < m_proxyCount; ++i)
	{
		b2FixtureProxy* proxy = m_proxies + i;

		// Compute an AABB that covers the swept shape (may miss some rotation effect).
		b2AABB aabb1, aabb2;
		m_shape->ComputeAABB(&aabb1, transform1, proxy->childIndex);
		m_shape->ComputeAABB(&aabb2, transform2, proxy->childIndex);

		proxy->aabb.Combine(aabb1, aabb2);

		b2Vec2 displacement = aabb2.GetCenter() - aabb1.GetCenter();

		broadPhase->MoveProxy(proxy->proxyId, proxy->aabb, displacement);
	}
}

void b2Fixture::SetFilterData(const b2Filter& filter)
{
	m_filter = filter;

	Refilter();
}

void b2Fixture::Refilter()
{
	if (m_body == nullptr)
	{
		return;
	}

	// Flag associated contacts for filtering.
	b2ContactEdge* edge = m_body->GetContactList();
	while (edge)
	{
		b2Contact* contact  = edge->contact;
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();
		if (fixtureA == this || fixtureB == this)
		{
			contact->FlagForFiltering();
		}

		edge = edge->next;
	}

	b2World* world = m_body->GetWorld();

	if (world == nullptr)
	{
		return;
	}

	// Touch each proxy so that new pairs may be created
	b2BroadPhase* broadPhase = &world->m_contactManager.m_broadPhase;
	for (std::int32_t i = 0; i < m_proxyCount; ++i)
	{
		broadPhase->TouchProxy(m_proxies[i].proxyId);
	}
}

void b2Fixture::SetSensor(bool sensor)
{
	if (sensor != m_isSensor)
	{
		m_body->SetAwake(true);
		m_isSensor = sensor;
	}
}
