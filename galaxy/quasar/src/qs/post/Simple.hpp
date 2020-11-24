///
/// Simple.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_SIMPLE_HPP_
#define QUASAR_SIMPLE_HPP_

#include "qs/post/PostEffect.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// PostEffect identifier.
	///
	namespace effect
	{
		///
		/// Draws output straight to screen with no effects.
		///
		class Simple final : public PostEffect
		{
		public:
			///
			/// Constructor.
			///
			Simple() = default;

			///
			/// Destructor.
			///
			virtual ~Simple() = default;

		private:
			///
			/// \brief Set the appropriate uniforms for the shader.
			///
			/// Called internally in the derived effect, then called by Window.
			///
			void apply_uniforms() override;
		};
	} // namespace effect
} // namespace qs

#endif