#include "TextRenderer.h"
#include "../utils/OpenGLUtils.h"
#include "../math/Vec4.h"
#include "FontLoader.h"
#include "../view/Transformations.h"
#include "../Configurations.h"

#define VERTICES 0

TextRenderer* TextRenderer::instance = nullptr;

TextRenderer::TextRenderer() {

	//Creating the buffers
	GL_CALL(glGenVertexArrays(1, &vaoId));
	GL_CALL(glGenBuffers(1, &vboId));
	{
		GL_CALL(glBindVertexArray(vaoId));
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vboId));

		// Dynamic draw because every time we draw a text, we will be sending data to this buffer, maybe consider stream draw
		GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(Vec4) * 6, NULL, GL_DYNAMIC_DRAW));
		GL_CALL(glEnableVertexAttribArray(VERTICES));
		GL_CALL(glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vec4), 0));
	}
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GL_CALL(glBindVertexArray(0));

	// Loading the text shaders
	Shader vs(GL_VERTEX_SHADER, "../Engine/shaders/textVertexShader.glsl");
	Shader fs(GL_FRAGMENT_SHADER, "../Engine/shaders/textFragmentShader.glsl");
	textShader = new ShaderProgram(vs, fs);
	textShader->use();
	textLocation = textShader->getUniformLocation("text");
	textColorLocation = textShader->getUniformLocation("textColor");
	modelLocation = textShader->getUniformLocation("model");
	projectionLocation = textShader->getUniformLocation("projection");
	textShader->stopUsing();

	// Creating a special orthographic projection matrix that does not have the near and far planes
	projection = ortho(0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT);
}

TextRenderer::~TextRenderer() {
	// Bind the the vao so that we can disable the vertex attrib array
	GL_CALL(glBindVertexArray(vaoId));
	GL_CALL(glDisableVertexAttribArray(VERTICES));
	GL_CALL(glDeleteBuffers(1, &vboId));

	delete textShader;
}

TextRenderer* TextRenderer::getInstance() {
	if (instance)
		return instance;
	
	instance = new TextRenderer();
	return instance;
}

void TextRenderer::destroyInstance() {
	if (instance)
		delete instance;
}

void TextRenderer::renderText(const std::vector<Character> text, const std::string& font, const Vec2& position, float scale, const Vec4& color) {

	textShader->use();
	textShader->setUniform(textLocation, 0);
	textShader->setUniform(textColorLocation, color);
	textShader->setUniform(modelLocation, Mat4::translation({ position.x, position.y, 0.0f }) * Mat4::scaling(scale));
	textShader->setUniform(projectionLocation, projection);

	GL_CALL(glBindVertexArray(vaoId));

	// We need to enable blending for the characters to be drawn transparent
	GL_CALL(glEnable(GL_BLEND));
	GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	for (Character character : text)
	{
		// Render glyph texture over quad
		character.texture->bind(GL_TEXTURE0);

		// Update content of VBO memory
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vboId));
		GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(character.vertices), character.vertices));
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

		// Drawing quad
		GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 6));
	}

	GL_CALL(glDisable(GL_BLEND));

	GL_CALL(glBindVertexArray(0));

	textShader->stopUsing();
}

void TextRenderer::updateProjection(const Mat4& projection) {
	this->projection = projection;
}