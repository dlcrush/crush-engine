//========================================================================
// This is a small test application for GLFW.
// The program opens a window (640x480),
// and renders a flat shaded sphere.
//========================================================================

#include <fstream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/glfw.h>

#define PI 3.141592653589793238463
#define BYTES_PER_FLOAT 4
#define FLOATS_PER_QUAD 18
#define FLOATS_PER_VERTEX 3


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

int main(void) {
    int width, height;          // Window width and height
    float ratio;                // Window aspect ratio
    float x_rot = 0.0f;         // X rotation scaling factor
    float y_rot = 0.0f;         // Y rotation scaling factor
    float delta_rot = 1.0f;     // Ammount rotation changes on key press

    GLuint vertex_shader_id;            // vertex shader handle
    GLuint fragment_shader_id;          // fragment shader handle
    const char *vertex_shader_source;   // vertex shader source code
    const char *fragment_shader_source; // fragment shader source code
    GLuint program_id;                  // shader program handle
    GLint result;                       // shader combile and link result
    GLuint diffuse_id, ambient_id;      // shander uniform variable handles
    GLuint specular_id, specular_coefficient_id;
    GLuint vertex_id, normal_id;        // shader attribute variable handles

    GLfloat vertices[] = {-0.6f, -0.4f, 0.0f, // vertex coordinates
    			  0.6f, -0.4f, 0.0f,
    			  0.0f, 0.6f, 0.0f};
    GLfloat normals[] = {0.0f, 0.0f, 1.0f,    // vertex normals
    			 0.0f, 0.0f, 1.0f,
    			 0.0f, 0.0f, 1.0f};
    int vertices_length = 9; // length of vertices array
    int buffer_size = 36;    // size of vertices array in bytes
    GLuint vertex_buffer_id; // vbo vertex handle
    GLuint normal_buffer_id; // vbo normal handle

    // Initialize GLFW
    if(!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }

    // Open a window and create its OpenGL context
    if(!glfwOpenWindow(640, 480, 0, 0, 0, 0, 16, 0, GLFW_WINDOW)) {
        fprintf(stderr, "Failed to open GLFW window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwSetWindowTitle("Sphere");

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

    // Set shader uniform variables
    ambient_id = glGetUniformLocation(program_id, "ambient_color_4f");
    glUniform4f(ambient_id, 0.0f, 0.4f, 0.2f, 1.0f);
    diffuse_id = glGetUniformLocation(program_id, "diffuse_color_4f");
    glUniform4f(diffuse_id, 0.0f, 0.8f, 0.4f, 1.0f);
    specular_id = glGetUniformLocation(program_id, "specular_color_4f");
    glUniform4f(specular_id, 1.0f, 1.0f, 1.0f, 1.0f);
    specular_coefficient_id = glGetUniformLocation(program_id, "specular_coefficient_1f");
    glUniform1f(specular_coefficient_id, 100.0f);


    // Set shader attribute variables
    vertex_id = glGetAttribLocation(program_id, "vertex_3f");
    normal_id = glGetAttribLocation(program_id, "normal_3f");

    // Create vertex buffer
    buffer_size = vertices_length * BYTES_PER_FLOAT;
    glGenBuffers(1, &vertex_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, buffer_size, vertices, GL_STATIC_DRAW);

    // Create normal buffer
    glGenBuffers(1, &normal_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_id);
    glBufferData(GL_ARRAY_BUFFER, buffer_size, normals, GL_STATIC_DRAW);

    // enable depth testing
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    do {
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

	// Draw sphere using vertex buffer object
	glEnableVertexAttribArray(vertex_id);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id);
	glVertexAttribPointer(vertex_id, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(normal_id);
	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer_id);
	glVertexAttribPointer(normal_id, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glDrawArrays(GL_TRIANGLES, 0, vertices_length / FLOATS_PER_VERTEX);
	glDisableVertexAttribArray(vertex_id);
	glDisableVertexAttribArray(normal_id);

        // Swap buffers
        glfwSwapBuffers();

    } // Check if the ESC key was pressed or the window was closed
    while(glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS &&
           glfwGetWindowParam(GLFW_OPENED));

    // Deallocate buffer memory
    glDeleteBuffers(1, &vertex_buffer_id);
    glDeleteBuffers(1, &normal_buffer_id);
    glDeleteProgram(program_id);
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
