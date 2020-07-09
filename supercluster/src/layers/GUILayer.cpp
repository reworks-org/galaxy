///
/// GUILayer.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>
#include <galaxy/res/FontBook.hpp>
#include <galaxy/fs/FileSystem.hpp>
#include <galaxy/res/ShaderBook.hpp>
#include <galaxy/ui/widgets/Image.hpp>

#include <galaxy/core/ServiceLocator.hpp>

#include "GUILayer.hpp"

///
/// Core namespace.
///
namespace sc
{
	GUILayer::GUILayer(protostar::ProtectedDouble* dt) noexcept
	{
		setName("GUILayer");

		m_gui.construct(dt, SL_HANDLE.pool());
		auto* theme = m_gui.createTheme();

		theme->setWindow(SL_HANDLE.window());
		theme->setRenderer(SL_HANDLE.renderer());

		theme->store<galaxy::FontBook>(SL_HANDLE.fontbook());
		theme->store<galaxy::ShaderBook>(SL_HANDLE.shaderbook());

		auto fp = std::filesystem::path(galaxy::FileSystem::s_root + galaxy::FileSystem::s_json + "atlas.json");
		std::ifstream ifs(fp.string(), std::ifstream::in);
		if (ifs.good())
		{
			nlohmann::json root;
			ifs >> root;

			auto arr = root.at("textures");
			std::for_each(arr.begin(), arr.end(), [&](const nlohmann::json& texture)
			{
				auto file = galaxy::FileSystem::s_root + galaxy::FileSystem::s_textures + texture.get<std::string>();
				theme->addTextureToAtlas(file);
			});

			ifs.close();

			theme->createTextureAtlas(*SL_HANDLE.shaderbook()->get("render_to_texture"));
		}

		auto* atlas = theme->getAtlas();
		auto parent = atlas->getTexQuad("ui_oga");

		atlas->defineCustomQuad("button_pressed", { parent.m_x + 353, parent.m_y + 302, 169, 32 });
		atlas->defineCustomQuad("button_default", { parent.m_x + 353, parent.m_y + 346, 169, 32 });
		atlas->defineCustomQuad("button_hovered", { parent.m_x + 354, parent.m_y + 390, 169, 32 });
		atlas->defineCustomQuad("textbox", { parent.m_x + 19, parent.m_y + 866, 566, 153 });
		atlas->defineCustomQuad("image", { parent.m_x + 31, parent.m_y + 75, 312, 287 });

		m_gui.createWidget<galaxy::Image>("image", 10.0f, 10.0f);
	}

	GUILayer::~GUILayer() noexcept
	{
	}

	void GUILayer::events() noexcept
	{
	}

	void GUILayer::update(protostar::ProtectedDouble* dt) noexcept
	{
	}

	void GUILayer::render(qs::Camera& camera) noexcept
	{
		m_gui.render(camera, "spritebatch");
	}
}