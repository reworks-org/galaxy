///
///  SerializeComponent.hpp
///  rework
///
///  Created by reworks on 21/10/2017.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef REWORK_SERIALIZECOMPONENT_HPP_
#define REWORK_SERIALIZECOMPONENT_HPP_

#include "sol2/sol_forward.hpp"

namespace re
{
	class SerializeComponent final
	{
	public:
		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		///
		SerializeComponent(const sol::table& table);

		///
		/// Move Constructor.
		///
		SerializeComponent(SerializeComponent&&) = default;

		///
		/// Destructor.
		///
		~SerializeComponent();

		///
		/// Calls imgui debug functions. Don't call this, done for you by debugmanager.
		///
		void debug();

	public:
		std::string m_id;

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		SerializeComponent() = delete;

		///
		/// Copy Constructor.
		/// Deleted.
		///
		SerializeComponent(const SerializeComponent&) = delete;
	};
}

#endif