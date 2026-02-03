#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include <string>
#include <vector>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <cstddef>
#include <glad/glad.h>
#include <glimac/FilePath.hpp>
#include <glimac/Program.hpp>
#include <glimac/glm.hpp>
#include <iostream>

int window_width = 800;
int window_height = 800;

static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int /*mods*/) {
  if (action == GLFW_PRESS) {
    const char *name = glfwGetKeyName(key, scancode);
    if (name != nullptr && std::string(name) == "q") {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
  }
}

static void mouse_button_callback(GLFWwindow * /*window*/, int /*button*/,
                                  int /*action*/, int /*mods*/) {}

static void scroll_callback(GLFWwindow * /*window*/, double /*xoffset*/,
                            double /*yoffset*/) {}

static void cursor_position_callback(GLFWwindow * /*window*/, double /*xpos*/,
                                     double /*ypos*/) {}

static void size_callback(GLFWwindow * /*window*/, int width, int height) {
  window_width = width;
  window_height = height;
}

struct Vertex2DColor {
  glm::vec2 position;
  glm::vec3 color;

  Vertex2DColor() {}
  Vertex2DColor(glm::vec2 pos, glm::vec3 col) {
    Vertex2DColor::position = pos;
    Vertex2DColor::color = col;
  }
};

int main(int argc, char **argv) {
  /* Initialize the library */
  if (!glfwInit()) {
    return -1;
  }

  /* Create a window and its OpenGL context */
  // #ifdef __APPLE__
  //     /* We need to explicitly ask for a 3.3 context on Mac */
  //     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  //     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  //     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // #endif
  GLFWwindow *window =
      glfwCreateWindow(window_width, window_height, "TP1", nullptr, nullptr);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  /* Intialize glad (loads the OpenGL functions) */
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    return -1;
  }

  glimac::FilePath applicationPath(argv[0]);
  glimac::Program program = glimac::loadProgram(
      applicationPath.dirPath() + "TP1/shaders/triangle.vs.glsl",
      applicationPath.dirPath() + "TP1/shaders/triangle.fs.glsl");
  program.use();

  /* Hook input callbacks */
  glfwSetKeyCallback(window, &key_callback);
  glfwSetMouseButtonCallback(window, &mouse_button_callback);
  glfwSetScrollCallback(window, &scroll_callback);
  glfwSetCursorPosCallback(window, &cursor_position_callback);
  glfwSetWindowSizeCallback(window, &size_callback);

  /*********************************
   * HERE SHOULD COME THE INITIALIZATION CODE
   *********************************/

  int N{10};    // nbr subdivisions
  float R{1.0}; // radius
  float teta{(2 * glm::pi<float>()) / N};
  float angle{0};

  GLuint vbo;
  glGenBuffers(1, &vbo);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  std::vector<Vertex2DColor> vertices;

  vertices.push_back(Vertex2DColor(glm::vec2(R, 0), glm::vec3(1, 0, 0)));
  for (int i{0}; i < N - 1; i++) {
    angle += teta;
    float x = R * glm::cos(angle);
    float y = R * glm::sin(angle);
    vertices.push_back(Vertex2DColor(glm::vec2(0, 0), glm::vec3(1, 0, 0)));
    vertices.push_back(Vertex2DColor(glm::vec2(x, y), glm::vec3(0, 1, 0)));
    vertices.push_back(Vertex2DColor(glm::vec2(x, y), glm::vec3(0, 0, 1)));
  }
  vertices.push_back(Vertex2DColor(glm::vec2(0, 0), glm::vec3(1, 0, 0)));
  vertices.push_back(Vertex2DColor(glm::vec2(R, 0), glm::vec3(1, 0, 0)));

  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex2DColor),
               vertices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  const GLuint VERTEX_ATTR_POSITION = 3;
  const GLuint VERTEX_ATTR_COLOR = 8;

  glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
  glEnableVertexAttribArray(VERTEX_ATTR_COLOR);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  // Position
  glVertexAttribPointer(VERTEX_ATTR_POSITION, 2, GL_FLOAT, GL_FALSE,
                        sizeof(Vertex2DColor), (const GLvoid *)0);

  // Couleur
  glVertexAttribPointer(VERTEX_ATTR_COLOR, 3, GL_FLOAT, GL_FALSE,
                        sizeof(Vertex2DColor),
                        (const GLvoid *)(sizeof(glm::vec2)));

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)) {
    glClearColor(1.f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    /*********************************
     * HERE SHOULD COME THE RENDERING CODE
     *********************************/

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);
    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();

  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
  return 0;
}