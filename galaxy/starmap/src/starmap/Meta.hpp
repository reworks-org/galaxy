///
/// Meta.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_META_HPP_
#define STARMAP_META_HPP_

#include <string>

#include <protostar/system/Concepts.hpp>

///
/// Core namespace.
///
namespace starmap
{
	///
	/// Concept for json property types.
	///
	template<typename Type>
	concept tiled_property = (std::is_same<Type, std::string>::value,
				  std::is_same<Type, int>::value,
				  std::is_same<Type, float>::value,
				  std::is_same<Type, bool>::value);
} // namespace starmap

#endif