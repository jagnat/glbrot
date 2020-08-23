#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glhelper.h"

// Global variables
SDL_Window *window;
SDL_GLContext glContext;

float view_verts[] = {
    -1, -1, 0,
    -1,  1, 0,
     1,  1, 0,
    -1, -1, 0,
     1,  1, 0,
     1, -1, 0
};
GLuint prog_id, vbo_id;

// Uniforms
int width = 1800;
int height = 1200;
float cr = -0.5;
float ci = 0.0;
float rad = 1.5;
int num_iters = 20;

void draw();

void init_render();
void do_debug_pane();
void move_coords_mouse_delta(int dx, int dy);
void zoom_scroll(int mousedelta);

int main(int argc, char **argv) {
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("glbrot",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            width,
            height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    glContext = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, glContext);

    init_render();

    prog_id = load_gl_program_from_files("vert.glsl", "frag.glsl");
    vbo_id = create_vertex_buffer(view_verts, 18);

    glClearColor(0.0, 0.0, 0.0, 0.0);

    bool running = true;
    SDL_Event event;
    bool dragging = false;

    while (true)
    {
        while (SDL_PollEvent(&event)) {

            ImGui_ImplSDL2_ProcessEvent(&event);

            switch(event.type)
            {
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            running = 0; break;
                    }
                    break;

                case SDL_MOUSEMOTION:
                    if (!ImGui::GetIO().WantCaptureMouse && dragging)
                    {
                        move_coords_mouse_delta(event.motion.xrel, event.motion.yrel);
                    }
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (!ImGui::GetIO().WantCaptureMouse && event.button.button == SDL_BUTTON_LEFT)
                    {
                        dragging = true;
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_LEFT)
                        dragging = false;
                    break;

                case SDL_MOUSEWHEEL:
                    if (!ImGui::GetIO().WantCaptureMouse)
                    {
                        zoom_scroll(event.wheel.y);
                    }
                    break;

                case SDL_WINDOWEVENT:
                    switch(event.window.event) {
                        case SDL_WINDOWEVENT_RESIZED:
                        case SDL_WINDOWEVENT_SIZE_CHANGED:
                            width = event.window.data1;
                            height = event.window.data2;
                            break;
                    }
                    break;

                case SDL_QUIT: running = 0; break;
            }
        }

        if (!running)
            break;

        do_debug_pane();
        draw();
        SDL_Delay(1);
    }

    return 0;
}

void draw() {
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(prog_id);
    glUniform2f(glGetUniformLocation(prog_id, "window"), (float)width, (float)height);
    glUniform2f(glGetUniformLocation(prog_id, "center"), cr, ci);
    glUniform1f(glGetUniformLocation(prog_id, "radius"), rad);
    glUniform1i(glGetUniformLocation(prog_id, "num_iters"), num_iters);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glUseProgram(0);
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    SDL_GL_SwapWindow(window);
}

void do_debug_pane() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();

    ImGui::SetNextWindowSize(ImVec2(360, 0), ImGuiCond_Once);
    ImGui::Begin("Mandelbrot config", nullptr, ImGuiWindowFlags_NoResize);
        ImGui::AlignTextToFramePadding();

        ImGui::Text("center re:");
        ImGui::SameLine(120);
        ImGui::SetNextItemWidth(-1);
        ImGui::DragFloat("##cr", &cr, 0.0001f);

        ImGui::Text("center im:");
        ImGui::SameLine(120);
        ImGui::SetNextItemWidth(-1);
        ImGui::DragFloat("##ci", &ci, 0.0001f);

        ImGui::Text("radius:");
        ImGui::SameLine(120);
        ImGui::SetNextItemWidth(-1);
        ImGui::DragFloat("##rad", &rad, 0.0001f);

        ImGui::Text("num iters:");
        ImGui::SameLine(120);
        ImGui::SetNextItemWidth(-1);
        ImGui::DragInt("##iter", &num_iters);

    ImGui::End();
}

void move_coords_mouse_delta(int dx, int dy) {
    float r = -(dx / (float)width) * (2 * rad);
    float i = (dy / (float)width) * (2 * rad);
    cr += r;
    ci += i;
}

void zoom_scroll(int del) {
    if (del < 0) // Zoom out
    {
        rad = rad + 0.00001 * ((float)width * 2 * rad);
    }
    else // Zoom in
    {
        rad = rad - 0.00001 * ((float)width * 2 * rad);
    }
}

void init_render() {
    glewInit();
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(window, glContext);
    ImGui_ImplOpenGL3_Init("#version 130");
}
