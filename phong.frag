#version 120

uniform vec3 light_position;
uniform vec4 light_color;

uniform float attenuation_amount;
uniform vec4 ambient_color_4f; // material ambient reflectance color
uniform vec4 diffuse_color_4f; // meterial diffuse reflectance color
uniform vec4 specular_color_4f;// material specular reflectance color
uniform float specular_coefficient_1f; // specular coefficient

uniform sampler2D textureSampler;

void main() {
  gl_FragColor = gl_Color;
}
