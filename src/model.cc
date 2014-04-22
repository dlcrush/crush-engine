#include "headers/model.h"
#include "headers/material.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include "headers/PPMReader.h"
using namespace std;

#define BYTES_PER_FLOAT 4
#define FLOATS_PER_VERTEX 3

// Updates the current minimum value if the current value
// is less than the current minimum value.
void updateMin(GLfloat & currMin, const GLfloat curr) {
  if (curr < currMin) {
    currMin = curr;
  }
}

// Updates the current maximum value if the current value
// is greater than the current maximum value.
void updateMax(GLfloat & currMax, const GLfloat curr) {
  if (curr > currMax) {
    currMax = curr;
  }
}

// Copies values in source vector to destination array
// NOTE: destination needs to be allocated space before function call
// NOTE: i should be a positive integer (or 0) and i < mapping.size()
void copyVectorToArray(vector<GLfloat> source, GLfloat * destination, 
  vector<int> mapping, int i) {
  if (source.size() == 0) {
    return;
  }

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
    destination[i - begin] = source.at(i);
  }
}

// Splits string into two parts. First part is face value.
// Second value is normal value.
void split(string input, string & face, string & normal, string & texture) {
  int index = input.find("/");
  if (index == string::npos) {
    face = input;
  }
  else {
    string subString = input.substr(index + 1, string::npos);
    int index2 = subString.find("/");
    face = input.substr(0, index);
    string test = input.substr(index + 1, string::npos);
    int index3 = subString.find("/");
    texture = test.substr(0, index3);
    normal = subString.substr(index2 + 1, string::npos);
  }
}

  GLfloat Model::get_min_x() {
    return minx;
  }

  GLfloat Model::get_min_y() {
    return miny;
  }

  GLfloat Model::get_min_z() {
    return minz;
  }

  GLfloat Model::get_max_x() {
    return maxx;
  }

  GLfloat Model::get_max_y() {
    return maxy;
  }

  GLfloat Model::get_max_z() {
    return maxz;
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
    }
    else if(type.compare("d") == 0 or type.compare("Tr") == 0) {
      GLfloat transparency;

      inputFile >> transparency;

      material.set_transparency(transparency);
    }
    else if(type.compare("map_Kd") == 0) {
      string fileName;

      inputFile >> fileName;

      PPMReader reader(fileName);
      int tex_size = 0;
      texture = new unsigned char[10];
      // GLfloat tex_width;
      // GLfloat tex_height;
      reader.read(texture, tex_size, tex_width, tex_height);
      material.set_tex_width(tex_width);
      material.set_tex_height(tex_height);
      material.set_has_texture(true);
      material.set_texture(texture);
    }
  }

  if (existingMtl) {
    materials.push_back(material);
  }
}

