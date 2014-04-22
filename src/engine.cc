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
#include "headers/model.h"
#include "headers/light.h"
#include "headers/window.h"
#include "headers/functions.h"
#include "headers/matrix.h"
#include "headers/clone.h"
#include "headers/camera.h"
#include "headers/vector.h"
#include <iostream>

#define DELTA_ROT 0.1f
#define NUM_CRATES 500

using namespace std;

bool overlap(float min1, float max1, float min2, float max2) {
  return (min2 >= min1 && max2 >= min2) 
          || (max1 >= min2 && max2 >= min1) 
          || (min2 >= min1 && max1 >= max2) 
          || (min2 >= min1 && max2 >= max1);
}

bool collide(Clone c1, Clone c2) {

  vector<Vector> c1Vectors = c1.getVectors();
  vector<Vector> c2Vectors = c2.getVectors();



  vector<Vector> normals;

  for (int i = 0; i < c1Vectors.size(); i ++) {
    //normals.push_back()
  }

  // cout << "begin c1 -----------------------------" << endl;
  // for (int i = 0; i < c1Vectors.size(); i ++) {
  //   cout << c1Vectors.at(i) << endl;
  //   cout << endl;
  // }
  // cout << "end c1 -------------------------------" << endl;
  // 
  


  return true;
}

