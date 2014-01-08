#ifndef GL_H
#define GL_H

#include <OpenGL/gl3.h>

namespace gl {
GLuint create_shader(const char *code, GLenum type);
GLuint create_program(const char *vshader, const char *fshader);
}

#endif // !GL_H
