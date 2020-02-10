///
/// QueuedTest.hpp
/// starlight
///
/// Refer to LICENSE.txt for more details.
///

#ifndef TESTBED_QUEUEDTEST_HPP_
#define TESTBED_QUEUEDTEST_HPP_

#include <starlight/event/Queued.hpp>

///
/// Core namespace.
///
namespace tests
{
	///
	/// Test queued class.
	///
	inline bool testQueued()
	{
		bool result = false;

		// Test constructor.
		starlight::QueuedEvent qe(std::make_any<int>(1), 5);

		int val = std::any_cast<int>(qe.m_event);
		if (val == 1) result = true;
		if (qe.m_type == 5) result = true;

		return result;
	}
}

#endif