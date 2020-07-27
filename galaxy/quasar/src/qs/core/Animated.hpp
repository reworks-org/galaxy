///
/// Animated.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_ANIMATED_HPP_
#define QUASAR_ANIMATED_HPP_

#include <glm/vec2.hpp>

///
/// Core namespace.
///
namespace qs
{
	///
	/// Allows for an object to be animated.
	///
	class Animated
	{
	public:
		///
		/// Constructor.
		///
		Animated() noexcept;

		///
		/// Virtual destructor.
		///
		virtual ~Animated() noexcept = default;

		///
		/// Updates.
		///
		virtual void update() = 0;

		///
		/// \brief Sets the top left texture coords.
		///
		/// This sets, and thus overrides current position.
		///
		/// \param x New x position.
		/// \param y New y position.
		///
		void set_texels(const float x, const float y) noexcept;

		///
		/// \brief Moves the texture coords.
		///
		/// This is additive. This does not set.
		///
		/// \param x Amount to move on x axis.
		/// \param y Amount to move on y axis.
		///
		void move_texels(const float x, const float y) noexcept;

		///
		/// Retrieve texture transform.
		///
		/// \return Reference to a glm::vec2. X/Y is U/V.
		///
		[[nodiscard]] glm::vec2& get_texels() noexcept;

	protected:
		///
		/// Update flag.
		///
		bool m_dirty_texels;

		///
		/// Texture transformation.
		///
		glm::vec2 m_texels;
	};
} // namespace qs

#endif