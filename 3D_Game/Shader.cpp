#include "Shader.h"
#include <vector>

Shader::Shader(const char* sourceCode, SHADER_TYPE type) {
	m_ShaderId = glCreateShader(type);
	m_Source = sourceCode;

}

Shader::~Shader() {
	glDeleteShader(m_ShaderId);
}

bool Shader::Compile() {
	glShaderSource(m_ShaderId, 1, &m_Source, 0);
	glCompileShader(m_ShaderId);

	GLint isCompiled = 0;
	glGetShaderiv(m_ShaderId, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(m_ShaderId, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(m_ShaderId, maxLength, &maxLength, infoLog.data());
		
		glDeleteShader(m_ShaderId);
		printf("Failed to compile shader: \n%s", infoLog.data());
		return false;
	}
	
	return true;
}