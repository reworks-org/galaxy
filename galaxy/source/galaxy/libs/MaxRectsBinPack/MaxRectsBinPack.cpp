/** 
	@file MaxRectsBinPack.cpp
	@author Jukka Jylänki

	@brief Implements different bin packer algorithms that use the MAXRECTS data structure.

	This work is released to Public Domain, do whatever you want with it.
*/

#include <cmath>
#include <limits>
#include <cassert>
#include <cstring>
#include <utility>
#include <iostream>
#include <algorithm>

#include "MaxRectsBinPack.hpp"

namespace rbp 
{
	MaxRectsBinPack::MaxRectsBinPack()
		:binWidth(0),
		binHeight(0),
		allowRotations(false)
	{
	}

	MaxRectsBinPack::MaxRectsBinPack(int width, int height, bool rotations)
	{
		Init(width, height, rotations);
	}

	void MaxRectsBinPack::Init(int width, int height, bool rotations)
	{
		binWidth = width;
		binHeight = height;
		allowRotations = rotations;

		galaxy::Rect<int> n;
		n.m_x = 0;
		n.m_y = 0;
		n.m_width = width;
		n.m_height = height;

		usedRectangles.clear();

		freeRectangles.clear();
		freeRectangles.push_back(n);
	}

	galaxy::Rect<int> MaxRectsBinPack::Insert(int width, int height, FreeRectChoiceHeuristic method)
	{
		galaxy::Rect<int> newNode;
		// Unused in this function. We don't need to know the score after finding the position.
		int score1 = std::numeric_limits<int>::max();
		int score2 = std::numeric_limits<int>::max();
		switch (method)
		{
		case RectBestShortSideFit: newNode = FindPositionForNewNodeBestShortSideFit(width, height, score1, score2); break;
		case RectBottomLeftRule: newNode = FindPositionForNewNodeBottomLeft(width, height, score1, score2); break;
		case RectContactPointRule: newNode = FindPositionForNewNodeContactPoint(width, height, score1); break;
		case RectBestLongSideFit: newNode = FindPositionForNewNodeBestLongSideFit(width, height, score2, score1); break;
		case RectBestAreaFit: newNode = FindPositionForNewNodeBestAreaFit(width, height, score1, score2); break;
		}

		if (newNode.m_height == 0)
			return newNode;

		size_t numRectanglesToProcess = freeRectangles.size();
		for (size_t i = 0; i < numRectanglesToProcess; ++i)
		{
			if (SplitFreeNode(freeRectangles[i], newNode))
			{
				freeRectangles.erase(freeRectangles.begin() + i);
				--i;
				--numRectanglesToProcess;
			}
		}

		PruneFreeList();

		usedRectangles.push_back(newNode);
		return newNode;
	}

	void MaxRectsBinPack::Insert(std::vector<galaxy::Vector2<int>> &rects, std::vector<galaxy::Rect<int>> &dst, FreeRectChoiceHeuristic method)
	{
		dst.clear();

		while (rects.size() > 0)
		{
			int bestScore1 = std::numeric_limits<int>::max();
			int bestScore2 = std::numeric_limits<int>::max();
			int bestRectIndex = -1;
			galaxy::Rect<int> bestNode;

			for (size_t i = 0; i < rects.size(); ++i)
			{
				int score1;
				int score2;
				galaxy::Rect<int> newNode = ScoreRect(rects[i].m_x, rects[i].m_y, method, score1, score2);

				if (score1 < bestScore1 || (score1 == bestScore1 && score2 < bestScore2))
				{
					bestScore1 = score1;
					bestScore2 = score2;
					bestNode = newNode;
					bestRectIndex = (int)i;
				}
			}

			if (bestRectIndex == -1)
				return;

			PlaceRect(bestNode);
			rects.erase(rects.begin() + bestRectIndex);
		}
	}

