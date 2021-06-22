#ifndef TEXT_H
#define TEXT_H

#include <vector>
#include <string>
#include <map>
#include "../math/Vec2.h"
#include "../math/Vec4.h"
#include "../shaders/ShaderProgram.h"
#include "FontLoader.h"
#include "GUIComponent.h"

class Text : public GUIComponent {

private:
	std::string content;
	Vec2 position; // The x and y components must be between 0 and 10, its easier to use this way, any values outside this values are clamped
	float size; // Must be between 0 and 100, any values outside this range are clamped
	Vec4 color;
	std::string font;
	std::vector<Character> characters;
	float width; // The width of the text within the UI
	float height; // The height of the text within the UI

public:
	Text(const std::string& content, const Vec2& position);
	Text(const std::string& content, const std::string& font, const Vec2& position, float size,const Vec4& color);

	void setFont(const std::string& font);
	void setColor(const Vec4& color);
	void setPosition(const Vec2& position);
	void setSize(float size);
	void setContent(const std::string& content, bool fontChanged = false);

	std::string getFont() const;
	Vec4 getColor() const;
	Vec2 getPosition() const;
	float getSize() const;
	std::string getContent() const;
	float getWidth() const;
	float getHeight() const;

	void draw() override;
};

#endif
