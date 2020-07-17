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

#include "Box2D/Dynamics/b2TimeStep.h"
#include "Box2D/MT/b2MtUtil.h"

void b2PartitionRange(uint32 begin, uint32 end, uint32 maxOutputRanges, uint32 minElementsPerRange, b2PartitionedRange& output)
{
	b2Assert(maxOutputRanges <= b2_maxRangeSubTasks);
	b2Assert(begin < end);

	uint32 elementCount = end - begin;

	if (elementCount <= minElementsPerRange)
	{
		b2Assert(output.count < b2_maxRangeSubTasks);
		output[output.count].begin = begin;
		output[output.count].end = end;
		output.count += 1;
		return;
	}

	uint32 minElementRangeCount = elementCount / minElementsPerRange;
	if (minElementRangeCount < maxOutputRanges)
	{
		maxOutputRanges = minElementRangeCount;
	}

	uint32 elementsPerTask = elementCount / maxOutputRanges;
	uint32 elementsRemainder = elementCount % maxOutputRanges;

	uint32 beginIndex = begin;
	uint32 endIndex = 0;
	for (uint32 i = 0; i < maxOutputRanges; ++i)
	{
		uint32 rangeSize = elementsPerTask;
		if (i < elementsRemainder)
		{
			rangeSize += 1;
		}
		endIndex = beginIndex + rangeSize;
		if (endIndex > end)
		{
			endIndex = end;
		}
		b2Assert(output.count < b2_maxRangeSubTasks);
		output[output.count].begin = beginIndex;
		output[output.count].end = endIndex;
		output.count += 1;
		if (endIndex == end)
		{
			break;
		}
		beginIndex = endIndex;
	}
}
