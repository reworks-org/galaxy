///
/// Layer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_LAYER_HPP_
#define GALAXY_LAYER_HPP_

#include <string>

#include <qs/graphics/Camera.hpp>
#include <protostar/async/ProtectedArithmetic.hpp>

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Represents a "layer" in the engine, encapsulating things like events and rendering.
	///
	class Layer
	{
	public:
		///
		/// Default virtual destructor.
		///
		virtual ~Layer() noexcept = default;

		///
		/// Process layer events.
		///
		virtual void events() noexcept = 0;

		///
		/// Process update (fixed timestep) logic.
		///
		/// \param deltaTime Delta-Time from fixed timestep gameloop.
		///
		virtual void update(pr::ProtectedDouble* deltaTime) noexcept = 0;

		///
		/// \brief Render layer.
		///
		/// Does NOT allow std::exceptions!
		///
		/// \param camera View camera to render with.
		///
		virtual void render(qs::Camera& camera) noexcept = 0;

		///
		/// Set layer name.
		///
		/// \param name Name in std::string format.
		///
		virtual void setName(const std::string& name) noexcept final;

		///
		/// Get layer name.
		///
		/// \return Const std::string.
		///
		virtual const std::string& getName() noexcept final;

	protected:
		///
		/// Default constructor.
		///
		Layer() noexcept;

		///
		/// Argument constructor.
		///
		/// \param name Gives the layer a name.
		///
		explicit Layer(const std::string& name) noexcept;

	protected:
		///
		/// Layer name. Useful to have.
		///
		std::string m_name;
	};
}

#endif