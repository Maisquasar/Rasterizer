#pragma once
#include <cstdio>
#include <imgui.h>

#include <glad/gl.h>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <Framebuffer.hpp>
#include <prng.hpp>

//Structure d'initialisation de l'app
struct AppInit
{
    int width;
    int height; 
    const char* title;
    void (*ErrorCallback)(int errorCode, const char* description);
    void (*DebugMessageCallback)(GLenum source, GLenum type, GLuint id, GLenum severity, 
                                GLsizei length, const GLchar* message, const void* userParam);
};

//Enum state of Init app
enum EAppState
{
    NONE = 0,
    GLFWFAILED = 1,
    WINDOWFAILED = 2,
    GLADFAILED = 3,
    SUCCESS = 4
};

//Unique Class of l'app
class App
{
private:
    GLFWwindow* window;
    EAppState state = NONE;
    char search[32]; //The search text in search bar.
    int index = 0;  //Index of the model.
    PRNG appRng;
public:
    App(const AppInit& p_init);
    ~App();

    void Update();
    inline GLFWwindow* GetWindow() {return window;}
    inline EAppState GetState() {return state;}

private:
    void NewFrame(bool mouseCaptured = true);
    void EndFrame();
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};

