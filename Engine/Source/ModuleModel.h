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

	unsigned int vao = 0;
	unsigned int vbo = 0;
	unsigned int ebo = 0;

	float4x4 modelMatrix;
	BoundingBox box;

	void load(const tinygltf::Model& model, const tinygltf::Primitive& primitive);
	void CleanUp();
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
	bool CleanUp();

	void modelProperties();

	bool Load(const char* assetFileName);
	void LoadTexture(const char* assetFileName);
};