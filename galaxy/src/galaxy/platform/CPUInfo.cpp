///
/// CPUInfo.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <SDL3/SDL_cpuinfo.h>

#include "CPUInfo.hpp"

namespace galaxy
{
	CPUInfo platform::get_cpu_info() noexcept
	{
		const CPUInfo info {
			.cpu_cache_line_size = SDL_GetCPUCacheLineSize(),
			.logical_core_count  = SDL_GetNumLogicalCPUCores(),
			.system_page_size    = SDL_GetSystemPageSize(),
			.simd_alignment      = SDL_GetSIMDAlignment(),
			.system_ram          = SDL_GetSystemRAM(),
			.has_altivec         = SDL_HasAltiVec(),
			.has_arm_simd        = SDL_HasARMSIMD(),
			.has_avx             = SDL_HasAVX(),
			.has_avx2            = SDL_HasAVX2(),
			.has_avx512f         = SDL_HasAVX512F(),
			.has_lasx            = SDL_HasLASX(),
			.has_lsx             = SDL_HasLSX(),
			.has_mmx             = SDL_HasMMX(),
			.has_neon            = SDL_HasNEON(),
			.has_sse             = SDL_HasSSE(),
			.has_sse2            = SDL_HasSSE2(),
			.has_sse3            = SDL_HasSSE3(),
			.has_sse41           = SDL_HasSSE41(),
			.has_sse42           = SDL_HasSSE42(),
			.has_sve2            = false
		};

		return info;
	}
} // namespace galaxy
