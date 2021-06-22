#include "GlyphTexture.h"
#include "GL/glew.h"
#include "../utils/OpenGLUtils.h"

GlyphTexture::GlyphTexture(FT_GlyphSlot glyph) {
    GL_CALL(glGenTextures(1, &id));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, id));
    GL_CALL(glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        glyph->bitmap.width,
        glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        glyph->bitmap.buffer
    ));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
}

GlyphTexture::~GlyphTexture() {
    GL_CALL(glDeleteTextures(1, &id));
}

void GlyphTexture::bind(unsigned int unitNumber) {
    GL_CALL(glActiveTexture(unitNumber));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, id));
}

void GlyphTexture::unBind(unsigned int unitNumber) {
    GL_CALL(glActiveTexture(unitNumber));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}