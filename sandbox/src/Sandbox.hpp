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
};

#endif
