///
/// KeyChar.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_EVENTS_KEYCHAR_HPP_
#define GALAXY_EVENTS_KEYCHAR_HPP_

namespace galaxy
{
	namespace events
	{
		///
		/// Contains UTF-8 char.
		///
		struct KeyChar final
		{
			///
			/// Default constructor.
			///
			KeyChar() noexcept;

			///
			/// Constructor.
			///
			/// \param character UTF-8 character input.
			///
			KeyChar(char character) noexcept;

			///
			/// Default destructor.
			///
			~KeyChar() noexcept = default;

			///
			/// UTF-8 key character.
			///
			char m_character;
		};
	} // namespace events
} // namespace galaxy

#endif