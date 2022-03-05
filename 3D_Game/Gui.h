#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <memory>
#include "Renderer.h"
#include "Shapes.h"

class GuiModule {
public:
	std::string m_Name;
	void Begin() {
		ImGui::Begin(m_Name.c_str());
	}
	void End() {
		ImGui::End();
	}
	virtual void MakeComponents() = 0;

	void Render() {
		Begin();
		MakeComponents();
		End();
	}

};

class Gui {
public:

	Gui(std::shared_ptr<Renderer>& renderer);

	void Begin();
	void Render();
	void AddModule(std::shared_ptr<GuiModule> mod) {
		m_Modules.emplace_back(mod);
	}

private:
	std::shared_ptr<Renderer> m_Renderer;
	std::vector<std::shared_ptr<GuiModule>> m_Modules;
};




class RendererOptionsGui : public GuiModule {
public:
	bool m_DisplayWireFrame = false;
	RendererOptionsGui() {
		m_Name = "Renderer options";
	}

	void MakeComponents() {
		ImGui::Checkbox("Display wireframe", &m_DisplayWireFrame);
	}
};

class CubeGui : public GuiModule {
public:
	//std::shared_ptr<std::vector<Cube>> m_Cubes;
	std::vector<std::shared_ptr<Cube>>* m_Cubes;

	CubeGui() {
		m_Name = "Cube options";
	}
	void MakeComponents() {
		uint16_t i = 1;
		for (auto& c : *m_Cubes) {
			ImGui::Text("Cube %d", i);
			ImGui::DragFloat(i + ". X", &c->m_Offset.x, 0.1f, -10.0f, 10.0f);
			ImGui::DragFloat(i + ". Y", &c->m_Offset.y, 0.1f, -10.0f, 10.0f);
			ImGui::DragFloat(i + ". Z", &c->m_Offset.z, 0.1f, -10.0f, 10.0f);
			i++;
		}
	}
};