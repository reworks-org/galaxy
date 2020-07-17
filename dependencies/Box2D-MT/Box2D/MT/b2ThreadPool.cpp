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

#include <algorithm>
#include "Box2D/MT/b2ThreadPool.h"
#include "Box2D/Common/b2Math.h"
#include "Box2D/Common/b2StackAllocator.h"
#include "Box2D/Dynamics/b2TimeStep.h"
#include "Box2D/Dynamics/b2World.h"

// Prevent false positives when testing with DRD.
#ifdef b2_drd
	#include "valgrind/drd.h"
	// These prevent the "Probably a race condition" warnings that occur when
	// notifying a condition variable without the associated mutex being locked.
	#define b2_holdLockDuringNotify
	// This is used on atomic variables to prevent false positive data races that are
	// detected on conflicting atomic loads and stores (DRD can't recognize atomics).
	#define b2_drdIgnoreVar(x) DRD_IGNORE_VAR(x)
#else
	#define b2_drdIgnoreVar(x) do { } while(0)
#endif

// This define expands the scope of lock guards to include the condition variable notify.
// It isn't necessary to hold a lock during notification, and doing so can force the
// notified thread to wait for the notifier to unlock the mutex, but tools like drd issue
// a warning when a condition variable is notified without holding a lock.
#ifdef b2_holdLockDuringNotify
	#define b2_notifyLockScopeBegin
	#define b2_notifyLockScopeEnd
#else
	#define b2_notifyLockScopeBegin {
	#define b2_notifyLockScopeEnd }
#endif

// Compare the cost of two tasks. Currently only b2SolveTask sets a cost.
inline bool b2TaskCostLessThan(const b2Task* a, const b2Task* b)
{
	return a->GetCost() < b->GetCost();
}

// Add to an atomic variable without an atomic read-modify-write.
// We use this for atomics that are only modified while a lock is held.
template<typename T, typename U>
void b2NonAtomicAdd(std::atomic<T>& a, U b)
{
	T value = a.load(std::memory_order_relaxed) + b;
	a.store(value, std::memory_order_relaxed);
}

b2ThreadPoolTaskGroup::b2ThreadPoolTaskGroup(b2ThreadPool& threadPool)
{
	m_threadPool = &threadPool;
	m_remainingTasks.store(0, std::memory_order_relaxed);

	b2_drdIgnoreVar(m_remainingTasks);
}

b2ThreadPoolTaskGroup::b2ThreadPoolTaskGroup()
	: m_threadPool(nullptr)
	, m_remainingTasks(0)
{
	b2_drdIgnoreVar(m_remainingTasks);
}

b2ThreadPool::b2ThreadPool(const b2ThreadPoolOptions& options)
{
	int32 totalThreadCount = options.totalThreadCount;

	b2Assert(totalThreadCount <= b2_maxThreads);
	b2Assert(totalThreadCount >= -1);

	if (totalThreadCount == -1)
	{
		// Use the number of logical cores.
		totalThreadCount = std::thread::hardware_concurrency();
	}

	m_lockMilliseconds = 0;
	m_pendingTaskCount.store(0, std::memory_order_relaxed);
	m_busyWaitTimeout.store(options.busyWaitTimeoutMs, std::memory_order_relaxed);
	m_signalShutdown = false;

	// This prevents DRD from generating false positive data races.
	b2_drdIgnoreVar(m_pendingTaskCount);
	b2_drdIgnoreVar(m_busyWaitTimeout);

	// Minus one for the user thread.
	m_threadCount = b2Clamp(totalThreadCount - 1, 0, b2_maxThreads - 1);
	for (uint32 i = 0; i < m_threadCount; ++i)
	{
		m_threads[i] = std::thread(&b2ThreadPool::WorkerMain, this, 1 + i);
	}
}

b2ThreadPool::~b2ThreadPool()
{
	Shutdown();
}

void b2ThreadPool::SetBusyWaitTimeout(float32 busyWaitTimeoutMs)
{
	m_busyWaitTimeout.store(busyWaitTimeoutMs, std::memory_order_relaxed);
}

void b2ThreadPool::SubmitTasks(b2ThreadPoolTaskGroup& group, b2Task** tasks, uint32 count)
{
	b2_notifyLockScopeBegin
		b2Timer lockTimer;
		std::lock_guard<std::mutex> lk(m_mutex);
		m_lockMilliseconds += lockTimer.GetMilliseconds();

		for (uint32 i = 0; i < count; ++i)
		{
			m_taskHeap.push_back(tasks[i]);
			std::push_heap(m_taskHeap.begin(), m_taskHeap.end(), b2TaskCostLessThan);
		}
		b2NonAtomicAdd(m_pendingTaskCount, count);
		b2NonAtomicAdd(group.m_remainingTasks, count);
	b2_notifyLockScopeEnd
	m_waitingForTasks.notify_all();
}

