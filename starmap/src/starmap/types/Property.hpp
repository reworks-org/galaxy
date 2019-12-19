///
/// Property.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_PROPERTY_HPP_
#define STARMAP_PROPERTY_HPP_

#include <any>
#include <string>

#include <nlohmann/json_fwd.hpp>

///
/// Core namespace.
///
namespace starmap
{
	///
	/// Represents a generic key-value for a tiled Type.
	///
	class Property final
	{
	public:
		///
		/// Default constructor.
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
		/// Copy constructor.
		///
		Property(const Property&) = default;

		///
		/// Move constructor.
		///
		Property(Property&&) = default;

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
		const std::string& getType() const noexcept;

		///
		/// Get value.
		///
		/// Can throw exceptions.
		///
		/// \return value cast as T.
		///
		template<typename T>
		const T& get() const;

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

	template<typename T>
	inline const T& Property::get() const
	{
		return std::any_cast<T>(m_value);
	}
}

#endif