#pragma once

#include "../../Encabezados/stdafx.h"

class Texture
{
public:
	Texture();
	Texture(const char* fileLoc);

	bool LoadTexture();
	bool LoadTextureA();

	void UseTexture();
	void ClearTexture();

	~Texture();

	int getWidth() { return width; }
	int getHeight() { return height; }
	int getbitDepth() { return bitDepth; }

private:
	GLuint textureID;
	int width, height, bitDepth;

	const char* fileLocation;
};

