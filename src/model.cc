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
#include "PPMReader.h"
using namespace std;

#define BYTES_PER_FLOAT 4
#define FLOATS_PER_VERTEX 3

// Copies values in source vector to destination array
// NOTE: destination needs to be allocated space before function call
// NOTE: i should be a positive integer (or 0) and i < mapping.size()
void copyVectorToArray(vector<GLfloat> source, GLfloat * destination, 
  vector<int> mapping, int i) {
  if ((i + 1) < mapping.size()) {
      // We aren't at the last element in the source.
      if (i == 0) {
        // initial element in source, start at 0.
        copyVectorToArray(source, destination, mapping.at(i), 
          mapping.at(i + 1) + 1);
      }
      else {
        // not initial element in source, increment starting position 
        // by 1
        copyVectorToArray(source, destination, mapping.at(i) + 1, 
          mapping.at(i + 1) + 1);
      }
    }
    else {
      // We are at the last element in the source.
      if (i == 0) {
        // initial element in source, start at 0
        copyVectorToArray(source, destination, mapping.at(i), 
          source.size());
      }
      else {
        // not initial element in source, increment starting position 
        // by 1
        copyVectorToArray(source, destination, mapping.at(i) + 1, 
          source.size());
      }
    }
}

// Copies values in source vector to destination array
// NOTE: destination needs to be allocated space before function call
// NOTE: begin should be a positive integer (or 0) and end should be a
// positive integer
// NOTE: begin <= end
void copyVectorToArray(vector<GLfloat> source, GLfloat * destination, 
  int begin, int end) {
  for (int i = begin; i < end; i ++) {
    //cout << "i: " << i << endl;
    //cout << "i - begin : " << i - begin << endl;
    destination[i - begin] = source.at(i);
  }
  //cout << endl;
}

// Splits string into two parts. First part is face value.
// Second value is normal value.
void split(string input, string & face, string & normal, string & texture) {
  int index = input.find("/");
  if (index == string::npos) {
    face = input;
  }
  else {
    //string texture = "";
    string subString = input.substr(index + 1, string::npos);
    int index2 = subString.find("/");
    face = input.substr(0, index);
    string test = input.substr(index + 1, string::npos);
    int index3 = subString.find("/");
    texture = test.substr(0, index3);
    //cout << "texture: " << texture << endl;
    normal = subString.substr(index2 + 1, string::npos);
  }
}

// loads Material file
// PRE: mtlFile is defined, materials has been allocated space
// POST: materials contains all the materials from the mtlFile
void loadMtlFile(string mtlFile, vector<Material> & materials,
  unsigned char * & texture, int & tex_width, int & tex_height) {
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
    else if(type.compare("map_Kd") == 0) {
      //cout << "map_Kd" << endl;

      string fileName;

      inputFile >> fileName;

      //PPMReader reader(fileName);
      PPMReader test(fileName);
      int tex_size = 0;
      texture = new unsigned char[10];
      test.read(texture, tex_size, tex_width, tex_height);
      //cout << tex_size << endl;
      //texture = test.read();
      for (int i = 0; i < 10; i ++) {
        //cout << (int) texture[i] << endl;
      }
    }
  }

  if (existingMtl) {
    materials.push_back(material);
  }
}

Model::Model(GLuint program_id) {
  this->program_id = program_id;
  tex_width = 0;
  tex_height = 0;
  ambient_id = glGetUniformLocation(program_id, 
      "ambient_color_4f");
  diffuse_id = glGetUniformLocation(program_id, 
    "diffuse_color_4f");
  specular_id = glGetUniformLocation(program_id, 
    "specular_color_4f");
  specular_coefficient_id = glGetUniformLocation(program_id, 
    "specular_coefficient_1f");
  texture_sampler_id = glGetUniformLocation(program_id, "textureSampler");


  model_view_projection_matrix_id = glGetUniformLocation(program_id, "model_view_projection_matrix4f");
  model_view_matrix_id = glGetUniformLocation(program_id, "model_view_matrix4f");
  normal_matrix_id = glGetUniformLocation(program_id, "normal_matrix4f");
}

// PRE:
// POST: All glBuffers have been deleted.
Model::~Model() {
  clear();
}

// PRE: vertices has been defined, points has been defined,
// normals has been defined, vn has been defined, face is defined and 
// an integer, normal is defined and an integer.
// POST: the vertex for the given face has been added to the vertices
// vector.
void Model::addFaceVertex(vector<GLfloat> & vertices, 
  vector<GLfloat> points, vector<GLfloat> & normals, 
  vector<GLfloat> & vn, vector<GLfloat> & textures,
  vector<GLfloat> & vt, int face, int normal, 
  int texture) {

  vertices.push_back(points.at((face - 1) * 3));
  vertices.push_back(points.at((face - 1) * 3 + 1));
  vertices.push_back(points.at((face - 1) * 3 + 2));
  normals.push_back(vn.at((normal - 1) * 3));
  normals.push_back(vn.at((normal - 1) * 3 + 1));
  normals.push_back(vn.at((normal - 1) * 3 + 2));
  textures.push_back(vt.at((texture - 1) * 2));
  textures.push_back(vt.at((texture - 1) * 2 + 1));
}

