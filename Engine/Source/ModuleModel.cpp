#include "ModuleModel.h"
#include <SDL_assert.h>
#include <GL/glew.h>
#include <Math/float3.h>
#include <Math/float2.h>
#include "Application.h"
#include "ModuleTexture.h"
#include "ImGui/imgui.h"
#include "Math/Quat.h"

ModuleModel::ModuleModel() {

}

ModuleModel::~ModuleModel() {

}

bool ModuleModel::Init() {
	return true;
}

void ModuleModel::modelProperties() {
	ImGui::Begin("Properties");
	int vertexCountModel = 0;
	int triangleCountModel = 0;
	for (const auto& mesh : App->GetModel()->meshes) {
		vertexCountModel = mesh.get()->vertexCount;
		triangleCountModel = mesh.get()->triangleCount;
	}
	ImGui::Text("Vertex Count: %d", vertexCountModel);
	ImGui::Text("Triangle Count: %d", triangleCountModel);
	App->GetTexture()->textProperties();
	ImGui::End();
};

void ModuleModel::LoadTexture(const char* assetFileName) {
	unsigned int textureId = 0;
	int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, assetFileName, -1, nullptr, 0);
	if (sizeNeeded > 0) {
		std::wstring wideString(sizeNeeded, 0);
		MultiByteToWideChar(CP_UTF8, 0, assetFileName, -1, &wideString[0], sizeNeeded);
		textureId = (App->GetTexture()->getTexture(wideString.c_str()));
		if (textureId == -1) return;
		textures.clear();
		textures.push_back(textureId);
	}
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
	meshes.clear();
	textures.clear();

	for (const auto& srcMesh : model.meshes)
	{	
		std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>();
		mesh->name = srcMesh.name;
		float3 translation(0.0f, 0.0f, 0.0f);
		Quat rotation(1.0f, 0.0f, 0.0f, 0.0f);
		float3 scale(1.0f, 1.0f, 1.0f);
		for (const auto& primitive : srcMesh.primitives)
		{
			mesh->load(model, primitive);
		}
		for (const auto& srcNode : model.nodes) {
			if (srcNode.name == mesh->name) {

				if (!srcNode.translation.empty()) {
					translation = float3(srcNode.translation[0], srcNode.translation[1], srcNode.translation[2]);
				}
				if (!srcNode.rotation.empty()) {
					rotation = Quat(srcNode.rotation[0], srcNode.rotation[1], srcNode.rotation[2], srcNode.rotation[3]);
				}
				if (!srcNode.scale.empty()) {
					scale = float3(srcNode.scale[0], srcNode.scale[1], srcNode.scale[2]);
				}
			}
		}
		mesh->modelMatrix = float4x4::FromTRS(translation, rotation, scale);
		meshes.push_back(std::move(mesh));
	}

	for (const auto& srcMaterial : model.materials)
	{
		unsigned int textureId = 0;
		if (srcMaterial.pbrMetallicRoughness.baseColorTexture.index >= 0)
		{
			const tinygltf::Texture& texture = model.textures[srcMaterial.pbrMetallicRoughness.baseColorTexture.index];
			const tinygltf::Image& image = model.images[texture.source];
			std::wstring wstr(image.uri.begin(), image.uri.end());
			textureId = (App->GetTexture()->getTexture(wstr.c_str()));
		}

		textures.push_back(textureId);
	}
}

void Mesh::load(const tinygltf::Model& model, const tinygltf::Primitive& primitive) {
	size_t vertexStride = 0;

	std::vector<float3> positions;

	const auto& itPos = primitive.attributes.find("POSITION");
	if (itPos != primitive.attributes.end())
	{
		const tinygltf::Accessor& posAcc = model.accessors[itPos->second];
		SDL_assert(posAcc.type == TINYGLTF_TYPE_VEC3);
		SDL_assert(posAcc.componentType == GL_FLOAT);
		const tinygltf::BufferView& posView = model.bufferViews[posAcc.bufferView];
		const tinygltf::Buffer& posBuffer = model.buffers[posView.buffer];
		const unsigned char* bufferPos = &(posBuffer.data[posAcc.byteOffset + posView.byteOffset]);

		vertexCount = posAcc.count;
		vertexStride += 3;

		for (size_t i = 0; i < posAcc.count; ++i) {
			positions.push_back(*reinterpret_cast<const float3*>(bufferPos));
			bufferPos += posView.byteStride;
		}
	}

	std::vector<float2> texCoords;
	bool hasTexCoords = false;
	const auto& itTexCoord = primitive.attributes.find("TEXCOORD_0");
	if (itTexCoord != primitive.attributes.end())
	{
		const tinygltf::Accessor& texCoordAcc = model.accessors[itTexCoord->second];
		SDL_assert(texCoordAcc.type == TINYGLTF_TYPE_VEC2);
		SDL_assert(texCoordAcc.componentType == GL_FLOAT);
		const tinygltf::BufferView& texCoordView = model.bufferViews[texCoordAcc.bufferView];
		const tinygltf::Buffer& texCoordBuffer = model.buffers[texCoordView.buffer];
		const unsigned char* bufferTexCoord = &(texCoordBuffer.data[texCoordAcc.byteOffset + texCoordView.byteOffset]);

		vertexStride += 2;
		hasTexCoords = true;

		for (size_t i = 0; i < texCoordAcc.count; ++i) {
			texCoords.push_back(*reinterpret_cast<const float2*>(bufferTexCoord));
			bufferTexCoord += texCoordView.byteStride;
		}
	}

	for (size_t i = 0; i < vertexCount; ++i) {
		vertexData.push_back(positions[i].x);
		vertexData.push_back(positions[i].y);
		vertexData.push_back(positions[i].z);
		if (hasTexCoords) {
			vertexData.push_back(texCoords[i].x);
			vertexData.push_back(texCoords[i].y);
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * vertexStride, (void*)0);

	if (hasTexCoords) {
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * vertexStride, (void*)(sizeof(float) * 3));
	}

	if (primitive.indices >= 0)
	{
		const tinygltf::Accessor& indAcc = model.accessors[primitive.indices];
		const tinygltf::BufferView& indView = model.bufferViews[indAcc.bufferView];
		const unsigned char* buffer = &(model.buffers[indView.buffer].data[indAcc.byteOffset + indView.byteOffset]);

		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indAcc.count, nullptr, GL_STATIC_DRAW);
		unsigned int* ptr = reinterpret_cast<unsigned int*>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY));

		triangleCount = indAcc.count / 3;


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
