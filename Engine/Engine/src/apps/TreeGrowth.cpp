#include "TreeGrowth.h"
#include "../view/Transformations.h"
#include "../controllers/FreeCameraController.h"
#include "../math/MathAux.h"
#include "../textures/Texture2D.h"
#include "TreeGrowthUI.h"
#include "../textures/DepthMap.h"
#include "../math/Qtrn.h"
#include "BranchNode.h"
#include "BranchModule.h"
#include "Morphospace.h"
#include "../utils/OpenGLUtils.h"
#include "Leaves.h"
#include "Tree.h"

static ShaderProgram* spCylinder; // Used with the cylinders
static ShaderProgram* spDepthMapCylinder; // Used with the cylinders to generate the depth map
static ShaderProgram* spShadows; // Used with any other object
static ShaderProgram* spDepthMap; // Used with any other object to generate the depth map
static ShaderProgram* spSimple;
static ShaderProgram* spLeaves;
static ShaderProgram* spLeavesDepthMap;

static Mesh* points;
static Mesh* plane;
static FreeCameraController* cameraController;
static SceneNode* cylinder;
static SceneNode* sceneNodeSphere;
static SceneNode* sceneNodePlane;
static float numberOfFrames = 0.0f;
static float totalElapsedTime = 0.0f;

static Tree* tree;

static GLint cameraPosLsp;
static GLint cameraPosLspShadows;
static GLint cameraPosLspLeaves;
static Vec3 lightPosition(4.0f, 30.0f, 20.0f);

static Texture2D* woodTexture;
static Texture2D* woodTextureNormalMap;
static Texture2D* leavesTexture;
static Texture2D* planeTexture;
static DepthMap* depthMap;

static TreeGrowthUI* treeGrowthUI;

static GLint normalMappingL;
static GLint normalMapping = GL_TRUE;

static Mesh* sphere;

static Leaves* leaves;

static bool paused = true;

