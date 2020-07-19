/*
* Copyright (c) 2006-2009 Erin Catto http://www.box2d.org
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

#ifndef B2_WORLD_CALLBACKS_H
#define B2_WORLD_CALLBACKS_H

#include "Box2D/Common/b2Settings.h"

struct b2Vec2;
struct b2Transform;
class b2Fixture;
class b2Body;
class b2Joint;
class b2Contact;
struct b2ContactResult;
struct b2Manifold;

/// Joints and fixtures are destroyed when their associated
/// body is destroyed. Implement this listener so that you
/// may nullify references to these joints and shapes.
class b2DestructionListener
{
public:
	virtual ~b2DestructionListener() {}

	/// Called when any joint is about to be destroyed due
	/// to the destruction of one of its attached bodies.
	virtual void SayGoodbye(b2Joint* joint) = 0;

	/// Called when any fixture is about to be destroyed due
	/// to the destruction of its parent body.
	virtual void SayGoodbye(b2Fixture* fixture) = 0;
};

/// Implement this class to provide collision filtering. In other words, you can implement
/// this class if you want finer control over contact creation.
class b2ContactFilter
{
public:
	virtual ~b2ContactFilter() {}

	/// Return true if contact calculations should be performed between these two shapes.
	/// For thread safety, don't access any contacts and don't modify any other Box2D objects.
	/// Note: threadId is unique per thread and less than the number of threads.
	/// @warning for performance reasons this is only called when the AABBs begin to overlap.
	/// @warning this function is called from multiple threads.
	virtual bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB, uint32 threadId);
};

/// Contact impulses for reporting. Impulses are used instead of forces because
/// sub-step forces may approach infinity for rigid body collisions. These
/// match up one-to-one with the contact points in b2Manifold.
struct b2ContactImpulse
{
	float32 normalImpulses[b2_maxManifoldPoints];
	float32 tangentImpulses[b2_maxManifoldPoints];
	int32 count;
};

/// Implement this class to get contact information. You can use these results for
/// things like sounds and game logic. You can also get contact results by
/// traversing the contact lists after the time step. However, you might miss
/// some contacts because continuous physics leads to sub-stepping.
/// Additionally you may receive multiple callbacks for the same contact in a
/// single time step.
/// You should strive to make your callbacks efficient because there may be
/// many callbacks per time step.
/// @warning You cannot create/destroy Box2D entities inside these callbacks.
/// @warning When multi-threading is enabled for the world, the immediate callback
/// functions are called in a nondeterministic order from multiple threads simultaneously.
/// Note: The non-immediate callbacks are always called in a deterministic order from
/// a single thread.
class b2ContactListener
{
public:
	virtual ~b2ContactListener() {}

	/// Called when two fixtures begin to touch.
	/// Note: this is called when no other threads are accessing the world's objects.
	/// Note: this is only called on contacts for which BeginContactImmediate returns true.
	virtual void BeginContact(b2Contact* contact) { B2_NOT_USED(contact); }

	/// Called when two fixtures cease to touch.
	/// Note: this is called when no other threads are accessing the world's objects.
	/// Note: this is only called on contacts for which EndContactImmediate returns true.
	virtual void EndContact(b2Contact* contact) { B2_NOT_USED(contact); }

	/// This is called after a contact is updated. This allows you to inspect a
	/// contact before it goes to the solver. If you are careful, you can modify the
	/// contact manifold (e.g. disable contact).
	/// A copy of the old manifold is provided so that you can detect changes.
	/// Note: this is called only for awake bodies.
	/// Note: this is called even when the number of contact points is zero.
	/// Note: this is not called for sensors.
	/// Note: if you set the number of contact points to zero, you will not
	/// get an EndContact callback. However, you may get a BeginContact callback
	/// the next step.
	/// Note: this is called when no other threads are accessing the world's objects.
	/// Note: this is only called on contacts for which PreSolveImmediate returns true.
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{
		B2_NOT_USED(contact);
		B2_NOT_USED(oldManifold);
	}

	/// This lets you inspect a contact after the solver is finished. This is useful
	/// for inspecting impulses.
	/// Note: the contact manifold does not include time of impact impulses, which can be
	/// arbitrarily large if the sub-step is small. Hence the impulse is provided explicitly
	/// in a separate data structure.
	/// Note: this is only called for contacts that are touching, solid, and awake.
	/// Note: this is called when no other threads are accessing the world's objects.
	/// Note: this is only called on contacts for which PostSolveImmediate returns true.
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	{
		B2_NOT_USED(contact);
		B2_NOT_USED(impulse);
	}

	/// This lets you process and filter BeginContact callbacks as they arise from multiple threads.
	/// Within this callback, bodies and joints must not be modified. It's safe to read and modify
	/// the provided contact. Other contacts must not be accessed. Unmentioned Box2d objects probably
	/// shouldn't be accessed.
	/// Note: threadId is unique per thread and less than the number of threads.
	/// @return true if BeginContact must be called for the contact.
	/// @warning this function is called from multiple threads.
	virtual bool BeginContactImmediate(b2Contact* contact, uint32 threadId) = 0;

	/// This lets you process and filter EndContact callbacks as they arise from multiple threads.
	/// Within this callback, bodies and joints must not be modified. It's safe to read and modify
	/// the provided contact. Other contacts must not be accessed. Unmentioned Box2d objects probably
	/// shouldn't be accessed.
	/// Note: threadId is unique per thread and less than the number of threads.
	/// @return true if EndContact must be called for the contact.
	/// @warning this function is called from multiple threads.
	virtual bool EndContactImmediate(b2Contact* contact, uint32 threadId) = 0;

	/// This lets you process and filter PreSolve callbacks as they arise from multiple threads.
	/// Within this callback, it's safe to read and modify the provided contact. A non-static body that is
	/// part of the provided contact is also safe to modify, except for the body's flags, which must be
	/// treated as read-only. Joints attached to a non-static body are safe to modify. A static body that
	/// is part of the provided contact must be treated as read-only, except for its flags, which must
	/// not be accessed. Other bodies, joints, and contacts must not be accessed. Unmentioned Box2d objects
	/// probably shouldn't be accessed.
	/// Note: threadId is unique per thread and less than the number of threads.
	/// @return true if PreSolve must be called for the contact.
	/// @warning this function is called from multiple threads.
	virtual bool PreSolveImmediate(b2Contact* contact, const b2Manifold* oldManifold, uint32 threadId) = 0;

	/// This lets you process and filter PostSolve callbacks as they arise from multiple threads.
	/// Within this callback, it's safe to read and modify the provided contact. Other contacts must not
	/// be accessed. It's safe to read or modify a non-static body that is part of the provided contact.
	/// A static body that is part of the provided contact must be treated as read-only. Unmentioned
	/// Box2d objects probably shouldn't be accessed.
	/// Note: threadId is unique per thread and less than the number of threads.
	/// @return true if PostSolve must be called for the contact.
	/// @warning this function is called from multiple threads.
	virtual bool PostSolveImmediate(b2Contact* contact, const b2ContactImpulse* impulse, uint32 threadId) = 0;
};

/// Callback class for AABB queries.
/// See b2World::Query
class b2QueryCallback
{
public:
	virtual ~b2QueryCallback() {}

	/// Called for each fixture found in the query AABB.
	/// @return false to terminate the query.
	virtual bool ReportFixture(b2Fixture* fixture) = 0;
};

/// Callback class for ray casts.
/// See b2World::RayCast
class b2RayCastCallback
{
public:
	virtual ~b2RayCastCallback() {}

	/// Called for each fixture found in the query. You control how the ray cast
	/// proceeds by returning a float:
	/// return -1: ignore this fixture and continue
	/// return 0: terminate the ray cast
	/// return fraction: clip the ray to this point
	/// return 1: don't clip the ray and continue
	/// @param fixture the fixture hit by the ray
	/// @param point the point of initial intersection
	/// @param normal the normal vector at the point of intersection
	/// @return -1 to filter, 0 to terminate, fraction to clip the ray for
	/// closest hit, 1 to continue
	virtual float32 ReportFixture(	b2Fixture* fixture, const b2Vec2& point,
									const b2Vec2& normal, float32 fraction) = 0;
};

#endif
