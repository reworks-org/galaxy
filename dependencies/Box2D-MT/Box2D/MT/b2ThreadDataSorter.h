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
#include <functional>
#include <atomic>

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

// A task for sorting and merging per-thread data.
template<typename T, typename Compare>
class b2SortThreadDataTask : public b2Task
{
public:
	struct Work
	{
		struct alignas(b2_cacheLineSize) PerThreadData
		{
			b2GrowableArray<T>* input;
			std::atomic<uint32> counter;
			uint32 outputIndex;
			uint32 outputCount;
		};
		PerThreadData perThreadData[b2_maxThreads];
		T* outputs[2];
		b2TaskExecutor* executor;
		b2TaskGroup* taskGroup;
		uint32 taskCost;
		std::atomic<uint32> mergeCounter;
		alignas(b2_cacheLineSize) Compare comp;
	};

	b2SortThreadDataTask() {}
	b2SortThreadDataTask(Work& work, uint32 index)
		: m_work(&work)
		, m_index(index)
	{}

	virtual b2Task::Type GetType() const override { return b2Task::e_sortThreadDataTask; }

	virtual void Execute(const b2ThreadContext& ctx) override
	{
		// Sort the input per-thread data arrays.
		SortInputs();

		if ((m_index & 1) != 0)
		{
			m_index -= 1;
		}

		if (b2FetchAdd(m_work->perThreadData[m_index].counter, 1) == 0)
		{
			// The neighboring thread will run MergeFromInputs.
			return;
		}

		// Merge pairs of sorted input arrays into half of the output buffer.
		MergeFromInputs();

		// The final thread to finish MergeFromInputs submits subsequent merges.
		if (b2FetchAdd(m_work->mergeCounter, -1) == 1)
		{
			// Merge pairs of sorted ranges from one half of the output buffer
			// to the other half, until all sorted ranges have been merged.
			MergeOutputs(ctx);
		}
	}

private:
	class MergeOutputsTask : public b2Task
	{
	public:
		MergeOutputsTask() {}
		MergeOutputsTask(Work& work, uint32 indexA, uint32 indexB)
			: m_work(&work)
			, m_indexA(indexA)
			, m_indexB(indexB)
		{}

		virtual b2Task::Type GetType() const override { return b2Task::e_sortThreadDataTask; }

		virtual void Execute(const b2ThreadContext&) override
		{
			typename Work::PerThreadData& tdA = m_work->perThreadData[m_indexA];
			typename Work::PerThreadData& tdB = m_work->perThreadData[m_indexB];

			const T* beginA = m_work->outputs[0] + tdA.outputIndex;
			const T* beginB = m_work->outputs[0] + tdB.outputIndex;
			const T* endA = beginA + tdA.outputCount;
			const T* endB = beginB + tdB.outputCount;

			T* output = m_work->outputs[1] + tdA.outputIndex;

			b2Merge(beginA, endA, beginB, endB, output, m_work->comp);

			tdA.outputCount += tdB.outputCount;
		}

		Work* m_work;
		uint32 m_indexA;
		uint32 m_indexB;
	};

	void SortInputs()
	{
		typename Work::PerThreadData& td = m_work->perThreadData[m_index];
		b2GrowableArray<T>& input = *td.input;

		std::sort(input.begin(), input.end(), m_work->comp);

		td.outputCount = input.size();
	}

	void MergeFromInputs()
	{
		typename Work::PerThreadData& tdA = m_work->perThreadData[m_index];
		typename Work::PerThreadData& tdB = m_work->perThreadData[m_index + 1];

		b2GrowableArray<T>& inputA = *tdA.input;
		b2GrowableArray<T>& inputB = *tdB.input;

		T* output = m_work->outputs[0] + tdA.outputIndex;

		b2Merge(inputA.begin(), inputA.end(), inputB.begin(), inputB.end(), output, m_work->comp);

		tdA.outputCount += tdB.outputCount;

		inputA.clear();
		inputB.clear();
	}

	void MergeOutputs(const b2ThreadContext& ctx)
	{
		MergeOutputsTask tasks[b2_maxThreads];

		b2TaskExecutor& executor = *m_work->executor;
		b2TaskGroup* taskGroup = m_work->taskGroup;
		uint32 taskCost = m_work->taskCost;

		for (uint32 stride = 2; stride < b2_maxThreads; stride *= 2)
		{
			uint32 taskCount = 0;
			for (uint32 i = 0; i + stride < b2_maxThreads; i += 2 * stride)
			{
				tasks[taskCount] = MergeOutputsTask(*m_work, i, i + stride);
				tasks[taskCount].SetCost(taskCost);
				++taskCount;
			}

			if (taskCount > 1)
			{
				b2SubmitTasks(executor, taskGroup, tasks, taskCount - 1);
				tasks[taskCount - 1].Execute(ctx);
				executor.Wait(taskGroup, ctx);
			}
			else
			{
				b2Assert(taskCount == 1);
				tasks[0].Execute(ctx);
			}

			std::swap(m_work->outputs[0], m_work->outputs[1]);
		}
	}

	Work* m_work;
	uint32 m_index;
};

// This class asynchronously sorts per-thread data and provides access to the output.
template<typename T, typename ThreadData, typename Member, typename Compare>
class b2ThreadDataSorter
{
public:
	/// Construct the sorter and submit the sorting tasks.
	/// @param executor runs the sorting tasks
	/// @param allocator used for allocating output storage
	/// @param threadDataArray the input thread data array of size b2_maxThreads
	/// @param member pointer to a member of ThreadData
	/// @param comp comparison function object equivalent to "bool comp(const T& a, const T& b)"
	/// @param taskCost the cost given to tasks (tasks from sorters with higher cost may execute first)
	b2ThreadDataSorter(b2TaskExecutor& executor, b2StackAllocator& allocator,
		ThreadData threadDataArray, Member member, Compare comp, uint32 taskCost = 0);