static void setupShaders(SceneGraph* sceneGraph, Camera* camera) {

	//////////// CYLINDER SHADERS /////////////////////////
	Shader vs(GL_VERTEX_SHADER, "../Engine/shaders/cylinder/cylinderVertexShader.glsl");
	Shader gs(GL_GEOMETRY_SHADER, "../Engine/shaders/cylinder/cylinderGeometryShader.geom");
	Shader fs(GL_FRAGMENT_SHADER, "../Engine/shaders/cylinder/cylinderFragmentShader.glsl");
	spCylinder = new ShaderProgram(vs, gs, fs);
	// Associating the shared matrix index with the binding point of the camera (0)
	GLuint sharedMatricesIndex = spCylinder->getUniformBlockIndex("SharedMatrices");
	spCylinder->bindUniformBlock(sharedMatricesIndex, camera->getUboBindingPoint());

	//////////// DEPTH MAP SHADERS /////////////////////////
	Shader vs1(GL_VERTEX_SHADER, "../Engine/shaders/vertexShaderSM.glsl");
	Shader fs1(GL_FRAGMENT_SHADER, "../Engine/shaders/fragmentShaderSM.glsl");
	spDepthMap = new ShaderProgram(vs1, fs1);
	// Associating the shared matrix index with the binding point of the camera (0)
	sharedMatricesIndex = spDepthMap->getUniformBlockIndex("SharedMatrices");
	spDepthMap->bindUniformBlock(sharedMatricesIndex, camera->getUboBindingPoint());

	//////////// PLANE SHADERS /////////////////////////
	Shader vs2(GL_VERTEX_SHADER, "../Engine/shaders/vertexShaderShadows.glsl");
	Shader fs2(GL_FRAGMENT_SHADER, "../Engine/shaders/fragmentShaderShadows.glsl");
	spShadows = new ShaderProgram(vs2, fs2);
	// Associating the shared matrix index with the binding point of the camera (0)
	sharedMatricesIndex = spShadows->getUniformBlockIndex("SharedMatrices");
	spShadows->bindUniformBlock(sharedMatricesIndex, camera->getUboBindingPoint());

	//////////// DEPTH MAP CYLINDER SHADERS /////////////////////////
	Shader vs3(GL_VERTEX_SHADER, "../Engine/shaders/cylinder/cylinderVertexShader.glsl");
	Shader gs3(GL_GEOMETRY_SHADER, "../Engine/shaders/cylinder/cylinderGeometryShaderSM.geom");
	Shader fs3(GL_FRAGMENT_SHADER, "../Engine/shaders/fragmentShaderSM.glsl");
	spDepthMapCylinder = new ShaderProgram(vs3, gs3,fs3);
	// Associating the shared matrix index with the binding point of the camera (0)
	sharedMatricesIndex = spDepthMapCylinder->getUniformBlockIndex("SharedMatrices");
	spDepthMapCylinder->bindUniformBlock(sharedMatricesIndex, camera->getUboBindingPoint());

	//////////// LEAVES SHADERS /////////////////////////
	Shader vs4(GL_VERTEX_SHADER, "../Engine/shaders/leaves/vertexShaderBlending.glsl");
	Shader fs4(GL_FRAGMENT_SHADER, "../Engine/shaders/leaves/fragmentShaderBlending.glsl");
	spLeaves = new ShaderProgram(vs4, fs4);
	// Associating the shared matrix index with the binding point of the camera (0)
	sharedMatricesIndex = spLeaves->getUniformBlockIndex("SharedMatrices");
	spLeaves->bindUniformBlock(sharedMatricesIndex, camera->getUboBindingPoint());

	//////////// DEPTH MAP LEAVES SHADERS /////////////////////////
	Shader vs5(GL_VERTEX_SHADER, "../Engine/shaders/leaves/vertexShaderBlendingSM.glsl");
	Shader fs5(GL_FRAGMENT_SHADER, "../Engine/shaders/leaves/fragmentShaderBlendingSM.glsl");
	spLeavesDepthMap = new ShaderProgram(vs5, fs5);
	// Associating the shared matrix index with the binding point of the camera (0)
	sharedMatricesIndex = spLeavesDepthMap->getUniformBlockIndex("SharedMatrices");
	spLeavesDepthMap->bindUniformBlock(sharedMatricesIndex, camera->getUboBindingPoint());

	// Loading the shaders and creating the shader program
	Shader vs6(GL_VERTEX_SHADER, "../Engine/shaders/simpleVertexShader.glsl");
	Shader fs6(GL_FRAGMENT_SHADER, "../Engine/shaders/simpleFragmentShader.glsl");
	spSimple = new ShaderProgram(vs6, fs6);
	// Associating the shared matrix index with the binding point of the camera (0)
	sharedMatricesIndex = spSimple->getUniformBlockIndex("SharedMatrices");
	spSimple->bindUniformBlock(sharedMatricesIndex, camera->getUboBindingPoint());
}

static void setupTextures() {
	// Loading textures
	woodTexture = new Texture2D("../Engine/textures/tree/barkTexture.jpg");
	woodTextureNormalMap = new Texture2D("../Engine/textures/tree/barkNormalMap.jpg");
	planeTexture = new Texture2D("../Engine/textures/tree/grass.png");
	depthMap = new DepthMap(2048, 2048);

	GLint textureID = spCylinder->getUniformLocation("diffuseMap");
	GLint normalMapID = spCylinder->getUniformLocation("normalMap");
	GLint shadowMapID = spCylinder->getUniformLocation("shadowMap");
	spCylinder->use();
	spCylinder->setUniform(textureID, 0);
	spCylinder->setUniform(normalMapID, 1);
	spCylinder->setUniform(shadowMapID, 2);
	spCylinder->stopUsing();

	textureID = spShadows->getUniformLocation("diffuseMap");
	shadowMapID = spShadows->getUniformLocation("shadowMap");
	spShadows->use();
	spShadows->setUniform(textureID, 0);
	spShadows->setUniform(shadowMapID, 1);
	spShadows->stopUsing();

	textureID = spLeaves->getUniformLocation("diffuseMap");
	shadowMapID = spLeaves->getUniformLocation("shadowMap");
	spLeaves->use();
	spLeaves->setUniform(textureID, 0);
	spLeaves->setUniform(shadowMapID, 1);
	spLeaves->stopUsing();

	textureID = spLeavesDepthMap->getUniformLocation("diffuseMap");
	spLeavesDepthMap->use();
	spLeavesDepthMap->setUniform(textureID, 0);
	spLeavesDepthMap->stopUsing();
}