	void MaxRectsBinPack::PlaceRect(const galaxy::Rect<int> &node)
	{
		size_t numRectanglesToProcess = freeRectangles.size();
		for (size_t i = 0; i < numRectanglesToProcess; ++i)
		{
			if (SplitFreeNode(freeRectangles[i], node))
			{
				freeRectangles.erase(freeRectangles.begin() + i);
				--i;
				--numRectanglesToProcess;
			}
		}

		PruneFreeList();

		usedRectangles.push_back(node);
		//		dst.push_back(bestNode); ///\todo Refactor so that this compiles.
	}

	galaxy::Rect<int> MaxRectsBinPack::ScoreRect(int width, int height, FreeRectChoiceHeuristic method, int &score1, int &score2) const
	{
		galaxy::Rect<int> newNode;
		score1 = std::numeric_limits<int>::max();
		score2 = std::numeric_limits<int>::max();
		switch (method)
		{
		case RectBestShortSideFit: newNode = FindPositionForNewNodeBestShortSideFit(width, height, score1, score2); break;
		case RectBottomLeftRule: newNode = FindPositionForNewNodeBottomLeft(width, height, score1, score2); break;
		case RectContactPointRule: newNode = FindPositionForNewNodeContactPoint(width, height, score1);
			score1 = -score1; // Reverse since we are minimizing, but for contact point score bigger is better.
			break;
		case RectBestLongSideFit: newNode = FindPositionForNewNodeBestLongSideFit(width, height, score2, score1); break;
		case RectBestAreaFit: newNode = FindPositionForNewNodeBestAreaFit(width, height, score1, score2); break;
		}

		// Cannot fit the current rectangle.
		if (newNode.m_height == 0)
		{
			score1 = std::numeric_limits<int>::max();
			score2 = std::numeric_limits<int>::max();
		}

		return newNode;
	}

	/// Computes the ratio of used surface area.
	float MaxRectsBinPack::Occupancy() const
	{
		unsigned long usedSurfaceArea = 0;
		for (size_t i = 0; i < usedRectangles.size(); ++i)
			usedSurfaceArea += usedRectangles[i].m_width * usedRectangles[i].m_height;

		return (float)usedSurfaceArea / (binWidth * binHeight);
	}

	galaxy::Rect<int> MaxRectsBinPack::FindPositionForNewNodeBottomLeft(int width, int height, int &bestY, int &bestX) const
	{
		galaxy::Rect<int> bestNode;
		memset(&bestNode, 0, sizeof(galaxy::Rect<int>));

		bestY = std::numeric_limits<int>::max();
		bestX = std::numeric_limits<int>::max();

		for (size_t i = 0; i < freeRectangles.size(); ++i)
		{
			// Try to place the rectangle in upright (non-flipped) orientation.
			if (freeRectangles[i].m_width >= width && freeRectangles[i].m_height >= height)
			{
				int topSideY = freeRectangles[i].m_y + height;
				if (topSideY < bestY || (topSideY == bestY && freeRectangles[i].m_x < bestX))
				{
					bestNode.m_x = freeRectangles[i].m_x;
					bestNode.m_y = freeRectangles[i].m_y;
					bestNode.m_width = width;
					bestNode.m_height = height;
					bestY = topSideY;
					bestX = freeRectangles[i].m_x;
				}
			}
			if (allowRotations && freeRectangles[i].m_width >= height && freeRectangles[i].m_height >= width)
			{
				int topSideY = freeRectangles[i].m_y + width;
				if (topSideY < bestY || (topSideY == bestY && freeRectangles[i].m_x < bestX))
				{
					bestNode.m_x = freeRectangles[i].m_x;
					bestNode.m_y = freeRectangles[i].m_y;
					bestNode.m_width = height;
					bestNode.m_height = width;
					bestY = topSideY;
					bestX = freeRectangles[i].m_x;
				}
			}
		}
		return bestNode;
	}

