///
/// Font.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <stb/stb_image.h>

#include "galaxy/graphics/text/FontInput.hpp"
#include "galaxy/graphics/text/MSDFConfig.hpp"
#include "galaxy/graphics/text/RangeMode.hpp"

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/utils/StringUtils.hpp"

#include "Font.hpp"

#define DEFAULT_ANGLE_THRESHOLD 3.0
#define DEFAULT_MITER_LIMIT     1.0
#define DEFAULT_PIXEL_RANGE     2.0
#define DEFAULT_THREAD_COUNT    2
#define LCG_MULTIPLIER          6364136223846793005ull

namespace galaxy
{
	namespace graphics
	{
		Font::Font() noexcept
			: m_size {0}
			, m_width {0}
			, m_height {0}
			, m_line_height {0.0}
			, m_ascender {0.0}
			, m_descender {0.0}
			, m_underline {0.0}
			, m_underline_thickness {0.0}
		{
		}

		Font::Font(std::string_view file, const int size)
		{
			if (!create(file, size))
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load font file '{0}'.", file);
			}
		}

		Font::Font(Font&& f) noexcept
		{
			this->m_texture             = std::move(f.m_texture);
			this->m_size                = f.m_size;
			this->m_width               = f.m_width;
			this->m_height              = f.m_height;
			this->m_name                = std::move(f.m_name);
			this->m_line_height         = f.m_line_height;
			this->m_ascender            = f.m_ascender;
			this->m_descender           = f.m_descender;
			this->m_underline           = f.m_underline;
			this->m_underline_thickness = f.m_underline_thickness;
			this->m_glyphs              = std::move(f.m_glyphs);
			this->m_kerning             = std::move(f.m_kerning);
		}

		Font& Font::operator=(Font&& f) noexcept
		{
			if (this != &f)
			{
				this->m_texture             = std::move(f.m_texture);
				this->m_size                = f.m_size;
				this->m_width               = f.m_width;
				this->m_height              = f.m_height;
				this->m_name                = std::move(f.m_name);
				this->m_line_height         = f.m_line_height;
				this->m_ascender            = f.m_ascender;
				this->m_descender           = f.m_descender;
				this->m_underline           = f.m_underline;
				this->m_underline_thickness = f.m_underline_thickness;
				this->m_glyphs              = std::move(f.m_glyphs);
				this->m_kerning             = std::move(f.m_kerning);
			}

			return *this;
		}

		Font::~Font() noexcept
		{
			m_glyphs.clear();
			m_kerning.clear();
		}

