#ifndef MESH_LOADER_H
#define MESH_LOADER_H

#include <iostream>
#include "Mesh.h"
#include <LMath/Vec4.hpp>
#include <LMath/Vec3.hpp>
#include <LMath/Vec2.hpp>

using namespace LMath;

class MeshLoader {

public:
	static Mesh* loadFromFile(const std::string& filePath);
private:

	static Mesh* createMeshFromFileStream(std::ifstream& meshFile, const std::string& filePath);

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
