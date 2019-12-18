///
/// WangSet.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_WANGSET_HPP_
#define STARMAP_WANGSET_HPP_

#include "nlohmann/json_fwd.hpp"

///
/// Core namespace.
///
namespace starmap
{
	///
	///
	///
	class WangSet final
	{
	public:
		///
		/// Default constructor.
		///
		WangSet() noexcept;

		///
		/// \brief Parse constructor.
		///
		/// Can throw exceptions.
		///
		/// \param json JSON structure/array containing WangSet json.
		///
		explicit WangSet(const nlohmann::json& json);

		///
		/// Copy constructor.
		///
		WangSet(const WangSet&) = default;

		///
		/// Move constructor.
		///
		WangSet(WangSet&&) = default;

		///
		/// Destructor.
		///
		~WangSet() = default;

		///
		/// \brief Parses json structure to member values; etc.
		///
		/// Can throws exceptions.
		///
		/// \param json JSON structure containing WangSet json.
		///
		void parse(const nlohmann::json& json);

	private:
	};
}

#endif