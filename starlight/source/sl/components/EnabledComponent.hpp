///
/// EnabledComponent.hpp
/// starlight
///
/// Created by reworks on 05/09/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_ENABLEDCOMPONENT_HPP_
#define STARLIGHT_ENABLEDCOMPONENT_HPP_

#include "sl/libs/cereal/access.hpp"
#include "sl/libs/sol2/sol_forward.hpp"

namespace sl
{
	///
	/// Flag component used to determine if an entity is "alive" or "dead".
	///
	class EnabledComponent final
	{
		friend class cereal::access;
	public:
		///
		/// Default Constructor.
		///
		EnabledComponent() = default;

		///
		/// \brief Alternate Constructor.
		///
		/// Even though this does nothing it allows this component to be created from a lua script.
		///
		/// \param table sol::table containing data.
		/// 
		explicit EnabledComponent(const sol::table& table);

		///
		/// Destructor.
		///
		~EnabledComponent() = default;

	private:
		///
		/// Cereal serialize function.
		///
		template<class Archive>
		inline void serialize(Archive& ar)
		{
		}
	};
}

#endif