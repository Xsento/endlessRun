#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include "glad/glad.h"

class EBO{
    public:
        //ID reference of EBO
        GLuint ID;

        EBO();

        void AssignValue(GLuint* vertices, GLsizeiptr size);
        //take a wild fucking guess
        void Bind();
        //take a wild fucking guess
        void Unbind();
        //take a wild fucking guess
        void Delete();
};

#endif