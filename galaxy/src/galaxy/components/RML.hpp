///
/// RML.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_RML_HPP_
#define GALAXY_COMPONENTS_RML_HPP_

#include <RmlUi/Core/ElementDocument.h>

#include "galaxy/fs/Serializable.hpp"

namespace galaxy
{
	namespace components
	{
		///
		/// RML an entity.
		///
		class RML final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			RML();

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			RML(const nlohmann::json& json);

			///
			/// Pointer constructor.
			///
			/// \param ptr Data to COPY into this component.
			///
			RML(RML* ptr);

			///
			/// Move constructor.
			///
			RML(RML&&);

			///
			/// Move assignment operator.
			///
			RML& operator=(RML&&);

			///
			/// Destructor.
			///
			virtual ~RML();

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
			RML& operator=(const RML&) = delete;

			///
			/// Copy constructor.
			///
			RML(const RML&) = delete;

		public:
			///
			/// File used to create the doc.
			///
			std::string m_file;

			///
			/// Pointer to the loaded document.
			///
			Rml::ElementDocument* m_doc;
		};
	} // namespace components
} // namespace galaxy

#endif