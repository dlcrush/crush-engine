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

// Copies values in source vector to destination array
// NOTE: destination needs to be allocated space before function call
// NOTE: i should be a positive integer (or 0) and i < mapping.size()
void copyVectorToArray(vector<GLfloat> source, GLfloat * destination, vector<int> mapping, int i) {
  if ((i + 1) < mapping.size()) {
      // We aren't at the last element in the source.
      if (i == 0) {
        // initial element in source, start at 0.
        copyVectorToArray(source, destination, mapping.at(i), mapping.at(i + 1) + 1);
      }
      else {
        // not initial element in source, increment starting position by 1
        copyVectorToArray(source, destination, mapping.at(i) + 1, mapping.at(i + 1) + 1);
      }
    }
    else {
      // We are at the last element in the source.
      if (i == 0) {
        // initial element in source, start at 0
        copyVectorToArray(source, destination, mapping.at(i), source.size());
      }
      else {
        // not initial element in source, increment starting position by 1
        copyVectorToArray(source, destination, mapping.at(i) + 1, source.size());
      }
    }
}

// Copies values in source vector to destination array
// NOTE: destination needs to be allocated space before function call
// NOTE: begin should be a positive integer (or 0) and end should be a positive integer
// NOTE: begin <= end
void copyVectorToArray(vector<GLfloat> source, GLfloat * destination, int begin, int end) {
  for (int i = begin; i < end; i ++) {
    //cout << "i: " << i << endl;
    //cout << "i - begin : " << i - begin << endl;
    destination[i - begin] = source.at(i);
  }
  //cout << endl;
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
    face = input.substr(0, index);
    normal = subString.substr(index2 + 1, string::npos);
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
}

// PRE:
// POST: vertex_buffer_id has been allocated space,
// color_buffer_id has been allocated space,
// number_of_vertices has been allocated space.
Model::~Model() {
  clear();
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

  clear();
  
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
          //cout << "vertices size: " << vertices.size() << endl;
          if (vertices.size() != 0) {
            material_vertex_map.push_back(vertices.size() - 1);
          }
          else {
            material_vertex_map.push_back(0);
          }
          
          //cout << "equal " << i << " " << material_vertex_map.at(material_vertex_map.size() - 1) << endl;
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

    for (int i = 0; i < material_vertex_map.size(); i ++) {

      GLfloat * verticeArray = new GLfloat[vertices.size()];
      GLfloat * normalArray = new GLfloat[normals.size()];

      copyVectorToArray(vertices, verticeArray, material_vertex_map, i);
      copyVectorToArray(normals, normalArray, material_vertex_map, i);

      // if ((i + 1) < material_vertex_map.size()) {
      //   if (i == 0) {
      //     copyVectorToArray(vertices, verticeArray, material_vertex_map.at(i), material_vertex_map.at(i + 1) + 1);
      //   }
      //   else {
      //     copyVectorToArray(vertices, verticeArray, material_vertex_map.at(i) + 1, material_vertex_map.at(i + 1) + 1);
      //   }
      // }
      // else {
      //   if (i == 0) {
      //     copyVectorToArray(vertices, verticeArray, material_vertex_map.at(i), vertices.size());
      //   }
      //   else {
      //     copyVectorToArray(vertices, verticeArray, material_vertex_map.at(i) + 1, vertices.size());
      //   }
      // }

      //GLfloat * normalArray = new GLfloat[normals.size()];
      // if ((i + 1) < material_vertex_map.size()) {
      //   if (i == 0) {
      //     copyVectorToArray(normals, normalArray, material_vertex_map.at(i), material_vertex_map.at(i + 1) + 1);
      //   }
      //   else {
      //     copyVectorToArray(normals, normalArray, material_vertex_map.at(i) + 1, material_vertex_map.at(i + 1) + 1);
      //   }
      //   //copyVectorToArray(normals, normalArray, material_vertex_map.at(i), material_vertex_map.at(i + 1) + 1);
      // }
      // else {
      //   copyVectorToArray(normals, normalArray, material_vertex_map.at(i) + 1, normals.size());
      //   //copyVectorToArray(normals, normalArray, material_vertex_map.at(i), normals.size());
      // }

      GLuint temp_vertex_buffer_id, temp_normal_buffer_id;
      glGenBuffers(1, &temp_vertex_buffer_id);
      vertex_buffer_id.push_back(temp_vertex_buffer_id);
      glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id.at(i));
      if ((i + 1) < material_vertex_map.size()) {
        //glBufferData(GL_ARRAY_BUFFER, (material_vertex_map.at(i + 1) - material_vertex_map.at(i)), verticeArray, GL_STATIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, 18 * 4, verticeArray, GL_STATIC_DRAW);
      }
      else {
        //glBufferData(GL_ARRAY_BUFFER, ((vertices.size()/3) - material_vertex_map.at(i)) * 12, verticeArray, GL_STATIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, 18 * 4, verticeArray, GL_STATIC_DRAW);
      }
      //glBufferData(GL_ARRAY_BUFFER, buffer_size, verticeArray, GL_STATIC_DRAW);

      glGenBuffers(1, &temp_normal_buffer_id);
      normal_buffer_id.push_back(temp_normal_buffer_id);
      glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_id.at(i));
      if ((i + 1) < material_vertex_map.size()) {
        //glBufferData(GL_ARRAY_BUFFER, material_vertex_map.at(i + 1) - material_vertex_map.at(i), normalArray, GL_STATIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, 18 * 4, normalArray, GL_STATIC_DRAW);
      }
      else {
        //glBufferData(GL_ARRAY_BUFFER, buffer_size - material_vertex_map.at(i), normalArray, GL_STATIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, 18 * 4, normalArray, GL_STATIC_DRAW);
      }

      // Set shader attribute variables
      vertex_id.push_back(glGetAttribLocation(program_id, "vertex_3f"));
      normal_id.push_back(glGetAttribLocation(program_id, "normal_3f"));

      delete[] verticeArray;
      delete[] normalArray;

      // cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;
      // for (int i = 0; i < vertex_id.size(); i ++) {
      //   cout << material_vertex_map.at(i) << endl;
      // }
      // cout << "&&&&&&&&&&&&&&&&&&&&&&&&&&&&" << endl;

      //cout << number_of_vertices << endl;

      // cout << vertex_id.size() << endl;
      // cout << normal_id.size() << endl;
      // cout << vertex_buffer_id.size() << endl;
      // cout << normal_buffer_id.size() << endl;

      // cout << "***************************" << endl;
      // for (int i = 0; i < vertex_buffer_id.size(); i ++) {
      //   cout << vertex_buffer_id.at(i) << endl;
      // }
      // cout << "****************************" << endl;
      // cout << "############################" << endl;
      // for (int i = 0; i < normal_buffer_id.size(); i ++) {
      //   cout << normal_buffer_id.at(i) << endl;
      // }
      // cout << "############################" << endl;
    }
  }
}

