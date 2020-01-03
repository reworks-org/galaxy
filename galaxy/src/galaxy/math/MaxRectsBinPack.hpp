/** 
	@file MaxRectsBinPack.hpp
	@author Jukka Jylänki

	@brief Implements different bin packer algorithms that use the MAXRECTS data structure.

	This work is released to Public Domain, do whatever you want with it.

	This repository contains the source code used in my survey 

	"A Thousand Ways to Pack the Bin - A Practical Approach to Two-Dimensional Rectangle Bin Packing."

	The code can be used to solve the problem of packing a set of 2D rectangles into a larger bin. The 
	algorithms are all approximations and use various heuristics, since the problem itself is intractable. 
	For more information, read the paper, which is also contained as a .pdf in this repository.

	The repository contains a build solution for Visual C++ 2010. It is configured to build a single static
	library of the algorithms. The preferred method of embedding these algorithms into your own code is to
	simply copy the code files into your own project and build them along your own build system. 

	To use these algorithms in Python, you can make use of Boost.Python to enable interoperability between the C++ algorithms and your Python code.

	There are no build scripts for other platforms provided.

	For more information, see a series of blog posts at

	http://clb.demon.fi/projects/rectangle-bin-packing
	http://clb.demon.fi/projects/more-rectangle-bin-packing
	http://clb.demon.fi/projects/even-more-rectangle-bin-packing

	The folder old\ contains an implementation of the bin packing algorithm as originally presented by
	Jim Scott at http://www.blackpawn.com/texts/lightmaps/ . This implementation is only provided for
	historical purposes, since in practice the Guillotine method (GuillotineBinPack.cpp) provides superior
	results in terms of performance and packing ratio, and also offers more configuration for variants of
	the algorithm.

	All the code is released to Public Domain. Patches and comments are welcome.
	It makes me happy to hear if someone finds the algorithms useful.

	   Jukka Jylänki


	Code has been modified for use in galaxy by DomRe, specifically the use of galaxy/protostar datatypes.
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
		void Insert(std::vector<galaxy::Vector2<int>> &rects, std::vector<galaxy::Rect<int>> &dst, FreeRectChoiceHeuristic method);

		/// Inserts a single rectangle into the bin, possibly rotated.
		galaxy::Rect<int> Insert(int width, int height, FreeRectChoiceHeuristic method);

		/// Computes the ratio of used surface area to the total bin area.
		float Occupancy() const;

	private:
		int binWidth;
		int binHeight;
		bool allowRotations;

		std::vector<galaxy::Rect<int>> usedRectangles;
		std::vector<galaxy::Rect<int>> freeRectangles;

	private:
		/// Disasble copy constructor.
		MaxRectsBinPack(rbp::MaxRectsBinPack &&) = delete;

		/// Disable const copy constructor.
		MaxRectsBinPack(const rbp::MaxRectsBinPack &) = delete;

		/// Computes the placement score for placing the given rectangle with the given method.
		/// \param score1 [out] The primary placement score will be outputted here.
		/// \param score2 [out] The secondary placement score will be outputted here. This isu sed to break ties.
		/// \return This struct identifies where the rectangle would be placed if it were placed.
		galaxy::Rect<int> ScoreRect(int width, int height, FreeRectChoiceHeuristic method, int &score1, int &score2) const;

		/// Places the given rectangle into the bin.
		void PlaceRect(const galaxy::Rect<int> &node);

		/// Computes the placement score for the -CP variant.
		int ContactPointScoreNode(int x, int y, int width, int height) const;

		galaxy::Rect<int> FindPositionForNewNodeBottomLeft(int width, int height, int &bestY, int &bestX) const;
		galaxy::Rect<int> FindPositionForNewNodeBestShortSideFit(int width, int height, int &bestShortSideFit, int &bestLongSideFit) const;
		galaxy::Rect<int> FindPositionForNewNodeBestLongSideFit(int width, int height, int &bestShortSideFit, int &bestLongSideFit) const;
		galaxy::Rect<int> FindPositionForNewNodeBestAreaFit(int width, int height, int &bestAreaFit, int &bestShortSideFit) const;
		galaxy::Rect<int> FindPositionForNewNodeContactPoint(int width, int height, int &contactScore) const;

		/// \return True if the free node was split.
		bool SplitFreeNode(galaxy::Rect<int> freeNode, const galaxy::Rect<int> &usedNode);

		/// Goes through the free rectangle list and removes any redundant entries.
		void PruneFreeList();
	};
}

#endif