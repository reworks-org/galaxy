///
/// ParticleGenerator.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_PARTICLEGENERATOR_HPP_
#define QUASAR_PARTICLEGENERATOR_HPP_

///
/// Core namespace.
///
namespace qs
{
	///
	///
	///
	class ParticleGenerator final
	{
	public:
		ParticleGenerator()                         = default;
		ParticleGenerator(const ParticleGenerator&) = default;
		ParticleGenerator(ParticleGenerator&&)      = default;
		ParticleGenerator& operator=(const ParticleGenerator&) = default;
		ParticleGenerator& operator=(ParticleGenerator&&) = default;
		~ParticleGenerator()                              = default;
	};
} // namespace qs

#endif