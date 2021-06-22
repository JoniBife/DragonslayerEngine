#ifndef PHYSICS_SIMULATION
#define PHYSICS_SIMULATION

#include "../Engine.h"

class PhysicsSimulation : public Engine {

public:
	PhysicsSimulation() {}

	void start() override;

	void update() override;

	void end() override;

};

#endif
