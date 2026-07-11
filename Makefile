CC = g++
CFLAGS = -g -Wall -Wno-deprecated -DGLFW_INCLUDE_NONE -Ilibs -Ilibs/glad/include
LIBS = -lglfw -lGL -lGLU -lglut -lm -lX11 -ldl -lpthread

DEMO_NAME = opengl-project
SRC_DIR = src/$(DEMO_NAME)
GLAD_SRC = libs/glad/src/glad.c
BUILD_DIR = build/x64/Release

all: $(BUILD_DIR)/$(DEMO_NAME)

$(BUILD_DIR)/$(DEMO_NAME): $(SRC_DIR)/main.cpp $(GLAD_SRC)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC_DIR)/main.cpp $(GLAD_SRC) -o $@ $(LIBS)

clean:
	-rm -rf build

.PHONY: all clean