	galaxy::Rect<int> MaxRectsBinPack::FindPositionForNewNodeBestShortSideFit(int width, int height,
		int &bestShortSideFit, int &bestLongSideFit) const
	{
		galaxy::Rect<int> bestNode;
		memset(&bestNode, 0, sizeof(galaxy::Rect<int>));

		bestShortSideFit = std::numeric_limits<int>::max();
		bestLongSideFit = std::numeric_limits<int>::max();

		for (size_t i = 0; i < freeRectangles.size(); ++i)
		{
			// Try to place the rectangle in upright (non-flipped) orientation.
			if (freeRectangles[i].m_width >= width && freeRectangles[i].m_height >= height)
			{
				int leftoverHoriz = std::abs(freeRectangles[i].m_width - width);
				int leftoverVert = std::abs(freeRectangles[i].m_height - height);
				int shortSideFit = std::min(leftoverHoriz, leftoverVert);
				int longSideFit = std::max(leftoverHoriz, leftoverVert);

				if (shortSideFit < bestShortSideFit || (shortSideFit == bestShortSideFit && longSideFit < bestLongSideFit))
				{
					bestNode.m_x = freeRectangles[i].m_x;
					bestNode.m_y = freeRectangles[i].m_y;
					bestNode.m_width = width;
					bestNode.m_height = height;
					bestShortSideFit = shortSideFit;
					bestLongSideFit = longSideFit;
				}
			}

			if (freeRectangles[i].m_width >= height && freeRectangles[i].m_height >= width)
			{
				int flippedLeftoverHoriz = std::abs(freeRectangles[i].m_width - height);
				int flippedLeftoverVert = std::abs(freeRectangles[i].m_height - width);
				int flippedShortSideFit = std::min(flippedLeftoverHoriz, flippedLeftoverVert);
				int flippedLongSideFit = std::max(flippedLeftoverHoriz, flippedLeftoverVert);

				if (flippedShortSideFit < bestShortSideFit || (flippedShortSideFit == bestShortSideFit && flippedLongSideFit < bestLongSideFit))
				{
					bestNode.m_x = freeRectangles[i].m_x;
					bestNode.m_y = freeRectangles[i].m_y;
					bestNode.m_width = height;
					bestNode.m_height = width;
					bestShortSideFit = flippedShortSideFit;
					bestLongSideFit = flippedLongSideFit;
				}
			}
		}
		return bestNode;
	}

	galaxy::Rect<int> MaxRectsBinPack::FindPositionForNewNodeBestLongSideFit(int width, int height,
		int &bestShortSideFit, int &bestLongSideFit) const
	{
		galaxy::Rect<int> bestNode;
		memset(&bestNode, 0, sizeof(galaxy::Rect<int>));

		bestShortSideFit = std::numeric_limits<int>::max();
		bestLongSideFit = std::numeric_limits<int>::max();

		for (size_t i = 0; i < freeRectangles.size(); ++i)
		{
			// Try to place the rectangle in upright (non-flipped) orientation.
			if (freeRectangles[i].m_width >= width && freeRectangles[i].m_height >= height)
			{
				int leftoverHoriz = std::abs(freeRectangles[i].m_width - width);
				int leftoverVert = std::abs(freeRectangles[i].m_height - height);
				int shortSideFit = std::min(leftoverHoriz, leftoverVert);
				int longSideFit = std::max(leftoverHoriz, leftoverVert);

				if (longSideFit < bestLongSideFit || (longSideFit == bestLongSideFit && shortSideFit < bestShortSideFit))
				{
					bestNode.m_x = freeRectangles[i].m_x;
					bestNode.m_y = freeRectangles[i].m_y;
					bestNode.m_width = width;
					bestNode.m_height = height;
					bestShortSideFit = shortSideFit;
					bestLongSideFit = longSideFit;
				}
			}

			if (allowRotations && freeRectangles[i].m_width >= height && freeRectangles[i].m_height >= width)
			{
				int leftoverHoriz = std::abs(freeRectangles[i].m_width - height);
				int leftoverVert = std::abs(freeRectangles[i].m_height - width);
				int shortSideFit = std::min(leftoverHoriz, leftoverVert);
				int longSideFit = std::max(leftoverHoriz, leftoverVert);

				if (longSideFit < bestLongSideFit || (longSideFit == bestLongSideFit && shortSideFit < bestShortSideFit))
				{
					bestNode.m_x = freeRectangles[i].m_x;
					bestNode.m_y = freeRectangles[i].m_y;
					bestNode.m_width = height;
					bestNode.m_height = width;
					bestShortSideFit = shortSideFit;
					bestLongSideFit = longSideFit;
				}
			}
		}
		return bestNode;
	}

