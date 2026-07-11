#include "Object.h"

Object::Object(std::string name, float vertexArray[], int vertexArraySize,
    unsigned int indexArray[], int indexArraySize,
    Shader& shader, bool light)
    : vertices(vertexArray),
    indices(indexArray),
    vertexCount(vertexArraySize),
    indexCount(indexArraySize),
    shaderProgram(shader),
    VBO1(vertexArray, vertexArraySize * sizeof(float)),
    EBO1(indexArray, indexArraySize * sizeof(unsigned int)),
    isLight(light),
    name(name)
{
    VAO1.Bind();
    EBO1.Bind();

    // X,Y,Z Pos
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
    // R,G,B Color
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    // TexCoords
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    // Normal Vector
    VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));

    VAO1.Unbind();
    VBO1.Unbind();

    shaderProgram.Activate();

    uniID = glGetUniformLocation(shaderProgram.ID, "scale");

    glEnable(GL_DEPTH_TEST);
}


void Object::Move(float x, float y, float z)
{
    position.x += x;
    position.y += y;
    position.z += z;
}
void Object::Rotate(float x, float y, float z)
{
    rotation.x += x;
    rotation.y += y;
    rotation.z += z;
}
void Object::SetScale(float amount)
{
    objectScale = glm::vec3(amount);
}

Object::~Object()
{
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
}
