#pragma once
#include "Module.h"
#include "Globals.h"
#include "DirectXTex.h"

class Application;

class ModuleTexture : public Module
{
public:
	ModuleTexture();
	~ModuleTexture();

	bool Init();
	update_status Update();
	bool CleanUp();

	int getTexture(const wchar_t* filename);
	void setTextConf();
	void textProperties();

	unsigned int texture;
private:
	DirectX::TexMetadata metadata;
	DirectX::ScratchImage textureData;
};