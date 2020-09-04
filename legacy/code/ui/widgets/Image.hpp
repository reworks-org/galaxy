///
/// Image.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_IMAGE_HPP_
#define GALAXY_IMAGE_HPP_

#include "galaxy/ui/Widget.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Image for UI objects. Does not support tooltips.
	///
	class Image final : public galaxy::Widget
	{
	public:
		///
		/// Constructor.
		///
		/// \param theme Theme from UI to apply to widget.
		/// \param textureID Name of texture region in theme texture atlas to use.
		/// \param x x pos.
		/// \param y y pos.
		///
		Image(galaxy::Theme* theme, std::string_view textureID, const float x, const float y) noexcept;

		///
		/// Destructor.
		///
		~Image() noexcept;

	private:
		///
		/// Activate widget.
		///
		void activate() noexcept override;

		///
		/// Deactivate widget.
		///
		void deactivate() noexcept override;

		///
		/// \brief Update the widget.
		///
		/// YOU MUST NOT CALL ANY GL CODE FROM THIS FUNCTION. THIS FUNCTION IS CALLED FROM A SEPERATE THREAD.
		///
		/// \param dt Delta Time.
		///
		void update(pr::ProtectedDouble* dt) noexcept override;

		///
		/// \brief Perform any GL functions on the main thread in prep for rendering.
		///
		/// THIS FUNCTION IS CALLED ON THE MAIN THREAD. PUT YOUR GL CODE HERE.
		///
		/// \param camera Camera projection to apply to GUI.
		///
		void render(qs::Camera& camera) noexcept override;

	private:
		///
		/// Image sprite.
		///
		qs::BatchedSprite m_sprite;
	};
}

#endif