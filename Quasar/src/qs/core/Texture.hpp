///
/// Texture.hpp
/// Quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_TEXTURE_HPP_
#define QUASAR_TEXTURE_HPP_

#include <string>

namespace qs
{
	class Texture final
	{
		Texture(const std::string& file) noexcept;

		~Texture() noexcept;
	};
}

#endif