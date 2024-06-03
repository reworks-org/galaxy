///
/// Viewport.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_PANELS_VIEWPORT_HPP_
#define SUPERCLUSTER_PANELS_VIEWPORT_HPP_

#include <array>

#include <glm/vec2.hpp>
#include <imgui/imgui.h>

#include <galaxy/graphics/Camera.hpp>
#include <galaxy/graphics/gl/Texture2D.hpp>
#include <galaxy/graphics/RenderTexture.hpp>

using namespace galaxy;

namespace sc
{
	enum class EditorState
	{
		GAME,
		EDITOR,
		GAME_IN_EDITOR
	};

	class Viewport final
	{
	  public:
		Viewport();
		~Viewport();

		void set_active();
		void render(EditorState state, graphics::Camera& editor_camera);

	  public:
		bool                     m_show;
		bool                     m_editor_cam_enabled = true;
		bool                     m_viewport_focused;
		bool                     m_viewport_hovered;
		ImVec2                   m_viewport_size;
		std::array<glm::vec2, 2> m_viewport_bounds;
		graphics::RenderTexture  m_framebuffer;

	  private:
		ImVec2              m_padding;
		graphics::Texture2D m_camera_btn;
		graphics::Texture2D m_editor_cam_btn;
	};
} // namespace sc

#endif
