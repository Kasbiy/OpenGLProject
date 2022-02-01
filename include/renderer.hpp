#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Visualizer.hpp"

#include <memory>

BEGIN_VISUALIZER_NAMESPACE

class Camera;

class Renderer
{
public:
    Renderer(const std::shared_ptr<Camera>& camera)
        : m_Camera(camera)
    {}

    Renderer() = delete;
    ~Renderer() = default;
    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;

    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    void Initialize();
    void Render();
    void Cleanup();

    void UpdateViewport(uint32_t width, uint32_t height);
    void UpdateCamera();

private:
    uint16_t m_IndexCount;
    GLuint m_VAO, m_VBO, m_IBO, m_UBO, m_ShaderProgram;

    glm::mat4* m_UBOData = nullptr;

    std::shared_ptr<Camera> m_Camera;
};

END_VISUALIZER_NAMESPACE

#endif // !RENDERER_HPP
