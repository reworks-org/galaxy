///
/// MSDFConfig.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_MSDFCONFIG_HPP_
#define GALAXY_GRAPHICS_MSDFCONFIG_HPP_

#include <msdf-atlas-gen/msdf-atlas-gen.h>

namespace galaxy
{
	namespace graphics
	{
		///
		/// MSDFConfig generator settings.
		///
		struct MSDFConfig final
		{
			msdf_atlas::ImageType image_type;
			msdf_atlas::ImageFormat image_format;
			msdf_atlas::YDirection y_direction;
			int width;
			int height;
			double em_size;
			double angle_threshold;
			double miter_limit;
			void (*edge_coloring)(msdfgen::Shape&, double, unsigned long long);
			unsigned long long coloring_seed;
			msdf_atlas::GeneratorAttributes generator_attributes;
			bool preprocess_geometry;
			bool kerning;
			int thread_count;
		};
	} // namespace graphics
} // namespace galaxy

#endif