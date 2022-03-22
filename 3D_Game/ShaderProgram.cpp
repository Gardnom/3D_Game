#include "ShaderProgram.h"

ShaderProgram::ShaderProgram() {
	m_ProgramId = glCreateProgram();
}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(m_ProgramId);
}

void ShaderProgram::Bind() {
	glUseProgram(m_ProgramId);
}

bool ShaderProgram::Link(std::vector<Shader*>const& shaders) {
	for (auto& s : shaders) {
		glAttachShader(m_ProgramId, s->m_ShaderId);
	}
	glLinkProgram(m_ProgramId);

	// Check for errors
	GLint isLinked = 0;
	glGetProgramiv(m_ProgramId, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(m_ProgramId, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(m_ProgramId, maxLength, &maxLength, infoLog.data());

		glDeleteProgram(m_ProgramId);
		// Delete shaders
		for (auto& s : shaders) {
			s->~Shader();
		}
		printf("Failed to link program: \n%s", infoLog.data());
		return false;
	}

	// Should always detach shaders after linking
	for (auto& s : shaders) {
		glDetachShader(m_ProgramId, s->m_ShaderId);
		s->~Shader();
	}

	ProbeUniforms();

	return true;
}

void ShaderProgram::UploadUniformMatrix4fv(const char* varName, glm::mat4 mat) {
	GLint varLocation = GetUniformLocation(varName);
	glUniformMatrix4fv(varLocation, 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderProgram::UploadUniformVec3fv(const char* varName, glm::vec3& vec) {
	GLint varLocation = GetUniformLocation(varName);
	glUniform3fv(varLocation, 1, glm::value_ptr(vec));
}

void ShaderProgram::ProbeUniforms() {
	int numUniforms;
	glGetProgramiv(m_ProgramId, GL_ACTIVE_UNIFORMS, &numUniforms);

	int maxCharLength;
	glGetProgramiv(m_ProgramId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxCharLength);
	if (numUniforms > 0 && maxCharLength > 0) {
		char* charBuffer = (char*)malloc(sizeof(char) * maxCharLength);
		for (int i = 0; i < numUniforms; i++) {
			int length, size;
			GLenum dataType;
			glGetActiveUniform(m_ProgramId, i, maxCharLength, &length, &size, &dataType, charBuffer);
			GLint varLocation = glGetUniformLocation(m_ProgramId, charBuffer);
			m_UniformCache.emplace(charBuffer, varLocation);
		}
		free(charBuffer);
	}
}

GLint ShaderProgram::GetUniformLocation(const char* varName) {
	if (m_UniformCache.find(std::string(varName)) != m_UniformCache.end()) {
		return m_UniformCache[varName];
	}

	return 0;
	/*int numUniforms;
	glGetProgramiv(m_ProgramId, GL_ACTIVE_UNIFORMS, &numUniforms);

	int maxCharLength;
	glGetProgramiv(m_ProgramId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxCharLength);
	if (numUniforms > 0 && maxCharLength > 0) {
		char* charBuffer = (char*)malloc(sizeof(char) * maxCharLength);
		for (int i = 0; i < numUniforms; i++) {
			int length, size;
			GLenum dataType;
			glGetActiveUniform(m_ProgramId, i, maxCharLength, &length, &size, &dataType, charBuffer);
			GLint varLocation = glGetUniformLocation(m_ProgramId, charBuffer);
			if (strcmp(charBuffer, varName) == 0) {
				//printf("Uniform %s has location %d\n", charBuffer, varLocation);
				m_UniformCache.emplace(varName, varLocation);
				return varLocation;
			}
		}
		free(charBuffer);
	}*/
}