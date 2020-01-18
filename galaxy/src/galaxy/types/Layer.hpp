///
/// Layer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_LAYER_HPP_
#define GALAXY_LAYER_HPP_

#include <string>

// Forward decs.
namespace sf { class Event; class Time; }

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
		virtual ~Layer() = default;

		///
		/// \brief Process layer events.
		///
		/// Allows for std::exceptions.
		///
		/// \param event SFML structure containing polled events.
		///
		virtual void event(const sf::Event& event) = 0;

		///
		/// \brief Process update (fixed timestep) logic.
		///
		/// Allows for std::exceptions.
		///
		/// \param dt Delta-Time from fixed timestep gameloop.
		///
		virtual void update(const sf::Time& dt) = 0;

		///
		/// \brief Render layer.
		///
		/// Does NOT allow std::exceptions!
		///
		virtual void render() noexcept = 0;

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