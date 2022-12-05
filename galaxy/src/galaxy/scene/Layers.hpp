///
/// Layers.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_STATE_LAYERS_HPP_
#define GALAXY_STATE_LAYERS_HPP_

#include "galaxy/scene/LayerRegistry.hpp"

namespace galaxy
{
	namespace scene
	{
		///
		/// Stack based storage for layers.
		///
		class Layers final : public fs::Serializable
		{
			///
			/// Typedef for layer stack.
			///
			using LayerStack = std::vector<std::shared_ptr<Layer>>;

			///
			/// Typedef for layer storage.
			///
			using LayerContainer = robin_hood::unordered_node_map<std::string, std::shared_ptr<Layer>>;

		public:
			///
			/// Argument Constructor.
			///
			/// \param scene Scene these layers belong to.
			///
			Layers(Scene* scene) noexcept;

			///
			/// Destructor.
			///
			virtual ~Layers() noexcept;

			///
			/// Create a layer.
			///
			/// \tparam ChildLayer Derived layer type.
			///
			/// \param name Layer name.
			///
			/// \return Shared pointer to newly created layer.
			///
			template<is_layer ChildLayer>
			[[maybe_unused]] std::weak_ptr<ChildLayer> make(const std::string& name);

			///
			/// Get a specific layer.
			///
			/// \tparam ChildLayer Derived layer type.
			///
			/// \param name Layer name.
			///
			/// \return Shared pointer to layer.
			///
			template<is_layer ChildLayer>
			[[nodiscard]] std::weak_ptr<ChildLayer> get(const std::string& name);

			///
			/// Push a layer based on name.
			///
			/// \param name Name of the layer to push.
			///
			void push(const std::string& name);

			///
			/// Destroy top-most layer.
			///
			void pop();

			///
			/// Pop a layer based on name.
			///
			void pop(const std::string& name);

			///
			/// Process layer events.
			///
			void events();

			///
			/// Process layer updates based on events.
			///
			void update();

			///
			/// Render all layers.
			///
			void render();

			///
			/// \brief Destroy all layers.
			///
			/// Also resets layer stack.
			///
			void clear();

			///
			/// Get layer stack.
			///
			/// \return Const ref to layer stack.
			///
			const LayerStack& stack() const noexcept;

			///
			/// Get layer cache.
			///
			/// \return Const ref to layer cache.
			///
			LayerContainer& cache() noexcept;

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to write out.
			///
			[[nodiscard]] nlohmann::json serialize() override;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			void deserialize(const nlohmann::json& json) override;

		private:
			///
			/// Constructor.
			///
			Layers() = delete;

			///
			/// Copy constructor.
			///
			Layers(const Layers&) = delete;

			///
			/// Move constructor.
			///
			Layers(Layers&&) = delete;

			///
			/// Copy assignment operator.
			///
			Layers& operator=(const Layers&) = delete;

			///
			/// Move assignment operator.
			///
			Layers& operator=(Layers&&) = delete;

		private:
			///
			/// Holds layer data.
			///
			LayerContainer m_layers;

			///
			/// Simulate a stack using a std::vector.
			///
			LayerStack m_stack;

			///
			/// Scene pointer.
			///
			Scene* m_scene;
		};

		template<is_layer ChildLayer>
		inline std::weak_ptr<ChildLayer> Layers::make(const std::string& name)
		{
			for (const auto& [key, layer] : m_layers)
			{
				if (layer->get_type() == "Runtime")
				{
					GALAXY_LOG(GALAXY_ERROR, "Runtime layer already exists.");
					return {};
				}
			}

			if (!m_layers.contains(name))
			{
				m_layers[name] = std::make_shared<ChildLayer>(name, m_scene);
				return std::static_pointer_cast<ChildLayer>(m_layers[name]);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Layer '{0}' already exists.", name);
				return {};
			}
		}

		template<is_layer ChildLayer>
		inline std::weak_ptr<ChildLayer> Layers::get(const std::string& name)
		{
			if (m_layers.contains(name))
			{
				return std::static_pointer_cast<ChildLayer>(m_layers[name]);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Layer with name '{0}' does not exist.", name);
				return {};
			}
		}
	} // namespace scene
} // namespace galaxy

#endif