///
/// PostProcess.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_POSTPROCESSOR_HPP_
#define GALAXY_GRAPHICS_POSTPROCESSOR_HPP_

#include "galaxy/graphics/post/ChromaticAberration.hpp"
#include "galaxy/graphics/post/FilmicGrain.hpp"
#include "galaxy/graphics/post/GammaCorrection.hpp"
#include "galaxy/graphics/post/GaussianBlur.hpp"
#include "galaxy/graphics/post/Sharpen.hpp"
#include "galaxy/graphics/post/SMAA.hpp"
#include "galaxy/meta/Memory.hpp"

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
			/// Constructor.
			///
			PostProcess();

			///
			/// Destructor.
			///
			~PostProcess();

			///
			/// Initialize post processor and GL buffers.
			///
			/// \param width Width of internal framebuffer.
			/// \param height Height of internal framebuffer.
			///
			void init(const int width, const int height);

			///
			/// Cleanup used memory.
			///
			void destroy();

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
			/// \return Weak pointer to newly created effect.
			///
			template<is_posteffect Effect, typename... Args>
			[[maybe_unused]] Effect* add(Args&&... args);

			///
			/// Bind to draw to post processor framebuffer.
			///
			void bind();

			///
			/// Unbind to draw to post processor framebuffer.
			///
			void unbind();

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
			meta::vector<std::unique_ptr<graphics::PostEffect>> m_effects;

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
		inline Effect* PostProcess::add(Args&&... args)
		{
			m_effects.push_back(std::make_unique<Effect>(std::forward<Args>(args)...));
			return dynamic_cast<Effect*>(m_effects.back().get());
		}
	} // namespace graphics
} // namespace galaxy

#endif
