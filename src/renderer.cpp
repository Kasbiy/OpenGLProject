#include <GL/glew.h>

#define TINYOBJLOADER_IMPLEMENTATION

#pragma warning(push, 0)
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#pragma warning(pop, 0)

#include <vector>
#include <string>
#include <iostream>

#include "tinyobjloader/tiny_obj_loader.h"
#include "camera.hpp"
#include "renderer.hpp"

BEGIN_VISUALIZER_NAMESPACE

struct VertexDataPosition3fColor3f
{
    glm::vec3 position;
    glm::vec3 color;
};

void GenerateSphereMesh(std::vector<VertexDataPosition3fColor3f>& vertices, std::vector<uint16_t>& indices, uint16_t sphereStackCount, uint16_t sphereSectorCount, glm::vec3 sphereCenter, float sphereRadius)
{
    std::size_t vertexId = 0;

    for (uint16_t i = 0; i <= sphereStackCount; ++i)
    {
        const float t = static_cast<float>(i) / static_cast<float>(sphereStackCount);

        const float stackAngle = glm::pi<float>() / 2.0f - t * glm::pi<float>();

        const float nxz = glm::cos(stackAngle);
        const float ny = glm::sin(stackAngle);

        const float xz = sphereRadius * nxz;
        const float y = sphereRadius * ny;

        const float mul = stackAngle < 0.0f ? -1.0f : 1.0f;

        const float dxz = -ny * mul;

        // When stackAngle is null dxz is equal to 0 and it is impossible to compute the tangents and bitangents
        // So we use the up vector as a bitangent
        const bool useUpVector = (sphereStackCount % 2 == 0) && (i == sphereStackCount / 2);

        for (uint16_t j = 0; j <= sphereSectorCount; ++j)
        {
            const float s = static_cast<float>(j) / static_cast<float>(sphereSectorCount);

            const float sectorAngle = s * 2.0f * glm::pi<float>();

            const float csa = glm::cos(sectorAngle);
            const float ssa = glm::sin(sectorAngle);

            const float nx = nxz * csa;
            const float nz = nxz * ssa;

            const float x = xz * csa;
            const float z = xz * ssa;

            const glm::vec3 direction = glm::vec3(x, y, z);

            const glm::vec3 position = sphereCenter + direction;

            const glm::vec2 texcoords = glm::vec2(s, t);

            const glm::vec3 normal = glm::vec3(nx, ny, nz);

            const glm::vec3 color = glm::vec3(0.5f) + 0.5f * glm::normalize(direction);

            vertices[vertexId++] = { position, color };

            if (useUpVector)
            {
                const glm::vec3 bitangent = glm::vec3(0.0f, 0.0f, 1.0f);
                const glm::vec3 tangent = glm::cross(normal, bitangent);
            }
            else
            {
                const glm::vec3 tangent = glm::normalize(glm::vec3(dxz * -glm::sin(sectorAngle), dxz * glm::cos(sectorAngle), 0.0f));
                const glm::vec3 bitangent = glm::cross(tangent, normal);
            }
        }
    }

    const uint16_t sectorCountplusOne = sphereSectorCount + 1;

    std::size_t indexID = 0;

    for (uint16_t j = 0; j < sphereSectorCount; ++j)
    {
        const uint16_t jp1 = j + 1;

        indices[indexID++] = j;
        indices[indexID++] = sectorCountplusOne + j;
        indices[indexID++] = sectorCountplusOne + jp1;
    }

    for (uint16_t i = 1; i < sphereStackCount - 1; ++i)
    {
        const uint16_t k1 = i * sectorCountplusOne;
        const uint16_t k2 = k1 + sectorCountplusOne;

        for (uint16_t j = 0; j < sphereSectorCount; ++j)
        {
            const uint16_t jp1 = j + 1;

            indices[indexID++] = k1 + j;
            indices[indexID++] = k2 + j;
            indices[indexID++] = k1 + jp1;

            indices[indexID++] = k1 + jp1;
            indices[indexID++] = k2 + j;
            indices[indexID++] = k2 + jp1;
        }
    }

    const uint16_t k1 = (sphereStackCount - 1) * sectorCountplusOne;
    const uint16_t k2 = k1 + sectorCountplusOne;

    for (uint16_t j = 0; j < sphereSectorCount; ++j)
    {
        const uint16_t jp1 = j + 1;

        indices[indexID++] = k1 + j;
        indices[indexID++] = k2 + j;
        indices[indexID++] = k1 + jp1;
    }
}