	galaxy::Rect<int> MaxRectsBinPack::FindPositionForNewNodeBestAreaFit(int width, int height,
		int &bestAreaFit, int &bestShortSideFit) const
	{
		galaxy::Rect<int> bestNode;
		memset(&bestNode, 0, sizeof(galaxy::Rect<int>));

		bestAreaFit = std::numeric_limits<int>::max();
		bestShortSideFit = std::numeric_limits<int>::max();

		for (size_t i = 0; i < freeRectangles.size(); ++i)
		{
			int areaFit = freeRectangles[i].m_width * freeRectangles[i].m_height - width * height;

			// Try to place the rectangle in upright (non-flipped) orientation.
			if (freeRectangles[i].m_width >= width && freeRectangles[i].m_height >= height)
			{
				int leftoverHoriz = std::abs(freeRectangles[i].m_width - width);
				int leftoverVert = std::abs(freeRectangles[i].m_height - height);
				int shortSideFit = std::min(leftoverHoriz, leftoverVert);

				if (areaFit < bestAreaFit || (areaFit == bestAreaFit && shortSideFit < bestShortSideFit))
				{
					bestNode.m_x = freeRectangles[i].m_x;
					bestNode.m_y = freeRectangles[i].m_y;
					bestNode.m_width = width;
					bestNode.m_height = height;
					bestShortSideFit = shortSideFit;
					bestAreaFit = areaFit;
				}
			}

			if (allowRotations && freeRectangles[i].m_width >= height && freeRectangles[i].m_height >= width)
			{
				int leftoverHoriz = std::abs(freeRectangles[i].m_width - height);
				int leftoverVert = std::abs(freeRectangles[i].m_height - width);
				int shortSideFit = std::min(leftoverHoriz, leftoverVert);

				if (areaFit < bestAreaFit || (areaFit == bestAreaFit && shortSideFit < bestShortSideFit))
				{
					bestNode.m_x = freeRectangles[i].m_x;
					bestNode.m_y = freeRectangles[i].m_y;
					bestNode.m_width = height;
					bestNode.m_height = width;
					bestShortSideFit = shortSideFit;
					bestAreaFit = areaFit;
				}
			}
		}
		return bestNode;
	}

	/// Returns 0 if the two intervals i1 and i2 are disjoint, or the length of their overlap otherwise.
	int CommonIntervalLength(int i1start, int i1end, int i2start, int i2end)
	{
		if (i1end < i2start || i2end < i1start)
			return 0;
		return std::min(i1end, i2end) - std::max(i1start, i2start);
	}

	int MaxRectsBinPack::ContactPointScoreNode(int x, int y, int width, int height) const
	{
		int score = 0;

		if (x == 0 || x + width == binWidth)
			score += height;
		if (y == 0 || y + height == binHeight)
			score += width;

		for (size_t i = 0; i < usedRectangles.size(); ++i)
		{
			if (usedRectangles[i].m_x == x + width || usedRectangles[i].m_x + usedRectangles[i].m_width == x)
				score += CommonIntervalLength(usedRectangles[i].m_y, usedRectangles[i].m_y + usedRectangles[i].m_height, y, y + height);
			if (usedRectangles[i].m_y == y + height || usedRectangles[i].m_y + usedRectangles[i].m_height == y)
				score += CommonIntervalLength(usedRectangles[i].m_x, usedRectangles[i].m_x + usedRectangles[i].m_width, x, x + width);
		}
		return score;
	}

