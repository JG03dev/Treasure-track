//
// Created by jg03dev on 10/20/23.
//
#pragma once

#include <SOIL2/SOIL2.h>
#include <cstdio>
#include <GL/glew.h>
#include "Entorn/stdafx.h"

class Texture {
public:
    Texture();
    Texture(char* fileLoc);

    void LoadTexture();
    void UseTexture();
    void ClearTexture();

    GLuint getTextureID() { return textureID; };

    ~Texture();
private:
    GLuint textureID;
    int width, height, bitDepth;

    char	szTexture[_MAX_PATH];// Filename of the texture
};

