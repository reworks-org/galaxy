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

#ifndef B2_THREAD_POOL_H
#define B2_THREAD_POOL_H

#include "Box2D/Common/b2GrowableArray.h"
#include "Box2D/Common/b2Timer.h"
#include "Box2D/MT/b2MtUtil.h"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

class b2ThreadPool;

/// Options for constructing a thread pool.
struct b2ThreadPoolOptions
{
	b2ThreadPoolOptions()
	{
		totalThreadCount = -1;
		busyWaitTimeoutMs = 0.03f;
	}

	/// The number of threads to make available for execution. This includes
	/// the user thread, so the thread pool will allocate one less thread.
	/// -1 is interpreted as the number of logical cores.
	int32 totalThreadCount;

	/// The number of milliseconds that a worker thread will busy wait before
	/// waiting on a condition variable.
	float32 busyWaitTimeoutMs;
};

/// A task group is used to wait for completion of a group of tasks.
class b2ThreadPoolTaskGroup : public b2TaskGroup
{
public:
	explicit b2ThreadPoolTaskGroup(b2ThreadPool& threadPool);

	~b2ThreadPoolTaskGroup();

private:
	b2ThreadPoolTaskGroup();

	friend class b2ThreadPool;
	friend class b2ThreadPoolTaskExecutor;

	b2ThreadPool* m_threadPool;
	std::atomic<uint32> m_remainingTasks;
};

/// This provides lower latency locking than libstdc++'s std::mutex (need to test others)
/// when there is contention on a lock that is not typically held for a long time.
class b2LowerLatencyLock
{
public:
	b2LowerLatencyLock();
	void lock();
	void unlock();
	bool try_lock();
private:
	// This is used to prevent more than one thread from spinning for an extended time.
	std::mutex m_mutex;

	// This is used as a spin lock.
	std::atomic_flag m_lock;
};

/// Executes tasks on worker threads.
class b2ThreadPool
{
public:
	/// Construct a thread pool.
	explicit b2ThreadPool(const b2ThreadPoolOptions& options);

	/// Join with workers.
	~b2ThreadPool();

	/// Set the amount of time in milliseconds that workers will busy wait for tasks before
	/// waiting on a condition variable.
	void SetBusyWaitTimeout(float32 busyWaitTimeoutMs);

	/// Submit a single task for execution.
	/// Returns immediately after submission.
	void SubmitTask(b2ThreadPoolTaskGroup& group, b2Task* task);

	/// Submit multiple tasks for execution.
	/// Returns immediately after submission.
	void SubmitTasks(b2ThreadPoolTaskGroup& group, b2Task** tasks, uint32 count);

	/// Wait for all tasks in the group to finish.
	/// The allocator is used to execute tasks while waiting.
	void Wait(const b2ThreadPoolTaskGroup& group, const b2ThreadContext& ctx);

	/// The number of threads available to execute tasks. This is the number of threads in the pool,
	/// plus one for the additional thread required to submit tasks and wait on them.
	int32 GetThreadCount() const;

	/// Time spent waiting to lock the mutex.
	/// @warning must only be called from a single thread while no tasks are being executed.
	float32 GetLockMilliseconds() const;

	/// Reset the lock timer.
	/// @warning must only be called from a single thread while no tasks are being executed.
	void ResetTimers();

	/// Restart with the specified number of threads
	/// @warning must only be called from a single thread while no tasks are being executed.
	void Restart(uint32 threadCount);

private:
	using Mutex = b2LowerLatencyLock;

	b2Task* PopTask();
	void WorkerMain(uint32 threadId);
	void Shutdown();

	std::thread m_threads[b2_maxThreads - 1];
	uint32 m_threadCount;

	std::atomic<float32> m_busyWaitTimeout;
	std::condition_variable_any m_waitingForTasks;

	mutable Mutex m_mutex;
	std::atomic<int32> m_pendingTaskCount;
	float32 m_lockMilliseconds;

	// A heap of tasks sorted by cost.
	b2GrowableArray<b2Task*> m_taskHeap;

	bool m_signalShutdown;
};

/// A task executor that uses b2ThreadPool.
/// @warning this class must not be accessed by multiple threads simultaneously.
class b2ThreadPoolTaskExecutor : public b2TaskExecutor
{
public:
	/// Construct a thread pool task executor.
	explicit b2ThreadPoolTaskExecutor(const b2ThreadPoolOptions& options = b2ThreadPoolOptions());

	/// Get the thread pool.
	b2ThreadPool* GetThreadPool();
	const b2ThreadPool* GetThreadPool() const;

	/// Get the number of threads available for execution.
	uint32 GetThreadCount() const override;

	/// Acquire a task group.
	b2TaskGroup* AcquireTaskGroup() override;

	/// Release the task group.
	void ReleaseTaskGroup(b2TaskGroup* taskGroup) override;

	/// Partition a range into sub-ranges that will each be assigned to a range task.
	void PartitionRange(b2Task::Type type, uint32 begin, uint32 end, b2PartitionedRange& output) override;

	/// Submit a single task for execution.
	void SubmitTask(b2TaskGroup* taskGroup, b2Task* task) override;

	/// Submit multiple tasks for execution.
	void SubmitTasks(b2TaskGroup* taskGroup, b2Task** tasks, uint32 count) override;

	/// Wait for all tasks in the group to finish.
	void Wait(b2TaskGroup* taskGroup, const b2ThreadContext& ctx) override;

private:
	b2ThreadPool m_threadPool;
	b2ThreadPoolTaskGroup m_taskGroups[b2_maxWorldStepTaskGroups];
	uint32 m_taskGroupCount;
#ifdef b2DEBUG
	bool m_taskGroupInUse[b2_maxWorldStepTaskGroups];
#endif
};

inline b2ThreadPoolTaskGroup::~b2ThreadPoolTaskGroup()
{
	// If any tasks were submitted, Wait must be called before the task group is destroyed.
	b2Assert(m_remainingTasks.load(std::memory_order_relaxed) == 0);
}

inline int32 b2ThreadPool::GetThreadCount() const
{
	return m_threadCount + 1;
}

inline float32 b2ThreadPool::GetLockMilliseconds() const
{
	std::lock_guard<Mutex> lk(m_mutex);
	return m_lockMilliseconds;
}

inline void b2ThreadPool::ResetTimers()
{
	std::lock_guard<Mutex> lk(m_mutex);
	m_lockMilliseconds = 0;
}

inline b2ThreadPoolTaskExecutor::b2ThreadPoolTaskExecutor(const b2ThreadPoolOptions& options)
	: m_threadPool(options)
	, m_taskGroupCount(0)
{
	for (uint32 i = 0; i < b2_maxWorldStepTaskGroups; ++i)
	{
		m_taskGroups[i].m_threadPool = &m_threadPool;
#ifdef b2DEBUG
		m_taskGroupInUse[i] = false;
#endif
	}
}

inline b2ThreadPool* b2ThreadPoolTaskExecutor::GetThreadPool()
{
	return &m_threadPool;
}

inline const b2ThreadPool* b2ThreadPoolTaskExecutor::GetThreadPool() const
{
	return &m_threadPool;
}

inline uint32 b2ThreadPoolTaskExecutor::GetThreadCount() const
{
	return m_threadPool.GetThreadCount();
}

#endif
