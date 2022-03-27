///
/// PostProcess.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_POSTPROCESSOR_HPP_
#define GALAXY_GRAPHICS_POSTPROCESSOR_HPP_

#include "galaxy/graphics/postfx/SMAA.hpp"
#include "galaxy/graphics/postfx/Sharpen.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Manages post processing effects to apply to combined scene.
		///
		class PostProcess final
		{
		public:
			///
			/// Argument constructor.
			///
			/// \param width Width of internal framebuffer.
			/// \param height Height of internal framebuffer.
			///
			PostProcess(const int width, const int height) noexcept;

			///
			/// Destructor.
			///
			~PostProcess() noexcept;

			///
			/// \brief Add an effect to process.
			///
			/// \tparam Effect Post-Processing Effect to use.
			/// \tparam Args Variadic Arguments.
			///
			/// Make sure the effects are in the correct order.
			/// They are rendered in the order they are added.
			///
			/// \param args Constructor arguments.
			///
			/// \return Reference to newly created effect.
			///
			template<is_posteffect Effect, typename... Args>
			[[maybe_unused]] Effect& add(Args&&... args);

			///
			/// Bind to draw to post processor framebuffer.
			///
			void bind() noexcept;

			///
			/// Unbind to draw to post processor framebuffer.
			///
			void unbind() noexcept;

			///
			/// Draw post effects to stored framebuffer.
			///
			void render_effects();

			///
			/// Draw finished post effects to default framebuffer (screen).
			///
			void render_output();

			///
			/// Resize framebuffers.
			///
			/// \param width New width of internal framebuffer.
			/// \param height New height of internal framebuffer.
			///
			void resize(const int width, const int height);

		private:
			///
			/// Constructor.
			///
			PostProcess() = delete;

		private:
			///
			/// For geometry and lighting.
			///
			RenderTexture m_fb;

			///
			/// Simple output shader.
			///
			Shader m_output;

			///
			/// List of effects to apply in order.
			///
			std::vector<std::shared_ptr<PostEffect>> m_effects;

			///
			/// Simple quad to draw when applying effects (buffer).
			///
			unsigned int m_screen_vbo;

			///
			/// Simple quad to draw when applying effects (array).
			///
			unsigned int m_screen_vao;

			///
			/// Output cache.
			///
			unsigned int m_output_fb;
		};

		template<is_posteffect Effect, typename... Args>
		inline Effect& PostProcess::add(Args&&... args)
		{
			auto effect = std::make_shared<Effect>(std::forward<Args>(args)...);

			m_effects.push_back(std::static_pointer_cast<PostEffect>(effect));
			return *effect;
		}
	} // namespace graphics
} // namespace galaxy

#endif