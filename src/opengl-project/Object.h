#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaderClass.h"
#include "EBO.h"
#include "VBO.h"
#include "VAO.h"

class Object
{
public:
    float* vertices;
    unsigned int* indices;
    bool isLight;
    int vertexCount;
    int indexCount;
    std::string name;

    Shader& shaderProgram;
    VAO VAO1;
    VBO VBO1;
    EBO EBO1;
    GLuint uniID;
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 objectScale = glm::vec3(1.0f);

    Object(std::string name, float vertexArray[], int vertexArraySize, unsigned int indexArray[], int indexArraySize, Shader& shader, bool isLight);

    void Move(float x, float y, float z);
    void Rotate(float x, float y, float z);
    void SetScale(float amount);

    ~Object();
};
