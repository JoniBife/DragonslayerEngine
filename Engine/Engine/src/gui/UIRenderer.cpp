#include "UIRenderer.h"
#include "../view/Transformations.h"
#include "../Configurations.h"
#include "../utils/OpenGLUtils.h"

UIRenderer* UIRenderer::instance = nullptr;

UIRenderer::UIRenderer() {
	// Loading the text shaders
	Shader vs(GL_VERTEX_SHADER, "../Engine/shaders/uiVertexShader.glsl");
	Shader fs(GL_FRAGMENT_SHADER, "../Engine/shaders/uiFragmentShader.glsl");
	uiShader = new ShaderProgram(vs, fs);
	uiShader->use();
	{
		modelLocation = uiShader->getUniformLocation("model");
		projectionLocation = uiShader->getUniformLocation("projection");
		uiTextureLocation = uiShader->getUniformLocation("text");

		uiShader->setUniform(uiTextureLocation, 0);
	}
	uiShader->stopUsing();
	// Creating a special orthographic projection matrix that does not have the near and far planes
	projection = ortho(0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT);
}

UIRenderer::~UIRenderer() {
	delete uiShader;
}

UIRenderer* UIRenderer::getInstance() {
	if (instance)
		return instance;
	instance = new UIRenderer();
	return instance;
}

void UIRenderer::destroyInstance() {
	if (instance)
		delete instance;
}

void UIRenderer::renderUI(Mesh* mesh, Texture2D* buttonTexture, const Vec2& position, const Vec2& scale) {
	uiShader->use();
	uiShader->setUniform(modelLocation, Mat4::translation({ position.x, position.y, 0.0f }) * Mat4::scaling({scale.x, scale.y, 0.0f}));
	uiShader->setUniform(projectionLocation, projection);

	mesh->bind();

	// We need to enable blending for the characters to be drawn transparent
	GL_CALL(glEnable(GL_BLEND));
	GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	buttonTexture->bind(GL_TEXTURE0);

	mesh->draw();
	
	buttonTexture->unBind(GL_TEXTURE0);

	GL_CALL(glDisable(GL_BLEND));

	mesh->unBind();

	uiShader->stopUsing();
}

void UIRenderer::updateProjection(const Mat4& projection) {
	this->projection = projection;
}