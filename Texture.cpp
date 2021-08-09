#include "Texture.hpp"

#include <GL/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./external/stb_image.h"

#include "Macros.hpp"

namespace Minecrap
{

namespace Graphics
{


void Texture::init(const std::string& tAddress)
{
	// Texture
	glGenTextures(1, &mId);
	bind();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int lTextureWidth;
	int lTextureHeight;
	int lTextureChannels;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* lTextureRaw = stbi_load(tAddress.c_str(), &lTextureWidth, &lTextureHeight, &lTextureChannels, 0);

	ASSERT(lTextureRaw != nullptr, "Could not load texture!!");

	unsigned int lTextureType = lTextureChannels == 3? GL_RGB: GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, lTextureType, lTextureWidth, lTextureHeight, 0, lTextureType, GL_UNSIGNED_BYTE, lTextureRaw);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(lTextureRaw);
};

void Texture::bind(void)
{
	glBindTexture(GL_TEXTURE_2D, mId);
};


}

}
