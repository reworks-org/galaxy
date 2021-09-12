/*
///
/// Theme.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Theme.hpp"

namespace galaxy
{
namespace ui
{
Theme::Theme() noexcept
: Serializable {this}, m_font_col {255, 0, 0, 255}
{
}

Theme::Theme(Theme&& t) noexcept
: Serializable {this}
{
this->m_font_col      = std::move(t.m_font_col);
this->m_camera        = std::move(t.m_camera);
this->m_event_manager = std::move(t.m_event_manager);
this->m_atlas         = std::move(t.m_atlas);
this->m_lower_layer   = std::move(t.m_lower_layer);
this->m_upper_layer   = std::move(t.m_upper_layer);
}

Theme& Theme::operator=(Theme&& t) noexcept
{
if (this != &t)
{
this->m_font_col      = std::move(t.m_font_col);
this->m_camera        = std::move(t.m_camera);
this->m_event_manager = std::move(t.m_event_manager);
this->m_atlas         = std::move(t.m_atlas);
this->m_lower_layer   = std::move(t.m_lower_layer);
this->m_upper_layer   = std::move(t.m_upper_layer);
}

return *this;
}

nlohmann::json Theme::serialize()
{
nlohmann::json json = "{}"_json;
json["camera"]      = m_camera.serialize();

json["colour"]      = nlohmann::json::object();
json["colour"]["r"] = m_font_col.m_red;
json["colour"]["g"] = m_font_col.m_green;
json["colour"]["b"] = m_font_col.m_blue;
json["colour"]["a"] = m_font_col.m_alpha;
json["upper-layer"] = m_upper_layer;
json["lower-layer"] = m_lower_layer;

return json;
}

void Theme::deserialize(const nlohmann::json& json)
{
m_event_manager.clear();

const auto& colour = json.at("colour");
m_font_col         = {colour.at("r"), colour.at("g"), colour.at("b"), colour.at("a")};
m_camera.deserialize(json.at("camera"));
m_lower_layer = json.at("lower-layer");
m_upper_layer = json.at("upper-layer");
}
} // namespace ui
} // namespace galaxy
*/