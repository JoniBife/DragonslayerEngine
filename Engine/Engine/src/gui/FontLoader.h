#ifndef FONT_LOADER_H
#define FONT_LOADER_H

#include <map>
#include "../math/Vec2.h"
#include <ft2build.h>
#include FT_FREETYPE_H 
#include "../textures/GlyphTexture.h"

// Unfortenately there is no enum with abstract methods in c++, that would ideal in this case
namespace Fonts {
	#define ARIAL "arial"
	#define TIMES_NEW_ROMAN "timesNewRoman"
};

struct Character {
	GlyphTexture* texture;  // Texture ID for the glyph texture
	unsigned int height;
	unsigned int width;
	Vec2 bearing; // Offset from baseline to left/top of glyph
	unsigned int advanceX; // Offset to advance to next glyph
	unsigned int advanceY; // Offset to advance to next glyph
	Vec4 vertices[6];
};

class FontLoader {

private:
	// A map that contains all the fonts that have been loaded, this way we don't reload them everytime
	std::map<std::string, std::map<char, Character>*> fontCache; 
	FT_Library ft;
	static FontLoader* instance; // Singleton

public:
	FontLoader(); // Loads the default font on constructor
	~FontLoader();

	static FontLoader* getInstance();

	static void destroyInstance();

	std::map<char, Character>* loadFont(const std::string& fontName);

};

#endif
