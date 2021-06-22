#ifndef MESH_LOADER_H
#define MESH_LOADER_H

#include <iostream>
#include "Mesh.h"
#include "../math/Vec4.h"
#include "../math/Vec3.h"
#include "../math/Vec2.h"

class MeshLoader {

public:
	static Mesh* loadFromFile(const std::string& filePath);
private:

	static Mesh* createMeshFromFileStream(std::ifstream& meshFile);

	static Vec4 parseVertex(std::stringstream& sin);

	static Vec3 parseNormal(std::stringstream& sin);

	static Vec2 parseTextCoord(std::stringstream& sin);

	static void parseFace(std::stringstream& sin,
		std::vector<unsigned int>& verticesIdx,
		std::vector<unsigned int>& textCoordsIdx,
		std::vector<unsigned int>& normalsIdx,
		std::vector<Vec3>& normalsData,
		std::vector<Vec2>& textCoordsData);
};
#endif
