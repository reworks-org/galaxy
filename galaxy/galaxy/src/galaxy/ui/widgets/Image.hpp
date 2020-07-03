///
/// Image.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_IMAGE_HPP_
#define CELESTIAL_IMAGE_HPP_

#include <qs/core/Transform.hpp>

#include "celestial/Widget.hpp"

///
/// Core namespace.
///
namespace celestial
{
	///
	/// Image for UI objects. Does not support tooltips.
	///
	class Image final : public celestial::Widget, public qs::Renderable, public qs::Transform
	{
	public:
		///
		/// Constructor.
		///
		/// \param x x pos.
		/// \param y y pos.
		/// \param region Name of texture region in theme to use.
		/// \param theme This is set for you by UI::add(). Do not set it.
		///
		Image(const float x, const float y, const std::string& region, celestial::UITheme* theme) noexcept;

		///
		/// Destructor.
		///
		~Image() noexcept override;

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
		void update(protostar::ProtectedDouble* dt) noexcept override;

		///
		/// \brief Perform any GL functions on the main thread in prep for rendering.
		///
		/// THIS FUNCTION IS CALLED ON THE MAIN THREAD. PUT YOUR GL CODE HERE.
		///
		/// \param shader Use to set any shader uniforms.
		///
		void render(qs::Shader& shader) noexcept override;

	private:
		///
		/// Render data update flag.
		///
		protostar::ProtectedBool m_updateRender;

		///
		/// Mutex to protect accessing vertex data.
		///
		std::mutex m_vertexMutex;
	};
}

#endif