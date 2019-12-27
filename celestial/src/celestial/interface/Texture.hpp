///
/// Texture.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_TEXTURE_HPP_
#define CELESTIAL_TEXTURE_HPP_

#include <memory>

///
/// Core namespace.
///
namespace celestial
{
	///
	/// Your textures should inherit from this class.
	/// Represents a Texture.
	///
	class Texture
	{
	public:
		///
		/// Virtual Destructor.
		///
		virtual ~Texture() noexcept;

	protected:
		///
		/// Default constructor.
		///
		Texture() noexcept = default;

		///
		/// Copy constructor.
		///
		Texture(const Texture&) = default;

		///
		/// Move constructor.
		///
		Texture(Texture&&) = default;
	};

	///
	/// Shortcut for defining a unique ptr texture.
	///
	using TexturePtr = std::unique_ptr<Texture>;
}

#endif