// Copyright (c) Guillem Serra. All Rights Reserved.

#pragma once

class CubeRenderer
{
public:
    void Init();
    void Draw();
    void Cleanup();

private:
    unsigned int VAO = 0, VBO = 0, EBO = 0;
};