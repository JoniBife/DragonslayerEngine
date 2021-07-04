#include "EditorCamera.h"
#include "../utils/OpenGLUtils.h"

EditorCamera::EditorCamera() : Camera()
{
    frameTexture = Texture2D::emptyTexture(getViewportWidth(), getViewportHeight());
    frameBuffer = new FrameBuffer([&]() {

        // Bind frameBuffer

        frameTexture->bind(GL_TEXTURE0);
        GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameTexture->getId(), 0));
        frameTexture->unBind(GL_TEXTURE0);

        GL_CALL(glGenRenderbuffers(1, &rbo));
        GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, rbo));
        GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, getViewportWidth(), getViewportHeight()));
        GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));

        GL_CALL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo));

        // Unbind frameBuffer
        });

    // TODO Consider resizing the the texture and renderbuffer sizes when window changes
    // Currently not working properly

    /*else if (camera.getViewportWidth() != frameTexture->getWidth() ||
        camera.getViewportHeight() != frameTexture->getHeight()) {

        // The camera width and height were changed so we resize both the texture
        // and framebuffer

        frameTexture->resize(camera.getViewportWidth(), camera.getViewportHeight());

        GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, rbo));
        GL_CALL(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, camera.getViewportWidth(), camera.getViewportHeight()));
        GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));
    }*/
}

EditorCamera::~EditorCamera()
{
    delete frameBuffer;
    delete frameTexture;
}

Texture2D& EditorCamera::getFrameTexture() const
{
    return *frameTexture;
}

FrameBuffer& EditorCamera::getFrameBuffer() const
{
    return *frameBuffer;
}
