#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <vector>
#include "../shaders/ShaderProgram.h"
#include "GL/glew.h"
#include "FontLoader.h"
#include "../math/Mat4.h"

class TextRenderer {

private:
	ShaderProgram* textShader;
	GLuint vaoId;
	GLuint vboId;
	
	GLint textLocation;
	GLint textColorLocation;
	GLint modelLocation;
	GLint projectionLocation;

	Mat4 projection; // Right now we are using orthografic with the screen size

	static TextRenderer* instance; // Singleton

	TextRenderer();
	~TextRenderer();

public:

	static TextRenderer* getInstance();

	static void destroyInstance();

	void renderText(const std::vector<Character> text, const std::string& font, const Vec2& position, float scale, const Vec4& color);

	void updateProjection(const Mat4& projection);
};

#endif
