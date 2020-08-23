#pragma once

GLuint load_gl_shader_from_buf(const char* buf, GLuint shader_type);
GLuint load_gl_shader_from_file(const char* filename, GLuint shader_type);
GLuint load_gl_program_from_files(const char* vert_filename, const char* frag_filename);
GLuint link_gl_program(GLuint vertex_shader, GLuint fragment_shader);
GLuint create_vertex_buffer(float* buf, int size);
GLuint create_index_buffer(unsigned* buf, int size);

