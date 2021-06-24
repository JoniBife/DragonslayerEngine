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

void GUI::drawUI()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::ShowDemoWindow();

	// Rendering
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
