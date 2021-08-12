///
/// MusicBook.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_MUSICBOOK_HPP_
#define GALAXY_RESOURCE_MUSICBOOK_HPP_

#include "galaxy/audio/Music.hpp"
#include "galaxy/fs/Serializable.hpp"
#include "galaxy/resource/ResourceCache.hpp"

namespace galaxy
{
	namespace res
	{
		///
		/// Resource manager for music.
		///
		class MusicBook final : public ResourceCache<audio::Music>, public fs::Serializable
		{
			friend audio::Music;

		public:
			///
			/// Constructor.
			///
			MusicBook() noexcept = default;

			///
			/// JSON constructor.
			///
			/// \param file JSON file to load.
			///
			MusicBook(std::string_view file);

			///
			/// Destructor.
			///
			virtual ~MusicBook() noexcept;

			///
			/// Create MusicBook from JSON.
			///
			/// \param file JSON file to load.
			///
			void create_from_json(std::string_view file);

			///
			/// Clean up.
			///
			void clear() noexcept override;

			///
			/// Stop all active music.
			///
			void stop_all() noexcept;

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to write out.
			///
			[[nodiscard]] nlohmann::json serialize() override;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			void deserialize(const nlohmann::json& json) override;

		private:
			///
			/// Copy constructor.
			///
			MusicBook(const MusicBook&) = delete;

			///
			/// Move constructor.
			///
			MusicBook(MusicBook&&) = delete;

			///
			/// Copy assignment operator.
			///
			MusicBook& operator=(const MusicBook&) = delete;

			///
			/// Move assignment operator.
			///
			MusicBook& operator=(MusicBook&&) = delete;

		private:
			///
			/// Stores pointers to playing music.
			///
			robin_hood::unordered_flat_map<audio::Music*, audio::Music*> m_playing;
		};
	} // namespace res
} // namespace galaxy

#endif