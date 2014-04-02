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
  Clone clone2(&model, &camera);
  Clone clone3(&model, &camera);
  Clone clone4(&model, &camera);
  Clone clone5(&model, &camera);
  Clone clone6(&model, &camera);
  Clone clone7(&model, &camera);
  Clone clone8(&model, &camera);
  Clone clone9(&model, &camera);
  Clone clone10(&model, &camera);
  Clone clone11(&model, &camera);
  Clone clone12(&model, &camera);
  Clone clone13(&model, &camera);
  Clone clone14(&model, &camera);
  Clone clone15(&model, &camera);

  // translate clones
  clone1.translate(-5.0f, 0.0f, -3.0f);
  clone2.translate(0.0f, 0.0f, -3.0f);
  clone3.translate(5.0f, 0.0f, -3.0f);
  clone4.translate(5.0f, 5.0f, -3.0f);
  clone5.translate(5.0f, -5.0f, -3.0f);
  clone6.translate(0.0f, -5.0f, -3.0f);
  clone7.translate(0.0f, 5.0f, -3.0f);
  clone8.translate(-5.0f, 5.0f, -3.0f);
  clone9.translate(-5.0f, -5.0f, -3.0f);
  clone10.translate(-10.0f, -5.0f, -3.0f);
  clone11.translate(-10.0f, 0.0f, -3.0f);
  clone12.translate(-10.0f, 5.0f, -3.0f);
  clone13.translate(10.0f, -5.0f, -3.0f);
  clone14.translate(10.0f, 5.0f, -3.0f);
  clone15.translate(10.0f, 0.0f, -3.0f);

  //cout << projection_matrix << endl;

  do {
      process_input(x_rot, y_rot, delta_rot, x_rot2, y_rot2, delta_rot2, light);
      process_view(x_rot, y_rot);

      // Get window size (may be different than the requested size)
      glfwGetWindowSize(&width, &height);
      ratio = width / (float) height;
      Matrix projection_matrix = Matrix::ortho(-10.0 * ratio, 10.0 * ratio, -10.0f, 10.0f, -10.0f, 10.0f);
      clone1.rotate(x_rot, y_rot, 0.0f);
      clone2.rotate(x_rot, y_rot, 0.0f);
      clone3.rotate(x_rot, y_rot, 0.0f);
      clone4.rotate(x_rot, y_rot, 0.0f);
      clone5.rotate(x_rot, y_rot, 0.0f);
      clone6.rotate(x_rot, y_rot, 0.0f);
      clone7.rotate(x_rot, y_rot, 0.0f);
      clone8.rotate(x_rot, y_rot, 0.0f);
      clone9.rotate(x_rot, y_rot, 0.0f);
      clone10.rotate(x_rot, y_rot, 0.0f);
      clone11.rotate(x_rot, y_rot, 0.0f);
      clone12.rotate(x_rot, y_rot, 0.0f);
      clone13.rotate(x_rot, y_rot, 0.0f);
      clone14.rotate(x_rot, y_rot, 0.0f);
      clone15.rotate(x_rot, y_rot, 0.0f);

      // y_rot2 and x_rot2 are weird right now
      camera.translate(-y_rot2, x_rot2, 0.0f);

      clone1.draw(projection_matrix);
      clone2.draw(projection_matrix);
      clone3.draw(projection_matrix);
      clone4.draw(projection_matrix);
      clone5.draw(projection_matrix);
      clone6.draw(projection_matrix);
      clone7.draw(projection_matrix);
      clone8.draw(projection_matrix);
      clone9.draw(projection_matrix);
      clone10.draw(projection_matrix);
      clone11.draw(projection_matrix);
      clone12.draw(projection_matrix);
      clone13.draw(projection_matrix);
      clone14.draw(projection_matrix);
      clone15.draw(projection_matrix);

      glfwSwapBuffers();

      /*
      //Matrix projection_matrix = Matrix::perspective(27.0f, ratio, 0.1f, 10.0f);
      Matrix view_matrix = Matrix::identity() * Matrix::translate(0.0f, 0.0f, -5.0f);
      Matrix model_matrix = Matrix::translate(-5.0f, 0.0f, -3.0f) * Matrix::rotateX(x_rot) * Matrix::rotateY(y_rot);
      Matrix model_view_matrix = view_matrix * model_matrix;
      Matrix model_view_projection_matrix = projection_matrix * model_view_matrix;
      Matrix normal_matrix = model_view_matrix.normalMatrix();

      model.draw(model_view_projection_matrix, model_view_matrix, normal_matrix);

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

      */
    } // Check if the ESC key was pressed or the window was closed
    while(window.isOpen());

    window.close(program_id);
}
