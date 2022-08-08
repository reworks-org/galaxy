///
/// LayerRegistry.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_STATE_LAYERREGISTRY_HPP_
#define GALAXY_STATE_LAYERREGISTRY_HPP_

#include <robin_hood.h>

#include "galaxy/error/Log.hpp"
#include "galaxy/state/Layer.hpp"

namespace galaxy
{
	namespace state
	{
		class LayerRegistry final
		{
			///
			/// Allows for defining types at runtime to be created based on a string.
			///
			using LayerTypeFactory = robin_hood::unordered_flat_map<std::string, std::function<std::shared_ptr<Layer>(std::string_view, Scene*)>>;

		public:
			///
			/// Register a layer type for serialization / deserialization purposes.
			///
			/// \tparam Layer Derived layer type.
			///
			/// \param type Type as a string.
			///
			template<is_layer Layer>
			inline static void register_type(const std::string& type)
			{
				if (!m_registry.contains(type))
				{
					m_registry[type] = [](std::string_view name, Scene* scene) -> std::shared_ptr<Layer> {
						return std::make_shared<Layer>(name, scene);
					};
				}
				else
				{
					GALAXY_LOG(GALAXY_WARNING, "Attempted to register duplicate type '{0}'.", type);
				}
			}

		public:
			///
			/// Layer type registry.
			///
			inline static LayerTypeFactory m_registry;

		private:
			///
			/// Constructor.
			///
			LayerRegistry() = delete;

			///
			/// Copy constructor.
			///
			LayerRegistry(const LayerRegistry&) = delete;

			///
			/// Move constructor.
			///
			LayerRegistry(LayerRegistry&&) = delete;

			///
			/// Copy assignment operator.
			///
			LayerRegistry& operator=(const LayerRegistry&) = delete;

			///
			/// Move assignment operator.
			///
			LayerRegistry& operator=(LayerRegistry&&) = delete;
		};
	} // namespace state
} // namespace galaxy

#endif