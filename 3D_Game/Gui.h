#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <memory>
#include "Renderer.h"
#include "Shapes.h"
#include "File.h"
#include "ObjFileLoader.h"

#define ZeroMemory(Destination, Length) memset((Destination), 0, (Length))

#include "EntityComponentSystem.h"

class GuiModule {
public:
	std::string m_Name;
	
	GuiModule(std::string name) {
		m_Name = name;
	}

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
	RendererOptionsGui(): GuiModule("Renderer options") {
	}

	void MakeComponents() {
		ImGui::Checkbox("Display wireframe", &m_DisplayWireFrame);
		ImGui::DragFloat("Fov", &m_Fov, 0.1f, 10.0f, 100.0f);
	}

	friend class Renderer;

private:
	float m_Fov = 45.0f;
};

class CubeGui : public GuiModule {
public:
	//std::shared_ptr<std::vector<Cube>> m_Cubes;
	std::vector<std::shared_ptr<Cube>>* m_Cubes;

	CubeGui(): GuiModule("Cube options") {
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

class EntityCreatorGui : public GuiModule {
public:
	std::vector<std::shared_ptr<Entity>> m_Entites;
	char* m_FilePathBuf;
	size_t m_FilePathBufSize;
	bool m_ButtonPressed = false;
	std::string m_FilePath;

	EntityCreatorGui(): GuiModule("EntityCreator") {
		m_FilePathBufSize = 60;
		m_FilePathBuf = new char[m_FilePathBufSize];
		//ZeroMemory(m_FilePathBuf, 30);
		//memset(m_FilePathBuf, 0, m_FilePathBufSize);
		ZeroMemory(m_FilePathBuf, m_FilePathBufSize);
	}

	void MakeComponents() {
		ImGui::InputText("Obj file", m_FilePathBuf, m_FilePathBufSize);
		if (ImGui::Button("Browse")){
			m_FilePath = File::BrowseForFile("H:\\Blender");
			memcpy(m_FilePathBuf, m_FilePath.c_str(), m_FilePathBufSize);
		}
		if (ImGui::Button("Import") && strcmp(m_FilePathBuf, "") != 0) {
			ObjFileLoader objFileLoader;
			Mesh mesh = objFileLoader.LoadMesh(m_FilePath);
			Entity ent(mesh, glm::vec3(0.0f, 0.0f, 0.0f));
			Engine::ECS::Ecs::AddEntity(ent);
		}
	}

};

typedef struct EntityGuiWrapper {
	Engine::ECS::PEntity pEntity;
	bool selected;
};

class EntityListGui : public GuiModule {
	
public:

	EntityListGui(): GuiModule("Entites"){
		//OnEntityListUpdate();
	
	}

	std::vector<EntityGuiWrapper> m_Entites;
		
	void MakeComponents() {
		/*for (auto& pair : Engine::ECS::Ecs::m_Entities) {
			//ImGui::List
		}*/
		OnEntityListUpdate();
		if (ImGui::ListBoxHeader("Entities")) {
			for (auto& ent : m_Entites) {

				if (!ent.pEntity->m_Name.compare("")) {
					//	m_EntityNames.push_back("Unnamed");
					ImGui::Selectable(unnamed, &ent.selected);
				}
				else
					ImGui::Selectable(ent.pEntity->m_Name.c_str(), &ent.selected);
				//m_EntityNames.push_back(pair.second->m_Name);
			}
			ImGui::ListBoxFooter();
		}
		//ImGui::ListBox("Entities", &m_SelectedEntity, m_EntityNames, m_NumEntites);
	}

	~EntityListGui() {
	}
	
private:
	int m_SelectedEntity;
	const char* unnamed = "unnamed";
	size_t m_NumEntites;

	void OnEntityListUpdate() {
		m_NumEntites = Engine::ECS::Ecs::m_Entities.size();
		m_Entites.clear();
		m_Entites.reserve(m_NumEntites);

		for (auto& pair : Engine::ECS::Ecs::m_Entities) {
			m_Entites.push_back({ pair.second, false });
		}
	}


};