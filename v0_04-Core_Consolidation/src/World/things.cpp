#include "World/things.h"

Shader& Things::LoadShader
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

Texture& Things::LoadTexture
    (std::string name, const char* file, bool flipImage) {

    textures.emplace(name, Texture());
    ImgLoader img(file);
    
    if (img.GetLog() == nullptr) {
        switch(img.GetChannels()){
            case 3: textures.at(name).format = GL_RGB; break;
            case 4: textures.at(name).format = GL_RGBA; break;
            default: textures.at(name).format = GL_RGB; break;
        }
        textures.at(name).Load(img.GetData(), img.GetWidth(), img.GetHeight());
    }
    else {
        std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD_TEXTURE: "
            << name << "\n" 
            << "->ERROR_CODE: " << img.GetLog() << std::endl;
    }
    return textures.at(name);
}

Texture& Things::LoadTexture(std::string name) {
    textures.emplace(name, Texture());
    return textures.at(name);
}

Shader& Things::GetShader(const std::string name) {
    try {
        return shaders.at(name);
    }
    catch (std::exception e) {
        std::cout << "ERROR::RENDERER::SHADER::FILE_NAME_NOT_FOUND: "
            << name << std::endl;
    }
}

Texture& Things::GetTexture(const std::string name) {
    try {
        return textures.at(name);
    }
    catch (std::exception e) {
        std::cout << "ERROR::RENDERER::TEXTURE::FILE_NAME_NOT_FOUND: "
            << name << std::endl;
    }
}

std::map<std::string, Shader> Things::shaders;
std::map<std::string, Texture> Things::textures;