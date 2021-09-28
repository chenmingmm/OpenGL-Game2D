#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <map>
#include <string>

#include <glad/glad.h>

#include "texture.h"
#include "../shaderManager.h"

class ResourceManager
{
public:
    ~ResourceManager();

    static std::map<std::string, Shader*> Shaders;
    static std::map<std::string, Texture*> Textures;

    static Shader *LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name);
    static Shader &GetShader(std::string name);

    static Texture *LoadTexture(const GLchar *file, GLboolean alpha, std::string name);
    static Texture &GetTexture(std::string name);

    static void Clear();

private:
    ResourceManager() { }
    // Loads and generates a shader from file
    static Shader *loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile = nullptr);
    // Loads a single texture from file
    static Texture *loadTextureFromFile(const GLchar *file, GLboolean alpha);
};



#endif