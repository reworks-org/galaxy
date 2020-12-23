///
/// PostEffect.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_POSTEFFECT_HPP_
#define QUASAR_POSTEFFECT_HPP_

#include "qs/core/Shader.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Post effect to apply to final image output.
	///
	class PostEffect
	{
		friend class Window;

	public:
		///
		/// Default destructor.
		///
		virtual ~PostEffect() = default;

		///
		/// Shader.
		///
		qs::Shader m_shader;

	protected:
		///
		/// Default constructor.
		///
		PostEffect() = default;

	private:
		///
		/// \brief Set the appropriate uniforms for the shader.
		///
		/// Called internally in the derived effect, then called by Window.
		///
		virtual void apply_uniforms() = 0;
	};
} // namespace qs

#endif