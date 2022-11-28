#define STB_VORBIS_HEADER_ONLY
#include <stb_vorbis.c>

#define MINIAUDIO_IMPLEMENTATION
#define MA_ENABLE_ONLY_SPECIFIC_BACKENDS
#define MA_ENABLE_DSOUND
#define MA_ENABLE_PULSEAUDIO
#define MA_NO_DEVICE_IO
#include "miniaudio.h"

#undef STB_VORBIS_HEADER_ONLY
#include <stb_vorbis.c>