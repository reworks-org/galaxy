///
/// MusicBook.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RES_MUSICBOOK_HPP_
#define GALAXY_RES_MUSICBOOK_HPP_

#include "galaxy/audio/Music.hpp"
#include "galaxy/fs/Serializable.hpp"
#include "galaxy/res/ResourceCache.hpp"

namespace galaxy
{
	namespace res
	{
		///
		/// Resource manager for music.
		///
		class MusicBook final : public ResourceCache<audio::Music>, public fs::Serializable
		{
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
		};
	} // namespace res
} // namespace galaxy

#endif