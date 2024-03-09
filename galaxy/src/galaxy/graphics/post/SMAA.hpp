///
/// SMAA.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_POST_SMAA_HPP_
#define GALAXY_GRAPHICS_POST_SMAA_HPP_

#include "galaxy/graphics/gl/Shader.hpp"
#include "galaxy/graphics/PostEffect.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// \brief Submorphological Anti Aliasing.
		///
		/// This is the first post effect you should run.
		///
		class SMAA final : public PostEffect
		{
		  public:
			///
			/// Argument constructor.
			///
			/// \param width Width of internal framebuffer.
			/// \param height Height of internal framebuffer.
			///
			SMAA(const int width, const int height);

			///
			/// Destructor.
			///
			virtual ~SMAA();

			///
			/// Resize framebuffers.
			///
			/// \param width New width of internal framebuffer.
			/// \param height New height of internal framebuffer.
			///
			void resize(const int width, const int height) override;

			///
			/// Render effect to input texture.
			///
			/// \param input Input texture from previous effect or window framebuffer.
			///
			/// \return Returns output texture for next effect or final framebuffer.
			///
			[[nodiscard]] unsigned int render(const unsigned int input) override;

			///
			/// Is this effect enabled?
			///
			/// \return True if effect is enabled. Otherwise false.
			///
			[[nodiscard]] bool is_enabled() override;

		  private:
			///
			/// Constructor.
			///
			SMAA() = delete;

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
