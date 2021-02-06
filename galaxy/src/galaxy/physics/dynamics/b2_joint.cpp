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

#include "galaxy/physics/b2_block_allocator.hpp"
#include "galaxy/physics/b2_body.hpp"
#include "galaxy/physics/b2_distance_joint.hpp"
#include "galaxy/physics/b2_friction_joint.hpp"
#include "galaxy/physics/b2_gear_joint.hpp"
#include "galaxy/physics/b2_motor_joint.hpp"
#include "galaxy/physics/b2_mouse_joint.hpp"
#include "galaxy/physics/b2_prismatic_joint.hpp"
#include "galaxy/physics/b2_pulley_joint.hpp"
#include "galaxy/physics/b2_revolute_joint.hpp"
#include "galaxy/physics/b2_weld_joint.hpp"
#include "galaxy/physics/b2_wheel_joint.hpp"
#include "galaxy/physics/b2_world.hpp"

#include <new>

void b2LinearStiffness(float& stiffness, float& damping,
	float frequencyHertz, float dampingRatio,
	const b2Body* bodyA, const b2Body* bodyB)
{
	float massA = bodyA->GetMass();
	float massB = bodyB->GetMass();
	float mass;
	if (massA > 0.0f && massB > 0.0f)
	{
		mass = massA * massB / (massA + massB);
	}
	else if (massA > 0.0f)
	{
		mass = massA;
	}
	else
	{
		mass = massB;
	}

	float omega = 2.0f * glm::pi<float>() * frequencyHertz;
	stiffness = mass * omega * omega;
	damping = 2.0f * mass * dampingRatio * omega;
}

void b2AngularStiffness(float& stiffness, float& damping,
	float frequencyHertz, float dampingRatio,
	const b2Body* bodyA, const b2Body* bodyB)
{
	float IA = bodyA->GetInertia();
	float IB = bodyB->GetInertia();
	float I;
	if (IA > 0.0f && IB > 0.0f)
	{
		I = IA * IB / (IA + IB);
	}
	else if (IA > 0.0f)
	{
		I = IA;
	}
	else
	{
		I = IB;
	}

	float omega = 2.0f * glm::pi<float>() * frequencyHertz;
	stiffness = I * omega * omega;
	damping = 2.0f * I * dampingRatio * omega;
}

b2Joint* b2Joint::Create(const b2JointDef* def, b2BlockAllocator* allocator)
{
	b2Joint* joint = nullptr;

	switch (def->type)
	{
	case e_distanceJoint:
		{
			void* mem = allocator->Allocate(sizeof(b2DistanceJoint));
			joint = new (mem) b2DistanceJoint(static_cast<const b2DistanceJointDef*>(def));
		}
		break;

	case e_mouseJoint:
		{
			void* mem = allocator->Allocate(sizeof(b2MouseJoint));
			joint = new (mem) b2MouseJoint(static_cast<const b2MouseJointDef*>(def));
		}
		break;

	case e_prismaticJoint:
		{
			void* mem = allocator->Allocate(sizeof(b2PrismaticJoint));
			joint = new (mem) b2PrismaticJoint(static_cast<const b2PrismaticJointDef*>(def));
		}
		break;

	case e_revoluteJoint:
		{
			void* mem = allocator->Allocate(sizeof(b2RevoluteJoint));
			joint = new (mem) b2RevoluteJoint(static_cast<const b2RevoluteJointDef*>(def));
		}
		break;

	case e_pulleyJoint:
		{
			void* mem = allocator->Allocate(sizeof(b2PulleyJoint));
			joint = new (mem) b2PulleyJoint(static_cast<const b2PulleyJointDef*>(def));
		}
		break;

	case e_gearJoint:
		{
			void* mem = allocator->Allocate(sizeof(b2GearJoint));
			joint = new (mem) b2GearJoint(static_cast<const b2GearJointDef*>(def));
		}
		break;

	case e_wheelJoint:
		{
			void* mem = allocator->Allocate(sizeof(b2WheelJoint));
			joint = new (mem) b2WheelJoint(static_cast<const b2WheelJointDef*>(def));
		}
		break;

	case e_weldJoint:
		{
			void* mem = allocator->Allocate(sizeof(b2WeldJoint));
			joint = new (mem) b2WeldJoint(static_cast<const b2WeldJointDef*>(def));
		}
		break;
        
	case e_frictionJoint:
		{
			void* mem = allocator->Allocate(sizeof(b2FrictionJoint));
			joint = new (mem) b2FrictionJoint(static_cast<const b2FrictionJointDef*>(def));
		}
		break;

	case e_motorJoint:
		{
			void* mem = allocator->Allocate(sizeof(b2MotorJoint));
			joint = new (mem) b2MotorJoint(static_cast<const b2MotorJointDef*>(def));
		}
		break;

	default:
		b2Assert(false);
		break;
	}

	return joint;
}

void b2Joint::Destroy(b2Joint* joint, b2BlockAllocator* allocator)
{
	joint->~b2Joint();
	switch (joint->m_type)
	{
	case e_distanceJoint:
		allocator->Free(joint, sizeof(b2DistanceJoint));
		break;

	case e_mouseJoint:
		allocator->Free(joint, sizeof(b2MouseJoint));
		break;

	case e_prismaticJoint:
		allocator->Free(joint, sizeof(b2PrismaticJoint));
		break;

	case e_revoluteJoint:
		allocator->Free(joint, sizeof(b2RevoluteJoint));
		break;

	case e_pulleyJoint:
		allocator->Free(joint, sizeof(b2PulleyJoint));
		break;

	case e_gearJoint:
		allocator->Free(joint, sizeof(b2GearJoint));
		break;

	case e_wheelJoint:
		allocator->Free(joint, sizeof(b2WheelJoint));
		break;
    
	case e_weldJoint:
		allocator->Free(joint, sizeof(b2WeldJoint));
		break;

	case e_frictionJoint:
		allocator->Free(joint, sizeof(b2FrictionJoint));
		break;

	case e_motorJoint:
		allocator->Free(joint, sizeof(b2MotorJoint));
		break;

	default:
		b2Assert(false);
		break;
	}
}

b2Joint::b2Joint(const b2JointDef* def)
{
	b2Assert(def->bodyA != def->bodyB);

	m_type = def->type;
	m_prev = nullptr;
	m_next = nullptr;
	m_bodyA = def->bodyA;
	m_bodyB = def->bodyB;
	m_index = 0;
	m_collideConnected = def->collideConnected;
	m_islandFlag = false;
	m_userData = def->userData;

	m_edgeA.joint = nullptr;
	m_edgeA.other = nullptr;
	m_edgeA.prev = nullptr;
	m_edgeA.next = nullptr;

	m_edgeB.joint = nullptr;
	m_edgeB.other = nullptr;
	m_edgeB.prev = nullptr;
	m_edgeB.next = nullptr;
}

bool b2Joint::IsEnabled() const
{
	return m_bodyA->IsEnabled() && m_bodyB->IsEnabled();
}
