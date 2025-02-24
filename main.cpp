#include "headers.hpp"

int main(int argc, char** argv) {
    std::cout << "hello\n" << std::endl;

    // glfw
    if (glfwInit() == GL_FALSE) {
        std::cerr << "Can't initialize GLFW" << std::endl;
        return 1;
    }

    // register the exit function
    atexit(glfwTerminate);

    // enable Open GL 3.2 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // make a window
    GLFWwindow* const window(glfwCreateWindow(640, 480, "Hello!", NULL, NULL));
    if (window == NULL) {
        std::cerr << "Can't create GLFW window." << std::endl;
        return 1;
    }

    // wait for vsync
    glfwSwapInterval(1);

    glfwMakeContextCurrent(window);

    // glew
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Can't initialize GLEW" << std::endl;
        return 1;
    }

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    while (glfwWindowShouldClose(window) == GL_FALSE) {
        glClear(GL_COLOR_BUFFER_BIT);

        // TODO: draw here

        // flip
        glfwSwapBuffers(window);
        glfwWaitEvents();
    }

    return 0;
}
