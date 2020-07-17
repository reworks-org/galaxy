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

#ifndef B2_MT_UTIL_H
#define B2_MT_UTIL_H

#include "Box2D/Common/b2StackAllocator.h"
#include "Box2D/MT/b2Task.h"
#include "Box2D/MT/b2TaskExecutor.h"

// Submit a task to an executor.
inline void b2SubmitTask(b2TaskExecutor& executor, b2TaskGroup* taskGroup, b2Task* task)
{
	task->SetTaskGroup(taskGroup);
	executor.SubmitTask(taskGroup, task);
}

// Submit an array of tasks to an executor.
template<typename TaskType>
inline void b2SubmitTasks(b2TaskExecutor& executor, b2TaskGroup* taskGroup, TaskType* tasks, uint32 count)
{
	b2Task* taskPtrs[b2_maxRangeSubTasks];
	for (uint32 i = 0; i < count; ++i)
	{
		taskPtrs[i] = tasks + i;
		taskPtrs[i]->SetTaskGroup(taskGroup);
	}
	executor.SubmitTasks(taskGroup, taskPtrs, count);
}

// Initialize a thread context for the user thread.
inline b2ThreadContext b2MainThreadCtx(b2StackAllocator* stackAllocator)
{
	b2ThreadContext threadCtx;
	threadCtx.stack = stackAllocator;
	threadCtx.threadId = 0;
	return threadCtx;
}

// Convenience function for executing a user range task.
template<typename RangeTaskType, typename... Args>
inline void b2ExecuteRangeTask(b2TaskExecutor& executor, RangeTaskType& task)
{
	b2TaskGroup* taskGroup = executor.AcquireTaskGroup();
	RangeTaskType tasks[b2_maxRangeSubTasks];
	b2RangeTaskRange r = task.GetRange();
	b2PartitionedRange ranges;
	executor.PartitionRange(task.GetType(), r.begin, r.end, ranges);
	for (uint32 i = 0; i < ranges.count; ++i)
	{
		tasks[i] = task;
		tasks[i].SetRange(ranges[i]);
	}
	b2SubmitTasks(executor, taskGroup, tasks, ranges.count);
	executor.Wait(taskGroup, b2MainThreadCtx(nullptr));
	executor.ReleaseTaskGroup(taskGroup);
}

// Replace an element in a vector
template<typename Vector>
void b2RemoveAndSwapBack(Vector& a, uint32 index)
{
	a[index] = a.pop_back();
}

#endif
