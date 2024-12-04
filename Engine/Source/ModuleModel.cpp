#include "ModuleModel.h"
#include <SDL_assert.h>
#include <GL/glew.h>
#include <Math/float3.h>
#include "Application.h"

ModuleModel::ModuleModel() {

}

ModuleModel::~ModuleModel() {

}

bool ModuleModel::Init() {
	return true;
}

//glDeleteBuffers I need to do it!!!
//glDeleteVertexArrays(1, &vao);
void ModuleModel::Load(const char* assetFileName) {
	tinygltf::TinyGLTF gltfContext;
	tinygltf::Model model;
	std::string error, warning;
	bool loadOk = gltfContext.LoadASCIIFromFile(&model, &error, &warning, assetFileName);
	if (!loadOk)
	{
		LOG("Error loading %s: %s", assetFileName, error.c_str());
		return;
	}

	for (const auto& srcMesh : model.meshes)
	{	
		for (const auto& primitive : srcMesh.primitives)
		{
			std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>();
			mesh->load(model, primitive);
			meshes.push_back(std::move(mesh));
		}	
	}
}

void Mesh::load(const tinygltf::Model& model, const tinygltf::Primitive& primitive) {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	const auto& itPos = primitive.attributes.find("POSITION");
	if (itPos != primitive.attributes.end())
	{
		const tinygltf::Accessor& posAcc = model.accessors[itPos->second];
		SDL_assert(posAcc.type == TINYGLTF_TYPE_VEC3);
		SDL_assert(posAcc.componentType == GL_FLOAT);
		const tinygltf::BufferView& posView = model.bufferViews[posAcc.bufferView];
		const tinygltf::Buffer& posBuffer = model.buffers[posView.buffer];
		const unsigned char* bufferPos = &(posBuffer.data[posAcc.byteOffset + posView.byteOffset]);

		unsigned int vbo = -1;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * posAcc.count, nullptr, GL_STATIC_DRAW);
		float3* ptr = reinterpret_cast<float3*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));

		for (size_t i = 0; i < posAcc.count; ++i) {
			positions.push_back(*reinterpret_cast<const float3*>(bufferPos));
			ptr[i] = *reinterpret_cast<const float3*>(bufferPos);
			bufferPos += posView.byteStride;
		}

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), (void*)0);

		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	if (primitive.indices >= 0)
	{
		const tinygltf::Accessor& indAcc = model.accessors[primitive.indices];
		const tinygltf::BufferView& indView = model.bufferViews[indAcc.bufferView];
		const unsigned char* buffer = &(model.buffers[indView.buffer].data[indAcc.byteOffset + indView.byteOffset]);

		unsigned int ebo = -1;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indAcc.count, nullptr, GL_STATIC_DRAW);
		unsigned int* ptr = reinterpret_cast<unsigned int*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));


		if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT) {
			const uint32_t* bufferInd = reinterpret_cast<const uint32_t*>(buffer);
			for (uint32_t i = 0; i < indAcc.count; ++i) {
				indices.push_back(bufferInd[i]);
				ptr[i] = bufferInd[i];
			}
		}
		else if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT) {
			const uint16_t* bufferInd = reinterpret_cast<const uint16_t*>(buffer);
			for (uint16_t i = 0; i < indAcc.count; ++i) {
				indices.push_back(static_cast<uint32_t>(bufferInd[i]));
				ptr[i] = bufferInd[i];
			}
		}
		else if (indAcc.componentType == TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE) {
			const uint8_t* bufferInd = reinterpret_cast<const uint8_t*>(buffer);
			for (uint8_t i = 0; i < indAcc.count; ++i) {
				indices.push_back(static_cast<uint32_t>(bufferInd[i]));
				ptr[i] = bufferInd[i];
			}
		}
		else {
			SDL_assert(false && "Unsupported index component type in GLTF primitive.");
		}
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	}

	glBindVertexArray(0);
}
