#include "Gui.h"
#include "Timer.h"


Gui::Gui(std::shared_ptr<Renderer>& renderer) {
	//m_Renderer = renderer;
	//m_Modules.push_back(renderer->m_Gui);
	m_Modules.emplace_back(renderer->m_Gui);
}

void Gui::Begin() {
	// Begin ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

}

void Gui::Render() {
	ImGui::Begin("Timings");

	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Timings");

	ImGui::BeginChild("Scrolling");
	for (auto& t : Timer::GetTimings()) {
		ImGui::Text("%s: %.2f ms", t.first.c_str(), (float)t.second / 1000);
	}
	ImGui::EndChild();
	
	ImGui::End();

	//ImGui::Begin("Options");
	//ImGui::Checkbox
	
	for (auto& mod : m_Modules) {
		mod->Render();
	}

	// Actual render
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}