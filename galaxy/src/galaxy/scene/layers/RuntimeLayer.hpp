///
/// RuntimeLayer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_STATE_LAYER_RUNTIMELAYER_HPP_
#define GALAXY_STATE_LAYER_RUNTIMELAYER_HPP_

#include "galaxy/scene/Layer.hpp"

namespace galaxy
{
	namespace scene
	{
		///
		/// Layer handling game logic.
		///
		class RuntimeLayer final : public Layer
		{
		public:
			///
			/// Argument Constructor.
			///
			/// \param name Name of the Layer.
			/// \param scene Pointer to scene this layer belongs to.
			///
			RuntimeLayer(std::string_view name, Scene* scene);

			///
			/// Destructor.
			///
			virtual ~RuntimeLayer();

			///
			/// On push of RuntimeLayer to stack.
			///
			void on_push() override;

			///
			/// On pop of RuntimeLayer off stack.
			///
			void on_pop() override;

			///
			/// Handle events.
			///
			void events() override;

			///
			/// Process events/updates and prepare for rendering.
			///
			void update() override;

			///
			/// Render layer.
			///
			void render() override;

			///
			/// Get layer type.
			///
			/// \return String.
			///
			[[nodiscard]] const std::string& get_type() const override;

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to be serialized.
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
			RuntimeLayer() = delete;
		};
	} // namespace scene
} // namespace galaxy

#endif