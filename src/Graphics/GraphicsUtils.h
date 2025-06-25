// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once

#include <string>
#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif

class GraphicsUtils
{
public:
    static GLuint LoadShader(const std::string& vertexPath, const std::string& fragmentPath);
};