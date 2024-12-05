#include "ModuleTexture.h"
#include <GL/glew.h>
#include "ImGui/imgui.h"

ModuleTexture::ModuleTexture(){
	texture = 0;
}

ModuleTexture::~ModuleTexture(){

}

void ModuleTexture::setTextConf() {

}

void ModuleTexture::textProperties() {
	ImGui::Begin("Texture Properties");
	ImGui::Text("Width: %d  Height: %d", metadata.width, metadata.height);
	switch (metadata.format) {
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
		ImGui::Text("Format: DXGI_FORMAT_R8G8B8A8_UNORM");
		break;
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
		ImGui::Text("Format: DXGI_FORMAT_B8G8R8A8_UNORM");
		break;
	case DXGI_FORMAT_B5G6R5_UNORM:
		ImGui::Text("Format: DXGI_FORMAT_B5G6R5_UNORM");
		break;
	default:
		assert(false && "Unsupported format");
	}
	ImGui::End();
}

int ModuleTexture::getTexture(const wchar_t* filename) {
	HRESULT hr = DirectX::LoadFromDDSFile(filename, DirectX::DDS_FLAGS_NONE, &metadata, textureData);

	// Check for errors
	if (FAILED(hr)) {
		hr = DirectX::LoadFromTGAFile(filename, &metadata, textureData);
		if (FAILED(hr)) {
			hr = DirectX::LoadFromWICFile(filename, DirectX::WIC_FLAGS_NONE, &metadata, textureData);
			if (FAILED(hr)) {
				return false;
			}
		}
	}


	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_CLAMP_TO_BORDER, GL_CLAMP, GL_REPEAT, GL_MIRRORED_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //GL_CLAMP_TO_BORDER, GL_CLAMP, GL_REPEAT, GL_MIRRORED_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); /*Without interpolation: GL_NEAREST_MIPMAP_NEAREST
																					Interpolating samples at a mipmap level: GL_LINEAR_MIPMAP_NEAREST
																					Interpolating mipmap levels: GL_NEAREST_MIPMAP_LINEAR,GL_LINEAR_MIPMAP_LINEAR*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//For using mipmaps
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
		break;
	case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
	case DXGI_FORMAT_B8G8R8A8_UNORM:
		internalFormat = GL_RGBA8;
		format = GL_BGRA;
		type = GL_UNSIGNED_BYTE;
		break;
	case DXGI_FORMAT_B5G6R5_UNORM:
		internalFormat = GL_RGB8;
		format = GL_BGR;
		type = GL_UNSIGNED_BYTE;
		break;
	default:
		assert(false && "Unsupported format");
	}

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
