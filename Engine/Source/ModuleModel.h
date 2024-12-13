#pragma once
#include "Module.h"
#include "Globals.h"
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#include "tiny_gltf.h"
#include <memory>
#include <Math/float4x4.h>

class Application;
class ModuleTexture;

struct BoundingBox {
	float3 min;
	float3 max;
};

struct Mesh {
	std::string name;
	std::vector<uint32_t> indices;
	std::vector<float> vertexData;

	size_t vertexCount = 0;
	size_t triangleCount = 0;

	unsigned int vao = -1;
	unsigned int vbo = -1;
	unsigned int ebo = -1;

	float4x4 modelMatrix;
	BoundingBox box;

	void load(const tinygltf::Model& model, const tinygltf::Primitive& primitive);
};

class ModuleModel : public Module
{
public:
	std::vector<std::unique_ptr<Mesh>> meshes;
	std::vector<int> textures;
	BoundingBox box;
	float3 center;
	int vertexCountModel = 0;
	int triangleCountModel = 0;

	ModuleModel();
	~ModuleModel();
	bool Init();

	void modelProperties();

	bool Load(const char* assetFileName);
	void LoadTexture(const char* assetFileName);
};