#ifndef INCLUDED_FUNCTIONS
#define INCLUDED_FUNCTIONS

#define   GLFW_KEY_W   87
#define   GLFW_KEY_S   83
#define   GLFW_KEY_D   68
#define   GLFW_KEY_A   65

void process_input(float & x_rot, float & y_rot, float delta_rot, float & x_rot2, float & y_rot2, float delta_rot2, Light light);

void process_view(float x_rot, float y_rot);

#endif