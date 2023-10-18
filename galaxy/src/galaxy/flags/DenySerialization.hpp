///
/// DenySerialization.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_FLAGS_DENYSERIALIZATION_HPP_
#define GALAXY_FLAGS_DENYSERIALIZATION_HPP_

#include <nlohmann/json_fwd.hpp>

namespace galaxy
{
	namespace flags
	{
		///
		/// Used to exclude a component when serializing.
		///
		class DenySerialization final
		{
		  public:
			///
			/// Constructor.
			///
			DenySerialization();

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			DenySerialization(const nlohmann::json& json);

			///
			/// Pointer constructor.
			///
			/// \param ptr Data to COPY into this component.
			///
			DenySerialization(DenySerialization* ptr);

			///
			/// Move constructor.
			///
			DenySerialization(DenySerialization&&);

			///
			/// Move assignment operator.
			///
			DenySerialization& operator=(DenySerialization&&);

			///
			/// Destructor.
			///
			virtual ~DenySerialization();
		};
	} // namespace flags
} // namespace galaxy

#endif
