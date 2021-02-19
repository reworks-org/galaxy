///
/// Property.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MAP_TYPES_PROPERTY_HPP_
#define GALAXY_MAP_TYPES_PROPERTY_HPP_

#include <any>

#include <nlohmann/json_fwd.hpp>

#include "galaxy/map/TiledProperty.hpp"

namespace galaxy
{
	namespace map
	{
		///
		/// Represents a generic key-value for a tiled Type.
		///
		class Property final
		{
		public:
			///
			/// Constructor.
			///
			Property() noexcept;

			///
			/// \brief Parse constructor.
			///
			/// Can throw exceptions.
			///
			/// \param json JSON structure/array containing Property.
			///
			explicit Property(const nlohmann::json& json);

			///
			/// Destructor.
			///
			~Property() noexcept;

			///
			/// \brief Parse Property level jston.
			///
			/// Can throw exceptions.
			///
			/// \param json JSON structure/array containing Property.
			///
			void parse(const nlohmann::json& json);

			///
			/// Get type typename as a std::string.
			///
			/// \return boolean true if ellipse.
			///
			[[nodiscard]] const std::string& get_type() const noexcept;

			///
			/// Get value. You should already know the type you want to retrieve.
			///
			/// \return value cast as Type.
			///
			template<tiled_property Type>
			[[nodiscard]] const Type& get() const;

		private:
			///
			/// Type of the property (string (default); int; float; bool; color or file (since 0.16; with color and file added in 0.17)).
			///
			std::string m_type;

			///
			/// Value of the property.
			///
			std::any m_value;
		};

		template<tiled_property Type>
		inline const Type& Property::get() const
		{
			return std::any_cast<Type>(m_value);
		}
	} // namespace map
} // namespace galaxy

#endif