#include "PhysicsSimulation.h"
#include "../meshes/MeshLoader.h"
#include "../view/Transformations.h"
#include "../math/MathAux.h"
#include "../controllers/FreeCameraController.h"
#include "../utils/ColorRGBA.h"
#include "../core/GameObject.h"
#include "../core/Renderer.h"
#include "../core/MeshRenderer.h"
#include "../core/BlinnPhongMaterial.h"
#include "../scripts/CubeController.h"

using namespace core;

Mesh* cubeMesh;
ShaderProgram* bliinPhongShader;
SceneNode* cube;

GLint cameraPosL;

FreeCameraController* cameraController;

GameObject* gm;

void PhysicsSimulation::start() {

	cubeMesh = MeshLoader::loadFromFile("../Engine/objs/cube.obj");
	cubeMesh->paint(ColorRGBA::ORANGE);
	cubeMesh->init();

	Mesh* sphereMesh = MeshLoader::loadFromFile("../Engine/objs/sphere.obj");
	sphereMesh->paint(ColorRGBA::BLUE);
	sphereMesh->init();

	gm = new GameObject("Cube");
	BlinnPhongMaterial* mat = new BlinnPhongMaterial();

	GLuint sharedMatricesIndex = mat->getShaderProgram().getUniformBlockIndex("SharedMatrices");
	mat->getShaderProgram().bindUniformBlock(sharedMatricesIndex, getCamera()->getUboBindingPoint());

	MeshRenderer* mr = new MeshRenderer(cubeMesh, mat);
	gm->addComponent(mr);
	gm->getTransform()->rotation.x  = PI / 4.0f;

	CubeController* cubeController = new CubeController();
	gm->addComponent(cubeController);

	GameObject* gm2 = new GameObject("Sphere");
	MeshRenderer* mr2 = new MeshRenderer(sphereMesh, mat);
	gm2->addComponent(mr2);

	gm->addChildren(gm2);
	
}

void PhysicsSimulation::update() {
	
}

void PhysicsSimulation::end() {
	delete cubeMesh;
	delete bliinPhongShader;
	delete cameraController;
}