void b2ThreadPool::SubmitTask(b2ThreadPoolTaskGroup& group, b2Task* task)
{
	b2_notifyLockScopeBegin
		b2Timer lockTimer;
		std::lock_guard<std::mutex> lk(m_mutex);
		m_lockMilliseconds += lockTimer.GetMilliseconds();

		m_taskHeap.push_back(task);
		std::push_heap(m_taskHeap.begin(), m_taskHeap.end(), b2TaskCostLessThan);

		b2NonAtomicAdd(m_pendingTaskCount, 1);
		b2NonAtomicAdd(group.m_remainingTasks, 1);
	b2_notifyLockScopeEnd
	m_waitingForTasks.notify_one();
}

void b2ThreadPool::Wait(const b2ThreadPoolTaskGroup& group, const b2ThreadContext& context)
{
	// We don't expect worker threads to call wait.
	b2Assert(context.threadId == 0);

	b2Timer lockTimer;
	std::unique_lock<std::mutex> lk(m_mutex);
	m_lockMilliseconds += lockTimer.GetMilliseconds();

	while (true)
	{
		if (group.m_remainingTasks.load(std::memory_order_relaxed) == 0)
		{
			return;
		}

		if (m_pendingTaskCount.load(std::memory_order_relaxed) == 0)
		{
			lk.unlock();
			// Busy wait.
			while (group.m_remainingTasks.load(std::memory_order_relaxed) > 0)
			{
				std::this_thread::yield();
			}
			lockTimer.Reset();
			lk.lock();
			m_lockMilliseconds += lockTimer.GetMilliseconds();
			return;
		}

		// Execute a task while waiting.
		b2Task* task = PopTask();

		lk.unlock();

		task->Execute(context);

		lockTimer.Reset();
		lk.lock();
		m_lockMilliseconds += lockTimer.GetMilliseconds();

		// This isn't necessarily the group we're waiting on.
		b2ThreadPoolTaskGroup* executeGroup = static_cast<b2ThreadPoolTaskGroup*>(task->GetTaskGroup());
		b2NonAtomicAdd(executeGroup->m_remainingTasks, -1);
	}
}

void b2ThreadPool::Restart(uint32 threadCount)
{
	// Save and restore the busy wait timeout.
	float32 busyWaitTimeout = m_busyWaitTimeout.load(std::memory_order_relaxed);

	Shutdown();
	m_signalShutdown = false;

	m_busyWaitTimeout.store(busyWaitTimeout, std::memory_order_relaxed);

	m_threadCount = b2Clamp((int32)threadCount - 1, 0, b2_maxThreads - 1);
	for (uint32 i = 0; i < m_threadCount; ++i)
	{
		m_threads[i] = std::thread(&b2ThreadPool::WorkerMain, this, 1 + i);
	}
}

b2Task* b2ThreadPool::PopTask()
{
	b2Task* task = nullptr;

	std::pop_heap(m_taskHeap.begin(), m_taskHeap.end(), b2TaskCostLessThan);
	task = m_taskHeap.pop_back();

	b2NonAtomicAdd(m_pendingTaskCount, -1);

	return task;
}

void b2ThreadPool::WorkerMain(uint32 threadId)
{
	b2StackAllocator stack;

	b2ThreadContext context;
	context.stack = &stack;
	context.threadId = threadId;

	b2Timer lockTimer;
	std::unique_lock<std::mutex> lk(m_mutex);

	while (true)
	{
		b2Timer waitTimer;
		while (m_pendingTaskCount.load(std::memory_order_relaxed) == 0)
		{
			// Busy wait.
			bool timedOut = false;
			lk.unlock();
			while (m_pendingTaskCount.load(std::memory_order_relaxed) == 0)
			{
				timedOut = waitTimer.GetMilliseconds() > m_busyWaitTimeout.load(std::memory_order_relaxed);
				if (timedOut)
				{
					break;
				}
				std::this_thread::yield();
			}
			lockTimer.Reset();
			lk.lock();
			m_lockMilliseconds += lockTimer.GetMilliseconds();

			if (timedOut == false)
			{
				// We saw a pending task while busy waiting but we need to check again now that we're holding the lock.
				continue;
			}

			m_waitingForTasks.wait(lk, [this]()
			{
				if (m_pendingTaskCount.load(std::memory_order_relaxed) > 0)
				{
					return true;
				}
				if (m_signalShutdown)
				{
					return true;
				}
				return false;
			});

			if (m_signalShutdown)
			{
				// Shutting down in the middle of processing tasks is not supported.
				b2Assert(m_pendingTaskCount.load(std::memory_order_relaxed) == 0);
				return;
			}
		}

		b2Task* task = PopTask();
		b2ThreadPoolTaskGroup* group = static_cast<b2ThreadPoolTaskGroup*>(task->GetTaskGroup());

		lk.unlock();

		task->Execute(context);

		lockTimer.Reset();
		lk.lock();
		m_lockMilliseconds += lockTimer.GetMilliseconds();

		b2NonAtomicAdd(group->m_remainingTasks, -1);
	}
}