	galaxy::Rect<int> MaxRectsBinPack::FindPositionForNewNodeContactPoint(int width, int height, int &bestContactScore) const
	{
		galaxy::Rect<int> bestNode;
		memset(&bestNode, 0, sizeof(galaxy::Rect<int>));

		bestContactScore = -1;

		for (size_t i = 0; i < freeRectangles.size(); ++i)
		{
			// Try to place the rectangle in upright (non-flipped) orientation.
			if (freeRectangles[i].m_width >= width && freeRectangles[i].m_height >= height)
			{
				int score = ContactPointScoreNode(freeRectangles[i].m_x, freeRectangles[i].m_y, width, height);
				if (score > bestContactScore)
				{
					bestNode.m_x = freeRectangles[i].m_x;
					bestNode.m_y = freeRectangles[i].m_y;
					bestNode.m_width = width;
					bestNode.m_height = height;
					bestContactScore = score;
				}
			}
			if (freeRectangles[i].m_width >= height && freeRectangles[i].m_height >= width)
			{
				int score = ContactPointScoreNode(freeRectangles[i].m_x, freeRectangles[i].m_y, height, width);
				if (score > bestContactScore)
				{
					bestNode.m_x = freeRectangles[i].m_x;
					bestNode.m_y = freeRectangles[i].m_y;
					bestNode.m_width = height;
					bestNode.m_height = width;
					bestContactScore = score;
				}
			}
		}
		return bestNode;
	}

	bool MaxRectsBinPack::SplitFreeNode(galaxy::Rect<int> freeNode, const galaxy::Rect<int> &usedNode)
	{
		// Test with SAT if the rectangles even intersect.
		if (usedNode.m_x >= freeNode.m_x + freeNode.m_width || usedNode.m_x + usedNode.m_width <= freeNode.m_x ||
			usedNode.m_y >= freeNode.m_y + freeNode.m_height || usedNode.m_y + usedNode.m_height <= freeNode.m_y)
			return false;

		if (usedNode.m_x < freeNode.m_x + freeNode.m_width && usedNode.m_x + usedNode.m_width > freeNode.m_x)
		{
			// New node at the top side of the used node.
			if (usedNode.m_y > freeNode.m_y && usedNode.m_y < freeNode.m_y + freeNode.m_height)
			{
				galaxy::Rect<int> newNode = freeNode;
				newNode.m_height = usedNode.m_y - newNode.m_y;
				freeRectangles.push_back(newNode);
			}

			// New node at the bottom side of the used node.
			if (usedNode.m_y + usedNode.m_height < freeNode.m_y + freeNode.m_height)
			{
				galaxy::Rect<int> newNode = freeNode;
				newNode.m_y = usedNode.m_y + usedNode.m_height;
				newNode.m_height = freeNode.m_y + freeNode.m_height - (usedNode.m_y + usedNode.m_height);
				freeRectangles.push_back(newNode);
			}
		}

		if (usedNode.m_y < freeNode.m_y + freeNode.m_height && usedNode.m_y + usedNode.m_height > freeNode.m_y)
		{
			// New node at the left side of the used node.
			if (usedNode.m_x > freeNode.m_x && usedNode.m_x < freeNode.m_x + freeNode.m_width)
			{
				galaxy::Rect<int> newNode = freeNode;
				newNode.m_width = usedNode.m_x - newNode.m_x;
				freeRectangles.push_back(newNode);
			}

			// New node at the right side of the used node.
			if (usedNode.m_x + usedNode.m_width < freeNode.m_x + freeNode.m_width)
			{
				galaxy::Rect<int> newNode = freeNode;
				newNode.m_x = usedNode.m_x + usedNode.m_width;
				newNode.m_width = freeNode.m_x + freeNode.m_width - (usedNode.m_x + usedNode.m_width);
				freeRectangles.push_back(newNode);
			}
		}

		return true;
	}

	void MaxRectsBinPack::PruneFreeList()
	{
		/// Go through each pair and remove any rectangle that is redundant.
		for (size_t i = 0; i < freeRectangles.size(); ++i)
			for (size_t j = i + 1; j < freeRectangles.size(); ++j)
			{
				if (freeRectangles[i].contains(freeRectangles[j]))
				{
					freeRectangles.erase(freeRectangles.begin() + i);
					--i;
					break;
				}
				if (freeRectangles[j].contains(freeRectangles[i]))
				{
					freeRectangles.erase(freeRectangles.begin() + j);
					--j;
				}
			}
	}
}