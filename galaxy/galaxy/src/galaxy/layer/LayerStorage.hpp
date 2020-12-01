///
/// LayerStorage.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_LAYERSTORAGE_HPP_
#define GALAXY_LAYERSTORAGE_HPP_

#include <memory>
#include <vector>

#include <protostar/system/Concepts.hpp>

#include "galaxy/layer/Layer.hpp"

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Is a layer class.
	///
	template<typename Type>
	concept is_layer = std::is_base_of<Layer, Type>::value;

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
		template<is_layer Layer, typename... Args>
		void add(Args&&... args);

		///
		/// Retrieve a layer from the provided name.
		///
		/// \param name Name of the layer.
		///
		/// \return Pointer to layer.
		///
		template<is_layer Layer>
		[[nodiscard]] Layer* get(std::string_view name);

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
		/// Destroy top-most layer.
		///
		void pop();

		///
		/// Remove layer based on name.
		///
		/// \param name Name of the layer to remove.
		///
		void remove(std::string_view name);

		///
		/// Destroy all layers.
		///
		void clear();

	private:
		///
		/// Stores layers in contiguous memory.
		///
		std::vector<std::unique_ptr<galaxy::Layer>> m_layers;
	};

	template<is_layer Layer, typename... Args>
	inline void LayerStorage::add(Args&&... args)
	{
		m_layers.emplace_back(std::make_unique<Layer>(std::forward<Args>(args)...));
	}

	template<is_layer Layer>
	inline Layer* LayerStorage::get(std::string_view name)
	{
		auto res = std::find_if(m_layers.begin(), m_layers.end(), [&](const auto&& layer) {
			return layer->get_name() == static_cast<std::string>(name);
		});

		if (res != m_layers.end())
		{
			return dynamic_cast<Layer*>(*res.get());
		}
		else
		{
			return nullptr;
		}
	}
} // namespace galaxy

#endif