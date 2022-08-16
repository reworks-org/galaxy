///
/// Loading.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_LOADING_HPP_
#define GALAXY_CORE_LOADING_HPP_

#include "galaxy/components/Transform.hpp"
#include "galaxy/graphics/Shader.hpp"
#include "galaxy/graphics/Texture.hpp"
#include "galaxy/graphics/VertexArray.hpp"
#include "galaxy/meta/Async.hpp"

struct GLFWwindow;

namespace galaxy
{
	namespace core
	{
		///
		/// Simple customizable loading screen.
		///
		class Loading final
		{
		public:
			///
			/// Constructor.
			///
			Loading() noexcept;

			///
			/// Destructor.
			///
			~Loading() noexcept;

			///
			///
			///

			void prep_window_for_loading() noexcept;
			void display_loadingscreen(std::string_view background) noexcept;
			void start_offthread_loading(const std::function<void(void)>& lambda);

			/// \param texture Texture file in the VFS.
			///
			/// draws to the bottom right of screen.
			/// rotated.
			void display_bg_until_finished(std::string_view background, std::string_view logo) noexcept;

		private:
			///
			/// Shader.
			///
			graphics::Shader m_shader;

			///
			/// Texture.
			///
			graphics::Texture m_bg;

			///
			/// Texture.
			///
			graphics::Texture m_animated;

			///
			/// Vertex Array Object.
			///
			graphics::VertexArray m_vao;

			///
			/// Vertex Array Object.
			///
			graphics::VertexArray m_anim_vao;

			///
			/// Animated texture transform.
			///
			components::Transform m_transform;

			///
			/// Default texture transform.
			///
			components::Transform m_default;

			///
			/// Handle to thread loading assets.
			///
			std::future<void> m_loading_thread;
		};

	} // namespace core
} // namespace galaxy

#endif