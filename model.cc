#include "model.h"
#include "material.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
using namespace std;

#define BYTES_PER_FLOAT 4
#define FLOATS_PER_VERTEX 3

void copyVectorToArray(vector<GLfloat> source, GLfloat * destination, int begin, int end) {
  for (int i = begin; i < end; i ++) {
    destination[i - begin] = source.at(i);
  }
}

// Splits string into two parts. First part is face value.
// Second value is normal value.
void split(string input, string & face, string & normal) {
  int index = input.find("/");
  if (index == string::npos) {
    face = input;
  }
  else {
    string subString = input.substr(index + 1, string::npos);
    int index2 = subString.find("/");
    // if (index2 == string::npos) {
    //   face = input.substr(0, index);
    //   normal = input.substr(index + 2, string::npos);
    // }
    // else {
    face = input.substr(0, index);
    normal = subString.substr(index2 + 1, string::npos);

    //cout << face << endl;
    //cout << normal << endl;
    // }
    //face = input.substr(0, index);
    //normal = input.substr(index + 2, string::npos);
  }
}

// loads Material file
// PRE: mtlFile is defined, materials has been allocated space
// POST: materials contains all the materials from the mtlFile
void loadMtlFile(string mtlFile, vector<Material> & materials) {
  ifstream inputFile(mtlFile.c_str());

  Material material;

  bool existingMtl = false;

  while (inputFile.good()) {

    // parse file here
    string type;
    inputFile >> type;

    if (type.compare("newmtl") == 0) {
      if (existingMtl) {
        materials.push_back(material);
        material.clear();
      }

      existingMtl = true;

      string mtlName;

      inputFile >> mtlName;

      material.set_name(mtlName);
      
    }
    else if (type.compare("Ka") == 0) {
      GLfloat r,g,b;

      inputFile >> r >> g >> b;


      material.set_Ka(r,g,b);
    }
    else if (type.compare("Ks") == 0) {
      GLfloat r,g,b;

      inputFile >> r >> g >> b;

      material.set_Ks(r,g,b);
    }
    else if(type.compare("Kd") == 0) {
      GLfloat r,g,b;

      inputFile >> r >> g >> b;

      material.set_Kd(r,g,b);
    }
    else if(type.compare("Ns") == 0) {
      GLfloat coefficient;

      inputFile >> coefficient;

      material.set_Ns(coefficient);
      //materials.push_back(material);
    }
  }

  if (existingMtl) {
    materials.push_back(material);
  }
}

// PRE:
// POST: vertex_buffer_id has been allocated space,
// color_buffer_id has been allocated space,
// number_of_vertices has been allocated space.
Model::Model() {
  // do nothing
  //vertex_buffer_id = 0;
  //normal_buffer_id = 0;
}

// PRE:
// POST: vertex_buffer_id has been allocated space,
// color_buffer_id has been allocated space,
// number_of_vertices has been allocated space.
Model::~Model() {
  // delete buffers, if any
  for (int i = 0; i < vertex_buffer_id.size(); i ++) {
    glDeleteBuffers(1, &vertex_buffer_id.at(i));
    glDeleteBuffers(1, &normal_buffer_id.at(i));
  }
  //glDeleteBuffers(1, &vertex_buffer_id);
  //glDeleteBuffers(1, &normal_buffer_id);
}

// PRE: vertices has been defined, points has been defined,
// colors has been defined, and face is defined and an integer.
// POST: the vertex for the given face has been added to the vertices
// vector.
void Model::addFaceVertex(vector<GLfloat> & vertices, vector<GLfloat> points, vector<GLfloat> & normals, vector<GLfloat> & vn, int face, int normal) {
  vertices.push_back(points.at((face - 1) * 3));
  vertices.push_back(points.at((face - 1) * 3 + 1));
  vertices.push_back(points.at((face - 1) * 3 + 2));
  normals.push_back(vn.at((normal - 1) * 3));
  normals.push_back(vn.at((normal - 1) * 3 + 1));
  normals.push_back(vn.at((normal - 1) * 3 + 2));

}

