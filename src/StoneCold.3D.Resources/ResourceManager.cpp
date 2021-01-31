
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x)

#include "ResourceManager.hpp"

using namespace StoneCold::Base;
using namespace StoneCold::Resources;

ResourceManager::ResourceManager()
: _resources(std::unordered_map<std::string, std::shared_ptr<Resource>>())
, _resouceLifetimes(std::unordered_map<ResourceLifeTime, std::vector<std::string>>()) {
#ifdef _WIN32
    // Get the current path to the running .exe
    // When NULL is passed to GetModuleHandle, the handle of the exe itself is returned
    char ownPth[MAX_PATH];
    HMODULE hModule = GetModuleHandle(NULL);
    if (hModule != NULL) {
        GetModuleFileName(hModule, ownPth, (sizeof(ownPth)));
    }
    // Create the relative path to the current assets folder
    _basePath = std::string(ownPth);
    _basePath = _basePath.substr(0, _basePath.find_last_of("\\") + 1);
    _basePath += "res\\";
#else
    _basePath = "/Users/peter/Documents/github/C++/StoneCold_3D/res/";
#endif
}


bool ResourceManager::Initialize() {
    _resouceLifetimes.insert({ ResourceLifeTime::Intro, std::vector<std::string>() });
    _resouceLifetimes.insert({ ResourceLifeTime::Game, std::vector<std::string>() });
    _resouceLifetimes.insert({ ResourceLifeTime::Menu, std::vector<std::string>() });
    _resouceLifetimes.insert({ ResourceLifeTime::Level, std::vector<std::string>() });
    return true;
}


template<typename T>
T* ResourceManager::LoadResource(ResourceLifeTime resourceLifeTime, const std::string& name) {
    try {
        // Load each ressource only once
        if (!IsResourceLoaded(name)) {
            if (std::is_same<T, MeshResource>::value) {
                // Create a simple Model Resource, loaded from a File (.obj, no Texture)
                _resources.insert({ name, LoadMeshResource(name) });
                _resouceLifetimes[resourceLifeTime].push_back(name);
            }
            else if (std::is_same<T, ModelResource>::value) {
                // Create a complex Model Resource, loaded from a File (.obj, textures, multiple meshes)
                _resources.insert({ name, LoadModelResource(name, resourceLifeTime) });
                _resouceLifetimes[resourceLifeTime].push_back(name);
            }
            else if (std::is_same<T, TextureResource>::value) {
                // Create a Texture Resource, loaded from a File (.png or .jpg)
                _resources.insert({ name, LoadTextureResource(name) });
                _resouceLifetimes[resourceLifeTime].push_back(name);
            }
        }
        return static_cast<T*>(_resources[name].get());
    }
    catch (std::exception ex) {
        throw GameException("Error on Resource creation: " + name + "\n" + ex.what());
    }
}


void ResourceManager::UnloadResources(ResourceLifeTime resourceLifeTime) {
    // Remove all Resources that are mapped to the specific lifetime
    const auto& keys = _resouceLifetimes[resourceLifeTime];
    for (const auto& key : keys) {
        _resources.erase(key);
    }
    
    // Clear all ResourceLifeTime keys
    _resouceLifetimes[resourceLifeTime] = std::vector<std::string>();
}


