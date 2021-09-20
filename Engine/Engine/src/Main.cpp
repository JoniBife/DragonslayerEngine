#include "Engine.h"
#include <iostream>

int main(int argc, char* argv[])
{
	Engine* eng = new Engine();
	eng->run();
	delete eng;
	
	exit(EXIT_SUCCESS);
}
