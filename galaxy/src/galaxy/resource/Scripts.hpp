///
/// Scripts.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_SCRIPTS_HPP_
#define GALAXY_RESOURCE_SCRIPTS_HPP_

#include "galaxy/resource/Cache.hpp"
#include "galaxy/scripting/Script.hpp"

namespace galaxy
{
	namespace resource
	{
		///
		/// Resource manager for lua scripts.
		///
		class Scripts final : public Cache<lua::Script>
		{
		public:
			///
			/// Constructor.
			///
			Scripts() noexcept = default;

			///
			/// Destructor.
			///
			virtual ~Scripts() noexcept = default;

			///
			///  Load scripts in a folder.
			///
			/// \param folder Folder located in the VFS.
			///
			void load(std::string_view folder);

			///
			/// \brief Reload all scripts from folder.
			///
			/// Does nothing if load hasn't been called.
			///
			void reload() override;

		private:
			///
			/// Copy constructor.
			///
			Scripts(const Scripts&) = delete;

			///
			/// Move constructor.
			///
			Scripts(Scripts&&) = delete;

			///
			/// Copy assignment operator.
			///
			Scripts& operator=(const Scripts&) = delete;

			///
			/// Move assignment operator.
			///
			Scripts& operator=(Scripts&&) = delete;

		private:
			///
			/// Script resource folder.
			///
			std::string m_folder;
		};
	} // namespace resource
} // namespace galaxy

#endif