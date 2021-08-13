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
#include "../core/PBRMaterial.h"

using namespace core;

Mesh* cubeMesh;
ShaderProgram* bliinPhongShader;
SceneNode* cube;

GLint cameraPosL;

FreeCameraController* cameraController;

void PhysicsSimulation::start() {

	Mesh* sphereMesh = MeshLoader::loadFromFile("../Engine/objs/sphere.obj");
	sphereMesh->paint(ColorRGBA::BLUE);
	sphereMesh->init();

	cubeMesh = MeshLoader::loadFromFile("../Engine/objs/cube.obj");
	cubeMesh->paint(ColorRGBA::GREY);
	cubeMesh->init();

	BlinnPhongMaterial* mat = new BlinnPhongMaterial();

	GameObject* gm2 = new GameObject("Sphere Blinn-Phong");
	{
		MeshRenderer* mr2 = new MeshRenderer(sphereMesh, mat);
		gm2->addComponent(mr2);
		gm2->getTransform()->position.x = 2.0f;
	}

	GameObject* gm = new GameObject("Cube");
	{
		MeshRenderer* mr = new MeshRenderer(cubeMesh, mat);
		//CubeController* cubeController = new CubeController();
		//gm->addComponent(cubeController);
		gm->addComponent(mr);
		gm->getTransform()->position.x = 1.0f;
		gm->getTransform()->rotation.x = PI / 2.0f;
		gm->getTransform()->scale.y = 2.0f;
		gm->addChildren(gm2);
	}

	PBRMaterial* pbrMat = new PBRMaterial();

	GameObject* spherePbr = new GameObject("Sphere PBR");
	MeshRenderer* mr3 = new MeshRenderer(sphereMesh, pbrMat);
	spherePbr->addComponent(mr3);
}

void PhysicsSimulation::update() {
	
}

void PhysicsSimulation::end() {
	delete cubeMesh;
	delete bliinPhongShader;
	delete cameraController;
}

