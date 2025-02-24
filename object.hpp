#pragma once
#include "headers.hpp"

class Object {
    // vertex array object
    GLuint vao;
    // vertex buffer object
    GLuint vbo;

  public:
    struct Vertex {
        GLfloat position[2];
    };

    Object(GLint dim, GLsizei vertexcount, const Vertex* vertex) {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertexcount * sizeof(Vertex), vertex,
                     GL_STATIC_DRAW);

        // make the vertex buffer object referencable by 'in' variable
        glVertexAttribPointer(0, dim, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
    }

    virtual ~Object() {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
    }

    void bind() const { glBindVertexArray(vao); }

  private:
    // disable copy constructor
    Object(const Object& o);
    // disable copy by assignment
    Object& operator=(const Object& o);
};
