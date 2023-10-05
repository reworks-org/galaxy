///
/// Fonts.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_RESOURCE_FONTS_HPP_
#define GALAXY_RESOURCE_FONTS_HPP_

#include "galaxy/graphics/Font.hpp"
#include "galaxy/resource/Cache.hpp"

namespace galaxy
{
	namespace resource
	{
		///
		/// Resource manager for fonts.
		///
		class FontLoader final
		{
		public:
			///
			/// \brief Builds font atlas'.
			///
			/// Not thread safe, calls OpenGL code.
			///
			/// \param cache Resource cache to build data from.
			///
			void build(robin_hood::unordered_node_map<std::uint64_t, std::shared_ptr<graphics::Font>>& cache);

			///
			/// Overloaded operator() used to load a resource.
			///
			/// \param file Path on disk to load file from. You don't need to check with the filesystem, already done by the cache.
			///
			/// \return Shared pointer to newly created resource.
			///
			std::shared_ptr<graphics::Font> operator()(const std::string& file);
		};

		///
		/// Abbreviation for font cache type.
		///
		using Fonts = Cache<graphics::Font, FontLoader, true>;
	} // namespace resource
} // namespace galaxy

#endif