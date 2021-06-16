///
/// Tag.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_TAG_HPP_
#define GALAXY_COMPONENTS_TAG_HPP_

#include "galaxy/fs/Serializable.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// Tag an entity.
		///
		class Tag final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			Tag() noexcept;

			///
			/// Constructor.
			///
			/// \param tag Tag for entity.
			///
			Tag(std::string_view tag) noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Tag(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Tag(Tag&&) noexcept;

			///
			/// Move assignment operator.
			///
			Tag& operator=(Tag&&) noexcept;

			///
			/// Destructor.
			///
			virtual ~Tag() noexcept = default;

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
			Tag& operator=(const Tag&) = delete;

			///
			/// Copy constructor.
			///
			Tag(const Tag&) = delete;

		public:
			///
			/// Tag.
			///
			std::string m_tag;
		};
	} // namespace components
} // namespace galaxy

#endif