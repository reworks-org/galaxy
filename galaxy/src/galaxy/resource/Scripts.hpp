///
/// Scripts.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_SCRIPTS_HPP_
#define GALAXY_RESOURCE_SCRIPTS_HPP_

#include "galaxy/resource/Cache.hpp"
#include "galaxy/scripting/BasicScript.hpp"

namespace galaxy
{
	namespace resource
	{
		///
		/// Resource manager for lua scripts.
		///
		class Scripts final : public Cache<lua::BasicScript>
		{
		public:
			///
			/// Constructor.
			///
			Scripts();

			///
			/// Destructor.
			///
			virtual ~Scripts();

			///
			/// Loads resources from a folder.
			///
			/// \param folder Folder located in the VFS.
			///
			/// \return Thread handle of loading thread.
			///
			std::future<void> load(std::string_view folder) override;

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