// PRE: objFileName is defined and contains the filename of a valid .obj file.
// This has been defined.
// POST: The vertices for the obj file have been loaded and binded to 
// vertex_buffer_id. Also, color (same for all) has also been binded to 
// color_buffer_id.
void Model::load(string objFileName, GLuint program_id) {
  vector<GLfloat> points;
  vector<GLfloat> vertices;
  vector<GLfloat> vn;
  vector<GLfloat> normals;
  vector<Material> materials;

  /* Delete existing buffers, if any. */
  for (int i = 0; i < vertex_buffer_id.size(); i ++) {
    glDeleteBuffers(1, &vertex_buffer_id.at(i));
    glDeleteBuffers(1, &normal_buffer_id.at(i));
  }
  //glDeleteBuffers(1, &vertex_buffer_id);
  //glDeleteBuffers(1, &normal_buffer_id);
  //vertex_buffer_id = 0;
  //normal_buffer_id = 0;

  /* input file stream for obj file */
  ifstream inputFile(objFileName.c_str());

  bool success = inputFile.good();

  int count = 0;

  while (inputFile.good()) {
    string type;
    inputFile >> type;
    if (type.compare("v") == 0) {
      
      // read in vertice (should be 3 float values)
      GLfloat point1, point2, point3;

      inputFile >> point1 >> point2 >> point3;
      
      points.push_back(point1);
      points.push_back(point2);
      points.push_back(point3);
    }
    else if (type.compare("vn") == 0) {
      GLfloat vn1, vn2, vn3;

      inputFile >> vn1 >> vn2 >> vn3;

      vn.push_back(vn1);
      vn.push_back(vn2);
      vn.push_back(vn3);
    }
    else if (type.compare("mtllib") == 0) {
      string mtlFile;

      inputFile >> mtlFile;

      loadMtlFile(mtlFile, materials);
    }
    else if (type.compare("usemtl") == 0) {
      string mtlName;

      inputFile >> mtlName;

      for (int i = 0; i < materials.size(); i ++) {
        if (materials.at(i).get_name() == mtlName) {
          materialIDs.push_back(i);
          cout << "vertices size: " << vertices.size() << endl;
          if (vertices.size() != 0) {
            material_vertex_map.push_back(vertices.size() - 1);
          }
          else {
            material_vertex_map.push_back(0);
          }
          
          cout << "equal " << i << " " << material_vertex_map.at(material_vertex_map.size() - 1) << endl;
        }
      }

      //cout << find(materials.begin(), materials.end(), mtlName)) << endl;

      //materialIDs.push_back((int) (find(materials.begin(), materials.end(), mtlName)));
      //cout << materialIDs.at(materialIDs.size() - 1) << endl;
    }
    else if (type.compare("f") == 0) {
      string input1, input2, input3;
      string face1, face2, face3;
      string normal1, normal2, normal3;
      inputFile >> input1 >> input2 >> input3;

      split(input1, face1, normal1);
      split(input2, face2, normal2);
      split(input3, face3, normal3);

      addFaceVertex(vertices, points, normals, vn, atoi(face1.c_str()), atof(normal1.c_str()));
      addFaceVertex(vertices, points, normals, vn, atoi(face2.c_str()), atof(normal2.c_str()));
      addFaceVertex(vertices, points, normals, vn, atoi(face3.c_str()), atof(normal3.c_str()));
    }
  }

  if (success) {
    number_of_vertices = vertices.size();
    int buffer_size = number_of_vertices * BYTES_PER_FLOAT;
	  
    // GLfloat * verticeArray = new GLfloat[vertices.size()]; 
    // copy(vertices.begin(), vertices.end(), verticeArray);
    // GLfloat * normalArray = new GLfloat[normals.size()];
    // copy(normals.begin(), normals.end(), normalArray);

    //cout << "material_vertex_map size: " << material_vertex_map.size() << endl;

    for (int i = 0; i < material_vertex_map.size(); i ++) {

      //cout << "vertices.begin(): " << vertices.begin() << endl;

      GLfloat * verticeArray = new GLfloat[vertices.size()];
      if ((i + 1) < material_vertex_map.size()) {
        cout << "begin: " << material_vertex_map.at(i) << endl;
        cout << "end: " << material_vertex_map.at(i + 1) << endl;
        cout << "size: " << vertices.size() << endl;
        copyVectorToArray(vertices, verticeArray, material_vertex_map.at(i), material_vertex_map.at(i + 1) + 1);
      }
      else {
        cout << "begin: " << material_vertex_map.at(i) << endl;
        cout << "end: " << buffer_size << endl;
        cout << "size: " << vertices.size() << endl;
        copyVectorToArray(vertices, verticeArray, material_vertex_map.at(i), vertices.size());
      }
      //copyVectorToArray(vertices, verticeArray, )
      //copy(vertices.begin(), vertices.end(), verticeArray);
      GLfloat * normalArray = new GLfloat[normals.size()];
      if ((i + 1) < material_vertex_map.size()) {
        //copyVectorToArray(normals, normalArray, material_vertex_map.at(i), material_vertex_map.at(i + 1) + 1);
      }
      else {
        //copyVectorToArray(normals, normalArray, material_vertex_map.at(i), buffer_size);
      }
      //copy(normals.begin(), normals.end(), normalArray);

      GLuint temp_vertex_buffer_id, temp_normal_buffer_id;
      glGenBuffers(1, &temp_vertex_buffer_id);
      vertex_buffer_id.push_back(temp_vertex_buffer_id);
      glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id.at(i));
      if ((i + 1) < material_vertex_map.size()) {
        glBufferData(GL_ARRAY_BUFFER, material_vertex_map.at(i + 1) - material_vertex_map.at(i), verticeArray, GL_STATIC_DRAW);
      }
      else {
        glBufferData(GL_ARRAY_BUFFER, buffer_size - material_vertex_map.at(i), verticeArray, GL_STATIC_DRAW);
      }
      //glBufferData(GL_ARRAY_BUFFER, buffer_size, verticeArray, GL_STATIC_DRAW);

      glGenBuffers(1, &temp_normal_buffer_id);
      normal_buffer_id.push_back(temp_normal_buffer_id);
      glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_id.at(i));
      if ((i + 1) < material_vertex_map.size()) {
        glBufferData(GL_ARRAY_BUFFER, material_vertex_map.at(i + 1) - material_vertex_map.at(i), normalArray, GL_STATIC_DRAW);
      }
      else {
        glBufferData(GL_ARRAY_BUFFER, buffer_size - material_vertex_map.at(i), normalArray, GL_STATIC_DRAW);
      }
      //glBufferData(GL_ARRAY_BUFFER, buffer_size, normalArray, GL_STATIC_DRAW);

      Material * material = &(materials.at(i));

      GLfloat * Ka = material->get_Ka();
      GLfloat * Kd = material->get_Kd();
      GLfloat * Ks = material->get_Ks();
      GLfloat Ns = material->get_Ns();

      GLuint attenuation_amount_id = glGetUniformLocation(program_id,
        "attenuation_amount");
      glUniform1f(attenuation_amount_id, 1.0f); 
      GLuint ambient_id = glGetUniformLocation(program_id, "ambient_color_4f");
      glUniform4f(ambient_id, Ka[0], Ka[1], Ka[2], 1.0f);
      GLuint diffuse_id = glGetUniformLocation(program_id, "diffuse_color_4f");
      glUniform4f(diffuse_id, Kd[0], Kd[1], Kd[2], 1.0f);
      GLuint specular_id = glGetUniformLocation(program_id, "specular_color_4f");
      glUniform4f(specular_id, Ks[0], Ks[1], Ks[2], 1.0f);
      GLuint specular_coefficient_id = glGetUniformLocation(program_id, "specular_coefficient_1f");
      glUniform1f(specular_coefficient_id, Ns);

      // Set shader attribute variables
      vertex_id.push_back(glGetAttribLocation(program_id, "vertex_3f"));
      normal_id.push_back(glGetAttribLocation(program_id, "normal_3f"));

      delete[] verticeArray;
      delete[] normalArray;
    }
  }
}

