///
/// RenderComponent.hpp
/// starlight
///
/// Created by reworks on 12/01/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_RENDERCOMPONENT_HPP_
#define STARLIGHT_RENDERCOMPONENT_HPP_

#include <vector>

#include "sl/libs/cereal/access.hpp"
#include "sl/libs/sol2/sol_forward.hpp"

namespace sl
{
	///
	/// Allows for an entity to be rendered.
	///
	class RenderComponent final
	{
		friend class cereal::access;
	public:
		///
		/// \brief Default Constructor.
		///
		/// Contains empty data values.
		///
		RenderComponent();

		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		/// 
		RenderComponent(const sol::table& table);

		///
		/// Alternate Constructor.
		///
		/// \param opacity Opacity of Texture / Sprite Texture. From 0.0f - 1.0f.
		/// \param atlasID Name of the sprite in the texture atlas.
		///
		RenderComponent(float opacity, const std::string& atlasID);

		///
		/// Destructor.
		///
		~RenderComponent() = default;

		///
		/// Default move assignment overload.
		///
		RenderComponent& operator=(const RenderComponent&);

	private:
		///
		/// Cereal serialize function.
		///
		template<class Archive>
		void serialize(Archive& ar)
		{
			ar(m_opacity, m_textureName);
		}

	public:
		///
		/// Opacity of entity. From 0.0f - 1.0f.
		///
		float m_opacity;

		///
		/// Name of the sprite or text in the textureatlas to render.
		///
		std::string m_textureName;
	};
}

#endif