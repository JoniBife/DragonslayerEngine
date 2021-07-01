#include "Renderer.h"

using namespace core;

Renderer::Renderer(const std::string& name) : Component(name) { }

Renderer::Renderer() : Renderer("Renderer") {}

bool Renderer::getType() const
{
    return false;
}

void Renderer::onGUI()
{
}

void Renderer::update()
{
}
