#ifndef MESH_GROUP_H
#define MESH_GROUP_H

#include <vector>
#include "Mesh.h"

struct MeshGroup {

	std::vector<Mesh*> meshes;
	
	static MeshGroup loadFromFile(const std::string& filePath);

};

#endif

