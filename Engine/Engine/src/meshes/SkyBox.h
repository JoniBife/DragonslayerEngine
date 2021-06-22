#ifndef SKYBOX_H
#define SKYBOX_H

#include "IDrawable.h"
#include "Mesh.h"
#include "../textures/CubeMap.h"
#include "../shaders/ShaderProgram.h"
#include "../view/Camera.h"

class SkyBox : IDrawable {

private:
	Mesh* cube;
	CubeMap* cubemap;
	ShaderProgram* skyboxShader;
	GLint skyBoxLocation;

public:
	SkyBox(const std::vector<std::string>& facesFilePath, Camera* camera);
	~SkyBox();
	void init() override;
	void bind() override;
	void unBind() override;
	void draw() override;
};


#endif
