//
// Created by jg03dev on 10/9/23.
//
#pragma once

#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

class Shader {
public:
    Shader();

    void CreateFromString(const char* vertexCode, const char* fragmentCode, const char* geometricCode="");
    void CreateFromFiles(std::string vertexLocation, std::string fragmentLocation, std::string geometricLocation="");

    std::string ReadFile(const char* fileLocation);

    GLuint GetProjectionLocation(){ return uniformProjection; }
    GLuint GetModelLocation(){ return uniformModel; }
    GLuint GetViewLocation(){ return uniformView; }
    GLuint GetShaderID() { return shaderID; }

    void UseShader();
    void ClearShader();

    ~Shader();

private:
    GLuint shaderID, uniformProjection, uniformModel, uniformView;

    void CompileShader(const char* vertexCode, const char* fragmentCode, const char* geometricCode = "");
    void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

