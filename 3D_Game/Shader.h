#pragma once
#include <GLAD/glad.h>

enum SHADER_TYPE {
	VERTEX = GL_VERTEX_SHADER,
	FRAGMENT = GL_FRAGMENT_SHADER
};

class Shader {
public:
	Shader(const char* sourceCode, SHADER_TYPE type);
	~Shader();
	bool Compile();

	friend class ShaderProgram;
private:
	GLuint m_ShaderId;
	const char* m_Source;
};