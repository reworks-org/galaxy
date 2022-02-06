///
/// AllowSerialize.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_FLAG_ALLOWSERIALIZE_HPP_
#define GALAXY_FLAG_ALLOWSERIALIZE_HPP_

namespace galaxy
{
	namespace flags
	{
		///
		/// Used to set if an entity is allowed to be serialized. (Does not affect deserialization).
		///
		struct AllowSerialize final
		{
			inline static constexpr const unsigned short value = 1;
		};
	} // namespace flags
} // namespace galaxy

#endif