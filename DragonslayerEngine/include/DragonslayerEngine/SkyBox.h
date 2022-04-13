#ifndef SKYBOX_H
#define SKYBOX_H

#include "DragonslayerEngine/IDrawable.h"
#include "DragonslayerEngine/Mesh.h"
#include "DragonslayerEngine/textures/CubeMap.h"
#include "DragonslayerEngine/shaders/ShaderProgram.h"
#include "DragonslayerEngine/Camera.h"

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
