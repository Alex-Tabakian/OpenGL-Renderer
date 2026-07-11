#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>


#include "shaderClass.h"
#include "Texture.h"
#include "EBO.h"
#include "VBO.h"
#include "VAO.h"
#include "Object.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>

const unsigned int sceneWidth = 800;
const unsigned int guiWidth = 300;
const unsigned int width = sceneWidth + guiWidth;
const unsigned int height = 800;

// Position              // Color
/*GLfloat vertices[] =
{
    //     COORDINATES        /        COLORS         /   TexCoord  //
    -0.5f, 0.0f,  0.5f,       0.83f, 0.70f, 0.44f,    0.0f, 0.0f,
    -0.5f, 0.0f, -0.5f,       0.83f, 0.70f, 0.44f,    5.0f, 0.0f,
     0.5f, 0.0f, -0.5f,       0.83f, 0.70f, 0.44f,    0.0f, 0.0f,
     0.5f, 0.0f,  0.5f,       0.83f, 0.70f, 0.44f,    5.0f, 0.0f,
     0.0f, 0.8f,  0.0f,       0.92f, 0.86f, 0.76f,    2.5f, 5.0f
};

// Indices for vertices order
GLuint indices[] =
{
    0, 1, 2,
    0, 2, 3,
    0, 1, 4,
    1, 2, 4,
    2, 3, 4,
    3, 0, 4
};*/
struct ObjData
{
    GLfloat* vertices = nullptr;
    GLuint* indices = nullptr;

    int vertexCount = 0;
    int indexCount = 0;
};