static void setupLightAndShadows() {

	Mat4 lightProj = ortho(-100.0f, 100.0f, -10.0f, 100.0f, 1.0f, 70.5f);

	Vec3 lightTarget(0.0f, 0.0f, 0.0f); // center
	Vec3 lightUp(0.0f, 1.0f, 0.0f); // up

	Mat4 lightView = lookAt(lightPosition, lightTarget, lightUp);

	Mat4 LSM = lightProj * lightView;

	Vec3 lightColor(1.0f, 1.0f, 1.0f);
	float ambientStrength = 0.1f;

	float specularStrength = 0.7f;
	unsigned int shininess = 32;

	/////////////////// CYLINDER ///////////////////////
	GLint lightColorL = spCylinder->getUniformLocation("lightColor");
	GLint ambientStrenghtL = spCylinder->getUniformLocation("ambientStrength");
	GLint lightPositionL = spCylinder->getUniformLocation("lightPosition");
	GLint specularStrengthL = spCylinder->getUniformLocation("specularStrength");
	GLint shininessL = spCylinder->getUniformLocation("shininess");
	normalMappingL = spCylinder->getUniformLocation("normalMapping");
	cameraPosLsp = spCylinder->getUniformLocation("viewPos");
	GLint uniformLSM = spCylinder->getUniformLocation("lightSpaceMatrix");

	spCylinder->use();
	spCylinder->setUniform(lightColorL, lightColor);
	spCylinder->setUniform(ambientStrenghtL, ambientStrength);
	spCylinder->setUniform(lightPositionL, lightPosition);
	spCylinder->setUniform(specularStrengthL, specularStrength);
	spCylinder->setUniform(shininessL, shininess);
	spCylinder->setUniform(normalMappingL, normalMapping);
	spCylinder->setUniform(uniformLSM, LSM);
	spCylinder->stopUsing();

	spDepthMapCylinder->use();
	uniformLSM = spDepthMapCylinder->getUniformLocation("lightSpaceMatrix");
	spDepthMapCylinder->setUniform(uniformLSM, LSM);
	spDepthMapCylinder->stopUsing();
	/////////////////////////////////////////////////////

	/////////////////// LEAVES ///////////////////////
	lightColorL = spLeaves->getUniformLocation("lightColor");
	ambientStrenghtL = spLeaves->getUniformLocation("ambientStrength");
	lightPositionL = spLeaves->getUniformLocation("lightPosition");
	specularStrengthL = spLeaves->getUniformLocation("specularStrength");
	shininessL = spLeaves->getUniformLocation("shininess");
	cameraPosLspLeaves = spLeaves->getUniformLocation("viewPos");
	uniformLSM = spLeaves->getUniformLocation("lightSpaceMatrix");

	spLeaves->use();
	spLeaves->setUniform(lightColorL, lightColor);
	spLeaves->setUniform(ambientStrenghtL, 0.3f);
	spLeaves->setUniform(lightPositionL, lightPosition);
	spLeaves->setUniform(specularStrengthL, specularStrength);
	spLeaves->setUniform(shininessL, shininess);
	spLeaves->setUniform(uniformLSM, LSM);
	spLeaves->stopUsing();

	spLeavesDepthMap->use();
	uniformLSM = spLeavesDepthMap->getUniformLocation("lightSpaceMatrix");
	spLeavesDepthMap->setUniform(uniformLSM, LSM);
	spLeavesDepthMap->stopUsing();
	/////////////////////////////////////////////////////


	/////////////////// OTHER OBJECTS ///////////////////
	lightColorL = spShadows->getUniformLocation("lightColor");
	ambientStrenghtL = spShadows->getUniformLocation("ambientStrength");
	lightPositionL = spShadows->getUniformLocation("lightPosition");
	specularStrengthL = spShadows->getUniformLocation("specularStrength");
	shininessL = spShadows->getUniformLocation("shininess");
	cameraPosLspShadows = spShadows->getUniformLocation("viewPos");
	uniformLSM = spShadows->getUniformLocation("lightSpaceMatrix");

	unsigned int shininessPlane = 4;

	spShadows->use();
	spShadows->setUniform(lightColorL, lightColor);
	spShadows->setUniform(ambientStrenghtL, 0.3f);
	spShadows->setUniform(lightPositionL, lightPosition);
	spShadows->setUniform(specularStrengthL, 0.1f);
	spShadows->setUniform(shininessL, shininessPlane);
	spShadows->setUniform(uniformLSM, LSM);
	spShadows->stopUsing();

	spDepthMap->use();
	uniformLSM = spDepthMap->getUniformLocation("lightSpaceMatrix");
	spDepthMap->setUniform(uniformLSM, LSM);
	spDepthMap->stopUsing();
	/////////////////////////////////////////////////////
}

