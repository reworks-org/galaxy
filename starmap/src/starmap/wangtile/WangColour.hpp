///
/// WangColour.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_WangColour_HPP_
#define STARMAP_WangColour_HPP_

#include "nlohmann/json_fwd.hpp"

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
		/// \brief Parse constructor.
		///
		/// Can throw exceptions.
		///
		/// \param json JSON structure/array containing WangColour json.
		///
		explicit WangColour(const nlohmann::json& json);

		///
		/// Copy constructor.
		///
		WangColour(const WangColour&) = default;

		///
		/// Move constructor.
		///
		WangColour(WangColour&&) = default;

		///
		/// Destructor.
		///
		~WangColour() = default;

		///
		/// \brief Parses json structure to member values; etc.
		///
		/// Can throws exceptions.
		///
		/// \param json JSON structure containing WangColour json.
		///
		void parse(const nlohmann::json& json);

	private:

	};
}

#endif