// PRE: This is defined. vertex_buffer_id is a valid buffer binded to an
// an array of vertices. color_buffer_id is a valid buffer binded to an
// array of colors.
// POST: The model has been drawn to the screen. 
void Model::draw() {
  
  for (int i = 0; i < vertex_id.size(); i ++) {
    glEnableVertexAttribArray(vertex_id.at(i));
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id.at(i));
    glVertexAttribPointer(vertex_id.at(i), 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(normal_id.at(i));
    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_id.at(i));
    glVertexAttribPointer(normal_id.at(i), 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glDrawArrays(GL_TRIANGLES, 0, number_of_vertices / FLOATS_PER_VERTEX);
    //glDrawArrays(GL_TRIANGLES, 0, 18);
    glDisableVertexAttribArray(vertex_id.at(i));
    glDisableVertexAttribArray(normal_id.at(i));
  }
  

  // if (vertex_id.size() > 0 && normal_id.size() > 0) {
  //   glEnableVertexAttribArray(vertex_id);
  //   glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
  //   glVertexAttribPointer(vertex_id, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  //   glEnableVertexAttribArray(normal_id);
  //   glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_id);
  //   glVertexAttribPointer(normal_id, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  //   glDrawArrays(GL_TRIANGLES, 0, number_of_vertices / FLOATS_PER_VERTEX);
  //   glDisableVertexAttribArray(vertex_id);
  //   glDisableVertexAttribArray(normal_id);
  // }
}

// Deletes the buffers from memory
void Model::clear() {
  /*
  glDeleteBuffers(1, &vertex_buffer_id);
  glDeleteBuffers(1, &normal_buffer_id);
  vertex_buffer_id = 0;
  normal_buffer_id = 0;
  */
}
