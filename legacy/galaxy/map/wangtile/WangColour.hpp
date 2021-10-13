///
/// WangColour.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MAP_WANGTILE_WANGCOLOUR_HPP_
#define GALAXY_MAP_WANGTILE_WANGCOLOUR_HPP_

#include <nlohmann/json_fwd.hpp>

namespace galaxy
{
	namespace map
	{
		///
		/// A colour that can be used to define the corner or edge of a wang tile.
		///
		class WangColour final
		{
		public:
			///
			/// Constructor.
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
			/// \return Const reference to graphics::Colour.
			///
			[[nodiscard]] const graphics::Colour& get_colour() const noexcept;

			///
			/// Get the name.
			///
			/// \return Const std::string reference.
			///
			[[nodiscard]] const std::string& get_name() const noexcept;

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
			/// Colour.
			///
			graphics::Colour m_colour;

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
	} // namespace map
} // namespace galaxy

#endif