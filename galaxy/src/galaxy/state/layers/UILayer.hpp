///
/// UILayer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_STATE_LAYER_UILAYER_HPP_
#define GALAXY_STATE_LAYER_UILAYER_HPP_

#include "galaxy/state/Layer.hpp"
#include "galaxy/ui/RMLEvents.hpp"

namespace galaxy
{
	namespace state
	{
		///
		/// A layer is usually a layering of events/updates/rendering grouped together that dont interact.
		/// I.e. UI, Debug UI, Game, etc.
		///
		class UILayer final : public Layer
		{
		public:
			///
			/// Argument Constructor.
			///
			/// \param name Name of the Layer.
			/// \param scene Pointer to scene this layer belongs to.
			///
			UILayer(std::string_view name, Scene* scene) noexcept;

			///
			/// Destructor.
			///
			virtual ~UILayer() noexcept;

			///
			/// On push of UILayer to stack.
			///
			void on_push() override;

			///
			/// On pop of UILayer off stack.
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
			UILayer() = delete;

		private:
			///
			/// RML context.
			/// This is non-owning.
			///
			Rml::Context* m_rml;

			///
			/// Event handler for Rml.
			///
			ui::RMLEvents m_rml_events;
		};
	} // namespace state
} // namespace galaxy

#endif