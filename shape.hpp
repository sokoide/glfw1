#pragma once
#include "headers.hpp"

#include "object.hpp"

class Shape {
    std::shared_ptr<const Object> object;

  protected:
    const GLsizei vertexcount;

  public:
    Shape(GLint dim, GLsizei vertexcount, const Object::Vertex* vertex)
        : object(new Object(dim, vertexcount, vertex)),
          vertexcount(vertexcount) {}

    void draw() const {
        object->bind();
        execute();
    }

    virtual void execute() const { glDrawArrays(GL_LINE_LOOP, 0, vertexcount); }
};
