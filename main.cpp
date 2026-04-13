#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

#include "Camera.h"
#include "CubeCollider.h"
#include "GameObject.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Shader.h"
#include "core/mesh.h"
#include "core/assimpLoader.h"
#include "core/texture.h"

//#define MAC_CLION
#define VSTUDIO

#ifdef MAC_CLION
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#endif

#ifdef VSTUDIO
// Note: install imgui with:
//     ./vcpkg.exe install imgui[glfw-binding,opengl3-binding]
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#endif

int g_width = 1600;
int g_height = 900;

int SEED = 0;

Camera camera(25,15,80);

SceneManager sceneManager;
Scene scene("Sample Scene", &camera);
Scene scene1("Scene2", &camera);
Scene scene2("Scene3", &camera);

Scene collision_detection_scene("Collision Detection Scene", &camera);

//Camera camera1(70, g_width/g_height, 0.1f, 200.0f);
GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);

void processInput(GLFWwindow *window) {

    glfwSetCursor(window, cursor);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse)
    {
        camera.firstMouse = true;
        return;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        if (camera.firstMouse)
        {
            camera.lastX = static_cast<float>(xpos);
            camera.lastY = static_cast<float>(ypos);
            camera.firstMouse = false;
            return;
        }

        auto xoffset = static_cast<float>(xpos - camera.lastX);
        auto yoffset = static_cast<float>(camera.lastY - ypos);

        camera.lastX = static_cast<float>(xpos);
        camera.lastY = static_cast<float>(ypos);

        camera.ProcessMouseMovement(xoffset, yoffset);
    }
    else
    {
        camera.firstMouse = true;
    }
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    g_width = width;
    g_height = height;
    glViewport(0, 0, width, height);
}

std::string readFileToString(const std::string &filePath) {
    std::ifstream fileStream(filePath, std::ios::in);
    if (!fileStream.is_open()) {
        printf("Could not open file: %s\n", filePath.c_str());
        return "";
    }
    std::stringstream buffer;
    buffer << fileStream.rdbuf();
    return buffer.str();
}

