#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "Visualizer.hpp"

#include <memory>

BEGIN_VISUALIZER_NAMESPACE

class Camera;
struct VertexDataPosition3fColor3f;

struct Object
{
    uint32_t m_IndexCount;
    GLuint m_VAO, m_VBO, m_IBO;
};

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

    Object InitObj(std::vector<VertexDataPosition3fColor3f> vertices, const std::vector<uint32_t>& indices, const glm::vec3& translate = glm::vec3(0.0));
    void Initialize();
    void Render();
    void Cleanup();

    void UpdateViewport(uint32_t width, uint32_t height);
    void UpdateCamera();

private:
    std::vector<Object> m_objects;

    GLuint m_UBO;
    glm::mat4* m_UBOData = nullptr;

    GLuint m_ShaderProgram;
    std::shared_ptr<Camera> m_Camera;
};

END_VISUALIZER_NAMESPACE

#endif // !RENDERER_HPP
