///
/// Context.hpp
/// context
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FRB_CONTEXT_HPP_
#define FRB_CONTEXT_HPP_

#include <AL/al.h>
#include <AL/alc.h>

///
/// Core namespace.
///
namespace frb
{
	///
	///
	///
	class Context
	{
	public:
		///
		/// Default constructor.
		///
		Context() noexcept;

		///
		/// \brief Initialize OpenAL.
		///
		/// Can throw exceptions.
		///
		/// \return True if successful.
		///
		bool initialize();

		///
		/// Default destructor.
		///
		~Context() noexcept;

	private:
		///
		/// OpenAL device.
		///
		ALCdevice* m_device;

		///
		/// OpenAL context.
		///
		ALCcontext* m_context;
	};
}

#endif