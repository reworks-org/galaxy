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

#ifndef B2_TASK_H
#define B2_TASK_H

#include "Box2D/Common/b2Settings.h"

class b2StackAllocator;

/// Thread data required for task execution.
struct b2ThreadContext
{
	b2StackAllocator* stack;
	uint32 threadId;
};

/// Represents a group of tasks. Implemented by executors if needed.
class b2TaskGroup { };

/// The base class for all tasks given to executors.
class b2Task
{
public:
	enum Type
	{
		// Range tasks.
		e_broadPhaseFindContacts = 0,
		e_broadPhaseSyncFixtures,
		e_clearContactSolveFlags,
		e_clearContactSolveToiFlags,
		e_clearBodySolveFlags,
		e_clearBodySolveToiFlags,
		e_clearForces,
		e_collide,
		e_findMinToiContact,
		e_raycast,
		e_queryAABB,

		e_rangeTypeCount,

		// Non-range tasks.
		e_solve = e_rangeTypeCount,
		e_sortThreadDataTask,
		e_broadPhaseUpdateSubTree,

		// Number of tasks defined by Box2D-MT
		e_typeCount,

		// The starting value for any user-defined tasks. Used by the testbed.
		e_userTask = e_typeCount
	};

	/// Construct a task.
	b2Task()
		: m_costEstimate(0)
		, m_taskGroup(nullptr)
	{ }

	/// Execute the task.
	virtual void Execute(const b2ThreadContext& ctx) = 0;

	/// Get the task type.
	virtual Type GetType() const { return e_userTask; };

	virtual ~b2Task() {}

	/// Set the estimated cost of executing the task so the executor can prioritize higher cost tasks.
	void SetCost(uint32 costEstimate);

	/// Get the estimated cost of executing the task.
	uint32 GetCost() const;

	/// Associate this task with a task group.
	void SetTaskGroup(b2TaskGroup* taskGroup);

	/// Get the group that this task is associated with.
	b2TaskGroup* GetTaskGroup() const;

private:
	uint32 m_costEstimate;
	b2TaskGroup* m_taskGroup;
};

/// Is the type a range task?
inline bool b2IsRangeTask(b2Task::Type type)
{
	return type < b2Task::e_rangeTypeCount;
}

/// A range over which a range task executes.
struct b2RangeTaskRange
{
	b2RangeTaskRange()
		: begin(0)
		, end(0)
	{ }

	b2RangeTaskRange(uint32 beginIn, uint32 endIn)
		: begin(beginIn)
		, end(endIn)
	{ }

	uint32 GetCount() const { return end - begin; }

	uint32 begin;
	uint32 end;
};

/// A set of sequential ranges.
struct b2PartitionedRange
{
	b2PartitionedRange()
		: count(0)
	{ }

	b2RangeTaskRange& operator[](size_t i);
	const b2RangeTaskRange& operator[](size_t i) const;

	b2RangeTaskRange ranges[b2_maxRangeSubTasks];
	uint32 count;
};

/// The base class for tasks that operate on a range of items.
class b2RangeTask : public b2Task
{
public:
	/// Construct a range task.
	b2RangeTask() { }
	b2RangeTask(const b2RangeTaskRange& range)
		: m_range{range}
	{}

	/// Execute the task over the specified range.
	/// Custom executors can use this to execute the task via something like
	/// parallel_for instead of implementing b2TaskExecutor::PartitionRange.
	virtual void Execute(const b2ThreadContext& ctx, const b2RangeTaskRange& range) = 0;

	/// Execute the task over the stored range.
	virtual void Execute(const b2ThreadContext& ctx) final;

	/// Get/set the range.
	const b2RangeTaskRange& GetRange() const;
	void SetRange(const b2RangeTaskRange& range);

protected:
	b2RangeTaskRange m_range;
};

/// Evenly divides the range [begin, end) into sub ranges.
void b2PartitionRange(uint32 begin, uint32 end, uint32 maxOutputRanges, uint32 minElementsPerRange, b2PartitionedRange& output);

inline void b2Task::SetCost(uint32 costEstimate)
{
	m_costEstimate = costEstimate;
}

inline uint32 b2Task::GetCost() const
{
	return m_costEstimate;
}

inline void b2Task::SetTaskGroup(b2TaskGroup* taskGroup)
{
	m_taskGroup = taskGroup;
}

inline b2TaskGroup* b2Task::GetTaskGroup() const
{
	return m_taskGroup;
}

inline b2RangeTaskRange& b2PartitionedRange::operator[](size_t i)
{
	return ranges[i];
}

inline const b2RangeTaskRange& b2PartitionedRange::operator[](size_t i) const
{
	return ranges[i];
}

inline void b2RangeTask::Execute(const b2ThreadContext& ctx)
{
	Execute(ctx, GetRange());
}

inline const b2RangeTaskRange& b2RangeTask::GetRange() const
{
	return m_range;
}

inline void b2RangeTask::SetRange(const b2RangeTaskRange& range)
{
	m_range = range;
}

#endif
