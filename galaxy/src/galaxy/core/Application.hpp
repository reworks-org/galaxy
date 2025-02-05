///
/// Application.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_APPLICATION_HPP_
#define GALAXY_CORE_APPLICATION_HPP_

#include <string_view>

#if _WIN32 || _WIN64
extern "C"
{
	// http://developer.amd.com/community/blog/2015/10/02/amd-enduro-system-for-developers/
	// http://developer.download.nvidia.com/devzone/devcenter/gamegraphics/files/OptimusRenderingPolicies.pdf

	inline __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
	inline __declspec(dllexport) std::uint32_t NvOptimusEnablement        = 0x00000001;
}
#elif
{
	inline int           AmdPowerXpressRequestHighPerformance = 1;
	inline std::uint32_t NvOptimusEnablement                  = 0x00000001;
}
#endif

namespace galaxy
{
	namespace core
	{
		///
		/// Base level class for any galaxy app.
		///
		class App final
		{
		public:
			///
			/// \brief Default constructor.
			///
			/// Sets up the engine. You need to inherit this and call it from a subclass.
			/// Also calls std::srand(std::time(nullptr)) for you.
			///
			/// \param log_dir Specify the base directory to store log files.
			/// \param config_file Filepath of config file.
			///
			explicit App(std::string_view log_dir, std::string_view config_file);

			///
			/// Destructor.
			///
			~App();

			///
			/// Loads the default appdata file.
			///
			void load();

			///
			/// Runs the app.
			///
			void run();

		private:
			///
			/// Copy constructor.
			///
			App(const App&) = delete;

			///
			/// Move constructor.
			///
			App(App&&) = delete;

			///
			/// Copy assignment operator.
			///
			App& operator=(const App&) = delete;

			///
			/// Move assignment operator.
			///
			App& operator=(App&&) = delete;

			void setup_platform();
			void setup_logging(std::string_view log_dir);
			void setup_async();
			void setup_config(std::string_view config_file);
			void setup_fs();
			void setup_window();
			void setup_audio();
			void setup_renderer();
			void setup_loader();
			void setup_entity_metadata();
			void setup_scripting();
		};
	} // namespace core
} // namespace galaxy

#endif
