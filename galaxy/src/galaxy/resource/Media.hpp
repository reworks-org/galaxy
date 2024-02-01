///
/// Media.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_MEDIA_HPP_
#define GALAXY_RESOURCE_MEDIA_HPP_

#include "galaxy/media/Sound.hpp"
#include "galaxy/media/Video.hpp"
#include "galaxy/resource/Cache.hpp"
#include "galaxy/resource/Loader.hpp"

namespace galaxy
{
	namespace resource
	{
		///
		/// Loads resources for the sound cache.
		///
		/// \tparam Resource A resource is a class containing data.
		///
		struct SoundLoader final : public Loader<media::Sound>
		{
			///
			/// Overloaded operator() used to load a resource.
			///
			/// \param file Path on disk/archive to load file from.
			///
			/// \return Handle to created resource.
			///
			std::shared_ptr<media::Sound> operator()(const std::string& file);
		};

		///
		/// Loads resources for the music cache.
		///
		/// \tparam Resource A resource is a class containing data.
		///
		struct MusicLoader final : public Loader<media::Sound>
		{
			///
			/// Overloaded operator() used to load a resource.
			///
			/// \param file Path on disk/archive to load file from.
			///
			/// \return Handle to created resource.
			///
			std::shared_ptr<media::Sound> operator()(const std::string& file);
		};

		///
		/// Loads resources for the voice cache.
		///
		/// \tparam Resource A resource is a class containing data.
		///
		struct VoiceLoader final : public Loader<media::Sound>
		{
			///
			/// Overloaded operator() used to load a resource.
			///
			/// \param file Path on disk/archive to load file from.
			///
			/// \return Handle to created resource.
			///
			std::shared_ptr<media::Sound> operator()(const std::string& file);
		};

		///
		/// Abbreviation for sound cache.
		///
		using SoundCache = Cache<media::Sound, SoundLoader>;

		///
		/// Abbreviation for sound cache.
		///
		using MusicCache = Cache<media::Sound, MusicLoader>;

		///
		/// Abbreviation for sound cache.
		///
		using VoiceCache = Cache<media::Sound, VoiceLoader>;

		///
		/// Abbreviation for video cache.
		///
		using VideoCache = Cache<media::Video, Loader<media::Video>>;
	} // namespace resource
} // namespace galaxy

#endif
