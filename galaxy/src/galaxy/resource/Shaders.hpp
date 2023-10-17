///
/// Shaders.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_SHADERS_HPP_
#define GALAXY_RESOURCE_SHADERS_HPP_

#include "galaxy/graphics/Shader.hpp"
#include "galaxy/resource/Cache.hpp"

namespace galaxy
{
	namespace resource
	{
		///
		/// Resource cache for shaders.
		///
		class ShaderLoader final
		{
		  public:
			///
			/// \brief Build shaders.
			///
			/// Not thread safe, calls OpenGL code.
			///
			/// \param cache Resource cache to build data from.
			///
			void build(robin_hood::unordered_node_map<std::uint64_t, std::shared_ptr<graphics::Shader>>& cache);

			///
			/// Overloaded operator() used to load a resource.
			///
			/// \param file Path on disk to load file from. You don't need to check with the filesystem, already done by the cache.
			///
			/// \return Shared pointer to newly created resource.
			///
			std::shared_ptr<graphics::Shader> operator()(const std::string& file);
		};

		///
		/// Abbreviation for font cache type.
		///
		using Shaders = Cache<graphics::Shader, ShaderLoader, true>;
	} // namespace resource
} // namespace galaxy

#endif
