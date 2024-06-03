///
/// DenySerialization.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_FLAGS_DENYSERIALIZATION_HPP_
#define GALAXY_FLAGS_DENYSERIALIZATION_HPP_

namespace galaxy
{
	namespace flags
	{
		///
		/// Used to stop an entity from being updated/rendered.
		///
		class DenySerialization final
		{
		  public:
			///
			/// Constructor.
			///
			DenySerialization();

			///
			/// Move constructor.
			///
			DenySerialization(DenySerialization&&);

			///
			/// Move assignment operator.
			///
			DenySerialization& operator=(DenySerialization&&);

			///
			/// \brief Destructor.
			///
			/// I dont know why this has to be virtual. Definitly a bug in MSVC somewhere.
			///
			virtual ~DenySerialization();

		  private:
			///
			/// Copy assignment operator.
			///
			DenySerialization& operator=(const DenySerialization&) = delete;

			///
			/// Copy constructor.
			///
			DenySerialization(const DenySerialization&) = delete;
		};
	} // namespace flags
} // namespace galaxy

#endif
