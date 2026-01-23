int main(int argc, char* argv[])
{
#if WIN32 && _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
#endif

	try
	{
		sol::state lua {};
		lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string);
		lua.require("scheduler", sol::c_call<AUTO_ARG(&open_scheduler)>, false);

		scheduler scheduler {};
		lua["scheduler"] = std::ref(scheduler); // Make the scheduler available to Lua

		lua.do_file("lua/process_chain.lua");

		using namespace std::chrono_literals;

		constexpr auto target_frame_time = 16ms;
		fsec           delta_time {target_frame_time};

		while (!scheduler.empty())
		{
			using clock            = std::chrono::high_resolution_clock;
			const auto begin_ticks = clock::now();

			lua.step_gc(4);

			scheduler.update(delta_time);
			std::this_thread::sleep_for(target_frame_time);

			delta_time = std::chrono::duration_cast<fsec>(clock::now() - begin_ticks);
			if (delta_time > 1s)
				delta_time = target_frame_time;

			if (_kbhit())
				break;
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "exception: " << e.what();
		return -1;
	}

	return 0;
}
