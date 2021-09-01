#include "Engine.h"
#include <iostream>
#include "apps/PhysicsSimulation.h"
#include "math/Mat4.h"
#include "math/MathAux.h"

int main(int argc, char* argv[])
{
	Engine* eng = new PhysicsSimulation();
	eng->run();
	delete eng;
	exit(EXIT_SUCCESS);
}
