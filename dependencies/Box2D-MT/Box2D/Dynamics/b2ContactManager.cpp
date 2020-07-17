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

#include "Box2D/Common/b2Timer.h"
#include "Box2D/Common/b2StackAllocator.h"
#include "Box2D/Dynamics/b2ContactManager.h"
#include "Box2D/Dynamics/b2Body.h"
#include "Box2D/Dynamics/b2Fixture.h"
#include "Box2D/Dynamics/Contacts/b2Contact.h"
#include "Box2D/MT/b2MtUtil.h"
#include "Box2D/MT/b2ThreadDataSorter.h"

/// A do-nothing contact listener.
class b2DefaultContactListener : public b2ContactListener
{
public:
	bool BeginContactImmediate(b2Contact* contact, uint32 threadId) override
	{
		B2_NOT_USED(contact);
		B2_NOT_USED(threadId);
		return false;
	}
	bool EndContactImmediate(b2Contact* contact, uint32 threadId) override
	{
		B2_NOT_USED(contact);
		B2_NOT_USED(threadId);
		return false;
	}
	bool PreSolveImmediate(b2Contact* contact, const b2Manifold* oldManifold, uint32 threadId) override
	{
		B2_NOT_USED(contact);
		B2_NOT_USED(oldManifold);
		B2_NOT_USED(threadId);
		return false;
	}
	bool PostSolveImmediate(b2Contact* contact, const b2ContactImpulse* impulse, uint32 threadId) override
	{
		B2_NOT_USED(contact);
		B2_NOT_USED(impulse);
		B2_NOT_USED(threadId);
		return false;
	}
};

b2ContactFilter b2_defaultFilter;
b2DefaultContactListener b2_defaultListener;

bool b2ContactPointerLessThan(const b2Contact* a, const b2Contact* b)
{
	return a->m_proxyIds < b->m_proxyIds;
}

bool b2DeferredContactCreateLessThan(const b2DeferredContactCreate& a, const b2DeferredContactCreate& b)
{
	return a.proxyIds < b.proxyIds;
}

bool b2DeferredMoveProxyLessThan(const b2DeferredMoveProxy& a, const b2DeferredMoveProxy& b)
{
	return a.proxyId < b.proxyId;
}

bool b2DeferredPreSolveLessThan(const b2DeferredPreSolve& a, const b2DeferredPreSolve& b)
{
	return b2ContactPointerLessThan(a.contact, b.contact);
}

bool b2DeferredPostSolveLessThan(const b2DeferredPostSolve& a, const b2DeferredPostSolve& b)
{
	return b2ContactPointerLessThan(a.contact, b.contact);
}

bool b2ToiContactPointerLessThan(const b2Contact* a, const b2Contact* b)
{
	return b2Contact::ToiLessThan(a->m_toi, a, b->m_toi, b);
}

inline bool b2ContactManager::IsContactActive(b2Contact* c)
{
	b2Body* bodyA = c->m_nodeB.other;
	b2Body* bodyB = c->m_nodeA.other;

	// At least one body must be awake and it must be dynamic or kinematic.
	if ((bodyA->IsAwake() && bodyA->GetType() != b2_staticBody) ||
		(bodyB->IsAwake() && bodyB->GetType() != b2_staticBody))
	{
		return true;
	}

	return false;
}

b2ContactManager::b2ContactManager()
	: m_perThreadData{}
{
	m_contactList = nullptr;
	m_contactFilter = &b2_defaultFilter;
	m_contactListener = &b2_defaultListener;
	m_allocator = nullptr;
	m_deferCreates = false;
	m_toiCount = 0;
}