void Model::readOBJFile(ifstream & inputFile, vector<GLfloat> &points, 
  vector<GLfloat> &vn, vector<GLfloat> &vt, vector<GLfloat> &vertices, 
  vector<GLfloat> &normals, vector<Material> &materials, 
  vector<GLfloat> &textures, unsigned char * & texture,
  vector<int> & materialIDs, vector<int> & material_vertex_map) {

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
    else if (type.compare("vt") == 0) {
      GLfloat vt1, vt2;

      inputFile >> vt1 >> vt2;

      vt.push_back(vt1);
      vt.push_back(vt2);
    }
    else if (type.compare("mtllib") == 0) {
      string mtlFile;

      inputFile >> mtlFile;

      loadMtlFile(mtlFile, materials, texture, tex_width, tex_height);
    }
    else if (type.compare("usemtl") == 0) {
      string mtlName;

      inputFile >> mtlName;

      for (int i = 0; i < materials.size(); i ++) {
        if (materials.at(i).get_name() == mtlName) {
          materialIDs.push_back(i);
          if (vertices.size() != 0) {
            material_vertex_map.push_back(vertices.size() - 1);
          }
          else {
            material_vertex_map.push_back(0);
          }
        }
      }
    }
    else if (type.compare("f") == 0) {
      string input1, input2, input3;
      string face1, face2, face3;
      string normal1, normal2, normal3;
      string texture1, texture2, texture3;
      inputFile >> input1 >> input2 >> input3;

      split(input1, face1, normal1, texture1);
      split(input2, face2, normal2, texture2);
      split(input3, face3, normal3, texture3);

      //cout << "texture1 = " << texture1 << endl;
      //cout << "texture2 = " << texture2 << endl;
      //cout << "texture3 = " << texture3 << endl;

      addFaceVertex(vertices, points, normals, vn,
        textures, vt,
        atoi(face1.c_str()), atof(normal1.c_str()),
        atof(texture1.c_str()));
      addFaceVertex(vertices, points, normals, vn,
        textures, vt,
        atoi(face2.c_str()), atof(normal2.c_str()),
        atof(texture2.c_str()));
      addFaceVertex(vertices, points, normals, vn,
        textures, vt,
        atoi(face3.c_str()), atof(normal3.c_str()),
        atof(texture3.c_str()));
    }
  }
}

// PRE: objFileName is defined and contains the filename of a valid 
// .obj file.
// POST: The vertices for the obj file have been loaded and binded to 
// vertex_buffer_id.
void Model::load(string objFileName) {
  vector<GLfloat> points;
  vector<GLfloat> vertices;
  vector<GLfloat> vn;
  vector<GLfloat> normals;
  vector<GLfloat> vt;
  vector<GLfloat> textures;
  unsigned char * texture;

  clear();
  
  /* input file stream for obj file */
  ifstream inputFile(objFileName.c_str());

  bool success = inputFile.good();

  int count = 0;

  readOBJFile(inputFile, points, vn, vt, vertices, normals, materials,
    textures, texture, materialIDs, material_vertex_map);

  for (int i = 0; i < vt.size(); i ++) {
    //cout << vt.at(i) << endl;
  }

  //cout << endl;

  for (int i = 0; i < textures.size(); i ++) {
    //cout << textures.at(i) << endl;
    if ((i + 1) % 2 == 0) {
      //cout << endl;
    }
  }

  if (success) {
    number_of_vertices = vertices.size();
    int buffer_size = number_of_vertices * BYTES_PER_FLOAT;
    for (int i = 0; i < material_vertex_map.size(); i ++) {

      GLfloat * verticeArray = new GLfloat[vertices.size()];
      GLfloat * normalArray = new GLfloat[normals.size()];

      copyVectorToArray(vertices,verticeArray,material_vertex_map,i);
      copyVectorToArray(normals,normalArray,material_vertex_map,i);

      int size = 0;

      if (material_vertex_map.size() < 2) {
        size = number_of_vertices;
      }
      else {
        if (i == 0) {
          size = material_vertex_map.at(i + 1) + 1;
        }
        else if ((i + 1) < material_vertex_map.size()) {
          size = material_vertex_map.at(i + 1) - 
          material_vertex_map.at(i);
        }
        else {
          size = (number_of_vertices) - material_vertex_map.at(i) - 1;
        }
      }

      sizes.push_back(size);

      GLuint temp_vertex_buffer_id, temp_normal_buffer_id;
      glGenBuffers(1, &temp_vertex_buffer_id);
      vertex_buffer_id.push_back(temp_vertex_buffer_id);
      glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id.at(i));
      if ((i + 1) < material_vertex_map.size()) {
        glBufferData(GL_ARRAY_BUFFER, size * BYTES_PER_FLOAT, 
          verticeArray, GL_STATIC_DRAW);
      }
      else {
        glBufferData(GL_ARRAY_BUFFER, size * BYTES_PER_FLOAT, 
          verticeArray, GL_STATIC_DRAW);
      }

      glGenBuffers(1, &temp_normal_buffer_id);
      normal_buffer_id.push_back(temp_normal_buffer_id);
      glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_id.at(i));
      if ((i + 1) < material_vertex_map.size()) {
        glBufferData(GL_ARRAY_BUFFER, size * BYTES_PER_FLOAT, 
          normalArray, GL_STATIC_DRAW);
      }
      else {
        glBufferData(GL_ARRAY_BUFFER, size * BYTES_PER_FLOAT, 
          normalArray, GL_STATIC_DRAW);
      }

      // Set shader attribute variables
      vertex_id.push_back(glGetAttribLocation(program_id, 
        "vertex_3f"));
      normal_id.push_back(glGetAttribLocation(program_id, 
        "normal_3f"));

      delete[] verticeArray;
      delete[] normalArray;
    }

    tex_coord_id = glGetAttribLocation(program_id, "tex_coord_2f");

    int count = 0;
    for (int i = 0; i < textures.size() - 1; i += 2) {
      count ++;
      cout << textures.at(i) << " " << textures.at(i + 1) << endl;
      if (count % 3 == 0) {
        cout << endl;
      }
    }

    GLfloat * textureArray = new GLfloat[textures.size()];
    copy(textures.begin(), textures.end(), textureArray);
    texture_size = textures.size();
    cout << texture_size << endl;

    // for (int i = 0; i < textures.size() - 1; i += 2) {
    //   count ++;
    //   cout << textureArray.at(i) << " " << textures.at(i + 1) << endl;
    //   if (count % 3 == 0) {
    //     cout << endl;
    //   }
    // }

    glGenBuffers(1, &tex_coord_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, tex_coord_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, texture_size * 4, textureArray, GL_STATIC_DRAW);

    for (int i = 0; i < 196605; i ++) {
      //cout << texture[i] << endl;
    }
    cout << tex_width << endl;
    cout << tex_height << endl;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  }
}

