#include "Renderer.h"
#include "Elements/Snake.h"

#include <glm/fwd.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "Graphics/Camera.h"
#include "Graphics/GraphicsUtils.h"
#include "Graphics/Themes.h"

#include "Map/Grid.h"
#include "Map/Cell.h"
#include "Map/GameMap.h"

#include "Pathfinding/Base/Path.h"

#include <SDL3/SDL_log.h>

Renderer::Renderer(Camera* camera): RendererBase(camera)
{
    _cubeRenderer.Init();

    _generalShader = GraphicsUtils::LoadShader("default.vert", "default.frag");
    if (_generalShader == 0)
    {
        SDL_Log("Failed to load shader program");
    }

    SwapTheme();
}

void Renderer::RenderBackground()
{
    glClearColor(_theme.BackgroundColor.r, _theme.BackgroundColor.g, _theme.BackgroundColor.b, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::RenderGrid(const Grid& grid)
{
    glDepthMask(GL_FALSE);
    std::vector<Cell*> cells = grid.GetCells();
    RenderCells(cells, glm::vec4(_theme.CellColor, 0.02f));
    glDepthMask(GL_TRUE);
}

void Renderer::RenderGameMap(const GameMap& gameMap)
{
    glUseProgram(_generalShader);

    glm::mat4 view = _camera->GetViewMatrix();
    glm::mat4 proj = _camera->GetProjectionMatrix();

    GLint locMVP = glGetUniformLocation(_generalShader, "uMVP");
    GLint locColor = glGetUniformLocation(_generalShader, "uColor");

    // Render snake body
    glUniform4f(locColor, _theme.SnakeBodyColor.r, _theme.SnakeBodyColor.g, _theme.SnakeBodyColor.b, 0.5f);

    for (const auto& segment : gameMap.GetSnake()->GetBody())
    {
        glm::vec3 worldPos = glm::vec3(segment) * 1.0f;
        glm::mat4 model = glm::translate(glm::mat4(1.0f), worldPos);
        model = glm::scale(model, glm::vec3(0.85f));
        glm::mat4 mvp = proj * view * model;
        glUniformMatrix4fv(locMVP, 1, GL_FALSE, &mvp[0][0]);
        _cubeRenderer.Draw();
    }

    glDepthMask(GL_FALSE);

    // Render snake head
    glm::vec3 headPos = glm::vec3(gameMap.GetSnake()->GetHeadLocation()) * 1.0f;
    glm::mat4 headModel = glm::translate(glm::mat4(1.0f), headPos);
    headModel = glm::scale(headModel, glm::vec3(0.9f));
    glm::mat4 headMvp = proj * view * headModel;
    glUniformMatrix4fv(locMVP, 1, GL_FALSE, &headMvp[0][0]);
    glUniform4f(locColor, _theme.SnakeHeadColor.r, _theme.SnakeHeadColor.g, _theme.SnakeHeadColor.b, 1.f);
    _cubeRenderer.Draw();

    // Render target location
    glm::vec3 targetPos = glm::vec3(gameMap.GetTargetLocation()) * 1.0f;
    glm::mat4 targetModel = glm::translate(glm::mat4(1.0f), targetPos);
    targetModel = glm::scale(targetModel, glm::vec3(0.8f));
    glm::mat4 targetMvp = proj * view * targetModel;
    glUniformMatrix4fv(locMVP, 1, GL_FALSE, &targetMvp[0][0]);
    glUniform4f(locColor, _theme.TargetColor.r, _theme.TargetColor.g, _theme.TargetColor.b, 1.f);
    _cubeRenderer.Draw();

    glDepthMask(GL_TRUE);
}

void Renderer::RenderPath(const Path& path)
{
    glDepthMask(GL_FALSE);

    glUseProgram(_generalShader);

    glm::mat4 view = _camera->GetViewMatrix();
    glm::mat4 proj = _camera->GetProjectionMatrix();

    GLint locMVP = glGetUniformLocation(_generalShader, "uMVP");
    GLint locColor = glGetUniformLocation(_generalShader, "uColor");

    for (int i = 0; i < path.Cells.size() - 1; ++i)
    {
        glm::vec3 start = glm::vec3(path.Cells[i]->GetGridPosition());
        glm::vec3 end = glm::vec3(path.Cells[i + 1]->GetGridPosition());

        glm::vec3 direction = end - start;
        float length = glm::length(direction);
        glm::vec3 center = (start + end) * 0.5f;

        glm::vec3 up = glm::vec3(0, 1, 0); // default cube orientation
        glm::mat4 rotation = glm::mat4(1.0f);
        if (glm::length(glm::cross(up, direction)) > 0.001f)
        {
            glm::vec3 axis = glm::normalize(glm::cross(up, direction));
            float angle = acos(glm::dot(glm::normalize(up), glm::normalize(direction)));
            rotation = glm::rotate(glm::mat4(1.0f), angle, axis);
        }

        glm::vec3 scale = glm::vec3(0.1f, length, 0.1f);
        glm::mat4 model = glm::translate(glm::mat4(1.0f), center);
        model = model * rotation;
        model = glm::scale(model, scale);
        glm::mat4 mvp = proj * view * model;
        glUniformMatrix4fv(locMVP, 1, GL_FALSE, &mvp[0][0]);
        glUniform4f(locColor, _theme.PathLineColor.r, _theme.PathLineColor.g, _theme.PathLineColor.b, 1.f);
        _cubeRenderer.Draw();
    }

    glDepthMask(GL_TRUE);
}

void Renderer::RenderHorizontalDirection(const std::vector<Cell*>& cells)
{
    RenderCells(cells, glm::vec4(_theme.DirectionColor, 0.05f));
}

void Renderer::RenderVerticalDirection(const std::vector<Cell*>& cells)
{
    RenderCells(cells, glm::vec4(_theme.VerticalDirectionColor, 0.05f));
}

void Renderer::RenderForwardDirection(const std::vector<Cell*>& cells)
{
    RenderCells(cells, glm::vec4(_theme.DirectionColor, 0.25f));
}

void Renderer::SwapTheme()
{
    _isLight = !_isLight;
    if (_isLight)
    {
        _theme = Themes::Light;
    }
    else
    {
        _theme = Themes::Dark;
    }
}

ImU32 Renderer::GetHUDColor()
{
    if (_isLight)
    {
        return IM_COL32(0, 0, 0, 255);
    }

    return IM_COL32(255, 255, 255, 255);
}

void Renderer::RenderCells(const std::vector<Cell*>& cells, glm::vec4 color)
{
    glUseProgram(_generalShader);

    glm::mat4 view = _camera->GetViewMatrix();
    glm::mat4 proj = _camera->GetProjectionMatrix();

    GLint locMVP = glGetUniformLocation(_generalShader, "uMVP");
    GLint locColor = glGetUniformLocation(_generalShader, "uColor");
    glUniform4f(locColor, color.r, color.g, color.b, color.a);

    for (Cell* const& cell : cells)
    {
        glm::vec3 worldPos = glm::vec3(cell->GetGridPosition());
        glm::mat4 model = glm::translate(glm::mat4(1.f), worldPos);
        model = glm::scale(model, glm::vec3(1.f));
        glm::mat4 mvp = proj * view * model;
        glUniformMatrix4fv(locMVP, 1, GL_FALSE, &mvp[0][0]);
        _cubeRenderer.Draw();
    }
}