void b2ContactManager::Destroy(b2Contact* c)
{
	b2Fixture* fixtureA = c->GetFixtureA();
	b2Fixture* fixtureB = c->GetFixtureB();
	b2Body* bodyA = fixtureA->GetBody();
	b2Body* bodyB = fixtureB->GetBody();

	if (m_contactListener && c->IsTouching() && m_contactListener->EndContactImmediate(c, 0))
	{
		m_contactListener->EndContact(c);
	}

	// Remove from the world.
	RemoveFromContactList(c);
	RemoveFromContactArray(c);

	// Remove from body 1
	if (c->m_nodeA.prev)
	{
		c->m_nodeA.prev->next = c->m_nodeA.next;
	}

	if (c->m_nodeA.next)
	{
		c->m_nodeA.next->prev = c->m_nodeA.prev;
	}

	if (&c->m_nodeA == bodyA->m_contactList)
	{
		bodyA->m_contactList = c->m_nodeA.next;
	}

	// Remove from body 2
	if (c->m_nodeB.prev)
	{
		c->m_nodeB.prev->next = c->m_nodeB.next;
	}

	if (c->m_nodeB.next)
	{
		c->m_nodeB.next->prev = c->m_nodeB.prev;
	}

	if (&c->m_nodeB == bodyB->m_contactList)
	{
		bodyB->m_contactList = c->m_nodeB.next;
	}

	// Call the factory.
	b2Contact::Destroy(c, m_allocator);

	SanityCheck();
}

// This is the top level collision call for the time step. Here
// all the narrow phase collision is processed for the world
// contact list.
void b2ContactManager::Collide(uint32 contactsBegin, uint32 contactsEnd, uint32 threadId)
{
	b2ContactManagerPerThreadData& td = m_perThreadData[threadId];

	// Update awake contacts.
	for (uint32 i = contactsBegin; i < contactsEnd; ++i)
	{
		b2Contact* c = m_contacts[i];

		b2Fixture* fixtureA = c->GetFixtureA();
		b2Fixture* fixtureB = c->GetFixtureB();
		b2Body* bodyA = fixtureA->GetBody();
		b2Body* bodyB = fixtureB->GetBody();

		// Is this contact flagged for filtering?
		if (c->m_flags & b2Contact::e_filterFlag)
		{
			// Should these bodies collide?
			if (bodyB->ShouldCollide(bodyA) == false)
			{
				td.m_destroys.push_back(c);
				continue;
			}

			// Check user filtering.
			if (m_contactFilter && m_contactFilter->ShouldCollide(fixtureA, fixtureB, threadId) == false)
			{
				td.m_destroys.push_back(c);
				continue;
			}

			// Clear the filtering flag.
			c->m_flags &= ~b2Contact::e_filterFlag;
		}

		if (c->m_flags & b2Contact::e_inactiveFlag)
		{
			b2Assert(IsContactActive(c) == false);
			continue;
		}

		bool overlap = m_broadPhase.TestOverlap(c->m_proxyIds.low, c->m_proxyIds.high);

		// Here we destroy contacts that cease to overlap in the broad-phase.
		if (overlap == false)
		{
			td.m_destroys.push_back(c);
			continue;
		}

		// The contact persists.
		c->Update(td, m_contactListener, threadId);
	}
}

void b2ContactManager::FindNewContacts(uint32 moveBegin, uint32 moveEnd, uint32 threadId)
{
	m_broadPhase.UpdatePairs(moveBegin, moveEnd, this, threadId);
}

void b2ContactManager::AddPair(void* proxyUserDataA, void* proxyUserDataB, uint32 threadId)
{
	b2FixtureProxy* proxyA = (b2FixtureProxy*)proxyUserDataA;
	b2FixtureProxy* proxyB = (b2FixtureProxy*)proxyUserDataB;

	b2Fixture* fixtureA = proxyA->fixture;
	b2Fixture* fixtureB = proxyB->fixture;

	b2Body* bodyA = fixtureA->GetBody();
	b2Body* bodyB = fixtureB->GetBody();

	// Are the fixtures on the same body?
	if (bodyA == bodyB)
	{
		return;
	}

	b2ContactProxyIds proxyIds(proxyA->proxyId, proxyB->proxyId);

	// TODO_ERIN use a hash table to remove a potential bottleneck when both
	// bodies have a lot of contacts.
	// Does a contact already exist?
	b2ContactEdge* edge = bodyB->GetContactList();
	while (edge)
	{
		if (edge->other == bodyA)
		{
			const b2ContactProxyIds& edgeProxyIds = edge->contact->m_proxyIds;

			if (edgeProxyIds == proxyIds)
			{
				// A contact already exists.
				return;
			}
		}

		edge = edge->next;
	}

	// Does a joint override collision? Is at least one body dynamic?
	if (bodyB->ShouldCollide(bodyA) == false)
	{
		return;
	}

	// Check user filtering.
	if (m_contactFilter && m_contactFilter->ShouldCollide(fixtureA, fixtureB, threadId) == false)
	{
		return;
	}

	int32 indexA = proxyA->childIndex;
	int32 indexB = proxyB->childIndex;

	if (m_deferCreates)
	{
		b2DeferredContactCreate deferredCreate;
		deferredCreate.fixtureA = proxyA->fixture;
		deferredCreate.fixtureB = proxyB->fixture;
		deferredCreate.indexA = indexA;
		deferredCreate.indexB = indexB;
		deferredCreate.proxyIds = proxyIds;
		m_perThreadData[threadId].m_creates.push_back(deferredCreate);
	}
	else
	{
		// Call the factory.
		b2Contact* c = b2Contact::Create(fixtureA, indexA, fixtureB, indexB, m_allocator);
		if (c == nullptr)
		{
			return;
		}

		OnContactCreate(c, proxyIds);
	}
}