GLuint generateShader(const std::string &shaderPath, GLuint shaderType) {
    printf("Loading shader: %s\n", shaderPath.c_str());
    const std::string shaderText = readFileToString(shaderPath);
    const GLuint shader = glCreateShader(shaderType);
    const char *s_str = shaderText.c_str();
    glShaderSource(shader, 1, &s_str, nullptr);
    glCompileShader(shader);
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        printf("Error! Shader issue [%s]: %s\n", shaderPath.c_str(), infoLog);
    }
    return shader;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // sceneManager.AddScene(&scene);
    // sceneManager.AddScene(&scene1);
    // sceneManager.AddScene(&scene2);
    sceneManager.AddScene(&collision_detection_scene);


    //camera.transform.position = glm::vec3(0, 0, 10);
    //glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

    GLFWwindow *window = glfwCreateWindow(g_width, g_height, "Pengine", nullptr, nullptr);
    if (window == nullptr) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        printf("Failed to initialize GLAD\n");
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    //Setup platforms
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 400");

    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // core::Mesh quad = core::Mesh::generateQuad();
    // core::Model quadModel({quad});
    // quadModel.translate(glm::vec3(0,0,-2.5));
    // quadModel.scale(glm::vec3(5, 5, 1));

    Shader edgeDetectionShader("shaders/modelVertex.vs", "shaders/edgeDetection.fs");
    Shader colorInversionShader("shaders/modelVertex.vs", "shaders/colorInversion.fs");
    Shader pixelationShader("shaders/modelVertex.vs", "shaders/pixelation.fs");
    Shader litShader("shaders/modelVertex.vs", "shaders/litShader.fs");
    Shader modelShader("shaders/modelVertex.vs", "shaders/fragment.fs");
    Shader colliderShader("shaders/collider.vs", "shaders/collider.fs");

    GameObject edgeDetectionQuad(core::Model({core::Mesh::generateQuad()}), edgeDetectionShader.ID);
    GameObject colorInversionQuad(core::Model({core::Mesh::generateQuad()}), colorInversionShader.ID);
    GameObject pixelationQuad(core::Model({core::Mesh::generateQuad()}), pixelationShader.ID);
    edgeDetectionQuad.useTexture = false;
    colorInversionQuad.useTexture = false;
    pixelationQuad.useTexture = false;
    // quadMod.transform.position = glm::vec3(0,0,-2.5f);
    // quadMod.transform.scale = glm::vec3(5,5,1);

    core::Texture grassTexture = core::Texture("textures/grass_texture.jpg");

    GameObject suzanne(core::AssimpLoader::loadModel("models/nonormalmonkey.obj"), litShader.ID);
    suzanne.transform.position = glm::vec3(0,0.5f,0);

    GameObject suzanne1(core::AssimpLoader::loadModel("models/nonormalmonkey.obj"), litShader.ID);
    suzanne1.transform.position = glm::vec3(0,0.5f,18.0f);
    suzanne1.transform.rotation = glm::vec3(0,30.0f,10.0f);

    GameObject suzanne2(core::AssimpLoader::loadModel("models/nonormalmonkey.obj"), litShader.ID);
    suzanne2.transform.position = glm::vec3(-3,0.5f,3.0f);

    GameObject suzanne3(core::AssimpLoader::loadModel("models/nonormalmonkey.obj"), litShader.ID);
    suzanne3.transform.position = glm::vec3(-7,0.5f,8.0f);

    GameObject suzanne4(core::AssimpLoader::loadModel("models/nonormalmonkey.obj"), litShader.ID);
    suzanne4.transform.position = glm::vec3(4,0.5f,12.0f);

    // GameObject car(core::AssimpLoader::loadModel("models/NissanS30.obj"), litShader.ID);
    // car.transform.position = glm::vec3(0.0f,0.0f,-5.0f);

    GameObject plane(core::AssimpLoader::loadModel("models/Plane.obj"), grassTexture, litShader.ID);
    plane.transform.position = glm::vec3(0.0f,-5.0f,-5.0f);
    plane.transform.scale = glm::vec3(5.0f,1.0f,5.0f);

    Light light(0,0,20, glm::vec4(1,1,1,1), 10, modelShader.ID);
    light.transform.scale *= 0.1;

    SEED = time(nullptr) ^ (uintptr_t)&SEED;
    srand(SEED);

    printf("Seed: %i\n", SEED);

    int cube_count = 200;
    std::vector<glm::vec3> speeds;

    for (int i = 0; i < cube_count; ++i)
    {
        float xPos = static_cast<float>(rand()) / RAND_MAX * 50.0f;
        float yPos = static_cast<float>(rand()) / RAND_MAX * 25.0f;
        float zPos = static_cast<float>(rand()) / RAND_MAX * 50.0f;

        float xRot = static_cast<float>(rand()) / RAND_MAX * 180.0f;
        float yRot = static_cast<float>(rand()) / RAND_MAX * 180.0f;
        float zRot = static_cast<float>(rand()) / RAND_MAX * 180.0f;

        float size = static_cast<float>(rand()) / RAND_MAX * 2.0f + 1.0f;

        auto* collider = new CubeCollider(glm::vec3(xPos, yPos, zPos), glm::vec3(xRot,yRot,zRot), size, colliderShader.ID);
        collision_detection_scene.AddCollider(collider);

        float speed = static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;
        speeds.push_back(glm::vec3(speed));
    }

    for (int i = 0; i < cube_count; i++)
    {
        for (int j = i + 1; j < cube_count; j++)
        {
            if (collision_detection_scene.GetColliders()[i]->intersects(*collision_detection_scene.GetColliders()[j]))
            {
                printf("[Collider %i] [Collider %i]", i, j);
            }
        }
    }



    scene.AddObject(&suzanne);
    scene.AddObject(&suzanne1);
    scene.AddObject(&suzanne2);
    scene.AddObject(&suzanne3);
    scene.AddObject(&suzanne4);
    scene.AddObject(&light);
    scene.AddLight(&light);
    //scene.AddObject(&quadMod);

    scene1.AddObject(&suzanne1);

    scene2.AddLight(&light);
    scene2.AddObject(&light);
    //scene2.AddObject(&car);
    scene2.AddObject(&plane);



    //edge detection buffers
    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, g_width, g_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, g_width, g_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    //color inversion buffers
    unsigned int ciframebuffer;
    glGenFramebuffers(1, &ciframebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, ciframebuffer);

    unsigned int citexture;
    glGenTextures(1, &citexture);
    glBindTexture(GL_TEXTURE_2D, citexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, g_width, g_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, citexture, 0);

    unsigned int cirbo;
    glGenRenderbuffers(1, &cirbo);
    glBindRenderbuffer(GL_RENDERBUFFER, cirbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, g_width, g_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, cirbo);

    //pixelation buffers
    unsigned int pixelationFramebuffer;
    glGenFramebuffers(1, &pixelationFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, pixelationFramebuffer);

    unsigned int pixelationTexture;
    glGenTextures(1, &pixelationTexture);
    glBindTexture(GL_TEXTURE_2D, pixelationTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, g_width, g_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pixelationTexture, 0);

    unsigned int pixelationRBO;
    glGenRenderbuffers(1, &pixelationRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, pixelationRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, g_width, g_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, pixelationRBO);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //core::Model suzanne = core::AssimpLoader::loadModel("models/nonormalmonkey.obj");

    //core::Texture cmgtGatoTexture("textures/CMGaTo_crop.png");
    //GameObject cat(core::AssimpLoader::loadModel(("textures/CMGaTo_crop.png")), textureShaderProgram);

    glm::vec4 clearColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    glClearColor(clearColor.r,
                 clearColor.g, clearColor.b, clearColor.a);

