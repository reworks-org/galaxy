///
///  Application.hpp
///  starlight
///
///  Created by reworks on 08/07/2016.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_APPLICATION_HPP_
#define STARLIGHT_APPLICATION_HPP_

#include <fstream>

namespace sl
{
	class Application
	{
	public:
		///
		/// Cleans up engine related memory usage.
		/// 
		virtual ~Application();

		///
		/// Return app->run() from your main method.
		///
		/// \return Returns EXIT_SUCCESS.
		/// 
		virtual int run() final;

	protected:
		///
		/// \brief Default constructor.
		///
		/// Sets up the engine. You need to inherit this and call it from a subclass.
		/// Also calls std::srand(std::time(nullptr)) for you.
		/// 
		/// \param archive Path or archive to mount.
		/// \param config Path to the config file.
		/// \param newConfig A function pointer to a function that contains ofstream code to write a default config file.
		///
		Application(const std::string& archive, const std::string& config, std::function<void(std::ofstream&)> newConfig);

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		Application() = delete;

		///
		/// Copy Constructor.
		/// Deleted.
		///
		Application(const Application&) = delete;

		///
		/// Move Constructor.
		/// Deleted.
		///
		Application(Application&&) = delete;
	};
}

#endif