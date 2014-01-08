#include "gl.h"
#include <stdexcept>

GLuint gl::create_shader(const char *code, GLenum type) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &code, nullptr);
  glCompileShader(shader);
  
  GLint compile_status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
  if (!compile_status) {
    throw std::runtime_error("Failed to compile shader");
  }
  
  return shader;
}

GLuint gl::create_program(const char *vshader, const char *fshader) {
  GLuint vertex_shader = create_shader(vshader, GL_VERTEX_SHADER);
  GLuint fragment_shader = create_shader(fshader, GL_FRAGMENT_SHADER);
  
  GLuint program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);  
  glLinkProgram(program);
  
  return program;  
}
