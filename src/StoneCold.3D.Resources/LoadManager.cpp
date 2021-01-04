#include "LoadManager.hpp"

using namespace StoneCold::Resources;

std::shared_ptr<Model> LoadManager::LoadSimpleModelFromFile(const std::string& path) {
	std::vector<Mesh> meshes;
	std::vector<Vertex> tmpVertices;
	std::vector<uint16> tmpIndices;

	// Parse the .obj file data
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_CalcTangentSpace);
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
			tmpvec.Texcoords = glm::vec2();
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

	meshes.push_back(Mesh(tmpVertices, tmpIndices, /*Texture(0, 0, 0, ""), */(path.substr(0, path.rfind('/')))));
	return std::make_shared<Model>(meshes);
}


//// Load complex Model: multiple Files, multiple Textures and Materials
//std::shared_ptr<Model> LoadComplexModelFromFile(const std::string& path) {
//    Assimp::Importer importer;
//    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
//    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
//        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
//        return nullptr;
//    }
//    else {
//        // Retrieve the directory path of the filepath
//        const std::string directory = path.substr(0, path.find_last_of('\\'));
//        std::vector<Mesh> tmpMeshes;
//        // Process ASSIMP nodes recursively
//        this->processNode(scene->mRootNode, scene, directory, tmpMeshes);
//        // Create the loaded Model
//        return std::make_shared<Model>(tmpMeshes);
//    }
//}

//void processNode(aiNode* node, const aiScene* scene, const std::string& dir, std::vector<Mesh>& meshes) {
//    // Process each mesh located at the current node
//    for (uint i = 0; i < node->mNumMeshes; i++) {
//        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
//        meshes.push_back(this->processMesh(mesh, scene, dir));
//    }
//    // After processing the meshes recursively process the childnodes
//    for (uint i = 0; i < node->mNumChildren; i++) {
//        this->processNode(node->mChildren[i], scene, dir, meshes);
//    }
//}

//Mesh processMesh(aiMesh* mesh, const aiScene* scene, const std::string& dir) {
//    std::vector<Vertex> tmpVertices;
//    std::vector<uint> tmpIndices;
//    Texture tmpTexture = Texture(0, 0, 0, "");
//    // Get all of the mesh's vertices
//    for (uint i = 0; i < mesh->mNumVertices; i++) {
//        Vertex tmpvec;
//        tmpvec.position.x = mesh->mVertices[i].x;
//        tmpvec.position.y = mesh->mVertices[i].y;
//        tmpvec.position.z = mesh->mVertices[i].z;
//        tmpvec.normal.x = mesh->mNormals[i].x;
//        tmpvec.normal.y = mesh->mNormals[i].y;
//        tmpvec.normal.z = mesh->mNormals[i].z;
//        tmpvec.tangent.x = mesh->mTangents[i].x;
//        tmpvec.tangent.y = mesh->mTangents[i].y;
//        tmpvec.tangent.z = mesh->mTangents[i].z;
//        if (mesh->mTextureCoords[0]) {
//            tmpvec.texcoords.x = mesh->mTextureCoords[0][i].x;
//            tmpvec.texcoords.y = mesh->mTextureCoords[0][i].y;
//        }
//        else {
//            tmpvec.texcoords = glm::vec2(0.0f, 0.0f);
//        }
//        tmpVertices.push_back(tmpvec);
//    }
//    // Get all the the mesh's indices (by faces / triangles).
//    for (uint i = 0; i < mesh->mNumFaces; i++) {
//        aiFace face = mesh->mFaces[i];
//        for (uint j = 0; j < face.mNumIndices; j++) {
//            tmpIndices.push_back(face.mIndices[j]);
//        }
//    }
//    // Process materials and textures
//    if (mesh->mMaterialIndex >= 0) {
//        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
//        // Get the texture data
//        uint textureDiffuse = loadOrGetTexture(material, dir, aiTextureType_DIFFUSE);
//        uint textureSpecular = loadOrGetTexture(material, dir, aiTextureType_SPECULAR);
//        uint textureNormal = loadOrGetTexture(material, dir, aiTextureType_HEIGHT);
//        // Get the material reflection values (but store them as a single float)
//        Material tmpMat;
//        aiColor3D color(0.f, 0.f, 0.f);
//        material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
//        tmpMat.diffuseReflectivity = (color.r + color.g + color.b) / 3.0f;
//        material->Get(AI_MATKEY_COLOR_SPECULAR, color);
//        tmpMat.spectralReflectivity = (color.r + color.g + color.b) / 3.0f;
//        material->Get(AI_MATKEY_COLOR_AMBIENT, color);
//        tmpMat.ambientReflectivity = (color.r + color.g + color.b) / 3.0f;
//        // Create the full Texture object
//        tmpTexture = Texture(textureDiffuse, textureNormal, textureSpecular, tmpMat, dir);
//    }
//    // Return a mesh object created from the extracted mesh data
//    return Mesh(tmpVertices, tmpIndices, tmpTexture, dir);
//}


//uint loadOrGetTexture(aiMaterial* material, const std::string& dir, aiTextureType type) {
//    aiString str;
//    uint tmpTex = 0;
//    material->GetTexture(type, 0, &str);
//    if (str.length) {
//        const std::string tmpPath = dir + "\\" + str.C_Str();
//        // Check if The texture was already loaded and get it, else load it
//        if (texturesLoaded.find(tmpPath) != texturesLoaded.end()) {
//            tmpTex = texturesLoaded[tmpPath];
//        }
//        else {
//            tmpTex = LoadTextureFromFile(tmpPath);
//            texturesLoaded.insert(std::make_pair(tmpPath, tmpTex));
//        }
//        return tmpTex;
//    }
//    else {
//        return 0;
//    }
//}


//uint LoadTextureFromFile(const std::string& filename) const {
//    //Generate texture ID and load texture data
//    uint textureID = 0;
//    glGenTextures(1, &textureID);
//    // get the texture data with the stb image lib
//    int width = 0, height = 0, numComponents = 0;
//    abyte* imageData = stbi_load(filename.c_str(), &width, &height, &numComponents, 4);
//    if (imageData == nullptr) { std::cerr << " ERROR: loading texture " << std::endl; }
//    // Assign texture to ID
//    glBindTexture(GL_TEXTURE_2D, textureID);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
//    // Parameters
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    // Activate MipMaping for the Texture
//    if (MIP_MAPPING) {
//        glGenerateMipmap(GL_TEXTURE_2D);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    }
//    // Activate Anisotropic Filtering for the Texture
//    if (ANISOTROPIC_FILTERING) {
//        GLfloat largest_supported_anisotropy;
//        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest_supported_anisotropy);
//        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest_supported_anisotropy);
//    }
//    // Cleanup
//    glBindTexture(GL_TEXTURE_2D, 0);
//    stbi_image_free(imageData);
//    return textureID;
//}