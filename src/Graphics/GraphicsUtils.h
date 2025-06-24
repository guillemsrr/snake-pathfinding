// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once

#include <string>
#include <glad/glad.h>

class GraphicsUtils
{
public:
    static GLuint LoadShader(const std::string& vertexPath, const std::string& fragmentPath);
};