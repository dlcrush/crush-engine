#version 120

attribute vec3 vertex_3f; // vertex position
attribute vec3 normal_3f; // vertex normal

uniform vec4 ambient_color_4f; // material ambient reflectance color
uniform vec4 diffuse_color_4f; // meterial diffuse reflectance color
uniform vec4 specular_color_4f;// material specular reflectance color
uniform float specular_coefficient_1f; // specular coefficient

void main() {
  vec3 normal, light, vertex, view, reflection;
  float NdotL, NdotR;

  normal = normalize(gl_NormalMatrix * normal_3f); // normal vector
  light = normalize(vec3(1.0, 1.0, 1.0)); // light vector
  NdotL = max(dot(normal, light), 0.0); // diffuse intensity
  vertex = (gl_ModelViewMatrix * vec4(vertex_3f, 1)).xyz; // position, camera space
  view = normalize(vec3(0,0,0) - vertex); // view vector
  reflection = reflect(-light, normal); // reflection vector
  NdotR = max(dot(normal, reflection), 0.0); // specular intensity
  gl_FrontColor = (pow(NdotR, 100.0) * specular_color_4f) + (NdotL * diffuse_color_4f) + ambient_color_4f; // simplified phong reflectance
  gl_Position = gl_ModelViewProjectionMatrix * vec4(vertex_3f, 1); // position, world space
}
