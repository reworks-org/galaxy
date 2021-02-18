///
/// Filters.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_TEXTURE_FILTERS_HPP_
#define GALAXY_GRAPHICS_TEXTURE_FILTERS_HPP_

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
		struct NearestTexFilter final
		{
			NearestTexFilter() = delete;

			inline static const constexpr auto value = GL_NEAREST_MIPMAP_LINEAR;
		};

		///
		/// Attempts to smooth texture pattern. Less visible pixels.
		///
		struct LinearTexFilter final
		{
			LinearTexFilter() = delete;

			inline static const constexpr auto value = GL_LINEAR_MIPMAP_LINEAR;
		};

		///
		/// Concept to restrict to filter type.
		///
		template<typename Type>
		concept tex_filter = (std::is_same<NearestTexFilter, Type>::value || std::is_same<LinearTexFilter, Type>::value);
	} // namespace graphics
} // namespace galaxy

#endif