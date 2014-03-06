#include "window.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <string>
#include <fstream>

using namespace std;

// Returns a c-string with the contents of the specified file.
// WARNING: This function does not check if the specified file exists
//          or is readable.
// WARNING: This function returns a c-string that must be deleted by
//          the calling function
char *read_file(const char *file_name) {
  int file_length;
  char *file_string;
  std::ifstream file_stream(file_name, std::ifstream::binary);
  file_stream.seekg(0, file_stream.end);
  file_length = file_stream.tellg();
  file_stream.seekg(0, file_stream.beg);
  file_string = new char[file_length + 1];
  file_stream.read(file_string, file_length);
  file_stream.close();
  file_string[file_length] = '\0';
  return file_string;
}


Window::Window(GLuint width, GLuint height, string windowTitle) {
	// Initialize GLFW
	if(!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(EXIT_FAILURE);
	}

	// Open a window and create its OpenGL context
	if(!glfwOpenWindow(width, height, 0, 0, 0, 0, 16, 0, GLFW_WINDOW)) {
		fprintf(stderr, "Failed to open GLFW window\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetWindowTitle(windowTitle.c_str());

	// Initialize GLEW
	if(glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize OpenGL Extensions\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Ensure we can capture the escape key being pressed below
	glfwEnable( GLFW_STICKY_KEYS );

	// Enable vertical sync (on cards that support it)
	glfwSwapInterval(1);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  	glEnable(GL_DEPTH_TEST);
  	glDepthFunc(GL_LESS);
}


void Window::test() {

}

void Window::process_input(float & x_rot, float & y_rot) {
	// Handle user input
	if (glfwGetKey(GLFW_KEY_UP) == GLFW_PRESS) {
		x_rot += delta_rot;
	}
	if (glfwGetKey(GLFW_KEY_DOWN) == GLFW_PRESS) {
		x_rot -= delta_rot;
	}
	if (glfwGetKey(GLFW_KEY_LEFT) == GLFW_PRESS) {
		y_rot += delta_rot;
	}
	if (glfwGetKey(GLFW_KEY_RIGHT) == GLFW_PRESS) {
		y_rot -= delta_rot;
	}
}

void Window::process_view() {
	int width, height;          // Window width and height
  	float ratio;                // Window aspect ratio

	// Get window size (may be different than the requested size)
	glfwGetWindowSize(&width, &height);

	// Set the rendering viewport location and dimenstions
	height = height > 0 ? height : 1;
	glViewport(0, 0, width, height);

	// Clear color buffer
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	// Select and setup the projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ratio = width / (float) height;
	glOrtho(-ratio, ratio, -1.0f, 1.0f, -1.0f, 1.0f);

	// Select and setup the modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Rotate view
	glRotatef(x_rot, 1.0f, 0.0f, 0.0f);
	glRotatef(y_rot, 0.0f, 1.0f, 0.0f);
}

void Window::set_up_shaders(string vertex_shader, string fragment_shader, GLuint & program_id) {
	GLuint vertex_shader_id;            // vertex shader handle
  	GLuint fragment_shader_id;          // fragment shader handle
  	const char *vertex_shader_source;   // vertex shader source code
  	const char *fragment_shader_source; // fragment shader source code
  	//GLuint program_id;                  // shader program handle
  	GLint result;                       // shader combile and link result

	// Load and compile vertex shader
	vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	vertex_shader_source = read_file("phong.vert");
	glShaderSource(vertex_shader_id, 1, &vertex_shader_source, NULL);
	delete[] vertex_shader_source;
	glCompileShader(vertex_shader_id);

	// Verify vertex shader compiled
	glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		char *log;
		int info_log_length;
		printf("Error compiling vertex shader\n");
		glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
		log = new char[info_log_length];
		glGetShaderInfoLog(vertex_shader_id, info_log_length, NULL, log);
		printf("%s\n", log);
		delete[] log;
	}

	// Load and compile fragment shader
	fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	fragment_shader_source = read_file("phong.frag");
	glShaderSource(fragment_shader_id, 1, &fragment_shader_source, NULL);
	delete[] fragment_shader_source;
	glCompileShader(fragment_shader_id);

	// Verify fragment shader compiled
	glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		char *log;
		int info_log_length;
		printf("Error compiling fragment shader\n");
		glGetShaderiv(fragment_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
		log = new char[info_log_length];
		glGetShaderInfoLog(fragment_shader_id, info_log_length, NULL, log);
		printf("%s\n", log);
		delete[] log;
	}

	// Link shaders
	program_id = glCreateProgram();

	glAttachShader(program_id, vertex_shader_id);
	glAttachShader(program_id, fragment_shader_id);
	glLinkProgram(program_id);

	// Verify shaders linked
	glGetProgramiv(program_id, GL_LINK_STATUS, &result);
	if (result == GL_FALSE) {
		char *log;
		int info_log_length;
		printf("Error linking shaders.\n");
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
		log = new char[info_log_length];
		glGetProgramInfoLog(program_id, info_log_length, NULL, log);
		printf("%s\n", log);
		delete[] log;
	}

	// Set shader
	glUseProgram(program_id);
}

