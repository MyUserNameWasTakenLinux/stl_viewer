# Compiler and flags
CXX = g++
CFLAGS = -Wall -Wextra -O2 -g
LDFLAGS = -lGL -lGLU -lglfw -lX11 -lpthread -lXrandr -lXi -ldl

# Directories
INCLUDE_DIRS = -Iexternal/include -Isrc
SRC_DIR = src
BUILD_DIR = build

# Source and object files
SOURCES = $(SRC_DIR)/main.cpp $(SRC_DIR)/renderer.cpp $(SRC_DIR)/glad.c
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES)))
TARGET = STLViewer

# Rules
.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CFLAGS) $(INCLUDE_DIRS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
