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
		/// \brief Sets the top left texture coords.
		///
		/// This sets, and thus overrides current position.
		///
		/// \param x New x position.
		/// \param y New y position.
		///
		virtual void setTexels(const float x, const float y) noexcept final;

		///
		/// \brief Moves the texture coords.
		///
		/// This is additive. This does not set.
		///
		/// \param x Amount to move on x axis.
		/// \param y Amount to move on y axis.
		///
		virtual void moveTexels(const float x, const float y) noexcept final;

		///
		/// Updates.
		///
		virtual void update() noexcept = 0;

		///
		/// Retrieve texture transform.
		///
		/// \return Reference to a glm::vec2. X/Y is U/V.
		///
		virtual glm::vec2& getTexels() noexcept final;

	protected:
		///
		/// Update flag.
		///
		bool m_areTexelsDirty;

		///
		/// Texture transformation.
		///
		glm::vec2 m_texels;
	};
}

#endif