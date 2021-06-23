#include "PhysicsSimulation.h"
#include "../meshes/MeshLoader.h"
#include "../view/Transformations.h"
#include "../math/MathAux.h"
#include "../controllers/FreeCameraController.h"
#include "../utils/ColorRGBA.h"

Mesh* cubeMesh;
ShaderProgram* bliinPhongShader;
SceneNode* cube;

GLint cameraPosL;

FreeCameraController* cameraController;

static void loadShaders(Camera* camera) {

	Shader vs(GL_VERTEX_SHADER, "../Engine/shaders/simpleVertexShader.glsl");
	Shader fs(GL_FRAGMENT_SHADER, "../Engine/shaders/fragmentShaderLighting.glsl");
	bliinPhongShader = new ShaderProgram(vs, fs);
	// Associating the shared matrix index with the binding point of the camera (0)
	GLuint sharedMatricesIndex = bliinPhongShader->getUniformBlockIndex("SharedMatrices");
	bliinPhongShader->bindUniformBlock(sharedMatricesIndex, camera->getUboBindingPoint());

	Vec3 lightPos(4.0f, 30.0f, 20.0f);

	Mat4 lightProj = ortho(-100.0f, 100.0f, -10.0f, 100.0f, 1.0f, 70.5f);

	Vec3 lightTarget(0.0f, 0.0f, 0.0f); // center
	Vec3 lightUp(0.0f, 1.0f, 0.0f); // up

	Mat4 lightView = lookAt(lightPos, lightTarget, lightUp);

	Mat4 LSM = lightProj * lightView;

	Vec3 lightColor(1.0f, 1.0f, 1.0f);
	float ambientStrength = 0.1f;

	float specularStrength = 0.7f;
	unsigned int shininess = 32;

	/////////////////// CYLINDER ///////////////////////
	GLint lightColorL = bliinPhongShader->getUniformLocation("lightColor");
	GLint ambientStrenghtL = bliinPhongShader->getUniformLocation("ambientStrength");
	GLint lightPositionL = bliinPhongShader->getUniformLocation("lightPosition");
	GLint specularStrengthL = bliinPhongShader->getUniformLocation("specularStrength");
	GLint shininessL = bliinPhongShader->getUniformLocation("shininess");
	cameraPosL = bliinPhongShader->getUniformLocation("viewPos");

	bliinPhongShader->use();
	bliinPhongShader->setUniform(lightColorL, lightColor);
	bliinPhongShader->setUniform(ambientStrenghtL, ambientStrength);
	bliinPhongShader->setUniform(lightPositionL, lightPos);
	bliinPhongShader->setUniform(specularStrengthL, specularStrength);
	bliinPhongShader->setUniform(shininessL, shininess);
	bliinPhongShader->stopUsing();
}
static void setupCamera(Camera* camera, GLFWwindow* window, int windowWidth, int windowHeight) {
	// Adding a spherical camera controller
	float cameraMovementSpeed = 30.0f;
	// Since we are looking at the -z axis in our initial orientation, yaw has to be set -90 degress otherwise we would look at +x axis
	float initialYaw = -90.0f;
	float initialPitch = 0.0f;

	Vec3 cameraPos(0.0f, 15.0f, 20.0f); // eye
	Vec3 cameraTarget(0.0f, 0.0f, 0.0f); // center
	Vec3 cameraFront = cameraTarget - cameraPos;
	Vec3 cameraUp(0.0f, 50.0f, 0.0f); // up

	Mat4 orthographicProj = ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.001f, 1000.0f);
	Mat4 perspectiveProj = perspective(PI / 2.0f, float(windowWidth / windowHeight), 0.001f, 1000.0f);

	//cameraController = new OrbitCameraController({ 0,0,0 }, Qtrn(1, 0, 0, 0), this->getWindow());
	cameraController = new FreeCameraController(cameraMovementSpeed, cameraPos, cameraFront, cameraUp, initialYaw, initialPitch, orthographicProj, perspectiveProj, window);

	//cameraController = new SphericalCameraController({ 0,0,0 }, Qtrn(1, 0, 0, 0), this->getWindow(), -5.0f);
	camera->addCameraController(cameraController);
	camera->setView(lookAt(cameraPos, cameraPos + cameraFront, cameraUp));
	camera->setProjection(perspectiveProj);
}

static void updateCameraPositionInShaders() {

	bliinPhongShader->use();
	bliinPhongShader->setUniform(cameraPosL, cameraController->position);
	bliinPhongShader->stopUsing();
}

void PhysicsSimulation::start() {
	loadShaders(getCamera());
	setupCamera(getCamera(), getWindow(), getWindowWidth(), getWindowHeight());

	cubeMesh = MeshLoader::loadFromFile("../Engine/objs/cube.obj");
	cubeMesh->paint(ColorRGBA::ORANGE);
	cube = getSceneGraph()->getRoot()->createChild(cubeMesh, Mat4::IDENTITY, bliinPhongShader);
}

void PhysicsSimulation::update() {
	updateCameraPositionInShaders();

}

void PhysicsSimulation::end() {
	delete cubeMesh;
	delete bliinPhongShader;
	delete cameraController;
}

