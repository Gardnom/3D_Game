#pragma once
#include "core.h"
#include <vector>
#include "Shader.h"
#include <unordered_map>

class ShaderProgram {
public:
	ShaderProgram();
	~ShaderProgram();
	
	bool Link(std::vector<Shader*>const & shaders);
	void Bind();

	void UploadUniformMatrix4fv(const char* varName, glm::mat4 mat);
	void UploadUniformVec4fv(const char* varName, glm::vec4& vec);
	void UploadUniformVec3fv(const char* varName, glm::vec3& vec);

	GLuint m_ProgramId;

private:
	GLint GetUniformLocation(const char* varName);
	void ProbeUniforms();
	std::unordered_map<std::string, GLint> m_UniformCache;
};