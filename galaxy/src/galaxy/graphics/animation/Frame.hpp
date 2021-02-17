///
/// Frame.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_ANIMATION_FRAME_HPP_
#define GALAXY_GRAPHICS_ANIMATION_FRAME_HPP_

#include <nlohmann/json_fwd.hpp>

#include "galaxy/fs/Serializer.hpp"
#include "galaxy/graphics/Rect.hpp"
#include "galaxy/meta/Concepts.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// A single frame of an animation.
		///
		class Frame final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
			///
			Frame() noexcept;

			///
			/// Argument constructor.
			///
			/// \param region Texture region on the texture atlas.
			/// \param time_per_frame Total time to display this frame for.
			///
			Frame(const graphics::fRect& region, const double time_per_frame) noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON object.
			///
			Frame(const nlohmann::json& json);

			///
			/// Destructor.
			///
			~Frame() noexcept = default;

			///
			/// Set region.
			///
			/// \param region Name of region in texture atlas.
			///
			void set_region(std::string_view region);

			///
			/// Get region this frame occupies.
			///
			/// \return Const float rectangle.
			///
			[[nodiscard]] const graphics::fRect& get_region() const noexcept;

			///
			/// Get Region string ID.
			///
			/// \return Const string reference.
			///
			[[nodiscard]] const std::string& get_region_id() const noexcept;

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to be serialized.
			///
			[[nodiscard]] nlohmann::json serialize() override;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			void deserialize(const nlohmann::json& json) override;

		public:
			///
			/// Time to spend on this frame in milliseconds.
			///
			double m_time_per_frame;

		private:
			///
			/// Texture region ID.
			///
			std::string m_id;

			///
			/// Region on the texture atlas.
			///
			graphics::fRect m_region;
		};
	} // namespace graphics
} // namespace galaxy

#endif