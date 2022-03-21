#include "ObjFileLoader.h"
#include "MaterialSystem.h"

Mesh ObjFileLoader::LoadMesh(std::string& filePath) {
	//std::string fileContents = File::ReadFile(filePath);
	
	Mesh mesh;
	std::string line;

	// Load Mtl file first to get all materials
	// I could maybe just store all the names of the materials in the vertices and retrieve them after
	// But that would just take longer, right?

	// Load Mtl file
	int lastObjOccurance = filePath.find(".obj", 0);
	
	std::string filePathMtl = filePath;
	filePathMtl.replace(lastObjOccurance, 4, ".mtl");

	auto streamMtl = File::OpenFile(filePathMtl);

	Engine::Material mat;


	while (std::getline(streamMtl, line)) {
		MtlLine mtlLine = ParseLineMtl(line);
		//if (mtlLine.identifier == MtlPrefix::mtlother) continue;
		switch (mtlLine.identifier) {

		case MtlPrefix::newmtl:
		{
			// Check if mat is initialized;
			if (mat.m_Name.compare("")) {
				//mesh.m_Materials.emplace(mat.m_Name, mat);
				Engine::MaterialSystem::AddMaterial(mat);
			}
			mat = Engine::Material(mtlLine.strValue);
			break;
		}
		case MtlPrefix::kd:
		{
			mat.m_Colour = glm::vec4(mtlLine.values[0], mtlLine.values[1], mtlLine.values[2], 1.0f);
			break;
		}

		default:
			break;
		}

		
	}

	if (mat.m_Name.compare("")) {
		//mesh.m_Materials.emplace(mat.m_Name, mat);
		Engine::MaterialSystem::AddMaterial(mat);
	}

	
	// --------------------------- Parsing OBJ File ----------------------------------------
	auto stream = File::OpenFile(filePath);
	std::shared_ptr<Engine::Material> currentMaterial;

	SPVector<glm::vec3> normals = MakeSPVector<glm::vec3>();
	SPVector<glm::vec3> positions = MakeSPVector<glm::vec3>();

	while (std::getline(stream, line)) {
		ObjLine objLine = ParseLine(line);
		if (objLine.identifier == ObjIdentifier::other) continue;
		switch (objLine.identifier) {

		case ObjIdentifier::v:
		{
			Vertex vertex;
			glm::vec3 position = glm::vec3(objLine.values[0], objLine.values[1], objLine.values[2]);
			vertex.position = position;
			m_Vertices.push_back(vertex);
			positions->push_back(position);

			break;
		}

		case ObjIdentifier::usemtl:
		{
			size_t space = line.find("usemtl");
			std::string matName = line.substr(space + 7);
			Engine::PMaterial _mat = Engine::MaterialSystem::GetMaterial(static_cast<Engine::HMaterial>(matName));
			if (_mat == nullptr) {
				printf("Failed to retrieve material with name: %s\n", matName.c_str());
				break;
			}
			currentMaterial = _mat;

			break;
		}

		case ObjIdentifier::f:
		{
			std::stringstream rowStream(line);
			std::string row;
			int i = 0;

			while (std::getline(rowStream, row, ' ')) {
				if (i > 3) break;

				if (i != 0) {
					ObjIndicies indicies = ParseIndexGroup(row);
					// -1 Because start index is 1 for faces in obj files
					Vertex _vert = m_Vertices.at(indicies.vertexIndex - 1);
					if (currentMaterial != nullptr) {
						_vert.colour = currentMaterial->m_Colour;
					}
					else _vert.colour = C_BLACK;
					glm::vec3& normal = normals->at(indicies.normalIndex - 1);
					_vert.normal = normal;

					m_FaceVertices.push_back(_vert);
					m_FaceIndicies.push_back(m_FaceVertices.size() - 1);

					m_VertexIndicies.push_back(indicies.vertexIndex);
				}

				i++;
			}
			break; 
		}

		case ObjIdentifier::vn:
		{
			//glm::vec3 normal = glm::vec3(objLine.values[0], objLine.values[1], objLine.values[2]);
			normals->push_back(glm::vec3(objLine.values[0], objLine.values[1], objLine.values[2]));
			break;
		}
			
			
	
		}
	}
	
	for (auto& vert : m_Vertices) {
		//vert.colour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	
	//mesh.m_Vertices = m_Vertices;
	//mesh.m_Indicies = m_VertexIndicies;
	
	mesh.m_Vertices = m_FaceVertices;
	mesh.m_Indicies = m_FaceIndicies;
	
	


	return mesh;
}

ObjIndicies ObjFileLoader::ParseIndexGroup(std::string& str) {
	std::string valueStr;
	std::stringstream stream(str);
	ObjIndicies indicies;
	int* ptr = &indicies.vertexIndex;

	while (std::getline(stream, valueStr, '/')) {
		if (!valueStr.compare(""))
			*ptr = 0;
		else
			*ptr = std::stoi(valueStr);
		ptr++;
	}
	return indicies;
}

ObjLine ObjFileLoader::ParseLine(std::string& line) {
	ObjLine l;
	
	std::string valueStr;
	std::stringstream stream(line);
	int i = 0;

	while (std::getline(stream, valueStr, ' ')) {
		if (i == 0) {
			if (!valueStr.compare("v")) {
				l.identifier = ObjIdentifier::v;
			}
			else if (!valueStr.compare("vn")) {
				l.identifier = ObjIdentifier::vn;
			}
			else if (!valueStr.compare("vt")) {
				l.identifier = ObjIdentifier::vt;
			}
			else if (!valueStr.compare("f")) {
				l.identifier = ObjIdentifier::f;
			}
			else if (!valueStr.compare("usemtl")) {
				l.identifier = ObjIdentifier::usemtl;
				break;
			}
			else {
				l.identifier = ObjIdentifier::other;
				break;
			}
			i++;
			continue;
		}
		else {
			float value = std::stof(valueStr);
			l.values.push_back(value);
			i++;
		}
	}

	return l;
}

MtlLine ObjFileLoader::ParseLineMtl(std::string& line) {
	MtlLine l;
	std::string valueStr;
	std::stringstream stream(line);
	int i = 0;

	while (std::getline(stream, valueStr, ' ')) {
		if (i == 0) {
			if (!valueStr.compare("Kd")) {
				l.identifier = MtlPrefix::kd;
			}
			else if (!valueStr.compare("newmtl")) {
				l.identifier = MtlPrefix::newmtl;
			}
			else {
				l.identifier = MtlPrefix::mtlother;
				break;
			}
			
			i++;
			continue;
		}
		if (l.identifier == MtlPrefix::newmtl) {
			l.strValue = valueStr;
		}
		else {
			float value = std::stof(valueStr);
			l.values.push_back(value);
			i++;
		}
	}

	return l;
}