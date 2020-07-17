/*
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

#ifndef B2_GROWABLE_ARRAY_H
#define B2_GROWABLE_ARRAY_H
#include "Box2D/Common/b2Settings.h"
#include "Box2D/Common/b2Math.h"
#include <cstring>

/// This is a growable array, meant for internal use only.
template <typename T>
class b2GrowableArray
{
public:
	b2GrowableArray(uint32 startCapacity = 128)
	{
		m_capacity = startCapacity;
		m_size = 0;

		m_array = (T*)b2Alloc(m_capacity * sizeof(T));
	}

	b2GrowableArray(b2GrowableArray&& rhs)
		: m_array(rhs.m_array)
		, m_size(rhs.m_size)
	{
		rhs.m_array = nullptr;
	}

	b2GrowableArray(const b2GrowableArray&) = delete;
	b2GrowableArray& operator=(const b2GrowableArray&) = delete;

	~b2GrowableArray()
	{
		b2Free(m_array);
		m_array = nullptr;
	}

	void push_back(T element)
	{
		if (m_size == m_capacity)
		{
			reserve(m_capacity * 2);
		}
		m_array[m_size] = element;
		++m_size;
	}

	T pop_back()
	{
		b2Assert(m_size > 0);
		--m_size;
		return m_array[m_size];
	}

	void assign(T* inputBegin, T* inputEnd)
	{
		uint32 size = (uint32)(inputEnd - inputBegin);
		if (size > m_capacity)
		{
			reserve(b2NextPowerOfTwo(size));
		}
		T* dest = begin();
		for(T* it = inputBegin; it != inputEnd; ++it)
		{
			*dest = *it;
			++dest;
		}
		m_size = size;
	}

	void reserve(uint32 capacity)
	{
		if (capacity > m_capacity)
		{
			m_capacity = capacity;
			T* old = m_array;
			m_array = (T*)b2Alloc(m_capacity * sizeof(T));
			memcpy(m_array, old, m_size * sizeof(T));
			b2Free(old);
		}
	}

	const T& front() const
	{
		b2Assert(m_size > 0);
		return m_array[0];
	}

	T& front()
	{
		b2Assert(m_size > 0);
		return m_array[0];
	}

	const T& back() const
	{
		b2Assert(m_size > 0);
		return m_array[m_size - 1];
	}

	T& back()
	{
		b2Assert(m_size > 0);
		return m_array[m_size - 1];
	}

	void clear()
	{
		m_size = 0;
	}

	T& operator[](size_t i)
	{
		return m_array[i];
	}

	const T& operator[](size_t i) const
	{
		return m_array[i];
	}

	uint32 size() const
	{
		return m_size;
	}

	T* data()
	{
		return m_array;
	}

	const T* data() const
	{
		return m_array;
	}

	T* begin()
	{
		return m_array;
	}

	const T* begin() const
	{
		return m_array;
	}

	T* end()
	{
		return m_array + m_size;
	}

	const T* end() const
	{
		return m_array + m_size;
	}

private:
	T* m_array;
	uint32 m_size;
	uint32 m_capacity;
};

#endif