#pragma region Uniforms
    GLint cameraPosUniform = glGetUniformLocation(litShader.ID, "cameraPos");
#pragma endregion

    double currentTime = glfwGetTime();
    double finishFrameTime = 0.0;
    float deltaTime = 0.0f;
    float cameraSpeed = 10.0f;
    float cameraRotationSpeed = 80.0f;
    float cameraFOV = 70.0f;
    float monkeyRot = 0.0f;

    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    float ambientIntensity = 0.0f;
    glm::vec3 ambientColor = {0.47f, 0.47f, 0.47f};
    glm::vec3 diffuseColor = {0.2f, 0.2f, 0.2f};
    glm::vec3 specularColor = {1, 1, 1};
    float specularPower = 8.0f;

    glm::vec3 newLightPos = glm::vec3(0,0,20);
    glm::vec4 newLightColor = glm::vec4(1, 0.1f, 0.5f, 1);
    float newLightRadius = 10.0f;

    bool useEdgeDetection = false;
    bool useInvertColors = false;
    bool usePixelation = false;

    float thickness = 1.0f;
    float clarity = 1.0f;

    float pixelSize = 16.0f;

    bool showWireframe = false;

    glm::vec3 materialAmbient = glm::vec3(1.0f, 0.5f, 0.31f);
    glm::vec3 materialDiffuse = glm::vec3(1.0f, 0.5f, 0.31f);
    glm::vec3 materialSpecular = glm::vec3(0.5f, 0.5f, 0.5f);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glfwSwapInterval(0); //VSYNC 1=on 0=off

        if (useEdgeDetection) {
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        }
        else if (useInvertColors) {
            glBindFramebuffer(GL_FRAMEBUFFER, ciframebuffer);
        }
        else if (usePixelation)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pixelationFramebuffer);
        }
        else {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        suzanne.transform.rotation = glm::vec3(0,monkeyRot,0);
        light.setColor(lightColor);

        Scene* currentScene = sceneManager.getCurrentScene();
        currentScene->Update(deltaTime);

        camera.ProcessMovementInput(window, deltaTime, cameraSpeed);
        //camera.ProcessMouseInput(window,deltaTime, cameraRotationSpeed);
        camera.fov = cameraFOV;

        suzanne1.transform.Rotate(glm::vec3(0,1,0), 360 * deltaTime);

        for (auto* c : collision_detection_scene.GetColliders())
            c->is_intersecting = false;

        for (int i = 0; i < cube_count; i++)
        {
            for (int j = i + 1; j < cube_count; j++)
            {
                if (collision_detection_scene.GetColliders()[i]->intersects(*collision_detection_scene.GetColliders()[j]))
                {
                    collision_detection_scene.GetColliders()[i]->is_intersecting = true;
                    collision_detection_scene.GetColliders()[j]->is_intersecting = true;
                    //printf("[Collider %i] [Collider %i]", i, j);
                }
            }
        }

        //VP
        glm::mat4 view = camera.getViewMatrix();
        glm::mat4 projection = camera.getProjectionMatrix();

        litShader.Use();

        //Take a look
        //litShader.setMat4("mvpMatrix", projection * view * suzanne.model.getModelMatrix());
        //glUniformMatrix4fv(litMvpUniform, 1, GL_FALSE, glm::value_ptr(projection * view * suzanne.model.getModelMatrix()));
        //glUniform3f(lightDirUniform,1.0f,0.5f,0.0f);

        litShader.setFloat("ambientIntensity", ambientIntensity);
        litShader.setVec4("lightColor", lightColor);

        litShader.setInt("lightCount", currentScene->GetLightCount());

        for (int i = 0; i < currentScene->GetLightCount(); i++)
        {
            litShader.setVec3("lights[" + std::to_string(i) + "].position", currentScene->GetLights()[i]->transform.position);
            litShader.setVec4("lights[" + std::to_string(i) + "].color", currentScene->GetLights()[i]->getColor());
            litShader.setFloat("lights[" + std::to_string(i) + "].radius",currentScene->GetLights()[i]->radius);
        }
        litShader.setVec3("ambientColor", ambientColor);
        litShader.setVec3("diffuseColor", diffuseColor);
        litShader.setVec3("speculaColor", specularColor);
        litShader.setFloat("specularIntensity", specularPower);

        glUniform3f(cameraPosUniform, camera.transform.position.x, camera.transform.position.y, camera.transform.position.z);

        litShader.setVec3("material.ambient", materialAmbient);
        litShader.setVec3("material.diffuse", materialDiffuse);
        litShader.setVec3("material.specular", materialSpecular);

        colliderShader.Use();
        currentScene->Render();

        for (int i = 0; i < currentScene->GetColliders().size(); i++)
        {
            currentScene->GetColliders()[i]->transform.Translate(speeds[i] * deltaTime);
        }

        // for (auto collider : currentScene->GetColliders())
        // {
        //     collider->transform.Translate(speeds[] * deltaTime);
        // }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        if (useEdgeDetection) {
            glDisable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT);
            edgeDetectionShader.Use();
            edgeDetectionShader.setFloat("thickness", thickness);
            edgeDetectionShader.setFloat("clarity", clarity);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            edgeDetectionQuad.Render(glm::mat4(1.0f));
        }
        else if (useInvertColors) {
            glDisable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT);
            colorInversionShader.Use();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, citexture);
            colorInversionQuad.Render(glm::mat4(1.0f));
        }
        else if (usePixelation)
        {
            glDisable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT);
            pixelationShader.Use();
            pixelationShader.setFloat("pixelSize", pixelSize);
            pixelationShader.setVec2("screenSize", glm::vec2(static_cast<float>(g_width), static_cast<float>(g_height)));
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, pixelationTexture);
            pixelationQuad.Render(glm::mat4(1.0f));
        }

        if (showWireframe)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        #pragma region ImGui

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Easy Collision Check");
        ImGui::SliderFloat3("Cube1 Position", glm::value_ptr(collision_detection_scene.GetColliders()[0]->transform.position), 1.0f, 100.0f);
        ImGui::SliderFloat3("Cube1 Rotation", glm::value_ptr(collision_detection_scene.GetColliders()[0]->transform.rotation), 1.0f, 180.0f);

        ImGui::SliderFloat3("Cube2 Position", glm::value_ptr(collision_detection_scene.GetColliders()[1]->transform.position), 1.0f, 100.0f);
        ImGui::SliderFloat3("Cube2 Rotation", glm::value_ptr(collision_detection_scene.GetColliders()[1]->transform.rotation), 1.0f, 180.0f);
        ImGui::End();

        ImGui::Begin("Wireframe");
        if (ImGui::Checkbox("Enable Wireframe", &showWireframe)) {
        }
        ImGui::End();

        ImGui::Begin("Post-Processing");

        if (ImGui::Checkbox("Edge Detection", &useEdgeDetection)) {
        }
        if (useEdgeDetection)
        {
            ImGui::SliderFloat("Line Thickness", &thickness, 1.0f, 10.0f);
            ImGui::SliderFloat("Line Clarity", &clarity, 1.0f, 10.0f);
        }
        if (ImGui::Checkbox("Invert Colors", &useInvertColors)) {
        }
        if (ImGui::Checkbox("Pixelate", &usePixelation)) {
        }
        if (usePixelation)
        {
            ImGui::SliderFloat("Pixle Size", &pixelSize, 4.0f, 32.0f);
        }
        ImGui::End();

        ImGui::Begin(currentScene->name.c_str());
        if (ImGui::Button("Next Scene")) {
            sceneManager.NextScene();
        };
        ImGui::SameLine();
        if (ImGui::Button("Previous Scene")) {
            sceneManager.PreviousScene();
        };
        ImGui::Text("Seed: %i", SEED);
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Begin("Camera Settings");
        ImGui::SliderFloat("Camera Speed", &cameraSpeed, 1.0f, 30.0f);
        ImGui::SliderFloat("Camera Rotation Speed", &cameraRotationSpeed, 1.0f, 100.0f);
        ImGui::SliderFloat("Camera FOV", &cameraFOV, 20.0f, 120.0f);
        ImGui::SliderFloat("Monkey Rot", &monkeyRot, 0.0f, 360.0f);
        ImGui::End();

        ImGui::Begin("Light Settings");

        ImGui::SliderFloat3("New Light Position", glm::value_ptr(newLightPos), -50.0f, 50.0f);
        ImGui::ColorEdit4("New Light Color", glm::value_ptr(newLightColor));
        ImGui::SliderFloat("New Light Radius", &newLightRadius, 1.0f, 50.0f);

        if (ImGui::Button("Add Light") && currentScene->GetLightCount() < 10) {

            auto newLight = new Light(newLightPos.x,newLightPos.y,newLightPos.z, newLightColor, newLightRadius, modelShader.ID);
            currentScene->AddLight(newLight);
            currentScene->AddObject(newLight);

            newLight->transform.scale *= 0.1;

            for (int i = 0; i < currentScene->GetLightCount(); i++) {
                litShader.setVec3("lights[" + std::to_string(i) + "].position", currentScene->GetLights()[i]->transform.position);
                litShader.setVec4("lights[" + std::to_string(i) + "].color", currentScene->GetLights()[i]->getColor());
                litShader.setFloat("lights[" + std::to_string(i) + "].radius",currentScene->GetLights()[i]->radius);
            }

            printf("Light was added \n");
        }
        ImGui::ColorEdit3("Light Color",&lightColor[0]);
        ImGui::SliderFloat("Ambient Intensity", &ambientIntensity, -1.0f, 1.0f);
        ImGui::ColorEdit3("Ambient Color", &ambientColor[0]);
        ImGui::ColorEdit3("Diffuse Color", &diffuseColor[0]);
        ImGui::ColorEdit3("Specular Color", &specularColor[0]);
        ImGui::SliderFloat("Spec Power", &specularPower, 1.0f, 256.0f);
        ImGui::SliderFloat("Light Radius", &light.radius, 1.0f, 50.0f);
        ImGui::SliderFloat3("Light Position", glm::value_ptr(light.transform.position), -20.0f, 30.0f);
        //ImGui::SliderFloat3("Camera rotation", rotations, 0.0f, 360.0f);
        ImGui::End();

        ImGui::Begin("Material Settings");
        ImGui::ColorEdit3("Ambient Color", &materialAmbient[0]);
        ImGui::ColorEdit3("Diffuse Color", &materialDiffuse[0]);
        ImGui::ColorEdit3("Specular Color", &materialSpecular[0]);
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        #pragma endregion ImGui


        glfwSwapBuffers(window);
        glfwPollEvents();
        finishFrameTime = glfwGetTime();
        deltaTime = static_cast<float>(finishFrameTime - currentTime);
        currentTime = finishFrameTime;
    }

    glDeleteProgram(modelShader.ID);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glDeleteRenderbuffers(1, &rbo);
    glDeleteRenderbuffers(1, &cirbo);
    glDeleteRenderbuffers(1, &pixelationRBO);
    glDeleteFramebuffers(1, &framebuffer);
    glDeleteFramebuffers(1, &ciframebuffer);
    glDeleteFramebuffers(1, &pixelationFramebuffer);

    glfwTerminate();
    return 0;
}