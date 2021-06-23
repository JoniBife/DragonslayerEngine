#ifndef RENDERER_H
#define RENDERER_H

#include "Component.h"

namespace core {

	class Renderer : public Component {

		virtual bool getType() const override;

	};

}


#endif
