#ifndef BUTTON_RENDERER_H
#define BUTTON_RENDERER_H

#include "GL/glew.h"
#include "../shaders/ShaderProgram.h"
#include "../meshes/Mesh.h"
#include "../textures/Texture2D.h"

/*
* This class is used to render generic 2D UI elements
* that only use a simple mesh and a texture and some transformations
*/
class UIRenderer {

private:
	ShaderProgram* uiShader;

	GLint uiTextureLocation;

	GLint modelLocation;
	GLint projectionLocation;

	Mat4 projection;

	static UIRenderer* instance;

	UIRenderer();
	~UIRenderer();

public:

	static UIRenderer* getInstance();

	static void destroyInstance();

	void renderUI(Mesh* mesh,
		Texture2D* texture,
		const Vec2& position,
		const Vec2& scale);

	void updateProjection(const Mat4& projection);
};

#endif
