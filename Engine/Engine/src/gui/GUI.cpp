#include "GUI.h"
#include <imgui/imgui_internal.h>
#include <filesystem>
#include <iostream>
#include "../core/Hierarchy.h"
#include "../view/Camera.h"

using namespace core;


void GUI::setDefaultTheme()
{
	// COLORS
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.48f, 0.16f, 0.16f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.98f, 0.26f, 0.26f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.98f, 0.26f, 0.26f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.48f, 0.16f, 0.16f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.98f, 0.75f, 0.26f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.98f, 0.75f, 0.26f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.88f, 0.63f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.98f, 0.26f, 0.26f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.98f, 0.06f, 0.06f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.98f, 0.26f, 0.26f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.98f, 0.26f, 0.26f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.75f, 0.10f, 0.10f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.75f, 0.10f, 0.10f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.98f, 0.26f, 0.26f, 0.20f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.98f, 0.26f, 0.26f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.98f, 0.26f, 0.26f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.58f, 0.18f, 0.18f, 0.86f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.98f, 0.26f, 0.26f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.68f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.07f, 0.07f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.42f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_DockingPreview] = ImVec4(0.98f, 0.75f, 0.26f, 1.00f);
	colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.98f, 0.26f, 0.26f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.98f, 0.26f, 0.26f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void GUI::setDefaultStyle()
{
	ImFont* font = imGuiIO.Fonts->Fonts[12];
	ImGui::PushID((void*)font);
	imGuiIO.FontDefault = font;
	ImGui::PopID();

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 0.3f;
	style.FrameRounding = 4.0f;
}

void GUI::loadFonts()
{
	imGuiIO.Fonts->AddFontDefault();

	// Loading all fonts in fonts dirrectory
	std::string fontsPath = "../Engine/fonts";
	
	for (const auto& file : std::filesystem::directory_iterator(fontsPath)) {
		if (file.path().extension().string().compare(".ttf") == 0) {
			imGuiIO.Fonts->AddFontFromFileTTF(file.path().string().c_str(), 17.0f);
		}
	}
}

GUI::GUI(ImGuiIO& imGuiIO, GLFWwindow* window) : imGuiIO(imGuiIO)
{
	imGuiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//imGuiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    imGuiIO.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable docking
    //imGuiIO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable multi-viewports

	// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();
	setDefaultTheme();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	loadFonts();
}

