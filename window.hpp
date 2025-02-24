#pragma once
#include "headers.hpp"

class Window {
    GLFWwindow* const window;
    GLfloat aspect;
    GLfloat size[2];
    GLfloat scale;

  public:
    Window(int width = 640, int height = 480, const char* title = "Hello!")
        : window(glfwCreateWindow(width, height, title, NULL, NULL)),
          scale(100.0f) {
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

        // save `this`
        glfwSetWindowUserPointer(window, this);

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

        // get `this` pointer
        Window* const instance(
            static_cast<Window*>(glfwGetWindowUserPointer(window)));
        if (instance != NULL) {
            // update apdect ratio of the instance
            instance->aspect =
                static_cast<GLfloat>(width) / static_cast<GLfloat>(height);
            instance->size[0] = static_cast<GLfloat>(width);
            instance->size[1] = static_cast<GLfloat>(height);
        }
    }

    GLfloat getAspect() const { return aspect; }
    const GLfloat* getSize() const { return size; }
    GLfloat getScale() const { return scale; }
};
