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

namespace galaxy
{
	namespace resource
	{
		///
		/// Resource cache for sound effects.
		///
		class SFXLoader final
		{
		public:
			///
			/// Overloaded operator() used to load a resource.
			///
			/// \param file Path on disk to load file from. You don't need to check with the filesystem, already done by the cache.
			///
			/// \return Shared pointer to newly created resource.
			///
			std::shared_ptr<media::Sound> operator()(const std::string& file);
		};

		///
		/// Resource cache for music.
		///
		class MusicLoader final
		{
		public:
			///
			/// Overloaded operator() used to load a resource.
			///
			/// \param file Path on disk to load file from. You don't need to check with the filesystem, already done by the cache.
			///
			/// \return Shared pointer to newly created resource.
			///
			std::shared_ptr<media::Sound> operator()(const std::string& file);
		};

		///
		/// Resource cache for sound dialogue.
		///
		class DialogueLoader final
		{
		public:
			///
			/// Overloaded operator() used to load a resource.
			///
			/// \param file Path on disk to load file from. You don't need to check with the filesystem, already done by the cache.
			///
			/// \return Shared pointer to newly created resource.
			///
			std::shared_ptr<media::Sound> operator()(const std::string& file);
		};

		///
		/// Resource cache for video files.
		///
		class VideoLoader final
		{
		public:
			///
			/// \brief Build video data.
			///
			/// Not thread safe, calls OpenGL code.
			///
			/// \param cache Resource cache to build data from.
			///
			void build(robin_hood::unordered_node_map<std::uint64_t, std::shared_ptr<media::Video>>& cache);

			///
			/// Overloaded operator() used to load a resource.
			///
			/// \param file Path on disk to load file from. You don't need to check with the filesystem, already done by the cache.
			///
			/// \return Shared pointer to newly created resource.
			///
			std::shared_ptr<media::Video> operator()(const std::string& file);
		};

		///
		/// Abbreviation for sfx cache.
		///
		using SFXCache = Cache<media::Sound, SFXLoader, false>;

		///
		/// Abbreviation for music cache.
		///
		using MusicCache = Cache<media::Sound, MusicLoader, false>;

		///
		/// Abbreviation for dialogue cache.
		///
		using DialogueCache = Cache<media::Sound, DialogueLoader, false>;

		///
		/// Abbreviation for video cache.
		///
		using VideoCache = Cache<media::Video, VideoLoader, true>;
	} // namespace resource
} // namespace galaxy

#endif