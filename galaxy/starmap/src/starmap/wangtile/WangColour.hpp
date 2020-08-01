///
/// WangColour.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_WANGCOLOUR_HPP_
#define STARMAP_WANGCOLOUR_HPP_

#include <nlohmann/json_fwd.hpp>

///
/// Core namespace.
///
namespace starmap
{
	///
	/// A colour that can be used to define the corner or edge of a wang tile.
	///
	class WangColour final
	{
	public:
		///
		/// Default constructor.
		///
		WangColour() noexcept;

		///
		/// Parse constructor.
		///
		/// \param json JSON structure/array containing WangColour json.
		///
		explicit WangColour(const nlohmann::json& json);

		///
		/// Destructor.
		///
		~WangColour() noexcept = default;

		///
		/// Parses json structure to member values; etc.
		///
		/// \param json JSON structure containing WangColour json.
		///
		void parse(const nlohmann::json& json);

		///
		/// Get colour.
		///
		/// \return Hex formatted std::string.
		///
		[[nodiscard]] std::string get_colour() const noexcept;

		///
		/// Get the name.
		///
		/// \return Const std::string reference.
		///
		[[nodiscard]] std::string get_name() const noexcept;

		///
		/// Get probability of being picked by editor randomizer.
		///
		/// \return Const double.
		///
		[[nodiscard]] const double get_probability() const noexcept;

		///
		/// Get local tile id.
		///
		/// \return Const int.
		///
		[[nodiscard]] const int get_tile() const noexcept;

	private:
		///
		/// Hex-formatted color (#RRGGBB or #AARRGGBB).
		///
		std::string m_colour;

		///
		/// Name of the Wang color.
		///
		std::string m_name;

		///
		/// Probability used when randomizing.
		///
		double m_probability;

		///
		/// Local ID of tile representing the Wang color.
		///
		int m_tile;
	};
} // namespace starmap

#endif