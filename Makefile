CC        := gcc
CFLAGS    := -std=c11 -O2 -g -Wall -Wextra -Wpedantic -Iinclude
LDLIBS    := -lraylib -lopengl32 -lgdi32 -lwinmm -lm
CORE_LIBS := -lm

BUILD_DIR := build

# Source discovery
SRCS      := $(wildcard src/*.c) $(wildcard src/core/*.c) $(wildcard src/render/*.c)
OBJS      := $(patsubst src/%.c, $(BUILD_DIR)/%.o, $(SRCS))

CORE_SRCS := $(wildcard src/core/*.c)
CORE_OBJS := $(patsubst src/core/%.c, $(BUILD_DIR)/core/%.o, $(CORE_SRCS))

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

# Headless tests
TEST_SRCS := $(wildcard tests/*.c)
TEST_BINS := $(patsubst tests/%.c, $(BUILD_DIR)/tests/%.exe, $(TEST_SRCS))

$(BUILD_DIR)/tests/%.exe: tests/%.c $(CORE_OBJS) | $(BUILD_DIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< $(CORE_OBJS) -o $@ $(CORE_LIBS)

test: $(BUILD_DIR)/tests/test_kepler.exe
	./$(BUILD_DIR)/tests/test_kepler.exe

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

.PHONY: all run test 2d 3d clean
