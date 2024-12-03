#pragma once
#include "Module.h"
#include "Globals.h"
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#include "tiny_gltf.h"

class Application;

struct Primitive {
	std::map<std::string, int> attributes;
	int material{ -1 };
	int indices{ -1 };
};

struct Mesh {
	std::string name;
	std::vector<Primitive> primitives;

	void load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
	void LoadEBO(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive);
};

class ModuleModel : public Module
{
public:
	ModuleModel();
	~ModuleModel();
	bool Init();

	void Load(const char* assetFileName);
	Mesh* mesh;
};