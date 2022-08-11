///
/// Fonts.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_FONTBOOK_HPP_
#define GALAXY_RESOURCE_FONTBOOK_HPP_

#include "galaxy/graphics/text/Font.hpp"
#include "galaxy/resource/Cache.hpp"

namespace galaxy
{
	namespace resource
	{
		///
		/// \brief Resource manager for fonts.
		///
		/// Fonts must be named in the following format:
		/// ARIAL_11.ttf
		/// This allows a font to have a size on load.
		///
		class Fonts final : public Cache<graphics::Font>
		{
		public:
			///
			/// Constructor.
			///
			Fonts() noexcept = default;

			///
			/// Destructor.
			///
			virtual ~Fonts() noexcept = default;

			///
			/// \brief Loads fonts in a folder.
			///
			/// Fonts must be named in the following format:
			/// ARIAL_11.ttf
			///
			/// \param folder Folder located in the VFS.
			///
			void load(std::string_view folder);

			///
			/// \brief Reload all shaders from folder.
			///
			/// Does nothing if load hasn't been called.
			///
			void reload() override;

		private:
			///
			/// Copy constructor.
			///
			Fonts(const Fonts&) = delete;

			///
			/// Move constructor.
			///
			Fonts(Fonts&&) = delete;

			///
			/// Copy assignment operator.
			///
			Fonts& operator=(const Fonts&) = delete;

			///
			/// Move assignment operator.
			///
			Fonts& operator=(Fonts&&) = delete;

		private:
			///
			/// Font resource folder.
			///
			std::string m_folder;
		};
	} // namespace resource
} // namespace galaxy

#endif