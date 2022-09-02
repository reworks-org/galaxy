///
/// Loading.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_LOADING_HPP_
#define GALAXY_CORE_LOADING_HPP_

#include "galaxy/components/Transform.hpp"
#include "galaxy/graphics/Font.hpp"
#include "galaxy/graphics/RenderTexture.hpp"
#include "galaxy/graphics/VertexArray.hpp"
#include "galaxy/graphics/Shader.hpp"
#include "galaxy/graphics/Texture.hpp"
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
		private:
			///
			/// Used to help get text on screen during loading.
			///
			class LoadingText final
			{
			public:
				///
				/// Constructor.
				///
				LoadingText() noexcept;

				///
				/// Destructor.
				///
				~LoadingText() noexcept;

				///
				/// Creates the rendered text.
				///
				/// \param text Text to display. Supports newlines.
				/// \param size Size of text in em.
				/// \param font Font to use.
				/// \param colour Font colour.
				///
				void create(const std::string& text, const float size, graphics::Font& font, const graphics::Colour& colour);

			public:
				///
				/// Text colour.
				///
				graphics::Colour m_colour;

				///
				/// Vertex Array Object.
				///
				graphics::VertexArray m_vao;

				///
				/// Font render texture.
				///
				graphics::RenderTexture m_texture;
			};

		public:
			///
			/// Argument constructor.
			///
			/// \param bg Background texture to load.
			/// \param font Font to use from disk.
			///
			Loading(std::string_view bg, const std::string& font);

			///
			/// Destructor.
			///
			~Loading() noexcept;

			///
			/// Display some text on the screen.
			///
			/// \param text Text to display. Supports newlines.
			/// \param size Size of text in em.
			/// \param colour Font colour.
			/// \param pos Upper-left coords.
			///
			void add_text(const std::string& text, const float size, const graphics::Colour& colour, glm::vec2 pos);

			///
			/// \brief Lua file containing a list of tips.
			///
			/// Format:
			/// tips = {"a?", "b!", "c."}
			///
			/// Will be drawn like: Tip: a? with ul being positioned at provided coords.
			///
			/// \param lua_file File to load from disk.
			/// \param size Size of text in em.
			/// \param colour Font colour.
			/// \param pos Upper-left coords.
			///
			void add_tips(const std::string& lua_file, const float size, const graphics::Colour& colour, glm::vec2 pos);

			///
			/// Start loading job in task pool.
			///
			/// \param lambda Task to call in thread pool.
			///
			void start(const std::function<void(void)>& lambda);

			///
			/// Draw loading screen while loading.
			///
			void display();

		private:
			///
			/// Constructor.
			///
			Loading() noexcept;

		private:
			///
			/// Background shader.
			///
			graphics::Shader m_bg_shader;

			///
			/// Text shader.
			///
			graphics::Shader m_text_shader;

			///
			/// Texture.
			///
			graphics::Texture m_bg;

			///
			/// Font.
			///
			graphics::Font m_font;

			///
			/// Vertex Array Object.
			///
			graphics::VertexArray m_vao;

			///
			/// Background transform.
			///
			components::Transform m_tf;

			///
			/// Handle to thread loading assets.
			///
			std::future<void> m_thread_handle;

			///
			/// Optional text to display on screen.
			///
			std::unique_ptr<LoadingText> m_text;

			///
			/// Text transform.
			///
			components::Transform m_text_tf;

			///
			/// Tip transform.
			///
			components::Transform m_tips_tf;

			///
			/// List of tips to cycle through.
			///
			std::vector<std::unique_ptr<LoadingText>> m_tips;

			///
			/// Shuffled order of tips to display.
			///
			std::vector<std::size_t> m_tip_order;
		};
	} // namespace core
} // namespace galaxy

#endif