ObjData parseOBJ(const std::string& filePath, float r = 0.0f, float g = 0.0f, float b = 0.0f)
{

    std::ifstream objFile(filePath);
    ObjData data{};
    int vertexCount = 0;
    int indexCount = 0;

    if (!objFile.is_open())
    {
        std::cout << "Could not open file: " << filePath << '\n';
        return data;
    }

    std::vector<glm::vec3> normals;
    std::string line;

    while (std::getline(objFile, line))
    {
        std::stringstream split(line);
        std::string type;

        split >> type;
        if (type == "v")
            vertexCount += 11;
        else if (type == "vn")
        {
            glm::vec3 normal;
            split >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if (type == "f")
            indexCount += 3;
    }

    data.vertices = new GLfloat[vertexCount]{};
    data.indices = new GLuint[indexCount]{};
    data.vertexCount = vertexCount;
    data.indexCount = indexCount;

    objFile.clear();
    objFile.seekg(0, std::ios::beg);

    int vertexPosition = 0;
    int indexPosition = 0;
    int normalPosition = 0;

    //Format OBJ File

    /*
        v 0.0 0.0 0.0           X,Y,Z of Triangle
        v 0.0 1.0 0.0
        ...
        vn 1.0 0.0 0.0          Normal Vectors
        vn -1.0 0.0 0.0
        ...
        f 3//1 7//1 8//1        Order of drawing trianges (Ignore second value)
        f 3//1 8//1 4//1
    */
    while (std::getline(objFile, line))
    {
        std::stringstream split(line);
        std::string type;

        split >> type;

        if (type == "v")
        {
            GLfloat x, y, z;
            split >> x >> y >> z;

            // X,Y,Z
            data.vertices[vertexPosition++] = x;
            data.vertices[vertexPosition++] = y;
            data.vertices[vertexPosition++] = z;

            // Color of vertex
            data.vertices[vertexPosition++] = r;
            data.vertices[vertexPosition++] = g;
            data.vertices[vertexPosition++] = b;

            //0,0 For TexCoords
            data.vertices[vertexPosition++] = 0.0f;
            data.vertices[vertexPosition++] = 0.0f;

            glm::vec3 normal = normalPosition < static_cast<int>(normals.size())
                ? normals[normalPosition++]
                : glm::vec3(0.0f, 1.0f, 0.0f);



            data.vertices[vertexPosition++] = normal.x;
            data.vertices[vertexPosition++] = normal.y;
            data.vertices[vertexPosition++] = normal.z;
        }
        else if (type == "f")
        {
            std::string a, b, c;
            split >> a >> b >> c;

            data.indices[indexPosition++] =
                std::stoi(a.substr(0, a.find('/'))) - 1;

            data.indices[indexPosition++] =
                std::stoi(b.substr(0, b.find('/'))) - 1;

            data.indices[indexPosition++] =
                std::stoi(c.substr(0, c.find('/'))) - 1;
        }
    }

    return data;
}


void draw(GLFWwindow* window, const std::vector<Object*>& objects, int width, int height) {

    // Main render loop
    while (!glfwWindowShouldClose(window))
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // BG color
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int framebufferWidth = 0;
        int framebufferHeight = 0;
        glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

        const int panelWidth = static_cast<int>(guiWidth);
        const int renderWidth = framebufferWidth > panelWidth ? framebufferWidth - panelWidth : framebufferWidth;
        glViewport(0, 0, renderWidth, framebufferHeight);

        // Loops through object list
        for (Object* object : objects)
        {
            object->shaderProgram.Activate();

            // 4x4 I matrix
            glm::mat4 model(1.0f);

            model = glm::translate(model, object->position);
            model = glm::scale(model, object->objectScale);

            model = glm::rotate(model, glm::radians(object->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(object->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(object->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

            glm::mat4 view(1.0f);
            glm::mat4 proj(1.0f);

            // 45 FOV, .1 Min view distance, 100 Max view distance
            proj = glm::perspective(glm::radians(45.0f), static_cast<float>(renderWidth) / static_cast<float>(framebufferHeight), 0.1f,100.0f);

            glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

            glUniformMatrix4fv(glGetUniformLocation(object->shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(glGetUniformLocation(object->shaderProgram.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(object->shaderProgram.ID, "proj"), 1, GL_FALSE, glm::value_ptr(proj));

            glUniform4f(glGetUniformLocation(object->shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
            glUniform1i(glGetUniformLocation(object->shaderProgram.ID, "isLight"), object->isLight);

            if (!object->isLight) {
                glm::vec3 lightPos = objects[0]->position;
                glUniform3f(glGetUniformLocation(object->shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
            }
                

            


            object->VAO1.Bind();

            glDrawElements(GL_TRIANGLES, object->indexCount, GL_UNSIGNED_INT, nullptr);
        }

        glViewport(0, 0, framebufferWidth, framebufferHeight);

        // GUI for object data

        ImGui::SetNextWindowPos(ImVec2(static_cast<float>(renderWidth), 0.0f));
        ImGui::SetNextWindowSize(ImVec2(static_cast<float>(panelWidth), static_cast<float>(framebufferHeight)));
        ImGui::Begin("Inspector", nullptr,
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoTitleBar);

        for (int i = 0; i < static_cast<int>(objects.size()); i++)
        {


            ImGui::PushID(i);

            ImGui::TextUnformatted(objects[i]->name.c_str());

            ImGui::InputFloat3(
                "Position",
                glm::value_ptr(objects[i]->position)
            );

            ImGui::InputFloat3(
                "Rotation",
                glm::value_ptr(objects[i]->rotation)
            );

            ImGui::InputFloat3(
                "Scale",
                glm::value_ptr(objects[i]->objectScale)
            );

            ImGui::PopID();
        }

        

        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}


int main()
{

    //wxIMPLEMENT_APP(App);

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    gladLoadGL();
    glViewport(0, 0, width, height);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


    
    Shader shaderProgram("default.vert", "default.frag");


    ObjData data1 = parseOBJ(R"(..\..\teapot.obj)", 0.8f, 0.0f, 0.2f);
    ObjData data2 = parseOBJ(R"(..\..\teapot.obj)", 0.0f, 1.0f, 1.0f);
    ObjData lightData = parseOBJ(R"(..\..\resourcefiles\objects\cube.obj)", 1.0f, 1.0f, 1.0f);

    if (data1.vertices == nullptr || data1.indices == nullptr ||
        lightData.vertices == nullptr || lightData.indices == nullptr)
    {
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    //Object obj(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]), shaderProgram);

    //Light Source

        
    std::vector<Object*> objects;
    //Light
    Object obj0("lightCube", lightData.vertices, lightData.vertexCount, lightData.indices, lightData.indexCount, shaderProgram, true);
    obj0.Move(1.8f, 1.8f, -4.0f);
    obj0.SetScale(0.15f);
    objects.push_back(&obj0);
        
    //Teapot
    Object obj1("cyanTeapot", data1.vertices, data1.vertexCount, data1.indices, data1.indexCount, shaderProgram, false);
    obj1.Move(0.0f, -2.5f, -8.0f);
    obj1.Rotate(0.0f, 45.0f, 0.0f);
    obj1.SetScale(0.75f);
    objects.push_back(&obj1);

    //Teapot2
    Object obj2("redTeapot", data2.vertices, data1.vertexCount, data1.indices, data1.indexCount, shaderProgram, false);
    obj2.Move(0.0f, 2.5f, -8.0f);
    obj2.Rotate(0.0f, -45.0f, 180.0f);
    obj2.SetScale(0.75f);
    objects.push_back(&obj2);

    //Horse
    /*Object obj2(data2.vertices, data2.vertexCount, data2.indices, data2.indexCount, shaderProgram, false);
    obj2.Move(0.0f, 1.0f, -7.0f);
    obj2.Rotate(-90.0f, 0.0f, 90.0f);
    obj2.SetScale(1.0f);
    objects.push_back(&obj2);*/

    draw(window, objects, sceneWidth, height);

    shaderProgram.Delete();
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);

    glfwTerminate();


    return 0;
} 
