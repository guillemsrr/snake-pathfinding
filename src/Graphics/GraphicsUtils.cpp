﻿#include "GraphicsUtils.h"
#include <fstream>
#include <sstream>
#include <iostream>

#include <SDL3/SDL_log.h>

#ifndef SHADER_DIR
#define SHADER_DIR "/Shaders"
#endif

GLuint GraphicsUtils::LoadShader(const std::string& vertexPath, const std::string& fragmentPath)
{
    std::string fullVertexPath = std::string(SHADER_DIR) + "/" + vertexPath;
    std::string fullFragmentPath = std::string(SHADER_DIR) + "/" + fragmentPath;

    std::ifstream vFile(fullVertexPath), fFile(fullFragmentPath);
    if (!vFile.is_open() || !fFile.is_open())
    {
        SDL_Log("Failed to open shader files");
        return 0;
    }

    std::stringstream vStream, fStream;
    vStream << vFile.rdbuf();
    fStream << fFile.rdbuf();

    std::string vertexCode = vStream.str();
    std::string fragmentCode = fStream.str();

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);

    GLint success;
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
        SDL_Log("Vertex shader compile error: %s", infoLog);
    }

    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
        SDL_Log("Fragment shader compile error: %s", infoLog);
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        SDL_Log("Shader linking error: %s", infoLog);
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}