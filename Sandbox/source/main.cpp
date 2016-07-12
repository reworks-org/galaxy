//
//  main.cpp
//  REngine3
//
//  Created by reworks on 8/07/2016.
//  Copyright (c) 2016 reworks. All rights reserved.
//

#include "re/utils/Log.hpp"
#include "re/app/Application.hpp"

class App : public re::Application
{
public:
	inline void Init()
	{
		RE_LOG_ENABLE(true);
		RE_LOG_ENABLE_FILE(false);

		RE_LOG(re::LogLevel::INFO, "Begin init");

		m_vfs.Mount("bin/Debug/assets/");

		//load config menu

		m_window.create(sf::VideoMode(800, 600), "REngine3");
	}
};

int main()
{
	App app;
	app.Init();

	return app.Run();
}