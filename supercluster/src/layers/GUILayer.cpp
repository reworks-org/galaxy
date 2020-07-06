///
/// GUILayer.cpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#define RTT_SHADER 0
#define SPRITEBATCH_SHADER 1

#include <nlohmann/json.hpp>
#include <galaxy/res/FontBook.hpp>
#include <galaxy/fs/FileSystem.hpp>
#include <galaxy/res/ShaderBook.hpp>

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
		auto* theme = m_gui.addTheme();

		theme->setWindow(SL_HANDLE.window());
		theme->setRenderer(SL_HANDLE.renderer());

		theme->store<galaxy::FontBook>(SL_HANDLE.fontbook());
		theme->store<galaxy::ShaderBook>(SL_HANDLE.shaderbook());

		auto fp = std::filesystem::path(galaxy::FileSystem::s_root + galaxy::FileSystem::s_json + "atlas.json");
		nlohmann::json root;
		std::ifstream ifs(fp.string(), std::ifstream::in);
		ifs >> root;
		ifs.close();

		auto arr = root.at("textures");
		std::for_each(arr.begin(), arr.end(), [&](const nlohmann::json& texture)
		{
			theme->addTextureToAtlas(texture.get<std::string>());
		});

		theme->createTextureAtlas(*SL_HANDLE.shaderbook()->get(RTT_SHADER));
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
		//m_gui.render(camera, SPRITEBATCH_SHADER);
	}
}