///
/// WangTile.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_WangTile_HPP_
#define STARMAP_WangTile_HPP_

#include "nlohmann/json_fwd.hpp"

///
/// Core namespace.
///
namespace starmap
{
	///
	///
	///
	class WangTile final
	{
	public:
		///
		/// Default constructor.
		///
		WangTile() noexcept;

		///
		/// \brief Parse constructor.
		///
		/// Can throw exceptions.
		///
		/// \param json JSON structure/array containing WangTile json.
		///
		explicit WangTile(const nlohmann::json& json);

		///
		/// Copy constructor.
		///
		WangTile(const WangTile&) = default;

		///
		/// Move constructor.
		///
		WangTile(WangTile&&) = default;

		///
		/// Destructor.
		///
		~WangTile() = default;

		///
		/// \brief Parses json structure to member values; etc.
		///
		/// Can throws exceptions.
		///
		/// \param json JSON structure containing WangTile json.
		///
		void parse(const nlohmann::json& json);

	private:
	};
}

#endif