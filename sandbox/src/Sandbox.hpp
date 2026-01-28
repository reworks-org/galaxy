///
/// Sandbox.hpp
/// sandbox
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SANDBOX_SANDBOX_HPP_
#define SANDBOX_SANDBOX_HPP_

#include <format>

#include <entt/signal/dispatcher.hpp>
#include <glad/glad.h>
#include <tinyfiledialogs.h>

#include <galaxy/core/Application.hpp>
#include <galaxy/core/Window.hpp>
#include <galaxy/graphics/Renderer.hpp>
#include <galaxy/graphics/gl/Shader.hpp>
#include <galaxy/graphics/gl/Texture.hpp>
#include <galaxy/graphics/Enums.hpp>
#include <galaxy/scene/SceneManager.hpp>

class Sandbox final : public galaxy::Scene
{
public:
	Sandbox(const std::string& name);
	virtual ~Sandbox();

	void on_push() override;
	void on_pop() override;
	void on_event(SDL_Event& event) override;
	void update(galaxy::EntityManager& em) override;
	void render() override;

private:
	galaxy::RenderPass     pass;
	galaxy::RenderPipeline pipeline;
	galaxy::Renderer       renderer;
	galaxy::Camera         camera;

	galaxy::Texture tex;
	galaxy::Texture tex2;
	galaxy::Texture tex3;
	galaxy::Shader  shader;

	std::vector<galaxy::Vertex> verts;
	std::vector<galaxy::Vertex> verts2;
	std::vector<galaxy::Vertex> verts3;

	galaxy::RenderCmd cmd1;
	galaxy::RenderCmd cmd2;
	galaxy::RenderCmd cmd3;
};

#endif