// This allows proxy synchronization to be somewhat parallel.
void b2ContactManager::SynchronizeFixtures(b2Body** bodies, uint32 count, uint32 threadId)
{
	b2ContactManagerPerThreadData& td = m_perThreadData[threadId];

	for (uint32 i = 0; i < count; ++i)
	{
		b2Body* b = bodies[i];

		b2Assert(b->GetType() != b2_staticBody);

		// If a body was not in an island then it did not move.
		if ((b->m_flags & b2Body::e_islandFlag) == 0)
		{
			continue;
		}

		b2Transform xf1;
		xf1.q.Set(b->m_sweep.a0);
		xf1.p = b->m_sweep.c0 - b2Mul(xf1.q, b->m_sweep.localCenter);

		for (b2Fixture* f = b->m_fixtureList; f; f = f->m_next)
		{
			for (int32 j = 0; j < f->m_proxyCount; ++j)
			{
				b2FixtureProxy* proxy = f->m_proxies + j;

				b2Shape* shape = f->m_shape;

				// Compute an AABB that covers the swept shape (may miss some rotation effect).
				b2AABB aabb1, aabb2;
				shape->ComputeAABB(&aabb1, xf1, proxy->childIndex);
				shape->ComputeAABB(&aabb2, b->m_xf, proxy->childIndex);

				proxy->aabb.Combine(aabb1, aabb2);

				// A move is required if the new AABB isn't contained by the fat AABB.
				bool requiresMove = m_broadPhase.GetFatAABB(proxy->proxyId).Contains(proxy->aabb) == false;

				if (requiresMove)
				{
					b2DeferredMoveProxy moveProxy;
					moveProxy.aabb = proxy->aabb;
					moveProxy.displacement = b->m_xf.p - xf1.p;
					moveProxy.proxyId = proxy->proxyId;
					td.m_moveProxies.push_back(moveProxy);
				}
			}
		}
	}
}

void b2ContactManager::FinishFindNewContacts(b2TaskExecutor& executor, b2TaskGroup* taskGroup, b2StackAllocator& allocator)
{
	m_broadPhase.ResetBuffers();
	auto creates = b2MakeStackAllocThreadDataSorter<b2DeferredContactCreate>(m_perThreadData,
		&b2ContactManagerPerThreadData::m_creates, b2DeferredContactCreateLessThan, allocator);

	b2Sort(creates, executor, taskGroup, allocator);

	b2ContactProxyIds prevIds{};

	for (auto it = creates.begin(); it != creates.end(); ++it)
	{
		if (it->proxyIds == prevIds)
		{
			continue;
		}
		prevIds = it->proxyIds;

		ConsumeCreate(*it);
	}
}

