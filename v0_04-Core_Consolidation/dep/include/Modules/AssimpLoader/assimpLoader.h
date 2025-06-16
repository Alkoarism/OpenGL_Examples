#ifndef ASSIMPLOADER_H
#define ASSIMPLOADER_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "headers.h"

class AssimpLoader{
public:
    AssimpLoader(std::string path);
    ~AssimpLoader();

private:
    std::string m_directory;
    const aiScene* m_scene;

    void processNode(aiNode *node, const aiScene *scene);
};

#endif