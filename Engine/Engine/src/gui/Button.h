#ifndef BUTTON_H
#define BUTTON_H

#include <functional>
#include "GL/glew.h"
#include "GUIComponent.h"
#include "../textures/Texture2D.h"
#include "../math/Vec2.h"
#include "../gui/Text.h"
#include <string>
#include "../shaders/ShaderProgram.h"
#include "../meshes/Mesh.h"

class Button : public GUIComponent {

private:
	float height;
	float width;
	Vec2 position;

	std::function<void()> onClick;

	Texture2D* defaultTexture;
	Texture2D* hoverTexture;
	Texture2D* onClickTexture;
	Text* text;

	Mesh* buttonMesh;

public:
	Button(const Vec2& position);
	~Button();

	void setHeight(float height);
	void setWidth(float width);
	void setPosition(const Vec2& position);
	void setButtonText(const std::string& text, const Vec4& color, float size);
	void setOnClickListener(const std::function<void()>& onClick);
	void setDefaultTexture(Texture2D* defaultTexture);
	void setHoverTexture(Texture2D* hoverTexture);
	void setOnClickTexture(Texture2D* onClickTexture);

	void draw() override;

private:
	bool isButtonPressed();
	bool isButtonHovered();
};

#endif
