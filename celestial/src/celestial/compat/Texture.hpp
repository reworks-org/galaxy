///
/// Texture.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef CELESTIAL_TEXTURE_HPP_
#define CELESTIAL_TEXTURE_HPP_

namespace celestial
{
	namespace compat
	{
		///
		/// Your t extures should inherit from this class.
		/// Represents a Texture.
		///
		template<typename CRTP>
		class Texture
		{
		public:
			///
			/// Virtual Destructor.
			///
			virtual ~Texture();

			///
			/// Casts the class to its child type.
			///
			virtual CRTP* getChild() final;

		protected:
			///
			/// Default constructor.
			///
			Texture() = default;

			///
			/// Copy constructor.
			///
			Texture(const Texture&) = default;

			///
			/// Move constructor.
			///
			Texture(Texture&&) = default;
		};

		template<typename CRTP>
		inline Texture<CRTP>::~Texture()
		{
		}

		template<typename CRTP>
		inline CRTP* Texture<CRTP>::getChild()
		{
			return static_cast<CRTP>(this);
		}
	}

	///
	/// Shortcut to get around defining an absolute type.
	///
	using Texture = celestial::compat::Texture<TEXTURE_CLASS>;
	using TexturePtr = std::unique_ptr<Texture>;
}

#endif