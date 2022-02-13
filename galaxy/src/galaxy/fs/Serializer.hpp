///
/// Serializer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_FS_SERIALIZER_HPP_
#define GALAXY_FS_SERIALIZER_HPP_

#include <future>

#include "galaxy/fs/Serializable.hpp"

namespace galaxy
{
	namespace fs
	{
		///
		/// Serializer service to keep track of data needing to be saved.
		///
		class Serializer final
		{
		public:
			///
			/// Constructor.
			///
			Serializer() noexcept;

			///
			/// Destructor.
			///
			~Serializer() noexcept;

			///
			/// Add a new object to be serialized.
			///
			/// \param obj Object to add.
			///
			void add(Serializable* obj) noexcept;

			///
			/// Clear all stored data.
			///
			void clear() noexcept;

			///
			/// Serialize all stored.
			///
			/// \param file File to save to.
			///
			/// \return Task to wait on until done.
			///
			[[nodiscard]] std::future<void> save_all(std::string_view file);

			///
			/// Deserialize all stored.
			///
			/// \param file File to load from.
			///
			/// \return Task to wait on until done.
			///
			[[nodiscard]] std::future<void> load_all(std::string_view file);

		private:
			///
			/// Copy constructor.
			///
			Serializer(const Serializer&) = delete;

			///
			/// Move constructor.
			///
			Serializer(Serializer&&) = delete;

			///
			/// Copy assignment operator.
			///
			Serializer& operator=(const Serializer&) = delete;

			///
			/// Move assignment operator.
			///
			Serializer& operator=(Serializer&&) = delete;

		private:
			///
			/// List of data to serialize.
			///
			std::vector<Serializable*> m_data;
		};
	} // namespace fs
} // namespace galaxy

#endif