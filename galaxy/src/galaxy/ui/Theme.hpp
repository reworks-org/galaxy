/*
///
/// Theme.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_UI_THEME_HPP_
#define GALAXY_UI_THEME_HPP_

#include "galaxy/events/Dispatcher.hpp"
#include "galaxy/fs/Serializable.hpp"
#include "galaxy/graphics/Camera2D.hpp"
#include "galaxy/graphics/Colour.hpp"
#include "galaxy/graphics/Shader.hpp"
#include "galaxy/graphics/SpriteBatch.hpp"
#include "galaxy/graphics/TextureAtlas.hpp"

namespace galaxy
{
namespace ui
{
///
/// Defines the theme used by the GUI.
///
class Theme final : public fs::Serializable
{
public:
///
/// Constructor.
///
Theme() noexcept;

///
/// Move constructor.
///
Theme(Theme&&) noexcept;

///
/// Move assignment operator.
///
Theme& operator=(Theme&&) noexcept;

///
/// Destructor.
///
~Theme() noexcept = default;

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
/// Font colour.
///
graphics::Colour m_font_col;

///
/// Camera view for GUI.
///
graphics::Camera2D m_camera;

///
/// GUI texture atlas.
///
graphics::TextureAtlas m_atlas;

///
/// Internal event manager.
///
events::Dispatcher m_event_manager;

///
/// Upper UI layer.
///
std::string m_upper_layer;

///
/// Lower UI layer.
///
std::string m_lower_layer;

private:
///
/// Copy constructor.
///
Theme(const Theme&) = delete;

///
/// Copy assignment operator.
///
Theme& operator=(const Theme&) = delete;
};
} // namespace ui
} // namespace galaxy

#endif
*/