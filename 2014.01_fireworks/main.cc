#include <iostream>
#include <vector>
#include <cmath>

#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>
  
static void draw_scene();
static void init_scene();
static void destroy_scene();
static void update_sprites(float dt);
static void render_sprites(std::vector<float> &out);
static void explode_at(float x, float y);

static GLuint create_shader(const char *code, GLenum type);
static GLuint create_program(const char *vshader, const char *fshader);

struct sprite_t {
  float x, y;
  float vx, vy;
  float radius;
  float r, g, b;
  float ttl;
};

GLuint vertices_buffer;
GLuint vertices_vao;
GLuint shader_program;

std::vector<sprite_t> sprites;

static const char *vertex_shader_code =
  "#version 150                \n"
  "in vec2 position;           \n"
  "in vec3 color;              \n"
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
  explode_at(0.0f, 0.0f);
}

void draw_scene() {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  update_sprites(1.0f/60.0f); // estimated 60 fps
  std::vector<float> vertices;
  render_sprites(vertices);
  
  glBindVertexArray(vertices_vao);
  glBindBuffer(GL_ARRAY_BUFFER, vertices_buffer);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);
  glUseProgram(shader_program);

  GLint position_attribute = glGetAttribLocation(shader_program, "position");
  GLint color_attribute = glGetAttribLocation(shader_program, "color");
  glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_TRUE, 5 * sizeof(vertices[0]), 0);
  glVertexAttribPointer(color_attribute, 3, GL_FLOAT, GL_TRUE, 5 * sizeof(vertices[0]), (GLvoid *)(2 * sizeof(vertices[0])));
  
  glEnableVertexAttribArray(position_attribute);
  glEnableVertexAttribArray(color_attribute);
  glDrawArrays(GL_TRIANGLES, 0, vertices.size() / (2 + 3));
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

static void update_sprites(float dt) {
  for (sprite_t &sprite : sprites) {
    sprite.vy -= 0.982 * dt;
    sprite.x += sprite.vx * dt;
    sprite.y += sprite.vy * dt;
    sprite.ttl -= dt;
  }
}

static void push_color(std::vector<float> &out, float r, float g, float b) {
  out.push_back(r);
  out.push_back(g);
  out.push_back(b);
}

static void render_sprites(std::vector<float> &out) {
  for (sprite_t &sprite : sprites) {
    float left = sprite.x - sprite.radius;
    float right = sprite.x + sprite.radius;
    float top = sprite.y - sprite.radius;
    float bottom = sprite.y + sprite.radius;
    
    // tri #1
    out.push_back(left);
    out.push_back(top);
    push_color(out, sprite.r, sprite.g, sprite.b);
    out.push_back(right);
    out.push_back(top);
    push_color(out, sprite.r, sprite.g, sprite.b);
    out.push_back(right);
    out.push_back(bottom);
    push_color(out, sprite.r, sprite.g, sprite.b);
    
    
    // tri #2
    out.push_back(right);
    out.push_back(bottom);
    push_color(out, sprite.r, sprite.g, sprite.b);
    out.push_back(left);
    out.push_back(bottom);
    push_color(out, sprite.r, sprite.g, sprite.b);
    out.push_back(left);
    out.push_back(top);
    push_color(out, sprite.r, sprite.g, sprite.b);
  }
}

static void explode_at(float x, float y) {
  const int steps = 15;
  for (float ang = 0.0f; ang <= 2.0f * M_PI; ang += (2.0f * M_PI) / steps) {
    sprite_t sprite;
    float speed = 0.6f;
    sprite.vx = cos(ang) * speed;
    sprite.vy = sin(ang) * speed;
    sprite.ttl = 2.0f;
    sprite.r = 1.0f;
    sprite.g = 0.0f;
    sprite.b = 0.0f;
    sprite.x = x;
    sprite.y = y;
    sprite.radius = 0.1f;
    sprites.push_back(sprite);
  }
}
