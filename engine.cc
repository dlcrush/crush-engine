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
#include <iostream>

#define DELTA_ROT 0.05f

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
  float delta_rot2 = DELTA_ROT; 
  string fileToLoad = argv[1];

  GLuint program_id;                  // shader program handle
  
  Window window(640,480,"Engine");
  int width, height;
  float ratio;

  window.set_up_shaders("shader.vert", "shader.frag", program_id);

  Model model(program_id);
  model.load(fileToLoad);
  Light light(program_id);
  light.set_position(1.0f, 1.0f, 1.0f);
  light.set_color(1.0f, 1.0f, 1.0f);
  light.set_attenuation(1.0f);
  light.activate();

  do {
      process_input(x_rot, y_rot, delta_rot, x_rot2, y_rot2, delta_rot2, light);
      process_view(x_rot, y_rot);

      // Get window size (may be different than the requested size)
      glfwGetWindowSize(&width, &height);

      // glMatrixMode(GL_PROJECTION);
      // glLoadIdentity();
      // ratio = width / (float) height;
      // glOrtho(-3.0 * ratio, 3.0 * ratio, -3.0f, 3.0f, -3.0f, 3.0f);

      // // Select and setup the modelview matrix
      // glMatrixMode(GL_MODELVIEW);
      // glLoadIdentity();

      // // Rotate view
      // glRotatef(x_rot, 1.0f, 0.0f, 0.0f);
      // glRotatef(y_rot, 0.0f, 1.0f, 0.0f);

      ratio = width / (float) height;
      //Matrix projection_matrix = Matrix::perspective(27.0f, ratio, 0.1f, 10.0f);
      Matrix projection_matrix = Matrix::ortho(-10.0 * ratio, 10.0 * ratio, -10.0f, 10.0f, -10.0f, 10.0f);
      Matrix view_matrix = Matrix::identity();
      Matrix model_matrix = Matrix::translate(-5.0f, 0.0f, -3.0f) * Matrix::rotateX(x_rot) * Matrix::rotateY(y_rot);
      Matrix model_view_matrix = view_matrix * model_matrix;
      Matrix model_view_projection_matrix = projection_matrix * model_view_matrix;
      Matrix normal_matrix = model_view_matrix.normalMatrix();

      model.draw(model_view_projection_matrix, model_view_matrix, normal_matrix);

      //projection_matrix = Matrix::ortho(-10.0 * ratio, 10.0 * ratio, -10.0f, 10.0f, -10.0f, 10.0f);
      //view_matrix = Matrix::identity();
      model_matrix = Matrix::translate(0.0f, 0.0f, -3.0f) * Matrix::rotateX(x_rot) * Matrix::rotateY(y_rot);
      model_view_matrix = view_matrix * model_matrix;
      model_view_projection_matrix = projection_matrix * model_view_matrix;
      normal_matrix = model_view_matrix.normalMatrix();

      model.draw(model_view_projection_matrix, model_view_matrix, normal_matrix);

      model_matrix = Matrix::translate(5.0f, 0.0f, -3.0f) * Matrix::rotateX(x_rot) * Matrix::rotateY(y_rot);
      model_view_matrix = view_matrix * model_matrix;
      model_view_projection_matrix = projection_matrix * model_view_matrix;
      normal_matrix = model_view_matrix.normalMatrix();

      model.draw(model_view_projection_matrix, model_view_matrix, normal_matrix);

      model_matrix = Matrix::translate(5.0f, 5.0f, -3.0f) * Matrix::rotateX(x_rot) * Matrix::rotateY(y_rot);
      model_view_matrix = view_matrix * model_matrix;
      model_view_projection_matrix = projection_matrix * model_view_matrix;
      normal_matrix = model_view_matrix.normalMatrix();

      model.draw(model_view_projection_matrix, model_view_matrix, normal_matrix);

      model_matrix = Matrix::translate(5.0f, -5.0f, -3.0f) * Matrix::rotateX(x_rot) * Matrix::rotateY(y_rot);
      model_view_matrix = view_matrix * model_matrix;
      model_view_projection_matrix = projection_matrix * model_view_matrix;
      normal_matrix = model_view_matrix.normalMatrix();

      model.draw(model_view_projection_matrix, model_view_matrix, normal_matrix);

      model_matrix = Matrix::translate(0.0f, -5.0f, -3.0f) * Matrix::rotateX(x_rot) * Matrix::rotateY(y_rot);
      model_view_matrix = view_matrix * model_matrix;
      model_view_projection_matrix = projection_matrix * model_view_matrix;
      normal_matrix = model_view_matrix.normalMatrix();

      model.draw(model_view_projection_matrix, model_view_matrix, normal_matrix);

      model_matrix = Matrix::translate(0.0f, 5.0f, -3.0f) * Matrix::rotateX(x_rot) * Matrix::rotateY(y_rot);
      model_view_matrix = view_matrix * model_matrix;
      model_view_projection_matrix = projection_matrix * model_view_matrix;
      normal_matrix = model_view_matrix.normalMatrix();

      model.draw(model_view_projection_matrix, model_view_matrix, normal_matrix);

      model_matrix = Matrix::translate(-5.0f, 5.0f, -3.0f) * Matrix::rotateX(x_rot) * Matrix::rotateY(y_rot);
      model_view_matrix = view_matrix * model_matrix;
      model_view_projection_matrix = projection_matrix * model_view_matrix;
      normal_matrix = model_view_matrix.normalMatrix();

      model.draw(model_view_projection_matrix, model_view_matrix, normal_matrix);

      model_matrix = Matrix::translate(-5.0f, -5.0f, -3.0f) * Matrix::rotateX(x_rot) * Matrix::rotateY(y_rot);
      model_view_matrix = view_matrix * model_matrix;
      model_view_projection_matrix = projection_matrix * model_view_matrix;
      normal_matrix = model_view_matrix.normalMatrix();

      model.draw(model_view_projection_matrix, model_view_matrix, normal_matrix);

      model_matrix = Matrix::translate(-10.0f, -5.0f, -3.0f) * Matrix::rotateX(x_rot) * Matrix::rotateY(y_rot);
      model_view_matrix = view_matrix * model_matrix;
      model_view_projection_matrix = projection_matrix * model_view_matrix;
      normal_matrix = model_view_matrix.normalMatrix();

      model.draw(model_view_projection_matrix, model_view_matrix, normal_matrix);

      model_matrix = Matrix::translate(-10.0f, -0.0f, -3.0f) * Matrix::rotateX(x_rot) * Matrix::rotateY(y_rot);
      model_view_matrix = view_matrix * model_matrix;
      model_view_projection_matrix = projection_matrix * model_view_matrix;
      normal_matrix = model_view_matrix.normalMatrix();

      model.draw(model_view_projection_matrix, model_view_matrix, normal_matrix);

      model_matrix = Matrix::translate(-10.0f, 5.0f, -3.0f) * Matrix::rotateX(x_rot) * Matrix::rotateY(y_rot);
      model_view_matrix = view_matrix * model_matrix;
      model_view_projection_matrix = projection_matrix * model_view_matrix;
      normal_matrix = model_view_matrix.normalMatrix();

      model.draw(model_view_projection_matrix, model_view_matrix, normal_matrix);

      model_matrix = Matrix::translate(10.0f, -5.0f, -3.0f) * Matrix::rotateX(x_rot) * Matrix::rotateY(y_rot);
      model_view_matrix = view_matrix * model_matrix;
      model_view_projection_matrix = projection_matrix * model_view_matrix;
      normal_matrix = model_view_matrix.normalMatrix();

      model.draw(model_view_projection_matrix, model_view_matrix, normal_matrix);

      model_matrix = Matrix::translate(10.0f, 5.0f, -3.0f) * Matrix::rotateX(x_rot) * Matrix::rotateY(y_rot);
      model_view_matrix = view_matrix * model_matrix;
      model_view_projection_matrix = projection_matrix * model_view_matrix;
      normal_matrix = model_view_matrix.normalMatrix();

      model.draw(model_view_projection_matrix, model_view_matrix, normal_matrix);

      model_matrix = Matrix::translate(10.0f, 0.0f, -3.0f) * Matrix::rotateX(x_rot) * Matrix::rotateY(y_rot);
      model_view_matrix = view_matrix * model_matrix;
      model_view_projection_matrix = projection_matrix * model_view_matrix;
      normal_matrix = model_view_matrix.normalMatrix();

      model.draw(model_view_projection_matrix, model_view_matrix, normal_matrix);


      // Swap buffers
      glfwSwapBuffers();
    } // Check if the ESC key was pressed or the window was closed
    while(window.isOpen());

    window.close(program_id);
}
