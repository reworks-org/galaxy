///
/// NoisePanel.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_EDITOR_PANELS_NOISEPANEL_HPP_
#define SUPERCLUSTER_EDITOR_PANELS_NOISEPANEL_HPP_

#include "editor/UpdateStack.hpp"
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

			void render(bool* show, UpdateStack& updates);

		  private:
			int                                  m_max_simd_level;
			std::vector<const char*>             m_level_names;
			std::vector<FastSIMD::eLevel>        m_level_enums;
			std::unique_ptr<FastNoiseNodeEditor> m_node_editor;
		};
	} // namespace panel
} // namespace sc

#endif
