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
#include "functions.h"
#include "matrix.h"
#include "clone.h"
#include "camera.h"
#include <iostream>

#define DELTA_ROT 0.005f

using namespace std;

// Main method of engine program
int main(int argc, char * argv[]) {
  if (argc < 2) {
      cout << "Please specify which .obj to load" << endl;
      return (0);
  }

  float x_rot = 0.0f;         // X rotation scaling factor
  float y_rot = 0.0f;         // Y rotation scaling factor
  float delta_rot = DELTA_ROT;     // Ammount rotation changes on key press
  float x_rot2 = 0.0f;         // X rotation scaling factor
  float y_rot2 = 0.0f;         // Y rotation scaling factor
  float x_rot3 = 0.0f;         // X rotation scaling factor
  float y_rot3 = 0.0f;   
  float delta_rot2 = DELTA_ROT;
  float delat_rot3 = DELTA_ROT;
  string fileToLoad = argv[1];

  GLuint program_id;                  // shader program handle
  
  Window window(640,480,"Engine");
  int width, height;
  float ratio;

  window.set_up_shaders("shaders/shader.vert", "shaders/shader.frag", program_id);

  Model model(program_id);
  model.load(fileToLoad);
  Light light(program_id);
  light.set_position(0.0f, 0.0f, 3.0f);
  light.set_color(1.0f, 1.0f, 1.0f);
  light.set_attenuation(1.0f);
  light.activate();

  Camera camera; // camera object
  // clone objects
  Clone clone1(&model, &camera);

  clone1.scale(0.50f,0.50f,0.50f);

  // translate clones
  clone1.translate(0.0f, 0.0f, -1.0f);

  do {
      process_input(x_rot, y_rot, delta_rot, x_rot2, y_rot2, delta_rot2, light);
      process_view(x_rot, y_rot);

      // Get window size (may be different than the requested size)
      glfwGetWindowSize(&width, &height);
      ratio = width / (float) height;
      //Matrix projection_matrix = Matrix::ortho(-10.0 * ratio, 10.0 * ratio, -10.0f, 10.0f, -10.0f, 10.0f);
      Matrix projection_matrix = Matrix::perspective(27.0f, ratio, 0.01f, 10.0f);
      clone1.rotate(0.0f, y_rot, 0.0f);

      // y_rot2 and x_rot2 are weird right now
      camera.translate(y_rot2, -x_rot, x_rot2);

      clone1.draw(projection_matrix);

      glfwSwapBuffers();
    } // Check if the ESC key was pressed or the window was closed
    while(window.isOpen());

    window.close(program_id);
}
