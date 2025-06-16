#include "Modules/AssimpLoader/assimpLoader.h"

AssimpLoader::AssimpLoader(std::string path){
    Assimp::Importer import;
    
    m_scene = import.ReadFile(path, 
        aiProcess_Triangulate | 
        aiProcess_FlipUVs);

    if(m_scene == nullptr || m_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_scene->mRootNode){
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
    }
    
    m_directory = path.substr(0, path.find_last_of("/"));
}

AssimpLoader::~AssimpLoader(){

}