#version 120

uniform vec3 light_position;
uniform vec4 light_color;

uniform float attenuation_amount;
uniform vec4 ambient_color_4f; // material ambient reflectance color
uniform vec4 diffuse_color_4f; // meterial diffuse reflectance color
uniform vec4 specular_color_4f;// material specular reflectance color
uniform float specular_coefficient_1f; // specular coefficient

uniform sampler2D textureSampler;

varying vec3 position; // vertex position, camera space
varying vec3 normal;   // vertex normal, camera space
varying vec3 view;     // view vector, camera space
varying vec3 light;    // light vector, camera space
varying vec2 texture;  // vertex texture coordinates

void main() {
  float NdotL, NdotR;      // diffuse and specular factors
  vec3 reflection;         // reflection vector
  vec4 texture_color;      // texture color
  vec4 ambient_intensity;  // ambient reflection intensity
  vec4 diffuse_intensity;  // diffuse reflection intensity
  vec4 specular_intensity; // specular reflection intensity

  NdotL = max(dot(normal, light), 0.0);
  reflection = reflect(-light, normal);
  NdotR = max(dot(normal, reflection), 0.0);

  texture_color = texture2D(textureSampler, texture);
  
  ambient_intensity = texture_color * ambient_color_4f * light_color;
  diffuse_intensity = NdotL * texture_color * diffuse_color_4f * light_color;
  specular_intensity = pow(NdotR, specular_coefficient_1f) * specular_color_4f * light_color;

  gl_FragColor = ambient_intensity + diffuse_intensity + specular_intensity;
}
