///
/// ResCacheBase.hpp
/// protostar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef PROTOSTAR_RESCACHEBASE_HPP_
#define PROTOSTAR_RESCACHEBASE_HPP_

///
/// Core namespace.
///
namespace protostar
{
	///
	/// Container structure for storing ResourceCache objects in containers.
	///
	struct ResCacheBase
	{
		///
		/// Default constructor.
		///
		ResCacheBase() noexcept = default;

		///
		/// Virtual default destructor.
		///
		virtual ~ResCacheBase() noexcept = default;
	};
}

#endif