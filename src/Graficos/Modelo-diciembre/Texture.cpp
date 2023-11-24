#include "Texture.h"

std::unordered_map<TextureType, std::string> TextureTypeName = {
	std::pair<TextureType, std::string>(TEX_UNKNOWN,  "texture_unknown"),
	std::pair<TextureType, std::string>(TEX_DIFFUSE,  "texture_diffuse"),
	std::pair<TextureType, std::string>(TEX_SPECULAR, "texture_specular"),
	std::pair<TextureType, std::string>(TEX_NORMAL,   "texture_normal"),
	std::pair<TextureType, std::string>(TEX_HEIGHT,   "texture_height"),
	std::pair<TextureType, std::string>(TEX_EMISSION, "texture_emission"),
	std::pair<TextureType, std::string>(TEX_AMBIENT,  "texture_ambient")
};

unsigned int LoadTexture(const std::string filename, bool gamma) {

	unsigned int textureID{};
	glGenTextures(1, &textureID);

	int width, height, bitDepth;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &bitDepth, 0);

	if (!data)
		std::cerr << "LoadTexture: Texture failed to load at path: " << filename << "\n";

	else {
		GLenum imageFormat;
		GLenum dataFormat;
		if (bitDepth == 1) {
			imageFormat = GL_RED;
			dataFormat = GL_RED;
		}
		else if (bitDepth == 3) {
			imageFormat = gamma ? GL_SRGB : GL_RGB;
			dataFormat = GL_RGB;
		}
		else if (bitDepth == 4) {
			imageFormat = gamma ? GL_SRGB_ALPHA : GL_RGBA;
			dataFormat = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	stbi_image_free(data);

	return textureID;
}