std::shared_ptr<MeshResource> ResourceManager::LoadMeshResource(const std::string& name) {
    auto fullPath = (_basePath + name);
    std::vector<Vertex> tmpVertices;
    std::vector<uint32> tmpIndices;
    
    // Parse the .obj file data
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(fullPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if (scene != nullptr) {
        // Get only the first mesh
        const aiMesh* mesh = scene->mMeshes[0];
        for (uint32 i = 0; i < mesh->mNumVertices; i++) {
            Vertex tmpvec;
            tmpvec.Position.x = mesh->mVertices[i].x;
            tmpvec.Position.y = mesh->mVertices[i].y;
            tmpvec.Position.z = mesh->mVertices[i].z;
            tmpvec.Normal.x = mesh->mNormals[i].x;
            tmpvec.Normal.y = mesh->mNormals[i].y;
            tmpvec.Normal.z = mesh->mNormals[i].z;
            // default values for tangents and texture coords
            tmpvec.Tangent = glm::vec3();
            tmpvec.BiTangent = glm::vec3();
            tmpvec.TextureCoords = glm::vec2();
            tmpVertices.push_back(tmpvec);
        }
        // Now retrieve the corresponding vertex indices from the meshs faces
        for (uint32 i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (uint32 j = 0; j < face.mNumIndices; j++) {
                tmpIndices.push_back(face.mIndices[j]);
            }
        }
    }
    
    return std::make_shared<MeshResource>(MeshResource(name, tmpVertices, tmpIndices, nullptr));
}


std::shared_ptr<ModelResource> ResourceManager::LoadModelResource(const std::string& name, ResourceLifeTime resourceLifeTime) {
    auto fullPath = (_basePath + name);
    Assimp::Importer importer;
    
    const aiScene* scene = importer.ReadFile(fullPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return nullptr;
    }
    
    // Retrieve the directory path of the filepath
#ifdef _WIN32
    const std::string directory = fullPath.substr(0, fullPath.find_last_of("\\"));
#else
    const std::string directory = fullPath.substr(0, fullPath.find_last_of("/"));
#endif
    std::vector<MeshResource> tmpMeshes;
    // Process ASSIMP nodes recursively
    ProcessAssimpNode(scene->mRootNode, scene, directory, tmpMeshes, resourceLifeTime);
    // Create the loaded Model
    return std::make_shared<ModelResource>(name, tmpMeshes);
}


std::shared_ptr<TextureResource> ResourceManager::LoadTextureResource(const std::string& name) {
    auto fullPath = _basePath + name;
    
    uint32 textureDiffuse = ProcessAssimpTexture(nullptr, fullPath, aiTextureType_DIFFUSE);
    return std::make_shared<TextureResource>(TextureResource(name, Material(), textureDiffuse, 0, 0));
}


void ResourceManager::ProcessAssimpNode(aiNode* node, const aiScene* scene, const std::string& dir, std::vector<MeshResource>& meshes, ResourceLifeTime resourceLifeTime) {
    // Process each mesh located at the current node
    for (uint32 i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(this->ProcessAssimpMesh(mesh, scene, dir, resourceLifeTime));
    }
    // After processing the meshes recursively process the childnodes
    for (uint32 i = 0; i < node->mNumChildren; i++) {
        this->ProcessAssimpNode(node->mChildren[i], scene, dir, meshes, resourceLifeTime);
    }
}

MeshResource ResourceManager::ProcessAssimpMesh(aiMesh* mesh, const aiScene* scene, const std::string& dir, ResourceLifeTime resourceLifeTime) {
    std::vector<Vertex> tmpVertices;
    std::vector<uint32> tmpIndices;
    
    // Get all of the mesh's vertices
    for (uint32 i = 0; i < mesh->mNumVertices; i++) {
        Vertex tmpvec;
        tmpvec.Position.x = mesh->mVertices[i].x;
        tmpvec.Position.y = mesh->mVertices[i].y;
        tmpvec.Position.z = mesh->mVertices[i].z;
        tmpvec.Normal.x = mesh->mNormals[i].x;
        tmpvec.Normal.y = mesh->mNormals[i].y;
        tmpvec.Normal.z = mesh->mNormals[i].z;
        tmpvec.Tangent.x = mesh->mTangents[i].x;
        tmpvec.Tangent.y = mesh->mTangents[i].y;
        tmpvec.Tangent.z = mesh->mTangents[i].z;
        tmpvec.BiTangent.x = mesh->mBitangents[i].x;
        tmpvec.BiTangent.y = mesh->mBitangents[i].y;
        tmpvec.BiTangent.z = mesh->mBitangents[i].z;
        if (mesh->mTextureCoords[0]) {
            tmpvec.TextureCoords.x = mesh->mTextureCoords[0][i].x;
            tmpvec.TextureCoords.y = mesh->mTextureCoords[0][i].y;
        }
        else {
            tmpvec.TextureCoords = glm::vec2(0.0f);
        }
        tmpVertices.push_back(tmpvec);
    }
    
    // Get all the the mesh's indices (by faces / triangles).
    for (uint32 i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (uint32 j = 0; j < face.mNumIndices; j++) {
            tmpIndices.push_back(face.mIndices[j]);
        }
    }
    
    // Process materials and textures
    TextureResource* texture = nullptr;
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // Get the material reflection values (but store them as a single float)
        Material tmpMat;
        aiColor3D color(0.f, 0.f, 0.f);
        material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        tmpMat.DiffuseReflectivity = (color.r + color.g + color.b) / 3.0f;
        material->Get(AI_MATKEY_COLOR_SPECULAR, color);
        tmpMat.SpectralReflectivity = (color.r + color.g + color.b) / 3.0f;
        material->Get(AI_MATKEY_COLOR_AMBIENT, color);
        tmpMat.AmbientReflectivity = (color.r + color.g + color.b) / 3.0f;
        
        // Get the default (DIFFUSE) Texture name and check if it was already loaded
        // If not, create the full Texture object with Diffuse, Normal and Specular images
        aiString aiTexStr;
        material->GetTexture(aiTextureType_DIFFUSE, 0, &aiTexStr);
        const std::string textureName = std::string(aiTexStr.C_Str());
        if (!textureName.length() || !IsResourceLoaded(textureName)) {
            // Get each part of the Texture if the "overall" TextureResource did not exist
            uint32 textureDiffuse = ProcessAssimpTexture(material, dir, aiTextureType_DIFFUSE);
            uint32 textureNormal = ProcessAssimpTexture(material, dir, aiTextureType_HEIGHT);
            uint32 textureSpecular = ProcessAssimpTexture(material, dir, aiTextureType_SPECULAR);
            
            auto tmpTexture = std::make_shared<TextureResource>(TextureResource(textureName, tmpMat, textureDiffuse, textureNormal, textureSpecular));
            _resources.insert({ textureName, tmpTexture });
            _resouceLifetimes[resourceLifeTime].push_back(textureName);
            texture = tmpTexture.get();
        }
    }
    
    // Return a mesh object created from the extracted mesh data
    return MeshResource(dir, tmpVertices, tmpIndices, texture);
}


