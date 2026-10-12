///
/// CPUInfo.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_PLATFORM_CPUINFO_HPP_
#define GALAXY_PLATFORM_CPUINFO_HPP_

#include <cstddef>

namespace galaxy
{
	///
	/// Data structure containing info about the cpu, such as RAM, cache, SIMD, etc.
	///
	struct CPUInfo
	{
		/// Determine the L1 cache line size of the CPU.
		int cpu_cache_line_size;
		/// Get the number of logical CPU cores available.
		int logical_core_count;
		/// Report the size of a page of memory.
		int system_page_size;
		/// This will return the minimum number of bytes to which a pointer must be aligned to be compatible with SIMD instructions on the current machine.
		std::size_t simd_alignment;
		/// Returns the amount of RAM configured in the system in MiB.
		int system_ram;
		/// Returns true if the CPU has AltiVec features or false if not.
		bool has_altivec;
		/// Determine whether the CPU has ARM SIMD (ARMv6) features.
		bool has_arm_simd;
		/// Determine whether the CPU has AVX features.
		bool has_avx;
		/// Determine whether the CPU has AVX2 features.
		bool has_avx2;
		/// Determine whether the CPU has AVX-512F (foundation) features.
		bool has_avx512f;
		/// Determine whether the CPU has LASX (LOONGARCH SIMD) features.
		bool has_lasx;
		/// Determine whether the CPU has LSX (LOONGARCH SIMD) features.
		bool has_lsx;
		/// Determine whether the CPU has MMX features.
		bool has_mmx;
		/// Determine whether the CPU has NEON (ARM SIMD) features.
		bool has_neon;
		/// Determine whether the CPU has SSE features.
		bool has_sse;
		/// Determine whether the CPU has SSE2 features.
		bool has_sse2;
		/// Determine whether the CPU has SSE3 features.
		bool has_sse3;
		/// Determine whether the CPU has SSE41 features.
		bool has_sse41;
		/// Determine whether the CPU has SSE42 features.
		bool has_sse42;
		/// Determine whether the CPU has SVE2 (Scalable Vector Extension 2).
		bool has_sve2;
	};

	namespace platform
	{
		///
		/// Generates CPU info data.
		///
		/// \return CPUInfo struct.
		///
		CPUInfo get_cpu_info() noexcept;
	} // namespace platform

} // namespace galaxy

#endif
