//
// Created by jg03dev on 10/9/23.
//

#include "Shader.h"

Shader::Shader()
{
    shaderID = 0;
    uniformModel = 0;
    uniformProjection = 0;
}

void Shader::CreateFromString(const char *vertexCode, const char *fragmentCode, const char *geometricCode="")
{
    this->CompileShader(vertexCode, fragmentCode, geometricCode);
}


Shader::~Shader()
{
    ClearShader();
}

void Shader::UseShader()
{
    glUseProgram(shaderID);
}

void Shader::ClearShader()
{
    if (shaderID != 0)
    {
        glDeleteProgram(shaderID);
        shaderID = 0;
    }

    uniformModel = 0;
    uniformProjection = 0;
}

void Shader::CompileShader(const char *vertexCode, const char *fragmentCode, const char *geometricCode="")
{
    shaderID = glCreateProgram();

    if (!shaderID)
    {
        printf("Error creating shader program! \n");
        return;
    }

    AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
    AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);
    if (geometricCode != "")
        AddShader(shaderID, geometricCode, GL_GEOMETRY_SHADER);

    // Linking shaders
    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shaderID);
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("Error linking program '%s'\n", eLog);
        return;
    }

    // Validating shaders
    glValidateProgram(shaderID);
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("Error validating program '%s'\n", eLog);
        return;
    }

    //TODO: make sure ALL shaders have this or change Class structure
    uniformModel = glGetUniformLocation(shaderID, "model");
    uniformProjection = glGetUniformLocation(shaderID, "projection");
    uniformView = glGetUniformLocation(shaderID, "view");
}

void Shader::AddShader(GLuint theProgram, const char *shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = (GLint) strlen(shaderCode);

    glShaderSource(theShader, 1, theCode, codeLength);

    // Compiling shader
    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glCompileShader(theShader);
    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(theShader, sizeof(eLog), NULL, eLog);
        fprintf(stderr, "Error compiling the %d shader: '%s'\n", shaderType, eLog);
        return;
    }

    glAttachShader(theProgram, theShader);
}

//TODO: Change Locations to std::strings and add an optional geometry shader
void Shader::CreateFromFiles(std::string vertexLocation, std::string fragmentLocation, std::string geometricLocation="")
{
    std::string vertexString = ReadFile(vertexLocation.c_str());
    std::string fragmentString = ReadFile(fragmentLocation.c_str());
    std::string geometricString = "";
    if (geometricLocation != "")
        geometricString = ReadFile(geometricLocation.c_str());

    const char* vertexCode = vertexString.c_str();
    const char* fragmentCode =  fragmentString.c_str();
    const char* geometricCode = geometricString.c_str();

    CompileShader(vertexCode, fragmentCode);
}

std::string Shader::ReadFile(const char *fileLocation)
{
    std::string content;
    std::ifstream fileStream(fileLocation, std::ios::in);

    if(!fileStream.is_open())
    {
        printf("Failed to read %s! File doesn't exist.", fileLocation);
        return "";
    }

    std::string line = "";
    while(!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}
