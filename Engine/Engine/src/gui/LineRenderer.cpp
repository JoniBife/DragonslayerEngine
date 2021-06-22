#include "LineRenderer.h"
#include "../utils/OpenGLUtils.h"
#include <assert.h>
#include "../view/Transformations.h"
#include "../Configurations.h"

#define VERTICES 0

Line::Line(float length, float width, const Vec4& color) : length(length), width(width), color(color) {

}
Line::~Line() {
	if (!hasBeenInitialized)
		return;
	GL_CALL(glDeleteBuffers(1, &vboId));
	GL_CALL(glDeleteVertexArrays(1, &vaoId));
}

void Line::init() {

	if (hasBeenInitialized)
		return;

	GL_CALL(glGenVertexArrays(1, &vaoId));
	GL_CALL(glBindVertexArray(vaoId));
	{
		GL_CALL(glGenBuffers(1, &vboId));
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vboId));
		{
			// The line has length 1
			Vec4 vertices[2] = {
				{0.0f, 0.5f, 0.0f, 1.0f},
				{0.0f, -0.5f, 0.0f, 1.0f}
			};

			GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
			GL_CALL(glEnableVertexAttribArray(VERTICES));

			GL_CALL(glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vec4), 0));
		}
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
	GL_CALL(glBindVertexArray(0));

	hasBeenInitialized = true;
}
void Line::bind() {
	assert(hasBeenInitialized);
	GL_CALL(glBindVertexArray(vaoId));
	hasBeenBound = true;
}
void Line::unBind() {
	GL_CALL(glBindVertexArray(0));
	hasBeenBound = false;
}
void Line::draw() {
	assert(hasBeenInitialized && hasBeenBound);

	GL_CALL(glEnable(GL_LINE_SMOOTH));
	GL_CALL(glLineWidth(width));

	GL_CALL(glDrawArrays(GL_LINES, 0, 2));

	GL_CALL(glDisable(GL_LINE_SMOOTH));
}

LineRenderer* LineRenderer::instance = nullptr;

LineRenderer::LineRenderer() {

	Shader vs(GL_VERTEX_SHADER, "../Engine/shaders/lineVertexShader.glsl");
	Shader fs(GL_FRAGMENT_SHADER, "../Engine/shaders/lineFragmentShader.glsl");
	lineShader = new ShaderProgram(vs, fs);

	lineShader->use();
	colorLocation = lineShader->getUniformLocation("color");
	modelLocation = lineShader->getUniformLocation("model");
	projectionLocation = lineShader->getUniformLocation("projection");
	lineShader->stopUsing();

	projection = ortho(0.0f, SCREEN_WIDTH, 0.0f, SCREEN_HEIGHT);
}
LineRenderer::~LineRenderer() {
	delete lineShader;
}

LineRenderer* LineRenderer::getInstance() {
	if (instance)
		return instance;
	instance = new LineRenderer();
	return instance;
}

void LineRenderer::destroyInstance() {
	if (instance)
		delete instance;
}

void LineRenderer::renderLine(Line* line, const Vec2& position) {
	lineShader->use();
	lineShader->setUniform(projectionLocation, projection);
	lineShader->setUniform(modelLocation, Mat4::translation({ position.x, position.y, 0.0f }) * Mat4::scaling({0.0f, line->length, 0.0f}));
	lineShader->setUniform(colorLocation, line->color);

	line->bind();
	line->draw();
	line->bind();

	lineShader->stopUsing();
}

void LineRenderer::updateProjection(const Mat4& projection) {
	this->projection = projection;
}

