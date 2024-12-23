#include "ResourceManager.hpp"

#include "Ocean/Core/Types/SharedPtr.hpp"

#include "Ocean/Core/Primitives/File.hpp"

#include "Renderer/Shader.hpp"
#include "Renderer/Texture.hpp"
#include "Renderer/Font.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Ocean {

    ResourceManager::~ResourceManager() {
        this->Clear();
    }



    SharedPtr<Shrimp::Shader>& ResourceManager::LoadShader(cstring path, cstring name) {
        return (Instance().m_Shaders[name] = Instance().LoadShaderFile(path));
    }

    SharedPtr<Shrimp::Shader>& ResourceManager::GetShader(cstring name) {
        return Instance().m_Shaders[name];
    }

    SharedPtr<Shrimp::Texture2D>& ResourceManager::LoadTexture(cstring path, cstring name) {
        return (Instance().m_Textures[name] = Instance().LoadTextureFile(path));
    }

    SharedPtr<Shrimp::Texture2D>& ResourceManager::GetTexture(cstring name) {
        return Instance().m_Textures[name];
    }

    SharedPtr<Shrimp::Font>& ResourceManager::LoadFont(cstring path, cstring name) {
        return (Instance().m_Fonts[name] = Instance().LoadFontFile(path));
    }

    SharedPtr<Shrimp::Font>& ResourceManager::GetFont(cstring name) {
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

    SharedPtr<Shrimp::Shader> ResourceManager::LoadShaderFile(cstring path) {
        File in(path);

        return Shrimp::Shader::Create(in.GetToKey("~f", "~v").c_str(), in.GetToKey("~g", "~f").c_str(), in.GetToKey("~e", "~g").c_str());    
    }

    SharedPtr<Shrimp::Texture2D> ResourceManager::LoadTextureFile(cstring path) {
        i32 width, height, channels;

        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = stbi_load(path, &width, &height, &channels, 0);

        SharedPtr<Shrimp::Texture2D> texture = Shrimp::Texture2D::Create(width, height);
        texture->SetFormat(static_cast<Shrimp::TextureFormat>(channels));
        texture->SetData(data, width * height * channels);

        stbi_image_free(data);

        return texture;
    }

    SharedPtr<Shrimp::Font> ResourceManager::LoadFontFile(cstring path) {
        
    }

}   // Ocean