// PRE: This is defined. vertex_buffer_id is a valid buffer binded to an
// an array of vertices. color_buffer_id is a valid buffer binded to an
// array of colors.
// POST: The model has been drawn to the screen. 
void Model::draw(GLuint program_id) {
  
  for (int i = 0; i < vertex_id.size(); i ++) {

    int current_material_id = materialIDs.at(i);

    Material * material = &(materials.at(current_material_id));

    GLfloat * Ka = material->get_Ka();
    GLfloat * Kd = material->get_Kd();
    GLfloat * Ks = material->get_Ks();
    GLfloat Ns = material->get_Ns();

    GLuint attenuation_amount_id = glGetUniformLocation(program_id,
      "attenuation_amount");
    //glUniform1f(attenuation_amount_id, 1.0f);

    int size = 0;

    if (vertex_id.size() < 2) {
      size = number_of_vertices / FLOATS_PER_VERTEX;
    }
    else {
      if (i == 0) {
        size = material_vertex_map.at(i + 1) + 1;
      }
      else if ((i + 1) < vertex_id.size()) {
        size = material_vertex_map.at(i + 1) - material_vertex_map.at(i);
      }
      else {
        size = (number_of_vertices) - material_vertex_map.at(i) - 1;
      }
    }

    //size /= FLOATS_PER_VERTEX;

    //cout << size << endl;

    GLuint ambient_id = glGetUniformLocation(program_id, "ambient_color_4f");
    glUniform4f(ambient_id, Ka[0], Ka[1], Ka[2], 1.0f);
    GLuint diffuse_id = glGetUniformLocation(program_id, "diffuse_color_4f");
    glUniform4f(diffuse_id, Kd[0], Kd[1], Kd[2], 1.0f);
    GLuint specular_id = glGetUniformLocation(program_id, "specular_color_4f");
    glUniform4f(specular_id, Ks[0], Ks[1], Ks[2], 1.0f);
    GLuint specular_coefficient_id = glGetUniformLocation(program_id, "specular_coefficient_1f");
    glUniform1f(specular_coefficient_id, Ns);

    glEnableVertexAttribArray(vertex_id.at(i));
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id.at(i));
    glVertexAttribPointer(vertex_id.at(i), 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(normal_id.at(i));
    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_id.at(i));
    glVertexAttribPointer(normal_id.at(i), 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glDrawArrays(GL_TRIANGLES, 0, size/3);
    glDisableVertexAttribArray(vertex_id.at(i));
    glDisableVertexAttribArray(normal_id.at(i));
  }
}

// Deletes the buffers from memory
void Model::clear() {
  for (int i = 0; i < vertex_buffer_id.size(); i ++) {
    glDeleteBuffers(1, &vertex_buffer_id.at(i));
    glDeleteBuffers(1, &normal_buffer_id.at(i));
  }

  if (vertex_buffer_id.size() > 0) {
    vertex_buffer_id.erase(vertex_buffer_id.begin());
  }

  if (normal_buffer_id.size() > 0) {
    normal_buffer_id.erase(normal_buffer_id.begin());
  }
}
