///
/// Frame.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_FRAME_HPP_
#define GALAXY_GRAPHICS_FRAME_HPP_

#include "galaxy/fs/Serializable.hpp"

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
			Frame();

			///
			/// JSON constructor.
			///
			/// \param json JSON object.
			///
			Frame(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Frame(Frame&&);

			///
			/// Move assignment operator.
			///
			Frame& operator=(Frame&&);

			///
			/// Copy constructor.
			///
			Frame(const Frame&);

			///
			/// Copy assignment operator.
			///
			Frame& operator=(const Frame&);

			///
			/// Destructor.
			///
			virtual ~Frame();

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

			///
			/// Texture region ID.
			///
			std::string m_texture_id;
		};
	} // namespace graphics
} // namespace galaxy

#endif
