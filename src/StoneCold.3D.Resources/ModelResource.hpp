
#ifndef STONECOLD_MODELRESOURCE_H
#define STONECOLD_MODELRESOURCE_H

#include "MeshResource.hpp"

namespace StoneCold::Resources {

	//
	// ModelResource
	//
	// A full Model is simply a collection of Meshes (MeshResources)
	//
	class ModelResource : public Resource {
	public:
		const std::vector<MeshResource> Model;

	public:
		ModelResource(const std::string& name, const std::vector<MeshResource>& model)
			: Resource(name), Model(model) {}
	};

}

#endif
