///
/// TextureFilters.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_TEXTUREFILTERS_HPP_
#define GALAXY_GRAPHICS_TEXTUREFILTERS_HPP_

#include <type_traits>

#include <glad/glad.h>

namespace galaxy
{
	namespace graphics
	{
		///
		/// Produces blocky/pixellated texture patterns.
		/// Think 8-bit.
		///
		struct NearestMipmapFilter final
		{
			NearestMipmapFilter() = delete;

			inline static constexpr const auto value = GL_NEAREST_MIPMAP_NEAREST;
		};

		///
		/// Trilinear filtering for textures.
		///
		struct TrilinearMipmapFilter final
		{
			TrilinearMipmapFilter() = delete;

			inline static constexpr const auto value = GL_LINEAR_MIPMAP_LINEAR;
		};

		///
		/// Produces blocky/pixellated texture patterns.
		/// Think 8-bit.
		///
		struct NearestTexFilter final
		{
			NearestTexFilter() = delete;

			inline static constexpr const auto value = GL_NEAREST;
		};

		///
		/// Attempts to smooth texture pattern. Less visible pixels.
		///
		struct BilinearTexFilter final
		{
			BilinearTexFilter() = delete;

			inline static constexpr const auto value = GL_LINEAR;
		};

		///
		/// Concept to restrict mag filter type.
		///
		template<typename Type>
		concept mag_filter = (std::is_same<NearestTexFilter, Type>::value || std::is_same<BilinearTexFilter, Type>::value);

		///
		/// Concept to restrict min filter type.
		///
		template<typename Type>
		concept min_filter = mag_filter<Type> || (std::is_same<NearestMipmapFilter, Type>::value || std::is_same<TrilinearMipmapFilter, Type>::value);
	} // namespace graphics
} // namespace galaxy

#endif