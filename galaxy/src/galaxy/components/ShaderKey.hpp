///
/// ShaderKey.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_SHADERID_HPP_
#define GALAXY_COMPONENTS_SHADERID_HPP_

#include <string_view>

#include "galaxy/fs/Serializable.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Shader key component.
		///
		class ShaderKey final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			ShaderKey();

			///
			/// Argument constructor.
			///
			/// \param key Key (id) of a shader in the shader resource cache.
			///
			ShaderKey(std::string_view key);

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			ShaderKey(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			ShaderKey(ShaderKey&&) noexcept;

			///
			/// Move assignment operator.
			///
			ShaderKey& operator=(ShaderKey&&) noexcept;

			///
			/// Destructor.
			///
			virtual ~ShaderKey() noexcept = default;

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

		public:
			///
			/// Key (id) of a shader in the shader resource cache.
			///
			std::string m_key;

		private:
			///
			/// Copy assignment operator.
			///
			ShaderKey& operator=(const ShaderKey&) = delete;

			///
			/// Copy constructor.
			///
			ShaderKey(const ShaderKey&) = delete;
		};
	} // namespace components
} // namespace galaxy

#endif