		bool Font::create(std::string_view file, const int size)
		{
			bool result = false;
			auto& fs    = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			const auto info = fs.find(file);
			if (info.m_code == fs::FileCode::FOUND)
			{
				FontInput font_input = {};
				MSDFConfig config    = {};

				font_input.font_filename    = info.m_string.c_str();
				font_input.charset          = msdf_atlas::Charset::ASCII;
				font_input.glyph_identifier = msdf_atlas::GlyphIdentifierType::UNICODE_CODEPOINT;
				font_input.font_scale       = 1.0;

				config.image_type                                 = msdf_atlas::ImageType::MSDF;
				config.image_format                               = msdf_atlas::ImageFormat::PNG;
				config.y_direction                                = msdf_atlas::YDirection::TOP_DOWN;
				config.edge_coloring                              = msdfgen::edgeColoringInkTrap;
				config.kerning                                    = true;
				config.preprocess_geometry                        = false;
				config.generator_attributes.config.overlapSupport = !config.preprocess_geometry;
				config.generator_attributes.scanlinePass          = !config.preprocess_geometry;
				config.angle_threshold                            = DEFAULT_ANGLE_THRESHOLD;
				config.miter_limit                                = DEFAULT_MITER_LIMIT;
				config.thread_count                               = DEFAULT_THREAD_COUNT;
				config.em_size                                    = size;

				// Load fonts.
				auto handle = msdfgen::initializeFreetype();
				auto font   = msdfgen::loadFont(handle, font_input.font_filename);

				if (font != nullptr)
				{
					// Load glyphs.
					std::vector<msdf_atlas::GlyphGeometry> glyphs;
					msdf_atlas::FontGeometry geometry(&glyphs);

					const auto glyphs_loaded =
						geometry.loadCharset(font, font_input.font_scale, font_input.charset, config.preprocess_geometry, config.kerning);
					if (glyphs_loaded < 0 || glyphs.empty())
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to load glyphs from font '{0}'.", file);
					}
					else
					{
						// Determine final atlas dimensions, scale and range, pack glyphs.
						msdf_atlas::TightAtlasPacker atlas_packer;
						atlas_packer.setDimensionsConstraint(msdf_atlas::TightAtlasPacker::DimensionsConstraint::SQUARE);
						atlas_packer.setPadding(0);
						atlas_packer.setScale(config.em_size);
						atlas_packer.setPixelRange(DEFAULT_PIXEL_RANGE);
						atlas_packer.setUnitRange(0);
						atlas_packer.setMiterLimit(config.miter_limit);

						if (atlas_packer.pack(glyphs.data(), static_cast<int>(glyphs.size())) < 0)
						{
							GALAXY_LOG(GALAXY_ERROR, "Failed to pack glyphs into atlas for font '{0}'.", file);
						}
						else
						{
							atlas_packer.getDimensions(config.width, config.height);
							if (config.width > 0 && config.height > 0)
							{
								auto glyph_seed = config.coloring_seed;
								for (msdf_atlas::GlyphGeometry& glyph : glyphs)
								{
									glyph_seed *= LCG_MULTIPLIER;
									glyph.edgeColoring(config.edge_coloring, config.angle_threshold, glyph_seed);
								}

								msdf_atlas::ImmediateAtlasGenerator<float, 3, msdf_atlas::msdfGenerator, msdf_atlas::BitmapAtlasStorage<msdf_atlas::byte, 3>>
									generator(config.width, config.height);
								generator.setAttributes(config.generator_attributes);
								generator.setThreadCount(config.thread_count);
								generator.generate(glyphs.data(), glyphs.size());

								std::vector<msdf_atlas::byte> output;
								if (msdf_atlas::encodePng(output, generator.atlasStorage()))
								{
									if (m_texture.load_mem(output))
									{
										m_texture.set_filter(TextureFilters::MIN_NEAREST);
										m_texture.set_filter(TextureFilters::MAG_NEAREST);

										m_size   = static_cast<int>(config.em_size);
										m_width  = config.width;
										m_height = config.height;
										m_name   = geometry.getName();

										const msdfgen::FontMetrics& metrics = geometry.getMetrics();

										m_line_height         = metrics.lineHeight;
										m_ascender            = metrics.ascenderY * -1;
										m_descender           = metrics.descenderY * -1;
										m_underline           = metrics.underlineY * -1;
										m_underline_thickness = metrics.underlineThickness;

										for (auto& glyph : glyphs)
										{
											m_glyphs.emplace(strutils::parse_codepoint(glyph.getCodepoint()), std::move(glyph));
										}

										for (const auto& kernPair : geometry.getKerning())
										{
											const auto* glyph1 = geometry.getGlyph(msdfgen::GlyphIndex(kernPair.first.first));
											const auto* glyph2 = geometry.getGlyph(msdfgen::GlyphIndex(kernPair.first.second));

											if (glyph1 && glyph2 && glyph1->getCodepoint() && glyph2->getCodepoint())
											{
												m_kerning.emplace(std::make_pair(strutils::parse_codepoint(glyph1->getCodepoint()),
																	  strutils::parse_codepoint(glyph2->getCodepoint())),
													kernPair.second);
											}
										}

										result = true;
									}
									else
									{
										GALAXY_LOG(GALAXY_ERROR, "Failed to load font '{0}' into a texture.", file);
									}
								}
								else
								{
									GALAXY_LOG(GALAXY_ERROR, "Failed to encode PNG from atlas for font '{0}'", file);
								}
							}
							else
							{
								GALAXY_LOG(GALAXY_ERROR, "Unable to identify atlas dimensions.");
							}
						}
					}

					msdfgen::destroyFont(font);
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to open font '{0}'.", file);
				}

				msdfgen::deinitializeFreetype(handle);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to find font '{0}' because '{1}'.", file, magic_enum::enum_name(info.m_code));
			}

			return result;
		}

		const Font::GlyphStorage& Font::glyphs() const noexcept
		{
			return m_glyphs;
		}

		const Font::Kerning& Font::kerning() const noexcept
		{
			return m_kerning;
		}
	} // namespace graphics
} // namespace galaxy