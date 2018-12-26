///
/// UI.cpp
/// starlight
///
/// Created by reworks on 14/09/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "sl/core/World.hpp"
#include "sl/fs/VirtualFS.hpp"
#include "sl/libs/sol2/sol.hpp"
#include "sl/core/ServiceLocator.hpp"

#include "sl/ui/widgets/Button.hpp"
#include "sl/ui/widgets/Image.hpp"
#include "sl/ui/widgets/Label.hpp"
#include "sl/ui/widgets/ProgressBar.hpp"
#include "sl/ui/widgets/Slider.hpp"
#include "sl/ui/widgets/Textbox.hpp"
#include "sl/ui/widgets/TextInput.hpp"
#include "sl/ui/widgets/ToggleButton.hpp"

#include "UI.hpp"

namespace sl
{
	UI::UI()
	{
		// Register library widgets.
		registerWidget<Button>("Button");
		registerWidget<Image>("Image");
		registerWidget<Label>("Label");
		registerWidget<ProgressBar>("ProgressBar");
		registerWidget<Slider>("Slider");
		registerWidget<Textbox>("Textbox");
		registerWidget<TextInput>("TextInput");
		registerWidget<ToggleButton>("ToggleButton");
	}

	UI::~UI()
	{
		// Ensure unique_ptr's are deconstructed and detroyed.
		m_panels.clear();
	}
	
	std::unordered_map<std::string, Widget*> UI::createFromScript(const std::string& luaScript)
	{
		// Load script into lua.
		std::string fullPath = Locator::world->m_scriptFolderPath + luaScript;
		m_lua.script(Locator::virtualFS->openAsString(fullPath));

		// Panel that will be created.
		sl::Panel* panel = nullptr;

		// Hashmap to be returned.
		std::unordered_map<std::string, Widget*> hashmap;
		
		// Get master table and check for validity.
		sol::table ui = m_lua.get<sol::table>("panel");
		if (!ui.valid() || ui.empty())
		{
			LOG_S(ERROR) << "UI script table \"panel\" is invalid or empty for script: " << luaScript;
		}
		else
		{
			// Now retrieve bounds and check for validity.
			sol::table bounds = ui.get<sol::table>("bounds");
			if (!bounds.valid() || bounds.empty())
			{
				LOG_S(ERROR) << "Panel table \"bounds\" is invalid or empty for script: " << luaScript;
			}
			else
			{
				int x = bounds.get<int>("x");
				int y = bounds.get<int>("y");
				int w = bounds.get<int>("w");
				int h = bounds.get<int>("h");

				// Check to see if colour / image exists.
				auto colourCheck = ui["colour"];
				auto imageCheck = ui["image"];
				if (colourCheck.valid())
				{
					// Make sure colour is not empty.
					sol::table colour = ui.get<sol::table>("colour");
					if (!colour.valid() || colour.empty())
					{
						LOG_S(ERROR) << "Panel table \"colour\" is invalid or empty for script: " << luaScript;
					}
					else
					{
						unsigned char r = colour.get<unsigned char>("r");
						unsigned char g = colour.get<unsigned char>("g");
						unsigned char b = colour.get<unsigned char>("b");
						unsigned char a = colour.get<unsigned char>("a");

						panel = addPanel(sl::Rect<int>{x, y, w, h}, al_map_rgba(r, g, b, a));
					}
				}
				else if (imageCheck.valid()) // Else if image is valid.
				{
					std::string image = ui.get<std::string>("image");
					if (!image.empty())
					{
						panel = addPanel(sl::Rect<int>{x, y, w, h}, image);
					}
					else
					{
						LOG_S(ERROR) << "Panel table image string is empty for script: " << luaScript;
					}
				}
				else
				{
					LOG_S(ERROR) << "Found no valid image or colour table for UI in script: " << luaScript;
				}
			}

			// Now to load the widgets.
			sol::table widgets = ui.get<sol::table>("widgets");
			if (!widgets.valid() || widgets.empty())
			{
				LOG_S(ERROR) << "Panel table \"widgets\" is invalid or empty for script: " << luaScript;
			}
			else
			{
				// Iterate over every widget.
				widgets.for_each([&](std::pair<sol::object, sol::object> pair)
				{
					// Here we are splitting up the full name string into the type and name of the string.
					// So we know what type of widget to create and have a unique identifier for that widget.
					std::string fullName = pair.first.as<std::string>();
					std::size_t markerPos = fullName.find("_");
					std::string widgetName = fullName.substr(markerPos + 1);
					std::string type = fullName.substr(0, markerPos - 1);

					// Use the assign function to create components for entities without having to know the type.
					Widget* createdWidget = m_widgetFactory[type](panel, pair.second.as<sol::table>());
					if (!createdWidget)
					{
						LOG_S(ERROR) << "Widget failed to be created: " << fullName;
					}

					hashmap.emplace(widgetName, createdWidget);
				});
			}
		}

		return hashmap;
	}

	void UI::update(const double dt)
	{
		// Iterate over panels, update, and call each widgets update function.
		for (auto& panel : m_panels)
		{
			panel->update(dt);
		}
	}

	void UI::render()
	{
		// Iterate over panels, render, and call each widgets render function.
		for (auto& panel : m_panels)
		{
			panel->render();
		}
	}
}