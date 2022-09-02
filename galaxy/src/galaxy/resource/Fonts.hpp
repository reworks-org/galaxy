///
/// Fonts.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_FONTS_HPP_
#define GALAXY_RESOURCE_FONTS_HPP_

#include "galaxy/graphics/Font.hpp"
#include "galaxy/resource/Cache.hpp"

namespace galaxy
{
	namespace resource
	{
		///
		/// Resource manager for fonts.
		///
		class Fonts final : public Cache<graphics::Font>
		{
		public:
			///
			/// Constructor.
			///
			Fonts() noexcept;

			///
			/// Destructor.
			///
			virtual ~Fonts() noexcept;

			///
			/// Loads fonts in a folder.
			///
			/// \param folder Folder located in the VFS.
			///
			void load(std::string_view folder);

			///
			/// \brief Reload all shaders from folder.
			///
			/// Does nothing if load hasn't been called.
			/// Calls build() for you.
			///
			void reload() override;

			///
			/// \brief Build all font altas'.
			///
			/// Not thread safe, calls OpenGL code.
			///
			void build();

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