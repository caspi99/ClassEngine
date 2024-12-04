#pragma once
#include "Module.h"
#include "Globals.h"
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#include "tiny_gltf.h"
#include <memory>
#include <Math/float3.h>

class Application;

struct Primitive {
	std::map<std::string, int> attributes;
	int material{ -1 };

	std::vector<uint32_t> indices;
};

struct Mesh {
	std::string name;
	std::vector<uint32_t> indices;
	std::vector<float3> positions;

	unsigned int vao = -1;

	void load(const tinygltf::Model& model, const tinygltf::Primitive& primitive);
};

class ModuleModel : public Module
{
public:
	std::vector<std::unique_ptr<Mesh>> meshes;

	ModuleModel();
	~ModuleModel();
	bool Init();

	void Load(const char* assetFileName);
};