#ifndef TEXTURE_H
#define TEXTURE_H

#include "../include/stdafx.h"

enum TextureType {
	TEX_UNKNOWN,
	TEX_DIFFUSE,
	TEX_SPECULAR,
	TEX_NORMAL,
	TEX_HEIGHT,
	TEX_EMISSION,
	TEX_AMBIENT
};

struct Texture {
	unsigned int id;
	TextureType type;
	std::string path;
};

extern std::unordered_map<TextureType, std::string> TextureTypeName;

/** Methods */

unsigned int LoadTexture(const std::string textureFile, bool gamma = false);

#endif