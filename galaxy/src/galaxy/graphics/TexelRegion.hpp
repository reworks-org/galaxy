///
/// TexelRegion.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_TEXELREGION_HPP_
#define GALAXY_GRAPHICS_TEXELREGION_HPP_

#include <glm/vec2.hpp>

namespace galaxy
{
	namespace resource
	{
		class TextureAtlas;
	} // namespace resource

	namespace graphics
	{
		///
		/// Represents a quad on a texture in OpenGL texels.
		///
		class TexelRegion final
		{
			friend class resource::TextureAtlas;

		public:
			///
			/// Get Upper-left coords.
			///
			/// \return const glm::vec2 reference.
			///
			[[nodiscard]] const glm::vec2& ul_texels() const noexcept;

			///
			/// Get Upper-right coords.
			///
			/// \return const glm::vec2 reference.
			///
			[[nodiscard]] const glm::vec2& ur_texels() const noexcept;

			///
			/// Get Bottom-left coords.
			///
			/// \return const glm::vec2 reference.
			///
			[[nodiscard]] const glm::vec2& bl_texels() const noexcept;

			///
			/// Get Bottom-left coords.
			///
			/// \return const glm::vec2 reference.
			///
			[[nodiscard]] const glm::vec2& br_texels() const noexcept;

		private:
			///
			/// UL region in OpenGL compatible texels.
			///
			glm::vec2 m_ul_texels;

			///
			/// UR region in OpenGL compatible texels.
			///
			glm::vec2 m_ur_texels;

			///
			/// BL region in OpenGL compatible texels.
			///
			glm::vec2 m_bl_texels;

			///
			/// BR region in OpenGL compatible texels.
			///
			glm::vec2 m_br_texels;
		};
	} // namespace graphics
} // namespace galaxy

#endif