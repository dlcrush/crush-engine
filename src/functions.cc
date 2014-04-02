#include <fstream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include "model.h"
#include "light.h"
#include "functions.h"
#include <iostream>

using namespace std;

// handles processing for user input
void process_input(float & x_rot, float & y_rot, float delta_rot, float & x_rot2, float & y_rot2, float delta_rot2, Light light) {
  // Handle user input
  if (glfwGetKey(GLFW_KEY_UP) == GLFW_PRESS) {
    x_rot += delta_rot;
  }
  if (glfwGetKey(GLFW_KEY_DOWN) == GLFW_PRESS) {
    x_rot -= delta_rot;
  }
  if (glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS) {
    y_rot += delta_rot;
  }
  if (glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS) {
    y_rot -= delta_rot;
  }

  if (glfwGetKey(GLFW_KEY_W) == GLFW_PRESS) {
    x_rot2 += delta_rot2;
  }
  if (glfwGetKey(GLFW_KEY_S) == GLFW_PRESS) {
    x_rot2 -= delta_rot2;
  }
  if (glfwGetKey(GLFW_KEY_A) == GLFW_PRESS) {
    y_rot2 += delta_rot2;
  }
  if (glfwGetKey(GLFW_KEY_D) == GLFW_PRESS) {
    y_rot2 -= delta_rot2;
  }

  //light.set_position(-y_rot2, x_rot2, 3.0f);
  //light.activate();
}

// Handles the processing for the current view
void process_view(float x_rot, float y_rot) {
  int width, height;          // Window width and height
  float ratio;                // Window aspect ratio

  // Get window size (may be different than the requested size)
  glfwGetWindowSize(&width, &height);

  // Set the rendering viewport location and dimenstions
  height = height > 0 ? height : 1;
  glViewport(0, 0, width, height);

  // Clear color buffer
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}