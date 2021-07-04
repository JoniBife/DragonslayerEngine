#include "SceneViewPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stdio.h>

#include "../core/Input.h"
#include "../math/Qtrn.h"
#include "../math/MathAux.h"

SceneViewPanel::SceneViewPanel(EditorCamera& editorCamera, HierarchyPanel& hierarchyPanel) : editorCamera(editorCamera), hierarchyPanel(hierarchyPanel)
{
}

void SceneViewPanel::onGUI()
{
	ImGui::Begin("SceneView");
	{
		ImGui::BeginChild("SceneRenderer");
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		editorCamera.setViewportSize(viewportPanelSize.x, viewportPanelSize.y);

		ImVec2 wsize = ImGui::GetWindowSize();

		ImGui::Image((ImTextureID)editorCamera.getFrameTexture().getId(), wsize, ImVec2(0, 1), ImVec2(1, 0));

		GameObject* selected = hierarchyPanel.getSelectedGameObject();

		if (selected != nullptr) {

			Transform* transform = selected->getTransform();

			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			float viewMatrix[16];
			editorCamera.getView().toOpenGLFormat(viewMatrix);

			float projectionMatrix[16];
			editorCamera.getProjection().toOpenGLFormat(projectionMatrix);

			float modelMatrix[16];
			transform->getModelMatrix().toOpenGLFormat(modelMatrix);

			float identityMatrix[16];
			Mat4::IDENTITY.toOpenGLFormat(identityMatrix);

			/*ImGuizmo::DrawGrid(
				viewMatrix,
				projectionMatrix,
				identityMatrix, 100.f);*/

			
			// TODO Remove hardcoded keys
			if (Input::isKeyDown(KeyCode::W)) {
				selectedOperation = ImGuizmo::OPERATION::TRANSLATE;
			} else if (Input::isKeyDown(KeyCode::E)) {
				selectedOperation = ImGuizmo::OPERATION::SCALE;
			} else if (Input::isKeyDown(KeyCode::R)) {
				selectedOperation = ImGuizmo::OPERATION::ROTATE;
			}

			ImGuizmo::Manipulate(
				viewMatrix, 
				projectionMatrix, 
				selectedOperation,
				ImGuizmo::WORLD, 
				modelMatrix);

			if (ImGuizmo::IsUsing()) {
				float matrixTranslation[3], matrixRotation[3], matrixScale[3];
				ImGuizmo::DecomposeMatrixToComponents(modelMatrix, matrixTranslation, matrixRotation, matrixScale);

				transform->position = { matrixTranslation[0], matrixTranslation[1], matrixTranslation[2] };
				transform->scale = { matrixScale[0], matrixScale[1], matrixScale[2] };
				
				/*Qtrn currRotation =
					Qtrn(transform->rotation.z, Vec3::Z)
					* Qtrn(transform->rotation.y, Vec3::Y)
					* Qtrn(transform->rotation.x, Vec3::X);

				Qtrn rotationFromGizmo = 
					Qtrn(degreesToRadians(matrixRotation[2]), Vec3::Z)
					* Qtrn(degreesToRadians(matrixRotation[1]), Vec3::Y)
					* Qtrn(degreesToRadians(matrixRotation[0]), Vec3::X);

				Qtrn newRotation = rotationFromGizmo * currRotation;

				float x;
				float y;
				float z;

				newRotation.toAngleAxis(x, Vec3::X);
				newRotation.toAngleAxis(y, Vec3::Y);
				newRotation.toAngleAxis(z, Vec3::Z);*/

				transform->rotation = { 
					degreesToRadians(matrixRotation[0])
					,degreesToRadians(matrixRotation[1]),
					degreesToRadians(matrixRotation[2]) };
				
			}
		}

		ImGui::EndChild();
	}
	ImGui::End();
}
