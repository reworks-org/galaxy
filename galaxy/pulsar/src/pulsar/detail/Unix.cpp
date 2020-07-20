///
/// Unix.cpp
///
/// pulsar
/// See LICENSE.txt.
///

#if defined(__APPLE__) || defined(__linux__) || defined(__unix__)

///
/// Core namespace.
///
namespace pl
{
	std::string colourText(const LogColours colour) noexcept
	{
		std::string out = "";

		switch (colour)
		{
			case LogColours::WHITE:
				out = "\x1B[37m";
				break;

			case LogColours::GREEN:
				out = "\x1B[32m";
				break;

			case LogColours::YELLOW:
				out = "\x1B[33m";
				break;

			case LogColours::RED:
				out = "\x1B[31m";
				break;

			case LogColours::FATAL:
				out = "\x1B[31m\x1B[47m";
				break;

			default:
				out = "\x1B[37m";
				break;
		}

		return std::move(out);
	}
} // namespace pulsar

#endif