///
/// NoisePanel.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_PANELS_NOISEPANEL_HPP_
#define SUPERCLUSTER_EDITOR_PANELS_NOISEPANEL_HPP_

#include <FastNoise/FastNoise.h>
#include "FastNoiseNodeEditor.h"

namespace sc
{
	namespace panel
	{
		// Adapted from
		// https://github.com/Auburn/FastNoise2/tree/master/NoiseTool
		class NoisePanel final
		{
		public:
			NoisePanel();
			~NoisePanel();

			void render(bool* show);

		private:
			int mMaxSIMDLevel = 0;
			std::vector<const char*> mLevelNames;
			std::vector<FastSIMD::eLevel> mLevelEnums;
			std::unique_ptr<FastNoiseNodeEditor> mNodeEditor;
		};
	} // namespace panel
} // namespace sc

#endif