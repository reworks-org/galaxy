//
//  Log.hpp
//  rework
//
//  Created by reworks on 17/04/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_LOG_HPP_
#define REWORK_LOG_HPP_

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

namespace logging = boost::log;
namespace keywords = boost::log::keywords;

namespace re
{
	namespace Log
	{
		///
		/// Initialize boost logging system.
		///
		inline void init()
		{
			logging::add_file_log
			(
				keywords::file_name = "sample_%N.log",
				keywords::rotation_size = 1 * 1024 * 1024,
				keywords::format = "[%TimeStamp%]: %Message%"
			);

			logging::core::get()->set_filter
			(
				logging::trivial::severity >= logging::trivial::info
			);

			logging::add_common_attributes();
		}
	}
}

#endif