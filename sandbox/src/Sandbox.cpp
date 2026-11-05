///
/// Sandbox.cpp
/// sandbox
///
/// Refer to LICENSE.txt for more details.
///

#include "Sandbox.hpp"

Sandbox::Sandbox(const std::string& name)
	: Scene {name}
{
}

Sandbox::~Sandbox()
{
}

void Sandbox::on_push()
{
}

void Sandbox::on_pop()
{
}

void Sandbox::on_event(SDL_Event& event)
{
}

void Sandbox::update(galaxy::EntityManager& em)
{
}

void Sandbox::render()
{
}