	/// No copies.
	b2ThreadDataSorter(b2ThreadDataSorter&) = delete;
	b2ThreadDataSorter& operator=(b2ThreadDataSorter&) = delete;

	/// Free output storage.
	~b2ThreadDataSorter();

	/// Wait for sorting tasks to finish.
	void wait();

	/// Get the sorted output beginning.
	/// @warning only valid after wait is called.
	const T* begin() const;
	T* begin();

	/// Get the sorted output end.
	/// @warning only valid after wait is called.
	const T* end() const;
	T* end();

	/// Get the size of the sorted output.
	/// @warning only valid after wait is called.
	uint32 size() const;

private:
	bool is_empty() const;

	typename b2SortThreadDataTask<T, Compare>::Work m_work;
	b2SortThreadDataTask<T, Compare> m_tasks[b2_maxThreads];
	b2TaskGroup* m_taskGroup;
	b2StackAllocator* m_allocator;
	void* m_mem;
};

template<typename T, typename ThreadData, typename Member, typename Compare>
b2ThreadDataSorter<T, ThreadData, Member, Compare>::b2ThreadDataSorter(
	b2TaskExecutor& executor, b2StackAllocator& allocator,
	ThreadData threadDataArray, Member member, Compare comp, uint32 taskCost)
: m_allocator(&allocator)
, m_mem(nullptr)
{
	uint32 outputCount = 0;
	for (int32 i = 0; i < b2_maxThreads; ++i)
	{
		m_work.perThreadData[i].outputIndex = outputCount;
		m_work.perThreadData[i].outputCount = (threadDataArray[i].*member).size();
		outputCount += m_work.perThreadData[i].outputCount;
	}

	if (outputCount == 0)
	{
		return;
	}

	m_mem = allocator.Allocate(2 * outputCount * sizeof(T));
	m_work.outputs[0] = (T*)m_mem;
	m_work.outputs[1] = (T*)m_mem + outputCount;

	for (uint32 i = 0; i < b2_maxThreads; ++i)
	{
		m_work.perThreadData[i].input = &(threadDataArray[i].*member);
		m_work.perThreadData[i].counter.store(0, std::memory_order_relaxed);

		m_tasks[i] = b2SortThreadDataTask<T, Compare>(m_work, i);
		m_tasks[i].SetCost(taskCost);

		b2_drdIgnoreVar(m_work.perThreadData[i].counter);
	}
	b2_drdIgnoreVar(m_work.mergeCounter);

	m_taskGroup = executor.AcquireTaskGroup();

	m_work.executor = &executor;
	m_work.taskGroup = executor.AcquireTaskGroup();
	uint32 mergeCounter = b2_maxThreads / 2;
	m_work.mergeCounter.store(mergeCounter, std::memory_order_relaxed);
	m_work.comp = comp;
	m_work.taskCost = taskCost;

	b2SubmitTasks(executor, m_taskGroup, m_tasks, b2_maxThreads);
}

template<typename T, typename ThreadData, typename Member, typename Compare>
b2ThreadDataSorter<T, ThreadData, Member, Compare>::~b2ThreadDataSorter()
{
	if (is_empty() == false)
	{
		m_work.executor->ReleaseTaskGroup(m_work.taskGroup);
		m_work.executor->ReleaseTaskGroup(m_taskGroup);
		m_allocator->Free(m_mem);
	}
}

template<typename T, typename ThreadData, typename Member, typename Compare>
void b2ThreadDataSorter<T, ThreadData, Member, Compare>::wait()
{
	if (is_empty() == false)
	{
		m_work.executor->Wait(m_taskGroup, b2MainThreadCtx(m_allocator));
	}
}

template<typename T, typename ThreadData, typename Member, typename Compare>
const T* b2ThreadDataSorter<T, ThreadData, Member, Compare>::begin() const
{
	return m_work.outputs[0];
}

template<typename T, typename ThreadData, typename Member, typename Compare>
T* b2ThreadDataSorter<T, ThreadData, Member, Compare>::begin()
{
	return m_work.outputs[0];
}

template<typename T, typename ThreadData, typename Member, typename Compare>
const T* b2ThreadDataSorter<T, ThreadData, Member, Compare>::end() const
{
	return m_work.outputs[0] + m_work.perThreadData[0].outputCount;
}

template<typename T, typename ThreadData, typename Member, typename Compare>
T* b2ThreadDataSorter<T, ThreadData, Member, Compare>::end()
{
	return m_work.outputs[0] + m_work.perThreadData[0].outputCount;
}

template<typename T, typename ThreadData, typename Member, typename Compare>
uint32 b2ThreadDataSorter<T, ThreadData, Member, Compare>::size() const
{
	return m_work.perThreadData[0].outputCount;
}

template<typename T, typename ThreadData, typename Member, typename Compare>
bool b2ThreadDataSorter<T, ThreadData, Member, Compare>::is_empty() const
{
	return m_mem == nullptr;
}

// Convenience macro to avoid verbose template arguments.
#define b2_threadDataSorter(var, T, cost, executor, allocator, threadDataArray, member, comp)\
b2ThreadDataSorter<T, decltype(threadDataArray), decltype(member), decltype(comp)> var(\
	 executor, allocator, threadDataArray, member, comp, cost)

#endif
