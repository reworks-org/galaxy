//
//  TransformComponent.hpp
//  rework
//
//  Created by reworks on 30/09/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_TRANSFORMCOMPONENT_HPP_
#define REWORK_TRANSFORMCOMPONENT_HPP_

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include "sol2/sol.hpp"

namespace re
{
	class TransformComponent
	{
		friend class boost::serialization::access;

	public:
		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		///
		TransformComponent(sol::table& table);

		///
		/// Destructor.
		///
		~TransformComponent();

		///
		/// Calls imgui debug functions. Don't call this, done for you by debugmanager.
		///
		void debug();

	private:
		///
		/// Boost.Serialization saving.
		///
		template<class Archive>
		void save(Archive& ar, const unsigned int version) const
		{
			ar & m_x;
			ar & m_y;
			ar & m_angle;
		}

		///
		/// Boost.Serialization loading.
		///
		template<class Archive>
		void load(Archive& ar, const unsigned int version)
		{
			ar & m_x;
			ar & m_y;
			ar & m_angle;
		}

		BOOST_SERIALIZATION_SPLIT_MEMBER()

	public:
		float m_x;
		float m_y;
		float m_angle;
	};
}

#endif