#include "RendererComponent.h"

using namespace core;

RendererComponent::RendererComponent(const std::string& name) : Component(name) { }

RendererComponent::RendererComponent() : RendererComponent("Renderer") {}

bool RendererComponent::getType() const
{
    return false;
}