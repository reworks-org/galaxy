///
/// DrawShader.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_DRAWSHADER_HPP_
#define GALAXY_COMPONENTS_DRAWSHADER_HPP_

#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/Shader.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Shader to draw a renderable entity with.
		///
		class DrawShader final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			DrawShader() noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			DrawShader(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			DrawShader(DrawShader&&) noexcept;

			///
			/// Move assignment operator.
			///
			DrawShader& operator=(DrawShader&&) noexcept;

			///
			/// Destructor.
			///
			virtual ~DrawShader() noexcept;

			///
			/// Set shader.
			///
			/// \param id Shader id in resource manager.
			///
			void set_shader(const std::string& id);

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
			DrawShader& operator=(const DrawShader&) = delete;

			///
			/// Copy constructor.
			///
			DrawShader(const DrawShader&) = delete;

		public:
			///
			/// Shader pointer.
			///
			std::shared_ptr<graphics::Shader> m_shader;

		private:
			///
			/// Shader Id.
			///
			std::string m_id;
		};
	} // namespace components
} // namespace galaxy

#endif