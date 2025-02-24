#include "headers.hpp"
#include "shape.hpp"
#include "window.hpp"

GLuint createProgram(const char* vsrc, const char* fsrc);
GLboolean printProgramInfoLog(GLuint program);
GLboolean printShaderInfoLog(GLuint shader, const char* str);
bool readShaderSource(const std::string& filename, std::vector<GLchar>& buffer);
GLuint loadProgram(const char* vert, const char* frag);

constexpr Object::Vertex rectangleVertex[] = {
    {-0.5f, -0.5f}, {0.5f, -0.5f}, {0.5f, 0.5f}, {-0.5f, 0.5f}, {-0.1f, 0.0f}};

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

// retrun buffer of the source
bool readShaderSource(const std::string& filename,
                      std::vector<GLchar>& buffer) {
    // read filename into buffer
    std::ifstream file(filename.c_str(), std::ios::binary);
    if (!file) {
        std::cerr << "Error: Can't open source file: " << filename << std::endl;
        return false;
    }
    file.seekg(0L, std::ios::end);
    GLsizei length = static_cast<GLsizei>(file.tellg());
    buffer.resize(length + 1);
    file.seekg(0L, std::ios::beg);
    file.read(buffer.data(), length);
    buffer[length] = '\0';

    if (file.bad()) {
        std::cerr << "Error: Can't read source file: " << filename << std::endl;
        file.close();
        return false;
    }
    file.close();
    return true;
}

// create shaders from shader source files
GLuint loadProgram(const std::string& vert, const std::string& frag) {
    std::vector<GLchar> vsrc;
    const bool vstat(readShaderSource(vert, vsrc));
    std::vector<GLchar> fsrc;
    const bool fstat(readShaderSource(frag, fsrc));
    return vstat && fstat ? createProgram(vsrc.data(), fsrc.data()) : 0;
}

int main(int argc, char** argv) {
    const std::string vsrc = "../shaders/point.vert";
    const std::string fsrc = "../shaders/point.frag";

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
    Window window;

    // glew
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Can't initialize GLEW" << std::endl;
        return 1;
    }

    // set background color
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    // shaders
    const GLuint program(loadProgram(vsrc, fsrc));

    // make a shape, 2 dimension, 5 points
    std::unique_ptr<const Shape> shape(new Shape(2, 5, rectangleVertex));

    while (window) {
        glClear(GL_COLOR_BUFFER_BIT);

        // use the shader
        glUseProgram(program);

        // draw
        shape->draw();

        // flip
        window.swapBuffers();
    }

    return 0;
}
