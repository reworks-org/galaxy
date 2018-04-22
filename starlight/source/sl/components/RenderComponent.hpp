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

namespace entt { typedef std::uint32_t Entity; }

namespace sl
{
	class RenderComponent final
	{
		friend class cereal::access;
	public:
		///
		/// Constructor.
		///
		/// \param entity Entity to attach to component fixtures.
		/// \param table sol::table containing data.
		/// 
		RenderComponent(entt::Entity entity, const sol::table& table);

		///
		/// Alternate Constructor.
		///
		/// \param opacity Opacity of Texture / Sprite Texture.
		/// \param textureName Name of the sprite in the VFS.
		///
		RenderComponent(float opacity, const std::string& textureName);

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
		/// Default constructor.
		///
		RenderComponent() = delete;

		///
		/// Cereal serialize function.
		///
		template<class Archive>
		void serialize(Archive& ar)
		{
			ar(m_opacity, m_textureName);
		}

	public:
		float m_opacity;
		std::string m_textureName;
	};
}

#endif