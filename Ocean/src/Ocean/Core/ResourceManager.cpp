#include "ResourceManager.hpp"

#include "Ocean/Renderer/Shader.hpp"
#include "Ocean/Renderer/Texture.hpp"
#include "Ocean/Renderer/Font.hpp"

// std
#include <iostream>

// libs
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Ocean {

    ResourceManager::~ResourceManager() {
        this->Clear();
    }



    Ref<Shrimp::Shader>& ResourceManager::LoadShader(cstring path, cstring name) {
        return (Instance().m_Shaders[name] = Instance().LoadShaderFile(path));
    }

    Ref<Shrimp::Shader>& ResourceManager::GetShader(cstring name) {
        return Instance().m_Shaders[name];
    }

    Ref<Shrimp::Texture2D>& ResourceManager::LoadTexture(cstring path, cstring name) {
        return (Instance().m_Textures[name] = Instance().LoadTextureFile(path));
    }

    Ref<Shrimp::Texture2D>& ResourceManager::GetTexture(cstring name) {
        return Instance().m_Textures[name];
    }

    Ref<Shrimp::Font>& ResourceManager::LoadFont(cstring path, cstring name) {
        return (Instance().m_Fonts[name] = Instance().LoadFontFile(path));
    }

    Ref<Shrimp::Font>& ResourceManager::GetFont(cstring name) {
        return Instance().m_Fonts[name];
    }

    void ResourceManager::Clear() {
        Instance().m_Shaders.clear();
        Instance().m_Textures.clear();
        Instance().m_Fonts.clear();
    }



    ResourceManager::ResourceManager() : m_Shaders(), m_Textures(), m_Fonts() {

    }

    ResourceManager& ResourceManager::Instance() {
        if (s_Instance != nullptr)
            return *s_Instance;

        return *(s_Instance = new ResourceManager);
    }

    enum ShaderType {
        VERTEX   = 0,
        FRAGMENT = 1,
        GEOMETRY = 2,
    };

    Ref<Shrimp::Shader> ResourceManager::LoadShaderFile(cstring path) {
        FILE* fp = fopen(path, "r");

        if (fp == NULL) {
            std::cerr << "Failed To Open File! (" << path << ")" << std::endl;
        }

        string vertexCode;
        string fragmentCode;
        string geometryCode;
        char buffer[256];
        ShaderType type;

        // Parse The Shader File Into Its Components
        while (std::fgets(buffer, 256, fp) != NULL) {
            if (buffer[0] == '~') {
                switch (buffer[1]) {
                    case 'v':
                        type = VERTEX;
                        break;
                    case 'f':
                        type = FRAGMENT;
                        break;
                    case 'g':
                        type = GEOMETRY;
                        break;
                }

                continue;
            }

            switch (type) {
                case VERTEX:
                    vertexCode.append(buffer);
                    break;
                case FRAGMENT:
                    fragmentCode.append(buffer);
                    break;
                case GEOMETRY:
                    geometryCode.append(buffer);
                    break;
            }
        }

        fclose(fp);

        if (geometryCode.empty())
            return Shrimp::Shader::Create(vertexCode.c_str(), fragmentCode.c_str());

        return Shrimp::Shader::Create(vertexCode.c_str(), fragmentCode.c_str(), geometryCode.c_str());
    }

    Ref<Shrimp::Texture2D> ResourceManager::LoadTextureFile(cstring path) {
        i32 width, height, channels;

        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = stbi_load(path, &width, &height, &channels, 0);

        Ref<Shrimp::Texture2D> texture = Shrimp::Texture2D::Create(width, height);
        texture->SetFormat(static_cast<Shrimp::TextureFormat>(channels));
        texture->SetData(data, width * height * channels);

        stbi_image_free(data);

        return texture;
    }

    Ref<Shrimp::Font> ResourceManager::LoadFontFile(cstring path) {
        
    }

}   // Ocean