#ifndef TEXT_FIELD_H
#define TEXT_FIELD_H

#include "LineRenderer.h"
#include "Text.h"
#include "Panel.h"
#include "GUIComponent.h"

class TextField : public GUIComponent {

private:
	Text* inputText;
	std::string input = "";
	Panel* outline;
	Panel* background;
	Line* cursor;
	Vec2 cursorPosition;
	bool isTextFieldSelected = false;
	bool reachedLimit = false;

	float height;
	float width;

	Vec2 position;

	int inputReceiverId;

	bool clicked();

public:

	TextField(Vec2 position, float width, float height);
	~TextField();

	void draw() override;

	void setInput(const std::string& input);
	std::string getInput() const;
};



#endif
