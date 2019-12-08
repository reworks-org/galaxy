///
/// ShaderLibrary.hpp
/// galaxy
///
/// Created by reworks on 14/10/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SHADERLIBRARY_HPP_
#define GALAXY_SHADERLIBRARY_HPP_

#include "galaxy/graphics/Shader.hpp"
#include "galaxy/types/ResourceCache.hpp"

namespace galaxy
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
		~ShaderLibrary() noexcept override;

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