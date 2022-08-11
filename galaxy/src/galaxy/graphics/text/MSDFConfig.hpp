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
			///
			/// MSDF image type.
			///
			msdf_atlas::ImageType image_type;

			///
			/// MSDF image format.
			///
			msdf_atlas::ImageFormat image_format;

			///
			/// MSDF image pixel layout.
			///
			msdf_atlas::YDirection y_direction;

			///
			/// Atlas width.
			///
			int width;

			///
			/// Atlas height.
			///
			int height;

			///
			/// Font size.
			///
			double em_size;

			///
			/// MSDF bitmap angle threshold.
			///
			double angle_threshold;

			///
			/// MSDF miter limit value.
			///
			double miter_limit;

			///
			/// Edge colouring function algorithm.
			///
			void (*edge_coloring)(msdfgen::Shape&, double, unsigned long long);

			///
			/// Pseudo-RNG seed.
			///
			unsigned long long coloring_seed;

			///
			/// MSDF Generation settings.
			///
			msdf_atlas::GeneratorAttributes generator_attributes;

			///
			/// Needed for scanline support.
			///
			bool preprocess_geometry;

			///
			/// Does font use kerning.
			///
			bool kerning;

			///
			/// Number of threads to use when generating atlas.
			int thread_count;
		};
	} // namespace graphics
} // namespace galaxy

#endif