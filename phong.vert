#version 120

attribute vec3 vertex_3f; // vertex position
attribute vec3 normal_3f; // vertex normal
attribute vec2 tex_coord_2f;

uniform vec3 light_position;
uniform mat4 model_view_projection_matrix4f; // MVP Matrix
uniform mat4 model_view_matrix4f;            // MV Matrix
uniform mat4 normal_matrix4f;                // normal Matrix

varying vec3 position; // vertex position, camera space
varying vec3 normal;   // vertex normal, camera space
varying vec3 view;     // view vector, camera space
varying vec3 light;    // light vector, camera space
varying vec2 texture;  // vertex texture coordinates

void main() {
  gl_Position = model_view_projection_matrix4f * vec4(vertex_3f, 1);
  //gl_Position = gl_ModelViewProjectionMatrix * vec4(vertex_3f, 1);
  position = (model_view_matrix4f * vec4(vertex_3f, 1)).xyz;
  //position = (gl_ModelViewMatrix * vec4(vertex_3f, 1)).xyz;
  normal = normalize((normal_matrix4f * vec4(normal_3f, 0)).xyz);
  //normal = normalize(gl_NormalMatrix * normal_3f);
  light = normalize(light_position - position);
  view = normalize(vec3(0,0,0) - position);
  texture = tex_coord_2f;

  // gl_Position = gl_ModelViewProjectionMatrix * vec4(vertex_3f, 1);
  // position = (gl_ModelViewMatrix * vec4(vertex_3f, 1)).xyz;
  // normal = normalize(gl_NormalMatrix * normal_3f);
  // light = normalize(light_position - position);
  // view = normalize(vec3(0,0,0) - position);
  // texture = tex_coord_2f;

  /*
  vec3 normal, light, vertex, view, reflection;
  float NdotL, NdotR;
  float distance;
  vec4 specular, diffuse, ambient;
  float delta_x, delta_y, delta_z;
  vec4 light_color2;

  // position, camera space
  vertex = (gl_ModelViewMatrix * vec4(vertex_3f, 1)).xyz;

  // normal vector
  normal = normalize(gl_NormalMatrix * normal_3f);

  // light vector
  light = normalize(light_position - vertex);

  // diffuse intensity
  NdotL = max(dot(normal, light), 0.0);

  // view vector
  view = normalize(vec3(0,0,0) - vertex);

  // reflection vector
  reflection = reflect(-light, normal);

  // specular intensity
  NdotR = max(dot(normal, reflection), 0.0);

  // Get difference in x,y,z from light to vector
  delta_x = light_position.x - vertex.x;
  delta_y = light_position.y - vertex.y;
  delta_z = light_position.z - vertex.z;

  // distance from light to vector
  distance = pow(delta_x,2) + pow(delta_y,2) + pow(delta_z,2);

  light_color2 = light_color * (1.0 / (1.0 + (distance * attenuation_amount)));

  // specular
  specular = specular_coefficient_1f * specular_color_4f * light_color2 * pow(NdotR, 100);

  // diffuse
  diffuse = diffuse_color_4f * light_color2 * NdotL;

  // ambient
  ambient = ambient_color_4f * light_color2;

  // simplified phong reflectance
  gl_FrontColor = specular + diffuse + ambient;

  // position, world space
  gl_Position = gl_ModelViewProjectionMatrix * vec4(vertex_3f, 1); */
}