// Main method of engine program
int main(int argc, char * argv[]) {
  // if (argc < 2) {
  //     //cout << "Please specify which .obj to load" << endl;
  //     return (0);
  // }

  float x_rot = 0.0f;         // X rotation scaling factor
  float y_rot = 0.0f;         // Y rotation scaling factor
  float delta_rot = DELTA_ROT;     // Ammount rotation changes on key press
  float x_rot2 = 180.0f;         // X rotation scaling factor
  float y_rot2 = 0.0f;         // Y rotation scaling factor
  float x_rot3 = 0.0f;         // X rotation scaling factor
  float y_rot3 = 0.0f;
  float z_position = -1.0f;
  float z_delta = -2.5f;   
  float delta_rot2 = 10.0f;
  float delat_rot3 = DELTA_ROT;
  //string fileToLoad = argv[1];
  string fileToLoad = "models/crate.obj";

  GLuint program_id;                  // shader program handle
  
  Window window(640,480,"Engine");
  int width, height;
  float ratio;

  window.set_up_shaders("shaders/shader.vert", "shaders/shader.frag", program_id);

  /* create models */
  Model crate(program_id);
  Model spaceship(program_id);

  /* Load models */
  spaceship.load("models/SpaceShip.obj");

  Model earth(program_id);
  earth.load("models/Earth.obj");

  //return 0;
  crate.load(fileToLoad);


  /* Create, set up, and activate light */
  Light light(program_id);
  light.set_position(0.0f, -1000.0f, 10.0f);
  light.set_color(1.0f, 1.0f, 1.0f);
  light.set_attenuation(0.0f);
  light.activate();

  /* Create camera */
  Camera camera;

  /* Create clone objects */
  Clone crate1(&crate, &camera);
  Clone crate2(&crate, &camera);
  Clone crate3(&crate, &camera);
  Clone crate4(&crate, &camera);
  Clone player(&spaceship, &camera);

  Clone earth_clone(&earth, &camera);

  Model ocean(program_id);
  ocean.load("models/ocean.obj");
  Clone floor(&ocean, &camera);
  floor.scale(25000.0f, 25000.0f, 0.0f);
  floor.rotate(3.14f / 2.0f, 0.0f, 0.0f);
  floor.translate(0.0f, -5.0f, 0.0f);

  vector<Clone> clones;

  for (int i = 0; i < NUM_CRATES; i ++) {
    Clone * temp_clone = new Clone(&crate, &camera);
    clones.push_back(*temp_clone);
  }

  for (int i = 0; i < NUM_CRATES - 5; i+=5) {
    clones.at(i).scale(3.0f, 3.0f, 3.0f);
    clones.at(i + 1).scale(3.0f, 3.0f, 3.0f);
    clones.at(i + 2).scale(3.0f, 3.0f, 3.0f);
    clones.at(i + 3).scale(3.0f, 3.0f, 3.0f);
    clones.at(i + 4).scale(3.0f, 3.0f, 3.0f);
    //cout << -5.0f - (2.5f * (i / 5)) << endl;
    clones.at(i).translate(-20.0f, 200.0f, -5.0f - (100.0f * (i / 5)));
    clones.at(i + 1).translate(-10.0f, 200.0f, -5.0f - (100.0f * (i / 5)));
    clones.at(i + 2).translate(0.0f, 205.0f, -5.0f - (100.0f * (i / 5)));
    clones.at(i + 3).translate(10.0f, 200.0f, -5.0f - (100.0f * (i / 5)));
    clones.at(i + 4).translate(20.0f, 200.0f, -5.0f - (100.0f * (i / 5)));
  }

  /* Scale clones */
  crate1.scale(0.3f,0.3f,0.3f);
  // crate2.scale(0.3f,0.3f,0.3f);
  // crate3.scale(0.3f,0.3f,0.3f);
  // crate4.scale(0.3f,0.3f,0.3f);
  player.scale(0.05f, 0.05f, 0.05f);

  /* Translate clones */
  crate1.translate(0.0f, 0.0f, -5.0f);
  // crate1.translate(2.0f, 1.0f, -5.0f);
  // crate2.translate(5.0f, 0.0f, -7.5f);
  // crate3.translate(-5.0f, 0.0f, -7.5f);
  // crate4.translate(2.0f, 2.0f, -10.0f);
  player.translate(0.0f, 0.0f, -1.0f);

  earth_clone.translate(0.0f, 1.0f, -10.0f);

  double lastTime = glfwGetTime();
  int nbFrames = 0;
    
  do {

    double currentTime = glfwGetTime(); // get current time
    nbFrames++; // will hold number of frames elapsed in the past second
    if (currentTime - lastTime >= 1.0){
      // at least a second has elapsed, calculate FPS
      float SPF = 1.0f/nbFrames;
      float FPS = 1.0f/SPF;
      printf("%f FPS\n", FPS);
      nbFrames = 0;
      lastTime += 1.0;
    }

    process_input(x_rot, y_rot, delta_rot, x_rot2, y_rot2, delta_rot2, light);
    process_view(x_rot, y_rot);

    // Get window size (may be different than the requested size)
    glfwGetWindowSize(&width, &height);
    ratio = width / (float) height;
    Matrix projection_matrix = Matrix::perspective(27.0f, ratio, 0.01f, 100000.0f);

    z_position += z_delta;

    // y_rot2 and x_rot2 are weird right now
    camera.translate(y_rot2, -180.5f, -z_position - 1.0f);

    //cout << -z_position << endl;

    // if (-z_position >= 2500) {
    //   z_delta = 0.0f;
    // }

    //cout << -x_rot2 << endl;
    //light.set_position(0.0f, 20.0f, z_position - 1.0f);
    //light.activate();
    //floor.translate(0.0f, -1.0f, z_position);

    //cout << camera.getViewMatrix() << endl;;
    //camera.rotate(0.0f, y_rot, 0.0f);

    //crate1.translate(-y_rot, x_rot, -5.0f);
    //player.translate(-y_rot2, x_rot2, z_position);
    player.translate(-y_rot2, 180.0f, z_position);

    int count = 0;

    Matrix view_matrix = camera.getViewMatrix();

    // for (int i = 0; i < NUM_FLOOR; i ++) {
    //   floor.at(i).draw(projection_matrix);
    // }

    floor.draw(projection_matrix);

    // for (int i = 0; i < NUM_CRATES; i ++) {
    //   //cout << view_matrix << endl;
    //   float * view_matrix_data = view_matrix.data();

    //   vector<Vector> vectors = clones.at(i).getVectors();
    //   float * vector_data = vectors.at(0).getData();

    //   //cout << "1: " << vector_data[2] << endl;
    //   //cout << "2: " << view_matrix_data[11] << endl;

    //   // if (vector_data[2] < (view_matrix_data[11])) {
    //   //   count ++;
    //     clones.at(i).draw(projection_matrix);
    //   // }

    //  }

    //cout << count << endl;

    //earth_clone.draw(projection_matrix);


    crate1.draw(projection_matrix);
    // crate2.draw(projection_matrix);
    // crate3.draw(projection_matrix);
    // crate4.draw(projection_matrix);
    player.draw(projection_matrix);


    //collide(player, crate1);



    //collide(player, player);

    glfwSwapBuffers();
    } // Check if the ESC key was pressed or the window was closed
    while(window.isOpen());

    window.close(program_id);
}
