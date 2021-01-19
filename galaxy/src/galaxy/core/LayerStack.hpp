///
/// LayerStack.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_LAYERSTACK_HPP_
#define GALAXY_CORE_LAYERSTACK_HPP_

#include <memory>
#include <vector>

#include <robin_hood.h>

#include "galaxy/core/Layer.hpp"
#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace core
	{
		///
		/// Restrict template types to only Layers.
		///
		template<typename Type>
		concept is_layer = std::derived_from<Type, Layer>;

		///
		/// Stack based storage for layers within a scene.
		///
		class LayerStack final
		{
		public:
			///
			/// Constructor.
			///
			LayerStack() noexcept = default;

			///
			/// Destructor.
			///
			~LayerStack();

			///
			/// Constructs a Layer from provided arguments.
			///
			/// \param args Arguments to construct Layer with.
			///
			template<is_layer DerivedLayer, typename... Args>
			[[maybe_unused]] std::shared_ptr<DerivedLayer> create(std::string_view name, Args&&... args);

			///
			/// Retrieve a layer from the provided name.
			///
			/// \param name Name of the layer.
			///
			/// \return Pointer to layer.
			///
			template<is_layer DerivedLayer>
			[[nodiscard]] std::shared_ptr<DerivedLayer> get(std::string_view name);

			///
			/// Get the layer on top of the stack.
			///
			/// \return Returns pointer to topmost layer.
			///
			template<is_layer DerivedLayer>
			[[nodiscard]] std::shared_ptr<DerivedLayer> top();

			///
			/// Process all layer events.
			///
			void events();

			///
			/// Update all layers.
			///
			/// \param dt Delta time from gameloop.
			///
			void update(const double dt);

			///
			/// Perform pre-rendering operations outside of opengl begin/end of rendering.
			///
			void pre_render();

			///
			/// Render all layers.
			///
			void render();

			///
			/// Push a layer based on name.
			///
			/// \param name Name of the layer to push.
			///
			void push(std::string_view name);

			///
			/// Destroy top-most layer.
			///
			void pop();

			///
			/// Destroy all layers.
			///
			void clear();

			///
			/// Get an array of all layer keys.
			///
			/// \return Vector of strings.
			///
			[[nodiscard]] std::vector<std::string> get_layer_keys() noexcept;

		private:
			///
			/// Copy constructor.
			///
			LayerStack(const LayerStack&) = delete;

			///
			/// Move constructor.
			///
			LayerStack(LayerStack&&) = delete;

			///
			/// Copy assignment operator.
			///
			LayerStack& operator=(const LayerStack&) = delete;

			///
			/// Move assignment operator.
			///
			LayerStack& operator=(LayerStack&&) = delete;

		private:
			///
			/// Holds layer data.
			///
			robin_hood::unordered_map<std::string, std::shared_ptr<Layer>> m_layers;

			///
			/// Contiguous
			///
			std::vector<std::shared_ptr<Layer>> m_stack;
		};

		template<is_layer DerivedLayer, typename... Args>
		inline std::shared_ptr<DerivedLayer> LayerStack::create(std::string_view name, Args&&... args)
		{
			const auto str = static_cast<std::string>(name);

			if (m_layers.contains(str))
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to create state that already exists!");
			}
			else
			{
				std::shared_ptr<Layer> layer = std::make_shared<DerivedLayer>(std::forward<Args>(args)...);
				layer->set_name(str);
				m_layers[str] = layer;

				return std::static_pointer_cast<DerivedLayer>(layer);
			}
		}

		template<is_layer DerivedLayer>
		inline std::shared_ptr<DerivedLayer> LayerStack::top()
		{
			if (!m_stack.empty())
			{
				return std::static_pointer_cast<DerivedLayer>(m_stack.back());
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "No layers in stack!");
				return nullptr;
			}
		}

		template<is_layer DerivedLayer>
		inline std::shared_ptr<DerivedLayer> LayerStack::get(std::string_view name)
		{
			const auto str = static_cast<std::string>(name);
			if (m_layers.contains(str))
			{
				return std::static_pointer_cast<DerivedLayer>(m_layers[str]);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to retrieve non-existant layer: {0}.", name);
				return nullptr;
			}
		}
	} // namespace core
} // namespace galaxy

#endif