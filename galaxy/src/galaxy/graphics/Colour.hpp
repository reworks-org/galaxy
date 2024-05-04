///
/// Colour.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_COLOUR_HPP_
#define GALAXY_GRAPHICS_COLOUR_HPP_

#include <array>
#include <compare>

#include <glm/vec4.hpp>

#undef OPAQUE
#undef TRANSPARENT

namespace galaxy
{
	namespace graphics
	{
		///
		/// Represents an RGBA colour.
		///
		class Colour final
		{
		  public:
			///
			/// Opaque colour.
			///
			inline static const constexpr std::uint8_t OPAQUE = 255;

			///
			/// Transparent colour.
			///
			inline static const constexpr std::uint8_t TRANSPARENT = 0;

		  public:
			///
			/// Constructor.
			///
			Colour();

			///
			/// Move constructor.
			///
			Colour(Colour&&);

			///
			/// Copy constructor.
			///
			Colour(const Colour&);

			///
			/// Move assignment operator.
			///
			Colour& operator=(Colour&&);

			///
			/// Copy assignment operator.
			///
			Colour& operator=(const Colour&);

			///
			/// Destructor.
			///
			~Colour();

			///
			/// Red.
			///
			/// \param r 0 - 255.
			///
			void set_r(const std::uint8_t r);

			///
			/// Green.
			///
			/// \param g 0 - 255.
			///
			void set_g(const std::uint8_t g);

			///
			/// Blue.
			///
			/// \param b 0 - 255.
			///
			void set_b(const std::uint8_t b);

			///
			/// Alpha.
			///
			/// \param a 0 - 255.
			///
			void set_a(const std::uint8_t a);

			///
			/// Get red.
			///
			/// \tparam R Determines return value.
			///
			/// \return Int for 0 - 255, Float for 0 - 1.
			///
			template<typename R>
			requires std::is_same_v<std::uint8_t, R> || std::is_same_v<float, R>
			R r() const;

			///
			/// Get green.
			///
			/// \tparam G Determines return value.
			///
			/// \return Int for 0 - 255, Float for 0 - 1.
			///
			template<typename G>
			requires std::is_same_v<std::uint8_t, G> || std::is_same_v<float, G>
			G g() const;

			///
			/// Get blue.
			///
			/// \tparam B Determines return value.
			///
			/// \return Int for 0 - 255, Float for 0 - 1.
			///
			template<typename B>
			requires std::is_same_v<std::uint8_t, B> || std::is_same_v<float, B>
			B b() const;

			///
			/// Get alpha.
			///
			/// \tparam A Determines return value.
			///
			/// \return Int for 0 - 255, Float for 0 - 1.
			///
			template<typename A>
			requires std::is_same_v<std::uint8_t, A> || std::is_same_v<float, A>
			A a() const;

			///
			/// Set RGBA.
			///
			/// \param rgba Clamps from 0 to 1.
			///
			void set_rgba(const glm::vec4& rgba);

			///
			/// Get integer array.
			///
			/// \return Integers 0 - 255.
			///
			[[nodiscard]] std::array<std::uint8_t, 4>& array();

			///
			/// Get vec4.
			///
			/// \return Floats 0 - 1.
			///
			[[nodiscard]] glm::vec4& vec4();

			///
			/// Get const integer array.
			///
			/// \return Integers 0 - 255.
			///
			[[nodiscard]] const std::array<std::uint8_t, 4>& array() const;

			///
			/// Get const vec4.
			///
			/// \return Floats 0 - 1.
			///
			[[nodiscard]] const glm::vec4& vec4() const;

			///
			/// Comparison operator.
			///
			[[nodiscard]] auto operator<=>(const Colour&) const = default;

		  private:
			///
			/// r,g,b,a = 0,1,2,3.
			///
			std::array<std::uint8_t, 4> m_array;

			///
			/// r,g,b,a = x,y,z,w.
			///
			glm::vec4 m_vec4;
		};

		template<>
		inline float Colour::r() const
		{
			return m_vec4.x;
		}

		template<>
		inline std::uint8_t Colour::r() const
		{
			return m_array[0];
		}

		template<>
		inline float Colour::g() const
		{
			return m_vec4.y;
		}

		template<>
		inline std::uint8_t Colour::g() const
		{
			return m_array[1];
		}

		template<>
		inline float Colour::b() const
		{
			return m_vec4.z;
		}

		template<>
		inline std::uint8_t Colour::b() const
		{
			return m_array[2];
		}

		template<>
		inline float Colour::a() const
		{
			return m_vec4.w;
		}

		template<>
		inline std::uint8_t Colour::a() const
		{
			return m_array[3];
		}
	} // namespace graphics
} // namespace galaxy

#endif
