#include <iostream>
#include <vector>

#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
  
static void draw_scene();
static void init_scene();
static void destroy_scene();
static GLuint create_shader(const char *code, GLenum type);
static GLuint create_program(const char *vshader, const char *fshader);

GLuint vertices_buffer;
GLuint vertices_vao;
GLuint shader_program;

static const char *vertex_shader_code =
  "#version 150                \n"
  "in vec2 position;           \n"
  "void main() {               \n"
  "  gl_Position = vec4(position.x, position.y, 0.0, 1.0);   \n"
  "}\n";

static const char *fragment_shader_code =
  "#version 150                            \n"
  " out vec4 color;                        \n"
  "void main() {                           \n"
  "  color = vec4(1.0, 0.0, 0.0, 1.0);     \n"
  "}\n";

int main() {
  GLFWwindow *window;

  if (!glfwInit())
    return 1;
  
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(1024, 768, "Fireworks!", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(window);
  init_scene();
  
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    draw_scene();
    
    glfwSwapBuffers(window);
  }

  glfwTerminate();
}

void init_scene() {
  glGenBuffers(1, &vertices_buffer);
  glGenVertexArrays(1, &vertices_vao);
  shader_program = create_program(vertex_shader_code, fragment_shader_code);
}

void draw_scene() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  std::vector<float> vertices;
  vertices.push_back(-0.5f);
  vertices.push_back(0.5f);
  vertices.push_back(0.0f);
  vertices.push_back(0.5f);
  vertices.push_back(0.0f);
  vertices.push_back(0.0f);
  
  glBindVertexArray(vertices_vao);
  glBindBuffer(GL_ARRAY_BUFFER, vertices_buffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

  glUseProgram(shader_program);

  GLint position_attribute = glGetAttribLocation(shader_program, "position");
  glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(position_attribute);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}



static GLuint create_shader(const char *code, GLenum type) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &code, nullptr);
  glCompileShader(shader);
  
  GLint compile_status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
  if (!compile_status) {
    std::cerr << "Failed to compile shader." << std::endl;
    exit(1);
  }
  
  return shader;
}

static GLuint create_program(const char *vshader, const char *fshader) {
  GLuint vertex_shader = create_shader(vshader, GL_VERTEX_SHADER);
  GLuint fragment_shader = create_shader(fshader, GL_FRAGMENT_SHADER);
  
  GLuint program = glCreateProgram();
  glAttachShader(program, vertex_shader);
  glAttachShader(program, fragment_shader);  
  glLinkProgram(program);
  
  return program;  
}
