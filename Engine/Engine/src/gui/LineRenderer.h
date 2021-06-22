#ifndef LINE_RENDERER_H
#define LINE_RENDERER_H

#include "../shaders/ShaderProgram.h"
#include "../math/Vec2.h"
#include "GL/glew.h"
#include "../meshes/IDrawable.h"
#include "../utils/ColorRGBA.h"

class Line  : IDrawable {

private:
	GLuint vboId = GLuint(0);
	GLuint vaoId = GLuint(0);

	bool hasBeenInitialized = false;
	bool hasBeenBound = false;

public:
	float length;
	float width;
	Vec4 color;

	Line(float length = 1.0f, float width = 1.0f, const Vec4& color = ColorRGBA::BLACK);
	~Line();

	void init() override;
	void bind() override;
	void unBind() override;
	void draw() override;
};

class LineRenderer {

private:
	ShaderProgram* lineShader;

	GLint projectionLocation;
	GLint modelLocation;
	GLint colorLocation;

	Mat4 projection;

	static LineRenderer* instance;

	LineRenderer();
	~LineRenderer();

public:
	void renderLine(Line* line,const Vec2& position);

	static LineRenderer* getInstance();

	static void destroyInstance();

	void updateProjection(const Mat4& projection);

};






#endif
