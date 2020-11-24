///
/// Property.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_PROPERTY_HPP_
#define STARMAP_PROPERTY_HPP_

#include <any>

#include <nlohmann/json_fwd.hpp>

#include <starmap/Meta.hpp>

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
		Property();

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
		~Property();

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
		[[nodiscard]] std::string get_type() const;

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
} // namespace starmap

#endif