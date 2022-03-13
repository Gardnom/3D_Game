#pragma once
#include "core.h"
#include <vector>
#include "File.h"
#include "Mesh.h"
#include "Material.h"

enum ObjIdentifier {
	v,
	vn,
	vt,
	f,
	other
};

enum MtlPrefix {
	kd,
	newmtl,
	other,
};

typedef struct ObjIndicies {
	int vertexIndex;
	int textureIndex;
	int normalIndex;
} ObjIndicies;

typedef struct ObjLine {
	ObjIdentifier identifier;
	std::vector<float> values;
} ObjLine;


typedef struct MtlLine {
	MtlPrefix identifier;
	std::vector<float> values;
	std::string strValue;
};


class ObjFileLoader {
public:
	std::vector<Vertex> m_Vertices;
	std::vector<glm::vec3> m_VertexNormals;
	std::vector<float> m_TextureCoordinates;

	std::vector<int> m_VertexIndicies;
	std::vector<int> m_NormalIndicies;
	std::vector<int> m_TextureIndicies;

	Mesh LoadMesh(std::string& filePath);
	
private:
	 ObjLine ParseLine(std::string& line);
	 MtlLine ParseLineMtl(std::string& line);
	 ObjIndicies ParseIndexGroup(std::string& str);

};