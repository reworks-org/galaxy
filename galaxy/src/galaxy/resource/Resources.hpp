///
/// Resources.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_RESOURCES_HPP_
#define GALAXY_RESOURCE_RESOURCES_HPP_

#include "galaxy/resource/Cache.hpp"
#include "galaxy/resource/Musical.hpp"
#include "galaxy/resource/Shaders.hpp"
#include "galaxy/resource/Sounds.hpp"

namespace galaxy
{
	namespace resource
	{
		///
		/// Typedef for shader cache.
		///
		using ShaderCache = resource::Cache<graphics::Shader, resource::Shaders>;

		///
		/// Typedef for sound cache.
		///
		using SoundCache = resource::Cache<audio::Sound, resource::Sounds>;

		///
		/// Typedef for music cache.
		///
		using MusicCache = resource::Cache<audio::Music, resource::Musical>;

		///
		/// Holds unique resources for a scene to be shared down to layers.
		///
		struct Resources final
		{
			///
			/// Shader cache.
			///
			ShaderCache m_shaders;

			///
			/// Sound cache.
			///
			SoundCache m_sounds;

			///
			/// Music cache.
			///
			MusicCache m_music;
		};
	} // namespace resource
} // namespace galaxy

#endif