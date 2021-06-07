#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <string>

class Shader {
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath);

	void use();

	//utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4(const std::string& name, glm::mat4 value) const;

	void getInt(const std::string& name, int& value) const;
	void getFloat(const std::string& name, float& value) const;

private:
	void checkCompileErrors(unsigned int shader, std::string type);
};

#endif