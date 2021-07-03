#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "Component.h"

namespace core {

	class Controller : public Component {

	protected:
		Controller(const std::string& name);
		virtual ~Controller();

		// Inherited via Component
		bool getType() const override;

		bool isSingular() const override;

	};

}

#endif