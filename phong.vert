#version 120

attribute vec3 vertex_3f; // vertex position
attribute vec3 normal_3f; // vertex normal

uniform vec3 light_position;
uniform vec4 light_color;
uniform float attenuation_amount;
uniform vec4 ambient_color_4f; // material ambient reflectance color
uniform vec4 diffuse_color_4f; // meterial diffuse reflectance color
uniform vec4 specular_color_4f;// material specular reflectance color
uniform float specular_coefficient_1f; // specular coefficient

void main() {
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

  //light = normalize(vec3(5,5,5));

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

  light_color2 = light_color * (1/ (distance * attenuation_amount));

  // specular
  specular = specular_coefficient_1f * specular_color_4f * light_color2 * pow(NdotR, 100);

  // diffuse
  diffuse = diffuse_color_4f * light_color2 * NdotL;

  // ambient
  ambient = ambient_color_4f * light_color2;

  // simplified phong reflectance
  gl_FrontColor = specular + diffuse + ambient;

  // position, world space
  gl_Position = gl_ModelViewProjectionMatrix * vec4(vertex_3f, 1); 
}
