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

#ifndef B2_SETTINGS_H
#define B2_SETTINGS_H

#include <cstddef>
#include <cassert>
#include <cstdint>
#include <limits>

#if !defined(NDEBUG)
	#define b2DEBUG
#endif

#define B2_NOT_USED(x) ((void)(x))

#define b2Assert(A) assert(A)

typedef std::int8_t		int8;
typedef std::int16_t	int16;
typedef std::int32_t	int32;
typedef std::int64_t	int64;
typedef std::uint8_t	uint8;
typedef std::uint16_t	uint16;
typedef std::uint32_t	uint32;
typedef std::uint64_t	uint64;
typedef float			float32;
typedef double			float64;

#define	b2_maxFloat		std::numeric_limits<float32>::max()
#define	b2_epsilon		std::numeric_limits<float32>::epsilon()
#define b2_pi			3.14159265359f

/// @file
/// Global tuning constants based on meters-kilograms-seconds (MKS) units.
///

// Collision

/// The maximum number of contact points between two convex shapes. Do
/// not change this value.
#define b2_maxManifoldPoints	2

/// The maximum number of vertices on a convex polygon. You cannot increase
/// this too much because b2BlockAllocator has a maximum object size.
#define b2_maxPolygonVertices	8

/// This is used to fatten AABBs in the dynamic tree. This allows proxies
/// to move by a small amount without triggering a tree adjustment.
/// This is in meters.
#define b2_aabbExtension		0.1f

/// This is used to fatten AABBs in the dynamic tree. This is used to predict
/// the future position based on the current displacement.
/// This is a dimensionless multiplier.
#define b2_aabbMultiplier		2.0f

/// A small length used as a collision and constraint tolerance. Usually it is
/// chosen to be numerically significant, but visually insignificant.
#define b2_linearSlop			0.005f

/// A small angle used as a collision and constraint tolerance. Usually it is
/// chosen to be numerically significant, but visually insignificant.
#define b2_angularSlop			(2.0f / 180.0f * b2_pi)

/// The radius of the polygon/edge shape skin. This should not be modified. Making
/// this smaller means polygons will have an insufficient buffer for continuous collision.
/// Making it larger may create artifacts for vertex collision.
#define b2_polygonRadius		(2.0f * b2_linearSlop)

/// Maximum number of sub-steps per contact in continuous physics simulation.
#define b2_maxSubSteps			8


// Dynamics

/// Maximum number of contacts to be handled to solve a TOI impact.
#define b2_maxTOIContacts			32

/// A velocity threshold for elastic collisions. Any collision with a relative linear
/// velocity below this threshold will be treated as inelastic.
#define b2_velocityThreshold		1.0f

/// The maximum linear position correction used when solving constraints. This helps to
/// prevent overshoot.
#define b2_maxLinearCorrection		0.2f

/// The maximum angular position correction used when solving constraints. This helps to
/// prevent overshoot.
#define b2_maxAngularCorrection		(8.0f / 180.0f * b2_pi)

/// The maximum linear velocity of a body. This limit is very large and is used
/// to prevent numerical problems. You shouldn't need to adjust this.
#define b2_maxTranslation			2.0f
#define b2_maxTranslationSquared	(b2_maxTranslation * b2_maxTranslation)

/// The maximum angular velocity of a body. This limit is very large and is used
/// to prevent numerical problems. You shouldn't need to adjust this.
#define b2_maxRotation				(0.5f * b2_pi)
#define b2_maxRotationSquared		(b2_maxRotation * b2_maxRotation)

/// This scale factor controls how fast overlap is resolved. Ideally this would be 1 so
/// that overlap is removed in one time step. However using values close to 1 often lead
/// to overshoot.
#define b2_baumgarte				0.2f
#define b2_toiBaugarte				0.75f


// Sleep

/// The time that a body must be still before it will go to sleep.
#define b2_timeToSleep				0.5f

/// A body cannot sleep if its linear velocity is above this tolerance.
#define b2_linearSleepTolerance		0.01f

/// A body cannot sleep if its angular velocity is above this tolerance.
#define b2_angularSleepTolerance	(2.0f / 180.0f * b2_pi)


// MT Defines

/// Force inline of larger functions that tend to not be inlined by default.
#ifdef _MSC_VER
    #define b2_forceInline __forceinline
#elif defined(__GNUC__)
    #define b2_forceInline inline __attribute__((__always_inline__))
#elif defined(__CLANG__)
    #if __has_attribute(__always_inline__)
        #define b2_forceInline inline __attribute__((__always_inline__))
    #else
        #define b2_forceInline inline
    #endif
#else
    #define b2_forceInline inline
#endif

/// Some global variables are used to gather stats on runtime behavior, but they are
/// disabled by default due to the data races they cause with multithreading.
/// Set this to true if you need to use those variables.
/// Note: behavior is undefined if multithreading is used while this is true.
#define b2_enableGlobalStats					false

/// The size of a cache line.
#define b2_cacheLineSize						64

/// The maximum number of threads. Must be greater than 1 and even.
#define b2_maxThreads							8

/// The maximum number of subtasks that a range task can be split into.
#define b2_maxRangeSubTasks						b2_maxThreads

/// The maximum number of islands per solve task.
#define b2_maxIslandsPerSolveTask				16

/// The number of task groups used by a world's step. Do not change this value.
#define b2_maxWorldStepTaskGroups				1

/// Defining this enables a modified broad-phase that tends to perform better with
/// many fixtures.
//#define b2_dynamicTreeOfTrees

// Memory Allocation

/// Implement this function to use your own memory allocator.
void* b2Alloc(int32 size);

/// If you implement b2Alloc, you should also implement this function.
void b2Free(void* mem);

/// Logging function.
void b2Log(const char* string, ...);

/// Version numbering scheme.
/// See http://en.wikipedia.org/wiki/Software_versioning
struct b2Version
{
	int32 major;		///< significant changes
	int32 minor;		///< incremental changes
	int32 revision;		///< bug fixes
};

/// Box2D-MT version.
extern b2Version b2_mtVersion;

/// Box2D version.
extern b2Version b2_version;

#endif
