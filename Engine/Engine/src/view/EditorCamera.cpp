#include "EditorCamera.h"
#include <utils/OpenGLUtils.h>
#include "../core/Input.h"
#include <math/MathAux.h>
#include <algorithm>

bool EditorCamera::freeMovement(float elapsedTime, Vec2 currMousePosition)
{
    bool performedAction = false;

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
        performedAction = true;
    }

    Vec2 mouseMovement = (currMousePosition - lastMousePosition);

    if (mouseMovement.magnitude() > 0.0f) {

        yaw += mouseMovement.x * elapsedTime * rotationSpeed;

        // Minus because the origin of the coordinates is at the top left corner
        pitch -= mouseMovement.y * elapsedTime * rotationSpeed;

        // Avoiding float overflow by returning to degrees after a full rotation
        yaw = fmodf(yaw, 360.0f);

        // Cannot move camera above or below origin
        pitch = std::clamp(pitch, -89.0f, 89.0f);

        front.x = cosf(degreesToRadians(yaw)) * cosf(degreesToRadians(pitch));
        front.y = sinf(degreesToRadians(pitch));
        front.z = sinf(degreesToRadians(yaw)) * cosf(degreesToRadians(pitch));
        front = front.normalize();

        performedAction = true;
    }

    return performedAction;
}

bool EditorCamera::drag(float elapsedTime, Vec2 currMousePosition)
{
    
    bool performedAction = false;

    if (startingDragPosition.x == 0.0f && startingDragPosition.y == 0.0f) {
        startingDragPosition = currMousePosition;
    }

    //Input::setCursorVisibility(false);

    Vec2 mouseMovement = (currMousePosition - lastMousePosition);
    mouseMovement.y *= -1;

    if (mouseMovement.magnitude() > 0.0f) {

        Vec3 cameraRight = cross(front, up).normalize();
        Vec3 cameraUp = cross(cameraRight, front).normalize();

        // TODO Improve the way the cursor follows the drag motion, currently
        // if the drag is too quick the cursor will not follow correctly

        position -= cameraRight * mouseMovement.x * elapsedTime * dragSpeed;
        position -= cameraUp * mouseMovement.y * elapsedTime * dragSpeed;
        performedAction = true;
    }

    return performedAction;
}

bool EditorCamera::zoom(float elapsedTime, float scroll)
{
    if (scroll != 0.0f) {

        position += front.normalize() * scroll * elapsedTime * zoomSpeed;
        /*fov -= scroll * elapsedTime * zoomSpeed;
        fov = std::clamp(fov, 0.001f, 179.0f);*/
    
        return true;
    }

    return false;
}

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

void EditorCamera::setEditorWindowFocus(bool editorWindowFocused)
{
    this->editorWindowFocused = editorWindowFocused;
}

void EditorCamera::setMovementSpeed(float movementSpeed)
{
    assert(movementSpeed > 0.0f && movementSpeed < 1000.0f);
    this->movementSpeed = movementSpeed;
}

void EditorCamera::setRotationSpeed(float rotationSpeed)
{
    assert(rotationSpeed > 0.0f && rotationSpeed < 180.0f);
    this->rotationSpeed = rotationSpeed;
}

void EditorCamera::setDragSpeed(float dragSpeed)
{
    assert(dragSpeed > 0.0f && dragSpeed < 50.0f);
    this->dragSpeed = dragSpeed;
}

void EditorCamera::setZoomSpeed(float zoomSpeed)
{
    assert(zoomSpeed > 0.0f && zoomSpeed < 1000.0f);
    this->zoomSpeed = zoomSpeed;
}

void EditorCamera::update(float elapsedTime)
{
    if (editorWindowFocused) {

        Vec2 currMousePosition = Input::getMousePosition();

        if (Input::isMouseButtonDown(MouseButtonCode::RIGHT)) {
            
            freeModeEnabled = true;
            dirty = freeMovement(elapsedTime, currMousePosition);

        } else if (Input::isMouseButtonDown(MouseButtonCode::MIDDLE)) {
            
            dragModeEnabled = true;
            dirty = drag(elapsedTime, currMousePosition);  

        } else if (float scroll = Input::getMouseScroll()) {
            
            dirty = zoom(elapsedTime, scroll);

        } else if (freeModeEnabled || dragModeEnabled) {

            Input::setCursorVisibility(true);

            if (dragModeEnabled) {
                Input::setMousePosition(currMousePosition);
                startingDragPosition = { 0,0 };
            }

            freeModeEnabled = false;
            dragModeEnabled = false;
        }

        lastMousePosition = currMousePosition;

    }

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

void EditorCamera::onGUI()
{
    /*float minMovementSpeed = 0.01f;
    float maxMovementSpeed = 1000.0f;
    float editedMovementSpeed = movementSpeed;
    ImGui::SliderScalar("Movement Speed", ImGuiDataType_Float, &editedMovementSpeed, &minMovementSpeed, &maxMovementSpeed);
    SWAP_IF_DIFFERENT(movementSpeed, editedMovementSpeed, dirty)

    float minRotationSpeed = 0.01f;
    float maxRotationSpeed = 180.0f;
    float editedRotationSpeed = rotationSpeed;
    ImGui::SliderScalar("Rotation Speed", ImGuiDataType_Float, &editedRotationSpeed, &minRotationSpeed, &maxRotationSpeed);
    SWAP_IF_DIFFERENT(rotationSpeed, editedRotationSpeed, dirty)

    float minDragSpeed = 0.01f;
    float maxDragSpeed = 50.0f;
    float editedDragSpeed = dragSpeed;
    ImGui::SliderScalar("Drag Speed", ImGuiDataType_Float, &editedDragSpeed, &minDragSpeed, &maxDragSpeed);
    SWAP_IF_DIFFERENT(dragSpeed, editedDragSpeed, dirty)

    float minZoomSpeed = 0.01f;
    float maxZoomSpeed = 1000.0f;
    float editedZoomSpeed = zoomSpeed;
    ImGui::SliderScalar("Zoom Speed", ImGuiDataType_Float, &editedZoomSpeed, &minZoomSpeed, &maxZoomSpeed);
    SWAP_IF_DIFFERENT(zoomSpeed, editedZoomSpeed, dirty)

    Camera::onGUI();*/
}

float EditorCamera::getMovementSpeed() const
{
    return movementSpeed;
}

float EditorCamera::getRotationSpeed() const
{
    return rotationSpeed;
}

float EditorCamera::getDragSpeed() const
{
    return dragSpeed;
}

float EditorCamera::getZoomSpeed() const
{
    return zoomSpeed;
}