void b2ThreadPool::Shutdown()
{
	{
		b2_notifyLockScopeBegin
			std::lock_guard<std::mutex> lk(m_mutex);
			m_signalShutdown = true;
			m_busyWaitTimeout.store(0, std::memory_order_relaxed);
		b2_notifyLockScopeEnd
		m_waitingForTasks.notify_all();
	}

	for (uint32 i = 0; i < m_threadCount; ++i)
	{
		if (m_threads[i].joinable())
		{
			m_threads[i].join();
		}
	}
}

b2TaskGroup* b2ThreadPoolTaskExecutor::AcquireTaskGroup()
{
	for (uint32 i = 0; i < b2_maxWorldStepTaskGroups; ++i)
	{
		if (m_taskGroupInUse[i] == false)
		{
			m_taskGroupInUse[i] = true;
			return m_taskGroups + i;
		}
	}

	// No task groups available.
	// Did you forget to release a task group?
	// Or are you stepping worlds from multiple threads with the same executor?
	// You can implement b2TaskExecutor if you need support for that.
	b2Assert(false);
	return nullptr;
}

void b2ThreadPoolTaskExecutor::ReleaseTaskGroup(b2TaskGroup* taskGroup)
{
	for (uint32 i = 0; i < b2_maxWorldStepTaskGroups; ++i)
	{
		b2ThreadPoolTaskGroup* tpTaskGroup = m_taskGroups + i;
		if (tpTaskGroup == taskGroup)
		{
			b2Assert(tpTaskGroup->m_remainingTasks.load(std::memory_order_relaxed) == 0);

			m_taskGroupInUse[i] = false;
			return;
		}
	}

	// Task group not found.
	b2Assert(false);
}

void b2ThreadPoolTaskExecutor::PartitionRange(b2Task::Type type, uint32 begin, uint32 end, b2PartitionedRange& output)
{
	static_assert(b2_maxThreads <= b2_maxRangeSubTasks, "Increase b2_maxRangeSubTasks.");
	b2Assert(b2IsRangeTask(type) || type >= b2Task::e_userTask);
	b2Assert(type < b2Task::e_rangeTypeCount || type >= b2Task::e_userTask);

	uint32 maxSubTasks = m_threadPool.GetThreadCount();
	uint32 itemsPerTask = 1;

	b2PartitionRange(begin, end, maxSubTasks, itemsPerTask, output);
}

void b2ThreadPoolTaskExecutor::SubmitTask(b2TaskGroup* taskGroup, b2Task* task)
{
	b2ThreadPoolTaskGroup* tpTaskGroup = static_cast<b2ThreadPoolTaskGroup*>(taskGroup);
	b2Assert(tpTaskGroup);

	m_threadPool.SubmitTask(*tpTaskGroup, task);
}

void b2ThreadPoolTaskExecutor::SubmitTasks(b2TaskGroup* taskGroup, b2Task** tasks, uint32 count)
{
	b2ThreadPoolTaskGroup* tpTaskGroup = static_cast<b2ThreadPoolTaskGroup*>(taskGroup);
	b2Assert(tpTaskGroup);

	m_threadPool.SubmitTasks(*tpTaskGroup, tasks, count);
}

void b2ThreadPoolTaskExecutor::Wait(b2TaskGroup* taskGroup, const b2ThreadContext& ctx)
{
	b2ThreadPoolTaskGroup* tpTaskGroup = static_cast<b2ThreadPoolTaskGroup*>(taskGroup);
	b2Assert(tpTaskGroup);

	m_threadPool.Wait(*tpTaskGroup, ctx);
}