void b2ContactManager::FinishCollide(b2TaskExecutor& executor, b2TaskGroup* taskGroup, b2StackAllocator& allocator)
{
	auto begins = b2MakeStackAllocThreadDataSorter<b2Contact*>(m_perThreadData,
		&b2ContactManagerPerThreadData::m_beginContacts, b2ContactPointerLessThan, allocator);

	auto ends = b2MakeStackAllocThreadDataSorter<b2Contact*>(m_perThreadData,
		&b2ContactManagerPerThreadData::m_endContacts, b2ContactPointerLessThan, allocator);

	auto preSolves = b2MakeStackAllocThreadDataSorter<b2DeferredPreSolve>(m_perThreadData,
		&b2ContactManagerPerThreadData::m_preSolves, b2DeferredPreSolveLessThan, allocator);

	auto destroys = b2MakeStackAllocThreadDataSorter<b2Contact*>(m_perThreadData,
		&b2ContactManagerPerThreadData::m_destroys, b2ContactPointerLessThan, allocator);

	while (true)
	{
		begins.SubmitSortTask(executor, taskGroup);
		ends.SubmitSortTask(executor, taskGroup);
		preSolves.SubmitSortTask(executor, taskGroup);
		destroys.SubmitSortTask(executor, taskGroup);

		if (begins.IsSubmitRequired() == false && ends.IsSubmitRequired() == false &&
			preSolves.IsSubmitRequired() == false && destroys.IsSubmitRequired() == false)
		{
			ConsumeAwakes();
			executor.Wait(taskGroup, b2MainThreadCtx(&allocator));
			break;
		}

		executor.Wait(taskGroup, b2MainThreadCtx(&allocator));
	}

	for (auto it = begins.begin(); it != begins.end(); ++it)
	{
		m_contactListener->BeginContact(*it);
	}

	for (auto it = ends.begin(); it != ends.end(); ++it)
	{
		m_contactListener->EndContact(*it);
	}

	for (auto it = preSolves.begin(); it != preSolves.end(); ++it)
	{
		m_contactListener->PreSolve(it->contact, &it->oldManifold);
	}

	for (auto it = destroys.begin(); it != destroys.end(); ++it)
	{
		Destroy(*it);
	}
}

void b2ContactManager::FinishSynchronizeFixtures(b2TaskExecutor& executor, b2TaskGroup* taskGroup, b2StackAllocator& allocator)
{
	auto moves = b2MakeStackAllocThreadDataSorter<b2DeferredMoveProxy>(m_perThreadData,
		&b2ContactManagerPerThreadData::m_moveProxies, b2DeferredMoveProxyLessThan, allocator);

	b2Sort(moves, executor, taskGroup, allocator);

	for (auto it = moves.begin(); it != moves.end(); ++it)
	{
		m_broadPhase.MoveProxy(it->proxyId, it->aabb, it->displacement);
	}
}

void b2ContactManager::FinishSolve(b2TaskExecutor& executor, b2TaskGroup* taskGroup, b2StackAllocator& allocator)
{
	auto postSolves = b2MakeStackAllocThreadDataSorter<b2DeferredPostSolve>(m_perThreadData,
		&b2ContactManagerPerThreadData::m_postSolves, b2DeferredPostSolveLessThan, allocator);

	while (postSolves.IsSubmitRequired())
	{
		postSolves.SubmitSortTask(executor, taskGroup);

		executor.Wait(taskGroup, b2MainThreadCtx(&allocator));
	}

	for (auto it = postSolves.begin(); it != postSolves.end(); ++it)
	{
		m_contactListener->PostSolve(it->contact, &it->impulse);
	}
}

void b2ContactManager::ConsumeAwakes()
{
	for (uint32 i = 0; i < b2_maxThreads; ++i)
	{
		while (m_perThreadData[i].m_awakes.size())
		{
			b2Contact* c = m_perThreadData[i].m_awakes.pop_back();
			b2Body* bodyA = c->m_nodeB.other;
			b2Body* bodyB = c->m_nodeB.other;

			bodyA->SetAwake(true);
			bodyB->SetAwake(true);
		}
	}
}

void b2ContactManager::ConsumeCreate(const b2DeferredContactCreate& create)
{
	b2Fixture* fixtureA = create.fixtureA;
	b2Fixture* fixtureB = create.fixtureB;

	int32 indexA = create.indexA;
	int32 indexB = create.indexB;

	// Call the factory.
	b2Contact* c = b2Contact::Create(fixtureA, indexA, fixtureB, indexB, m_allocator);
	if (c == nullptr)
	{
		return;
	}

	// Finish creating.
	OnContactCreate(c, create.proxyIds);
}