// PRE: This is defined. vertex_buffer_id is a valid buffer binded to an
// an array of vertices. color_buffer_id is a valid buffer binded to an
// array of colors.
// POST: The model has been drawn to the screen. 
void Model::draw(Matrix projection_matrix, Matrix view_matrix, Matrix model_matrix) {
  
  //glActiveTexture(GL_TEXTURE0);
  //glBindTexture(GL_TEXTURE_2D, texture_id);

  glEnableVertexAttribArray(tex_coord_id);
  glBindBuffer(GL_ARRAY_BUFFER, tex_coord_buffer_id);
  glVertexAttribPointer(tex_coord_id, 2, GL_FLOAT, GL_FALSE, 
    0, NULL);

  //cout << vertex_id.size() << endl;

  for (int i = 0; i < vertex_id.size(); i ++) {
    int current_material_id = materialIDs.at(i);

    Material * material = &(materials.at(current_material_id));

    GLfloat * Ka = material->get_Ka();
    GLfloat * Kd = material->get_Kd();
    GLfloat * Ks = material->get_Ks();
    GLfloat Ns = material->get_Ns();

    glUniform4f(ambient_id, Ka[0], Ka[1], Ka[2], 1.0f);
    glUniform4f(diffuse_id, Kd[0], Kd[1], Kd[2], 1.0f);
    glUniform4f(specular_id, Ks[0], Ks[1], Ks[2], 1.0f);
    glUniform1f(specular_coefficient_id, Ns);
    glUniform1i(texture_sampler_id, 0);

    Matrix model_view_matrix = view_matrix * model_matrix;
    Matrix model_view_projection_matrix = projection_matrix * model_view_matrix;
    Matrix normal_matrix = model_view_matrix.normalMatrix();

    glUniformMatrix4fv(model_view_projection_matrix_id, 1, GL_TRUE, model_view_projection_matrix.data());
    glUniformMatrix4fv(model_view_matrix_id, 1, GL_TRUE, model_view_matrix.data());
    glUniformMatrix4fv(normal_matrix_id, 1, GL_TRUE, normal_matrix.data());

    glEnableVertexAttribArray(vertex_id.at(i));
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id.at(i));
    glVertexAttribPointer(vertex_id.at(i), 3, GL_FLOAT, GL_FALSE, 
      0, NULL);

    glEnableVertexAttribArray(normal_id.at(i));
    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_id.at(i));
    glVertexAttribPointer(normal_id.at(i), 3, GL_FLOAT, GL_FALSE, 
      0, NULL);

    glDrawArrays(GL_TRIANGLES, 0, sizes.at(i));

    glDisableVertexAttribArray(vertex_id.at(i));
    glDisableVertexAttribArray(normal_id.at(i));
    
  }

  glDisableVertexAttribArray(tex_coord_id);
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
