#pragma once
#include "Module.h"
#include "Globals.h"
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#include "tiny_gltf.h"
#include <memory>

class Application;
class ModuleTexture;

struct Mesh {
	std::string name;
	std::vector<uint32_t> indices;
	std::vector<float> vertexData;

	size_t vertexCount = 0;
	size_t triangleCount = 0;

	unsigned int vao = -1;

	void load(const tinygltf::Model& model, const tinygltf::Primitive& primitive);
};

class ModuleModel : public Module
{
public:
	std::vector<std::unique_ptr<Mesh>> meshes;
	std::vector<int> textures;

	ModuleModel();
	~ModuleModel();
	bool Init();

	void modelProperties();

	void Load(const char* assetFileName);
};