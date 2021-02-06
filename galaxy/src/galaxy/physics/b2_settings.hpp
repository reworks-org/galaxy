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

#ifndef B2_SETTINGS_H
#define B2_SETTINGS_H

/// @file
/// Settings that can be overriden for your application
///

#include <stdarg.h>
#include <stdint.h>

#include <cstdint>

#include <glm/gtc/constants.hpp>

#include "galaxy/error/Log.hpp"

// Tunable Constants

/// You can use this to change the length scale used by your game.
/// For example for inches you could use 39.4.
#define b2_lengthUnitsPerMeter 1.0f

/// The maximum number of vertices on a convex polygon. You cannot increase
/// this too much because b2BlockAllocator has a maximum object size.
#define b2_maxPolygonVertices 32

// User data

/// You can define this to inject whatever data you want in b2Body
struct b2BodyUserData
{
	b2BodyUserData()
	{
		pointer = 0;
	}

	/// For legacy compatibility
	uintptr_t pointer;
};

/// You can define this to inject whatever data you want in b2Fixture
struct b2FixtureUserData
{
	b2FixtureUserData()
	{
		pointer = 0;
	}

	/// For legacy compatibility
	uintptr_t pointer;
};

/// You can define this to inject whatever data you want in b2Joint
struct b2JointUserData
{
	b2JointUserData()
	{
		pointer = 0;
	}

	/// For legacy compatibility
	uintptr_t pointer;
};

// Memory Allocation

/// Default allocation functions
void* b2Alloc_Default(std::int32_t size);
void b2Free_Default(void* mem);

/// Implement this function to use your own memory allocator.
inline void* b2Alloc(std::int32_t size)
{
	return b2Alloc_Default(size);
}

/// If you implement b2Alloc, you should also implement this function.
inline void b2Free(void* mem)
{
	b2Free_Default(mem);
}

#include "galaxy/physics/b2_common.hpp"

#endif
