///
/// LayerStorage.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_LAYERSTORAGE_HPP_
#define GALAXY_CORE_LAYERSTORAGE_HPP_

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
		/// Is a layer class.
		///
		template<typename Type>
		concept is_layer = std::derived_from<Type, Layer>;

		///
		/// Storage for layers in a state.
		///
		class LayerStorage final
		{
		public:
			///
			/// Constructor.
			///
			LayerStorage() = default;

			///
			/// Copy constructor.
			///
			LayerStorage(const LayerStorage&) = delete;

			///
			/// Move constructor.
			///
			LayerStorage(LayerStorage&&) = delete;

			///
			/// Copy assignment operator.
			///
			LayerStorage& operator=(const LayerStorage&) = delete;

			///
			/// Move assignment operator.
			///
			LayerStorage& operator=(LayerStorage&&) = delete;

			///
			/// Destructor.
			///
			~LayerStorage();

			///
			/// Constructs a Layer from provided arguments.
			///
			/// \param args Arguments to construct Layer with.
			///
			template<is_layer DerivedLayer, typename... Args>
			void add(Args&&... args);

			///
			/// Retrieve a layer from the provided name.
			///
			/// \param name Name of the layer.
			///
			/// \return Pointer to layer.
			///
			template<is_layer DerivedLayer>
			[[nodiscard]] DerivedLayer* get(std::string_view name);

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
			[[nodiscard]] std::vector<std::string> get_layer_keys();

		private:
			///
			/// Holds layer data.
			///
			robin_hood::unordered_map<std::string, std::shared_ptr<Layer>> m_layers;

			///
			/// Allows for layer access in contiguous memory.
			///
			std::vector<std::shared_ptr<Layer>> m_stack;
		};

		template<is_layer DerivedLayer, typename... Args>
		inline void LayerStorage::add(Args&&... args)
		{
			std::shared_ptr<DerivedLayer> layer = std::make_shared<DerivedLayer>(std::forward<Args>(args)...);

			auto layer_base = std::static_pointer_cast<Layer>(layer);

			m_layers[layer->get_name()] = layer_base;
			m_stack.push_back(layer_base);
		}

		template<is_layer DerivedLayer>
		inline DerivedLayer* LayerStorage::get(std::string_view name)
		{
			const auto str = static_cast<std::string>(name);
			if (m_layers.contains(str))
			{
				return dynamic_cast<DerivedLayer*>(m_layers[str].get());
			}

			return nullptr;
		}
	} // namespace core
} // namespace galaxy

#endif