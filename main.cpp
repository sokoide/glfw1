#include "headers.hpp"
GLuint createProgram(const char* vsrc, const char* fsrc);
GLboolean printProgramInfoLog(GLuint program);
GLboolean printShaderInfoLog(GLuint shader, const char* str);

// vsrc: vertex shader source program string
// fsrc: fragment shader source program string
GLuint createProgram(const char* vsrc, const char* fsrc) {
    // empty program
    const GLuint program(glCreateProgram());
    if (vsrc != NULL) {
        // vertex shader
        const GLuint vobj(glCreateShader(GL_VERTEX_SHADER));
        glShaderSource(vobj, 1, &vsrc, NULL);
        glCompileShader(vobj);
        if (printShaderInfoLog(vobj, "vertex shader"))
            glAttachShader(program, vobj);
        glDeleteShader(vobj);
    }
    if (fsrc != NULL) {
        // fragment shader
        const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
        glShaderSource(fobj, 1, &fsrc, NULL);
        glCompileShader(fobj);
        if (printShaderInfoLog(fobj, "fragment shader"))
            glAttachShader(program, fobj);
        glDeleteShader(fobj);
    }
    // link
    glBindAttribLocation(program, 0, "position");
    glBindFragDataLocation(program, 0, "fragment");
    glLinkProgram(program);

    if (printProgramInfoLog(program))
        return program;
    glDeleteProgram(program);
    return 0;
}

// print link result of the program object
GLboolean printProgramInfoLog(GLuint program) {
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
        std::cerr << "Link Error." << std::endl;
    // get shader link log length
    GLsizei bufSize;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);
    if (bufSize > 1) {
        // get shader link log
        std::vector<GLchar> infoLog(bufSize);
        GLsizei length;
        glGetProgramInfoLog(program, bufSize, &length, &infoLog[0]);
        std::cerr << &infoLog[0] << std::endl;
    }
    return static_cast<GLboolean>(status);
}

// print compile result of the shader object
// shader: shader object
// str: place of the compile error
GLboolean printShaderInfoLog(GLuint shader, const char* str) {
    // get compile result
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
        std::cerr << "Compile Error in " << str << std::endl;
    // get length of shader compile log
    GLsizei bufSize;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);
    if (bufSize > 1) {
        // get the shader compile log
        std::vector<GLchar> infoLog(bufSize);
        GLsizei length;
        glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);
        std::cerr << &infoLog[0] << std::endl;
    }
    return static_cast<GLboolean>(status);
}

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

    // shaders
    // vertex shader
    static constexpr GLchar vsrc[] = "#version 150 core\n"
                                     "in vec4 position;\n"
                                     "void main()\n"
                                     "{\n"
                                     " gl_Position = position;\n"
                                     "}\n";
    // fragment shader
    static constexpr GLchar fsrc[] = "#version 150 core\n"
                                     "out vec4 fragment;\n"
                                     "void main()\n"
                                     "{\n"
                                     " fragment = vec4(1.0, 0.0, 0.0, 1.0);\n"
                                     "}\n";
    const GLuint program(createProgram(vsrc, fsrc));

    while (glfwWindowShouldClose(window) == GL_FALSE) {
        glClear(GL_COLOR_BUFFER_BIT);
        // use the shder
        glUseProgram(program);

        // TODO: draw here

        // flip
        glfwSwapBuffers(window);
        glfwWaitEvents();
    }

    return 0;
}
