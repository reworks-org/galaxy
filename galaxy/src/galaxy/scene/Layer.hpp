///
/// Layer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_STATE_LAYER_HPP_
#define GALAXY_STATE_LAYER_HPP_

#include <concepts>

#include <entt/signal/dispatcher.hpp>

#include "galaxy/fs/Serializable.hpp"

namespace galaxy
{
	namespace core
	{
		class Window;
	} // namespace core

	namespace scene
	{
		class Scene;

		///
		/// A layer is a contained part of your game engine.
		/// I.e. A scene would have a runtime layer and a UI layer.
		/// It then might toggle a debug ui layer on top.
		///
		class Layer : public fs::Serializable
		{
		public:
			///
			/// Argument Constructor.
			///
			/// \param name Name of the Layer.
			/// \param scene Pointer to scene this layer belongs to.
			///
			Layer(std::string_view name, Scene* scene) noexcept;

			///
			/// Destructor.
			///
			virtual ~Layer() noexcept;

			///
			/// On push of Layer to stack.
			///
			virtual void on_push() = 0;

			///
			/// On pop of Layer off stack.
			///
			virtual void on_pop() = 0;

			///
			/// Handle events.
			///
			virtual void events() = 0;

			///
			/// Process events/updates and prepare for rendering.
			///
			virtual void update() = 0;

			///
			/// Render layer.
			///
			virtual void render() = 0;

			///
			/// Set layer name.
			///
			/// \param name String name for debug purposes.
			///
			void set_name(std::string_view name) noexcept;

			///
			/// Get layer name.
			///
			/// \return Const string reference.
			///
			[[nodiscard]] const std::string& get_name() const noexcept;

			///
			/// Get a reference to this layers event dispatcher.
			///
			/// \return Reference to entt::dispatcher.
			///
			[[nodiscard]] entt::dispatcher& dispatcher() noexcept;

			///
			/// Get layer type.
			///
			/// \return String.
			///
			[[nodiscard]] virtual const std::string& get_type() const noexcept = 0;

		protected:
			///
			/// Layer name.
			///
			std::string m_name;

			///
			/// Scene pointer.
			///
			Scene* m_scene;

			///
			/// Event dispatcher.
			///
			entt::dispatcher m_dispatcher;

		private:
			///
			/// Constructor.
			///
			Layer() = delete;
		};

		///
		/// Concept to restrict templates to layers.
		///
		/// \tparam Type Type to constrain.
		///
		template<typename Type>
		concept is_layer = std::derived_from<Type, Layer>;
	} // namespace scene
} // namespace galaxy

#endif