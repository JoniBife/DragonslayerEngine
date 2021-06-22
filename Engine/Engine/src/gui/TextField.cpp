#include "TextField.h"
#include "../InputManager.h"

#define CURSOR_OFFSET 0.1f
#define CURSOR_SCALE 0.7f
#define CURSOR_WIDTH 1.0f

#define INPUT_OFFSET 3.0f
#define INPUT_SCALE 0.1f

#define OUTLINE_WIDTH 1.5f

TextField::TextField(Vec2 position, float width, float height) : width(width), height(height), position(position) {
	background = new Panel(position, width, height);
	background->setTexture(new Texture2D("../Engine/textures/panels/whiteTexture.png"));
	outline = new Panel(position, width + OUTLINE_WIDTH * 2.0f, height + OUTLINE_WIDTH * 2.0f);
	outline->setTexture(new Texture2D("../Engine/textures/panels/darkGreyTexture.png"));
	inputText = new Text("", ARIAL, position, height * 0.1f, ColorRGBA::BLACK);
	cursor = new Line(height * CURSOR_SCALE, CURSOR_WIDTH);
	cursorPosition = position;
	cursorPosition.x += - width / 2 + CURSOR_OFFSET + INPUT_OFFSET;
	cursor->init();
	inputReceiverId = InputManager::getInstance()->generateInputReceiverId();
}
TextField::~TextField() {
	delete background;
	delete inputText;
	delete cursor;
}

void TextField::draw() {

	background->setWindow(window);
	inputText->setWindow(window);

	InputManager* im = InputManager::getInstance();

	bool isClicked = clicked();
	bool withinTextField = background->isMouseWithinPanel();

	if (isClicked && withinTextField && !isTextFieldSelected) {
		std::string currTextFieldInput = input;
		if (currTextFieldInput.empty())
			im->startTextInput(inputReceiverId);
		else
			im->resumeTextInput(inputReceiverId,currTextFieldInput);

		isTextFieldSelected = true;
	}
	else if (isClicked && !withinTextField && isTextFieldSelected) {
		im->stopTextInput(inputReceiverId);
		isTextFieldSelected = false;
	}
	
	if (isTextFieldSelected)
	{
		std::string newInput = im->getCurrTextInput();

		// If the input from the inputManager is different from the current drawn text in the textfield then we update it
		if (newInput != input) {

			input = newInput; // We first update the current input with the new input

			if (reachedLimit && input.size() < inputText->getContent().size()) {
				reachedLimit = false;
			}

			setInput(input);
		}
	}

	outline->draw();
	background->draw();
	inputText->draw();

	// If Text field is selected so we draw the cursor
	if (isTextFieldSelected)
		LineRenderer::getInstance()->renderLine(cursor, cursorPosition);
}

void TextField::setInput(const std::string& input) {
	this->input = input;

	if (!reachedLimit) {
		inputText->setContent(input);

		if (inputText->getWidth() > (width - INPUT_OFFSET * 2)) {
			reachedLimit = true;
			std::string resizedContent = input;
			resizedContent.pop_back();
			inputText->setContent(resizedContent);
		}

		// We also have to ajust the text position so that it's close to the left border
		Vec2 textInputPosition = position;
		textInputPosition.x -= background->getWidth() / 2 - inputText->getWidth() / 2;
		textInputPosition.x += INPUT_OFFSET;

		inputText->setPosition(textInputPosition);

		// We also have to ajust the cursor
		cursorPosition = textInputPosition;
		cursorPosition.x += inputText->getWidth() / 2 + CURSOR_OFFSET;

		float cursorEndPosition = position.x + width / 2 - INPUT_OFFSET + CURSOR_OFFSET;
		if (cursorPosition.x > cursorEndPosition)
			cursorPosition.x = cursorEndPosition;
	}
}

std::string TextField::getInput() const {
	return input;
}

bool TextField::clicked() {
	static bool canRelease = false;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
		canRelease = true;
		return true;
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE && canRelease) {
		canRelease = false;
		return false;
	}

	return false;
}
