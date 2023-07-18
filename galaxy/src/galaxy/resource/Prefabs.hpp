///
/// Prefabs.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_PREFABS_HPP_
#define GALAXY_RESOURCE_PREFABS_HPP_

#include "galaxy/core/Prefab.hpp"
#include "galaxy/resource/Cache.hpp"

namespace galaxy
{
	namespace resource
	{
		///
		/// Resource manager for lua Prefabs.
		///
		class Prefabs final : public Cache<core::Prefab>
		{
		public:
			///
			/// Constructor.
			///
			Prefabs();

			///
			/// Destructor.
			///
			virtual ~Prefabs();

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
			Prefabs(const Prefabs&) = delete;

			///
			/// Move constructor.
			///
			Prefabs(Prefabs&&) = delete;

			///
			/// Copy assignment operator.
			///
			Prefabs& operator=(const Prefabs&) = delete;

			///
			/// Move assignment operator.
			///
			Prefabs& operator=(Prefabs&&) = delete;

		private:
			///
			/// Prefab resource folder.
			///
			std::string m_folder;
		};
	} // namespace resource
} // namespace galaxy

#endif