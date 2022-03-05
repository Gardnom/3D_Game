#pragma once
#include "core.h"
#include "Constants.h"
#include <vector>
#include "Mesh.h"

typedef std::pair<std::vector<Vertex>, std::vector<int>> ShapeRenderInfo;

class Shape {
public:
	float xPos = 0.0f, yPos = 0.0f, zPos = 0.0f;
	float m_ObjectScale = 1.0f;
	glm::vec3 m_Offset;
	Mesh m_Mesh;

	Shape() {
		m_Offset = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	Shape(glm::vec3& offset) {
		m_Offset = offset;
	}
};

class Cube : public Shape {

public:
	
	Cube(glm::vec3& offset) : Shape(offset) {
		auto renderInfo = GetRenderInfo();
		m_Mesh.m_Vertices = renderInfo.first;
		m_Mesh.m_Indicies = renderInfo.second;
	}
	Cube(glm::vec3 offset) : Shape(offset) {
		auto renderInfo = GetRenderInfo();
		m_Mesh.m_Vertices = renderInfo.first;
		m_Mesh.m_Indicies = renderInfo.second;
	}
	
public:
	ShapeRenderInfo GetRenderInfo() {
		std::vector<Vertex> vertices = {
			// Top Left Front
			{glm::vec3(xPos - 0.5f , yPos + 0.5f , zPos + 0.5f), C_BLACK}, // 0
			// Top Right Front
			{glm::vec3(xPos + 0.5f , yPos + 0.5f , zPos + 0.5f), C_BLACK}, // 1
			// Bottom Right Front
			{glm::vec3(xPos + 0.5f , yPos - 0.5f , zPos + 0.5f), C_BLACK}, // 2
			// Bottom Left Front
			{glm::vec3(xPos - 0.5f , yPos - 0.5f , zPos + 0.5f), C_BLACK}, // 3

			// Top Left Behind
			{glm::vec3(xPos - 0.5f , yPos + 0.5f , zPos - 0.5f), C_BLACK}, // 4
			// Top Right Behind
			{glm::vec3(xPos + 0.5f , yPos + 0.5f , zPos - 0.5f), C_BLACK}, // 5
			// Bottom Right Behind
			{glm::vec3(xPos + 0.5f , yPos - 0.5f , zPos - 0.5f), C_BLACK}, // 6
			// Bottom Left Behind
			{glm::vec3(xPos - 0.5f , yPos - 0.5f , zPos - 0.5f), C_BLACK}, // 7
		};

		for (auto& vert : vertices)
			vert.position = vert.position * m_ObjectScale;

		std::vector<int> indicies = {
			// Face
			0, 2, 3,
			0, 1, 2,

			//Top
			4, 5, 0,
			5, 1, 0,

			// Left
			4, 0, 7,
			0, 3, 7,

			// Right
			1, 5, 6,
			1, 2, 6,

			// Back
			4, 5, 7,
			5, 6, 7,
		};

		return std::make_pair(vertices, indicies);
	}	
	

};

