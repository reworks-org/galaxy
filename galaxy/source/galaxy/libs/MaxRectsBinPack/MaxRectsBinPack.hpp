/** 
	@file MaxRectsBinPack.hpp
	@author Jukka Jylänki

	@brief Implements different bin packer algorithms that use the MAXRECTS data structure.

	This work is released to Public Domain, do whatever you want with it.

	THIS FILE HAS BEEN MODIFIED BY REWORKS FOR USE IN REWORK.
*/

#ifndef JUKKA_MAXRECTSBINPACK_HPP_
#define JUKKA_MAXRECTSBINPACK_HPP_

#include <vector>

#include "galaxy/math/Rect.hpp"
#include "galaxy/math/Vector2.hpp"

namespace rbp
{
	/** MaxRectsBinPack implements the MAXRECTS data structure and different bin packing algorithms that
	use this structure. */
	class MaxRectsBinPack
	{
	public:
		/// Instantiates a bin of size (0,0). Call Init to create a new bin.
		MaxRectsBinPack();

		/// Instantiates a bin of the given size.
		MaxRectsBinPack(int width, int height, bool rotations = false);

		/// (Re)initializes the packer to an empty bin of width x height units. Call whenever
		/// you need to restart with a new bin.
		void Init(int width, int height, bool rotations = false);

		/// Specifies the different heuristic rules that can be used when deciding where to place a new rectangle.
		enum FreeRectChoiceHeuristic
		{
			RectBestShortSideFit, ///< -BSSF: Positions the rectangle against the short side of a free rectangle into which it fits the best.
			RectBestLongSideFit, ///< -BLSF: Positions the rectangle against the long side of a free rectangle into which it fits the best.
			RectBestAreaFit, ///< -BAF: Positions the rectangle into the smallest free rect into which it fits.
			RectBottomLeftRule, ///< -BL: Does the Tetris placement.
			RectContactPointRule ///< -CP: Choosest the placement where the rectangle touches other rects as much as possible.
		};

		/// Inserts the given list of rectangles in an offline/batch mode, possibly rotated.
		/// \param rects The list of rectangles to insert. This vector will be destroyed in the process.
		/// \param dst [out] This list will contain the packed rectangles. The indices will not correspond to that of rects.
		/// \param method The rectangle placement rule to use when packing.
		void Insert(std::vector<galaxyVector2<int>> &rects, std::vector<galaxyRect<int>> &dst, FreeRectChoiceHeuristic method);

		/// Inserts a single rectangle into the bin, possibly rotated.
		galaxyRect<int> Insert(int width, int height, FreeRectChoiceHeuristic method);

		/// Computes the ratio of used surface area to the total bin area.
		float Occupancy() const;

	private:
		int binWidth;
		int binHeight;
		bool allowRotations;

		std::vector<galaxyRect<int>> usedRectangles;
		std::vector<galaxyRect<int>> freeRectangles;

	private:
		/// Disasble copy constructor.
		MaxRectsBinPack(rbp::MaxRectsBinPack &&) = delete;

		/// Disable const copy constructor.
		MaxRectsBinPack(const rbp::MaxRectsBinPack &) = delete;

		/// Computes the placement score for placing the given rectangle with the given method.
		/// \param score1 [out] The primary placement score will be outputted here.
		/// \param score2 [out] The secondary placement score will be outputted here. This isu sed to break ties.
		/// \return This struct identifies where the rectangle would be placed if it were placed.
		galaxyRect<int> ScoreRect(int width, int height, FreeRectChoiceHeuristic method, int &score1, int &score2) const;

		/// Places the given rectangle into the bin.
		void PlaceRect(const galaxyRect<int> &node);

		/// Computes the placement score for the -CP variant.
		int ContactPointScoreNode(int x, int y, int width, int height) const;

		galaxyRect<int> FindPositionForNewNodeBottomLeft(int width, int height, int &bestY, int &bestX) const;
		galaxyRect<int> FindPositionForNewNodeBestShortSideFit(int width, int height, int &bestShortSideFit, int &bestLongSideFit) const;
		galaxyRect<int> FindPositionForNewNodeBestLongSideFit(int width, int height, int &bestShortSideFit, int &bestLongSideFit) const;
		galaxyRect<int> FindPositionForNewNodeBestAreaFit(int width, int height, int &bestAreaFit, int &bestShortSideFit) const;
		galaxyRect<int> FindPositionForNewNodeContactPoint(int width, int height, int &contactScore) const;

		/// \return True if the free node was split.
		bool SplitFreeNode(galaxyRect<int> freeNode, const galaxyRect<int> &usedNode);

		/// Goes through the free rectangle list and removes any redundant entries.
		void PruneFreeList();
	};
}

#endif