#include "Renderer.h"

core::Renderer::Renderer(const std::string& name) : Component(name) { }

core::Renderer::Renderer() : Renderer("Renderer") {}

bool core::Renderer::getType() const
{
    return false;
}

void core::Renderer::onGUI() const
{
}
