//========================================================================
// This is a small test application for GLFW.
// The program opens a window (640x480),
// and renders the specified obj file.
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

#define DELTA_ROT 0.01f

using namespace std;

// Main method of engine program
int main(int argc, char * argv[]) {
  // if (argc < 2) {
  //     //cout << "Please specify which .obj to load" << endl;
  //     return (0);
  // }

  float x_rot = 0.0f;         // X rotation scaling factor
  float y_rot = 0.0f;         // Y rotation scaling factor
  float delta_rot = DELTA_ROT;     // Ammount rotation changes on key press
  float x_rot2 = -5.0f;         // X rotation scaling factor
  float y_rot2 = 0.0f;         // Y rotation scaling factor
  float x_rot3 = 0.0f;         // X rotation scaling factor
  float y_rot3 = 0.0f;   
  float delta_rot2 = DELTA_ROT;
  float delat_rot3 = DELTA_ROT;
  //string fileToLoad = argv[1];
  string fileToLoad = "models/crate.obj";

  GLuint program_id;                  // shader program handle
  
  Window window(640,480,"Engine");
  int width, height;
  float ratio;

  window.set_up_shaders("shaders/shader.vert", "shaders/shader.frag", program_id);

  Model model(program_id);
  model.load(fileToLoad);
  Light light(program_id);
  light.set_position(0.0f, 0.0f, 10.0f);
  light.set_color(1.0f, 1.0f, 1.0f);
  light.set_attenuation(0.0f);
  light.activate();

  Camera camera; // camera object
  // clone objects
  Clone clone1(&model, &camera);
  Clone clone2(&model, &camera);
  Clone clone3(&model, &camera);
  Clone clone4(&model, &camera);

  clone1.scale(0.3f,0.3f,0.3f);
  clone2.scale(0.5f,0.5f,0.5f);
  clone3.scale(0.75f,0.75f,0.75f);
  clone4.scale(0.1f,0.1f,0.1f);

  // translate clones
  clone1.translate(0.0f, 0.0f, -1.0f);
  clone2.translate(5.0f, 0.0f, -1.0f);
  clone3.translate(-5.0f, 0.0f, -1.0f);
  clone4.translate(0.0f, 5.0f, -1.0f);

  do {
      process_input(x_rot, y_rot, delta_rot, x_rot2, y_rot2, delta_rot2, light);
      process_view(x_rot, y_rot);

      // Get window size (may be different than the requested size)
      glfwGetWindowSize(&width, &height);
      ratio = width / (float) height;
      Matrix projection_matrix = Matrix::perspective(27.0f, ratio, 0.01f, 100.0f);

      // y_rot2 and x_rot2 are weird right now
      camera.translate(y_rot2, -x_rot, x_rot2);
      camera.rotate(0.0f, y_rot, 0.0f);
      clone1.draw(projection_matrix);
      clone2.draw(projection_matrix);
      clone3.draw(projection_matrix);
      clone4.draw(projection_matrix);

      glfwSwapBuffers();
    } // Check if the ESC key was pressed or the window was closed
    while(window.isOpen());

    window.close(program_id);
}
