///
/// PostEffect.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_POSTEFFECT_HPP_
#define GALAXY_GRAPHICS_POSTEFFECT_HPP_

#include <concepts>

namespace galaxy
{
	namespace graphics
	{
		///
		/// Manages post processing effects to apply to combined scene.
		///
		class PostEffect
		{
		  public:
			///
			/// Destructor.
			///
			virtual ~PostEffect() = default;

			///
			/// Resize framebuffers.
			///
			/// \param width New width of internal framebuffer.
			/// \param height New height of internal framebuffer.
			///
			virtual void resize(const int width, const int height) = 0;

			///
			/// Render effect to input texture.
			///
			/// \param input Input texture from previous effect or window framebuffer.
			///
			/// \return Returns output texture for next effect or final framebuffer.
			///
			[[nodiscard]] virtual unsigned int render(const unsigned int input) = 0;

			///
			/// Is this effect enabled?
			///
			/// \return True if effect is enabled. Otherwise false.
			///
			[[nodiscard]] virtual bool is_enabled() = 0;

		  protected:
			///
			/// Constructor.
			///
			PostEffect() = default;

			///
			/// Copy constructor.
			///
			PostEffect(const PostEffect&) = default;

			///
			/// Move constructor.
			///
			PostEffect(PostEffect&&) = default;

			///
			/// Copy assignment operator.
			///
			PostEffect& operator=(const PostEffect&) = default;

			///
			/// Move assignment operator.
			///
			PostEffect& operator=(PostEffect&&) = default;
		};

		///
		/// Constraint to ensure type is a Post Effect.
		///
		/// \tparam T Type to test.
		///
		template<typename T>
		concept is_posteffect = std::derived_from<T, PostEffect>;
	} // namespace graphics
} // namespace galaxy

#endif
