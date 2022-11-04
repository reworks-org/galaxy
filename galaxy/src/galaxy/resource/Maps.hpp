///
/// Maps.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_MAPS_HPP_
#define GALAXY_RESOURCE_MAPS_HPP_

#include <tileson.hpp>

#include "galaxy/resource/Cache.hpp"

namespace galaxy
{
	namespace resource
	{
		///
		/// Resource manager for Tiled maps.
		///
		class Maps final : public Cache<tson::Map>
		{
		public:
			///
			/// Constructor.
			///
			Maps() noexcept;

			///
			/// Destructor.
			///
			virtual ~Maps() noexcept;

			///
			/// Load maps in a folder.
			///
			/// \param folder Folder located in the VFS.
			///
			void load(std::string_view folder);

		private:
			///
			/// Copy constructor.
			///
			Maps(const Maps&) = delete;

			///
			/// Move constructor.
			///
			Maps(Maps&&) = delete;

			///
			/// Copy assignment operator.
			///
			Maps& operator=(const Maps&) = delete;

			///
			/// Move assignment operator.
			///
			Maps& operator=(Maps&&) = delete;

		private:
			///
			/// Maps resource folder.
			///
			std::string m_folder;
		};
	} // namespace resource
} // namespace galaxy

#endif