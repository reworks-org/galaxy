///
/// RenderCommand.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_RENDERCOMMAND_HPP_
#define GALAXY_COMPONENTS_RENDERCOMMAND_HPP_

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/RenderCommand.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Controls how an entity should be rendered.
		///
		class RenderCommand final : public fs::Serializable
		{
		  public:
			///
			/// Constructor.
			///
			RenderCommand();

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			RenderCommand(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			RenderCommand(RenderCommand&&);

			///
			/// Move assignment operator.
			///
			RenderCommand& operator=(RenderCommand&&);

			///
			/// Destructor.
			///
			virtual ~RenderCommand();

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
			/// Copy assignment operator.
			///
			RenderCommand& operator=(const RenderCommand&) = delete;

			///
			/// Copy constructor.
			///
			RenderCommand(const RenderCommand&) = delete;

		  public:
			///
			/// RenderCommand.
			///
			graphics::RenderCommand m_command;
		};
	} // namespace components
} // namespace galaxy

#endif
