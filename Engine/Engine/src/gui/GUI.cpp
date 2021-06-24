#include "GUI.h"
#include <filesystem>

void GUI::loadFonts()
{
	imGuiIO.Fonts->AddFontDefault();

	// Loading all fonts in fonts dirrectory
	std::string fontsPath = "../Engine/fonts";
	
	for (const auto& file : std::filesystem::directory_iterator(fontsPath)) {
		if (file.path().extension().string().compare(".ttf") == 0) {
			imGuiIO.Fonts->AddFontFromFileTTF(file.path().string().c_str(), 16.0f);
		}
	}
}

GUI::GUI(ImGuiIO& imGuiIO, GLFWwindow* window) : imGuiIO(imGuiIO)
{
	//imGuiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//imGuiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	this->imGuiIO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	this->imGuiIO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

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

static void fileMenu() {
	if (ImGui::MenuItem("Open", "Ctrl + O")) {}
	if (ImGui::MenuItem("Save")) {}
	if (ImGui::MenuItem("Save as..")) {}
}

static void mainMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			fileMenu();
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

static void tempUI() {
	mainMenuBar();
}

void GUI::drawUI()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	//tempUI();
	bool open = true;
	ImGui::ShowDemoWindow(&open);

	// Rendering
	ImGui::Render();

	// Update and Render additional Platform Windows
	if (imGuiIO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
