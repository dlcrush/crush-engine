#ifndef INCLUDED_WINDOW
#define INCLUDED_WINDOW

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <string>

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

class Window {

public:
	// PRE: width is defined, height is defined, windowTitle is defined
	// POST: window has been created
	Window(GLuint width, GLuint height, string windowTitle) {
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

		glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
	}

	// sets up the shader programs
	// NOTE: vertex_shader should be the name of a valid vertex shader file
	// NOTE: fragment_shader should be the name of a valid fragment shader file
	void set_up_shaders(string vertex_shader, string fragment_shader, 
		GLuint & program_id) {

		GLuint vertex_shader_id;            // vertex shader handle
	  	GLuint fragment_shader_id;          // fragment shader handle
	  	const char *vertex_shader_source;   // vertex shader source code
	  	const char *fragment_shader_source; // fragment shader source code
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
			glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, 
				&info_log_length);
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
			glGetShaderiv(fragment_shader_id, GL_INFO_LOG_LENGTH, 
				&info_log_length);
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

};
#endif
