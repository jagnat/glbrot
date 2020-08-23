
CPPS := glbrot.cpp glhelper.cpp imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_demo.cpp imgui/imgui_widgets.cpp imgui/imgui_impl_sdl.cpp imgui/imgui_impl_opengl3.cpp
SRCS := $(addprefix src/, $(CPPS))
OBJS := $(notdir $(CPPS:%.cpp=%.o))
CFLAGS := -g

glbrot: $(OBJS)
	clang++ $(OBJS) -L/usr/lib -lSDL2 -ldl -lGLEW -lGLU -lGL -lm -o glbrot

run: glbrot
	./glbrot

%.o: src/%.cpp
	clang++ $(CFLAGS) -c $< -o $@

%.o: src/imgui/%.cpp
	clang++ $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r glbrot *.o
