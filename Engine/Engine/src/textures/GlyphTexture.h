#ifndef GLYPH_TEXTURE_H
#define GLYPH_TEXTURE_H

#include "ITexture.h"
#include <ft2build.h>
#include FT_FREETYPE_H 

class GlyphTexture : public ITexture {

public:
	GlyphTexture(FT_GlyphSlot glyph);

	~GlyphTexture();

	void bind(unsigned int unitNumber) override;

	void unBind(unsigned int unitNumber) override;
};

#endif