void LoadSceneMesh(std::vector<VertexDataPosition3fColor3f> &vertices, std::vector<uint32_t> &indices)
{
    tinyobj::ObjReader reader;
    std::string inputfile = "C:/Users/Charles Polart/Documents/EpitechOpenGL/OpenGLProject/obj/dragon.obj";
    tinyobj::ObjReaderConfig reader_config;

    reader_config.mtl_search_path = "C:/Users/Charles Polart/Documents/EpitechOpenGL/OpenGLProject/obj/";

    if (!reader.ParseFromFile(inputfile, reader_config))
    {
        if (!reader.Error().empty())
        {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    if (!reader.Warning().empty())
    {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    const std::vector<tinyobj::shape_t> &shapes = reader.GetShapes();
    const tinyobj::attrib_t &attrib = reader.GetAttrib();
    size_t indices_size = 0;

    for (size_t s = 0; s < shapes.size(); ++s)
    {
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); ++f)
        {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            for (size_t v = 0; v < fv; ++v)
            {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[fv * size_t(idx.vertex_index) + 0];
                tinyobj::real_t vy = attrib.vertices[fv * size_t(idx.vertex_index) + 1];
                tinyobj::real_t vz = attrib.vertices[fv * size_t(idx.vertex_index) + 2];
                glm::vec3 position(vx, vy, vz);
                glm::vec3 color(0.8, 0.8, 0.8);
                vertices.push_back(visualizer::VertexDataPosition3fColor3f{position, color});
            }
            index_offset += fv;
            indices_size += fv;
        }
    }
    for (size_t i = 0; i < indices_size; ++i)
    {
        indices.push_back((uint32_t)i);
    }
}

void Renderer::Initialize()
{
    std::vector<VertexDataPosition3fColor3f> vertices;
    std::vector<uint32_t> indices;

    LoadSceneMesh(vertices, indices);

    const uint32_t vertexCount = vertices.size();
    const uint32_t indexCount = indices.size();

    m_IndexCount = indexCount;

    glCreateBuffers(1, &m_UBO);
    glNamedBufferStorage(m_UBO, sizeof(glm::mat4), glm::value_ptr(m_Camera->GetViewProjectionMatrix()), GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);
    m_UBOData = reinterpret_cast<glm::mat4*>(glMapNamedBufferRange(m_UBO, 0, sizeof(glm::mat4), GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT));

    glCreateBuffers(1, &m_VBO);
    glNamedBufferStorage(m_VBO, sizeof(VertexDataPosition3fColor3f) * vertexCount, vertices.data(), 0);

    glCreateBuffers(1, &m_IBO);
    glNamedBufferStorage(m_IBO, sizeof(uint32_t) * indexCount, indices.data(), 0);

    glCreateVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexDataPosition3fColor3f), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexDataPosition3fColor3f), reinterpret_cast<GLvoid*>(sizeof(glm::vec3)));

    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    m_ShaderProgram = glCreateProgram();

    glAttachShader(m_ShaderProgram, vShader);
    glAttachShader(m_ShaderProgram, fShader);

    {
        char const* const vertexShader =
            R"(#version 450 core

layout(location = 0) in vec3 inWorldPos;
layout(location = 1) in vec3 inColor;

layout(location = 0) smooth out vec3 color;

layout(std140, binding = 0) uniform Matrix
{
    mat4 modelViewProjection;
};

void main()
{
    color = inColor;
    gl_Position = modelViewProjection*vec4(inWorldPos, 1.);
}
)";

        glShaderSource(vShader, 1, &vertexShader, nullptr);

        glCompileShader(vShader);

        {
            GLint length = 0;

            glGetShaderiv(vShader, GL_INFO_LOG_LENGTH, &length);

            if (length > 1)
            {
                std::string log(length, '\0');

                glGetShaderInfoLog(vShader, length, nullptr, log.data());

                std::cerr << "Vertex shader log:\n" << log << '\n';
            }
        }

        char const* const fragmentShader =
            R"(#version 450 core

layout(location = 0) out vec4 outColor;

layout(location = 0) smooth in vec3 color;

void main()
{
    outColor = vec4(color, 1.0);
}
)";

        glShaderSource(fShader, 1, &fragmentShader, nullptr);

        glCompileShader(fShader);

        {
            GLint length = 0;

            glGetShaderiv(fShader, GL_INFO_LOG_LENGTH, &length);

            if (length > 1)
            {
                std::string log(length, '\0');

                glGetShaderInfoLog(fShader, length, nullptr, log.data());

                std::cerr << "Vertex shader log:\n" << log << '\n';
            }
        }
    }

    glLinkProgram(m_ShaderProgram);

    {
        GLint length = 0;

        glGetProgramiv(m_ShaderProgram, GL_INFO_LOG_LENGTH, &length);

        if (length > 1)
        {
            std::string log(length, '\0');

            glGetProgramInfoLog(m_ShaderProgram, length, nullptr, log.data());

            std::cerr << "Shader program log:\n" << log << '\n';
        }
    }

    glDetachShader(m_ShaderProgram, vShader);
    glDetachShader(m_ShaderProgram, fShader);

    glDeleteShader(vShader);
    glDeleteShader(fShader);
}

void Renderer::Render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_UBO, 0, sizeof(glm::mat4));
    glUseProgram(m_ShaderProgram);
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glUseProgram(0);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, 0, 0, 0);
}

void Renderer::Cleanup()
{
    glUnmapNamedBuffer(m_UBO);

    glDeleteBuffers(1, &m_UBO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_IBO);
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteProgram(m_ShaderProgram);
}

void Renderer::UpdateViewport(uint32_t width, uint32_t height)
{
    (void)width;
    (void)height;

    glViewport(0, 0, width, height);
    UpdateCamera();
}

void Renderer::UpdateCamera()
{
    std::memcpy(m_UBOData, glm::value_ptr(m_Camera->GetViewProjectionMatrix()), sizeof(glm::mat4));
    glFlushMappedNamedBufferRange(m_UBO, 0, sizeof(glm::mat4));
}

END_VISUALIZER_NAMESPACE
