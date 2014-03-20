//========================================================================
// This is a small test application for GLFW.
// The program opens a window (640x480),
// and renders a flat shaded sphere.
//========================================================================

#include <fstream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include "model.h"
#include "light.h"
#include "window.h"
#include <iostream>

#define   GLFW_KEY_W   87
#define   GLFW_KEY_S   83
#define   GLFW_KEY_D   68
#define   GLFW_KEY_A   65
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

  light.set_position(-y_rot2, x_rot2, 2.0f);
  light.activate();
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

  // Select and setup the projection matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  ratio = width / (float) height;
  glOrtho(-3.0 * ratio, 3.0 * ratio, -3.0f, 3.0f, -3.0f, 3.0f);

  // Select and setup the modelview matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Rotate view
  glRotatef(x_rot, 1.0f, 0.0f, 0.0f);
  glRotatef(y_rot, 0.0f, 0.0f, 1.0f);
}

// Main method of shader program
int main(int argc, char * argv[]) {
  if (argc < 2) {
      cout << "Please specify which .obj to load" << endl;
      return (0);
  }

  float x_rot = 0.0f;         // X rotation scaling factor
  float y_rot = 0.0f;         // Y rotation scaling factor
  float delta_rot = 1.0f;     // Ammount rotation changes on key press
  float x_rot2 = -1.0f;         // X rotation scaling factor
  float y_rot2 = -1.0f;         // Y rotation scaling factor
  float delta_rot2 = 0.05f; 
  string fileToLoad = argv[1];

  GLuint program_id;                  // shader program handle
  
  Model model;
  Window window(640,480,"shader");

  window.set_up_shaders("phong.vert", "phong.frag", program_id);

  model.load(fileToLoad, program_id);
  Light light(program_id);
  light.set_position(1.0f, 1.0f, 1.0f);
  light.set_color(1.0f, 1.0f, 1.0f);
  light.set_attenuation(1.0f);
  light.activate();

  do {
      process_input(x_rot, y_rot, delta_rot, x_rot2, y_rot2, delta_rot2, light);
      process_view(x_rot, y_rot);

      model.draw(program_id);

      // Swap buffers
      glfwSwapBuffers();

    } // Check if the ESC key was pressed or the window was closed
    while(glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS &&
           glfwGetWindowParam(GLFW_OPENED));

    // Deallocate buffer memory
    glDeleteProgram(program_id);
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
