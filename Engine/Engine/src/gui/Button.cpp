#include "Button.h"
#include "../utils/OpenGLUtils.h"
#include "../Configurations.h"
#include "UIRenderer.h"
#include "../view/Transformations.h"
#include "../utils/ColorRGBA.h"

#define VERTICES 0

Button::Button(const Vec2& position) : 
	position(position),
	text(new Text("", ARIAL, position, 3.0f, ColorRGBA::BLACK)),
	height(30.0f), width(90.0f)
	{

	buttonMesh = Mesh::rectangle(1.0f, 1.0f);
	buttonMesh->init();

	defaultTexture = new Texture2D("../Engine/textures/buttons/defaultTexture.png");
	hoverTexture = new Texture2D("../Engine/textures/buttons/hoverTexture.png");
	onClickTexture = new Texture2D("../Engine/textures/buttons/onClickTexture.png");
}

Button::~Button() {
	delete defaultTexture;
	delete hoverTexture;
	delete onClickTexture;
}

void Button::draw() {

	bool isClicked = false;
	bool isHovered = isButtonHovered();

	Texture2D* buttonTexture = defaultTexture;

	// We only check for clicks if the button is hovered
	if (isHovered) {
		isClicked = isButtonPressed();	
		buttonTexture = isClicked ? onClickTexture : hoverTexture;
	}

	UIRenderer::getInstance()->renderUI(
		buttonMesh,
		buttonTexture,
		position,
		{ width, height }
	);
	text->draw();
}
bool Button::isButtonPressed() {

	static bool canRelease = false;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
		canRelease = true;
		return true;
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE && canRelease) {
		onClick(); // When the button is released we fire onClick
		canRelease = false;
		return false;
	}

	return false;
}
bool Button::isButtonHovered() {

	// Getting mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// The GLFW window anchor point is on the top left while our anchor point is on the bottom left
	// So we ajust the cursor position to match our coordinate system
	//Vec2 cursorPosition(float(xpos), float(ypos) - SCREEN_HEIGHT);

	int winWidth, winHeight;
	glfwGetFramebufferSize(window, &winWidth, &winHeight);

	Vec2 cursorPosition = Vec2(float(xpos),  winHeight -  float(ypos));

	//std::cout << "Cursor position: " << cursorPosition << std::endl;

	float buttonTop = position.y + height / 2;
	float buttonBottom = position.y - height / 2;
	float buttonRight = position.x + width / 2;
	float buttonLeft = position.x - width / 2;

	if (cursorPosition.x > buttonLeft && cursorPosition.x < buttonRight && cursorPosition.y > buttonBottom && cursorPosition.y < buttonTop) {
		return true;
	}

	return false;
}

//////////// SETTERS //////////////
void Button::setHeight(float height) {
	this->height = height;
}
void Button::setWidth(float width) {
	this->width = width;
}
void Button::setPosition(const Vec2& position) {
	this->position = position;
	text->setPosition(position);
}
void Button::setButtonText(const std::string& text, const Vec4& color, float size) {
	this->text->setContent(text);
	this->text->setColor(color);
	this->text->setSize(size);
}
void Button::setOnClickListener(const std::function<void()>& onClick) {
	this->onClick = onClick;
}
void Button::setDefaultTexture(Texture2D* defaultTexture) {
	this->defaultTexture = defaultTexture;
}
void Button::setHoverTexture(Texture2D* hoverTexture) {
	this->hoverTexture = hoverTexture;
}
void Button::setOnClickTexture(Texture2D* onClickTexture) {
	this->onClickTexture = onClickTexture;
}
