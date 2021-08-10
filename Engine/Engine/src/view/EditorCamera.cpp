#include "EditorCamera.h"
#include "../utils/OpenGLUtils.h"
#include "../core/Input.h"
#include "../math/MathAux.h"
#include <algorithm>

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

void EditorCamera::update(float elapsedTime)
{
    

    Vec2 currMousePosition = Input::getMousePosition();
    if (Input::isMouseButtonDown(MouseButtonCode::RIGHT)) {
       
        Input::setCursorVisibility(false);

        Vec3 positionUpdate = Vec3::ZERO;

        if (Input::isKeyDown(KeyCode::UP) || Input::isKeyDown(KeyCode::W)) {
            positionUpdate += front.normalize();
        }
        else if (Input::isKeyDown(KeyCode::DOWN) || Input::isKeyDown(KeyCode::S)) {
            positionUpdate -= front.normalize();
        }

        if (Input::isKeyDown(KeyCode::LEFT) || Input::isKeyDown(KeyCode::A)) {
            if (positionUpdate.sqrMagnitude() > 0) {
                positionUpdate += cross(up, front).normalize();
                positionUpdate = positionUpdate.normalize();
            }
            else {
                positionUpdate += cross(up, front).normalize();
            }
        }
        else if (Input::isKeyDown(KeyCode::RIGHT) || Input::isKeyDown(KeyCode::D)) {
            if (positionUpdate.sqrMagnitude() > 0) {
                positionUpdate += cross(front, up).normalize();
                positionUpdate = positionUpdate.normalize();
            }
            else {
                positionUpdate += cross(front, up).normalize();
            }
        }

        // Only update position if there was any change to positionUpdate
        if (positionUpdate.sqrMagnitude() > 0) {
            //target += positionUpdate * movementSpeed * elapsedTime;
            position += positionUpdate * movementSpeed * elapsedTime;
            dirty = true;
        }
        
        Vec2 mouseMovement = (currMousePosition - lastMousePosition);
        
        if (mouseMovement.magnitude() > 0.0f) {

            std::cout << mouseMovement << std::endl;

            yaw += mouseMovement.x * elapsedTime * rotationSpeed;

            // Minus because the origin of the coordinates is at the top left corner
            pitch -= mouseMovement.y * elapsedTime * rotationSpeed;

            // Cannot move camera above or below origin
            pitch = std::clamp(pitch, -89.0f, 89.0f);

            std::cout << "Yaw - " << yaw << std::endl;
            std::cout << "Pitch - " << pitch << std::endl;


            front.x = cosf(degreesToRadians(yaw)) * cosf(degreesToRadians(pitch));
            front.y = sinf(degreesToRadians(pitch));
            front.z = sinf(degreesToRadians(yaw)) * cosf(degreesToRadians(pitch));
            front = front.normalize();

            dirty = true;
        }
    } 
    else if (Input::isMouseButtonDown(MouseButtonCode::MIDDLE)) {

        // TODO Replace mouse at start position

        Input::setCursorVisibility(false);

        Vec2 mouseMovement = (currMousePosition - lastMousePosition);
        mouseMovement.y *= -1;

        if (mouseMovement.magnitude() > 0.0f) {
            position.x -= mouseMovement.x * elapsedTime * movementSpeed;
            position.y -= mouseMovement.y * elapsedTime * movementSpeed;
            dirty = true;
        }
    } 
    else {
        Input::setCursorVisibility(true);
    }

    lastMousePosition = currMousePosition;
    

    Camera::update(elapsedTime);
}

Texture2D& EditorCamera::getFrameTexture() const
{
    return *frameTexture;
}

FrameBuffer& EditorCamera::getFrameBuffer() const
{
    return *frameBuffer;
}
