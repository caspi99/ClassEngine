#include "ModuleTexture.h"
#include <GL/glew.h>
#include "ImGui/imgui.h"

ModuleTexture::ModuleTexture(){
	texture = 0;
	formatName = nullptr;
	wrapSIndex = 0;
	wrapTIndex = 0;
	minFilterIndex = 3;
	maxMipmapLevel = 0;
}

ModuleTexture::~ModuleTexture(){

}

GLint GetGLWrapMode(int index) {
	switch (index) {
	case 0: return GL_REPEAT;
	case 1: return GL_CLAMP_TO_BORDER;
	case 2: return GL_CLAMP;
	case 3: return GL_MIRRORED_REPEAT;
	default: return GL_REPEAT;
	}
}

GLint GetGLMinFilter(int index) {
	switch (index) {
	case 0: return GL_NEAREST_MIPMAP_NEAREST;
	case 1: return GL_LINEAR_MIPMAP_NEAREST;
	case 2: return GL_NEAREST_MIPMAP_LINEAR;
	case 3: return GL_LINEAR_MIPMAP_LINEAR;
	default: return GL_LINEAR_MIPMAP_LINEAR;
	}
}

void ModuleTexture::setTextConf() {
	const char* wrapModes[] = { "GL_REPEAT", "GL_CLAMP_TO_BORDER", "GL_CLAMP", "GL_MIRRORED_REPEAT" };
	const char* minFilters[] = { "GL_NEAREST_MIPMAP_NEAREST", "GL_LINEAR_MIPMAP_NEAREST", "GL_NEAREST_MIPMAP_LINEAR", "GL_LINEAR_MIPMAP_LINEAR"	};
	glBindTexture(GL_TEXTURE_2D, texture);

	if (ImGui::Combo("GL_TEXTURE_WRAP_S", &wrapSIndex, wrapModes, IM_ARRAYSIZE(wrapModes))) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetGLWrapMode(wrapSIndex));
	}
	if (ImGui::Combo("GL_TEXTURE_WRAP_T", &wrapTIndex, wrapModes, IM_ARRAYSIZE(wrapModes))) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetGLWrapMode(wrapTIndex));
	}

	if (ImGui::Combo("GL_TEXTURE_MIN_FILTER", &minFilterIndex, minFilters, IM_ARRAYSIZE(minFilters))) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetGLMinFilter(minFilterIndex));
	}

	if (ImGui::SliderInt("Max Mipmap Level", &maxMipmapLevel, 0, textureData.GetMetadata().mipLevels - 1)) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, maxMipmapLevel);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void ModuleTexture::textProperties() {
	ImGui::Text("Texture Width: %d  Texture Height: %d", metadata.width, metadata.height);
	ImGui::Text("Format Texture: %s", formatName);
}

int ModuleTexture::getTexture(const wchar_t* filename) {
	HRESULT hr = DirectX::LoadFromDDSFile(filename, DirectX::DDS_FLAGS_NONE, &metadata, textureData);

	// Check for errors
	if (FAILED(hr)) {
		hr = DirectX::LoadFromTGAFile(filename, &metadata, textureData);
		if (FAILED(hr)) {
			hr = DirectX::LoadFromWICFile(filename, DirectX::WIC_FLAGS_NONE, &metadata, textureData);
			if (FAILED(hr)) {
				return -1;
			}
		}
	}


	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, textureData.GetMetadata().mipLevels - 1);

	unsigned int internalFormat;
	unsigned int format;
	unsigned int type;
	switch (metadata.format) {
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
		internalFormat = GL_RGBA8;
		format = GL_RGBA;
		type = GL_UNSIGNED_BYTE;
		formatName = "DXGI_FORMAT_R8G8B8A8_UNORM";
		break;
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
		internalFormat = GL_RGBA8;
		format = GL_BGRA;
		type = GL_UNSIGNED_BYTE;
		formatName = "DXGI_FORMAT_B8G8R8A8_UNORM";
		break;
	case DXGI_FORMAT_B5G6R5_UNORM:
		internalFormat = GL_RGB8;
		format = GL_BGR;
		type = GL_UNSIGNED_BYTE;
		formatName = "DXGI_FORMAT_B5G6R5_UNORM";
		break;
	default:
		assert(false && "Unsupported format");
	}

	maxMipmapLevel = textureData.GetMetadata().mipLevels - 1;
	if (metadata.mipLevels > 1) {
		for (size_t i = 0; i < textureData.GetMetadata().mipLevels; ++i)
		{
			const DirectX::Image* mip = textureData.GetImage(i, 0, 0);
			glTexImage2D(GL_TEXTURE_2D, i, internalFormat, mip->width, mip->height, 0, format, type, mip->pixels);
		}
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, metadata.width, metadata.height, 0, format, type, textureData.GetPixels());
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	return texture;
}

bool ModuleTexture::Init(){
	return true;
}

update_status ModuleTexture::Update(){
	return UPDATE_CONTINUE;
}

bool ModuleTexture::CleanUp(){
	glDeleteTextures(1, &texture);
	return true;
}
