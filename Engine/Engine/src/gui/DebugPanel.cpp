#include "DebugPanel.h"
#include <imgui/imgui.h>

DebugPanel::DebugPanel(WarriorRenderer::Renderer* renderer) : renderer(renderer)
{

}

void DebugPanel::onGUI()
{
	ImGui::Begin("Renderer metrics");

	std::string geometryRenderPass = "Geometry pass: " + std::to_string(renderer->getFrameTime(WarriorRenderer::RenderPass::GEOMETRY)) + " ms";
	ImGui::Text(geometryRenderPass.c_str());

	std::string shadowRenderPass = "Shadow pass: " + std::to_string(renderer->getFrameTime(WarriorRenderer::RenderPass::SHADOW)) + " ms";
	ImGui::Text(shadowRenderPass.c_str());

	std::string lightRenderPass = "Light pass: " + std::to_string(renderer->getFrameTime(WarriorRenderer::RenderPass::LIGHT)) + " ms";
	ImGui::Text(lightRenderPass.c_str());

	std::string skyboxRenderPass = "Skybox pass: " + std::to_string(renderer->getFrameTime(WarriorRenderer::RenderPass::SKYBOX)) + " ms";
	ImGui::Text(skyboxRenderPass.c_str());

	std::string postProcessingRenderPass = "Postprocessing passes: " + std::to_string(renderer->getFrameTime(WarriorRenderer::RenderPass::POSTPROCESSING)) + " ms";
	ImGui::Text(postProcessingRenderPass.c_str());

	ImGui::End();
}