static void updateCameraPositionInShaders() {
	spCylinder->use();
	spCylinder->setUniform(cameraPosLsp, cameraController->position);
	spCylinder->stopUsing();

	spShadows->use();
	spShadows->setUniform(cameraPosLspShadows, cameraController->position);
	spShadows->stopUsing();

	spLeaves->use();
	spLeaves->setUniform(cameraPosLspLeaves, cameraController->position);
	spLeaves->stopUsing();
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

static void setupTree(SceneGraph* sceneGraph) {

	float scaleLength = 0.5f;
	Morphospace::instance = new Morphospace(scaleLength);

	BranchNode* rootNode = new BranchNode();
	rootNode->relativePosition = {0.0f, 0.0f, 0.0f};
	rootNode->vigour = (float)GrowthParameters::instance->vRootMax;

	BranchModule* root = Morphospace::instance->selectModule(GrowthParameters::instance->apicalControl, GrowthParameters::instance->determinacy, rootNode);
	root->vigour = (float)GrowthParameters::instance->vRootMax; // The vigour in the root module of the tree is vRootMax

	points = new Mesh();
	points->setPrimitive(GL_LINES);
	points->setVerticesBufferType(GL_STREAM_DRAW);
	points->setVerticesBufferSize(10000);
	points->init();

	tree = new Tree(root);

	tree->sceneNode = sceneGraph->getRoot()->createChild(points, Mat4::IDENTITY, spCylinder);
	tree->sceneNode->addTexture(woodTexture);
	tree->sceneNode->addTexture(woodTextureNormalMap);
	tree->sceneNode->addTexture(depthMap);

	root->tree = tree;
}

static void setDefaultGrowthParameters() {
	GrowthParameters* growthParameters = new GrowthParameters();
	growthParameters->gP = 1.29f;
	growthParameters->scalingCoefficient = 100.0f;
	growthParameters->vRootMax = 300;
	growthParameters->thickeningFactor = 0.005f; // original 1.41
	growthParameters->vMin = 100.0f;
	growthParameters->vMax = 900;
	growthParameters->determinacy = 0.93f;
	growthParameters->apicalControl = 0.87f;
	growthParameters->g1 = 0.2f;
	growthParameters->g2 = 3.0f;
	growthParameters->leavesPerTip = 2;
	growthParameters->maxModules = 18;
	growthParameters->gravityDir = Vec3(0.0f, -1.0f, 0.0f);
	GrowthParameters::instance = growthParameters;
}

void TreeGrowth::start() {

	setDefaultGrowthParameters();

	treeGrowthUI = new TreeGrowthUI(getGui(), float(getWindowWidth()), float(getWindowHeight()), [&](GrowthParameters gp) {
		if (tree) {
			tree->sceneNode->deleteSceneNode();
			delete tree;
		}
		// When the user presses start we setup the tree again 
		GrowthParameters::instance = new GrowthParameters(gp);
		setupTree(getSceneGraph());
		paused = false;
	}, [&]() {
		paused = !paused;
	}, [&]() {
		GrowthParameters::instance = new GrowthParameters(treeGrowthUI->getGrowthParameters());
	});

	setSkyBox({
		"../Engine/textures/skybox/right.png",
		"../Engine/textures/skybox/left.png",
		"../Engine/textures/skybox/top.png",
		"../Engine/textures/skybox/bottom.png",
		"../Engine/textures/skybox/front.png",
		"../Engine/textures/skybox/back.png"
	});

	setupCamera(getCamera(), getWindow(), getWindowWidth(), getWindowHeight());
	setupShaders(getSceneGraph(), getCamera());
	setupTextures();
	setupLightAndShadows();

	// Setup leaves
	leaves = new Leaves(depthMap, spLeaves, spLeavesDepthMap);

	plane = Mesh::loadFromFile("../Engine/objs/cylinder32.obj");
	plane->paint(ColorRGBA::WHITE);
	sceneNodePlane = getSceneGraph()->getRoot()->createChild(plane, Mat4::IDENTITY, spShadows);
	sceneNodePlane->setModel(Mat4::scaling({100.0f, 0.1f, 100.0f}));
	sceneNodePlane->addTexture(planeTexture);
	sceneNodePlane->addTexture(depthMap);

	depthMap->setOnRenderToDepthMap(this, [&]() {
		// First we render to the depth map using the depth shaders
		for (SceneNode* leave : leaves->sceneNodes) {
			leave->setShaderProgram(spLeavesDepthMap);
		}
		if (tree) tree->sceneNode->setShaderProgram(spDepthMapCylinder);
		sceneNodePlane->setShaderProgram(spDepthMap);
	}, [&]() {

		// Then return to the regular shaders
		for (SceneNode* leave : leaves->sceneNodes) {
			leave->setShaderProgram(spLeaves);
		}
		if (tree) tree->sceneNode->setShaderProgram(spCylinder);
		sceneNodePlane->setShaderProgram(spShadows);
	});
}

bool hasLeaves = false;

void TreeGrowth::update() {

	if (!paused && tree) {
		tree->grow(float(getElapsedTime()));

		leaves->removeLeaves();
	}

	treeGrowthUI->updateFPSCounter(float(getElapsedTime()));
	
	updateCameraPositionInShaders();

	static bool isNReleased = false;
	if (glfwGetKey(getWindow(), GLFW_KEY_N) == GLFW_PRESS && isNReleased) {
		isNReleased = false;
		normalMapping = (normalMapping == GL_TRUE) ? GL_FALSE : GL_TRUE;
		spCylinder->use();
		spCylinder->setUniform(normalMappingL, normalMapping);
		spCylinder->stopUsing();
	}
	else if (glfwGetKey(getWindow(), GLFW_KEY_N) == GLFW_RELEASE) {
		isNReleased = true;
	}

	static bool isLReleased = false;
	if (glfwGetKey(getWindow(), GLFW_KEY_L) == GLFW_PRESS && isLReleased) {
		isLReleased = false;

		if (paused) {
			hasLeaves = !hasLeaves;

			if (hasLeaves)
				tree->root->generateLeaves(getSceneGraph(), leaves);
			else
				leaves->removeLeaves();
		}
	}
	else if (glfwGetKey(getWindow(), GLFW_KEY_L) == GLFW_RELEASE) {
		isLReleased = true;
	}

}

void TreeGrowth::end() {
	if (spCylinder != nullptr) delete spCylinder; 
	if (spDepthMapCylinder != nullptr)delete spDepthMapCylinder;
	if (spShadows != nullptr)delete spShadows;
	if (spDepthMap != nullptr)delete spDepthMap;
	if (spSimple != nullptr)delete spSimple;

	if (points != nullptr)delete points;
	if (plane != nullptr)delete plane;
	if (cameraController!= nullptr)delete cameraController;
	if (cylinder!= nullptr)delete cylinder;

	if (tree != nullptr)delete tree;

	if (woodTexture!= nullptr) 
		delete woodTexture;
	if (woodTextureNormalMap != nullptr) 
		delete woodTextureNormalMap;
	if (planeTexture != nullptr) 
		delete planeTexture;
	if (depthMap != nullptr) 
		delete depthMap;

	if (treeGrowthUI != nullptr)
		delete treeGrowthUI;

	if (sphere != nullptr)
		delete sphere;

	if (leaves != nullptr)
		delete leaves;
}