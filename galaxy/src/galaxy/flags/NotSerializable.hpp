///
/// NotSerializable.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_FLAGS_NOTSERIALIZABLE_HPP_
#define GALAXY_FLAGS_NOTSERIALIZABLE_HPP_

namespace galaxy
{
	namespace flags
	{
		///
		/// Used to stop an entity from being updated/rendered.
		///
		class NotSerializable final
		{
		public:
			///
			/// Constructor.
			///
			NotSerializable() noexcept = default;

			///
			/// Move constructor.
			///
			NotSerializable(NotSerializable&&) noexcept = default;

			///
			/// Move assignment operator.
			///
			NotSerializable& operator=(NotSerializable&&) noexcept = default;

			///
			/// \brief Destructor.
			///
			/// I dont know why this has to be virtual. Definitly a bug in MSVC somewhere.
			///
			virtual ~NotSerializable() noexcept = default;

		private:
			///
			/// Copy assignment operator.
			///
			NotSerializable& operator=(const NotSerializable&) = delete;

			///
			/// Copy constructor.
			///
			NotSerializable(const NotSerializable&) = delete;
		};
	} // namespace flags
} // namespace galaxy

#endif
