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
	/// Identifies this class as an interface.
	///
	namespace interface
	{
		///
		/// Your textures should inherit from this class.
		/// Represents a Texture.
		///
		class Texture
		{
		public:
			///
			/// Default virtual destructor.
			///
			virtual ~Texture() noexcept = default;

		protected:
			///
			/// Constructor.
			///
			Texture() noexcept = default;

			///
			/// Copy constructor.
			///
			Texture(const Texture&) noexcept = default;

			///
			/// Move constructor.
			///
			Texture(Texture&&) noexcept = default;
		};

		///
		/// Shortcut for defining a unique ptr texture.
		///
		using TexturePtr = std::unique_ptr<celestial::interface::Texture>;
	}
}

#endif