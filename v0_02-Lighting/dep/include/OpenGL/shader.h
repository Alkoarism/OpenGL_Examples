#ifndef SHADER_H
#define SHADER_H

#include "headers.h"

class Shader {
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath);

	void Use() const;

	template <typename T>
	bool SetUniform(const std::string&, const T&) const;
	template <typename T>
	bool GetUniform(const std::string&, T&) const;

private:
	//utility uniform functions- template classes to avoid nondefined types to compile
	template <typename T>
	void SetGLUniform(const std::string&, const T&) const {
		throw std::invalid_argument("ERROR::SHADER::INVALID_UNIFORM_TYPE");
	}
	void SetGLUniform(const std::string&, const bool&) const;
	void SetGLUniform(const std::string&, const int&) const;
	void SetGLUniform(const std::string&, const float&) const;
	void SetGLUniform(const std::string&, const glm::vec3&) const;
	void SetGLUniform(const std::string&, const glm::mat4&) const;

	template <typename T>
	void GetGLUniform(const std::string&, const T&) const {
		throw std::invalid_argument("ERROR::SHADER::UNIFORM_TYPE_NOT_FOUND");
	}
	void GetGLUniform(const std::string&, int&) const;
	void GetGLUniform(const std::string&, float&) const;

	void CheckCompileErrors(unsigned int shader, std::string type);
};

template <typename T>
bool Shader::SetUniform(const std::string& name, const T& value) const {
	this->Use();
	try {
		SetGLUniform(name, value);
		return true;
	}
	catch (std::invalid_argument err) {
		std::cout << "COULD_NOT_DEFINE_UNIFORM: \"" << name << "\"\n"
			<< err.what() << std::endl;
		return false;
	}
}

template <typename T>
bool Shader::GetUniform(const std::string& name, T& value) const {
	this->Use();
	try {
		GetGLUniform(name, value);
		return true;
	}
	catch (std::invalid_argument err) {
		std::cout << "COULD_NOT_FIND_UNIFORM: \"" << name << "\"\n"
			<< err.what() << std::endl;
		return false;
	}
}


#endif