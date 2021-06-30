#ifndef RENDERER_H
#define RENDERER_H

#include "Component.h"

namespace core {

	class Renderer : public Component {

	protected:
		Renderer(const std::string& name);

	public:
		Renderer();
		bool getType() const override;
		
		virtual void onGUI() override = 0;

		virtual void update() override = 0;

	};

}

#endif
