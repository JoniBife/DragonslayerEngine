#ifndef SKY_H
#define SKY_H

#include "textures/CubeMap.h"
#include <vector>
#include <string>

class Sky {

private:
	CubeMap cubeMap;
	std::vector<std::string> facesPaths;

};

#endif

