///
/// UI.cpp
/// galaxy
///
/// Created by reworks on 14/09/2018.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/World.hpp"
#include "galaxy/fs/VirtualFS.hpp"
#include "galaxy/core/ServiceLocator.hpp"

#include "galaxy/ui/widgets/Button.hpp"
#include "galaxy/ui/widgets/Image.hpp"
#include "galaxy/ui/widgets/Label.hpp"
#include "galaxy/ui/widgets/ProgressBar.hpp"
#include "galaxy/ui/widgets/Slider.hpp"
#include "galaxy/ui/widgets/Textbox.hpp"
#include "galaxy/ui/widgets/TextInput.hpp"
#include "galaxy/ui/widgets/ToggleButton.hpp"

#include "UI.hpp"

namespace galaxy
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
	
	void UI::createFromScript(const std::string& luaScript, WidgetStorage* widgetsMap, ThemeStorage* themesMap)
	{
		// Load script into lua.
		std::string fullPath = Locator::world->m_scriptFolderPath + luaScript;
		Locator::lua->script(Locator::virtualFS->openAsString(fullPath));

		// Panel currently being created.
		galaxy::Panel* panel = nullptr;

		// Get master table and check for validity.
		sol::table ui = Locator::lua->get<sol::table>("ui");
		if (!ui.valid() || ui.empty())
		{
			LOG_S(ERROR) << "UI script table \"ui\" is invalid or empty for script: " << luaScript;
		}
		else
		{
			// Create theme(s).
			sol::table themes = ui.get<sol::table>("themes");
			if (!themes.valid() || themes.empty())
			{
				LOG_S(ERROR) << "UI script table \"themes\" is invalid or empty for script: " << luaScript;
			}
			else
			{
				// Load each theme.
				themes.for_each([&](std::pair<sol::object, sol::object> pair)
				{
					sol::table themeTable = pair.second.as<sol::table>();
					if (!themeTable.valid() || themeTable.empty())
					{
						LOG_S(ERROR) << "Themes script table \"" << pair.first.as<std::string>() << "\" is invalid or empty for script: " << luaScript;
					}
					else
					{
						// Have to work around std::pair's constructor issues.
						themesMap->emplace(pair.first.as<std::string>(), themeTable);
					}
				});
			}

			sol::table panelsTable = ui.get<sol::table>("panels");
			int x = 0, y = 0, w = 0, h = 0;
			if (!panelsTable.valid() || panelsTable.empty())
			{
				LOG_S(ERROR) << "UI script table \"panels\" is invalid or empty for script: " << luaScript;
			}
			else
			{
				// Iterate over each panel.
				panelsTable.for_each([&](std::pair<sol::object, sol::object> pair)
				{
					// Retrieve table, checking validity to ensure loading can proceed.
					sol::table panelTable = pair.second.as<sol::table>();
					if (!panelTable.valid() || panelTable.empty())
					{
						LOG_S(ERROR) << "Panels script table \"" << pair.first.as<std::string>() << "\" is invalid or empty for script: " << luaScript;
					}
					else
					{
						// Now retrieve bounds and check for validity.
						sol::table bounds = panelTable.get<sol::table>("bounds");
						if (!bounds.valid() || bounds.empty())
						{
							LOG_S(ERROR) << "Panel table \"bounds\" is invalid or empty for script: " << luaScript;
						}
						else
						{
							x = bounds.get<int>("x");
							y = bounds.get<int>("y");
							w = bounds.get<int>("w");
							h = bounds.get<int>("h");
						}

						// Check to see if colour / image exists.
						auto colourCheck = panelTable["colour"];
						auto themeCheck = panelTable["theme"]; // Name of theme from above.
						if (colourCheck.valid())
						{
							// Make sure colour is not empty.
							sol::table colour = panelTable.get<sol::table>("colour");
							if (!colour.valid() || colour.empty())
							{
								LOG_S(ERROR) << "Panel table \"colour\" is invalid or empty for script: " << luaScript;
							}
							else
							{
								panel = addPanel(galaxy::Rect<int>{x, y, w, h}, al_map_rgba(colour.get<int>("r"), colour.get<int>("g"), colour.get<int>("b"), colour.get<int>("a")));
							}
						}
						else if (themeCheck.valid()) // Else if image is valid.
						{
							std::string themeName = panelTable.get<std::string>("theme");
							std::string themeTextureID = panelTable.get<std::string>("themeTextureID");
							if (!themeName.empty())
							{
								panel = addPanel(galaxy::Rect<int>{x, y, w, h}, &(themesMap->at(themeName)), themeTextureID);
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

						// Now to load the widgets.
						sol::table widgets = panelTable.get<sol::table>("widgets");
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
								std::string type = fullName.substr(markerPos + 1);
								std::string widgetName = fullName.substr(0, markerPos);

								// The table and theme
								sol::table wfTable = pair.second.as<sol::table>();
								UITheme* wfTheme = &(themesMap->at(wfTable.get<std::string>("theme")));

								// Use the assign function to create components for entities without having to know the type.
								Widget* createdWidget = m_widgetFactory[type](panel, wfTable, wfTheme);
								if (!createdWidget)
								{
									LOG_S(ERROR) << "Widget failed to be created: " << fullName;
								}

								// Make tooltip if there is the table for one.
								auto ttCheck = wfTable["tooltip"];
								if (ttCheck.valid())
								{
									sol::table ttTable = wfTable["tooltip"];
									createdWidget->setTooltip(ttTable.get<std::string>("text"), ttTable.get<std::string>("texture"), galaxy::Vector2<int>{ttTable.get<int>("sectionL"), ttTable.get<int>("sectionR")}, &(themesMap->at(ttTable.get<std::string>("theme"))));
								}

								widgetsMap->emplace(widgetName, createdWidget);
							});
						}
					}
				});
			}
		}
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