Model::Model(GLuint program_id) {
  this->program_id = program_id;
  ambient_id = glGetUniformLocation(program_id, 
      "ambient_color_4f");
  diffuse_id = glGetUniformLocation(program_id, 
    "diffuse_color_4f");
  specular_id = glGetUniformLocation(program_id, 
    "specular_color_4f");
  specular_coefficient_id = glGetUniformLocation(program_id, 
    "specular_coefficient_1f");
  texture_sampler_id = glGetUniformLocation(program_id, "textureSampler");

  vertex_id = glGetAttribLocation(program_id, "vertex_3f");
  normal_id = glGetAttribLocation(program_id, "normal_3f");
  tex_coord_id = glGetAttribLocation(program_id, "tex_coord_2f");

  model_view_projection_matrix_id = glGetUniformLocation(program_id, "model_view_projection_matrix4f");
  model_view_matrix_id = glGetUniformLocation(program_id, "model_view_matrix4f");
  normal_matrix_id = glGetUniformLocation(program_id, "normal_matrix4f");

  is_textured_id = glGetUniformLocation(program_id, "is_textured_b");
  is_shaded_id = glGetUniformLocation(program_id, "is_shaded_b");

  transparency_id = glGetUniformLocation(program_id, "transparency_coefficient_1f");

  minx = 0.0f;
  miny = 0.0f;
  minz = 0.0f;
  maxx = 0.0f;
  maxy = 0.0f;
  maxz = 0.0f;
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
  if (vn.size() > 0) {
    normals.push_back(vn.at((normal - 1) * 3));
    normals.push_back(vn.at((normal - 1) * 3 + 1));
    normals.push_back(vn.at((normal - 1) * 3 + 2));
  }
  if (vt.size() > 0) {
    textures.push_back(vt.at((texture - 1) * 2));
    textures.push_back(vt.at((texture - 1) * 2 + 1));
  }
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

      int tex_width;
      int tex_height;
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

  bool is_textured = false;
  bool is_shaded = false;

  if (vt.size() > 0) {
    is_textured = true;
    glUniform1i(is_textured_id, 1);
  }
  else {
    glUniform1i(is_textured_id, 0);
  }
  if (vn.size() > 0) {
    is_shaded = true;
    glUniform1i(is_shaded_id, 1);
  }
  else {
    glUniform1i(is_shaded_id, 0);
  }

  if (success) {
    number_of_vertices = vertices.size();
    int buffer_size = number_of_vertices * BYTES_PER_FLOAT;
    for (int i = 0; i < material_vertex_map.size(); i ++) {


      GLfloat * verticeArray = new GLfloat[vertices.size()];
      GLfloat * normalArray = new GLfloat[normals.size()];
      GLfloat * textureArray = new GLfloat[textures.size()];


      copyVectorToArray(vertices,verticeArray,material_vertex_map,i);
      copyVectorToArray(normals,normalArray,material_vertex_map,i);
      copyVectorToArray(textures,textureArray,material_vertex_map,i);


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

      GLuint temp_vertex_buffer_id, temp_normal_buffer_id, temp_tex_coord_buffer_id;
      glGenBuffers(1, &temp_vertex_buffer_id);
      vertex_buffer_id.push_back(temp_vertex_buffer_id);
      glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id.at(i));
      glBufferData(GL_ARRAY_BUFFER, size * BYTES_PER_FLOAT, 
          verticeArray, GL_STATIC_DRAW);

      glGenBuffers(1, &temp_normal_buffer_id);
      normal_buffer_id.push_back(temp_normal_buffer_id);
      glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_id.at(i));
      glBufferData(GL_ARRAY_BUFFER, size * BYTES_PER_FLOAT, 
          normalArray, GL_STATIC_DRAW);

      int texture_size = textures.size();

      glGenBuffers(1, &temp_tex_coord_buffer_id);
      tex_coord_buffer_id.push_back(temp_tex_coord_buffer_id);
      glBindBuffer(GL_ARRAY_BUFFER, tex_coord_buffer_id.at(i));
      if (1 == 1 || is_textured) {
        glBufferData(GL_ARRAY_BUFFER, texture_size * BYTES_PER_FLOAT, textureArray, GL_STATIC_DRAW);
      }
      // else {
      //   textureArray = new GLfloat[100];
      //   for (int i = 0; i < textureArray; i ++) {

      //   }
      // }
      
      

      delete[] verticeArray;
      delete[] normalArray;
      delete[] textureArray;

    }



    for (int i = 0; i < vertices.size() - 3; i += 3) {
      //cout << "in for loop" << endl;
      GLfloat currx = vertices.at(i);
      GLfloat curry = vertices.at(i + 1);
      GLfloat currz = vertices.at(i + 2);

      updateMin(minx, currx);
      updateMin(miny, curry);
      updateMin(minz, currz);
      updateMax(maxx, currx);
      updateMax(maxy, curry);
      updateMax(maxz, currz);
    }
    
    // cout << "minx: " << minx << endl;
    // cout << "maxx: " << maxx << endl;

    // cout << "miny: " << miny << endl;
    // cout << "maxy: " << maxy << endl;

    // cout << "minz: " << minz << endl;
    // cout << "maxz: " << maxz << endl;
  }
}

