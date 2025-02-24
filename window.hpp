#pragma once
#include "headers.hpp"

class Window {
    GLFWwindow* const window;

  public:
    Window(int width = 640, int height = 480, const char* title = "Hello!")
        : window(glfwCreateWindow(width, height, title, NULL, NULL)) {
        if (window == NULL) {
            std::cerr << "Can't create GLFW window." << std::endl;
            exit(1);
        }
        glfwMakeContextCurrent(window);

        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK) {
            std::cerr << "Can't initialize GLEW" << std::endl;
            exit(1);
        }
        // wait for vsync
        glfwSwapInterval(1);

        // callback when window resized
        glfwSetWindowSizeCallback(window, resize);

        // init
        resize(window, width, height);
    }

    virtual ~Window() { glfwDestroyWindow(window); }

    // continue the draw loop or not
    explicit operator bool() {
        glfwWaitEvents();
        // return true if it should close the window
        return !glfwWindowShouldClose(window);
    }

    // double buffering
    void swapBuffers() const { glfwSwapBuffers(window); }

    static void resize(GLFWwindow* const window, int width, int height) {
        int fbWidth, fbHeight;
        glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
        // set entire frame buffer to the viewport
        // the window's rectangle (0, 0, width, height) will be in OpenGL
        // coordinate (0, 0, 1.0, 1.0)
        glViewport(0, 0, fbWidth, fbHeight);
    }
};
