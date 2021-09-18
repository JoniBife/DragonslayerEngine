#ifndef RENDERER_COMPONENT_H
#define RENDERER_COMPONENT_H

#include "Component.h"

namespace core {

	class RendererComponent : public Component {

	protected:
		RendererComponent(const std::string& name);

	public:
		RendererComponent();
		bool getType() const override;

	};

}

#endif#pragma once
