#include "GUI.h"
#include <imgui/imgui_internal.h>
#include <filesystem>

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
	//imGuiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//imGuiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    imGuiIO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    //imGuiIO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

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

static void showHierarchyPanel() {
	ImGui::Begin("Hierarchy");                        

	ImGui::Text("Cube");              

	ImGui::End();
}

static void showObjectPanel() {
	ImGui::Begin("Object");

	ImGui::End();
}

static void showSceneView() {
	ImGui::Begin("SceneView");


	ImGui::End();
}

static void showTempUI() {
	showMainMenuBar();
    showDockspace();
	showHierarchyPanel();
	showObjectPanel();
	showSceneView();
}

void GUI::drawUI()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	showTempUI();
	bool open = true;
	ImGui::ShowDemoWindow(&open);

	// Rendering
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	/*if (imGuiIO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}*/
}
