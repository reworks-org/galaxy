///
/// Materials.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_MATERIALS_HPP_
#define GALAXY_RESOURCE_MATERIALS_HPP_

#include "galaxy/physics/Material.hpp"
#include "galaxy/resource/Cache.hpp"

namespace galaxy
{
	namespace resource
	{
		///
		/// Resource manager for a physics material.
		///
		class Materials final : public Cache<physics::Material>
		{
		public:
			///
			/// Constructor.
			///
			Materials();

			///
			/// Destructor.
			///
			virtual ~Materials();

			///
			/// Load materials in a folder.
			///
			/// \param folder Folder located in the VFS.
			///
			void load(std::string_view folder);

		private:
			///
			/// Copy constructor.
			///
			Materials(const Materials&) = delete;

			///
			/// Move constructor.
			///
			Materials(Materials&&) = delete;

			///
			/// Copy assignment operator.
			///
			Materials& operator=(const Materials&) = delete;

			///
			/// Move assignment operator.
			///
			Materials& operator=(Materials&&) = delete;

		private:
			///
			/// Material resource folder.
			///
			std::string m_folder;
		};
	} // namespace resource
} // namespace galaxy

#endif