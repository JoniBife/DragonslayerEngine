#include "UIRenderer.h"
#include "Panel.h"


Panel::Panel(const Vec2& position, float width, float height) : position(position), width(width), height(height) {
	texture = new Texture2D("../Engine/textures/panels/defaultTexture.png");

	panelQuad = Mesh::rectangle(1.0f, 1.0f);
	panelQuad->init();
}
Panel::~Panel() {
	delete texture;
}

void Panel::setWidth(float width) {
	this->width = width;
}
void Panel::setHeight(float height) {
	this->height = height;
}
void Panel::setPosition(const Vec2& position) {
	this->position = position;
}

void Panel::setTexture(Texture2D* texture) {
	delete this->texture;
	this->texture = texture;
}

float Panel::getWidth() const { return width; }
float Panel::getHeight() const { return height; }
Vec2 Panel::getPosition() const { return position; }

bool Panel::isMouseWithinPanel() const {
	// Getting mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// The GLFW window anchor point is on the top left while our anchor point is on the bottom left
	// So we ajust the cursor position to match our coordinate system
	//Vec2 cursorPosition(float(xpos), float(ypos) - SCREEN_HEIGHT);

	int winWidth, winHeight;
	glfwGetFramebufferSize(window, &winWidth, &winHeight);

	Vec2 cursorPosition = Vec2(float(xpos), winHeight - float(ypos));

	//std::cout << "Cursor position: " << cursorPosition << std::endl;

	float panelTop = position.y + height / 2;
	float panelBottom = position.y - height / 2;
	float panelRight = position.x + width / 2;
	float panelLeft = position.x - width / 2;

	if (cursorPosition.x > panelLeft && cursorPosition.x < panelRight && cursorPosition.y > panelBottom && cursorPosition.y < panelTop) {
		return true;
	}

	return false;
}

void Panel::draw() {
	UIRenderer::getInstance()->renderUI(panelQuad, texture, position, { width, height });
}