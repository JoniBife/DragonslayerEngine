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

using namespace core;

Mesh* cubeMesh;
ShaderProgram* bliinPhongShader;
SceneNode* cube;

GLint cameraPosL;

FreeCameraController* cameraController;

GameObject* gm;

static void updateCameraPositionInShaders() {

	bliinPhongShader->use();
	bliinPhongShader->setUniform(cameraPosL, cameraController->position);
	bliinPhongShader->stopUsing();
}

void PhysicsSimulation::start() {
	//loadShaders(getCamera());

	cubeMesh = MeshLoader::loadFromFile("../Engine/objs/sphere.obj");
	cubeMesh->paint(ColorRGBA::ORANGE);
	cubeMesh->init();
	//cube = getSceneGraph()->getRoot()->createChild(cubeMesh, Mat4::IDENTITY, bliinPhongShader);


	gm = new GameObject("Cube");
	BlinnPhongMaterial* mat = new BlinnPhongMaterial();

	GLuint sharedMatricesIndex = mat->getShaderProgram().getUniformBlockIndex("SharedMatrices");
	mat->getShaderProgram().bindUniformBlock(sharedMatricesIndex, getCamera()->getUboBindingPoint());

	MeshRenderer* mr = new MeshRenderer(cubeMesh, mat);
	gm->addComponent(mr);

	gm->getTransform()->rotation.x  = PI / 4.0f;

	/*/std::vector<Renderer*> rr = gm->getComponents<Renderer>();

	for (auto r2 : rr) {
		std::cout << r2 << std::endl;
	}

	gm->getTransform()->position = { 1.0f, 5.0f, 10.0f };

	std::cout << gm->getTransform()->position << std::endl;

	GameObject* piloca2 = new GameObject("cl2");
	GameObject* piloca = new GameObject("cl");
	piloca->addChildren(piloca2);
	GameObject* ines = new GameObject("Clone");
	ines->addChildren(piloca);

	GameObject* perna1 = new GameObject("perna");

	GameObject* perna2 = new GameObject("perna");

	GameObject* joao = new GameObject("Joao");
	joao->addChildren(perna1);
	joao->addChildren(perna2);*/

}

void PhysicsSimulation::update() {
	gm->getTransform()->rotation.y += (PI / 8.0f) * getElapsedTime();
}

void PhysicsSimulation::end() {
	delete cubeMesh;
	delete bliinPhongShader;
	delete cameraController;
}

