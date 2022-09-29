#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "DragonslayerEngine/Mesh.h"
#include "DragonslayerEngine/MeshGroup.h"

static void processNode(aiNode& node, const aiScene& scene, MeshGroup& meshGroup);
static Mesh* processMesh(aiMesh& aiMesh);

MeshGroup MeshGroup::loadFromFile(const std::string& filePath)
{
	MeshGroup meshGroup;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP " << importer.GetErrorString() << std::endl;
	} else {
		processNode(*scene->mRootNode, *scene, meshGroup);
	}

	return meshGroup;
}

void processNode(aiNode& node, const aiScene& scene, MeshGroup& meshGroup)
{
	for (unsigned int i = 0u; i < node.mNumMeshes; ++i) {
		Mesh* mesh = processMesh(*scene.mMeshes[node.mMeshes[i]]);
		if (mesh != nullptr) {
			mesh->calculateTangents();
			mesh->init();
			meshGroup.meshes.push_back(mesh);
		}
	}
	
	for (unsigned int i = 0u; i < node.mNumChildren; ++i) {
		processNode(*node.mChildren[i], scene, meshGroup);
	}
}

Mesh* processMesh(aiMesh& aiMesh)
{
	if (!aiMesh.HasPositions() || !aiMesh.HasNormals() || !aiMesh.HasTextureCoords(0)) {
		std::cout << "WARNING::ASSIMP Failed to load mesh " << aiMesh.mName.C_Str() << ", it does not have all the necessary properties(position & normals & texture coordinates" << std::endl;
		return nullptr;
	}
	
	Mesh* mesh = new Mesh(std::string(aiMesh.mName.C_Str()));

	for (unsigned int i = 0; i < aiMesh.mNumVertices; ++i) {

		mesh->vertices.push_back({
			aiMesh.mVertices[i].x,
			aiMesh.mVertices[i].y,
			aiMesh.mVertices[i].z,
			1.0
		});

		mesh->normals.push_back({
			aiMesh.mNormals[i].x,
			aiMesh.mNormals[i].y,
			aiMesh.mNormals[i].z
		});

		// A mesh can have multiple texture coordinates on the same vertex, however this is only used
		// on very special cases which we are currently ignoring

		// Currently ignoring the third coordinate
		mesh->textCoords.push_back({
			aiMesh.mTextureCoords[0][i].x,
			aiMesh.mTextureCoords[0][i].y
		});
		

		if (aiMesh.HasTangentsAndBitangents()) {
			mesh->tangents.push_back({
				aiMesh.mTangents[i].x,
				aiMesh.mTangents[i].y,
				aiMesh.mTangents[i].z
				});
		}
	}

	if (aiMesh.HasFaces()) {

		for (unsigned int i = 0; i < aiMesh.mNumFaces; i++)
		{
			aiFace face = aiMesh.mFaces[i];

			for (unsigned int j = 0; j < face.mNumIndices; ++j) {
				mesh->indices.push_back(
					face.mIndices[j]
				);
			}
		}
	}


	return mesh;
}
