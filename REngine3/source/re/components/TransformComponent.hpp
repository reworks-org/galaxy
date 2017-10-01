//
//  TransformComponent.hpp
//  REngine3
//
//  Created by reworks on 30/09/2016.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef RENGINE3_TRANSFORMCOMPONENT_HPP_
#define RENGINE3_TRANSFORMCOMPONENT_HPP_

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include "re/types/Component.hpp"

namespace re
{
	class TransformComponent : public Component
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
		~TransformComponent() override;

	public:
		float m_x;
		float m_y;
		float m_angle;

	private:
		///
		/// Debug function used by IMGUI.
		///
		void debug(sol::table& table, const std::string& curEntityName) override;

		///
		/// Boost.Serialization saving.
		///
		template<class Archive>
		void save(Archive & ar, const unsigned int version) const
		{
			ar & x;
			ar & y;
			ar & angle;
		}

		///
		/// Boost.Serialization loading.
		///
		template<class Archive>
		void load(Archive & ar, const unsigned int version)
		{
			ar & x;
			ar & y;
			ar & angle;
		}

		BOOST_SERIALIZATION_SPLIT_MEMBER()
	};
}

#endif
