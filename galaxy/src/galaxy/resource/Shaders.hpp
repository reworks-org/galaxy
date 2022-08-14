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
		/// \brief Resource cache for shaders.
		///
		/// Shaders are required to have an extension of .vs for vertex shaders
		/// and .fs for fragment shaders.
		/// Or whatever has been configured in GALAXY GLOBALS CONFIG.
		///
		class Shaders final : public Cache<graphics::Shader>
		{
		public:
			///
			/// Constructor.
			///
			Shaders() noexcept;

			///
			/// Destructor.
			///
			virtual ~Shaders() noexcept;

			///
			/// \brief Loads shaders in a folder.
			///
			/// Paired shaders must share a common filename.
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
			Shaders(const Shaders&) = delete;

			///
			/// Move constructor.
			///
			Shaders(Shaders&&) = delete;

			///
			/// Copy assignment operator.
			///
			Shaders& operator=(const Shaders&) = delete;

			///
			/// Move assignment operator.
			///
			Shaders& operator=(Shaders&&) = delete;

		private:
			///
			/// Shader resource folder.
			///
			std::string m_folder;
		};
	} // namespace resource
} // namespace galaxy

#endif