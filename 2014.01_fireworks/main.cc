#include <iostream>
#include <vector>
#include <cmath>
#include <memory>

#include <GLFW/glfw3.h>
  
#include "gl.h"
#include "utils.h"

static void draw_scene();
static void init_scene();
static void destroy_scene();
static void update_sprites(float dt);
static void render_sprites(std::vector<float> &out);
static void explode_at(float x, float y);
static void mouse_clicked(GLFWwindow *window, int button, int action, int mods);


static void push_color(std::vector<float> &out, float r, float g, float b) {
  out.push_back(r);
  out.push_back(g);
  out.push_back(b);
}

static void push_quad(std::vector<float> &buffer, float left, float top, float right, float bottom, float r, float g, float b) {
  // tri #1
  buffer.push_back(left);
  buffer.push_back(top);
  push_color(buffer, r, g, b);
  buffer.push_back(right);
  buffer.push_back(top);
  push_color(buffer, r, g, b);
  buffer.push_back(right);
  buffer.push_back(bottom);
  push_color(buffer, r, g, b);
    
  // tri #2
  buffer.push_back(right);
  buffer.push_back(bottom);
  push_color(buffer, r, g, b);
  buffer.push_back(left);
  buffer.push_back(bottom);
  push_color(buffer, r, g, b);
  buffer.push_back(left);
  buffer.push_back(top);
  push_color(buffer, r, g, b);
}

struct sprite_t {
  float x, y;
  float vx, vy;
  float radius;
  float r, g, b;
  float ttl;
  
  void render(std::vector<float> &buffer) const {
    float left = x - radius;
    float right = x + radius;
    float top = y - radius;
    float bottom = y + radius;
    float alpha = clamp(ttl);
    
    push_quad(buffer, left, top, right, bottom, r * alpha, g * alpha, b * alpha);
  }
  
  static constexpr size_t rendered_data_size = 30 * sizeof(float);
};

GLuint vertices_buffer;
GLuint vertices_vao;
GLuint shader_program;

std::vector<sprite_t> sprites;

static const char *vertex_shader_code =
  "#version 150                \n"
  "in vec2 position;           \n"
  "in vec3 color;              \n"
  "out vec3 frag_color;    \n"
  "void main() {               \n"
  "  gl_Position = vec4(position.x, position.y, 0.0, 1.0);   \n"
  "  frag_color = color; \n"
  "}\n";

static const char *fragment_shader_code =
  "#version 150                            \n"
  "in vec3 frag_color;                     \n"
  "out vec4 color;                         \n"
  "void main() {                           \n"
  "  color = vec4(frag_color.rgb, 1.0);     \n"
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

  glfwSetMouseButtonCallback(window, mouse_clicked);
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
  shader_program = gl::create_program(vertex_shader_code, fragment_shader_code);
  
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

static void mouse_clicked(GLFWwindow *window, int button, int action, int mods) {
  int width, height;
  glfwGetWindowSize(window, &width, &height);

  double x, y;
  glfwGetCursorPos(window, &x, &y);

  x = (x / width - 0.5) * 2;
  y = (y / height - 0.5) * -2;

  explode_at(x, y);
}

static void update_sprites(float dt) {
  using std::begin;
  using std::end;
  using std::swap;
  
  // Use additive blending or something and the changing of order shouldn't be noticed
  
  for (auto iter = begin(sprites); iter != end(sprites); ) {
    if (iter->ttl <= 0.0f) {
      *iter = sprites.back();
      sprites.pop_back();
    }
    else {
      iter->vy -= 0.982 * dt + ((rand() % 100) - 50) / 7000.0f;
      iter->vx += ((rand() % 100) - 50) / 7000.0f;
      iter->x += iter->vx * dt;
      iter->y += iter->vy * dt;
      iter->ttl -= dt;
      iter->radius += dt * 0.1;
      ++iter;
    }
  }
}

void render_sprites(std::vector<float> &out) {
  out.reserve(sprite_t::rendered_data_size);
  
  for (sprite_t &sprite : sprites) {
    sprite.render(out);
  }
}

void explode_at(float x, float y) {
  const int steps = 15;
  float r = (rand() % 10) / 10.0f;
  float g = (rand() % 10) / 10.0f;
  float b = (rand() % 10) / 10.0f;
  float ang_ofs = (rand() % 10) / 10.0f;
  
  for (float ang = 0.0f; ang <= 2.0f * M_PI; ang += (2.0f * M_PI) / steps) {
    sprite_t sprite;
    float speed = 0.6f + (rand() % 10) / 200.0f;
    sprite.vx = cos(ang + ang_ofs) * speed;
    sprite.vy = sin(ang + ang_ofs) * speed;
    sprite.ttl = 2.0f;
    sprite.r = r;
    sprite.g = g;
    sprite.b = b;
    sprite.x = x;
    sprite.y = y;
    sprite.radius = 0.001f;
    sprites.push_back(sprite);
  }
}
