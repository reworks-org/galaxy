///
/// TestResult.cpp
/// Tests
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <iostream>

#include <qs/error/Result.hpp>

#include "gtest/gtest.h"

TEST(Result, ErrorHandling)
{
	// Set up results.
	qs::Result default;
	qs::Result success { qs::Result::Status::SUCCESS, "success" };
	qs::Result error { qs::Result::Status::ERROR, "error" };
	qs::Result fatal { qs::Result::Status::FATAL, "fatal" };

	// Test constructors.
	ASSERT_EQ(default.m_status, qs::Result::Status::SUCCESS);
	ASSERT_EQ(default.m_message, std::string(""));

	ASSERT_EQ(fatal.m_status, qs::Result::Status::FATAL);
	ASSERT_EQ(fatal.m_message, std::string("fatal"));

	// Set up operator tests and simultaneously test static results.
	ASSERT_TRUE(success == qs::Result::SUCCESS);
	ASSERT_TRUE(success != qs::Result::ERROR);
	ASSERT_TRUE(fatal == qs::Result::FATAL);

	ASSERT_FALSE(success == qs::Result::ERROR);
	ASSERT_FALSE(error != qs::Result::ERROR);
}