GUI::~GUI()
{
	// ImGUI cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

static void showFileMenu() {
	if (ImGui::MenuItem("Open", "Ctrl + o")) {}
	if (ImGui::MenuItem("Save")) {}
	if (ImGui::MenuItem("Save as..", "Ctrl + s")) {}
}

static void showMainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			showFileMenu();
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

static void showDockspace() {

    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
	/*ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
	ImGuiID dockspaceID = ImGui::GetID("DockSpace");
	if (!ImGui::DockBuilderGetNode(dockspaceID)) {
		ImGui::DockBuilderRemoveNode(dockspaceID);
		ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_None);

		ImGuiID dock_main_id = dockspaceID;
		ImGuiID dock_up_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.05f, nullptr, &dock_main_id);
		ImGuiID dock_right_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.2f, nullptr, &dock_main_id);
		ImGuiID dock_left_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.2f, nullptr, &dock_main_id);
		ImGuiID dock_down_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.2f, nullptr, &dock_main_id);
		ImGuiID dock_down_right_id = ImGui::DockBuilderSplitNode(dock_down_id, ImGuiDir_Right, 0.6f, nullptr, &dock_down_id);

		ImGui::DockBuilderDockWindow("Actions", dock_up_id);
		ImGui::DockBuilderDockWindow("Hierarchy", dock_right_id);
		ImGui::DockBuilderDockWindow("Inspector", dock_left_id);
		ImGui::DockBuilderDockWindow("Console", dock_down_id);
		ImGui::DockBuilderDockWindow("Project", dock_down_right_id);
		ImGui::DockBuilderDockWindow("Scene", dock_main_id);

		// Disable tab bar for custom toolbar
		ImGuiDockNode* node = ImGui::DockBuilderGetNode(dock_up_id);
		node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;

		ImGui::DockBuilderFinish(dock_main_id);
	}
	ImGui::DockSpace(dockspaceID, ImVec2(0.0f, 0.0f), dockspaceFlags);
	*/
}

static void showHierarchyRecurs(std::list<GameObject*> gameObjects);

static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
static void showHierarchyPanel() {
	ImGui::Begin("Hierarchy");   

	Hierarchy& hierarchy = Hierarchy::getHierarchy();

	auto rootGameObjects = hierarchy.getRootGameObjects();

	showHierarchyRecurs(rootGameObjects);

	if (ImGui::BeginPopupContextWindow())
	{
		if (ImGui::MenuItem("Create GameObject")) {
			hierarchy.createGameObject();
		}
		if (ImGui::MenuItem("Close")) {}
		ImGui::EndPopup();
	}
	
	ImGui::End();
}

static GameObject* selected = nullptr;

static void showHierarchyRecurs(std::list<GameObject*> gameObjects) {

	auto treeNodeFlags = base_flags;

	for (GameObject* gameObject : gameObjects) {

		if (gameObject->isSelected())
			treeNodeFlags |= ImGuiTreeNodeFlags_Selected;

		// If there it does not have children then use selectable
		if (gameObject->numberOfChildren() == 0) {

			treeNodeFlags |= ImGuiTreeNodeFlags_Leaf;
			bool open = ImGui::TreeNodeEx(gameObject->getName().c_str(), treeNodeFlags);

			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Delete GameObject")) {
				
				}
				if (ImGui::MenuItem("Close")) {}
				ImGui::EndPopup();
			}

			if (ImGui::IsItemClicked()) {
				gameObject->select();
				if (selected != nullptr) {
					selected->unselect();
				}
				selected = gameObject;
			}

			if (open)
			ImGui::TreePop();
		}
		else {

			bool open = ImGui::TreeNodeEx(gameObject->getName().c_str(), treeNodeFlags);
			
			if (ImGui::IsItemClicked()) {
				gameObject->select();
				if (selected != nullptr) {
					selected->unselect();
				}
				selected = gameObject;
			}

			if (open) {
				showHierarchyRecurs(gameObject->getChildren());
				ImGui::TreePop();
			}

			
		}

		treeNodeFlags = base_flags;
	}
}

static void showObjectPanel() {
	ImGui::Begin("Object");

	if (selected != nullptr) {
		ImGui::Text(selected->getName().c_str());

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;

		if (ImGui::CollapsingHeader("Transform", flags))
		{
			selected->getTransform()->onGUI();
		}
		ImGui::Dummy(ImVec2(0.0f, 2.5f));


		for (Component* component : selected->getAttachedComponents()) {
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;

			if (ImGui::CollapsingHeader(component->getName().c_str(), flags))
			{
				component->onGUI();
			}
			ImGui::Dummy(ImVec2(0.0f, 2.5f));
		}
	}

	ImGui::End();
}

static void showSceneView(GLuint id, Camera& camera) {
	ImGui::Begin("SceneView");
	{
		ImGui::BeginChild("SceneRenderer");
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		camera.setViewportSize(viewportPanelSize.x, viewportPanelSize.y);

		ImVec2 wsize = ImGui::GetWindowSize();

		ImGui::Image((ImTextureID)id, wsize, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::EndChild();
	}
	ImGui::End();
}

static void showEditorCameraPanel(Camera& camera) {
	ImGui::Begin("Editor camera configs");
	{
		camera.OnGUI();
	}
	ImGui::End();
}

static void showTempUI(GLuint id, Camera& camera) {
	showMainMenuBar();
    showDockspace();
	showHierarchyPanel();
	showObjectPanel();
	showSceneView(id, camera);
	showEditorCameraPanel(camera);
}

void GUI::drawUI(GLuint id, Camera& camera)
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	setDefaultStyle();

	showTempUI(id, camera);
	bool open = true;
	ImGui::ShowDemoWindow(&open);

	// Rendering
	ImGui::Render();

	//ImGui::PopFont();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	

	// Update and Render additional Platform Windows
	/*if (imGuiIO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}*/
}
