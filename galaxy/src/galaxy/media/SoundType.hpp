///
/// SoundType.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MEDIA_SOUNDTYPE_HPP_
#define GALAXY_MEDIA_SOUNDTYPE_HPP_

namespace galaxy
{
	namespace media
	{
		///
		/// Sound type.
		///
		enum class SoundType : int
		{
			///
			/// A sound effect. Is not streamed.
			///
			SFX = 0,

			///
			/// Music. Is streamed.
			///
			MUSIC = 1,

			///
			/// Dialogue. Is streamed.
			///
			DIALOGUE = 2
		};
	} // namespace media
} // namespace galaxy

#endif