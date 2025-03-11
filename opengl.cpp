#include "iostream"
#include "vector"

#include "glad/glad.h"

#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "camera.h"
#include "mesh.h"
#include "model.h"
#include "shader.h"

void framebuffer_size_callback([[maybe_unused]] GLFWwindow *window, int width,
                               int height);
void mouse_callback([[maybe_unused]] GLFWwindow *window, double xpos,
                    double ypos);
void scroll_callback([[maybe_unused]] GLFWwindow *window,
                     [[maybe_unused]] double xoffset, double yoffset);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800, SCR_HEIGHT = 600;

float mixValue = 0.2f;

Camera camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  stbi_set_flip_vertically_on_load(true);
  glEnable(GL_DEPTH_TEST);

  // clang-format off
  glm::vec3 pointLightPositions[] = {
    glm::vec3( -1.7f,  -0.2f,  1.3f),
    // glm::vec3( 2.3f, -3.3f, -4.0f),
    // glm::vec3(-4.0f,  2.0f, -12.0f),
    // glm::vec3( 0.0f,  0.0f, -10.0f)
  };
  // clang-format on

  Shader ourShader("../shaders/shader.vs", "../shaders/shader.fs");
  ourShader.use();
  ourShader.setVec3("viewPos", camera.Position);
  const char *path = "../textures/backpack.obj";
  Model ourModel(path);

  // directional light
  // ourShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
  // ourShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
  // ourShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
  // ourShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
  // point light 1
  ourShader.setVec3("pointLights[0].position", pointLightPositions[0]);
  ourShader.setVec3("pointLights[0].ambient", 0.1f, 0.1f, 0.1f);
  ourShader.setVec3("pointLights[0].diffuse", 1.0f, 147.0f / 255.0f,
                    41.0f / 255.0f);
  ourShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
  ourShader.set("pointLights[0].constant", 1.0f);
  ourShader.set("pointLights[0].linear", 0.09f);
  ourShader.set("pointLights[0].quadratic", 0.032f);
  // // point light 2
  // ourShader.setVec3("pointLights[1].position", pointLightPositions[1]);
  // ourShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
  // ourShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
  // ourShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
  // ourShader.set("pointLights[1].constant", 1.0f);
  // ourShader.set("pointLights[1].linear", 0.09f);
  // ourShader.set("pointLights[1].quadratic", 0.032f);
  // // point light 3
  // ourShader.setVec3("pointLights[2].position", pointLightPositions[2]);
  // ourShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
  // ourShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
  // ourShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
  // ourShader.set("pointLights[2].constant", 1.0f);
  // ourShader.set("pointLights[2].linear", 0.09f);
  // ourShader.set("pointLights[2].quadratic", 0.032f);
  // // point light 4
  // ourShader.setVec3("pointLights[3].position", pointLightPositions[3]);
  // ourShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
  // ourShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
  // ourShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
  // ourShader.set("pointLights[3].constant", 1.0f);
  // ourShader.set("pointLights[3].linear", 0.09f);
  // ourShader.set("pointLights[3].quadratic", 0.032f);
  // // spotLight
  // ourShader.setVec3("spotLight.position", camera.Position);
  // ourShader.setVec3("spotLight.direction", camera.Front);
  // ourShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
  // ourShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
  // ourShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
  // ourShader.set("spotLight.constant", 1.0f);
  // ourShader.set("spotLight.linear", 0.09f);
  // ourShader.set("spotLight.quadratic", 0.032f);
  // ourShader.set("spotLight.innerCutoff", glm::cos(glm::radians(12.5f)));
  // ourShader.set("spotLight.outerCutoff", glm::cos(glm::radians(15.0f)));

  while (!glfwWindowShouldClose(window)) {
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    processInput(window);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = camera.GetViewMatrix();

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(camera.Zoom),
                                  static_cast<float>(SCR_WIDTH) /
                                      static_cast<float>(SCR_HEIGHT),
                                  0.1f, 100.0f);

    ourShader.use();
    // Geometry data
    ourShader.setMat4("view", view);
    ourShader.setMat4("projection", projection);
    ourShader.setVec3("viewPos", camera.Position);
    // Spotlight data
    ourShader.setVec3("spotLight.position", camera.Position);
    ourShader.setVec3("spotLight.direction", camera.Front);

    glm::mat4 model = glm::mat4(1.0f);
    ourShader.setMat4("model", model);
    glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(model)));
    ourShader.setMat3("normalMat", normalMat);
    ourModel.Draw(ourShader);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback([[maybe_unused]] GLFWwindow *window, int width,
                               int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    mixValue = (mixValue <= 0.999f) ? mixValue + 0.001f : 1.0f;
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    mixValue = (mixValue >= 0.001f) ? mixValue - 0.001f : 0.0f;
  }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.ProcessKeyboard(FORWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.ProcessKeyboard(LEFT, deltaTime);
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.ProcessKeyboard(RIGHT, deltaTime);
  }
}

void mouse_callback([[maybe_unused]] GLFWwindow *window, double xpos,
                    double ypos) {
  static float lastX;
  static float lastY;
  float xpos_f = static_cast<float>(xpos);
  float ypos_f = static_cast<float>(ypos);

  if (firstMouse) {
    lastX = xpos_f;
    lastY = ypos_f;
    firstMouse = false;
  }

  float xoffset = xpos_f - lastX;
  float yoffset = lastY - ypos_f;
  lastX = xpos_f, lastY = ypos_f;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback([[maybe_unused]] GLFWwindow *window,
                     [[maybe_unused]] double xoffset, double yoffset) {
  camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
