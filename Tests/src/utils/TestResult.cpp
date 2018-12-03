///
/// TestResult.cpp
/// Tests
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <iostream>

#include <qs/utils/Result.hpp>

#include "gtest/gtest.h"

TEST(Result, ErrorHandling)
{
	// Set up results.
	qs::Result def;
	qs::Result success("success", qs::Result::Status::SUCCESS);
	qs::Result failure("failure", qs::Result::Status::FAILURE);

	// Test constructors.
	ASSERT_EQ(def.m_status, qs::Result::Status::SUCCESS);
	ASSERT_EQ(def.m_message, std::string("default"));

	ASSERT_EQ(success.m_status, qs::Result::Status::SUCCESS);
	ASSERT_EQ(success.m_message, std::string("success"));

	ASSERT_EQ(failure.m_status, qs::Result::Status::FAILURE);
	ASSERT_EQ(failure.m_message, std::string("failure"));

	// Set up operator tests and simultaneously test static results.
	ASSERT_TRUE(success == qs::Result::SUCCESS);
	ASSERT_TRUE(failure == qs::Result::FAILURE);
	
	ASSERT_TRUE(success != qs::Result::FAILURE);

	// Logical not operator from explicit bool operator()
	ASSERT_FALSE(!success);
}