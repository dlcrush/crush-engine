#version 120

uniform vec4 ambient_color_4f; // material ambient reflectance color
uniform vec4 diffuse_color_4f; // meterial diffuse reflectance color
uniform vec4 specular_color_4f;// material specular reflectance color
uniform float specular_coefficient_1f; // specular coefficient
uniform vec3 light_position_3f; // location of light
uniform vec4 light_color_4f;    // color of light
uniform float attenuation_factor_1f; // distance attenuation scale factor
uniform bool is_textured_b; // whether to apply texture map
uniform bool is_shaded_b;   // whether to apply light shading
uniform sampler2D textureSampler;

varying vec3 position; // vertex position, camera space
varying vec3 normal;   // vertex normal, camera space
varying vec3 view;     // view vector, camera space
varying vec3 light;    // light vector, camera space
varying vec2 texture;  // vertex texture coordinates

void main() {
  vec4 texture_color = vec4(0, 0, 0, 1);
  gl_FragColor = vec4(0, 0, 0, 1);
  if (is_textured_b) {
    texture_color = texture2D(textureSampler, texture);
    gl_FragColor = texture_color;
  }
  if (is_shaded_b) {
    float NdotL = max(dot(normal, light), 0.0);
    vec3 reflection = normalize(reflect(light, normal));
    float VdotR = max(dot(view, reflection), 0.0);
    float dist = length(light_position_3f - position);
    float attenuation = 1.0 / (1.0 + (attenuation_factor_1f * dist));
    vec4 ambient_intensity = texture_color * ambient_color_4f;
    vec4 diffuse_intensity = NdotL * texture_color * diffuse_color_4f;
    vec4 specular_intensity = pow(VdotR, specular_coefficient_1f) * specular_color_4f;
    vec4 intensity = ambient_intensity + diffuse_intensity + specular_intensity;
    gl_FragColor = attenuation * light_color_4f * intensity;
  }
}

// #version 120

// uniform vec4 ambient_color_4f; // material ambient reflectance color
// uniform vec4 diffuse_color_4f; // meterial diffuse reflectance color
// uniform vec4 specular_color_4f;// material specular reflectance color
// uniform float specular_coefficient_1f; // specular coefficient
// uniform vec3 light_position_3f; // location of light
// uniform vec4 light_color_4f;    // color of light
// uniform float attenuation_factor_1f; // distance attenuation scale factor
// uniform sampler2D textureSampler;

// varying vec3 position; // vertex position, camera space
// varying vec3 normal;   // vertex normal, camera space
// varying vec3 view;     // view vector, camera space
// varying vec3 light;    // light vector, camera space
// varying vec2 texture;  // vertex texture coordinates

// void main() {
//   float NdotL = max(dot(normal, light), 0.0);
//   vec3 reflection = normalize(reflect(light, normal));
//   float VdotR = max(dot(view, reflection), 0.0);
//   float dist = length(light_position_3f - position);
//   float attenuation = 1.0 / (1.0 + (attenuation_factor_1f * dist));

//   vec4 texture_color = texture2D(textureSampler, texture);
//   vec4 ambient_intensity = texture_color * ambient_color_4f;
//   vec4 diffuse_intensity = NdotL * texture_color * diffuse_color_4f;
//   vec4 specular_intensity = pow(VdotR, specular_coefficient_1f) * specular_color_4f;
//   vec4 intensity = ambient_intensity + diffuse_intensity + specular_intensity;

//   gl_FragColor = attenuation * light_color_4f * intensity;
//   //gl_FragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
//   //gl_FragColor = intensity * light_color_4f;
//   //gl_FragColor = texture_color;
// }

// #version 120

// uniform vec3 light_position;
// uniform vec4 light_color;

// uniform float attenuation_amount;
// uniform vec4 ambient_color_4f; // material ambient reflectance color
// uniform vec4 diffuse_color_4f; // meterial diffuse reflectance color
// uniform vec4 specular_color_4f;// material specular reflectance color
// uniform float specular_coefficient_1f; // specular coefficient

// uniform sampler2D textureSampler;

// varying vec3 position; // vertex position, camera space
// varying vec3 normal;   // vertex normal, camera space
// varying vec3 view;     // view vector, camera space
// varying vec3 light;    // light vector, camera space
// varying vec2 texture;  // vertex texture coordinates

// void main() {
//   float NdotL, NdotR;      // diffuse and specular factors
//   vec3 reflection;         // reflection vector
//   vec4 texture_color;      // texture color
//   vec4 ambient_intensity;  // ambient reflection intensity
//   vec4 diffuse_intensity;  // diffuse reflection intensity
//   vec4 specular_intensity; // specular reflection intensity

//   NdotL = max(dot(normal, light), 0.0);
//   reflection = reflect(-light, normal);
//   NdotR = max(dot(normal, reflection), 0.0);

//   texture_color = texture2D(textureSampler, texture);
  
//   ambient_intensity = texture_color * ambient_color_4f * light_color;
//   diffuse_intensity = NdotL * texture_color * diffuse_color_4f * light_color;
//   specular_intensity = pow(NdotR, specular_coefficient_1f) * specular_color_4f * light_color;

//   gl_FragColor = ambient_intensity + diffuse_intensity + specular_intensity;
// }
