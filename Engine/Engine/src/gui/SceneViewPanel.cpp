#include "SceneViewPanel.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stdio.h>

#include "../core/Input.h"
#include <math/Qtrn.h>
#include <math/MathAux.h>

SceneViewPanel::SceneViewPanel(EditorCamera& editorCamera, HierarchyPanel& hierarchyPanel) : editorCamera(editorCamera), hierarchyPanel(hierarchyPanel)
{
}

void SceneViewPanel::onGUI()
{
	//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("SceneView");
	{
		ImGui::BeginChild("SceneRenderer");

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		editorCamera.setViewportSize(viewportPanelSize.x, viewportPanelSize.y);

		bool isWindowFocused = ImGui::IsWindowFocused();

		editorCamera.setEditorWindowFocus(isWindowFocused);

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
			}
			else if (Input::isKeyDown(KeyCode::E)) {
				selectedOperation = ImGuizmo::OPERATION::SCALE;
			}
			else if (Input::isKeyDown(KeyCode::R)) {
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

				if (selectedOperation == ImGuizmo::TRANSLATE) {
					transform->position = { matrixTranslation[0], matrixTranslation[1], matrixTranslation[2] };
				}
				else if (selectedOperation == ImGuizmo::ROTATE) {
					transform->rotation = {
					degreesToRadians(matrixRotation[0])
					,degreesToRadians(matrixRotation[1]),
					degreesToRadians(matrixRotation[2]) };
				}
				else if (selectedOperation == ImGuizmo::SCALE) {
					transform->scale = { matrixScale[0], matrixScale[1], matrixScale[2] };
				}

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

			}
		}

		if (isWindowFocused) {
			ImDrawList* drawList = ImGui::GetWindowDrawList();

			ImVec2 vMin = ImGui::GetWindowContentRegionMin();
			ImVec2 vMax = ImGui::GetWindowContentRegionMax();

			vMin.x += ImGui::GetWindowPos().x;
			vMin.y += ImGui::GetWindowPos().y;
			vMax.x += ImGui::GetWindowPos().x;
			vMax.y += ImGui::GetWindowPos().y;
			
			drawList->AddRect(vMin, vMax, IM_COL32(255, 0, 255, 255), 2.0f);
		}

		ImGui::EndChild();
	}
	ImGui::End();
	//ImGui::PopStyleVar();
}
