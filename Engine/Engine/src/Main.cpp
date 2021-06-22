#include "Engine.h"
#include <iostream>
#include "apps/PhysicsSimulation.h"

int main(int argc, char* argv[])
{
	Engine* eng = new PhysicsSimulation();
	eng->run();
	delete eng;
	exit(EXIT_SUCCESS);
}
