#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include "imgui/imgui.h"

#include "glhelper.h"

GLuint load_gl_shader_from_buf(const char* buf, GLuint shader_type) {
    uint id = glCreateShader(shader_type);
    if (!id)
        return id;

    glShaderSource(id, 1, &buf, 0);
    glCompileShader(id);
    int res = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &res);
    if (res == GL_FALSE) {
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &res);
        char *error_buf = (char*)malloc(res);
        glGetShaderInfoLog(id, res, 0, error_buf);
        printf("Error compiling shader: %s\n", error_buf);
        free(error_buf);
        glDeleteShader(id);
        return 0;
    }

    return id;
}

GLuint load_gl_shader_from_file(const char* filename, GLuint shader_type) {
    FILE* fptr = fopen(filename, "r");
	if (!fptr)
        return 0;
	fseek(fptr, 0, SEEK_END);
	int fsz = ftell(fptr);
	rewind(fptr);
	char *buf = (char*)calloc(1, fsz * sizeof(char) + 1);
	fread(buf, 1, fsz, fptr);
	fclose(fptr);
    GLuint r = load_gl_shader_from_buf(buf, shader_type);
    free(buf);

    return r;
}

GLuint load_gl_program_from_files(const char* vert_filename, const char* frag_filename) {
    GLuint vid = load_gl_shader_from_file(vert_filename, GL_VERTEX_SHADER);
    GLuint fid = load_gl_shader_from_file(frag_filename, GL_FRAGMENT_SHADER);
    GLuint id = link_gl_program(vid, fid);
    glDeleteShader(vid);
    glDeleteShader(fid);
    return id;
}

GLuint link_gl_program(GLuint vertex_shader, GLuint fragment_shader) {
    GLuint id = glCreateProgram();
    glAttachShader(id, vertex_shader);
    glAttachShader(id, fragment_shader);
    glLinkProgram(id);
    glDetachShader(id, vertex_shader);
    glDetachShader(id, fragment_shader);

    int res = 0;
    glGetProgramiv(id, GL_LINK_STATUS, &res);
    if (res == GL_FALSE) {
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &res);
        char *error_buf = (char*)malloc(res);
        glGetProgramInfoLog(id, res, 0, error_buf);
        printf("Error linking program: %s\n", error_buf);
        free(error_buf);
        glDeleteShader(id);
        return 0;
    }

    return id;
}

GLuint create_vertex_buffer(float* buf, int size) {
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), buf, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return vbo;
}

GLuint create_index_buffer(unsigned* buf, int size) {
    GLuint ibo;
    glGenBuffers(1, &ibo);

    return ibo;
}

