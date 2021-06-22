#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H

#include <iostream>
#include "../shaders/ShaderProgram.h"
#include "../view/Camera.h"
#include "../meshes/Mesh.h"
#include "../math/Mat4.h"
#include "../textures/ITexture.h"
#include <list>

class SceneNode {

private:
	Mesh* mesh = nullptr;
	Mat4 model = Mat4::IDENTITY;
	GLint modelUniformLocation = GLint(-1);
	GLint normalUniformLocation = GLint(-1);
	ShaderProgram* shaderProgram = nullptr;
	std::vector<ITexture*> textures;
	std::list<SceneNode*> children;
	std::function<void(ShaderProgram*)> beforeDraw;
	std::function<void()> afterDraw;

	Mat4 retriveModelRecursively();

public:
	SceneNode* parent = nullptr;
	SceneNode();
	~SceneNode();

	SceneNode(Mesh* mesh, const Mat4& model, SceneNode* parent, ShaderProgram* shaderProgram);

	SceneNode* createChild(SceneNode* parent); // Copies the parents mesh to child
	SceneNode* createChild();
	//SceneNode* createChild(Mesh* mesh);
	SceneNode* createChild(Mesh* mesh, const Mat4& model);
	SceneNode* createChild(Mesh* mesh, const Mat4& model, ShaderProgram* shaderProgram);

	void deleteSceneNode();

	void setModel(const Mat4& model);
	void setShaderProgram(ShaderProgram* shaderProgram);
	void addTexture(ITexture* texture);
	void setBeforeDrawFunction(const std::function<void(ShaderProgram*)>& beforeDraw);
	void setAfterDrawFunction(const std::function<void()>& afterDraw);

	Mat4 getModel() const;
	Mesh* getMesh() const;
	ShaderProgram* getShaderProgram() const;
	std::list<SceneNode*> getChildren() const;
	std::vector<ITexture*> getTextures() const;

	void init();

	void draw();
};

class SceneGraph {

private:
	Camera* camera;
	SceneNode* root;

public:

	SceneGraph(Camera* camera);

	~SceneGraph();

	void init();

	void draw(const float elapsedTime);

	SceneNode* createChild();

	SceneNode* getRoot() const;

	Camera* getCamera() const;
};

#endif
