///
/// SMAA.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_POSTEFFECTS_SMAA_HPP_
#define GALAXY_GRAPHICS_POSTEFFECTS_SMAA_HPP_

#include "galaxy/graphics/Shader.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Submorphological Anti Aliasing.
		///
		class SMAA final
		{
		public:
			///
			/// Constructor.
			///
			SMAA();

			///
			/// Destructor.
			///
			~SMAA();

			///
			/// Resize framebuffers.
			///
			/// \param width New width of internal framebuffer.
			/// \param height New height of internal framebuffer.
			///
			void resize(const int width, const int height);

			///
			/// Render effect to input texture.
			///
			/// \return Returns output texture of SMAA'd scene.
			///
			[[nodiscard]] const unsigned int render(const unsigned int input);

		private:
			///
			/// SMAA Edge shader.
			///
			Shader m_smaa_edge;

			///
			/// SMAA Blending shader.
			///
			Shader m_smaa_blend;

			///
			/// SMAA neighbourhood shader.
			///
			Shader m_smaa_neighbourhood;

			///
			/// SMAA neighbour texture.
			///
			unsigned int m_neighbour_tex;

			///
			/// SMAA edge texture.
			///
			unsigned int m_edge_tex;

			///
			/// SMAA blend texture.
			///
			unsigned int m_blend_tex;

			///
			/// SMAA area texture.
			///
			unsigned int m_area_tex;

			///
			/// SMAA search texture.
			///
			unsigned int m_search_tex;

			///
			/// SMAA neighbour framebuffer.
			///
			unsigned int m_neighbour_fbo;

			///
			/// SMAA edge framebuffer.
			///
			unsigned int m_edge_fbo;

			///
			/// SMAA blend framebuffer.
			///
			unsigned int m_blend_fbo;
		};
	} // namespace graphics
} // namespace galaxy

#endif