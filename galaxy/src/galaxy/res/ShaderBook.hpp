///
/// ShaderBook.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RES_SHADERBOOK_HPP_
#define GALAXY_RES_SHADERBOOK_HPP_

#include "galaxy/graphics/gl/Shader.hpp"
#include "galaxy/res/ResourceCache.hpp"

namespace galaxy
{
	namespace res
	{
		///
		/// Resource manager for shaders.
		///
		class ShaderBook final : public ResourceCache<graphics::Shader>
		{
		public:
			///
			/// Constructor.
			///
			ShaderBook();

			///
			/// JSON constructor.
			///
			/// \param file JSON file to load.
			///
			ShaderBook(std::string_view file);

			///
			/// Destructor.
			///
			virtual ~ShaderBook() noexcept;

			///
			/// Create ShaderBook from JSON.
			///
			/// \param file JSON file to load.
			///
			void create_from_json(std::string_view file);

			///
			/// Clean up.
			///
			void clear() noexcept override;

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
			ShaderBook(const ShaderBook&) = delete;

			///
			/// Move constructor.
			///
			ShaderBook(ShaderBook&&) = delete;

			///
			/// Copy assignment operator.
			///
			ShaderBook& operator=(const ShaderBook&) = delete;

			///
			/// Move assignment operator.
			///
			ShaderBook& operator=(ShaderBook&&) = delete;

			///
			/// Create default shaderbook shaders.
			///
			void create_default();

		private:
			///
			/// Vertex extension.
			///
			std::string m_vert_ext;

			///
			/// Fragment extension.
			///
			std::string m_frag_ext;
		};
	} // namespace res
} // namespace galaxy

#endif