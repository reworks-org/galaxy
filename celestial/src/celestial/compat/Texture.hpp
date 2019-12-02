///
/// Texture.hpp
/// celestial
///
/// Refer to LICENSE.txt for more details.
///

#ifndef TEXTURE_CELESTIAL_HPP_
#define TEXTURE_CELESTIAL_HPP_

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
			virtual ~Texture() noexcept;

			///
			/// Casts the class to its child type.
			///
			CRTP* getChild() final noexcept;

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
		inline Texture<CRTP>::~Texture() noexcept
		{
		}

		template<typename CRTP>
		inline CRTP* Texture<CRTP>::getChild() noexcept
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