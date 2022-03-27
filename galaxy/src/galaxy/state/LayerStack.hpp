///
/// LayerStack.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_STATE_LAYERSTACK_HPP_
#define GALAXY_STATE_LAYERSTACK_HPP_

#include <stack>

#include <robin_hood.h>

#include "galaxy/state/Layer.hpp"

namespace galaxy
{
	namespace state
	{
		///
		/// Typedef for layer storage.
		///
		using LayerContainer = robin_hood::unordered_node_map<std::string, std::shared_ptr<Layer>>;

		///
		/// Stack based storage for layers.
		///
		class LayerStack final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			LayerStack() noexcept;

			///
			/// Destructor.
			///
			virtual ~LayerStack() noexcept;

			///
			/// Create a layer.
			///
			/// \param name Layer name.
			///
			/// \return Pointer to newly created layer.
			///
			[[maybe_unused]] std::shared_ptr<Layer> make(const std::string& name);

			///
			/// Get a specific layer.
			///
			/// \param name Layer name.
			///
			/// \return Shared pointer to layer.
			///
			[[nodiscard]] std::shared_ptr<Layer> get(const std::string& name);

			///
			/// Get the layer at the top of the stack.
			///
			/// \return Pointer to layer.
			///
			[[nodiscard]] std::shared_ptr<Layer> top();

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
			/// Process layer stack.
			///
			void events();

			///
			/// Update layer stack.
			///
			void update();

			///
			/// Perform pre-rendering operations outside of opengl begin/end of rendering.
			///
			void pre_render();

			///
			/// Render all layers.
			///
			void render();

			///
			/// Destroy all layers.
			///
			void clear();

			///
			/// Get layer stack.
			///
			/// \return Array of pointers to layers.
			///
			[[nodiscard]] std::vector<std::shared_ptr<Layer>>& get_stack() noexcept;

			///
			/// Get all layers.
			///
			/// \return Unordered node map containing all stack layers.
			///
			[[nodiscard]] LayerContainer& get_layers() noexcept;

			///
			/// Get an array of all layer keys.
			///
			/// \return Vector of strings.
			///
			[[nodiscard]] std::vector<std::string> get_layer_keys() noexcept;

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
			LayerContainer m_layers;

			///
			/// Simulate a stack using a std::vector.
			///
			std::vector<std::shared_ptr<Layer>> m_stack;
		};
	} // namespace state
} // namespace galaxy

#endif