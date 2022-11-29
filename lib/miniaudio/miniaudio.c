#define STB_VORBIS_HEADER_ONLY
#include <stb_vorbis.c>

#define MINIAUDIO_IMPLEMENTATION
#define MA_NO_DEVICE_IO // Playback and recording.
#include "miniaudio.h"

#undef STB_VORBIS_HEADER_ONLY
#include <stb_vorbis.c>