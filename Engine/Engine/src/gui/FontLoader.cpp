#include "FontLoader.h"
#include "GL/glew.h"
#include "../utils/OpenGLUtils.h"

FontLoader* FontLoader::instance = nullptr;

FontLoader::FontLoader() {
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "FREETYPE ERROR [ Could not init FreeType Library ]." << std::endl;
		exit(EXIT_FAILURE);
	}
}

FontLoader::~FontLoader() {
	FT_Done_FreeType(ft); // We first free the library as mentioned in the docs
	for (std::map<std::string, std::map<char, Character>*>::iterator it = fontCache.begin(); it != fontCache.end(); ++it) {
        auto fontMap = it->second;
        auto characterIterator = fontMap->begin();

        for (; characterIterator != fontMap->end(); ++characterIterator) {
            delete characterIterator->second.texture; // We have to delete the textures
        }

		delete it->second; // We also have to delete the map entry itself
	}
}

FontLoader* FontLoader::getInstance() {
    if (instance)
        return instance;

    instance = new FontLoader();
    return instance;
}

void FontLoader::destroyInstance() {
    if (instance)
    delete instance;
}

std::map<char, Character>* FontLoader::loadFont(const std::string& name) {
	
	auto cacheEntry = fontCache.find(name);
	// Was there a cache hit? If there was we return the found font map
	if (cacheEntry != fontCache.end()) {
		return cacheEntry->second; // Obtaining the fontMap
	}

	// Otherwise we have to load the font from the file

    // Creating path to find the font
    std::string path = "../Engine/fonts/";
    path.append(name).append(".ttf");

    // Loading font from a TrueType file as an 8 - bit grayscale bitmap image for us that we can access via face->glyph->bitmap.
    FT_Face face;
    FT_Error error = FT_New_Face(ft, path.c_str(), 0, &face);
    if (error == FT_Err_Unknown_File_Format) {
        std::cout << "FREETYPE ERROR [ The font file from path: " << path << " was opened and read but its file format is unsupported ]." << std::endl;
        exit(EXIT_FAILURE);
    }
    else if (error) {
        std::cout << "FREETYPE ERROR [ Failed to load a font from path: " << path << " ]." << std::endl;
        exit(EXIT_FAILURE);
    }

    GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1)); // disable byte-alignment restriction from OpenGL which requires that textures all have a 4-byte alignment

    // set size to load glyphs as
    FT_Set_Pixel_Sizes(face, 0, 48);

    std::map<char, Character>* fontMap = new std::map<char, Character>();

    // We iterate over the first 128 characters because the others might not be supported by the c chars
    // and generate a texture that is saved in a map, so that it can be used later without
    // having to generate a texture again
    for (unsigned char c = 0; c < 128; c++)
    {
        // Loading character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "FREETYPE ERROR [ Failed to load Glyph ]." << std::endl;
            continue; //TODO Maybe we should stop here instead of continuing 
        }

        // Generating texture with the glyph
        GlyphTexture* glyphTexture = new GlyphTexture(face->glyph);
        
        // Storing the font character for later use when drawing the texture
        Character character = {
            glyphTexture,
            face->glyph->bitmap.rows,
            face->glyph->bitmap.width,
            Vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x, 
            face->glyph->advance.y
        };

        float xpos = character.bearing.x;
        float ypos = character.height - character.bearing.y;

        float w = character.width;
        float h = character.height;

        // Different characters have quads of different sizes so we ajust the vertices for each character
        character.vertices[0] = { xpos,     ypos + h,   0.0f, 0.0f };
        character.vertices[1] = { xpos,     ypos,       0.0f, 1.0f };
        character.vertices[2] = { xpos + w, ypos,       1.0f, 1.0f };
        character.vertices[3] = { xpos,     ypos + h,   0.0f, 0.0f };
        character.vertices[4] = { xpos + w, ypos,       1.0f, 1.0f };
        character.vertices[5] = { xpos + w, ypos + h,   1.0f, 0.0f };
       
        fontMap->insert(std::pair<char, Character>(c, character));
    }

    // Finally we cache the newly loaded and generated fontMap 
    fontCache.insert(std::pair<std::string, std::map<char, Character>*>(name, fontMap));

    FT_Done_Face(face); // We can't also forget to delete the font face as mentioned in docs

    return fontMap;
}