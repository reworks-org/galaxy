///
/// UniformInfo.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_UNIFORMINFO_HPP_
#define GALAXY_GRAPHICS_UNIFORMINFO_HPP_

namespace galaxy
{
	namespace graphics
	{
		///
		/// Data structure wrapping information about a shader uniform.
		///
		struct UniformInfo final
		{
			///
			/// Uniform location in shader.
			///
			int m_location = 0;

			///
			/// Refers to the number of locations the uniform takes up with mat3, vec4 with float, etc. being 1.
			///
			int m_count = 0;
		};
	} // namespace graphics
} // namespace galaxy

#endif