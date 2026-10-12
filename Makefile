CC        := gcc
CFLAGS    := -std=c11 -O2 -g -Wall -Wextra -Wpedantic -Iinclude
LDLIBS    := -lraylib -lopengl32 -lgdi32 -lwinmm -lm
CORE_LIBS := -lm

BUILD_DIR := build

# Source discovery
SRCS      := $(wildcard src/*.c) $(wildcard src/core/*.c) $(wildcard src/render/*.c)
OBJS      := $(patsubst src/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Default target
all: $(BUILD_DIR)/nbody.exe

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Object file pattern rule
$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Main executable linking
$(BUILD_DIR)/nbody.exe: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

# Run main simulation
run: all
	./$(BUILD_DIR)/nbody.exe

# Phase 0 sandbox targets
2d: $(BUILD_DIR)/2d.exe
	./$(BUILD_DIR)/2d.exe

$(BUILD_DIR)/2d.exe: sandbox/2d.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@ $(LDLIBS)

3d: $(BUILD_DIR)/3d.exe
	./$(BUILD_DIR)/3d.exe

$(BUILD_DIR)/3d.exe: sandbox/3d.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@ $(LDLIBS)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run 2d 3d clean
