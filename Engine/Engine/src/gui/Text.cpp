#include "Text.h"
#include "../utils/ColorRGBA.h"
#include <iostream>
#include "TextRenderer.h"


Text::Text(const std::string& content, const Vec2& position) : Text(content, ARIAL, position, 15, ColorRGBA::BLACK) {}

Text::Text(const std::string& content, const std::string& font, const Vec2& position, float size, const Vec4& color): font(font), color(color)
{
	if (size < 0.0f || size > 100.0f) {
		std::cout << "Text GUI WARNING [ Text smaller than 0 or larger than 100 so it has been clamped ]." << std::endl;
	}

	// TODO Clamp
	this->size = size/10;
	this->position = position;

	setContent(content);
}

void Text::draw() {

	// We ajust the position so that the anchor point is the center of the text
	Vec2 ajustedPosition = position;
	ajustedPosition.x -= (width * size) / 2; // We multiply the length by the size because it has to be scaled if the size if different from 1
	ajustedPosition.y -= (height * size) / 2; // We multiply the height by the size because it has to be scaled if the size if different from 1

	TextRenderer::getInstance()->renderText(
		characters, font, ajustedPosition, size, color
	);
}

/////////////// SETTERS //////////////////
void Text::setFont(const std::string& font) {
	this->font = font;
	setContent(content, true);
}
void Text::setColor(const Vec4& color) {
	this->color = color;
}
void Text::setPosition(const Vec2& position) {
	// TODO Clamp
	this->position = position;
}
void Text::setSize(float size) {
	// TODO Clamp
	this->size = size/10;
}
void Text::setContent(const std::string& content, bool fontChanged) {

	if (this->content == content && !fontChanged)
		return;

	auto fontMap = FontLoader::getInstance()->loadFont(font);

	// The position of the current character being drawn onto the screen
	Vec2 currPosition = { 0.0f, 0.0f };

	float maxHeight = 0;

	// Remove all old characters to add new ones
	characters.clear();

	std::string::const_iterator c;
	for (c = content.begin(); c != content.end(); c++)
	{
		Character character = fontMap->at(*c);

		float xpos = currPosition.x + character.bearing.x;
		float ypos = currPosition.y - (character.height - character.bearing.y);

		float w = character.width;
		float h = character.height;

		if (h > maxHeight)
			maxHeight = h;

		// Different characters have quads of different sizes so we ajust the vertices for each character
		character.vertices[0] = { xpos,     ypos + h,   0.0f, 0.0f };
		character.vertices[1] = { xpos,     ypos,       0.0f, 1.0f };
		character.vertices[2] = { xpos + w, ypos,       1.0f, 1.0f };
		character.vertices[3] = { xpos,     ypos + h,   0.0f, 0.0f };
		character.vertices[4] = { xpos + w, ypos,       1.0f, 1.0f };
		character.vertices[5] = { xpos + w, ypos + h,   1.0f, 0.0f };

		characters.push_back(character);

		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		currPosition.x += (character.advanceX >> 6); // bitshift by 6 to get value in pixels (2^6 = 64)
	}

	width = currPosition.x;
	height = maxHeight;

	this->content = content;
}

/////////////// GETTERS //////////////////
std::string Text::getFont() const {
	return font;
}
Vec4 Text::getColor() const {
	return color;
}
Vec2 Text::getPosition() const {
	return position;
}
float Text::getSize() const {
	return size;
}
std::string Text::getContent() const {
	return content;
}
float Text::getWidth() const {
	return width * size;
}
float Text::getHeight() const {
	return height * size;
}