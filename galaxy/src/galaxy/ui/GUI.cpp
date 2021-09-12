/*
///
/// GUI.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <magic_enum.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/graphics/Renderer2D.hpp"
#include "galaxy/resource/Shaderbook.hpp"

#include "GUI.hpp"

namespace galaxy
{
namespace ui
{
GUI::GUI()
: Serializable {this}, m_state {ConstructionState::DEFAULT}, m_id_counter {0}, m_theme {nullptr}, m_allow_input {true}
{
}

GUI::~GUI()
{
if (m_state != ConstructionState::DEFAULT)
{
clear();
}
}

void GUI::set_theme(Theme* theme)
{
m_theme = theme;
m_state = ConstructionState::THEME_SET;
}

void GUI::enable_input() noexcept
{
m_allow_input = true;
}

void GUI::disable_input() noexcept
{
m_allow_input = false;
}

void GUI::on_event(const events::MouseMoved& mme) noexcept
{
if (m_allow_input)
{
m_theme->m_event_manager.trigger<events::MouseMoved>(mme.m_x, mme.m_y);
}
}

void GUI::on_event(const events::MousePressed& mpe) noexcept
{
if (m_allow_input)
{
m_theme->m_event_manager.trigger<events::MousePressed>(mpe.m_x, mpe.m_y, mpe.m_button);
}
}

void GUI::on_event(const events::MouseReleased& mre) noexcept
{
if (m_allow_input)
{
m_theme->m_event_manager.trigger<events::MouseReleased>(mre.m_x, mre.m_y, mre.m_button);
}
}

void GUI::on_event(const events::KeyDown& kde) noexcept
{
if (m_allow_input)
{
m_theme->m_event_manager.trigger<events::KeyDown>(kde.m_keycode);
}
}

void GUI::update(const double dt)
{
std::for_each(m_widgets.begin(), m_widgets.end(), [&](const auto& widget) {
widget->update(dt);
});
}

void GUI::render()
{
m_theme->m_sb.buffer_data();

RENDERER_2D().buffer_camera(m_theme->m_camera);
RENDERER_2D().bind_sb_shader();
RENDERER_2D().draw(&m_theme->m_sb);

for (const auto& widget : m_widgets)
{
widget->render();
}
}

void GUI::remove(const unsigned int id)
{
if (id >= m_widgets.size())
{
GALAXY_LOG(GALAXY_WARNING, "Attempted to remove widget that does not exist: {0}.", id);
}
else
{
// Don't erase because that will mess up ordering.
m_widgets[id].reset();
m_widgets[id] = nullptr;

m_free.emplace_back(id);
}
}

void GUI::clear()
{
m_state      = ConstructionState::DEFAULT;
m_id_counter = 0;
m_free.clear();

for (auto& widget : m_widgets)
{
widget.reset();
}
m_widgets.clear();
}

nlohmann::json GUI::serialize()
{
nlohmann::json json = "{}"_json;

json["state"]   = static_cast<std::string>(magic_enum::enum_name<ConstructionState>(m_state));
json["widgets"] = nlohmann::json::array();
for (const auto& widget : m_widgets)
{
nlohmann::json widget_json = "{}"_json;
widget_json["type"]        = static_cast<std::string>(magic_enum::enum_name<Widget::Type>(widget->m_type));
widget_json["data"]        = widget->serialize();

json["widgets"].push_back(widget_json);
}

return json;
}

void GUI::deserialize(const nlohmann::json& json)
{
clear();

m_state = magic_enum::enum_cast<ConstructionState>(json.at("state").get<std::string>()).value();

const auto& widgets = json.at("widgets");
for (const auto& widget_obj : widgets)
{
const auto type = magic_enum::enum_cast<Widget::Type>(widget_obj.at("type").get<std::string>()).value();
switch (type)
{
case Widget::Type::BUTTON:
{
auto* widget = create_widget<Button>();
widget->deserialize(widget_obj.at("data"));
}
break;

case Widget::Type::IMAGE:
{
auto* widget = create_widget<Image>();
widget->deserialize(widget_obj.at("data"));
}
break;

case Widget::Type::LABEL:
{
auto* widget = create_widget<Label>();
widget->deserialize(widget_obj.at("data"));
}
break;

case Widget::Type::PROGRESSBAR:
{
auto* widget = create_widget<Progressbar>();
widget->deserialize(widget_obj.at("data"));
}
break;

case Widget::Type::SLIDER:
{
auto* widget = create_widget<Slider>();
widget->deserialize(widget_obj.at("data"));
}
break;

case Widget::Type::TEXTBOX:
{
auto* widget = create_widget<Textbox>();
widget->deserialize(widget_obj.at("data"));
}
break;

case Widget::Type::TEXTINPUT:
{
auto* widget = create_widget<TextInput>();
widget->deserialize(widget_obj.at("data"));
}
break;

case Widget::Type::TOGGLEBUTTON:
{
auto* widget = create_widget<ToggleButton>();
widget->deserialize(widget_obj.at("data"));
}
break;

default:
GALAXY_LOG(GALAXY_ERROR, "Failed to identify widget type: {0}.", json.at("type").get<std::string>());
break;
}
}
}
} // namespace ui
} // namespace galaxy
*/