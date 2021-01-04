
#ifndef STONECOLD_MODELRESOURCE_H
#define STONECOLD_MODELRESOURCE_H

#include "MeshResource.hpp"

namespace StoneCold::Resources {

	//
	// ModelResource
	//
	// A full Model is simply a collection of Meshes (MeshResources)
	//
	class Mesh;
	class ModelResource : public Resource {
	public:
		const std::unique_ptr<std::vector<MeshResource>> Model;

	public:
		ModelResource(const std::string& name, std::unique_ptr<std::vector<MeshResource>>&& model)
			: Resource(name), Model(std::move(model)) {}
	};

}

#endif
