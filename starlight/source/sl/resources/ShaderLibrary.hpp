///
/// ShaderLibrary.hpp
/// starlight
///
/// Created by reworks on 14/10/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_SHADERLIBRARY_HPP_
#define STARLIGHT_SHADERLIBRARY_HPP_

#include "sl/graphics/Shader.hpp"
#include "sl/types/ResourceCache.hpp"

namespace sl
{
	///
	/// Manages all the shaders the library uses.
	///
	class ShaderLibrary final : public ResourceCache<Shader>
	{
	public:
		///
		/// \brief Construct ShaderLibrary and add shaders.
		///
		/// \param script Script file containing shader info to parse.
		///
		explicit ShaderLibrary(const std::string& script);

		///
		/// Destructor. Cleans up shaders.
		///
		~ShaderLibrary() override;

		///
		/// Clean up resources.
		///
		void clean() override;

	private:
		///
		/// Default Constructor.
		/// Deleted.
		///
		ShaderLibrary() = delete;
	};
}

#endif