inline void b2ContactManager::OnContactCreate(b2Contact* c, b2ContactProxyIds proxyIds)
{
	b2Fixture* fixtureA = c->GetFixtureA();
	b2Fixture* fixtureB = c->GetFixtureB();

	c->m_proxyIds = proxyIds;

	// Mark for TOI if needed.
	if (b2Contact::IsToiCandidate(fixtureA, fixtureB))
	{
		c->m_flags |= b2Contact::e_toiCandidateFlag;
	}

	// Connect to island graph.
	b2Body* bodyA = fixtureA->GetBody();
	b2Body* bodyB = fixtureB->GetBody();

	// Wake up the bodies
	if (fixtureA->IsSensor() == false && fixtureB->IsSensor() == false)
	{
		bodyA->SetAwake(true);
		bodyB->SetAwake(true);
	}

	// Connect to body A
	c->m_nodeA.contact = c;
	c->m_nodeA.other = bodyB;

	c->m_nodeA.prev = nullptr;
	c->m_nodeA.next = bodyA->m_contactList;
	if (bodyA->m_contactList != nullptr)
	{
		bodyA->m_contactList->prev = &c->m_nodeA;
	}
	bodyA->m_contactList = &c->m_nodeA;

	// Connect to body B
	c->m_nodeB.contact = c;
	c->m_nodeB.other = bodyA;

	c->m_nodeB.prev = nullptr;
	c->m_nodeB.next = bodyB->m_contactList;
	if (bodyB->m_contactList != nullptr)
	{
		bodyB->m_contactList->prev = &c->m_nodeB;
	}
	bodyB->m_contactList = &c->m_nodeB;

	// Is the contact inactive?
	if (IsContactActive(c) == false)
	{
		c->m_flags |= b2Contact::e_inactiveFlag;
	}

	// Insert into the world.
	AddToContactList(c);
	AddToContactArray(c);
}

void b2ContactManager::RecalculateToiCandidacy(b2Body* body)
{
	for (b2ContactEdge* ce = body->GetContactList(); ce; ce = ce->next)
	{
		RecalculateToiCandidacy(ce->contact);
	}
}

void b2ContactManager::RecalculateToiCandidacy(b2Fixture* fixture)
{
	b2Body* body = fixture->GetBody();

	for (b2ContactEdge* ce = body->GetContactList(); ce; ce = ce->next)
	{
		b2Contact* contact = ce->contact;

		if (contact->GetFixtureA() == fixture || contact->GetFixtureB() == fixture)
		{
			RecalculateToiCandidacy(ce->contact);
		}
	}
}

void b2ContactManager::RecalculateToiCandidacy(b2Contact* c)
{
	b2Fixture* fixtureA = c->GetFixtureA();
	b2Fixture* fixtureB = c->GetFixtureB();

	uint32 flags = c->m_flags;

	if (b2Contact::IsToiCandidate(fixtureA, fixtureB))
	{
		flags |= b2Contact::e_toiCandidateFlag;
	}
	else
	{
		flags &= ~b2Contact::e_toiCandidateFlag;
	}

	if (flags == c->m_flags)
	{
		return;
	}

	c->m_flags = flags & ~b2Contact::e_toiFlag;
	c->m_toiCount = 0;
	c->m_toi = 1.0f;

	if (c->m_managerIndex == -1)
	{
		SanityCheck();
		return;
	}

	if (c->m_flags & b2Contact::e_toiCandidateFlag)
	{
		b2Assert(c->m_managerIndex >= (int32)m_toiCount);
		m_contacts[m_toiCount]->m_managerIndex = c->m_managerIndex;
		m_contacts[c->m_managerIndex] = m_contacts[m_toiCount];
		m_contacts[m_toiCount] = c;
		c->m_managerIndex = m_toiCount;
		++m_toiCount;
	}
	else
	{
		b2Assert(c->m_managerIndex < (int32)m_toiCount);
		--m_toiCount;
		m_contacts[m_toiCount]->m_managerIndex = c->m_managerIndex;
		m_contacts[c->m_managerIndex] = m_contacts[m_toiCount];
		m_contacts[m_toiCount] = c;
		c->m_managerIndex = m_toiCount;
	}

	SanityCheck();
}

