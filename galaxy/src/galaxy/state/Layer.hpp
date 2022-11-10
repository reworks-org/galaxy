///
/// Layer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_STATE_LAYER_HPP_
#define GALAXY_STATE_LAYER_HPP_

#include <concepts>

#include "galaxy/core/World.hpp"
#include "galaxy/fs/Serializable.hpp"

namespace galaxy
{
	namespace core
	{
		class Window;
	} // namespace core

	namespace state
	{
		class Scene;

		///
		/// A layer is usually a layering of events/updates/rendering grouped together that dont interact.
		/// I.e. UI, Debug UI, Game, etc.
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
			/// Get layer type.
			///
			/// \return String.
			///
			[[nodiscard]] virtual const std::string& get_type() const noexcept = 0;

			///
			/// Only update the rendersystem.
			///
			void update_rendersystem();

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
			/// Get layer entity world.
			///
			/// \return Reference to World.
			///
			[[nodiscard]] core::World& world() noexcept;

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
			/// Entitys, events, etc.
			///
			core::World m_world;

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
	} // namespace state
} // namespace galaxy

#endif