///
/// Sound.hpp
/// frb
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FRB_SOUND_HPP_
#define FRB_SOUND_HPP_

#include "frb/detail/Source.hpp"
#include "frb/detail/SourceManipulator.hpp"

///
/// Core namespace.
///
namespace frb
{
	///
	/// \brief Short length ogg-vorbis audio.
	///
	/// Contains a Buffer and Source.
	///
	class Sound final : public Buffer, public SourceManipulator
	{
	public:
		///
		/// Constructor.
		///
		Sound();

		///
		/// Argument constructor.
		///
		/// \param file File to load from disk. Can only load ogg vorbis.
		///
		explicit Sound(std::string_view file);

		///
		/// Copy constructor.
		///
		Sound(const Sound&) noexcept = delete;

		///
		/// Move constructor.
		///
		Sound(Sound&&);

		///
		/// Copy assignment operator.
		///
		Sound& operator=(const Sound&) noexcept = delete;

		///
		/// Move assignment operator.
		///
		Sound& operator=(Sound&&);

		///
		/// Destructor.
		///
		virtual ~Sound() = default;

		///
		/// Load a file from disk.
		///
		/// \param file File to load from disk. Can only load ogg vorbis.
		///
		/// \return False if load failed.
		///
		[[maybe_unused]] bool load(std::string_view file);

	private:
		///
		/// OpenAL audio source.
		///
		Source m_source;
	};
} // namespace frb

#endif