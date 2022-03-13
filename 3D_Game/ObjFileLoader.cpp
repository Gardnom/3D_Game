#include "ObjFileLoader.h"

Mesh ObjFileLoader::LoadMesh(std::string& filePath) {
	//std::string fileContents = File::ReadFile(filePath);
	
	Mesh mesh;

	auto stream = File::OpenFile(filePath);
	std::string line;
	while (std::getline(stream, line)) {
		ObjLine objLine = ParseLine(line);
		if (objLine.identifier == ObjIdentifier::other) continue;
		switch (objLine.identifier) {
		case ObjIdentifier::v:
			Vertex vertex;
			vertex.position = glm::vec3(objLine.values[0], objLine.values[1], objLine.values[2]);
			m_Vertices.push_back(vertex);
			break;

		case ObjIdentifier::f:
			std::stringstream rowStream(line);
			std::string row;
			int i = 0;

			while (std::getline(rowStream, row, ' ')) {
				if (i > 3) break;

				if (i != 0) {
					ObjIndicies indicies = ParseIndexGroup(row);
					m_VertexIndicies.push_back(indicies.vertexIndex);
				}

				i++;
			}
			break;
		}
	}
	
	for (auto& vert : m_Vertices) {
		vert.colour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	
	mesh.m_Vertices = m_Vertices;
	mesh.m_Indicies = m_VertexIndicies;
	

	// Load Mtl file
	std::string filePathMtl = filePath.replace(filePath.find_first_of(".obj", 0), 4, ".mtl");
	auto streamMtl = File::OpenFile(filePathMtl);
	std::string line;
	
	Material mat;


	while (std::getline(stream, line)) {
		MtlLine mtlLine = ParseLineMtl(line);
		if (mtlLine.identifier == MtlPrefix::other) continue;
		switch (mtlLine.identifier) {

		case MtlPrefix::newmtl:
			// Check if mat is initialized;
			if (mat.m_Name.compare("")) {
				mesh.m_Materials.emplace(mat.m_Name, mat);
			}
			mat = Material(mtlLine.strValue);
			break;
		case MtlPrefix::kd:
			mat.m_Colour = glm::vec4(mtlLine.values[0], mtlLine.values[1], mtlLine.values[2], 1.0f);
			break;
		}
	}



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
			if (!valueStr.compare("kd")) {
				l.identifier = MtlPrefix::kd;
			}
			else if (!valueStr.compare("newmtl")) {
				l.identifier = MtlPrefix::newmtl;
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