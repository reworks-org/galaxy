/*
* Copyright (c) 2019 Justin Hoffman https://github.com/jhoffman0x/Box2D-MT
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

#ifndef B2_THREAD_DATA_SORTER_H
#define B2_THREAD_DATA_SORTER_H

#include "Box2D/Common/b2StackAllocator.h"
#include "Box2D/MT/b2Task.h"
#include "Box2D/MT/b2TaskExecutor.h"
#include "Box2D/MT/b2MtUtil.h"
#include <algorithm>

// Merge two sorted arrays. No std::merge because we're not requiring C++17 support yet.
template<typename InputIt, typename OutputIt, typename Compare>
OutputIt b2Merge(InputIt beginA, InputIt endA, InputIt beginB, InputIt endB, OutputIt output, Compare comp)
{
	for (; beginA != endA; ++output)
	{
		if (beginB == endB)
		{
			return std::copy(beginA, endA, output);
		}
		if (comp(*beginB, *beginA))
		{
			*output = *beginB;
			++beginB;
		}
		else
		{
			*output = *beginA;
			++beginA;
		}
	}
    return std::copy(beginB, endB, output);
}

// A task for sorting.
template<typename InputIt, typename Compare>
class b2SortTask : public b2Task
{
public:
	b2SortTask() {}
	b2SortTask(InputIt begin, InputIt end, Compare comp)
		: m_begin(begin)
		, m_end(end)
		, m_comp(comp)
	{}

	virtual b2Task::Type GetType() const override { return b2Task::e_sort; }

	virtual void Execute(const b2ThreadContext&) override
	{
		std::sort(m_begin, m_end, m_comp);
	}

private:
	InputIt m_begin;
	InputIt m_end;
	Compare m_comp;
};

// A task for merging two sorted inputs.
template<typename InputIt, typename OutputIt, typename Compare>
class b2MergeTask : public b2Task
{
public:
	b2MergeTask() {}
	b2MergeTask(InputIt beginA, InputIt endA, InputIt beginB, InputIt endB, OutputIt output, Compare comp)
		: m_beginA(beginA)
		, m_endA(endA)
		, m_beginB(beginB)
		, m_endB(endB)
		, m_output(output)
		, m_comp(comp)
	{}

	virtual b2Task::Type GetType() const override { return b2Task::e_merge; }

	virtual void Execute(const b2ThreadContext&) override
	{
		b2Merge(m_beginA, m_endA, m_beginB, m_endB, m_output, m_comp);
	}
private:
	InputIt m_beginA;
	InputIt m_endA;
	InputIt m_beginB;
	InputIt m_endB;
	OutputIt m_output;
	Compare m_comp;
};

// A class for async sorting of per thread data.
template<typename T, typename ThreadData, typename Member, typename Compare, uint32 ThreadDataCount = b2_maxThreads>
class b2ThreadDataSorter
{
public:
	b2ThreadDataSorter() {}

	// Construct the sorter.
	// outputDoubleBuffer must be large enough to hold 2 * outputCount.
	b2ThreadDataSorter(ThreadData* threadDataArray, Member ThreadData::* members,
		T* outputDoubleBuffer, uint32 outputCount, Compare comp);

	// Submit a sorting task.
	// Note: this gives better parallelism when called on multiple sorters before waiting.
	void SubmitSortTask(b2TaskExecutor& executor, b2TaskGroup* taskGroup);

	// Have all required sort tasks been submitted?
	bool IsSubmitRequired() const
	{
		return m_sortedRangeCount > 1;
	}

	// Get the sorted output.
	T* GetSortedOutput() const
	{
		b2Assert(IsSubmitRequired() == false);
		return m_outputBuffer;
	}

	// Get the number of elements in the output.
	uint32 GetOutputCount() const
	{
		return m_outputCount;
	}

private:
	using SortTask = b2SortTask<T*, Compare>;
	using MergeTask = b2MergeTask<T*, T*, Compare>;

	// Track the phase of our work.
	enum Phase
	{
		e_threadDataSort,
		e_threadDataToBufferMerge,
		e_clearThreadData,
		e_bufferToBufferMerge
	};

	// Sort the per thread data.
	void SubmitThreadDataSort(b2TaskExecutor& executor, b2TaskGroup* taskGroup);

	// Merge the sorted per thread data into the output buffer.
	void SubmitThreadDataToBufferMerge(b2TaskExecutor& executor, b2TaskGroup* taskGroup);

	// Merge the sorted ranges within the output buffer.
	void SubmitBufferToBufferMerge(b2TaskExecutor& executor, b2TaskGroup* taskGroup);

	// Not expected to work under these conditions.
	// Create an issue or submit a pull request if you want to lift these restrictions.
	static_assert(ThreadDataCount > 1, "b2ThreadDataSorter expects ThreadDataCount to be greater than 1.");
	static_assert((ThreadDataCount & 1) == 0, "b2ThreadDataSorter expects ThreadDataCount to be even.");

	SortTask m_sortTasks[ThreadDataCount];
	MergeTask m_mergeTasks[ThreadDataCount / 2];

	T* m_sortedRanges[ThreadDataCount / 2 + 1];

	uint16 m_sortedRangeCount;
	uint16 m_nextPhase;
	uint32 m_outputCount;

	T* m_outputBuffer;
	T* m_workingBuffer;

	ThreadData* m_td;
	Member ThreadData::* m_member;
	Compare m_comp;
};

// A thread data sorter that uses b2StackAllocator for output storage.
template<typename T, typename ThreadData, typename Member, typename Compare, uint32 ThreadDataCount = b2_maxThreads>
class b2StackAllocThreadDataSorter
{
public:
	// Construct the sorter.
	// Allocate output storage using allocator.
	b2StackAllocThreadDataSorter(ThreadData* threadDataArray, Member ThreadData::* member,
		Compare comp, b2StackAllocator& allocator);

	// No copies.
	b2StackAllocThreadDataSorter(b2StackAllocThreadDataSorter&) = delete;
	b2StackAllocThreadDataSorter& operator=(b2StackAllocThreadDataSorter&) = delete;

	// Move.
	b2StackAllocThreadDataSorter(b2StackAllocThreadDataSorter&& rhs);

	// Free output storage.
	~b2StackAllocThreadDataSorter();

	// Submit a sorting task.
	// Note: this gives better parallelism when called on multiple sorters before waiting.
	void SubmitSortTask(b2TaskExecutor& executor, b2TaskGroup* taskGroup)
	{
		m_sorter.SubmitSortTask(executor, taskGroup);
	}

	// Have all required sort tasks been submitted?
	bool IsSubmitRequired() const
	{
		return m_sorter.IsSubmitRequired();
	}

	// Get the sorted output begin.
	// @warning must already be sorted.
	T* begin() const
	{
		return m_sorter.GetSortedOutput();
	}

	// Get the sorted output end.
	// @warning must already be sorted.
	T* end() const
	{
		return m_sorter.GetSortedOutput() + m_sorter.GetOutputCount();
	}

	// Get the size of the sorted output.
	uint32 size() const
	{
		return m_sorter.GetOutputCount();
	}

private:

	b2ThreadDataSorter<T, ThreadData, Member, Compare, ThreadDataCount> m_sorter;
	b2StackAllocator* m_allocator;
	void* m_mem;
};

template<typename T, typename ThreadData, typename Member, typename Compare, uint32 ThreadDataCount>
b2ThreadDataSorter<T, ThreadData, Member, Compare, ThreadDataCount>::b2ThreadDataSorter(ThreadData* td,
		Member ThreadData::* member, T* outputDoubleBuffer, uint32 outputCount, Compare comp)
	: m_sortedRangeCount(ThreadDataCount)
	, m_nextPhase(e_threadDataSort)
	, m_outputCount(outputCount)
	, m_outputBuffer(outputDoubleBuffer)
	, m_workingBuffer(outputDoubleBuffer + outputCount)
	, m_td(td)
	, m_member(member)
	, m_comp(comp)
{

}

template<typename T, typename ThreadData, typename Member, typename Compare, uint32 ThreadDataCount>
void b2ThreadDataSorter<T, ThreadData, Member, Compare, ThreadDataCount>::SubmitSortTask(
	b2TaskExecutor& executor, b2TaskGroup* taskGroup)
{
	switch(m_nextPhase)
	{
	case e_threadDataSort:
		SubmitThreadDataSort(executor, taskGroup);
		m_nextPhase = e_threadDataToBufferMerge;
		break;
	case e_threadDataToBufferMerge:
		SubmitThreadDataToBufferMerge(executor, taskGroup);
		m_nextPhase = e_clearThreadData;
		break;
	case e_clearThreadData:
		for (uint32 i = 0; i < ThreadDataCount; ++i)
		{
			(m_td[i].*m_member).clear();
		}
		m_nextPhase = e_bufferToBufferMerge;
		// C++17 TODO: [[fallthrough]];
		// The following comment prevents a warning in gcc.
		// fall through
	case e_bufferToBufferMerge:
		SubmitBufferToBufferMerge(executor, taskGroup);
		break;
	default:
		b2Assert(false);
	}
}

template<typename T, typename ThreadData, typename Member, typename Compare, uint32 ThreadDataCount>
void b2ThreadDataSorter<T, ThreadData, Member, Compare, ThreadDataCount>::SubmitThreadDataSort(
	b2TaskExecutor& executor, b2TaskGroup* taskGroup)
{
	b2Assert(m_sortedRangeCount == ThreadDataCount);

	if (m_outputCount <= 1)
	{
		return;
	}

	for (uint32 i = 0; i < ThreadDataCount; ++i)
	{
		auto& m = m_td[i].*m_member;

		m_sortTasks[i] = SortTask(m.begin(), m.end(), m_comp);
	}

	b2SubmitTasks(executor, taskGroup, m_sortTasks, ThreadDataCount);
}

template<typename T, typename ThreadData, typename Member, typename Compare, uint32 ThreadDataCount>
void b2ThreadDataSorter<T, ThreadData, Member, Compare, ThreadDataCount>::SubmitThreadDataToBufferMerge(
	b2TaskExecutor& executor, b2TaskGroup* taskGroup)
{
	b2Assert(m_sortedRangeCount == ThreadDataCount);

	if (m_outputCount == 0)
	{
		return;
	}

	// Initialize sorted ranges with the first output locations.
	m_sortedRanges[0] = m_outputBuffer;
	m_sortedRanges[ThreadDataCount / 2] = m_outputBuffer + m_outputCount;
	for (uint32 i = 2; i < ThreadDataCount; i += 2)
	{
		const auto& mA = m_td[i - 1].*m_member;
		const auto& mB = m_td[i - 2].*m_member;
		const uint32 outputIndex = i / 2;

		m_sortedRanges[outputIndex] = m_sortedRanges[outputIndex - 1] + mA.size() + mB.size();
	}

	for (uint32 i = 0; i < ThreadDataCount; i += 2)
	{
		auto& mA = m_td[i].*m_member;
		auto& mB = m_td[i + 1].*m_member;
		const uint32 outputIndex = i / 2;

		m_mergeTasks[outputIndex] = MergeTask(mA.begin(), mA.end(), mB.begin(), mB.end(), m_sortedRanges[outputIndex], m_comp);
	}
	b2SubmitTasks(executor, taskGroup, m_mergeTasks, ThreadDataCount / 2);

	m_sortedRangeCount /= 2;
}

template<typename T, typename ThreadData, typename Member, typename Compare, uint32 ThreadDataCount>
void b2ThreadDataSorter<T, ThreadData, Member, Compare, ThreadDataCount>::SubmitBufferToBufferMerge(
	b2TaskExecutor& executor, b2TaskGroup* taskGroup)
{
	if (m_outputCount <= 1)
	{
		m_sortedRangeCount = 1;
		return;
	}

	if (m_sortedRangeCount > 1)
	{
		std::swap(m_outputBuffer, m_workingBuffer);

		T* outputPtr = m_outputBuffer;
		for (uint32 i = 0; i < m_sortedRangeCount; i += 2)
		{
			const uint32 outputIndex = i / 2;

			m_mergeTasks[outputIndex] = MergeTask(m_sortedRanges[i], m_sortedRanges[i + 1],
				m_sortedRanges[i + 1], m_sortedRanges[i + 2], outputPtr, m_comp);

			uint32 count = (uint32)(m_sortedRanges[i + 2] - m_sortedRanges[i]);
			m_sortedRanges[outputIndex] = outputPtr;
			outputPtr += count;
		}
		m_sortedRangeCount /= 2;
		m_sortedRanges[m_sortedRangeCount] = m_outputBuffer + m_outputCount;

		b2SubmitTasks(executor, taskGroup, m_mergeTasks, m_sortedRangeCount);
	}
}

template<typename T, typename ThreadData, typename Member, typename Compare, uint32 ThreadDataCount>
b2StackAllocThreadDataSorter<T, ThreadData, Member, Compare, ThreadDataCount>::b2StackAllocThreadDataSorter(
	ThreadData* threadDataArray, Member ThreadData::* member, Compare comp, b2StackAllocator& allocator)
	: m_allocator(&allocator)
{
	uint32 outputCount = 0;
	for (int32 i = 0; i < b2_maxThreads; ++i)
	{
		outputCount += (threadDataArray[i].*member).size();
	}
	m_mem = allocator.Allocate(2 * outputCount * sizeof(T));

	m_sorter = b2ThreadDataSorter<T, ThreadData, Member, Compare, ThreadDataCount>(
		threadDataArray, member, (T*)m_mem, outputCount, comp);
}

template<typename T, typename ThreadData, typename Member, typename Compare, uint32 ThreadDataCount>
b2StackAllocThreadDataSorter<T, ThreadData, Member, Compare, ThreadDataCount>::b2StackAllocThreadDataSorter(
		b2StackAllocThreadDataSorter&& rhs)
	: m_sorter(rhs.m_sorter)
	, m_allocator(rhs.m_allocator)
	, m_mem(rhs.m_mem)
{
	rhs.m_mem = nullptr;
}

template<typename T, typename ThreadData, typename Member, typename Compare, uint32 ThreadDataCount>
b2StackAllocThreadDataSorter<T, ThreadData, Member, Compare, ThreadDataCount>::~b2StackAllocThreadDataSorter()
{
	if (m_mem)
	{
		m_allocator->Free(m_mem);
	}
}

// Convenience function to make a sorter with template argument deduction.
template<typename T, typename ThreadData, typename Member, typename Compare, uint32 ThreadDataCount = b2_maxThreads>
b2StackAllocThreadDataSorter<T, ThreadData, Member, Compare, ThreadDataCount>
	b2MakeStackAllocThreadDataSorter(ThreadData* threadDataArray, Member ThreadData::* member, Compare comp,
		b2StackAllocator& allocator)
{
	return b2StackAllocThreadDataSorter<T, ThreadData, Member, Compare, ThreadDataCount>(threadDataArray, member, comp, allocator);
}

// Convenience function to run all sorting tasks and wait for them to finish.
// Note: this will sacrifice parallelism if used on multiple sorters sequentially.
template<typename Sorter>
void b2Sort(Sorter& sorter, b2TaskExecutor& executor, b2TaskGroup* taskGroup, b2StackAllocator& allocator)
{
	b2Assert(sorter.IsSubmitRequired());

	while (sorter.IsSubmitRequired())
	{
		sorter.SubmitSortTask(executor, taskGroup);
		executor.Wait(taskGroup, b2MainThreadCtx(&allocator));
	}
}

#endif
