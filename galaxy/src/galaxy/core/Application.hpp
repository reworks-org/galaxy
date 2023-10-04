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
	inline int AmdPowerXpressRequestHighPerformance = 1;
	inline std::uint32_t NvOptimusEnablement        = 0x00000001;
}
#endif

namespace galaxy
{
	namespace core
	{
		///
		/// Base level class for any galaxy application.
		///
		class Application
		{
		public:
			///
			/// \brief Virtual destructor.
			///
			/// Cleans up engine related memory usage.
			///
			virtual ~Application();

			///
			/// Runs the application.
			///
			void run();

		protected:
			///
			/// \brief Default constructor.
			///
			/// Sets up the engine. You need to inherit this and call it from a subclass.
			/// Also calls std::srand(std::time(nullptr)) for you.
			///
			/// \param log_dir Specify the base directory to store log files.
			/// \param config_file Filepath of config file.
			///
			explicit Application(std::string_view log_dir, std::string_view config_file);

			///
			/// Copy constructor.
			///
			Application(const Application&) = delete;

			///
			/// Move constructor.
			///
			Application(Application&&) = delete;

			///
			/// Copy assignment operator.
			///
			Application& operator=(const Application&) = delete;

			///
			/// Move assignment operator.
			///
			Application& operator=(Application&&) = delete;

			///
			/// \brief Create default asset layout.
			///
			/// This uses a const string reference over string_view since
			/// we need to combine the strings into a new string.
			///
			/// \param root Root asset folder.
			/// \param asset_folder Path to the asset folder to create.
			///
			void create_asset_layout(const std::string& root, const std::string& asset_folder);
		};
	} // namespace core
} // namespace galaxy

#endif