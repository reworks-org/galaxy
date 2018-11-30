///
/// TestError.cpp
/// Tests
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <qs/error/Error.hpp>

#include "gtest/gtest.h"

TEST(Error, Functionality)
{
	// Check to make sure what we set is what is current error.
	std::string setTest = "test";
	qs::Error::handle.setError(setTest);

	ASSERT_EQ(setTest, qs::Error::handle.current());

	// Add some more errors, and then check to make sure history has all.
	// And is in correct position.
	std::string historyTest = "history";
	qs::Error::handle.setError(historyTest);

	std::vector<std::string> historyVec = qs::Error::handle.getHistory();
	ASSERT_EQ(setTest, historyVec[0]);
	ASSERT_EQ(historyTest, historyVec[1]);

	// Make sure clear only destroys current error, but history remains.
	std::string empty = "";
	qs::Error::handle.clear();

	ASSERT_EQ(empty, qs::Error::handle.current());

	std::vector<std::string> historyVec2 = qs::Error::handle.getHistory();
	ASSERT_EQ(setTest, historyVec2[0]);
	ASSERT_EQ(historyTest, historyVec2[1]);
}