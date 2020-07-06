///
/// LayerStorage.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_LAYERSTORAGE_HPP_
#define GALAXY_LAYERSTORAGE_HPP_

#include <vector>

#include "galaxy/layer/Layer.hpp"

///
/// Core namespace. 
///
namespace galaxy
{
	///
	/// Storage for layers in a state.
	///
	class LayerStorage final
	{
	public:
		///
		/// Constructor.
		///
		LayerStorage() noexcept;

		///
		/// Destructor.
		///
		~LayerStorage() noexcept;

		///
		/// Constructs a Layer from provided arguments.
		///
		/// \param args Arguments to construct Layer with.
		///
		template<typename Layer, typename... Args>
		void add(Args&& ...args) noexcept;

		///
		/// Retrieve a layer from the provided name.
		///
		/// \param name Name of the layer.
		///
		/// \return Pointer to layer.
		///
		template<typename Layer>
		Layer* get(const std::string& name) noexcept;

		///
		/// Process all layer events.
		///
		void events() noexcept;

		///
		/// Update all layers.
		///
		/// \param deltaTime Pointer to main loop delta time.
		///
		void update(protostar::ProtectedDouble* deltaTime) noexcept;

		///
		/// Render all layers.
		///
		/// \param camera Camera to render layers with.
		///
		void render(qs::Camera& camera) noexcept;

		///
		/// Destroy top-most layer.
		///
		void pop() noexcept;

		///
		/// Remove layer based on name.
		///
		/// \param name Name of the layer to remove.
		///
		void remove(const std::string& name) noexcept;

		///
		/// Destroy all layers.
		///
		void clear() noexcept;

	private:
		///
		/// Stores layers in contiguous memory.
		///
		std::vector<std::unique_ptr<galaxy::Layer>> m_layers;
	};

	template<typename Layer, typename ...Args>
	inline void LayerStorage::add(Args&& ...args) noexcept
	{
		m_layers.emplace_back(std::make_unique<Layer>(std::forward<Args>(args)...));
	}

	template<typename Layer>
	inline Layer* LayerStorage::get(const std::string& name) noexcept
	{
		auto pos = std::find_if(m_layers.begin(), m_layers.end(), [&](std::unique_ptr<Layer> const& layer)
		{
			return layer->getName() == name;
		});

		if (pos != m_layers.end())
		{
			return dynamic_cast<Layer*>(*pos);
		}
		else
		{
			return nullptr;
		}
	}
}

#endif