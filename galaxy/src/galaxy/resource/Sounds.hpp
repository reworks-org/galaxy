///
/// Sounds.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_SOUNDS_HPP_
#define GALAXY_RESOURCE_SOUNDS_HPP_

#include "galaxy/audio/Audio.hpp"
#include "galaxy/resource/Cache.hpp"

namespace galaxy
{
	namespace resource
	{
		///
		/// Resource cache for sounds.
		///
		class Sounds final : public Cache<audio::Audio>
		{
		public:
			///
			/// Constructor.
			///
			Sounds() noexcept;

			///
			/// Destructor.
			///
			virtual ~Sounds() noexcept;

			///
			/// Load sound effects from VFS.
			///
			/// \param folder Folder located in the VFS.
			///
			void load_sfx(std::string_view folder);

			///
			/// Load music from VFS.
			///
			/// \param folder Folder located in the VFS.
			///
			void load_music(std::string_view folder);

			///
			/// Load dialogue from VFS.
			///
			/// \param folder Folder located in the VFS.
			///
			void load_dialogue(std::string_view folder);

			///
			/// \brief Reload all sound assets from folders.
			///
			/// Does nothing if the above load functions haven't been called.
			///
			void reload() override;

		private:
			///
			/// Copy constructor.
			///
			Sounds(const Sounds&) = delete;

			///
			/// Move constructor.
			///
			Sounds(Sounds&&) = delete;

			///
			/// Copy assignment operator.
			///
			Sounds& operator=(const Sounds&) = delete;

			///
			/// Move assignment operator.
			///
			Sounds& operator=(Sounds&&) = delete;

		private:
			///
			/// SFX folder path.
			///
			std::string m_sfx_path;

			///
			/// Music folder path.
			///
			std::string m_music_path;

			///
			/// Voice folder path.
			///
			std::string m_voice_path;
		};
	} // namespace resource
} // namespace galaxy

#endif