uint32 ResourceManager::ProcessAssimpTexture(aiMaterial* material, const std::string& dir, aiTextureType type) const {
    std::string filename;
    // In case a Assimp Material is available: Parse the Texture path based on that information
    if(material != nullptr) {
        aiString texStr;
        material->GetTexture(type, 0, &texStr);
        if (texStr.length) {
            #ifdef _WIN32
            filename = (dir + "\\" + texStr.C_Str());
            #else
            filename = (dir + "/" + texStr.C_Str());
            #endif
        }
    }
    // Otherwise just use the given directory (and hope its a full path)
    else {
        filename = dir;
    }
    
    if(!filename.length())
        return 0;

    //Generate texture ID and load texture data
    uint32 textureID = 0;
    glGenTextures(1, &textureID);
    // get the texture data with the stb image lib
    int width = 0, height = 0, numComponents = 0;
    byte* imageData = stbi_load(filename.c_str(), &width, &height, &numComponents, 4);
    if (imageData == nullptr) { std::cerr << " ERROR: loading texture " << std::endl; }
    // Parameters
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    if (MIP_MAPPING) {
        // GL_LINEAR and GL_LINEAR_MIPMAP_LINEAR to creata a "smooth" mip-mapped Image
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else {
        // GL_NEAREST to create one "pixel-sharp" Texture Image
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    // Activate Anisotropic Filtering for the Texture
    if (ANISOTROPIC_FILTERING) {
        GLfloat largest_supported_anisotropy;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest_supported_anisotropy);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest_supported_anisotropy);
    }
    // Finally set the Texutre data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
    glGenerateMipmap(GL_TEXTURE_2D);
    // Cleanup
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(imageData);
    return textureID;
}


//
// Explicitly instanciate every form of LoadExternalResource
// This has two upsides:
// - .hpp will not get cluttered with function definitions
// - There are not many Resource-Types so its easy to provide a concrete interface
//
template MeshResource* ResourceManager::LoadResource<MeshResource>(ResourceLifeTime resourceLifeTime, const std::string& name);
template ModelResource* ResourceManager::LoadResource<ModelResource>(ResourceLifeTime resourceLifeTime, const std::string& name);
template TextureResource* ResourceManager::LoadResource<TextureResource>(ResourceLifeTime resourceLifeTime, const std::string& name);
