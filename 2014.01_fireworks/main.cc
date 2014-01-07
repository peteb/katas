#include <iostream>
#include <GLFW/glfw3.h>

int main() {
  GLFWwindow *window;

  if (!glfwInit())
    return 1;

  window = glfwCreateWindow(1024, 768, "Fireworks!", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(window);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    glfwSwapBuffers(window);
    
  }

  glfwTerminate();
}