// PRE: This is defined. vertex_buffer_id is a valid buffer binded to an
// an array of vertices. color_buffer_id is a valid buffer binded to an
// array of colors.
// POST: The model has been drawn to the screen. 
void Model::draw(Matrix projection_matrix, Matrix view_matrix, Matrix model_matrix) {
  
  //cout << view_matrix << endl;

  glEnableVertexAttribArray(vertex_id);
  glEnableVertexAttribArray(normal_id);
  glEnableVertexAttribArray(tex_coord_id);


  for (int i = 0; i < materialIDs.size(); i ++) {
    int current_material_id = materialIDs.at(i);


    Material * material = &(materials.at(current_material_id));


    GLfloat * Ka = material->get_Ka();
    GLfloat * Kd = material->get_Kd();
    GLfloat * Ks = material->get_Ks();
    GLfloat Ns = material->get_Ns();
    bool has_texture = material->has_texture();
    if (has_texture) {

      glUniform1i(is_textured_id, 1);

      unsigned char * texture = material->get_texture();

      GLfloat tex_width = material->get_tex_width();
      GLfloat tex_height = material->get_tex_height();

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture_id);      

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

      glBindBuffer(GL_ARRAY_BUFFER, tex_coord_buffer_id.at(i));
      glVertexAttribPointer(tex_coord_id, 2, GL_FLOAT, GL_FALSE, 
        0, NULL);
    }
    else {
      glUniform1i(is_textured_id, 0);
    }


    glUniform4f(ambient_id, Ka[0], Ka[1], Ka[2], 1.0f);
    glUniform4f(diffuse_id, Kd[0], Kd[1], Kd[2], 1.0f);
    glUniform4f(specular_id, Ks[0], Ks[1], Ks[2], 1.0f);
    glUniform1f(specular_coefficient_id, Ns);
    glUniform1i(texture_sampler_id, 0);
    glUniform1f(transparency_id, material->get_transparency());


    Matrix model_view_matrix = view_matrix * model_matrix;
    Matrix model_view_projection_matrix = projection_matrix * model_view_matrix;
    Matrix normal_matrix = model_view_matrix.normalMatrix();


    glUniformMatrix4fv(model_view_projection_matrix_id, 1, GL_TRUE, model_view_projection_matrix.data());
    glUniformMatrix4fv(model_view_matrix_id, 1, GL_TRUE, model_view_matrix.data());
    glUniformMatrix4fv(normal_matrix_id, 1, GL_TRUE, normal_matrix.data());

    
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id.at(i));
    glVertexAttribPointer(vertex_id, 3, GL_FLOAT, GL_FALSE, 
      0, NULL);

    
    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_id.at(i));
    glVertexAttribPointer(normal_id, 3, GL_FLOAT, GL_FALSE, 
      0, NULL);



    glDrawArrays(GL_TRIANGLES, 0, sizes.at(i));

  }

  glDisableVertexAttribArray(vertex_id);
  glDisableVertexAttribArray(normal_id);
  glDisableVertexAttribArray(tex_coord_id);

}

// Deletes the buffers from memory
void Model::clear() {
  for (int i = 0; i < vertex_buffer_id.size(); i ++) {
    glDeleteBuffers(1, &vertex_buffer_id.at(i));
    glDeleteBuffers(1, &normal_buffer_id.at(i));
    glDeleteBuffers(1, &tex_coord_buffer_id.at(i));
  }

  if (vertex_buffer_id.size() > 0) {
    vertex_buffer_id.erase(vertex_buffer_id.begin());
  }

  if (normal_buffer_id.size() > 0) {
    normal_buffer_id.erase(normal_buffer_id.begin());
  }

  if (tex_coord_buffer_id.size() > 0) {
    tex_coord_buffer_id.erase(tex_coord_buffer_id.begin());
  }
}
