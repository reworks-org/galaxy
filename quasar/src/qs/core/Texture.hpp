///
/// Texture.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_TEXTURE_HPP_
#define QUASAR_TEXTURE_HPP_

#include <string>

///
/// Core namespace.
///
namespace qs
{
	class Texture final
	{
	public:
		Texture(const std::string& file, bool repeat = false, const std::array<float, 4>& border = { 1.0f, 1.0f, 1.0f, 0.0f });
		~Texture() noexcept;

		void bind() noexcept;
		void unbind() noexcept;

	private:
		unsigned int m_id;
	};
}

#endif