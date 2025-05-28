#include "OpenGL/renderer.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STBI_FAILURE_USERMSG
#define STBI_ONLYJPEG

void Renderer::Render
	(const VertexArray& va, const IndexBuffer& ib, const Shader& s) {
		va.Bind();
		ib.Bind();
        s.Use();

		glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, 0);
	}

void Renderer::RenderConfig
	(const float& r, const float& g,
	 const float& b, const float& a) {

	glClearColor(r, g, b, a);
    if (render3D)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    else
        glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::FrameTimeTracker() {
	const float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

Shader& Renderer::LoadShader
	(const std::string name, const char* vertPath, const char* fragPath) {
	
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vShaderFile.open(vertPath);
        fShaderFile.open(fragPath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_TO_LOAD_FILE" << std::endl;
    }

    shaders.emplace(name, Shader());
    shaders[name].Compile(vertexCode.c_str(), fragmentCode.c_str());
    return shaders[name];
}

Texture& Renderer::LoadTexture
    (std::string name, const char* file, bool alpha, bool flipImage) {

    textures.emplace(name, Texture());
    if (alpha) {
        textures.at(name).format = GL_RGBA;
    }

    if (flipImage)
        stbi_set_flip_vertically_on_load(flipImage);

    int imgWidth, imgHeight, nrChannels;
    unsigned char* data
        = stbi_load(file, &imgWidth, &imgHeight, &nrChannels, 0);
    if (data) {
        textures.at(name).Load(data, imgWidth, imgHeight);
    }
    else {
        const char* failLog = stbi_failure_reason();
        std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD_TEXTURE: "
            << name << "\n" << failLog << std::endl;
    }
    stbi_image_free(data);

    return textures.at(name);
}

Texture& Renderer::LoadTexture(std::string name) {
    textures.emplace(name, Texture());
    return textures.at(name);
}

Shader& Renderer::GetShader(const std::string name) {
    try {
        return shaders.at(name);
    }
    catch (std::exception e) {
        std::cout << "ERROR::RENDERER::SHADER::FILE_NAME_NOT_FOUND: "
            << name << std::endl;
    }
}

Texture& Renderer::GetTexture(const std::string name) {
    try {
        return textures.at(name);
    }
    catch (std::exception e) {
        std::cout << "ERROR::RENDERER::TEXTURE::FILE_NAME_NOT_FOUND: "
            << name << std::endl;
    }
}

bool Renderer::render3D = false;
float Renderer::lastFrame = 0.0f;
float Renderer::deltaTime = 0.0f;
std::map<std::string, Shader> Renderer::shaders;
std::map<std::string, Texture> Renderer::textures;