void b2ContactManager::RecalculateSleeping(b2Body* body)
{
	for (b2ContactEdge* ce = body->GetContactList(); ce; ce = ce->next)
	{
		b2Contact* c = ce->contact;

		if (IsContactActive(c) == false)
		{
			c->m_flags |= b2Contact::e_inactiveFlag;
		}
		else
		{
			c->m_flags &= ~b2Contact::e_inactiveFlag;
		}
	}
}

inline void b2ContactManager::AddToContactArray(b2Contact* c)
{
	b2Assert(c->m_managerIndex == -1);

	if (c->m_flags & b2Contact::e_toiCandidateFlag)
	{
		if (m_toiCount < m_contacts.size())
		{
			c->m_managerIndex = m_toiCount;
			b2Assert((m_contacts[m_toiCount]->m_flags & b2Contact::e_toiCandidateFlag) == 0);
			m_contacts[m_toiCount]->m_managerIndex = m_contacts.size();
			m_contacts.push_back(m_contacts[m_toiCount]);
			m_contacts[m_toiCount] = c;
			++m_toiCount;
		}
		else
		{
			c->m_managerIndex = m_contacts.size();
			m_contacts.push_back(c);
			++m_toiCount;
		}
	}
	else
	{
		c->m_managerIndex = m_contacts.size();
		m_contacts.push_back(c);
	}
}

inline void b2ContactManager::RemoveFromContactArray(b2Contact* c)
{
	b2Assert(c->m_managerIndex >= 0);

	if (c->m_managerIndex < (int32)m_toiCount)
	{
		b2Assert((c->m_flags & b2Contact::e_toiCandidateFlag) == b2Contact::e_toiCandidateFlag);
		--m_toiCount;
		m_contacts[m_toiCount]->m_managerIndex = c->m_managerIndex;
		m_contacts[c->m_managerIndex] = m_contacts[m_toiCount];
		b2Contact* backContact = m_contacts.pop_back();
		if (m_contacts.size() > m_toiCount)
		{
			m_contacts[m_toiCount] = backContact;
			m_contacts[m_toiCount]->m_managerIndex = m_toiCount;
		}
	}
	else
	{
		b2Assert((c->m_flags & b2Contact::e_toiCandidateFlag) == 0);
		m_contacts.back()->m_managerIndex = c->m_managerIndex;
		b2RemoveAndSwapBack(m_contacts, c->m_managerIndex);
	}

	c->m_managerIndex = -1;
}

inline void b2ContactManager::AddToContactList(b2Contact* c)
{
	c->m_prev = nullptr;
	c->m_next = m_contactList;
	if (m_contactList != nullptr)
	{
		m_contactList->m_prev = c;
	}
	m_contactList = c;
}

inline void b2ContactManager::RemoveFromContactList(b2Contact* c)
{
	if (c->m_prev)
	{
		c->m_prev->m_next = c->m_next;
	}
	if (c->m_next)
	{
		c->m_next->m_prev = c->m_prev;
	}
	if (c == m_contactList)
	{
		m_contactList = c->m_next;
	}
}

void b2ContactManager::SanityCheck()
{
#if 0
	for (uint32 i = 0; i < m_contacts.size(); ++i)
	{
		b2Contact* c = m_contacts[i];
		b2Assert(c->m_managerIndex == (int32)i);
	}

	for (b2Contact* c = m_contactList; c; c = c->m_next)
	{
		int32 index = c->m_managerIndex;
		b2Assert(index > -1);
		b2Contact* other = m_contacts[index];
		b2Assert(c == other);
		if (b2Contact::IsToiCandidate(c->GetFixtureA(), c->GetFixtureB()))
		{
			b2Assert(index < (int32)m_toiCount);
		}
		else
		{
			b2Assert(index >= (int32)m_toiCount);
		}
	}
#endif
}